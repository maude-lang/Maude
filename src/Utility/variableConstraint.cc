/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class VariableConstraint.
//
#include "macros.hh"
#include "variableConstraint.hh"

bool
VariableConstraint::intersect(VariableConstraint other)
{
  //
  //	We check if our constraint is compatible with some other
  //	constraint. If not we return false.
  //	If so we return true and our constraint is updated to the intersection;
  //	i.e. the weakest constraint that satisfies both parent constraints.
  //
  if (hasTheoryConstraint())
    {
      if (other.hasTheoryConstraint())
        {
	  if (getTheoryConstraint() == other.getTheoryConstraint())
	    {
	      //
	      //	Really we are just conjuncting TAKE_EMPTY_FLAG
	      //	flags because the other bits are equal.
	      //
	      constraint &= other.constraint;
	      return true;
	    }
	  //
	  //	Different theories mean unsatisfiable constraint.
	  //
	  return false;
        }
      //
      //	Theory constraint is always tighter that upperBound
      //	constraint so just conjunct in TAKE_EMPTY_FLAG.
      //
      constraint &= (other.constraint | ~TAKE_EMPTY_FLAG);
      return true;
   }
  if (other.hasTheoryConstraint())
    {
      //
      //	Theory constraint is always tighter that upperBound
      //	constraint so just conjunct in TAKE_EMPTY_FLAG.
      //
      constraint = other.constraint & (constraint | ~TAKE_EMPTY_FLAG);
      return true;
    }
  //
  //	Take the tighter upperBound and the conjunction of the TAKE_EMPTY_FLAGs.
  //
  int upperBound = constraint >> INDEX_SHIFT;
  int otherUpperBound = other.constraint >> INDEX_SHIFT;
  if (otherUpperBound != 0 && (upperBound == 0 || otherUpperBound < upperBound))
    upperBound = otherUpperBound;
  constraint = (upperBound << INDEX_SHIFT) |
    (TAKE_EMPTY_FLAG & constraint & other.constraint);
  return true;
}

ostream&
operator<<(ostream& s, VariableConstraint c)
{
  if (c.hasTheoryConstraint())
    s << "theory=" << c.getTheoryConstraint();
  else
    s << "bound=" << c.getUpperBound();
  s << " takeEmpty=" << c.canTakeEmpty();
  return s;
}
