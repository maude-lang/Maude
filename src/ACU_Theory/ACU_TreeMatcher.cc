/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.

*/

//
//      Implementation for AC/ACU matcher that works on red-black trees.
//

inline int
ACU_LhsAutomaton::eliminateBoundVariables(Substitution& solution)
{
  nrUnboundVariables = 0;
  Term* identity = topSymbol->getIdentity();
  for (const TopVariable& i : topVariables)
    {
      DagNode* d = solution.value(i.index);
      if (d != 0)
	{
	  if (d->symbol() == topSymbol)
	    return UNDECIDED;  // variable was bound to a term with our top symbol - needs a more complicated analysis
	  if (identity == 0 || !(identity->equal(d)))  // we can ignore variables bound to our identity
	    {
	      ACU_SlowIter j;
	      if (current.getSize() == 0 || !(current.find(d, j)))
		return false;
	      int multiplicity = i.multiplicity;
	      if (j.getMultiplicity() < multiplicity)
		return false;
	      current.deleteMult(j, multiplicity);
	      matchedMultiplicity += multiplicity;
	    }
	}
      else
	++nrUnboundVariables;
    }
  return true;
}

local_inline bool
ACU_LhsAutomaton::eliminateGroundAliens()
{
  for (const GroundAlien& i : groundAliens)
    {
      ACU_SlowIter j;
      if (current.getSize() == 0 || !(current.find(i.term, j)))
	return false;
      int multiplicity = i.multiplicity;
      if (j.getMultiplicity() < multiplicity)
	return false;
      current.deleteMult(j, multiplicity);
      matchedMultiplicity += multiplicity;
    }
  return true;
}

local_inline bool
ACU_LhsAutomaton::eliminateGroundedOutAliens(Substitution& solution)
{
  for (const NonGroundAlien& i : groundedOutAliens)
    {
      Term* t = i.term;
      Assert(t != 0, "shouldn't be running on unstable terms");
      ACU_SlowIter j;
      if (current.getSize() != 0 && current.findFirstPotentialMatch(t, solution, j))
	{
	  LhsAutomaton* a = i.automaton;
	  DagNode* d = j.getDagNode();
	  for (;;)
	    {
	      Subproblem* sp;
	      if (a->match(d, solution, sp))
		{
		  Assert(sp == 0, "grounded out alien gave rise to subproblem!");
		  int multiplicity = i.multiplicity;
		  if (j.getMultiplicity() < multiplicity)
		    return false;
		  current.deleteMult(j, multiplicity);
		  matchedMultiplicity += multiplicity;
		  goto nextGroundedOutAlien;
		}
	      j.next();
	      if (!j.valid())
		break;
	      d = j.getDagNode();
	      if (t->partialCompare(solution, d) == Term::LESS)
		{
		  //
		  //	Since t is less then d, it will also be less than
		  //	all next nodes so we can quit now.
		  //
		  break;
		}
	    }
	}
      return false;
    nextGroundedOutAlien:
      ;
    }
  return true;
}

int
ACU_LhsAutomaton::greedyMatch(ACU_TreeDagNode* subject,
			      Substitution& solution,
			      ACU_ExtensionInfo* extensionInfo)
{
  local.copy(solution);  // greedy matching is speculative so make a copy
  scratch.copy(solution);  // keep a scratch copy as well
  FOR_EACH_CONST(i, Vector<NonGroundAlien>, nonGroundAliens)
    {
      Term* t = i->term;
      Assert(t != 0, "shouldn't be running on unstable terms");
      ACU_SlowIter j;
      if (current.getSize() != 0 && current.findFirstPotentialMatch(t, solution, j))
	{
	  int multiplicity = i->multiplicity;
	  LhsAutomaton* a = i->automaton;
	  DagNode* d = j.getDagNode();
	  for (;;)
	    {
	      if (j.getMultiplicity() >= multiplicity)
		{
		  Subproblem* sp;
		  if (a->match(d, scratch, sp))
		    {
		      if (sp != 0)
			{
			  //
			  //	On a hiding to nothing so pack up and go home.
			  //
			  delete sp;
			  return UNDECIDED;
			}
		      local.copy(scratch);  // preserve any new bindings
		      current.deleteMult(j, multiplicity);
		      matchedMultiplicity += multiplicity;
		      goto nextNonGroundAlien;
		    }
		  scratch.copy(local);  // restore scratch copy
		}
	      j.next();
	      if (!j.valid())
		break;
	      d = j.getDagNode();
	      if (t->partialCompare(solution, d) == Term::LESS)
		{
		  //
		  //	Since t is less then d, it will also be less than
		  //	all next nodes so we can quit now.
		  //
		  break;
		}
	    }
	}
      return ((i - nonGroundAliens.begin()) < nrIndependentAliens) ?
	false : UNDECIDED;
    nextNonGroundAlien:
      ;
    }
  if (greedyPureMatch(subject, local, extensionInfo))
    {
      solution.copy(local);
      return true;
    }
  //
  //	When the pure matching step fails we always treat it as
  //	UNDECIDED for safety.
  //
  return UNDECIDED;
}

