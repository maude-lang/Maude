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
//	Class for constraints on variables occurring in word problems.
//
#ifndef _variableConstraint_hh_
#define _variableConstraint_hh_

class VariableConstraint
{
  //
  //	Variable may or may not be able to take the empty word.
  //	A variable may have a bound on the length of the word
  //	it is assigned OR it may have a theory constraint (which
  //	implies a bound of 1).
  //
public:
  //
  //	The default is that a variable cannot take the empty
  //	word but has no other constraints.
  //
  VariableConstraint();
  //
  //	Allow the variable to take the empty word.
  //
  void setTakeEmpty();
  //
  //	Set an upper-bound on the length of word a variable may take.
  //	This will usually be 1, but could be another small positive
  //	integer. An upper-bound of 0 means no upper-bound.
  //
  void setUpperBound(int upperBound);
  //
  //	Set a theory constraint on a variable. This implies
  //	an upper bound of one, and furthermore, two variables
  //	constrained by different theory indices cannot be made equal.
  //	Theory indices are natural numbers starting at 0.
  //
  void setTheoryConstraint(int theoryIndex);

public:
  bool none() const;
  bool operator==(const VariableConstraint& other) const;
  
  bool intersect(VariableConstraint other);

  //
  //	New tests.
  //
  bool isUnbounded() const;
  int getUpperBound() const;  // 0 means no upper-bound
  bool hasTheoryConstraint() const;
  int getTheoryConstraint() const;  // NONE means no theory constraint
  bool canTakeEmpty() const;

private:
  //
  //	Constraint is stored in a 32-bit unsigned int
  //    (30-bit upper-bound or theory index) (theory flag) (take empty flag)
  //
  enum Special
    {
      TAKE_EMPTY_FLAG = 1,
      THEORY_FLAG = 2,
      INDEX_SHIFT = 2
    };

  uint32_t constraint;

  friend ostream& operator<<(ostream& s, VariableConstraint c);  // HACK
};

inline
VariableConstraint::VariableConstraint()
{
  constraint = 0;  // TAKE_EMPTY_FLAG, THEORY_FLAG = 0; upper-bound = 0 (unbounded)
}

inline void
VariableConstraint::setTakeEmpty()
{
  constraint |= TAKE_EMPTY_FLAG;
}

inline void
VariableConstraint::setUpperBound(int upperBound)
{
  constraint = (constraint & TAKE_EMPTY_FLAG) | (upperBound << INDEX_SHIFT);
}

inline void
VariableConstraint::setTheoryConstraint(int theoryIndex)
{
  constraint = (constraint & TAKE_EMPTY_FLAG) | THEORY_FLAG | (theoryIndex << INDEX_SHIFT);
}

inline bool
VariableConstraint::canTakeEmpty() const
{
  return constraint & TAKE_EMPTY_FLAG;
}

inline bool
VariableConstraint::hasTheoryConstraint() const
{
  return constraint & THEORY_FLAG;
}

inline bool
VariableConstraint::isUnbounded() const
{
  //
  //	True if THEORY_FLAG clear and bound = 0; i.e.
  //	everything except possibly TAKE_EMPTY_FLAG is 0.
  //
  return (constraint & ~TAKE_EMPTY_FLAG) == 0;
}

inline int
VariableConstraint::getUpperBound() const
{
  return (constraint & THEORY_FLAG) ? 1 : (constraint >> INDEX_SHIFT);
}

inline int
VariableConstraint::getTheoryConstraint() const
{
  return (constraint & THEORY_FLAG) ? (constraint >> INDEX_SHIFT) : NONE;
}

inline bool
VariableConstraint::operator==(const VariableConstraint& other) const
{
  return constraint == other.constraint;
}

ostream& operator<<(ostream& s, VariableConstraint c);

#endif
