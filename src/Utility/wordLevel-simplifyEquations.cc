/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2019 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for simplfying equations in a level.
//

WordLevel::Result
WordLevel::simplifyEquations()
{
  //
  //	Try to simplify equations, expanding any changes the partial solution as we go.
  //	Return values:
  //	  DONE : no changes to partial solution
  //	  CHANGED : partial solution changed
  //	  FAIL : variable constraint clash or occur-check failure
  //
  bool changed = false;
  int nrEquations = unsolvedEquations.size();
  for (int i = 0; i < nrEquations; ++i)
    {
      Result result = simplifyEquation(unsolvedEquations[i]);
      if (result == FAIL)
	return FAIL;
      if (result == CHANGED)
	{
	  //
	  //	Partial solution may now be denormalized.
	  //
	  changed = true;
	  if (!fullyExpandAssignments())
	    return FAIL;
	}
    }
  return changed ? CHANGED : DONE;
}

WordLevel::Result
WordLevel::simplifyEquation(Equation& equation)
{
  DebugEnter(equation.lhs << " =? " << equation.rhs);
  //
  //	Try to simplify equation by left and right cancellation.
  //	Return values:
  //	  DONE : either no cancellation or cancellation didn't change partial solution.
  //	  CHANGED : cancellation and partial solution or null equations changed
  //	  FAIL : variable constraint clash or occur-check failure
  //
  int nrLhsVariables = equation.lhs.size();
  if (nrLhsVariables == 0)
    return DONE;  // equation already simplified away

  //
  //	First we substitute in any assignments to the lhs and rhs.
  //
  Word newLhs;
  expandWord(newLhs, equation.lhs);
  Word newRhs;
  expandWord(newRhs, equation.rhs);
  DebugInfo("after expansion " << newLhs << " =? " << newRhs);
  if (levelType != PIGPUG)
    {
      //
      //	Check for the possibility that one or both sides may
      //	have become null. This should not be possible in PIGPUG
      //	levels because PigPug never assigns empty and simplification
      //	isn't allowed to assign empty because this would duplicate
      //	work in another branch.
      //
      Result result = checkForNull(newLhs, newRhs);
      if (result != CONTINUE)
	{
	  equation.lhs.clear();
	  equation.rhs.clear();
	  return result;
	}
    }
  //
  //	We mark the non-cancelled portion of each word with cursors.
  //
  int lhsLeftCursor = 0;
  int lhsRightCursor = newLhs.size() - 1;
  int rhsLeftCursor = 0;
  int rhsRightCursor = newRhs.size() - 1;
  //
  //	First we check for singleton case.
  //
  {
    Result result = checkForSingleton(newLhs, lhsLeftCursor, lhsRightCursor,
				      newRhs, rhsLeftCursor, rhsRightCursor);
    if (result == FAIL)
      return FAIL;
    if (result != CONTINUE)
      {
	//
	//	Singleton case - equation disappears.
	//
	equation.lhs.clear();
	equation.rhs.clear();
	return result;
    }
  }
  //
  //	Now we work from the ends, cancelling equal variables and
  //	variables that are both constrained.
  //
  bool changed = false;
  //
  //	Left cancellation
  //
  for (;;)
    {
      Result result = cancel(newLhs[lhsLeftCursor], newRhs[rhsLeftCursor]);
      if (result == FAIL)
	return FAIL;  // variable theory clash
      if (result == DONE)
	break;  // no cancellation
      //
      //	We cancelled.
      //
      ++lhsLeftCursor;
      ++rhsLeftCursor;
      DebugInfo("cancelled at left end " <<  lhsLeftCursor << " ~ " << rhsLeftCursor);

      if (result == CHANGED)
	{
	  //
	  //	We know that the only possible change is a constrained variable to
	  //	constrained variable assignment, so we can do a cheap update on the remainders.
	  //
	  changed = true;
	  updateRemainder(newLhs, lhsLeftCursor, lhsRightCursor);
	  updateRemainder(newRhs, rhsLeftCursor, rhsRightCursor);
	}

      result = checkForSingleton(newLhs, lhsLeftCursor, lhsRightCursor,
				 newRhs, rhsLeftCursor, rhsRightCursor);
      if (result == FAIL)
	return FAIL;
      if (result != CONTINUE)
	{
	  //
	  //	Singleton case - equation disappears.
	  //
	  equation.lhs.clear();
	  equation.rhs.clear();
	  return changed ? CHANGED : result;
	}
    }
  //
  //	Right cancellation
  //
  for (;;)
    {
      Result result = cancel(newLhs[lhsRightCursor], newRhs[rhsRightCursor]);
      if (result == FAIL)
	return FAIL;  // variable theory clash
      if (result == DONE)
	break;  // no cancellation
      //
      //	We cancelled.
      //
      --lhsRightCursor;
      --rhsRightCursor;
      //cout << "cancelled at right end " <<  lhsRightCursor << " ~ " << rhsRightCursor << endl;

      if (result == CHANGED)
	{
	  //
	  //	We know that the only possible change is a constrained variable to
	  //	constraint variable assignment, so we can do a cheap update on the remainders.
	  //
	  changed = true;
	  updateRemainder(newLhs, lhsLeftCursor, lhsRightCursor);
	  updateRemainder(newRhs, rhsLeftCursor, rhsRightCursor);
	}

      result = checkForSingleton(newLhs, lhsLeftCursor, lhsRightCursor,
				 newRhs, rhsLeftCursor, rhsRightCursor);
      if (result == FAIL)
	return FAIL;
      if (result != CONTINUE)
	{
	  //
	  //	Singleton case - equation disappears.
	  //
	  equation.lhs.clear();
	  equation.rhs.clear();
	  return changed ? CHANGED : result;
	}
    }
  //
  //	If we reach here, we have fully cancelled and both sides are non-singleton.
  //
  copyBack(equation.lhs, newLhs, lhsLeftCursor, lhsRightCursor);
  copyBack(equation.rhs, newRhs, rhsLeftCursor, rhsRightCursor);
  if (levelType == PIGPUG && !feasibleWithoutCollapse(equation.lhs, equation.rhs))
    return FAIL;
  return changed ? CHANGED : DONE;
}