local_inline bool
ACU_LhsAutomaton::tryToBindVariable(const TopVariable& tv,
				    Substitution& solution)
{
  //
  //	Try to assign one thing to variable.
  //
  int multiplicity = tv.multiplicity;
  if (multiplicity == 1)
    {
      //
      //	Choose first subject with low enough sort.
      //
      ACU_SlowIter j(current);
      do
	{
	  DagNode* d = j.getDagNode();
	  if (d->leq(tv.sort))
	    {
	      solution.bind(tv.index, d);
	      current.deleteMult(j, 1);
	      ++matchedMultiplicity;
	      return true;
	    }
	  j.next();
	}
      while (j.valid());
    }
  else
    {
      //
      //	Try first subject with high enough
      //	multiplicity and give up if sort too big.
      //
      ACU_SlowIter j;
      if (current.findGeqMult(multiplicity, j))
	{
	  DagNode* d = j.getDagNode();
	  if (d->leq(tv.sort))
	    {
	      solution.bind(tv.index, d);
	      current.deleteMult(j, multiplicity);
	      matchedMultiplicity += multiplicity;
	      return true;
	    }
	}
    }
  //
  //	If we weren't able to assign a subject we can still
  //	try the identity element.
  //
  if (tv.takeIdentity)
    {
      solution.bind(tv.index, topSymbol->getIdentityDag());
      return true;
    }
  return false;
}

DagNode*
ACU_LhsAutomaton::makeHighMultiplicityAssignment(int multiplicity,
						 Sort* sort,
						 ACU_Tree& tree)
{
  ACU_SlowIter i;
  if (!(tree.findGeqMult(multiplicity, i)))
    return 0;
  DagNode* d = i.getDagNode();
  int currentSortIndex = d->getSortIndex();
  if (!leq(currentSortIndex, sort))
    return 0;
  //
  //	We have a legal assignment; now try to find a "better" one.
  //
  int m = i.getMultiplicity();
  int a = m / multiplicity;
  Assert(a > 0, "multiplicity error");
  if (a > 1)
    {
      currentSortIndex = topSymbol->computeMultSortIndex(currentSortIndex,
							 currentSortIndex,
							 a - 1);
      if (!leq(currentSortIndex, sort))
	{
	  tree.deleteMult(i, multiplicity);
	  return d;  // quit trying to improve substitution
	}
    }
  //
  //	We build the details in the reusable matched vector.
  //
  matched.clear();
  do
    {
      matched.append(ACU_DagNode::Pair(d, a));
      tree.deleteMult(i, a * multiplicity);
      if (tree.getSize() == 0 ||
	  !(tree.findGeqMult(multiplicity, i)))
	break;
      d = i.getDagNode();
      m = i.getMultiplicity();
      a = m / multiplicity;
      Assert(a > 0, "multiplicity error");
      currentSortIndex = topSymbol->computeMultSortIndex(currentSortIndex,
							 d->getSortIndex(),
							 a);
    }
  while (leq(currentSortIndex, sort));
  //
  //	Now make the assignment.
  //
  int nrMatched = matched.length();
  if (nrMatched == 1 && matched[0].multiplicity == 1)
    return matched[0].dagNode;

  ACU_DagNode* d2 = new ACU_DagNode(topSymbol, nrMatched, ACU_DagNode::ASSIGNMENT);
  ArgVec<ACU_DagNode::Pair>::iterator dest = d2->argArray.begin();
  for (const ACU_Pair& i : matched)
    *dest++ = i;
  return d2;
}

