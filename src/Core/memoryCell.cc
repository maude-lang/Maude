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
//      Implementation for base class MemoryCell
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

#include "symbol.hh"
#include "dagNode.hh"
#include "rootContainer.hh"

#include "memoryCell.hh"
//#include "memoryCellNew.hh"

const int
MemoryCell::dagNodeSize =
sizeof(MemoryCell::FullSizeMemoryCell) / sizeof(MachineWord);

struct MemoryCell::Arena
{
  union
  {
    Arena* nextArena;
    Int64 alignmentDummy;  // force 8 byte alignment for FullSizeMemoryCell objects
  };
  MachineWord storage[ARENA_SIZE * dagNodeSize];
  FullSizeMemoryCell* firstNode();
};

inline MemoryCell::FullSizeMemoryCell*
MemoryCell::Arena::firstNode()
{
  return reinterpret_cast<FullSizeMemoryCell*>(storage);
}

bool MemoryCell::showGC = false;
//
//	Arena management variables.
//
int MemoryCell::nrArenas = 0;
int MemoryCell::nrNodesInUse = 0;
bool MemoryCell::currentArenaPastActiveArena = true;
bool MemoryCell::needToCollectGarbage = false;
MemoryCell::Arena* MemoryCell::firstArena = 0;
MemoryCell::Arena* MemoryCell::lastArena = 0;
MemoryCell::Arena* MemoryCell::currentArena = 0;
MemoryCell::FullSizeMemoryCell* MemoryCell::nextNode = 0;
MemoryCell::FullSizeMemoryCell* MemoryCell::endPointer = 0;
MemoryCell::Arena* MemoryCell::lastActiveArena = 0;
MemoryCell::FullSizeMemoryCell* MemoryCell::lastActiveNode = 0;
//
//	Bucket management variables.
//
int MemoryCell::nrBuckets = 0;
MemoryCell::Bucket* MemoryCell::bucketList = 0;
MemoryCell::Bucket* MemoryCell::unusedList = 0;
size_t MemoryCell::bucketStorage = 0;
size_t MemoryCell::storageInUse = 0;
size_t MemoryCell::target = INITIAL_TARGET;

MemoryCell::Arena*
MemoryCell::allocateNewArena()
{
#ifdef GC_DEBUG
  cerr << "allocateNewArena()\n";
  dumpMemoryVariables(cerr);
#endif
  Arena* a = new Arena;
  a->nextArena = 0;
  if (lastArena == 0)
    firstArena = a;
  else
    lastArena->nextArena = a;
  lastArena = a;
  FullSizeMemoryCell* d = a->firstNode();
  for (int i = 0; i < ARENA_SIZE; i++, d++)
    d->h.flags = 0;
  ++nrArenas;
  return a;
}

MemoryCell*
MemoryCell::slowNew()
{
#ifdef GC_DEBUG
  cerr << "slowNew()\n";
  dumpMemoryVariables(cerr);
#endif
  for(;;)
    {
      if (currentArena == 0)
	{
	  //
	  //	Allocate first arena.
	  //
	  currentArena = allocateNewArena();
	  FullSizeMemoryCell* d = currentArena->firstNode();
	  endPointer = d + ARENA_SIZE - RESERVE_SIZE;
	  nextNode = d + 1;
	  Assert(d->h.flags == 0, "flags not cleared");
	  d->initialize();
	  return d;
	}
      Arena* a = currentArena->nextArena;
      if (a == 0)
	{
	  needToCollectGarbage = true;
	  FullSizeMemoryCell* e = currentArena->firstNode() + ARENA_SIZE;
	  if (endPointer != e)
	    {
	      //
	      //	Use up reserve.
	      //
	      nextNode = endPointer;  // nextNode is invalid where we are called
	      endPointer = e;
	    }
	  else
	    {
	      //
	      //	Allocate a new arena.
	      //
	      if (currentArena == lastActiveArena)
		currentArenaPastActiveArena = true;
	      currentArena = allocateNewArena();
	      FullSizeMemoryCell* d = currentArena->firstNode();
	      endPointer = d + ARENA_SIZE;
	      nextNode = d + 1;
	      Assert(d->h.flags == 0, "flags not cleared");
	      d->initialize();
	      return d;
	    }
	}
      else
	{
	  //
	  //	Use next arena.
	  //
	  if (currentArena == lastActiveArena)
	    currentArenaPastActiveArena = true;
	  currentArena = a;
	  nextNode = a->firstNode();
	  endPointer = nextNode +
	    ((a->nextArena != 0) ? ARENA_SIZE : ARENA_SIZE - RESERVE_SIZE);
	}
      //
      //	Now execute lazy sweep to actually find a free location
      //
#ifdef GC_DEBUG
      checkInvariant();
#endif
      FullSizeMemoryCell* e = endPointer;
      for (FullSizeMemoryCell* d = nextNode; d != e; d++)
	{
	  if ((d->h.flags & (MARKED | CALL_DTOR)) == 0)
	    {
	      nextNode = d + 1;
	      d->initialize();
	      return d;
	    }
	  if ((d->h.flags & MARKED) == 0)
	    {
	      d->callDtor();
	      nextNode = d + 1;
	      d->initialize();
	      return d;
	    }
	  d->h.flags &= ~MARKED;
	}
    }
}

