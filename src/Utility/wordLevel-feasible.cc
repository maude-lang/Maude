/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for checking feasibility of equations and system.
//

bool
WordLevel::feasibleWithoutCollapse(const Word& lhs, const Word& rhs) const
{
  int nrVariables = partialSolution.size();
  Vector<int> counts(nrVariables);
  for (int& count : counts)
    count = 0;
  //
  //	lhs is positive.
  //
  for (int i : lhs)
    ++(counts[i]);
  //
  //	rhs is negative.
  //
  for (int i : rhs)
    --(counts[i]);
  //
  //	Now we compute min-needed and max-can-take values
  //	for each side of the equation.
  //
  int lhsMinNeeded = 0;
  int lhsMaxCanTake = 0;
  int rhsMinNeeded = 0;
  int rhsMaxCanTake = 0;
  for (int i = 0; i < nrVariables; ++i)
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

bool
WordLevel::levelFeasibleWithoutCollapse() const
{
  //
  //	First we check for bounds issues with partial solution.
  //
  int nrVariables = partialSolution.size();
  for (int i = 0; i < nrVariables; ++i)
    {
      if (!legalWithoutCollapse(i, partialSolution[i]))
	return false;
    }
  //
  //	Then we check that each unsolved equation is feasible on its own.
  //
  for (const Equation& e : unsolvedEquations)
    {
      if (!feasibleWithoutCollapse(e.lhs, e.rhs))
	return false;
    }
  return true;
}
