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
//      Class for small garbage collected chunks of memory.
//
#ifndef _memoryCell_hh_
#define _memoryCell_hh_
#include "sort.hh"

class MemoryCell
{
  NO_COPYING(MemoryCell);
  //
  //	MemoryCells can only be made via allocateMemoryCell().
  //
  MemoryCell(){};

public:
  //
  //	This is the raison d'etre for MemoryCell - fast allocation
  //	of small garbage collected chunks of memory.
  //
  static MemoryCell* allocateMemoryCell();
  static void* allocateStorage(size_t bytesNeeded);
  static void okToCollectGarbage();
  static void setShowGC(bool polarity);
  //
  //	Access to the unused byte and half word in a raw memory cell.
  //
  int getHalfWord() const;
  void setHalfWord(int hw);
  int getByte() const;
  void setByte(int bt);
  //
  //	Flags 1, 2, 4, 8, 16 and 32 are available for derived classes.
  //	Flags 64 and 128 are reserved for garbage collector.
  //
  bool getFlag(int flag) const;
  void setFlag(int flag);
  void clearFlag(int flag);
  void copySetFlags(int flags, const MemoryCell* other);
  //
  //	Access to garbage collector flags. We can't allow marked flag
  //	to be cleared and it only makes sense to clear the call dtor
  //	flag if we are clearing all flags other than marked.
  //
  bool isMarked() const;
  void setMarked();
  bool needToCallDtor() const;
  void setCallDtor();
  //
  //	This is needed when we reallocate a node that is already in use
  //	(for in-place replacement of a subterm); the marked flag must
  //	be preserved.
  //
  void clearAllExceptMarked();

private:
  enum Sizes
  {
    //
    //	Memory cells have this much extra memory allocated for
    //	derived classes to use. Some uses require at least room
    //	for 5 pointers so this is the minimum value.
    //
    NR_EXTRA_WORDS = 5  // minimum value seems best on average
  };

  enum Flags
  {
    MARKED = 64,	// marked in most recent mark phase
    CALL_DTOR = 128	// call DagNode::~DagNode() before reusing
  };

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

  struct Header;
  struct FullSizeMemoryCell;	// this is what we allocate internally
  struct Arena;			// arena of fixed size nodes
  struct Bucket;		// bucket of variable length allocations

  static bool showGC;		// do we report GC stats to user
  //
  //	Arena management variables.
  //
  static int nrArenas;
  static int nrNodesInUse;
  static bool currentArenaPastActiveArena;
  static bool needToCollectGarbage;
  static Arena* firstArena;
  static Arena* lastArena;
  static Arena* currentArena;
  static FullSizeMemoryCell* nextNode;
  static FullSizeMemoryCell* endPointer;
  static Arena* lastActiveArena;
  static FullSizeMemoryCell* lastActiveNode;
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
  static void collectGarbage();

  void callDtor();

#ifdef GC_DEBUG
  static void stompArenas();
  static void checkArenas();
  static void checkInvariant();
  static void dumpMemoryVariables(ostream& s);
#endif

  MachineWord filler[NR_EXTRA_WORDS];
  static const int dagNodeSize;
};

//
//	Header info associated to but not in each MemoryCell.
//	Header occupies one machine word, but actually we only need
//	two flag bits for memory management. The remaining storage
//	is made available to the MemoryCell.
//
struct MemoryCell::Header
{
  short halfWord;
  Byte byte;
  Ubyte flags;
};

//
//	A FullSizeMemoryCell is a MemoryCell with associated
//	header info.
//
struct MemoryCell::FullSizeMemoryCell : MemoryCell
{
  void initialize();

  Header h;
};

inline void
MemoryCell::FullSizeMemoryCell::initialize()
{
  h.flags = 0;
  h.halfWord = Sort::SORT_UNKNOWN;
}

struct MemoryCell::Bucket
{
  size_t bytesFree;
  void* nextFree;
  size_t nrBytes;
  Bucket* nextBucket;
}; 

inline int
MemoryCell::getHalfWord() const
{
  return (static_cast<const FullSizeMemoryCell*>(this))->h.halfWord;
}

inline void
MemoryCell::setHalfWord(int hw)
{
  (static_cast<FullSizeMemoryCell*>(this))->h.halfWord = hw;
}

inline int
MemoryCell::getByte() const
{
  return (static_cast<const FullSizeMemoryCell*>(this))->h.byte;
}

inline void
MemoryCell::setByte(int bt)
{
  (static_cast<FullSizeMemoryCell*>(this))->h.byte = bt;
}

inline bool
MemoryCell::getFlag(int flag) const
{
  return (static_cast<const FullSizeMemoryCell*>(this))->h.flags & flag;
}

inline void
MemoryCell::setFlag(int flag)
{
  (static_cast<FullSizeMemoryCell*>(this))->h.flags |= flag;
}

inline void
MemoryCell::clearFlag(int flag)
{
  (static_cast<FullSizeMemoryCell*>(this))->h.flags &= ~flag;
}

inline void
MemoryCell::copySetFlags(int flags, const MemoryCell* other)
{
  (static_cast<FullSizeMemoryCell*>(this))->h.flags |= flags & 
    (static_cast<const FullSizeMemoryCell*>(other))->h.flags;
}

inline bool
MemoryCell::isMarked() const
{
  return getFlag(MARKED);
}

inline void
MemoryCell::setMarked()
{
  ++nrNodesInUse;
  setFlag(MARKED);
}

inline void
MemoryCell::clearAllExceptMarked()
{
  clearFlag(~MARKED);
}

inline bool
MemoryCell::needToCallDtor() const
{
  return getFlag(CALL_DTOR);
}

inline void
MemoryCell::setCallDtor()
{
  setFlag(CALL_DTOR);
}

inline void
MemoryCell::okToCollectGarbage()
{
  if (needToCollectGarbage || storageInUse > target)
    collectGarbage();
}

inline void*
MemoryCell::allocateStorage(size_t bytesNeeded)
{
  Assert(bytesNeeded % sizeof(MachineWord) == 0,
	 "only whole machine words can be allocated");
  storageInUse += bytesNeeded;
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

inline void
MemoryCell::callDtor()
{
  //
  //	MemoryCell cannot have a virtual destructor since we want to
  //	avoid a virtual function table pointer. Instead we assume
  //	it is a DagNode and crosscast it. This means that only classes
  //	derived from DagNode can use the call dtor flag.
  //
  (static_cast<DagNode*>(static_cast<void*>(this)))->~DagNode();
}

inline MemoryCell*
MemoryCell::allocateMemoryCell()
{
#ifdef GC_DEBUG
  checkInvariant();
#endif
  FullSizeMemoryCell* e = endPointer;
  for (FullSizeMemoryCell* c = nextNode; c != e; c++)
    {
      if ((c->h.flags & (MARKED | CALL_DTOR)) == 0)
	{
	  c->initialize();
	  nextNode = c + 1;
	  //DebugAdvisory("gc new nondtor case at "<< (void*)(c));
	  Assert(c->h.halfWord == Sort::SORT_UNKNOWN, "bad sort init");
	  return c;
	}
      if ((c->h.flags & MARKED) == 0)
	{
	  c->callDtor();
	  c->initialize();
	  nextNode = c + 1;
	  //DebugAdvisory("gc new dtor case at " << (void*)(c));
	  Assert(c->h.halfWord == Sort::SORT_UNKNOWN, "bad sort init");
	  return c;
	}
      c->clearFlag(MARKED);
    }
  return slowNew();
}

#endif
