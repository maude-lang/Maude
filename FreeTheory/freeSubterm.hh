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
//	Class for subterms occurring under free skeleton.
//
#ifndef _freeSubterm_hh_
#define _freeSubterm_hh_

class FreeSubterm
{
public:
  enum Type
  {
    FREE_VARIABLE,
    BOUND_VARIABLE,
    GROUND_ALIEN,
    NON_GROUND_ALIEN
  };

  FreeSubterm() {}  // needed for Vector<FreeSubterm>
  FreeSubterm(Type type, int positionIndex);

  Type type() const;
  int positionIndex() const;

private:
  Type subtermType;
  int subtermPositionIndex;
};

inline
FreeSubterm::FreeSubterm(Type type, int positionIndex)
{
  subtermType = type;
  subtermPositionIndex = positionIndex;
}

inline FreeSubterm::Type
FreeSubterm::type() const
{
  return subtermType;
}

inline int
FreeSubterm::positionIndex() const
{
  return subtermPositionIndex;
}

#endif
