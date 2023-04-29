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

local_inline int
ACU_LhsAutomaton::eliminateBoundVariables(Substitution& solution)
{
  nrUnboundVariables = 0;
  Term* identity = topSymbol->getIdentity();
  for (const TopVariable& i : topVariables)
    {
      if (DagNode* d = solution.value(i.index))
	{
	  if (d->symbol() == topSymbol)
	    {
	      //
	      //	Variable was bound to a term with our top symbol.
	      //	This needs more complicated handling that is unlikely to be efficient with a red-black tree.
	      //
	      return UNDECIDED;
	    }
	  if (identity == nullptr || !(identity->equal(d)))  // we can ignore variables bound to our identity
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
      Assert(t != nullptr, "shouldn't be running on unstable terms");
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
		  Assert(sp == nullptr, "grounded out alien gave rise to subproblem!");
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
ACU_LhsAutomaton::treeMatch(ACU_TreeDagNode* subject,
			    Substitution& solution,
			    Subproblem*& returnedSubproblem,
			    ACU_ExtensionInfo* extensionInfo)
{
  Assert(returnedSubproblem == nullptr, "returnedSubproblem not cleared by caller");
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
  //
  //	Pick off special cases.
  //
  if (extensionInfo == nullptr && nonGroundAliens.empty())
    {
      if (nrUnboundVariables == 0)
	{
	  //
	  //	Everything in the pattern has had a forced match and no extension.
	  //	We succeed iff there are no subject arguments left.
	  //
	  return current.getSize() == 0;
	}
      if (nrUnboundVariables == 1)
	{
	  //
	  //	Forced lone variable case.
	  //
	  for (const TopVariable& i : topVariables)
	    {
	      if (solution.value(i.index) == nullptr)
		return forcedLoneVariableCase(subject, i, solution, returnedSubproblem);
	    }
	  CantHappen("didn't find unbound variable");
	}
    }
  if (matchStrategy == FULL)
    {
      //
      //	We're here because treeMatchOK was true, which implies:
      //	  We're not matching at the top
      //	  No abstraction variable arguments in pattern
      //	  Collector variable exists (unbounded, multiplicity = 1, expected unbound)
      //	  And either
      //	    Stripper variable exists (bound = 1, multiplicity = 1, cannot take identity, expected unbound)
      //	  or
      //	    NGA exists (stable, multiplicity = 1)
      //	  and nothing else left unmatched in the pattern.
      //	Of course it is always possible that the collector and/or stripper variable
      //	has already be bound.
      //
      Assert(extensionInfo == nullptr, "should not have extension");
      if (nonGroundAliens.empty())
	{
	  //
	  //	First case. The possibility that one or both variables have been bound was caught above.
	  //
	  Assert(nrUnboundVariables == 2, "nrUnboundVariables = " << nrUnboundVariables);
	  Assert(solution.value(topVariables[stripperIndex].index) == nullptr, "stripper variable bound");
	  Assert(solution.value(topVariables[collectorIndex].index) == nullptr, "collector variable bound - variable case");
	  if (current.getSize() == 0)
	    return false;  // no subject arguments left; stripper variable cannot take identity so no match is possible
	  if (current.getSize() == 1 && current.getMaxMult() == 1)
	    return loneSubjectVariableCase(solution);  // one subject argument left; both variables unbound
	  //
	  //	We have 2 or more subject arguments, and unbound stripper and collector variables.
	  //	We return a subproblem that will try the stripper variable against each distinct subject
	  //	arugment and put the remaining subject arguments in the collector variable.
	  //
	  returnedSubproblem = new ACU_TreeVariableSubproblem(subject,
							      current,
							      topVariables[stripperIndex].index,
							      topVariables[stripperIndex].sort,
							      topVariables[collectorIndex].index,
							      topVariables[collectorIndex].sort);
	}
      else
	{
	  //
	  //	Second case.
	  //
	  Assert(nonGroundAliens.size() == 1, "nonGroundAliens.size() = " << nonGroundAliens.size());
	  Assert(nrUnboundVariables <= 1, "nrUnboundVariables = " << nrUnboundVariables);
	  if (current.getSize() == 0)
	    return false;  // no subjects arguments left; NGA cannot match identity otherwise it would have been abstracted
	  if (current.getSize() == 1 && current.getMaxMult() == 1)
	    return loneSubjectNGA_Case(solution, returnedSubproblem);  // one subject left; collector variable could be bound
	  if (nrUnboundVariables == 0)
	    return false;  // >= 2 subject arguments left; NGA can only match 1 otherwise it would have been abstracted
	  Assert(solution.value(topVariables[collectorIndex].index) == nullptr, "collector variable bound - NGA case");
	  //
	  //	We have 2 or more subject arguments, an NGA and an unbound collector variable.
	  //	We return a subproblem that will lazily match the NGA against each distinct subject argument that it could
	  //	match and put the remaining subject arguments in the collector variable on success.
	  //
	  returnedSubproblem = new ACU_LazySubproblem(subject,
						      current,
						      solution,
						      nonGroundAliens[0].automaton,
						      nonGroundAliens[0].term,
						      topVariables[collectorIndex].index,
						      topVariables[collectorIndex].sort);
	}
      return true;
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
  //
  //	We don't make any assumptions about the variable other than that it is not an abstraction
  //	variable (which would have disabled tree matching).
  //
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
      //	The sort of the subject will already be known.
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
  //	General case: need to assign everything, and compute the sort.
  //
  ACU_BaseDagNode* b;
  if (multiplicity == 1)
    b = new ACU_TreeDagNode(topSymbol, current);
  else
    {
      //
      //	High multiplicity - must divide all of the subject multiplicities.
      //
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
  //
  //	May need to deal with sort constraints.
  //
  if (b->checkSort(tv.sort, returnedSubproblem))
    {
      solution.bind(tv.index, b);
      //
      //	If the subject was reduced, and doing the sort check fill out the sort information,
      //	we can mark the binding as reduced.
      //
      if (subject->isReduced() && b->getSortIndex() != Sort::SORT_UNKNOWN)
	b->setReduced();
      return true;
    }
  return false;
}

bool
ACU_LhsAutomaton::loneSubjectVariableCase(Substitution& solution)
{
  //
  //	We have a single subject argument and stripper-collector variables with no extension.
  //	Since the stripper cannot take identity, we must see if the collector can.
  //
  if (!(topVariables[collectorIndex].takeIdentity))
    return false;
  //
  //	Now can the stripper variable take the lone subject argument?
  //	Its sort must already have been computed.
  //
  DagNode* d = current.getSoleDagNode();
  if (!(d->leq(topVariables[stripperIndex].sort)))
    return false;
  //
  //	Success.
  //
  solution.bind(topVariables[stripperIndex].index, d);
  solution.bind(topVariables[collectorIndex].index, topSymbol->getIdentityDag());
  return true;
}

bool
ACU_LhsAutomaton::loneSubjectNGA_Case(Substitution& solution, Subproblem*& returnedSubproblem)
{
  //
  //	We have a single subject argument, and an NGA and a collector variable in the pattern
  //	with no extension. The collector variable might be bound or unbound.
  //	Since the NGA cannot match identity, either the collector must be bound or it must
  //	be able to take identity.
  //
  const TopVariable& tv = topVariables[collectorIndex];
  if (solution.value(tv.index) == nullptr)
    {
      //
      //	If the collector variable isn't bound we bind it before matching the NGA
      //	which might contain the collector variable and bind it to something other
      //	than the identity.
      //
      if (!tv.takeIdentity)
	return false;
      solution.bind(tv.index, topSymbol->getIdentityDag());
    }
  //
  //	Now match the NGA against the remaining subject argument.
  //
  return nonGroundAliens[0].automaton->match(current.getSoleDagNode(), solution, returnedSubproblem);
}
