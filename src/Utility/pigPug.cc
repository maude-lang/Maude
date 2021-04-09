/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2020 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class PigPug.
//
#include "macros.hh"
#include "vector.hh"
#include "pigPug.hh"

//	our stuff
#include "pigPug-search.cc"
#include "pigPug-cycleDetection.cc"
#include "pigPug-checks.cc"
#include "pigPug-stack.cc"
#include "pigPug-extract.cc"

PigPug::PigPug(const Word& lhs,
	       const Word& rhs,
	       const ConstraintMap& constraintMap,
	       int lastOriginalVariable,
	       int freshVariableStart,
	       int linearity,
	       bool equateOptimization)
  : lastOriginalVariable(lastOriginalVariable),
    freshVariableStart(freshVariableStart),
    linearity(linearity),
    equateOptimization(equateOptimization)
{
  DebugEnter("|lhs| = " << lhs.size() <<
	     "  |rhs| = " << rhs.size() <<
	     "  lastOriginalVariable = " << lastOriginalVariable <<
	     "  freshVariableStart = " << freshVariableStart <<
	     "  linearity = " << linearity);
  DebugInfo(lhs << " =? " << rhs);
  //
  //	We assume we have a real equation; i.e. both sides have at least
  //	two variable occurrences.
  //
  Assert(lhs.size() >= 2, "lhs too small " << lhs.size());
  Assert(rhs.size() >= 2, "rhs too small " << rhs.size());
  //
  //	If there is linearity, it must appear on the left, whether or not
  //	it appears in the right.
  //
  Assert(linearity != STRICT_RIGHT_LINEAR,
	 "strict right linear without strict left linearity not supported");

  lhsStack.push_back(Unificand());
  lhsStack.back().index = 0;
  lhsStack.back().word = lhs;  // deep copy
  rhsStack.push_back(Unificand());
  rhsStack.back().index = 0;
  rhsStack.back().word = rhs;  // deep copy
  constraintStack.push_back(constraintMap);  // deep copy

  incompletenessFlag = 0;
  depthBound = NONE;
  cycleDetection = false;
  if (!(linearity & STRICT_LEFT_LINEAR))
    {
      //
      //	If we are not strictly left linear then we need either cycle detection
      //	or depth bounded to ensure termination.
      //	Both of these measures introduce the possibility of incompleteness.
      //
      if (variableOccurrencesBoundedBy2(lhs, rhs, lastOriginalVariable))
	{
	  cycleDetection = true;
	  Verbose("Associative unification using cycle detection.");
	}
      else
	{
	  depthBound =  lhs.size() + rhs.size();
	  Verbose("Associative unification using depth bound of " << depthBound << ".");
	}
    }
}

bool
PigPug::variableOccurrencesBoundedBy2(const Word& lhs, const Word& rhs, int maxVarNumber)
{
  const ConstraintMap& constraintMap = constraintStack.back();
  //
  //	We are only concerned with unbounded variables.
  //
  Vector<int> varCount(maxVarNumber + 1);
  for (int& i : varCount)
    i = 0;
  for (int var : lhs)
    {
      if (constraintMap[var].isUnbounded() && ++(varCount[var]) > 2)
	return false;
    }
  for (int var : rhs)
    {
      if (constraintMap[var].isUnbounded() && ++(varCount[var]) > 2)
	return false;
    }
  return true;
}

PigPug::ResultPair
PigPug::getNextUnifier(Subst& unifier, ConstraintMap& constraintMap)
{
  //
  //	Returns (SUCCESS | incompleteness flag, index of next unused variable)
  //	and unifier and constraintMap are filled out on success.
  //	Returns (FAILURE | incompleteness flag, NONE) on failure.
  //
  int nextFreshVariable;
  do
    {
      int result = cycleDetection ?
	runWithCycleDetection(path.empty() ? OK : FAIL) :
	run(path.empty() ? OK : FAIL);
      if (result == FAIL)
	return ResultPair(FAILURE | incompletenessFlag, NONE);
      //
      //	nextFreshVariable == NONE indicates a upperBound violation
      //	was discovered during the construction of the unifier.
      //
      //	If no violation was found, constraintMap is filled out
      //	with entries for all variables, original and fresh.
      //	Variables that appear in the rhs of the solution other than
      //	as self-mappings (which necessarily includes all fresh
      //	variables) will get a constraint calculated by PigPug. Other
      //	original variables are given their original constraint.
      //
      nextFreshVariable = extractUnifier(unifier, constraintMap);
    }
  while (nextFreshVariable == NONE);
  return ResultPair(SUCCESS | incompletenessFlag, nextFreshVariable);
}

ostream& operator<<(ostream& s, const PigPug::Word& word)
{
  for (int index : word)
    s << "x" << index << " ";
  return s;
}
