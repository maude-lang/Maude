/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for main AC/ACU matcher.
//

local_inline bool
ACU_LhsAutomaton::multiplicityChecks(ACU_DagNode* subject)
{
  //
  //	Copy argument multiplicities and check for trivial failure.
  //
  const ArgVec<ACU_DagNode::Pair>::const_iterator e = subject->argArray.end();
  if (maxPatternMultiplicity > 1)
    {
      //
      //	Because failure here is common we check this first.
      //
      for (ArgVec<ACU_DagNode::Pair>::const_iterator i = subject->argArray.begin();
	   i != e; ++i)
	{
	  if (i->multiplicity >= maxPatternMultiplicity)
	    goto okay;
	}
      return false;
    }
okay:
  currentMultiplicity.resize(subject->argArray.length());
  Vector<int>::iterator cm = currentMultiplicity.begin();
  int totalSubjectMultiplicity = 0;
  for (ArgVec<ACU_DagNode::Pair>::const_iterator i = subject->argArray.begin();
       i != e; ++i, ++cm)
    {
      int m = i->multiplicity;
      *cm = m;
      totalSubjectMultiplicity += m;
    }
  if (totalSubjectMultiplicity < totalLowerBound ||
      totalSubjectMultiplicity > totalUpperBound)
    return false;
  totalMultiplicity = totalSubjectMultiplicity;
  return true;
}

local_inline bool
ACU_LhsAutomaton::eliminateGroundAliens(ACU_DagNode* subject)
{
  FOR_EACH_CONST(i, Vector<GroundAlien>, groundAliens)
    {
      int pos;
      if (!(subject->binarySearch(i->term , pos)) ||
	  (currentMultiplicity[pos] -= i->multiplicity) < 0)
	return false;
    }
  return true;
}

local_inline bool
ACU_LhsAutomaton::eliminateBoundVariables(ACU_DagNode* subject,
					  Substitution& solution)
{
  int nrTopVariables = topVariables.length();
  lastUnboundVariable = NONE;
  for (int i = 0; i < nrTopVariables; i++)
    {
      DagNode* d = solution.value(topVariables[i].index);
      if (d != 0)
	{
	  if (!(subject->eliminateSubject(d,
					  topVariables[i].multiplicity,
					  currentMultiplicity)))
	    return false;
	}
      else
	{
	  //
	  //	Make linked list of unbound variables.
	  //
	  topVariables[i].prevUnbound = lastUnboundVariable;
	  lastUnboundVariable = i;
	}
    }
  return true;
}

local_inline bool
ACU_LhsAutomaton::eliminateGroundedOutAliens(ACU_DagNode* subject,
					     Substitution& solution)
{
  ArgVec<ACU_DagNode::Pair>& args = subject->argArray;
  int nrArgs = args.length();
  FOR_EACH_CONST(i, Vector<NonGroundAlien>, groundedOutAliens)
    {
      Term* t = i->term;
      int j = (t == 0) ? 0 : subject->findFirstPotentialMatch(t, solution);
      if (j < nrArgs)
	{
	  LhsAutomaton* a = i->automaton;
	  DagNode* d = args[j].dagNode;
	  do
	    {
	      Subproblem* sp;
	      if (a->match(d, solution, sp))
		{
		  Assert(sp == 0, "grounded out alien gave rise to subproblem!");
		  if ((currentMultiplicity[j] -= i->multiplicity) < 0)
		    return false;
		  goto nextGroundedOutAlien;
		}
	      ++j;
	      if (j == nrArgs)
		break;
	      d = args[j].dagNode;
	    }
	  while (t == 0 || t->partialCompare(solution, d) != Term::GREATER);
	}
      return false;
    nextGroundedOutAlien:
      ;
    }
  return true;
}

