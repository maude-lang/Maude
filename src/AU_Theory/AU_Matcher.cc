/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2021 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for common parts of A/AU matcher.
//

bool
AU_LhsAutomaton::match(DagNode* subject,
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
  
  if (safeCast(AU_BaseDagNode*, subject)->isDeque())
    {
      AU_DequeDagNode* t = safeCast(AU_DequeDagNode*, subject);
      if (matchStrategy == LONE_VARIABLE ||
	  matchStrategy == FAST_LONE_VARIABLE)
	{
	  int r = dequeMatch(t, solution, returnedSubproblem);
	  if (r == true || r == false)
	    return r;
	}
      (void) AU_DequeDagNode::dequeToArgVec(t);
    }
  
  AU_DagNode* s = safeCast(AU_DagNode*, subject);

  int nrArgs = s->argArray.length();
  if (nrArgs < wholeLowerBound || nrArgs > wholeUpperBound)
    return false;
  leftPos = 0;
  rightPos = nrArgs - 1;
  flexLeftPos = 0;
  flexRightPos = flexPart.length() - 1;

  SubproblemAccumulator subproblems;
  AU_ExtensionInfo* e = safeCast(AU_ExtensionInfo*, extensionInfo);
  if (e == 0)
    {
      if (!matchRigidPart(s, solution, subproblems) ||
	  (flexPart.length() != 0 &&
	   !checkForRigidEnds(s, solution, subproblems)))
	return false;
      int flexRemaining = flexRightPos - flexLeftPos + 1;
      if (flexRemaining == 0)
	{
	  DebugAdvisoryCheck(matchStrategy == GROUND_OUT,
			     "match strategy changed from " << matchStrategy <<
			     " to GROUND_OUT at match time (top symbol = \"" <<
			     topSymbol << "\")");
	  if (rightPos - leftPos + 1 != 0)
	    return false;
	  goto success;
	}
      if (flexRemaining == 1 && !(topSymbol->oneSidedId()))
	{
	  DebugAdvisoryCheck(matchStrategy == LONE_VARIABLE ||
			     matchStrategy == FAST_LONE_VARIABLE,
			     "match strategy changed from " << matchStrategy <<
			     " to LONE_VARIABLE at match time (top symbol = \"" <<
			     topSymbol << "\")");
	  Subproblem* sp;
	  if (!forcedLoneVariableCase(s, solution, sp))
	    return false;
	  subproblems.add(sp);
	  goto success;
	}
    }
  Assert(matchStrategy != GROUND_OUT &&
	 matchStrategy != LONE_VARIABLE &&
	 matchStrategy != FAST_LONE_VARIABLE, "bad strategy");
  determineRigidBlocks(solution);
  if (matchStrategy == GREEDY)
    {
      int r = greedyMatch(s, solution, e);
      if (r == false)
	return false;
      if (r == true)
	goto success;
    }
  if (!fullMatch(s, solution, subproblems, e))
    return false;
success:
  returnedSubproblem = subproblems.extractSubproblem();
  return true;
}

