/*

    This file is part of the Maude 3 interpreter.

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
//	Class for table of positions in free skeletons.
//
//	It is maintained as a digital search tree with each node containing
//	the full path to nodes as a Vector for fast index to path conversion.
//
#ifndef _freePositionTable_hh_
#define _freePositionTable_hh_

class FreePositionTable
{
public:
  const Vector<int>& index2Position(int index) const;
  int position2Index(const Vector<int>& position);
  int parentIndex(int index) const;

private:
  struct Pair
  {
    Vector<int> position;
    Vector<int> arcs;
  };

  int build(const Vector<int>& position, int start);

  Vector<Pair> table;
};

inline const Vector<int>&
FreePositionTable::index2Position(int index) const
{
  return table[index].position;
}

#endif
