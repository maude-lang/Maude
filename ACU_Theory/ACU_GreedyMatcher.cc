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
//      Implementation for greedy AC/ACU matcher.
//

int
ACU_LhsAutomaton::greedyMatch(ACU_DagNode* subject,
			      Substitution& solution,
			      ACU_ExtensionInfo* extensionInfo)
{
  ArgVec<ACU_DagNode::Pair>& args = subject->argArray;
  int nrArgs = args.length();
  local.copy(solution);  // greedy matching is speculative so make a copy
  scratch.copy(solution);  // keep a scratch copy as well
  FOR_EACH_CONST(i, Vector<NonGroundAlien>, nonGroundAliens)
    {
      Term* t = i->term;
      int j = (t == 0) ? 0 : subject->findFirstPotentialMatch(t, solution);
      if (j < nrArgs)
	{
	  int m = i->multiplicity;
	  LhsAutomaton* a = i->automaton;
	  DagNode* d = args[j].dagNode;
	  do
	    {
	      if (subjects[j].multiplicity >= m)
		{
		  Subproblem *sp;
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
		      subjects[j].multiplicity -= m;
		      goto nextNonGroundAlien;
		    }
		  scratch.copy(local);  // restore scratch copy
		}
	      ++j;
	      if (j == nrArgs)
		break;
	      d = args[j].dagNode;
	    }
	  while (t == 0 || t->partialCompare(solution, d) != Term::GREATER);
	}
      return ((i - nonGroundAliens.begin()) < nrIndependentAliens) ?
	false : UNDECIDED;
    nextNonGroundAlien:
      ;
    }
  if (greedyPureMatch(subject, local, extensionInfo) == true)
    {
      solution.copy(local);
      return true;
    }
  return UNDECIDED;
}

int
ACU_LhsAutomaton::decidePhase1FailureMode(TopVariable& fv)
{
  //
  //	We start with the assumption that everything prior to greedy
  //	pure matching was forced. Now we have failed to find something
  //	to assign to variable fv we need to decide if we might have
  //	succeeded doing something differently.
  //
  //	If we fail on some variable which cannot take identity we
  //	have to return UNDECIDED unless:
  //	(1) all variables upto and including fv one have sorts in increasing
  //	    order and have either pure sorts or sorts with bound 1; and
  //	(2) the fv has multiplicity 1 or it is the first variable.
  //
  if (fv.structure != AssociativeSymbol::PURE_SORT && fv.upperBound != 1)
    return UNDECIDED;  // combination may have be assignable
  if (fv.multiplicity != 1 && fv.prevUnbound != NONE)
    return UNDECIDED;  // previous may have used high multiplicity subject

  Sort* nextSort = fv.sort;
  for (int i = fv.prevUnbound; i != NONE;)
    {
      TopVariable& tv = topVariables[i];
      if (tv.structure != AssociativeSymbol::PURE_SORT && tv.upperBound != 1)
	return UNDECIDED;  // might have assigned a combination for tv
      if (!(leq(tv.sort, nextSort)))
	return UNDECIDED;  // might have assigned a larger subject to tv
      nextSort = tv.sort;
      i = tv.prevUnbound;
    }
  return false;
}

int
ACU_LhsAutomaton::decidePhase2FailureMode()
{
  //
  //	If we fail to assign a subject to some variable or the extension we
  //	have to return UNDECIDED unless all unbound variables have pure sorts
  //	and multiplicity 1. Weaker conditions are possible but it is tricky
  //	to avoid subtle bugs.
  //
  for (int j = lastUnboundVariable; j != NONE; j = topVariables[j].prevUnbound)
    {
      TopVariable& tv = topVariables[j];
      if (!(tv.structure == AssociativeSymbol::PURE_SORT && tv.multiplicity == 1))
	return UNDECIDED;
    }
  return false;
}