bool
AU_LhsAutomaton::matchRigidPart(AU_DagNode* subject,
				Substitution& solution,
				SubproblemAccumulator& subproblems)
{
  ArgVec<DagNode*>& args = subject->argArray;
  for (const Subterm& i : rigidPart)
    {
      switch (i.type)
	{
	case VARIABLE:
	  {
	    const TopVariable& tv = i.variable;
	    DagNode* b = solution.value(tv.index);
	    if (b != 0)
	      {
		//
		//	Bound variable case.
		//
		if (i.leftEnd)
		  {
		    if (!(subject->eliminateForward(b, leftPos, rightPos - flexLowerBound)))
		      return false;
		  }
		else
		  {
		    if (!(subject->eliminateBackward(b, rightPos, leftPos + flexLowerBound)))
		      return false;
		  }
	      }
	    else
	      {
		//
		//	Unbound non-identity unit variable case.
		//
		Assert(tv.upperBound == 1,
		       "unbound non-unit variable in rigid part");
		Assert(!(tv.takeIdentity),
		       "unbound variable which can take identity in rigid part");
		if (rightPos - leftPos < flexLowerBound)
		  return false;
		DagNode* d = args[i.leftEnd ? leftPos++ : rightPos--];
		if (!(d->leq(tv.sort)))
		  return false;
		solution.bind(tv.index, d);
	      }
	    break;
	  }
	case GROUND_ALIEN:
	  {
	    if (rightPos - leftPos < flexLowerBound ||
		!(i.groundAlien->equal(args[i.leftEnd ? leftPos++ : rightPos--])))
	      return false;
	    break;
	  }
	case NON_GROUND_ALIEN:
	  {
	    Subproblem* sp;
	    if (rightPos - leftPos < flexLowerBound ||
		!(i.alienAutomaton->
		  match(args[i.leftEnd ? leftPos++ : rightPos--], solution, sp)))
	      return false;
	    subproblems.add(sp);
	    break;
	  }
	}
    }
  return  rightPos - leftPos + 1 <= flexUpperBound;
}

bool
AU_LhsAutomaton::checkForRigidEnds(AU_DagNode* subject,
				   Substitution& solution,
				   SubproblemAccumulator& subproblems)
{
  int mustLeave = flexLowerBound;
  bool leftStalled = false;
  bool rightStalled = false;
  for(;;)
    {
      switch(checkLeftEnd(subject, solution, subproblems, mustLeave, rightStalled))
	{
	case false:
	  return false;
	case true:
	  {
	    if (flexLeftPos++ == flexRightPos)
	      return true;
	    break;
	  }
	case STALLED:
	  {
	    if (rightStalled)
	      return true;
	    leftStalled = true;
	    break;
	  }
	}
      switch(checkRightEnd(subject, solution, subproblems, mustLeave, leftStalled))
	{
	case false:
	  return false;
	case true:
	  {
	    if (flexLeftPos == flexRightPos--)
	      return true;
	    break;
	  }
	case STALLED:
	  {
	    if (leftStalled)
	      return true;
	    rightStalled = true;
	    break;
	  }
	}
    }
}

int
AU_LhsAutomaton::checkLeftEnd(AU_DagNode* subject,
			      Substitution& solution,
			      SubproblemAccumulator& subproblems,
			      int& mustLeave,
			      bool& rightStalled)
{
  Subterm& f = flexPart[flexLeftPos];
  switch (f.type)
    {
    case VARIABLE:
      {
	TopVariable& tv = f.variable;
	DagNode* b = solution.value(tv.index);
	if (b != 0)
	  {
	    if (!tv.takeIdentity)
	      --mustLeave;
	    return subject->eliminateForward(b, leftPos, rightPos - mustLeave);
	  }
	else if (tv.upperBound == 1 && !(tv.takeIdentity))
	  {
	    if (rightPos - leftPos < --mustLeave)
	      return false;
	    DagNode* d = subject->argArray[leftPos++];
	    if (!(d->leq(tv.sort)))
	      return false;
	    solution.bind(tv.index, d);
	    break;
	  }
	return STALLED;
      }
    case GROUND_ALIEN:
      {
	return rightPos - leftPos >= --mustLeave &&
	  f.groundAlien->equal(subject->argArray[leftPos++]);
      }
    case NON_GROUND_ALIEN:
      {
	Subproblem* sp;
	if (rightPos - leftPos < --mustLeave ||
	    !(f.alienAutomaton->match(subject->argArray[leftPos++], solution, sp)))
	  return false;
	subproblems.add(sp);
	rightStalled = false;  // we may have bound variable stalling right end
	break;
      }
    }
  return true;
}

