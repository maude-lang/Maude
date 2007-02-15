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
//      Class for cache of pointers to dag nodes.
//
//	Unlike DagNodeSet, we protect our members from garbage collection.
//	One day we might trade cpu for memory by doing a hash-cons copy of inserted stuff.
//
#ifndef _dagNodeCache_hh_
#define _dagNodeCache_hh_
#include "pointerSet.hh"
#include "simpleRootContainer.hh"

class DagNodeCache : private PointerSet, private SimpleRootContainer
{
  NO_COPYING(DagNodeCache);

public:
  DagNodeCache();
  int insert(DagNode* d);
  //PointerSet::cardinality;
  //PointerSet::makeEmpty;
  int dagNode2Index(DagNode* d) const;
  DagNode* index2DagNode(int i) const;

private:
  unsigned int hash(void* pointer) const;
  bool isEqual(void* pointer1, void* pointer2) const;
  void markReachableNodes();
};

#endif