bool
ACU_LhsAutomaton::forcedLoneVariableCase(ACU_DagNode* subject,
					 Substitution& solution,
					 Subproblem*& returnedSubproblem)
{
  TopVariable& loneVariable = topVariables[lastUnboundVariable];
  int m = loneVariable.multiplicity;
  //
  //	Locate remaining subterms.
  //
  int nrArgs = currentMultiplicity.length();
  int nrSubterms = 0;
  int lastSubterm = UNDEFINED;
  for (int i = 0; i < nrArgs; i++)
    {
      int cm = currentMultiplicity[i];
      if (cm > 0)
	{
	  if (cm % m > 0)
	    return false;
	  ++nrSubterms;
	  lastSubterm = i;
	}
    }
  //
  //	Build assignment for lone variable.
  //
  if (nrSubterms == 0)
    {
      //
      //	Special case: assign identity.
      //
      if (loneVariable.takeIdentity)
	{
	  DagNode* d = topSymbol->getIdentityDag();
	  solution.bind(loneVariable.index, d);
	  if (loneVariable.abstracted != 0)
	    return loneVariable.abstracted->match(d, solution, returnedSubproblem);
	  return true;
	}
      return false;
    }
  ArgVec<ACU_DagNode::Pair>& args = subject->argArray;
  if (nrSubterms == 1 && currentMultiplicity[lastSubterm] == m)
    {
      //
      //	Special case: assign one subject.
      //
      DagNode* d = args[lastSubterm].dagNode;
      solution.bind(loneVariable.index, d);
      if (loneVariable.abstracted != 0)
	return loneVariable.abstracted->match(d, solution, returnedSubproblem);
      return d->leq(loneVariable.sort);
    }
  //
  //	General case: need to assign everything.
  //
  ACU_DagNode* d = new ACU_DagNode(topSymbol, nrSubterms, ACU_DagNode::ASSIGNMENT);
  int pos = 0;
  for (int i = 0; i < nrArgs; i++)
    {
      int cm = currentMultiplicity[i];
      if (cm > 0)
	{
	  d->argArray[pos].dagNode = args[i].dagNode;
	  d->argArray[pos].multiplicity = cm / m;
	  ++pos;
	}
    }
  Assert(pos == nrSubterms, "inconsistant number of subterms");
  solution.bind(loneVariable.index, d);
  if (loneVariable.abstracted != 0)
    return loneVariable.abstracted->match(d, solution, returnedSubproblem);
  if (d->checkSort(loneVariable.sort, returnedSubproblem))
    {
      if (subject->isReduced() && d->getSortIndex() != Sort::SORT_UNKNOWN)
	d->setReduced();  // not worth checking if variable is useful
      return true;
    }
  return false;
}

local_inline bool 
ACU_LhsAutomaton::aliensOnlyMatch(ACU_DagNode* subject,
				  Substitution& solution,
				  Subproblem*& returnedSubproblem)
{
  ArgVec<ACU_DagNode::Pair>& args = subject->argArray;
  int nrArgs = args.length();
  SubproblemAccumulator subproblems;
  if (nrIndependentAliens > 0)
    {
      //
      //	Anything matched by an independent alien can be "forced"
      //	since it can't be matched by another alien (except one that
      //	subsumes the first) and there are no variables or extension.
      //
      local.copy(solution);  // make a local copy of solution
      for (int i = 0; i < nrIndependentAliens; i++)
	{
	  NonGroundAlien& nga = nonGroundAliens[i];
	  Term* t = nga.term;
	  LhsAutomaton* a = nga.automaton;
	  int m = nga.multiplicity;
	  for (int j = (t == 0) ? 0 : subject->findFirstPotentialMatch(t, solution);
	       j < nrArgs; j++)
	    {
	      DagNode* d = args[j].dagNode;
	      if (t != 0 && t->partialCompare(solution, d) == Term::GREATER)
		break;
	      if (currentMultiplicity[j] >= m)
		{
		  Subproblem* sp;
		  if (a->match(d, local, sp))
		    {
		      solution.copy(local);
		      currentMultiplicity[j] -= m;
		      subproblems.add(sp);
		      goto nextIndependentAlien;
		    }
		  local.copy(solution);  // restore local copy of solution
		}
	    }
	  return false;
	nextIndependentAlien:
	  ;
	}
    }

  if (nonGroundAliens.length() > nrIndependentAliens)
    {
      //
      //	Need to build bipartite graph for remaining aliens as usual.
      //
      ACU_Subproblem* sp = buildBipartiteGraph(subject,
					       solution,
					       0,
					       nrIndependentAliens,
					       subproblems);
      if (sp == 0)
	return false;
      if (sp->noPatterns())
	delete sp;  // remaining aliens were forced
      else
	{
	  sp->addSubjects(currentMultiplicity);
	  subproblems.add(sp);
	}
    }

  returnedSubproblem = subproblems.extractSubproblem();
  return true;
}