int
ACU_LhsAutomaton::greedyPureMatch(ACU_DagNode* subject,
				  Substitution& solution,
				  ACU_ExtensionInfo* extensionInfo)
{
  ArgVec<ACU_DagNode::Pair>& args = subject->argArray;
  int nrArgs = args.length();
  //
  //	Phase 1:
  //	Try to find at least one subject assignable to each variable.
  //
  {
    int nrTopVariables = topVariables.length();
    for (int i = 0; i < nrTopVariables; i++)
      {
	TopVariable& tv = topVariables[i];
	if (solution.value(tv.index) == 0)
	  {
	    int m = tv.multiplicity;
	    for (int j = 0; j < nrArgs; j++)
	      {
		if (subjects[j].multiplicity >= m &&
		    args[j].dagNode->leq(tv.sort))
		  {
		    subjects[j].multiplicity -= m;
		    tv.firstSubject = j;
		    tv.nrSubjects = 1;
		    goto nextVariable;
		  }
	      }
	    if (tv.takeIdentity)
	      {
		tv.firstSubject = NONE;
		tv.nrSubjects = 0;
	      }
	    else
	      return decidePhase1FailureMode(tv);
	  }
      nextVariable:
	;
      }
  }
  //
  //	Phase 2:
  //	Now we have to deal with any unassigned subjects.
  //
  int extensionAssignments = 0;
  for (int i = 0; i < nrArgs; i++)
    {
      int sm = subjects[i].multiplicity;
      if (sm > 0)
	{
	  DagNode* d = args[i].dagNode;
	  for (int j = lastUnboundVariable; j != NONE;)
	    {
	      TopVariable& tv = topVariables[j];
	      int m = tv.multiplicity;
	      if (tv.structure >= AssociativeSymbol::LIMIT_SORT &&
		  d->leq(tv.sort) &&
		  (m == 1 || (sm % m) == 0))
		{
		  subjects[i].assignee = j;
		  if (tv.firstSubject == i)
		    {
		      //
		      //	Slight of hand to make building normal form
		      //	substitutions easier in this tricky special case
		      //
		      tv.firstSubject = NONE;
		      subjects[i].multiplicity += m;
		    }
		  else
		    ++tv.nrSubjects;
		  goto nextSubject;
		}
	      j = tv.prevUnbound;
	    }
	  //
	  //	Need to fix failure mode
	  //
	  if (extensionInfo != 0)
	    {
	      extensionAssignments += sm;
	      if (extensionAssignments <= extensionInfo->getUpperBound())
		{
		  subjects[i].assignee = NONE;
		  goto nextSubject;
		}
	    }
	  return decidePhase2FailureMode();
	}
    nextSubject:
      ;
    }
  //
  //	Phase 3:
  //	We have a succeessful match; now we actually build the
  //	assignments we computed in the two previous phases.
  //
  bool buildReducedNodes = subject->isReduced() && topSymbol->sortConstraintFree();
  for (int i = lastUnboundVariable; i != NONE; i = topVariables[i].prevUnbound)
    {
      TopVariable& tv = topVariables[i];
      int index = tv.index;
      int firstSubject = tv.firstSubject;
      int nrSubjects = tv.nrSubjects;
      Assert(solution.value(index) == 0, "unbound variable is bound");
      if (nrSubjects == 1 && firstSubject != NONE)
	solution.bind(index, args[firstSubject].dagNode);
      else if (nrSubjects == 0)
	solution.bind(index, topSymbol->getIdentityDag());
      else
	{     
	  int m = tv.multiplicity;
	  ACU_DagNode* d = new ACU_DagNode(topSymbol, nrSubjects, ACU_DagNode::ASSIGNMENT);
	  int pos = 0;
	  for (int j = 0; j < nrArgs; j++)
	    {
	      Subject& s = subjects[j];
	      int sm = s.multiplicity;
	      if (sm > 0 && s.assignee == i)
		{
		  ACU_DagNode::Pair& p = d->argArray[pos];
		  p.dagNode = args[j].dagNode;
		  p.multiplicity = (m == 1) ? sm : (sm / m);
		  ++pos;
		}
	      else if (j == firstSubject)
		{
		  ACU_DagNode::Pair& p = d->argArray[pos];
		  p.dagNode = args[j].dagNode;
		  p.multiplicity = 1;
		  ++pos;
		}
	    }
	  Assert(pos == nrSubjects, "inconsistant number of subjects");
	  if (buildReducedNodes)
	    {
	      //	Ideally we would like to know that the variable
	      //	occurs in rhs/condition before spending time here.
	      //
	      //	If the ACU subject is fully reduced then any piece 
	      //	of it is also fully reduced and if there are no
	      //	sort constraints applicable to the top symbol we
	      //	can compute the true sort and mark the new node as
	      //	reduced to avoid wasted future normalization and
	      //	matching attempts.
	      //
	      topSymbol->computeBaseSort(d);
	      d->setReduced();
	    }
	  solution.bind(index, d);
	}
    }
  //
  //	Phase 4:
  //	Finally we build extension info if it is needed.
  //
  if (extensionInfo != 0)
    {
      extensionInfo->setValidAfterMatch(true);
      extensionInfo->setMatchedWhole(true);
      bool partial = false;
      for (int i = 0; i < nrArgs; i++)
	{
	  int m = subjects[i].multiplicity;
	  if (m > 0 && subjects[i].assignee == NONE)
	    {
	      if (!partial)
		{
		  extensionInfo->setMatchedWhole(false);
		  extensionInfo->clear();
		  partial = true;
		}
	      extensionInfo->setUnmatched(i, m);
	    }
	}
    }
  return true;
}