int
AU_LhsAutomaton::checkRightEnd(AU_DagNode* subject,
			       Substitution& solution,
			       SubproblemAccumulator& subproblems,
			       int& mustLeave,
			       bool& leftStalled)
{
  Subterm& f = flexPart[flexRightPos];
  switch (f.type)
    {
    case VARIABLE:
      {
	TopVariable& tv = f.variable;
	DagNode* b = solution.value(tv.index);
	if (b != 0)
	  {
	    if (!tv.takeIdentity)
	      --mustLeave;
	    return subject->eliminateBackward(b, rightPos, leftPos + mustLeave);
	  }
	else if (tv.upperBound == 1 && !(tv.takeIdentity))
	  {
	    if (rightPos - leftPos < --mustLeave)
	      return false;
	    DagNode* d = subject->argArray[rightPos--];
	    if (!(d->leq(tv.sort)))
	      return false;
	    solution.bind(tv.index, d);
	    break;
	  }
	return STALLED;
      }
    case GROUND_ALIEN:
      {
	return rightPos - leftPos >= --mustLeave &&
	  f.groundAlien->equal(subject->argArray[rightPos--]);
      }
    case NON_GROUND_ALIEN:
      {
	Subproblem* sp;
	if (rightPos - leftPos < --mustLeave ||
	    !(f.alienAutomaton->match(subject->argArray[rightPos--], solution, sp)))
	  return false;
	leftStalled = false;  // we may have bound variable stalling left end
	subproblems.add(sp);
	break;
      }
    }
  return true;
}

bool
AU_LhsAutomaton::forcedLoneVariableCase(AU_DagNode* subject,
					Substitution& solution,
					Subproblem*& returnedSubproblem)
{
  Assert(flexPart[flexLeftPos].type == VARIABLE, "lone variable is not a variable");
  returnedSubproblem = 0;
  ArgVec<DagNode*>& args = subject->argArray;
  int nrSubjectsRemaining = rightPos - leftPos + 1;
  TopVariable& loneVariable = flexPart[flexLeftPos].variable;
  Assert(solution.value(loneVariable.index) == 0, "lone variable bound");

  if (nrSubjectsRemaining == 0)
    {
      if (!(loneVariable.takeIdentity))
	return false;
      DagNode* d = topSymbol->getIdentityDag();
      solution.bind(loneVariable.index, d);
      return (loneVariable.abstracted == 0) ? true :
	loneVariable.abstracted->match(d, solution, returnedSubproblem);
    }
  else if (nrSubjectsRemaining == 1)
    {
      DagNode* d = args[leftPos];
      solution.bind(loneVariable.index, d);  // presuming we succeed
      return (loneVariable.abstracted == 0) ?
	(d->leq(loneVariable.sort)) :
	loneVariable.abstracted->match(d, solution, returnedSubproblem);
    }
  else if (matchStrategy != FAST_LONE_VARIABLE)
    {
      //
      //	General lone variable case.
      //
      //	We construct the binding for the lone variable and then
      //	check its sort, possibly creating a sort check subproblem.
      //
      AU_DagNode* d = new AU_DagNode(topSymbol, nrSubjectsRemaining);
      int pos = 0;
      for (int i = leftPos; i <= rightPos; i++)
	d->argArray[pos++] = args[i];
      Assert(pos == nrSubjectsRemaining, "inconsistant number of subterms");
      solution.bind(loneVariable.index, d);
      if (loneVariable.abstracted != 0)
	return loneVariable.abstracted->match(d, solution, returnedSubproblem);
      if (d->checkSort(loneVariable.sort, returnedSubproblem))
	{
	  d->setProducedByAssignment();
	  if (subject->isReduced() && d->getSortIndex() != Sort::SORT_UNKNOWN)
	    d->setReduced();
	  return true;
	}
    }
  else
    {
      //
      //	Fast lone variable case.
      //
      //	We know the sort of binding will <= that of that lone
      //	variable, iff each of the subterm sorts are <= that
      //	of the lone variable, so we only need look at the sorts
      //	of the subterms.
      //
      AU_DagNode* d = new AU_DagNode(topSymbol, nrSubjectsRemaining);
      int lastIndex = Sort::SORT_UNKNOWN;
      const Sort* cs = loneVariable.sort;
      ArgVec<DagNode*>::iterator j = d->argArray.begin();
      const ArgVec<DagNode*>::const_iterator e = args.begin() + rightPos + 1;
      for (ArgVec<DagNode*>::const_iterator i = args.begin() + leftPos; i != e; ++i, ++j)
	{
	  DagNode* sd = *i;
	  int index = sd->getSortIndex();
	  Assert(index != Sort::SORT_UNKNOWN, "bad sort");
	  if (index != lastIndex)
	    {
	      if (!(leq(index, cs)))
		return false;
	      lastIndex = index;
	    }
	  *j = sd;
	}
      Assert(j == d->argArray.end(), "iterator problem");
      d->setProducedByAssignment();
      if (subject->isReduced() && topSymbol->sortConstraintFree())
	{
	  topSymbol->computeBaseSort(d);
	  d->setReduced();
	}
      solution.bind(loneVariable.index, d);
      return true;
    }
  return false;
}

