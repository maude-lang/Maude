/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for saving stuff on stacks.
//

bool
PigPug::checkUnificand(UnificandStack& unificandStack, int oldVar, int newVar)
{
  //
  //	Check unificand for occurrences of oldVar _from_ current position, and
  //	if necessary push a modified unificand (oldVar |-> newVar) on the stack.
  //	Returns true if a modified unificand is pushed, false if the unificand is
  //	unmodified.
  //
  Unificand& unificand = unificandStack.back();
  int index = unificand.index;
  int unificandLength = unificand.word.size();
  for (int i = index; i < unificandLength; ++i)
    {
      if (unificand.word[i] == oldVar)
	{
	  //
	  //	Another instance of oldVar found - make a new unificand
	  //
	  int remainingUnificand = unificandLength - index;
	  unificandStack.push_back(Unificand());
	  Unificand& newUnificand = unificandStack.back();
	  newUnificand.index = 0;
	  newUnificand.word.resize(remainingUnificand);
	  for (int j = 0; j < remainingUnificand; ++j)
	    {
	      int var = unificand.word[j + index];
	      newUnificand.word[j] = (var == oldVar) ? newVar : var;
	    }
	  return true;
	}
    }
  return false;
}

bool
PigPug::checkUnificand2(UnificandStack& unificandStack, int oldVar, int newVar, int offset)
{
  //
  //	Check unificand for occurrences of oldVar from current position + offset, and
  //	if necessary push a modified unificand (oldVar |-> newVar oldVar) on the stack.
  //	Returns true if a modified unificand is pushed, false if the unificand is
  //	unmodified. We expect that offset will usually be 0 or 1.
  //
  Unificand& unificand = unificandStack.back();
  int index = unificand.index;
  int unificandLength = unificand.word.size();
  for (int i = index + offset; i < unificandLength; ++i)
    {
      if (unificand.word[i] == oldVar)
	{
	  //
	  //	Another instance of oldVar found - make a new unificand
	  //
	  unificandStack.push_back(Unificand());
	  Unificand& newUnificand = unificandStack.back();
	  newUnificand.index = 0;
	  for (int j = index; j < i; ++j)
	    newUnificand.word.append(unificand.word[j]);
	  newUnificand.word.append(newVar);
	  newUnificand.word.append(oldVar);
	  for (int j = i + 1; j < unificandLength; ++j)
	    {
	      int var = unificand.word[j];
	      if (var == oldVar)
		newUnificand.word.append(newVar);
	      newUnificand.word.append(var);
	    }
	  return true;
	}
    }
  return false;
}

bool
PigPug::checkConstraintMap(int knownBigVariable, int otherVariable)
{
  //
  //	This handles the
  //	  knownBigVariable |-> otherVariable knownBigVariable
  //	case.
  //
  const ConstraintMap& constraintMap = constraintStack.back();
  Assert(!(constraintMap[knownBigVariable].hasTheoryConstraint()),
	 "big variable cannot have theory constraint");

  int upperBound = constraintMap[knownBigVariable].getUpperBound();
  if (upperBound == 0)
    return false;  // big variable is unbounded, so no update needed
  Assert(upperBound != 1, "big variable cannot have upperbound of 1");
  //
  //	Push a duplicate constraint map on the stack.
  //
  constraintStack.push_back(constraintMap);
  ConstraintMap& newConstraintMap = constraintStack.back();
  //
  //	big variable has a finite bound; need to decrement it.
  //
  int newUpperBound = upperBound - 1;
  newConstraintMap[knownBigVariable].setUpperBound(newUpperBound);
  //
  //	Check if we need to update other variables bound.
  //	Theory constraints always return an upper-bound of 1 so
  //	never get updated.
  //
  int otherUpperBound = constraintMap[otherVariable].getUpperBound();
  if (otherUpperBound == 0 || otherUpperBound > newUpperBound)
    newConstraintMap[otherVariable].setUpperBound(newUpperBound);
  return true;
}

bool
PigPug::checkConstraintMap(int knownBigVariable, const Unificand& otherSide)
{
  //
  //	This handles the
  //	  knownBigVariable |-> whole other unificand
  //	case.
  //
  const ConstraintMap& constraintMap = constraintStack.back();
  Assert(!(constraintMap[knownBigVariable].hasTheoryConstraint()),
	 "big variable cannot have theory constraint");

  int upperBound = constraintMap[knownBigVariable].getUpperBound();
  if (upperBound == 0)
    return false;  // big variable is unbounded, so no update needed
  Assert(upperBound != 1, "big variable cannot have upperbound of 1");
  //
  //	big variable has a finite bound; need to decrement it.
  //
  int newUpperBound = upperBound - 1;
  //
  //	Check if any variable in the unificand has a looser bound.
  //
  int len = otherSide.word.size();
  for (int i = otherSide.index; i < len; ++i)
    {
      int otherVariable = otherSide.word[i];
      int upperBound = constraintMap[otherVariable].getUpperBound();
      if (upperBound == 0 || upperBound > newUpperBound)
	goto needToUpdate;
    }
  return false;
  
 needToUpdate:
  //
  //	We're going to update constraint(s) so push a duplicate
  //	constraint map on the stack.
  //
  constraintStack.push_back(constraintMap);
  ConstraintMap& newConstraintMap = constraintStack.back();
  for (int i = otherSide.index; i < len; ++i)
    {
      int otherVariable = otherSide.word[i];
      int upperBound = constraintMap[otherVariable].getUpperBound();
      if (upperBound == 0 || upperBound > newUpperBound)
	newConstraintMap[otherVariable].setUpperBound(newUpperBound);
    }
  return true;
}