local_inline bool
ACU_LhsAutomaton::tryToBindLastVariable(ACU_TreeDagNode* subject,
					const TopVariable& tv,
					Substitution& solution)
{
  int multiplicity = tv.multiplicity;
  if (multiplicity == 1)
    {
      if (current.getSize() == 1 && current.getMaxMult() == 1)
	{
	  //
	  //	Just one subject left so try to assign it.
	  //
	  DagNode* d = current.getSoleDagNode();
	  if (d->leq(tv.sort))
	    {
	      solution.bind(tv.index, d);
	      current.clear();  // no need to update matchedMultiplicity
	      return true;
	    }
	}
      else
	{
	  {
	    //
	    //	First see if we can give it everything.
	    //
	    ACU_TreeDagNode* t = new ACU_TreeDagNode(topSymbol, current);
	    int index = current.computeBaseSort(topSymbol);
	    if (leq(index, tv.sort))
	      {
		if (subject->isReduced() && topSymbol->sortConstraintFree())
		  {
		    t->setSortIndex(index);
		    t->setReduced();
		  }
		solution.bind(tv.index, t);
		current.clear();  // no need to update matchedMultiplicity
		return true;
	      }
	  }
	  if (matchAtTop && matchedMultiplicity >= 1)
	    {
	      //
	      //	Plan B: We must have extension so try assigning
	      //	just one subject.
	      //
	      ACU_SlowIter j(current);
	      do
		{
		  DagNode* d = j.getDagNode();
		  if (d->leq(tv.sort))
		    {
		      solution.bind(tv.index, d);
		      current.deleteMult(j, 1);
		      ++matchedMultiplicity;
		      return true;
		    }
		  j.next();
		}
	      while (j.valid());
	    }
	}
    }
  else
    {
      //
      //	Last unbound variable has multiplicity >= 2.
      //
      if (matchAtTop)
	{
	  DagNode* d = makeHighMultiplicityAssignment(multiplicity,
						      tv.sort,
						      current);
	  if (d != 0)
	    {
	      solution.bind(tv.index, d);
	      matchedMultiplicity = 2;  // wrong but good enough
	      return true;
	    }
	}
      else
	{
	  int size = current.getSize();
	  if (size == 1 && current.getSoleMultiplicity() == multiplicity)
	    {
	      DagNode* d = current.getSoleDagNode();
	      if (d->leq(tv.sort))
		{
		  solution.bind(tv.index, d);
		  current.clear();  // no need to update matchedMultiplicity
		  return true;
		}
	      return false;
	    }

	  ACU_DagNode* d =
	    new ACU_DagNode(topSymbol,current.getSize(), ACU_DagNode::ASSIGNMENT);
	  ArgVec<ACU_DagNode::Pair>::iterator dest = d->argArray.begin();
	  ACU_SlowIter i(current); 
	  do
	    {
	      int m = i.getMultiplicity();
	      if (m % multiplicity != 0)
		return false;
	      dest->dagNode = i.getDagNode();
	      dest->multiplicity = m / multiplicity;
	      ++dest;
	      i.next();
	    }
	  while (i.valid());
	  int index = d->argVecComputeBaseSort();
	  if (!leq(index, tv.sort))
	    return false;
	  if (subject->isReduced() && topSymbol->sortConstraintFree())
	    {
	      d->setSortIndex(index);
	      d->setReduced();
	    }
	  solution.bind(tv.index, d);
	  current.clear();  // no need to update matchedMultiplicity
	  return true;
	}
    }
  //
  //	Last hope: see if we can assign the identity.
  //
  if (matchAtTop && matchedMultiplicity >= 2 && tv.takeIdentity)
    {
      solution.bind(tv.index, topSymbol->getIdentityDag());
      return true;
    }
  return false;
}