void
WordLevel::expandWord(Word& newWord, const Word& oldWord)
{
  FOR_EACH_CONST(i, Word, oldWord)
    append(newWord, partialSolution[*i]);
}

void
WordLevel::append(Word& newWord, const Word& word)
{
  FOR_EACH_CONST(i, Word, word)
    newWord.append(*i);
}

void
WordLevel::updateRemainder(Word& word, int leftCursor, int rightCursor)
{
  for (; leftCursor <= rightCursor; ++leftCursor)
    word[leftCursor] = partialSolution[word[leftCursor]][0];  // if variable got a binding, it will be another variable
}

WordLevel::Result
WordLevel::cancel(int lhsVar, int rhsVar)
{
  DebugEnter("x" << lhsVar << " =? x" << rhsVar);
  //
  //	Try to cancel lhsVar and rhsVar by unifying them.
  //	Return values:
  //	  DONE : no cancellation possible
  //	  CHANGED : cancellation caused change to partial solution
  //	  FAIL : incompatible constrained variables
  //	  CONTINUE : cancellation, no change to partial solution
  //	
  //
  if (lhsVar == rhsVar)
    return CONTINUE;  // cancel variable
  else
    {
      //
      //	If both variables are constrained to unify with exactly one other
      //	variable we cancel them via an assignment or we have a clash.
      //
      VariableConstraint lhsConstraint = constraintMap[lhsVar];
      VariableConstraint rhsConstraint = constraintMap[rhsVar];
      if (levelType == PIGPUG)
	{
	  //
	  //	In PIGPUG levels we ignore the possibility of taking empty so
	  //	we can force more things.
	  //
	  if (lhsConstraint.getUpperBound() == 1 && rhsConstraint.getUpperBound() == 1)
	    return unifyVariables(lhsVar, rhsVar);
	}
      else
	{ 
	  if (lhsConstraint.getUpperBound() == 1 &&
	      !(lhsConstraint.canTakeEmpty()) &&
	      rhsConstraint.getUpperBound() == 1 &&
	      !(rhsConstraint.canTakeEmpty()))
	    return unifyVariables(lhsVar, rhsVar);  // we never return DONE since lhsVar != rhsVar in this branch
	}
    }
  return DONE;
}

WordLevel::Result
WordLevel::checkForNull(const Word& newLhs, const Word& newRhs)
{
  Assert(levelType != PIGPUG, "shouldn't be called for PIGPUG level");
  DebugEnter(newLhs << " =? " << newRhs);
  //
  //	Handle the case where one or both unificands has been expanded to empty.
  //	Return values:
  //	  DONE : handled null =? null case
  //	  CHANGED : handled null case, null equation created
  //	  CONTINUE : non-null equation
  //
  if (newLhs.empty())
    {
     if (newRhs.empty())
	return DONE;  // both sides became null
      nullEquations.push_back(newRhs);
      return CHANGED;
    }
  if (newRhs.empty())
    {
      nullEquations.push_back(newLhs);
      return CHANGED;
    }
  return CONTINUE;
}

WordLevel::Result
WordLevel::checkForSingleton(const Word& newLhs, int lhsLeftCursor, int lhsRightCursor,
			     const Word& newRhs, int rhsLeftCursor, int rhsRightCursor)
{
  //
  //	Handle cases when one or both unificands has been cancelled down to a single variable.
  //	Return values:
  //	  DONE : handled singleton case, but partial solution didn't change (equal variables)
  //	  CHANGED : handled singleton, partial solution changed
  //	  FAIL : handled singleton case, variable constraint clash or occur-check failure
  //	  CONTINUE : non-singleton equation
  //
  if (lhsLeftCursor == lhsRightCursor)
    {
      if (rhsLeftCursor == rhsRightCursor)
	return unifyVariables(newLhs[lhsLeftCursor], newRhs[rhsLeftCursor]);
      else
	return makeAssignment(newLhs[lhsLeftCursor], newRhs, rhsLeftCursor, rhsRightCursor);
    }
  else if (rhsLeftCursor == rhsRightCursor)
    return makeAssignment(newRhs[rhsLeftCursor], newLhs, lhsLeftCursor, lhsRightCursor);
  return CONTINUE;
}

