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
    {
      //
      //	This can happen if the variable got an empty assignment
      //	during simplification in the INITIAL level, during selection
      //	or during simplification in the SELECTION level.
      //
      return DONE;
    }

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
	  //cout << "lowered bound from " << rhsBound << " to " << newBound << endl;
	  rhsConstraint.setUpperBound(newBound);
	  result = CHANGED;
	}
    }
  return result;
}

bool
WordLevel::checkAssignmentsNormalCase()
{
  //
  //	Check all assignments once. Because all assignments
  //	are fully interreduced, all rhs variables are free and
  //	thus any constrains propagated from lhs to rhs cannot
  //	propagate any further, so once is enough.
  //
  int nrAssignments = partialSolution.size();
  for (int i = 0; i < nrAssignments; ++i)
    {
      if (checkAssignmentNormalCase(i) == FAIL)
	return false;
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
  return true;
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
	return reallyExpandAssignmentNormalCase(i) ? CHANGED : FAIL;
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
  //	We keep expanding assignments until fixed point.
  //	This is needed beause we could have something like
  //	  X |-> Y a
  //	  Y |-> Z b
  //	  Z |-> c
  //	and after one pass through we would still have
  //	  X |-> Z b a
  //	where Z appears in the range but is bound to something
  //	other than itself.
  //
  for (;;)
    {
      Result result = expandAssignmentsNormalCase();
      if (result == FAIL)
	return false;
      if (result == DONE)
	break;
    }
  //
  //	Check and propagate constraints.
  //
  return checkAssignmentsNormalCase();
}
