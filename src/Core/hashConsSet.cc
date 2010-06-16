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
//      Implementation for class DagNodeSet
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
#include "hashConsSet.hh"

int
HashConsSet::insert(DagNode* d)
{
  unsigned int hashValue = d->getHashValue();
  int index = pointer2Index(d, hashValue);
  return (index == NONE) ? PointerSet::insert(d->symbol()->makeCanonical(d, this), hashValue) : index;
}

unsigned int
HashConsSet::hash(void* /* pointer */) const
{
  CantHappen("should never be called");
  return 0;
}

bool
HashConsSet::isEqual(void* pointer1, void* pointer2) const
{
  DagNode* d1 = static_cast<DagNode*>(pointer1);
  DagNode* d2 = static_cast<DagNode*>(pointer2);
  return d1->equal(d2);
}

#ifndef NO_ASSERT
//
//	Analysis/debugging of collisions.
//
void
HashConsSet::collision(void* pointer1,
		       unsigned int rawHashValue1, 
		       void* pointer2,
		       unsigned int rawHashValue2,
		       unsigned int tableSize,
		       unsigned int disputedSlot) const
{
  ++collisionCounter;
  /*
  DebugAdvisory("HashConsSet::collision() new dag = " << static_cast<DagNode*>(pointer1) <<
		" hash code = " << rawHashValue1 <<
		" old dag = " << static_cast<DagNode*>(pointer2) <<
		" hash code = " << rawHashValue2 <<
		" hash table size = " << tableSize <<
		" used = " << cardinality() <<
		" slot = " << disputedSlot);
  */
}
		
#endif
