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
//	collapse to empty is not allowed.
//

WordLevel::Result
WordLevel::checkAssignmentNormalCase(int i)
{
  //
  //	Check the assignment to variable i.
  //	We can FAIL if assignment can't satisfy its constraint.
  //	We can tighten constraints on variables in the assignment.
  //
  //	Return values:
  //	  FAIL : no consistent - whole layer fails
  //	  DONE : no changes
  //	  CHANGED : at least one constraint or binding changed.
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

  if (wordSize > upperBound)
    return FAIL;  // without collapse there is no way to satisfy the constraint
  //
  //	Constraint is satisfied but we might be able to tighten constraints
  //	on rhs variables.
  //
  Result result = DONE;
  int newBound = upperBound - wordSize + 1;
  for (int j : word)
    {
      VariableConstraint& rhsConstraint = constraintMap[j];
      int rhsBound = rhsConstraint.getUpperBound();
      if (rhsBound == 0 || newBound < rhsBound)
	{
	  rhsConstraint.setUpperBound(newBound);
	  result = CHANGED;
	}
    }
  return result;
}

WordLevel::Result
WordLevel::checkAssignmentsNormalCase()
{
  //
  //	Check all assignments once.
  //
  bool changed = false;
  int nrAssignments = partialSolution.size();
  for (int i = 0; i < nrAssignments; ++i)
    {
      Result result = checkAssignmentNormalCase(i);
      if (result == FAIL)
	return FAIL;
      if (result == CHANGED)
	changed = true;
    }
  return changed ? CHANGED : DONE;
}

bool
WordLevel::checkAssignmentsToFixedPointNormalCase()
{
  //
  //	Keep propagating constraints to fixed point.
  //
  for (;;)
    {
      Result result = checkAssignmentsNormalCase();
      if (result == FAIL)
	return false;
      if (result == DONE)
	break;
    }
  return true;
}

bool
WordLevel::reallyExpandAssignmentNormalCase(int i)
{
  //
  //	Returns false if this provokes level failure.
  //
  Word& word = partialSolution[i];
  Word newWord;
  for (int j : word)
    {
      Assert(j != i, "occurs-check issue for assignment variable x"
	     << i << " |-> " << word);
      Word& assigned = partialSolution[j];
      if (assigned.size() == 1 && assigned[0] == j)
	newWord.append(j);  // j is assigned itself
      else if (append(newWord, assigned, i))
	return false;  // occur-check failure can't be solved without collapse
    }
  //
  //	Replace old assignment with new assignment.
  //
  word.swap(newWord);
  Result result = checkAssignmentNormalCase(i);
  if (result == FAIL)
    return false;
  return result == DONE || checkAssignmentsToFixedPointNormalCase();
}

WordLevel::Result
WordLevel::expandAssignmentNormalCase(int i)
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
      Word& assigned = partialSolution[j];
      if (!(assigned.size() == 1 && assigned[0] == j))
	return reallyExpandAssignmentCollapseCase(i) ? CHANGED : FAIL;
    }
  return DONE;
}

WordLevel::Result
WordLevel::expandAssignmentsNormalCase()
{
  bool changed = false;
  int nrAssignments = partialSolution.size();
  for (int i = 0; i < nrAssignments; ++i)
    {
      Result result = expandAssignmentNormalCase(i);
      if (result == FAIL)
	return FAIL;
      if (result == CHANGED)
	changed = true;
    }
  return changed ? CHANGED : DONE;
}

bool
WordLevel::expandAssignmentsToFixedPointNormalCase()
{
  //
  //	Check and maximally propagate constraints.
  //
  if (!checkAssignmentsToFixedPointNormalCase())
    return false;
  //
  //	Then we keep expanding assignments until fixed point. Any
  //	time we change an assignment, we will check it and if
  //	might affect constraints we maximally propagate them again.
  //
  for (;;)
    {
      Result result = expandAssignmentsNormalCase();
      if (result == FAIL)
	return false;
      if (result == DONE)
	break;
    }
  return true;
}
