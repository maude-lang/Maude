/*

    This file is part of the Maude 3 interpreter.

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
//	Classic to solve sequence assignment problems arising from single elementary
//	linear associative and AU unification problems.
//

#ifndef _sequenceAssignment_hh_
#define _sequenceAssignment_hh_

class SequenceAssignment
{
public:
  SequenceAssignment(int nrLhsVariables, int nrRhsVariables);

  void setLhsBound(int varIndex, int bound);
  void setRhsBound(int varIndex, int bound);
  //
  //	Solutions are recorded as a sequence of moves on a grid. The number
  //	of free variables needed to express the correpsonding unifier is 1 plus
  //	the number of moves.
  //
  enum Move
    {
      NEXT_RIGHT = 1,
      NEXT_LEFT = 2,
      NEXT_BOTH = NEXT_RIGHT | NEXT_LEFT
    };

  static int leftDelta(int move);
  static int rightDelta(int move);

  typedef Vector<int> Solution;

  bool findNextSolution(bool findFirst);
  const Solution& getSolution() const;

private:
  typedef Vector<int> IntVec;

  void computeBoundSum(const IntVec& bounds, IntVec& boundSum);
  bool checkAndMakeMove(int move, int& lIndex, int& rIndex);
  int unmakeMove(int moveIndex, int& lIndex, int& rIndex);
  
  IntVec lhsBounds;
  IntVec rhsBounds;
  IntVec lhsBoundSum;
  IntVec rhsBoundSum;
  IntVec lhsCount;
  IntVec rhsCount;

  Solution currentSolution;
};

inline void
SequenceAssignment::setLhsBound(int varIndex, int bound)
{
  lhsBounds[varIndex] = bound;
}

inline void
SequenceAssignment::setRhsBound(int varIndex, int bound)
{
  rhsBounds[varIndex] = bound;
}

inline const SequenceAssignment::Solution&
SequenceAssignment::getSolution() const
{
  return currentSolution;
}

inline int
SequenceAssignment::leftDelta(int move)
{
  return move >> 1;
}

inline int
SequenceAssignment::rightDelta(int move)
{
  return move & 1;
}

#endif
