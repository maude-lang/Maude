/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2008 SRI International, Menlo Park, CA 94025, USA.

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
//	Base class for delayed matching subproblems.
//	The idea is that delayed problems can be batched up. They therefore
//	need a flag to record if they have aleady been solved as part of
//	a previous batch.
//
#ifndef _delayedSubproblem_hh_
#define _delayedSubproblem_hh_

class DelayedSubproblem
{
public:
  virtual ~DelayedSubproblem() {}  // deep
  virtual bool delayedSolve(bool findFirst, RewritingContext& solution) { CantHappen("Not implemented"); return false; }

  bool solved() const;
  void setSolvedSatus(bool flag);

private:
  bool solvedFlag;
};

inline bool
DelayedSubproblem::solved() const
{
  return solvedFlag;
}

inline void
DelayedSubproblem::setSolvedSatus(bool flag)
{
  solvedFlag = flag;
}

#endif
