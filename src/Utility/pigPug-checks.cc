/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for feasibility, completion checks.
//

bool
PigPug::feasible()
{
  //
  //	Conservative estimate of whether an equation is solvable.
  //	  false means definitely not solvable.
  //	  true means possibily solvable.
  //
  //	First we cancel variables using commutativity.
  //	We count the net occurrences of each variable.
  //
  Vector<int> counts(lastOriginalVariable + 1);
  for (int i = 0; i <= lastOriginalVariable; ++i)
    counts[i] = 0;
  {
    //
    //	lhs is positive.
    //
    Unificand& lhs = lhsStack.back();
    int length = lhs.word.size();
    for (int i = lhs.index; i < length; ++i)
      ++(counts[lhs.word[i]]);
  }
  {
    //
    //	rhs is negative.
    //
    Unificand& rhs = rhsStack.back();
    int length = rhs.word.size();
    for (int i = rhs.index; i < length; ++i)
      --(counts[rhs.word[i]]);
  }
  //
  //	Now we compute min-needed and max-can-take values
  //	for each side of the equation.
  //
  int lhsMinNeeded = 0;
  int lhsMaxCanTake = 0;
  int rhsMinNeeded = 0;
  int rhsMaxCanTake = 0;
  const ConstraintMap& constraintMap = constraintStack.back();
  for (int i = 0; i <= lastOriginalVariable; ++i)
    {
      int balance = counts[i];
      if (balance != 0)
	{
	  //
	  //	Variable didn't cancel completely so it will play a role in
	  //	the feasibility calculation.
	  //
	  VariableConstraint vc = constraintMap[i];
	  if (balance > 0)
	    {
	      lhsMinNeeded += balance;
	      if (lhsMaxCanTake != UNBOUNDED)  // once we get to UNBOUNDED we stay there
		{
		  int upperBound = vc.getUpperBound();
		  if (upperBound == 0)
		    lhsMaxCanTake = UNBOUNDED;  // unbounded case
		  else
		    lhsMaxCanTake += upperBound * balance;
		}
	    }
	  else
	    {
	      rhsMinNeeded -= balance;
	      if (rhsMaxCanTake != UNBOUNDED)  // once we get to UNBOUNDED we stay there
		{
		  int upperBound = vc.getUpperBound();
		  if (upperBound == 0)
		    rhsMaxCanTake = UNBOUNDED;  // unbounded case
		  else
		    rhsMaxCanTake -= upperBound * balance;
		}
	    }
	}
    }
  //
  //	We're feasible if there is no conflict among these values.
  //
  return lhsMinNeeded <= rhsMaxCanTake && rhsMinNeeded <= lhsMaxCanTake;
}

int
PigPug::completed(int status)
{
  //
  //	Deal with the case where at least one side of the equation is down
  //	to a single variable.
  //
  const ConstraintMap& constraintMap = constraintStack.back();
  //
  //	At least side of current equation is down to a single variable; see
  //	if we can make a solution.
  //
  Unificand& lhs = lhsStack.back();
  Unificand& rhs = rhsStack.back();
  if (status == LHS_DONE)
    {
      //
      //	lhs is down to one variable.
      //
      int lhsVar = lhs.word[lhs.index]; 
      if (rhs.index + 1 == rhs.word.length())
	{
	  //
	  //	rhs one variable also. Need to check for constraint compatibility.
	  //
	  int rhsVar = rhs.word[rhs.index];
	  if (lhsVar == rhsVar)
	    {
	      //
	      //	We don't put a FINAL move on the stack - if we
	      //	did it would be a no-op for extractUnifier() and
	      //	would be immediately popped without further action
	      //	during backtracking by undoMove().
	      //
	      return status;
	    }

	  VariableConstraint lhsConstraint = constraintMap[lhsVar];
	  VariableConstraint rhsConstraint = constraintMap[rhsVar];
	  VariableConstraint meet(lhsConstraint);
	  if (meet.intersect(rhsConstraint))
	    {
	      if (rhsConstraint == meet)
		path.append(EQUATE | FINAL);  // rhs is at least as tight as lhs
	      else if (lhsConstraint == meet)
		path.append(EQUATE | FINAL | RHS_ASSIGN);  // lhs is at least as tight as lhs
	      else
		{
		  //
		  //	Compatible but incomparable constraints.
		  //	We put lhs |-> rhs but rhs constraint must become the meet.
		  //
		  constraintStack.push_back(constraintMap);
		  ConstraintMap& newConstraintMap = constraintStack.back();
		  newConstraintMap[rhsVar] = meet;
		  path.append(EQUATE | FINAL | PUSH_CONSTRAINT_MAP);
		}
	      return status;
	    }
	}
      else
	{
	  //
	  //	rhs has more than one variable.
	  //
	  if (feasible())
	    {
	      int move = LHS_TAKES_ALL;
	      if (checkConstraintMap(lhsVar, rhs))
		move |= PUSH_CONSTRAINT_MAP;
	      path.append(move);
	      return status;
	    }
	}
    }
  else
    {
      Assert(status == RHS_DONE, "bad status for completed()");
      if (feasible())
	{
	  int rhsVar = rhs.word[rhs.index];
	  int move = RHS_TAKES_ALL;
	  if (checkConstraintMap(rhsVar, lhs))
	    move |= PUSH_CONSTRAINT_MAP;
	  path.append(move);
	  return status;
	}
    }
  return FAIL;
}