local_inline int
ACU_LhsAutomaton::computeTotalMultiplicity()
{
  int totalSubjectMultiplicity = 0;
  FOR_EACH_CONST(i, Vector<int>, currentMultiplicity)
    totalSubjectMultiplicity += *i;
  return totalSubjectMultiplicity;
}

bool
ACU_LhsAutomaton::match(DagNode* subject,
			Substitution& solution,
			Subproblem*& returnedSubproblem,
			ExtensionInfo* extensionInfo)
{
  if (subject->symbol() != topSymbol)
    {
      if (collapsePossible)
	return collapseMatch(subject, solution, returnedSubproblem, extensionInfo);
      return false;
    }
  Assert(matchAtTop == (extensionInfo != 0), "matchAtTop disagreement");
  returnedSubproblem = 0;
  //
  //	Check to see if we should use red-black matcher.
  //
  if (safeCast(ACU_BaseDagNode*, subject)->isTree())
    {
      ACU_TreeDagNode* t = safeCast(ACU_TreeDagNode*, subject);
      if (treeMatchOK)
	{
	  int r = treeMatch(t,
			    solution,
			    returnedSubproblem,
			    safeCast(ACU_ExtensionInfo*, extensionInfo));
	  if (r == true || r == false)
	    return r;
	}
      (void) ACU_TreeDagNode::treeToArgVec(t);
    }
  ACU_DagNode* s = safeCast(ACU_DagNode*, subject);
  //
  //	Check for trivial failure and eliminate stuff that can only
  //	be matched in one way.
  //
  if (!multiplicityChecks(s) ||
      !eliminateGroundAliens(s) ||
      !eliminateBoundVariables(s, solution) ||
      !eliminateGroundedOutAliens(s, solution))
    return false;

  if (extensionInfo == 0)
    {
      //
      //	Matching without extension special cases:
      //
      if (lastUnboundVariable == NONE)
	{
	  if (totalNonGroundAliensMultiplicity != computeTotalMultiplicity())
	    return false;
	  if (totalNonGroundAliensMultiplicity == 0)
	    return true;
	  if (matchStrategy == ALIENS_ONLY)
	    return aliensOnlyMatch(s, solution, returnedSubproblem);
	}
      else
	{
	  Assert(matchStrategy != GROUND_OUT, "didn't ground out");
	  Assert(matchStrategy != ALIENS_ONLY, "variable left");
	  TopVariable& tv = topVariables[lastUnboundVariable];
	  if (tv.prevUnbound == NONE)
	    {
	      int needed = totalNonGroundAliensMultiplicity;
	      if (needed == 0)
		return forcedLoneVariableCase(s, solution, returnedSubproblem);
	      //
	      //	Check for trivial failure.
	      //
	      if (!(tv.takeIdentity))
		needed += tv.multiplicity;
	      if (computeTotalMultiplicity() < needed)
		return false;
	    }
	  Assert(matchStrategy != LONE_VARIABLE, "> 1 variable left");
	}
    }
  //
  //	If we have are matching with extension we have to ensure that at least
  //	two real subterms are actually matched; i.e. extension cannot be whole
  //	subject or whole subject except for one alien subterm.
  //
  ACU_ExtensionInfo* e = safeCast(ACU_ExtensionInfo*, extensionInfo);
  if (e != 0)
    e->setUpperBound(totalMultiplicity - 2);

  if (matchStrategy == GREEDY)
    {
      //
      //	Greedy matching special cases:
      //
      subjects.resize(currentMultiplicity.length());
      Vector<Subject>::iterator dest = subjects.begin();
      FOR_EACH_CONST(i, Vector<int>, currentMultiplicity)
	{
	  dest->multiplicity = *i;
	  ++dest;
	}
      int r = (totalNonGroundAliensMultiplicity != 0) ?
	greedyMatch(s, solution, e) : greedyPureMatch(s, solution, e);
      if (r != UNDECIDED)
	return r;
    }
  //
  //	If all else fails...
  //
  return fullMatch(s, solution, returnedSubproblem, e);
}

