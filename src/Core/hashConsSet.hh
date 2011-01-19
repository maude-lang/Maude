/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2009 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for hash cons'd set of pointers  to dag nodes.
//
#ifndef _hashConsSet_hh_
#define _hashConsSet_hh_
#include "pointerSet.hh"
#include "simpleRootContainer.hh"

class HashConsSet : private PointerSet, private SimpleRootContainer
{
public:
#ifndef NO_ASSERT
  //
  //	Analysis/debugging of collisions.
  //
  mutable Int64 collisionCounter;
  void collision(void* pointer1,
		 unsigned int rawHashValue1, 
		 void* pointer2,
		 unsigned int rawHashValue2,
		 unsigned int tableSize,
		 unsigned int disputedSlot) const;
  HashConsSet() { collisionCounter = 0;}
#endif

  //
  //	Returns an index that is unique to dag node.
  //
  int insert(DagNode* d);
  //
  //	As above, but unreduced eager dag nodes are always
  //	copied and can never become canonical.
  //
  int insertCopy(DagNode* d);
  //
  //	Returns the canonical dag node for the index.
  //
  DagNode* getCanonical(int index) const;

private:
  //
  //	For PointerSet call backs.
  //
  unsigned int hash(void* pointer) const;
  bool isEqual(void* pointer1, void* pointer2) const;
  //
  //	To protect hash cons'd dagnodes from garbage collection.
  //
  void markReachableNodes();
};

inline DagNode*
HashConsSet::getCanonical(int index) const
{
  return static_cast<DagNode*>(index2Pointer(index));
}

#endif
