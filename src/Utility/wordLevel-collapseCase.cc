/*

    This file is part of the Maude 3 interpreter.

    Copyright 2021 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for handling checking and inter-reducing assignments where
//	collapse to empty is allowed.
//

WordLevel::Result
WordLevel::checkAssignmentCollapseCase(int i)
{
  //
  //	Check the assignment to variable i.
  //	We can FAIL if assignment can't satisfy its constraint.
  //	We can force empty assignments for variables in the assignment.
  //	We can create null equations.
  //	We can tighten constraints on variables in the assignment.
  //	We can mark assignments as being unsafe to substitute in other
  //	  assignments and equations.
  //
  //	Return values:
  //	  FAIL : no consistent - whole layer fails
  //	  DONE : no changes
  //	  CHANGED : at least one constraint or binding changed.
  //
  //	We about to recheck the safety of this assignment so
  //	we assume it is safe until proven otherwise.
  //
  unsafeAssignments.subtract(i);
  //
  //	See if we actually have an upperbound constraint to enforce.
  //
  VariableConstraint lhsConstraint = constraintMap[i];
  int upperBound = lhsConstraint.getUpperBound();
  if (upperBound == 0)
    return DONE;  // no constraint to enforce
  //
  //	Three cases depending on the size of the binding: 0, 1 or many variables.
  //
  Word& word = partialSolution[i];
  int wordSize = word.size();
  
  if (wordSize == 0)
    return DONE;

  if (wordSize == 1)
    {
      int rhs = word[0];
      if (rhs == i)
	return DONE;  // bound to self - i.e. free
      //
      //	We have X |-> Y
      //	We propagate the constraint of X on to Y.
      //
      VariableConstraint lhsConstraint = constraintMap[i];
      VariableConstraint rhsConstraint = constraintMap[rhs];
      if (!(rhsConstraint.intersect(lhsConstraint)))
	return FAIL;  // incompatible
      if (constraintMap[rhs] == rhsConstraint)
	return DONE;  // no change
      constraintMap[rhs] = rhsConstraint;  // update
      return CHANGED;  // might trigger more propagations
    }
  //
  //	First we calculate the bound we need if all variables
  //	that can take empty actually do so.
  //
  int neededBound = 0;
  for (int j : word)
    {
      if (!(constraintMap[j].canTakeEmpty()))
	++neededBound;
    }
  if (neededBound > upperBound)
    {
      //
      //	Even with collapse, there is no way to meet our bound.
      //
      return FAIL;
    }
  if (neededBound == upperBound)
    {
      //
      //	Our bound is tight so we can force a unique collapse.
      //	This puts all rhs variables that can take empty to empty
      //	and places a bound of 1 on all others.
      //
      bool changed = false;
      for (int j : word)
	{
	  if (constraintMap[j].canTakeEmpty())
	    {
	      if (makeEmptyAssignment(j))  // may introduce null equations
		changed = true;
	    }
	  else
	    {
	      if (constraintMap[j].getUpperBound() != 1)
		{
		  constraintMap[j].setUpperBound(1);
		  changed = true;
		}
	    }
	}
      if (changed)
	return handleNullEquations() ? CHANGED : FAIL;
      return DONE;
    }
  //
  //	Bound is loose so there may be multiple ways to satisfy it.
  //
  if (wordSize > upperBound)
    {
      //
      //	Current assignment doesn't satisfy bound so it could
      //	produce unnecessarily hard equations and incompleteness
      //	if we substitututed. But there is currently no unique
      //	resolution that we can force without losing solutions.
      //	We defer consideration of this assignment until it is
      //	resolved by constraint propagation or selection.
      //
      unsafeAssignments.insert(i);
      return DONE;
    }
  //
  //	Constraint is satisfied but we might be able to tighten constraints
  //	on rhs variables.
  //
  Result result = DONE;
  int boundForTakeEmpty = upperBound - neededBound;
  for (int j : word)
    {
      VariableConstraint& rhsConstraint = constraintMap[j];
      int rhsBound = rhsConstraint.getUpperBound();
      //
      //	Bound for variables that can take the empty assignment
      //	is one variable tighter than those that can't.
      //
      int newBound = constraintMap[j].canTakeEmpty() ?
	boundForTakeEmpty : (boundForTakeEmpty + 1);
      if (rhsBound == 0 || newBound < rhsBound)
	{
	  rhsConstraint.setUpperBound(newBound);
	  result = CHANGED;
	}
    }
  return result;
}

WordLevel::Result
WordLevel::checkAssignmentsCollapseCase()
{
  //
  //	Check all assignments once.
  //
  bool changed = false;
  int nrAssignments = partialSolution.size();
  for (int i = 0; i < nrAssignments; ++i)
    {
      Result result = checkAssignmentCollapseCase(i);
      if (result == FAIL)
	return FAIL;
      if (result == CHANGED)
	changed = true;
    }
  return changed ? CHANGED : DONE;
}

bool
WordLevel::checkAssignmentsToFixedPointCollapseCase()
{
  //
  //	Keep propagating constraints to fixed point.
  //
  for (;;)
    {
      Result result = checkAssignmentsCollapseCase();
      if (result == FAIL)
	return false;
      if (result == DONE)
	break;
    }
  return true;
}

bool
WordLevel::reallyExpandAssignmentCollapseCase(int i)
{
  //
  //	Returns false if this provokes level failure.
  //
  Word& word = partialSolution[i];
  Word newWord;
  bool occursCheckFail = false;
  for (int j : word)
    {
      Assert(j != i, "occurs-check issue for assignment variable x"
	     << i << " |-> " << word);
      if (unsafeAssignments.contains(j))
	newWord.append(j);  // don't replace j by its assignment
      else
	{
	  Word& assigned = partialSolution[j];
	  if (assigned.size() == 1 && assigned[0] == j)
	    newWord.append(j);  // j is assigned itself
	  else
	    occursCheckFail |= append(newWord, assigned, i);
	}
    }
  if (occursCheckFail)
    return resolveOccursCheckFailure(i, newWord);
  //
  //	Replace old assignment with new assignment.
  //
  word.swap(newWord);
  Result result = checkAssignmentCollapseCase(i);
  return result == DONE || (result == CHANGED && checkAssignmentsToFixedPointCollapseCase());
}

WordLevel::Result
WordLevel::expandAssignmentCollapseCase(int i)
{
  //
  //	Check if assignment needs expansion, i.e. that a variable in
  //	rhs has an assignment different from itself.
  //
  Word& word = partialSolution[i];
  for (int j : word)
    {
      if (j == i)
	{
	  Assert(word.size() == 1, "already dealt with occurs-check failure");
	  return DONE;
	}
      if (!unsafeAssignments.contains(j))
	{
	  Word& assigned = partialSolution[j];
	  if (!(assigned.size() == 1 && assigned[0] == j))
	    return reallyExpandAssignmentCollapseCase(i) ? CHANGED : FAIL;
	}
    }
  return DONE;
}

WordLevel::Result
WordLevel::expandAssignmentsCollapseCase()
{
  bool changed = false;
  int nrAssignments = partialSolution.size();
  for (int i = 0; i < nrAssignments; ++i)
    {
      Result result = expandAssignmentCollapseCase(i);
      if (result == FAIL)
	return FAIL;
      if (result == CHANGED)
	changed = true;
    }
  return changed ? CHANGED : DONE;
}

bool
WordLevel::expandAssignmentsToFixedPointCollapseCase()
{
  //
  //	First we check and maximally propagate constraints.
  //
  if (!checkAssignmentsToFixedPointCollapseCase())
    return false;
  //
  //	Then we keep expanding assignments until fixed point. Any
  //	time we change an assignment, we will check it and if
  //	might affect constraints we maximally propagate them again.
  //
  for (;;)
    {
      Result result = expandAssignmentsCollapseCase();
      if (result == FAIL)
	return false;
      if (result == DONE)
	break;
    }
  return true;
}