bool
ACU_LhsAutomaton::greedyPureMatch(ACU_TreeDagNode* subject,
				  Substitution& solution,
				  ACU_ExtensionInfo* extensionInfo)
{
  //
  //	Greedy pure matching can fail for so many reasons
  //	in the red-black case, we don't bother trying to
  //	detect true failure: false always means UNDECIDED.
  //
  for (const TopVariable& i : topVariables)
    {
      if (solution.value(i.index) == 0)
	{
	  --nrUnboundVariables;
	  if (current.getSize() == 0)
	    {
	      if (!(i.takeIdentity))
		return false;
	      solution.bind(i.index, topSymbol->getIdentityDag());
	      if (nrUnboundVariables == 0)
		break;
	    }
	  else
	    {
	      if (nrUnboundVariables == 0)
		{
		  if (!tryToBindLastVariable(subject, i, solution))
		    return false;
		  break;
		}
	      else
		{
		  if (!tryToBindVariable(i, solution))
		    return false;
		}
	    }
	}
    }

  if (current.getSize() == 0)
    {
      //
      //	Everything matched; fill out empty extension if needed.
      //
      if (extensionInfo != 0)
	{
	  extensionInfo->setValidAfterMatch(true);
	  extensionInfo->setMatchedWhole(true);
	}
    }
  else
    {
      //
      //	Stuff left over; see if we can put it in the extension.
      //
      if (extensionInfo != 0 && matchedMultiplicity >= 2)
	{
	  extensionInfo->setValidAfterMatch(true);
	  extensionInfo->setMatchedWhole(false);
	  if (current.getSize() == 1 && current.getMaxMult() == 1)
	    extensionInfo->setUnmatched(current.getSoleDagNode());
	  else
	    extensionInfo->setUnmatched(new ACU_TreeDagNode(topSymbol, current));
	}
      else
	return false;
    }
  return true;
}

int
ACU_LhsAutomaton::fullVariableMatch(ACU_TreeDagNode* subject,
				    Substitution& solution,
				    Subproblem*& returnedSubproblem)
{
  //
  //	Everything has been matched except for two variables which should have multiplicity 1,
  //	and we need to generate all solutions.
  //	If one variable is an element variable and the other can take unbounded multiplicity, we
  //	create an ACU_TreeVariableSubproblem() to generate all matches efficiently using the
  //	stripper-collector technique on the red-black tree.
  //
  int stripperVarIndex = NONE;
  Sort* stripperSort = nullptr;
  int collectorVarIndex = NONE;
  Sort* collectorSort = nullptr;
  for (const TopVariable& tv : topVariables)
    {
      if (solution.value(tv.index) == nullptr)
	{
	  Assert(tv.multiplicity == 1, "multiplicity = " << tv.multiplicity);
	  if (tv.upperBound == 1)
	    {
	      Assert(!tv.takeIdentity, "stripper takes identity");
	      Assert(stripperVarIndex == NONE, "double stripper");
	      stripperVarIndex = tv.index;
	      stripperSort = tv.sort;
	    }
	  else
	    {
	      Assert(tv.upperBound == UNBOUNDED, "upperBound = " << tv.upperBound);
	      Assert(collectorVarIndex == NONE, "double collector");
	      collectorVarIndex = tv.index;
	      collectorSort = tv.sort;
	    }
	}
    }
  Assert(stripperVarIndex != NONE && collectorVarIndex != NONE, "didn't find stripper-collector variables");
  if (stripperVarIndex != NONE && collectorVarIndex != NONE)
    {
      returnedSubproblem = new ACU_TreeVariableSubproblem(subject,
							  current,
							  stripperVarIndex,
							  stripperSort,
							  collectorVarIndex,
							  collectorSort);
      return true;
    }
  return UNDECIDED;
}