bool
ACU_LhsAutomaton::fullMatch(ACU_DagNode* subject,
			    Substitution& solution,
			    Subproblem*& returnedSubproblem,
			    ACU_ExtensionInfo* extensionInfo)
{
  SubproblemAccumulator subproblems;
  ACU_Subproblem* subproblem = buildBipartiteGraph(subject,
						   solution,
						   extensionInfo,
						   0,
						   subproblems);
  if (subproblem == 0)
    return false;
  if (subproblem->noPatterns())
    {
      //cout << "no patterns\n";
      //
      //	Check for trivial cases.
      //
      if (extensionInfo == 0)
	{
	  if (lastUnboundVariable == NONE)
	    {
	      delete subproblem;
	      FOR_EACH_CONST(i, Vector<int>, currentMultiplicity)
		{
		  if (*i > 0)
		    return false;
		}
	      goto succeed;
	    }
	  else
	    {
	      TopVariable& tv = topVariables[lastUnboundVariable];
	      if (tv.prevUnbound == NONE)
		{
		  delete subproblem;
		  Subproblem* sp = 0;
		  if (forcedLoneVariableCase(subject, solution, sp))
		    {
		      subproblems.add(sp);
		      goto succeed;
		    } 
		  return false;
		}
	    }
	}
      else
	{
	  if (lastUnboundVariable == NONE)
	    {
	      delete subproblem;
	      extensionInfo->clear();
	      int total = 0;
	      int nrSubjects = currentMultiplicity.length();
	      for (int i = 0; i < nrSubjects; i++)
		{
		  int t = currentMultiplicity[i];
		  if (t > 0)
		    {
		      extensionInfo->setUnmatched(i, t);
		      total += t;
		    }
		}
	      if (total > extensionInfo->getUpperBound())
		return false;  // would give extension too much
	      extensionInfo->setMatchedWhole(total == 0);
	      extensionInfo->setValidAfterMatch(true);
	      goto succeed;
	    }
	  else
	    {
	      TopVariable& tv = topVariables[lastUnboundVariable];
	      if (tv.prevUnbound == NONE)
		{
		  //
		  //	We have extension, and a single unbound top variable.
		  //	We check if we can force the lone variable to take a lone subject.
		  //
		  if (totalMultiplicity > 2)
		    {
		      if (tv.takeIdentity)
			goto cantForce;
		      //
		      //	We had more than two subject, but we can't take the identity
		      //	so there is still hope that there is only one left that
		      //	can be forced.
		      //
		      int total = 0;
		      int nrSubjects = currentMultiplicity.length();
		      for (int i = 0; i < nrSubjects; i++)
			{
			  total += currentMultiplicity[i];
			  if (total > 1)
			    goto cantForce;
			}
		    }
		  {
		    delete subproblem;
		    Subproblem* sp = 0;
		    if (forcedLoneVariableCase(subject, solution, sp))
		      {
			subproblems.add(sp);
			extensionInfo->setMatchedWhole(true);
			extensionInfo->setValidAfterMatch(true);
			goto succeed;
		      } 
		    return false;
		  }
		cantForce:
		  ;
		}
	    }      
	}
    }
  if (!handleElementVariables(subject, solution, subproblem))
    {
      delete subproblem;
      return false;
    }
  subproblem->addSubjects(currentMultiplicity);
  subproblems.add(subproblem);
  {
    FOR_EACH_CONST(i, Vector<TopVariable>, topVariables)
      {
	if ((i->upperBound != 1 || i->takeIdentity) && solution.value(i->index) == 0)
	  {
	    subproblem->addTopVariable(i->index,
				       i->multiplicity,
				       i->takeIdentity ? 0 : 1,
				       i->upperBound,
				       i->sort);
	    if (i->abstracted != 0)
	      {
		subproblems.add
		  (new VariableAbstractionSubproblem(i->abstracted,
						     i->index,
						     solution.nrFragileBindings()));
	      }
	  }
      }
  }
  if (extensionInfo != 0)
    {
      //
      //	Extension info will not be valid until solve phase.
      //
      extensionInfo->setValidAfterMatch(false);
    }
 succeed:
  returnedSubproblem = subproblems.extractSubproblem();
  return true;
}

//#include "tty.hh"

