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
  for (Equation& eq : unsolvedEquations)
    {
      Result result = simplifyEquation(eq);
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

void
WordLevel::append(Word& newWord, const Word& word)
{
  for (int i : word)
    newWord.append(i);
}

void
WordLevel::expandWord(Word& newWord, const Word& oldWord)
{
  //
  //	We replace each variable in oldWord by its assignment
  //	unless the assignment is marked as unsafe. This is to
  //	avoid replacing a constrained variable with a word that
  //	cannot meet the constraint without collapse and yet does
  //	not have a unique collapse that could be forced.
  //	In particular if we have
  //	  A |-> X Y Z
  //	where A is theory constrained, we don't want to replace
  //	a abstraction with a bunch of variables into which we
  //	cannot propagate the constriant.
  //	
  for (int i : oldWord)
    {
      if (unsafeAssignments.contains(i))
	newWord.append(i);
      else
	append(newWord, partialSolution[i]);
    }
}

void
WordLevel::updateRemainder(Word& word, int leftCursor, int rightCursor)
{
  //
  //	We update the remainer of the word. Note that we have already
  //	used all safe substitutions except any we just made; and they
  //	will be variable for variable.
  //
  for (; leftCursor <= rightCursor; ++leftCursor)
    {
      int original = word[leftCursor];
      if (!(unsafeAssignments.contains(original)))
	word[leftCursor] = partialSolution[original][0];
    }
}

void
WordLevel::copyBack(Word& destination, const Word& source, int leftCursor, int rightCursor)
{
  int nrVariables = rightCursor - leftCursor + 1;
  destination.resize(nrVariables);
  for (int i = 0; i < nrVariables; ++i, ++leftCursor)
    destination[i] = source[leftCursor];
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
WordLevel::simplifyEquation(Equation& equation)
{
  DebugEnter(equation.lhs << " =? " << equation.rhs);
#if 0
  int nrVariables = partialSolution.size();
  for (int i = 0; i < nrVariables; ++i)
    {
      cerr << "x" << i << " |-> " << partialSolution[i] << "    " << constraintMap[i];
      if (unsafeAssignments.contains(i))
	cerr << "  (UNSAFE)";
      cerr << endl;
    }
#endif
  //
  //	Try to simplify equation by left and right cancellation.
  //	Return values:
  //	  DONE : either no cancellation or cancellation didn't change partial solution.
  //	  CHANGED : cancellation and partial solution or null equations changed
  //	  FAIL : variable constraint clash or occur-check failure
  //
  if (equation.lhs.empty())
    return DONE;  // equation already simplified away
  //
  //	First we substitute in any assignments to the lhs and rhs.
  //
  Word newLhs;
  expandWord(newLhs, equation.lhs);
  Word newRhs;
  expandWord(newRhs, equation.rhs);
  DebugInfo("after expansion " << newLhs << " =? " << newRhs);
  if (levelType == PIGPUG)
    {
      //
      //	We should not have new null assignments in a PIGPUG level
      //	because PigPug does not make null bindings and simplyEquations()
      //	isn't allowed to (to avoid duplicating work that will be done
      //	in a SELECTION level).
      //	Thus substituting in an equation that didn't previously have
      //	a null unificand should not produce a null unificand.
      //
      Assert(!newLhs.empty() , "lhs of equation exanded to empty in PIGPUG level");
      Assert(!newRhs.empty() , "rhs of equation exanded to empty in PIGPUG level");
    }
  else
    {
      //
      //	Check for the possibility that one or both sides may
      //	have become null. This can happen in the INITIAL level
      //	or in a SELECTION level because of null assignments.
      //
      Result result = checkForNull(newLhs, newRhs);
      if (result == CHANGED)
	{
	  if (!handleNullEquations())
	    return FAIL;
	}
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
    if (result == UNSAFE)
      return DONE;  // give up with this simplification
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
      if (result == UNSAFE)
	{
	  //
	  //	We've gotten the equation simplified to a singleton case
	  //	but the singleton variable has an unsafe assignment so
	  //	we have to postpone any further simplification until
	  //	constraint propagation or selection have resolved the unsafe
	  //	binding. We just save the work we did back into the equation.
	  //
	  copyBack(equation.lhs, newLhs, lhsLeftCursor, lhsRightCursor);
	  copyBack(equation.rhs, newRhs, rhsLeftCursor, rhsRightCursor);
	  DebugInfo("Can't turn " << equation.lhs << " =? " << equation.rhs <<
		    " into an assignment because an unsafe assignment already exists");
	  return changed ? CHANGED : DONE;
	}
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
      DebugInfo("cancelled at right end " <<  lhsRightCursor << " ~ " << rhsRightCursor);

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
      if (result == UNSAFE)
	{
	  //
	  //	We've gotten the equation simplified to a singleton case
	  //	but the singleton variable has an unsafe assignment so
	  //	we have to postpone any further simplification until
	  //	constraint propagation or selection have resolved the unsafe
	  //	binding. We just save the work we did back into the equation.
	  //
	  copyBack(equation.lhs, newLhs, lhsLeftCursor, lhsRightCursor);
	  copyBack(equation.rhs, newRhs, rhsLeftCursor, rhsRightCursor);
	  return changed ? CHANGED : DONE;
	}
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
      if (lhsConstraint.getUpperBound() == 1 && rhsConstraint.getUpperBound() == 1)
	{
	  //
	  //	Forced unification if we in a PIGPUG level or
	  //	neither variable can take empty nor has an unsafe assignment.
	  //
	  if (levelType == PIGPUG ||
	      !(lhsConstraint.canTakeEmpty() ||
		unsafeAssignments.contains(lhsVar) ||
		rhsConstraint.canTakeEmpty() ||
		unsafeAssignments.contains(rhsVar)))
	    {
	      //
	      //	Both variables should be unbound because we already substituted
	      //	any safe bindings in.
	      //
	      Word& assignment = partialSolution[lhsVar];
	      Assert(assignment.size() == 1 && assignment[0] == lhsVar,
		     "unexpected bound variable");
	      //
	      //	It doesn't matter which way around we may the assigment
	      //	because we're going to check/propagate constraints
	      //	afterwards.
	      //
	      assignment[0] = rhsVar;
	      Result result = (levelType == PIGPUG) ?
		checkAssignmentNormalCase(lhsVar) : checkAssignmentCollapseCase(lhsVar);
	      return (result == FAIL) ? FAIL : CHANGED;
	    }
	}
    }
  return DONE;
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
  //	  UNSAFE : singleton case that can't be handled because of unsafe assignment(s).
  //
  bool singleton = false;
  if (lhsLeftCursor == lhsRightCursor)
    {
      int var = newLhs[lhsLeftCursor];
      //
      //	We can't assign to the lhs variable if it has an
      //	unsafe assignment already.
      //
      if (!(unsafeAssignments.contains(var)))
	return makeAssignment(var, newRhs, rhsLeftCursor, rhsRightCursor);
      DebugInfo("lhs x" << var << " has an unsafe assignment " <<
		partialSolution[var]);
      DebugInfo("unsafeAssignments = " << unsafeAssignments);
      singleton = true;
    }
  if (rhsLeftCursor == rhsRightCursor)
    {
      int var = newRhs[rhsLeftCursor];
      //
      //	We can't assign to the rhs variable if it has an
      //	unsafe assignment already.
      //
      if (!(unsafeAssignments.contains(var)))
	return makeAssignment(var, newLhs, lhsLeftCursor, lhsRightCursor);
      DebugInfo("rhs x" << var << " has an unsafe assignment " <<
		partialSolution[var]);
      DebugInfo("unsafeAssignments = " << unsafeAssignments);
      return UNSAFE;
    }
  return singleton ? UNSAFE : CONTINUE;
}

WordLevel::Result
WordLevel::makeAssignment(int variable, const Word& source, int leftCursor, int rightCursor)
{
  Word& destination = partialSolution[variable];
  Assert(destination.size() == 1 && destination[0] == variable, "should be free");
  int nrVariables = rightCursor - leftCursor + 1;
  if (nrVariables == 1 && source[leftCursor] == variable)
    return DONE;  // equal variables
  destination.resize(nrVariables);
  if (levelType == PIGPUG)
    {
      for (int i = 0; i < nrVariables; ++i, ++leftCursor)
	{
	  int var = source[leftCursor];
	  if (var == variable)
	    return FAIL;  // occurs-check failure
	  destination[i] = var;
	}
      return (checkAssignmentNormalCase(variable) == FAIL) ? FAIL : CHANGED;
    }
  //
  //	In INITIAL and SELECTION levels we try to resolve occurs-checks failure.
  //
  bool occursCheckFailure = false;
  for (int i = 0; i < nrVariables; ++i, ++leftCursor)
    {
      int var = source[leftCursor];
      if (var == variable)
	occursCheckFailure = true;
      destination[i] = var;
    }
  if (occursCheckFailure)
    return resolveOccursCheckFailure(variable, destination) ? CHANGED : FAIL;
  return (checkAssignmentCollapseCase(variable) == FAIL) ? FAIL : CHANGED;
}
