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
//      Class for hash cons'd cache of from-dags and to-dags together with their
//	equational rewrite mapping
//
#ifndef _memoMap_hh_
#define _memoMap_hh_
#include "hashConsSet.hh"

class MemoMap
{
public:
  int getFromIndex(DagNode* fromDag);
  DagNode* getToDag(int fromIndex) const;
  void assignToDag(int fromIndex, DagNode* toDag);

private:
  HashConsSet dags;
  Vector<int> toIndices;  // maps from-indices to to-indices
};

inline DagNode*
MemoMap::getToDag(int fromIndex) const
{
  int toIndex = toIndices[fromIndex];
  return toIndex == NONE ? 0 : dags.getCanonical(toIndex);
}

inline void
MemoMap::assignToDag(int fromIndex, DagNode* toDag)
{
  toIndices[fromIndex] = dags.insert(toDag);
}

#endif