void
AU_LhsAutomaton::determineRigidBlocks(Substitution& solution)
{
  RigidBlock r;
  r.start = NONE;
  r.nrSubjectsForUs = 0;
  r.nrSubjectsToLeave = 0;
  r.firstMatch = NONE;  // to avoid compiler warning
  rigidBlocks.clear();
  nrSubjectsUsed = 0;
  int lastFlexPart = flexPart.length() - 1;
  int skip;
  for (int i = flexLeftPos; i <= flexRightPos; i += skip)
    {
      Subterm& f = flexPart[i];
      skip = f.blockLength;
      if (skip == NOT_FIXED)
	{
	  skip = 1;
	  f.variable.nastyBinding = NONE;
	  DagNode* d = solution.value(f.variable.index);
	  if (d == 0)
	    {
	      //
	      //	Unbound variable terminates current rigid block (if there is one).
	      //	We treat variables with nasty bindings as being unbound
	      //	since we can't be sure how many subjects they will match.
	      //
	      if (r.start != NONE)
		{
		  r.end = i - 1;
		  rigidBlocks.append(r);
		  r.start = NONE;
		  r.nrSubjectsForUs = 0;
		  r.nrSubjectsToLeave = 0;
		}
	      if (!(f.variable.takeIdentity))
		{
		  ++r.nrSubjectsToLeave;  // for this unbound variable
		  ++nrSubjectsUsed;
		}
	    }
	  else
	    {
	      bool nasty;
	      int n = topSymbol->
		calculateNrSubjectsMatched(d, i == 0, i == lastFlexPart, nasty);
	      nrSubjectsUsed += n;
	      if (nasty)
		{
		  DebugAdvisoryCheck(false, "nasty binding of " << d <<
				     " to variable with index " << f.variable.index <<
				     " detected in match phase");
		  Assert(matchAtTop,
			 "can't have nasty binding without extension");
		  //
		  //	Unbound variable terminates current rigid block (if there is one).
		  //	We treat variables with nasty bindings as being unbound
		  //	since we can't be sure how many subjects they will match.
		  //
		  if (r.start != NONE)
		    {
		      r.end = i - 1;
		      rigidBlocks.append(r);
		      r.start = NONE;
		      r.nrSubjectsForUs = 0;
		      r.nrSubjectsToLeave = 0;
		    }
		  r.nrSubjectsToLeave += n;
		  f.variable.nastyBinding = n;
		}
	      else
		{
		  r.nrSubjectsForUs += n;
		  if (r.start == NONE)
		    r.start = i;
		}
	    }
	}
      else
	{
	  r.nrSubjectsForUs += skip;
	  nrSubjectsUsed += skip;
	  if (r.start == NONE)
	    r.start = i;
	}   
    }
  //
  //	End of flex part terminates current rigid block (if there is one).
  //
  if (r.start != NONE)
    {
      Assert(matchAtTop, "missing extension");
      r.end = flexRightPos;
      rigidBlocks.append(r);
      nrSubjectsForRightVars = 0;
    }
  else
    nrSubjectsForRightVars = r.nrSubjectsToLeave;
}
