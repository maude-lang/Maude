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
//      Class for small garbage collected chunks of memory.
//
#ifndef _memoryCell_hh_
#define _memoryCell_hh_
#include "memoryInfo.hh"
#include "memoryBlock.hh"
#include "sort.hh"

class MemoryCell :
  private MemoryBlock,
  public MemoryInfo
{
  NO_COPYING(MemoryCell);
  //
  //	MemoryCell objects can only be created by members of this class.
  //
  MemoryCell(){};

public:
  //
  //	This is the raison d'etre for MemoryCell - fast allocation
  //	of small garbage collected chunks of memory.
  //
  static void* allocateMemoryCell();
  static void* allocateStorage(size_t bytesNeeded);
  //
  //	Normally we just inform the allocator at times when all the non-garbage
  //	is accessible via root pointers.
  //
  static void okToCollectGarbage();
  //
  //	Sometime we want separate the check and the call to the garbage collector
  //	so we can do some extra work just before a garbage collect to make
  //	sure all non-garbage is accessible from root pointers.
  //
  static bool wantToCollectGarbage();
  static void collectGarbage();

  static void setShowGC(bool polarity);

  //
  //	We provide functions for getting access to the MemoryInfo object
  //	corresponding the a memory block we allocated.
  //
  static MemoryInfo* getMemoryInfo(void* p);
  static const MemoryInfo* getMemoryInfo(const void* p);

private:
  enum MemoryManagementParameters
  {
    ARENA_SIZE = 5460,		// arena size in nodes;
    				// 5460 * 6 + 1 + new/malloc_overhead <= 32768 words
    RESERVE_SIZE = 256,		// if fewer nodes left call GC when allowed
    BUCKET_MULTIPLIER = 8,	// to determine bucket size for huge allocations
    MIN_BUCKET_SIZE = 256 * 1024 - 8,	// bucket size for normal allocations
    INITIAL_TARGET = 220 * 1024,	// just under 8/9 of MIN_BUCKET_SIZE
    TARGET_MULTIPLIER = 8	// to determine bucket usage target
  };

  struct Arena;			// arena of fixed size nodes
  struct Bucket;		// bucket of variable length allocations

  static bool showGC;		// do we report GC stats to user
  //
  //	Arena management variables.
  //
  static int nrArenas;
  //static int nrNodesInUse;
  static bool currentArenaPastActiveArena;
  static bool needToCollectGarbage;
  static Arena* firstArena;
  static Arena* lastArena;
  static Arena* currentArena;
  static MemoryCell* nextNode;
  static MemoryCell* endPointer;
  static Arena* lastActiveArena;
  static MemoryCell* lastActiveNode;
  //
  //	Bucket management variables.
  //
  static int nrBuckets;		// total number of buckets
  static Bucket* bucketList;	// linked list of "in use" buckets
  static Bucket* unusedList;	// linked list of unused buckets
  static size_t bucketStorage;	// total amount of bucket storage (bytes)
  static size_t storageInUse;	// amount of bucket storage in use (bytes)
  static size_t target;		// amount to use before GC (bytes)

  static Arena* allocateNewArena();
  static void tidyArenas();
  static MemoryCell* slowNew();
  static void* slowAllocateStorage(size_t bytesNeeded);

#ifdef GC_DEBUG
  static void stompArenas();
  static void checkArenas();
  static void checkInvariant();
  static void dumpMemoryVariables(ostream& s);
#endif
};

struct MemoryCell::Bucket
{
  size_t bytesFree;
  void* nextFree;
  size_t nrBytes;
  Bucket* nextBucket;
}; 

inline void
MemoryCell::okToCollectGarbage()
{
  if (needToCollectGarbage)
    collectGarbage();
}

inline bool
MemoryCell::wantToCollectGarbage()
{
  return needToCollectGarbage;
}

inline void*
MemoryCell::allocateStorage(size_t bytesNeeded)
{
  Assert(bytesNeeded % sizeof(MachineWord) == 0,
	 "only whole machine words can be allocated");
  storageInUse += bytesNeeded;
  if (storageInUse > target)
    needToCollectGarbage = true;
  for (Bucket* b = bucketList; b; b = b->nextBucket)
    {
      if (b->bytesFree >= bytesNeeded)
	{
	  b->bytesFree -= bytesNeeded;
	  void* t = b->nextFree;
	  b->nextFree = static_cast<char*>(t) + bytesNeeded;
	  return t;
	}
    }
  return slowAllocateStorage(bytesNeeded);
}

inline void
MemoryCell::setShowGC(bool polarity)
{
  showGC = polarity;
}

inline void*
MemoryCell::allocateMemoryCell()
{
#ifdef GC_DEBUG
  checkInvariant();
#endif

  MemoryCell* e = endPointer;
  MemoryCell* c = nextNode;
  for (;; ++c)
    {
      if (c == e)
       {
         c = slowNew();
         break;
       }
      if (c->simpleReuse())
	break;
      if (!(c->isMarked()))
	{
	  c->callDtor();
	  break;
	}
      c->clearFlag(MARKED);
    }
  // could clear flags here
  nextNode = c + 1;
  MemoryBlock* b = c;
  return b;
}

inline MemoryInfo*
MemoryCell::getMemoryInfo(void* p)
{
  //
  //	First convert the void* pointer we returned back into
  //	the MemoryBlock* pointer it came from.
  //
  MemoryBlock* b = static_cast<MemoryBlock*>(p);
  //
  //	Then downcast to get a pointer to the full MemoryCell.
  //
  MemoryCell* c = static_cast<MemoryCell*>(b);
  //
  //	Then we can upcast in the return to get a pointer to the MemoryInfo
  //	object associated with this allocation.
  //
  return c;
}
  
inline const MemoryInfo*
MemoryCell::getMemoryInfo(const void* p)
{
  //
  //	First convert the void* pointer we returned back into
  //	the MemoryBlock* pointer it came from.
  //
  const MemoryBlock* b = static_cast<const MemoryBlock*>(p);
  //
  //	Then downcast to get a pointer to the full MemoryCell.
  //
  const MemoryCell* c = static_cast<const MemoryCell*>(b);
  //
  //	Then we can upcast in the return to get a pointer to the MemoryInfo
  //	object associated with this allocation.
  //
  return c;
}

#endif
