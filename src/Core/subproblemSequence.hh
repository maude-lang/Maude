/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for sequence of subproblems to be solved sequentially
//
#ifndef _subproblemSequence_hh_
#define _subproblemSequence_hh_
#include "subproblem.hh"

class SubproblemSequence : public Subproblem
{
public:
  SubproblemSequence(Subproblem* first, Subproblem* second);
  ~SubproblemSequence();

  void append(Subproblem* sp);
  bool solve(bool findFirst, RewritingContext& solution);
  bool unificationSolve(bool findFirst, UnificationContext& solution);

private:
  Vector<Subproblem*> sequence;
};

inline
SubproblemSequence::SubproblemSequence(Subproblem* first, Subproblem* second)
  : sequence(2)
{
  sequence[0] = first;
  sequence[1] = second;
}

inline void 
SubproblemSequence::append(Subproblem* sp)
{
  sequence.append(sp);
}

#endif