void*
MemoryCell::slowAllocateStorage(size_t bytesNeeded)
{
  Bucket* p = 0;
  for (Bucket* b = unusedList; b; p = b, b = b->nextBucket)
    {
      if (b->bytesFree >= bytesNeeded)
	{
	  //
	  //	Move b from unused list to bucket list
	  //
	  if (p == 0)
	    unusedList = b->nextBucket;
	  else
	    p->nextBucket = b->nextBucket;
	  b->nextBucket = bucketList;
	  bucketList = b;
	  //
	  //	Allocate storage from b
	  //
	  b->bytesFree -= bytesNeeded;
	  void* t = b->nextFree;
	  b->nextFree = static_cast<char*>(t) + bytesNeeded;
	  return t;
	}
    }
  //
  //	Create new bucket
  //
  size_t size = BUCKET_MULTIPLIER * bytesNeeded;
  if (size < MIN_BUCKET_SIZE)
    size = MIN_BUCKET_SIZE;

  Bucket* b = static_cast<Bucket*>(operator new[](size));
  ++nrBuckets;
  void* t = b + 1;
  size_t nrBytes = size - sizeof(Bucket);
  bucketStorage += nrBytes;
  b->nrBytes = nrBytes;
  b->bytesFree = nrBytes - bytesNeeded;
  b->nextFree = static_cast<char*>(t) + bytesNeeded;
  b->nextBucket = bucketList;
  bucketList = b;
  return t;
}

void
MemoryCell::tidyArenas()
{
#ifdef GC_DEBUG
  cerr << "tidyArenas()\n";
  dumpMemoryVariables(cerr);
#endif
  //
  //	Tidy up lazy sweep phase - clear marked flags and call dtors
  //	where necessary.
  //
  Arena* newLastActiveArena = currentArena;
  FullSizeMemoryCell* newLastActiveNode = nextNode - 1;  // nextNode never points to first node

  if (!currentArenaPastActiveArena)
    {
      //
      //	First tidy arenas from current up to lastActive.
      //
    FullSizeMemoryCell* d = nextNode;
    Arena* c = currentArena;
    for (; c != lastActiveArena; c = c->nextArena, d = c->firstNode())
      {
	FullSizeMemoryCell* e = c->firstNode() + ARENA_SIZE;
	for (; d != e; d++)
	  {
	    if (d->h.flags & MARKED)
	      {
		newLastActiveArena = c;
		newLastActiveNode = d;
		d->h.flags &= ~MARKED;
	      }
	    else
	      {
		if (d->h.flags & CALL_DTOR)
		  d->callDtor();
		d->h.flags = 0;
	      }
	  }
      }
    //
    //	Now tidy lastActiveArena from d upto and including lastActiveNode.
    //
    FullSizeMemoryCell* e = lastActiveNode;
    for(; d <= e; d++)
      {
	if (d->h.flags & MARKED)
	  {
	    newLastActiveArena = c;
	    newLastActiveNode = d;
	    d->h.flags &= ~MARKED;
	  }
	else
	  {
	    if (d->h.flags & CALL_DTOR)
	      d->callDtor();
	    d->h.flags = 0;
	  }
      }
    }

  lastActiveArena = newLastActiveArena;
  lastActiveNode = newLastActiveNode;
}