int
ACU_LhsAutomaton::treeMatch(ACU_TreeDagNode* subject,
			    Substitution& solution,
			    Subproblem*& returnedSubproblem,
			    ACU_ExtensionInfo* extensionInfo)
{
  current = subject->getTree();  // deep copy
  if (current.getMaxMult() < maxPatternMultiplicity)
    return false;
  //
  //	Eliminate subpatterns that must match a specific subterm
  //	in the subject.
  //
  matchedMultiplicity = 0;  // so we know when we've matched at least 2 subjects
  int r = eliminateBoundVariables(solution);
  if (r != true)
    return r;
  if (!eliminateGroundAliens() || !eliminateGroundedOutAliens(solution))
    return false;
  if (extensionInfo == 0 && nrUnboundVariables == 1 && nonGroundAliens.empty())
    {
      //
      //	Forced lone variable case.
      //
      for (const TopVariable& i : topVariables)
	{
	  if (solution.value(i.index) == 0)
	    return forcedLoneVariableCase(subject, i, solution, returnedSubproblem);
	}
      CantHappen("didn't find unbound variable");
    }
  if (matchStrategy == FULL)
    {
      if (nrUnboundVariables == 2)
	{
	  Assert(extensionInfo == 0, "should not have extension");
	  Assert(nonGroundAliens.empty(), "non ground alien with two unbound variables");
	  return fullVariableMatch(subject, solution, returnedSubproblem);
	}
      //
      //	We're here because treeMatchOK was true, which implies:
      //	  We're not matching at the top
      //	  Expected nrUnboundVariables = 1
      //	  That one variable has unbounded sort and multiplicity 1
      //	  Number of NGAs = 1
      //	  That one NGA is stable and has multiplicity 1
      //	
      Assert(nrUnboundVariables <= 1, "nrUnboundVariables = " << nrUnboundVariables);
      if (nrUnboundVariables != 1)
	{
	  //
	  //	The variable we expected to be unbound and act as a collector
	  //	variable was bound after all. We could potentially be
	  //	smarter here but this is a very unlikely case.
	  //
	  return UNDECIDED;
	}
      if (current.getSize() == 0)
	{
	  //
	  //	Subject exhausted - we don't expect this to happen in the
	  //	red-black case where the subject is expected to be large.
	  //	Though we could handle this efficiently it might be tricky
	  //	to reach this code, even for test purposes so we don't bother.
	  //
	  return UNDECIDED;
	}
      if (current.getSize() == 1 && current.getMaxMult() == 1)
	{
	  //
	  //	Subject reduced to a single item; again it would be tricky
	  //	to reach this case, so we don't both with an efficient
	  //	implementation.
	  //
	  return UNDECIDED;
	}
      //
      //	The only way we can be here is if we have a nonground alien
      //	and a collector variable, and no extension.
      //
      Assert(nonGroundAliens.length() == 1,
	     "wrong number of nonGroundAliens" << nonGroundAliens.length());
      Assert(extensionInfo == 0, "should not have extension");

      for (const TopVariable& i : topVariables)
	{
	  if (solution.value(i.index) == 0)
	    {
	      Assert(i.multiplicity == 1, "collector multiplicity = " << i.multiplicity);
	      returnedSubproblem =
		new ACU_LazySubproblem(subject,
				       current,
				       solution,
				       nonGroundAliens[0].automaton,
				       nonGroundAliens[0].term,
				       i.index,
				       i.sort);
	      return true;
	    }
	}
      CantHappen("didn't find unbound variable");
    }
  //
  //	Match everything else using greedy algorithms.
  //
  return greedyMatch(subject, solution, extensionInfo);
}

bool
ACU_LhsAutomaton::forcedLoneVariableCase(ACU_TreeDagNode* subject,
					 const TopVariable& tv,
					 Substitution& solution,
					 Subproblem*& returnedSubproblem)
{
  if (current.getSize() == 0)
    {
      //
      //	Special case: assign identity.
      //
      if (tv.takeIdentity)
	{
	  solution.bind(tv.index, topSymbol->getIdentityDag());
	  return true;
	}
      return false;
    }

  int multiplicity = tv.multiplicity;
  if (current.getSize() == 1 && current.getSoleMultiplicity() == multiplicity)
    {
      //
      //	Special case: assign one subject.
      //
      DagNode* d = current.getSoleDagNode();
      if (d->leq(tv.sort))
	{
	  solution.bind(tv.index, d);
	  return true;
	}
      return false;
    }
  //
  //	General case: need to assign everything.
  //
  ACU_BaseDagNode* b;
  if (multiplicity == 1)
    b = new ACU_TreeDagNode(topSymbol, current);
  else
    {
      ACU_DagNode* d = new ACU_DagNode(topSymbol, current.getSize(), ACU_DagNode::ASSIGNMENT);
      ArgVec<ACU_DagNode::Pair>::iterator dest = d->argArray.begin();
      ACU_SlowIter i(current); 
      do
	{
	  int m = i.getMultiplicity();
	  if (m % multiplicity != 0)
	    return false;
	  dest->dagNode = i.getDagNode();
	  dest->multiplicity = m / multiplicity;
	  ++dest;
	  i.next();
	}
      while (i.valid());
      b = d;
    }
  if (b->checkSort(tv.sort, returnedSubproblem))
    {
      solution.bind(tv.index, b);
      if (subject->isReduced() && b->getSortIndex() != Sort::SORT_UNKNOWN)
	b->setReduced();
      return true;
    }
  return false;
}
