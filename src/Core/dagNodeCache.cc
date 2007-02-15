/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2006 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class DagNodeCache
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	core class definitions
#include "dagNodeCache.hh"

DagNodeCache::DagNodeCache()
{
}

int
DagNodeCache::insert(DagNode* d)
{
  return PointerSet::insert(d, d->getHashValue());
}

int
DagNodeCache::dagNode2Index(DagNode* d) const
{
  return pointer2Index(d, d->getHashValue());
}

DagNode*
DagNodeCache::index2DagNode(int i) const
{
  return static_cast<DagNode*>(index2Pointer(i));
}

unsigned int
DagNodeCache::hash(void* /* pointer */) const
{
  CantHappen("should never be called");
  return 0;
}

bool
DagNodeCache::isEqual(void* pointer1, void* pointer2) const
{
  DagNode* d1 = static_cast<DagNode*>(pointer1);
  DagNode* d2 = static_cast<DagNode*>(pointer2);
  return d1->equal(d2);
}

void
DagNodeCache::markReachableNodes()
{
  int nrStates = cardinality();
  for (int i = 0; i < nrStates; i++)
    index2DagNode(i)->mark();
}