void
MemoryCell::collectGarbage()
{
  if (firstArena == 0)
    return;
  tidyArenas();
#ifdef GC_DEBUG
  checkArenas();
#endif
  //
  //	Mark phase
  //
  nrNodesInUse = 0;
  size_t oldStorageInUse = storageInUse;
  Bucket* b = bucketList;
  bucketList = unusedList;
  unusedList = 0;
  storageInUse = 0;

  RootContainer::markPhase();

  unusedList = b;
  for (; b; b = b->nextBucket)
    {
      b->bytesFree = b->nrBytes;
      b->nextFree = b + 1;  // reset
    }
  size_t newTarget = TARGET_MULTIPLIER * storageInUse;
  if (target < newTarget)
    target = newTarget;
  //
  //	Calculate if we should allocate more arenas to avoid an early gc.
  //
  int nrNodes = nrArenas * ARENA_SIZE;
  if (showGC)
    {
      cout << "Arenas: " << nrArenas <<
	"\tNodes: " << nrNodes <<
	//	"\tIn use: " << nrNodesInUse <<
	//	"\tCollected: " << reclaimed << 
	"\tNow: " << nrNodesInUse <<
	"\nBuckets: " << nrBuckets <<
	"\tBytes: " << bucketStorage <<
	"\tIn use: " << oldStorageInUse <<
	"\tCollected: " << oldStorageInUse - storageInUse <<
	"\tNow: " << storageInUse << '\n';
    }
  //
  //	Allocate new arenas so that we have at least 50% of nodes unused.
  //
  int neededArenas = ceilingDivision(2 * nrNodesInUse, ARENA_SIZE);
  while (nrArenas < neededArenas)
    (void) allocateNewArena();
  //
  //	Reset node stuff.
  //
  currentArenaPastActiveArena = false;
  currentArena = firstArena;
  nextNode = currentArena->firstNode();
  endPointer = nextNode +
    ((firstArena->nextArena != 0) ? ARENA_SIZE : ARENA_SIZE - RESERVE_SIZE);
  needToCollectGarbage = false;
#ifdef GC_DEBUG
  // stompArenas();
  cerr << "end of GC\n";
  dumpMemoryVariables(cerr);
#endif
}

#ifdef GC_DEBUG
void
MemoryCell::stompArenas()
{
  for (Arena* a = firstArena; a != 0; a = a->nextArena)
    {
      FullSizeMemoryCell* d = a->firstNode();
      for (int i = 0; i < ARENA_SIZE; i++, d++)
	{
	  if (!(d->h.flags & MARKED) && !(d->h.flags & CALL_DTOR))
	    d->topSymbol = reinterpret_cast<Symbol*>(0x33);
	}
    }
}

void
MemoryCell::checkArenas()
{
  int n = 0;
  for (Arena* a = firstArena; a != 0; a = a->nextArena, n++)
    {
      FullSizeMemoryCell* d = a->firstNode();
      for (int i = 0; i < ARENA_SIZE; i++, d++)
	{
	  if (d->h.flags & MARKED)
	    {
	      cerr << "checkArenas(): MARKED DagNode! arena = " <<
		n << " node = " << i << '\n';
	    }
	}
    }
}

void
MemoryCell::checkInvariant()
{
  int n = 0;
  for (Arena* a = firstArena;; a = a->nextArena, n++)
    {
      FullSizeMemoryCell* d = a->firstNode();
      int bound = (a == currentArena) ? nextNode - d : ARENA_SIZE;
      for (int i = 0; i < bound; i++, d++)
	{
	  if (d->h.flags & MARKED)
	    {
	      cerr << "checkInvariant() : MARKED DagNode! arena = " <<
		n << " node = " << i << '\n';
	    }
	}
      if (a == currentArena)
	break;
    }
}

void
MemoryCell::dumpMemoryVariables(ostream& s)
{
  s << "nrArenas = " << nrArenas <<
    "\nnrNodesInUse = " << nrNodesInUse <<
    "\ncurrentArenaPastActiveArena = " << currentArenaPastActiveArena <<
    "\nneedToCollectGarbage = " << needToCollectGarbage <<
    "\nfirstArena = " << firstArena <<
    "\nlastArena = " << lastArena <<
    "\ncurrentArena = " << currentArena <<
    "\nnextNode = " << static_cast<void*>(nextNode) <<
    "\nendPointer = " << static_cast<void*>(endPointer) <<
    "\nlastActiveArena = " << lastActiveArena <<
    "\nlastActiveNode = " << static_cast<void*>(lastActiveNode) <<
    "\n\n";
}
#endif