ACU_Subproblem*
ACU_LhsAutomaton::buildBipartiteGraph(ACU_DagNode* subject,
				      Substitution& solution,
				      ACU_ExtensionInfo* extensionInfo,
				      int firstAlien,
				      SubproblemAccumulator& subproblems)
{
  ArgVec<ACU_DagNode::Pair>& args = subject->argArray;
  int nrArgs = args.length();
  ACU_Subproblem* subproblem =
    new ACU_Subproblem(subject, extensionInfo);
  int nrNonGroundAliens = nonGroundAliens.length();
  //cout << "\n bipartite: ";
  for (int i = firstAlien; i < nrNonGroundAliens; i++)
    {
      int nrMatches = 0;
      NonGroundAlien& nga = nonGroundAliens[i];
      Term* t = nga.term;
      //cout << Tty(Tty::GREEN) << t << Tty(Tty::RESET) << ' ';
      LhsAutomaton* a = nga.automaton;
      int m = nga.multiplicity;
      int pn = subproblem->addPatternNode(m);
      for (int j = (t == 0) ? 0 : subject->findFirstPotentialMatch(t, solution);
	   j < nrArgs; j++)
        {
	  DagNode* d = args[j].dagNode;
	  if (t != 0 && t->partialCompare(solution, d) == Term::GREATER)
	    break;
          if (currentMultiplicity[j] >= m)
            {
              local.copy(solution);
              Subproblem* sp;
              if (a->match(d, local, sp))
                {
                  subproblem->addEdge(pn, j, local - solution, sp);
		  ++nrMatches;
                }
            }
        }
      //cout << Tty(Tty::RED) << nrMatches << Tty(Tty::RESET) << ' ';
      if (nrMatches == 0)
        {
          delete subproblem;
          return 0;
        }
      if (nrMatches == 1)
	{
	  //cout << "forcing\n";
	  //
	  //	Only one match so we can force it.
	  //
	  int uniqueSubject;
	  LocalBinding* lb;
	  Subproblem* sp;
	  subproblem->removePatternNode(uniqueSubject, lb, sp);
	  currentMultiplicity[uniqueSubject] -= m;
	  if (lb != 0)
	    {
	      lb->assert(solution);
	      delete lb;
	    }
	  subproblems.add(sp);
	  //
	  //	We need to check if any of our unbound variables became bound.
	  //
	  int parent = NONE;
	  for (int i = lastUnboundVariable; i != NONE;)
	    {
	      TopVariable& tv = topVariables[i];
	      DagNode* d = solution.value(tv.index);
	      if (d != 0)
		{
		  if (!(subject->eliminateSubject(d,
						  tv.multiplicity,
						  currentMultiplicity)))
		    {
		      delete subproblem;
		      return 0;
		    }
		  //
		  //	Need to unlink from linked list of unbound variables.
		  //
		  i = tv.prevUnbound;
		  if (parent == NONE)
		    lastUnboundVariable = i;
		  else
		    topVariables[parent].prevUnbound = i;
		}
	      else
		{
		  parent = i;
		  i = tv.prevUnbound;
		}
	    }
	}
    }
  return subproblem;
}

bool
ACU_LhsAutomaton::handleElementVariables(ACU_DagNode* subject,
					 Substitution& solution,
					 ACU_Subproblem* subproblem)
{
  ArgVec<ACU_DagNode::Pair>& args = subject->argArray;
  int nrArgs = args.length();
  //
  //	Treat unbound variables that take exactly 1 subject like non-ground aliens.
  //
  FOR_EACH_CONST(i, Vector<TopVariable>, topVariables)
    {
      if (i->upperBound == 1 && !(i->takeIdentity) && solution.value(i->index) == 0)
	{
	  bool matchable = false;
	  int m = i->multiplicity;
	  int pn = subproblem->addPatternNode(m); 
	  for (int j = 0; j < nrArgs; j++)
	    {
	      if (currentMultiplicity[j] >= m)
		{
		  DagNode* d = args[j].dagNode;
		  if(d->leq(i->sort))
		    {
		      LocalBinding* b = new LocalBinding(1);
		      b->addBinding(i->index, d);
		      subproblem->addEdge(pn, j, b, 0);
		      matchable = true;
		    }
		}
	    }
	  if (!matchable)
	    return false;
	}
    }
  return true;
}