WordLevel::Result
WordLevel::unifyVariables(int lhsVar, int rhsVar)
{
  DebugEnter("x" << lhsVar << " =? x" << rhsVar);
  //
  //	We try to unify two variables by assigning one to the other.
  //	We always want to assign from the least constrained variable to the most constrained variable.
  //	Which way around we do this determined by the constraints on the variables and is
  //	summarized is the following table:
  //
  //    lhs \ rhs     |	theory			bounded			unbounded
  //  ------------------------------------------------------------------------------
  //  theory	      | lhs |-> rhs or clash	rhs |-> lhs		rhs |-> lhs
  //  bounded	      |	lhs |-> rhs		bigger |-> smaller	rhs |-> lhs
  //  unbounded       | lhs |-> rhs		lhs |-> rhs		lhs |-> rhs
  //
  //	Return values:
  //	  DONE : equal variables
  //	  CHANGED : assignment made
  //	  FAIL : incompatible constrained variables
  //
  if (lhsVar == rhsVar)
    return DONE;

  VariableConstraint lhsConstraint = constraintMap[lhsVar];
  VariableConstraint rhsConstraint = constraintMap[rhsVar];

  if (lhsConstraint.isUnbounded() ||  // bottom row
      (lhsConstraint.hasTheoryConstraint() ?
       (lhsConstraint.getTheoryConstraint() == rhsConstraint.getTheoryConstraint()) : // top-left corner
       (!(rhsConstraint.isUnbounded()) &&
	lhsConstraint.getUpperBound() >= rhsConstraint.getUpperBound()))) // middle row
    {
      //
      //	lhsVar |-> rhsVar
      //
      Word& assignment = partialSolution[lhsVar];
      assignment.resize(1);
      assignment[0] = rhsVar;
      return CHANGED;
    }
  //
  //	We dealt with the lhs |-> rhs cases. So if we don't have a theory clash we must
  //	be in a rhs |-> lhs case.
  //
  if (!(rhsConstraint.hasTheoryConstraint()))
    {
      //
      //	rhsVar |-> lhsVar
      //
      Word& assignment = partialSolution[rhsVar];
      assignment.resize(1);
      assignment[0] = lhsVar;
      return CHANGED;
    }
  //
  //	Theory clash.
  //
  return FAIL;
}

WordLevel::Result
WordLevel::makeAssignment(int variable, const Word& source, int leftCursor, int rightCursor)
{
  //
  //	Assign
  //	  variable |-> source[leftCursor]...source[rightCursor]
  //	We require
  //	  leftCursor < rightCursor
  //	This only succeeds if variable is unbounded or has a large enough upperbound.
  //	Since exactly one non-element variable can be assigned to during an equation
  //	simplification (this is it), and all bound variables were substituted out
  //	at the start of simplification, we assumed that variable is unbound (actually
  //	bound to itself using current Subst representation).
  //	Return values:
  //	  CHANGED : assignment made
  //	  FAIL : variable is constrained, or occur-check failure
  //
  int nrVariables = rightCursor - leftCursor + 1;
  Assert(nrVariables >= 2, "nrVariables = " << nrVariables);
  //
  //	Extract the subword and determine what minimum it needs
  //	for upperbound, and where an there is an occurs-check failure.
  //
  int needed = 0;
  bool occursCheckFailure = false;
  Word subword(nrVariables);
  for (int i = 0; i < nrVariables; ++i, ++leftCursor)
    {
      int var = source[leftCursor];
      ++needed;
      if (var == variable)
	occursCheckFailure = true;
      subword[i] = var;
    }
  DebugInfo("Want x" << variable << " |-> " << subword);
  if (levelType == PIGPUG)
    {
      if (occursCheckFailure)
	return FAIL;
      const VariableConstraint&  varConstraint = constraintMap[variable];
      if (!(varConstraint.isUnbounded()) && needed > varConstraint.getUpperBound())
	return FAIL;  // variable can't take subword
    }
  else
    {
      //
      //	We don't enforce upperbounds in INITIAL and SELECTION
      //	layers until all the simplification has been done because
      //	the simplification may make empty assignments. We do try
      //	to use empty assignments to resolve occurs-check failure.
      //
      if (occursCheckFailure)
	return resolveOccursCheckFailure(variable, subword) ? CHANGED : FAIL;
    }

  Word& destination = partialSolution[variable];
  Assert(destination.size() == 1 && destination[0] == variable,
	 "variable " << variable << " not bound to itself");
  destination.swap(subword);
  return CHANGED;
}

void
WordLevel::copyBack(Word& destination, const Word& source, int leftCursor, int rightCursor)
{
  int nrVariables = rightCursor - leftCursor + 1;
  destination.resize(nrVariables);
  for (int i = 0; i < nrVariables; ++i, ++leftCursor)
    destination[i] = source[leftCursor];
}
