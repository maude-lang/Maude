/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2014 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class SequenceAssignment.
//
#include "macros.hh"
#include "vector.hh"
#include "sequenceAssignment.hh"


SequenceAssignment::SequenceAssignment(int nrLhsVariables, int nrRhsVariables)
  : lhsBounds(nrLhsVariables),
    rhsBounds(nrRhsVariables),
    lhsBoundSum(nrLhsVariables),
    rhsBoundSum(nrRhsVariables),
    lhsCount(nrLhsVariables),
    rhsCount(nrRhsVariables)
{
  for (int i = 0; i < nrLhsVariables; ++i)
    {
      lhsBounds[i] = NONE;
      lhsCount[i] = 0;
    }
      
  for (int i = 0; i < nrRhsVariables; ++i)
    {
      rhsBounds[i] = NONE;
      rhsCount[i] = 0;
    }
  //
  //	Before the first move is made, there is already an implicit assignment to
  //	the first variables on each side.
  //
  lhsCount[0] = 1;
  rhsCount[0] = 1;
}

void
SequenceAssignment::computeBoundSum(const IntVec& bounds, IntVec& boundSum)
{
#ifndef NO_ASSERT
  if (globalAdvisoryFlag)
    cerr << Tty(Tty::GREEN) << "SequenceAssignment::computeBoundSum()";
#endif

  int sum = 0;
  for (int i = bounds.size() - 1; i >= 0; --i)
    {
#ifndef NO_ASSERT
      if (globalAdvisoryFlag)
	cerr << "    " << bounds[i];
#endif
      if (sum != NONE)
	{
	  int bound = bounds[i];
	  if (bound != NONE)
	    sum += bound;
	  else
	    sum = NONE;
	}
      boundSum[i] = sum;
    }

#ifndef NO_ASSERT
  if (globalAdvisoryFlag)
    cerr << Tty(Tty::RESET) << endl;
#endif
}

bool
SequenceAssignment::checkAndMakeMove(int move, int& lIndex, int& rIndex)
{
  int nrLhsVariables = lhsBoundSum.size();
  int nrRhsVariables = rhsBoundSum.size();

  int lDelta = leftDelta(move);
  int newLhsIndex = lIndex + lDelta;
  int rDelta = rightDelta(move);
  int newRhsIndex = rIndex + rDelta;

  int remainingLhsVariables = nrLhsVariables - newLhsIndex;
  int remainingRhsVariables = nrRhsVariables - newRhsIndex;

#ifndef NO_ASSERT
  if (globalAdvisoryFlag)
    {
      cerr << Tty(Tty::GREEN) << "SequenceAssignment::checkAndMakeMove() move is " << move <<
	"    lDelta is " << lDelta <<
	"    rDelta is " << rDelta << Tty(Tty::RESET) << endl;
    }
#endif

  if (lDelta == 1)
    {
      if (newLhsIndex == nrLhsVariables)
	return false;  // ran out of lhs variables
    }
  else
    {
      if (lhsBounds[lIndex] != NONE && lhsBounds[lIndex] == lhsCount[lIndex])
	return false;  // lhs variable can't take another binding
    }

  if (rDelta == 1)
    {
      if (newRhsIndex == nrRhsVariables)
	return false;  // ran out of rhs variables
    }
  else
    {
#ifndef NO_ASSERT
      if (globalAdvisoryFlag)
	{
	  cerr << Tty(Tty::RED) << "rIndex = " << rIndex <<
	    "    rhsBounds[rIndex] = " << rhsBounds[rIndex] <<
	    "    rhsCount[rIndex] = " << rhsCount[rIndex] << Tty(Tty::RESET) << endl;
	}
#endif
      if (rhsBounds[rIndex] != NONE && rhsBounds[rIndex] == rhsCount[rIndex])
	return false;  // rhs variable can't take another binding
    }

  int lhsCapacityBound = lhsBoundSum[newLhsIndex];
  if (lhsCapacityBound != NONE)
    {
      if (lhsCapacityBound < remainingRhsVariables)
	return false;  //  can't advance to next lhs variable since we don't have enought capacity for rhs variables
    }

  int rhsCapacityBound = rhsBoundSum[newRhsIndex];
  if (rhsCapacityBound != NONE)
    {
      if (rhsCapacityBound < remainingLhsVariables)
	return false;  //  can't advance to next lhs variable since we don't have enought capacity for rhs variables
    }
  DebugAdvisory("move OK");

  //
  //	Move is OK.
  //
  currentSolution.append(move);
  lIndex = newLhsIndex;
  rIndex = newRhsIndex;
  ++(lhsCount[lIndex]);
  ++(rhsCount[rIndex]);
  return true;
}

int
SequenceAssignment::unmakeMove(int moveIndex, int& lIndex, int& rIndex)
{
  --(lhsCount[lIndex]);
  --(rhsCount[rIndex]);
  int move = currentSolution[moveIndex];
  currentSolution.resize(moveIndex);
  lIndex -= leftDelta(move);
  rIndex -= rightDelta(move);
  return move;
}

bool
SequenceAssignment::findNextSolution(bool findFirst)
{
  int nrLhsVariables = lhsBoundSum.size();
  int nrRhsVariables = rhsBoundSum.size();

  int lIndex;
  int rIndex;
  int moveIndex;

  if (findFirst)
    {
      computeBoundSum(lhsBounds, lhsBoundSum);
      if (lhsBoundSum[0] != NONE && lhsBoundSum[0] < nrRhsVariables)
	return false;  // lhs variables don't have enought capacity for the rhs variables

      computeBoundSum(rhsBounds, rhsBoundSum);
      if (rhsBoundSum[0] != NONE && rhsBoundSum[0] < nrLhsVariables)
	return false;  // rhs variables don't have enought capacity for the lhs variables

      lIndex = 0;
      rIndex = 0;
      moveIndex = 0;
      currentSolution.clear();
      goto forwards;
    }
  else
    {
      lIndex = nrLhsVariables - 1;
      rIndex = nrRhsVariables - 1;
      moveIndex = currentSolution.size();
      goto back;
    }

 back:
  {
    --moveIndex;
    if (moveIndex < 0)
      return false;

    int move = unmakeMove(moveIndex, lIndex, rIndex);
 
  nextMove:
    if (move == NEXT_BOTH)
      goto back;
    ++move;

    if (!(checkAndMakeMove(move, lIndex, rIndex)))
      goto nextMove;
    ++moveIndex;
    goto forwards;
  }

 forwards:
  {
    while ((lIndex + 1 < nrLhsVariables) || (rIndex  + 1 < nrRhsVariables))
      {
	int move = NEXT_RIGHT;

      nextMoveForwards:
	if (!(checkAndMakeMove(move, lIndex, rIndex)))
	  {
	    Assert(move != NEXT_BOTH, "could not find a forwards move");
	    ++move;
	    goto nextMoveForwards;
	  }
	++moveIndex;
      }
  }
  return true;
}
