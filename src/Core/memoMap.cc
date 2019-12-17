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
//      Implementation for class MemoTable.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      core class definitions"
#include "memoMap.hh"

int
MemoMap::getFromIndex(DagNode* fromDag)
{
  //
  //	We assume that a fromDag is unreduced and therefore we never use
  //	the original in the hash cons table incase it is reduced in place.
  //
  int fromIndex =  dags.insertCopy(fromDag);
  int nrFromDags = toIndices.size();
  if (fromIndex >= nrFromDags)
    {
      toIndices.resize(fromIndex + 1);
      for (int i = nrFromDags; i <= fromIndex; ++i)
	toIndices[i] = NONE;
    }
  return fromIndex;
}
