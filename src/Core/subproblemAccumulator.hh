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
//	Class for accumulating sequeces of subproblems
//
#ifndef _subproblemAccumulator_hh_
#define _subproblemAccumulator_hh_
#include "subproblemSequence.hh"

class SubproblemAccumulator
{
public:
  SubproblemAccumulator();
  ~SubproblemAccumulator();

  void add(Subproblem* sp);
  Subproblem* extractSubproblem();

private:
  Subproblem* first;
  SubproblemSequence* sequence;
};

inline
SubproblemAccumulator::SubproblemAccumulator()
{
  first = 0;
  sequence = 0;
}

inline
SubproblemAccumulator::~SubproblemAccumulator()
{
  if (sequence != 0)
    delete sequence;
  else
    delete first;
}

inline void
SubproblemAccumulator::add(Subproblem* sp)
{
  if (sp != 0)
    {
      if (first == 0)
	first = sp;
      else if (sequence == 0)
	sequence = new SubproblemSequence(first, sp);
      else
	sequence->append(sp);
    }
}

inline Subproblem*
SubproblemAccumulator::extractSubproblem()
{
  Subproblem* r = 0;
  if (sequence != 0)
    {
      r = sequence;
      sequence = 0;
      first = 0;
    }
  else if (first != 0)
    {
      r = first;
      first = 0;
    }
  return r;
}

#endif
