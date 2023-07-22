/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
#include "cmath"
#include <sys/resource.h>

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

static constexpr double SMALL_MODEL_SLOP = 8.0;
static constexpr double BIG_MODEL_SLOP = 2.0;
static constexpr size_t LOWER_BOUND = 4 * 1024 * 1024;  // use small model if <= 4 million nodes
static constexpr size_t UPPER_BOUND = 32 * 1024 * 1024;  // use big model if >= 32 million nodes

struct MemoryCell::Arena
{
  union
  {
    Arena* nextArena;
    int_fast64_t alignmentDummy;  // force 8 byte alignment for MemoryCell objects
  };
  MemoryCell storage[ARENA_SIZE];
  MemoryCell* firstNode();
};

inline MemoryCell*
MemoryCell::Arena::firstNode()
{
  return storage;
}

bool MemoryCell::showGC = false;
bool MemoryCell::showResourcesFlag = false;
int_fast64_t MemoryCell::earlyQuit = 0;

//
//	Arena management variables.
//
int MemoryCell::nrArenas = 0;
bool MemoryCell::currentArenaPastActiveArena = true;
bool MemoryCell::needToCollectGarbage = false;
MemoryCell::Arena* MemoryCell::firstArena = 0;
MemoryCell::Arena* MemoryCell::lastArena = 0;
MemoryCell::Arena* MemoryCell::currentArena = 0;
MemoryCell* MemoryCell::nextNode = 0;
MemoryCell* MemoryCell::endPointer = 0;
MemoryCell::Arena* MemoryCell::lastActiveArena = 0;
MemoryCell* MemoryCell::lastActiveNode = 0;
//
//	Bucket management variables.
//
int MemoryCell::nrBuckets = 0;
MemoryCell::Bucket* MemoryCell::bucketList = 0;
MemoryCell::Bucket* MemoryCell::unusedList = 0;
size_t MemoryCell::bucketStorage = 0;
size_t MemoryCell::storageInUse = 0;
size_t MemoryCell::target = INITIAL_TARGET;

pair<double, const char*>
MemoryCell::memConvert(uint_fast64_t nrBytes)
{
  //
  //	Convert bytes to a more convenient unit.
  //
  double value = nrBytes;
  value /= 1024.0;
  if (value > 1024.0)
    {
      value /= 1024.0;
      if (value > 1024.0)
	{
	  value /= 1024.0;
	  return {value, "GB"};
	}
      return {value, "MB"};
    }
  return {value, "KB"};
}

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
  MemoryCell* d = a->firstNode();
  for (int i = 0; i < ARENA_SIZE; i++, d++)
    d->clearAllFlags();
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
	  MemoryCell* d = currentArena->firstNode();
	  endPointer = d + ARENA_SIZE - RESERVE_SIZE;
	  //Assert(d->h.flags == 0, "flags not cleared");
	  return d;
	}
      Arena* a = currentArena->nextArena;
      if (a == 0)
	{
	  needToCollectGarbage = true;
	 MemoryCell* e = currentArena->firstNode() + ARENA_SIZE;
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
	      MemoryCell* d = currentArena->firstNode();
	      endPointer = d + ARENA_SIZE;
	      //Assert(d->h.flags == 0, "flags not cleared");
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
      MemoryCell* e = endPointer;
      for (MemoryCell* d = nextNode; d != e; d++)
	{
	  if (d->simpleReuse())
	    {
	      return d;
	    }
	  if (!(d->isMarked()))
	    {
	      d->callDtor();
	      return d;
	    }
	  d->clearFlag(MARKED);
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
  MemoryCell* newLastActiveNode = nextNode - 1;  // nextNode never points to first node

  if (!currentArenaPastActiveArena)
    {
      //
      //	First tidy arenas from current up to lastActive.
      //
      MemoryCell* d = nextNode;
      Arena* c = currentArena;
      for (; c != lastActiveArena; c = c->nextArena, d = c->firstNode())
	{
	  MemoryCell* e = c->firstNode() + ARENA_SIZE;
	  for (; d != e; d++)
	    {
	      if (d->isMarked())
		{
		  newLastActiveArena = c;
		  newLastActiveNode = d;
		  d->clearFlag(MARKED);
		}
	      else
		{
		  if (d->needToCallDtor())
		    d->callDtor();
		  d->clearAllFlags();
		}
	    }
	}
      //
      //	Now tidy lastActiveArena from d upto and including lastActiveNode.
      //
      MemoryCell* e = lastActiveNode;
      for(; d <= e; d++)
	{
	  if (d->isMarked())
	    {
	      newLastActiveArena = c;
	      newLastActiveNode = d;
	      d->clearFlag(MARKED);
	    }
	  else
	    {
	      if (d->needToCallDtor())
		d->callDtor();
	      d->clearAllFlags();
	    }
	}
    }
  lastActiveArena = newLastActiveArena;
  lastActiveNode = newLastActiveNode;
}

void
MemoryCell::collectGarbage()
{
  static int_fast64_t gcCount = 0;

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
  ++gcCount;
  if (showGC)
    {
      cout << "\nGarbage collection: " << gcCount << "\n";
      {
	pair<double, const char*> ap = memConvert(nrNodes * sizeof(MemoryCell));
	pair<double, const char*> np = memConvert(nrNodesInUse * sizeof(MemoryCell));
	cout << "Arenas: " << nrArenas << "\tNodes: " << nrNodes << " (" << ap.first << " " << ap.second << ")" <<
	  "\tNow: " << nrNodesInUse << " (" << np.first << " " << np.second << ")\n";
      }
      {
	pair<double, const char*> bp = memConvert(bucketStorage);
	pair<double, const char*> op = memConvert(oldStorageInUse);
	pair<double, const char*> cp = memConvert(oldStorageInUse - storageInUse);
	pair<double, const char*> up = memConvert(storageInUse);
	cout << "Buckets: " << nrBuckets << "\tBytes: " << bucketStorage << " (" << bp.first << " " << bp.second << ")" <<
	  "\tIn use: " << oldStorageInUse << " (" << op.first << " " << op.second << ")" <<
	  "\tCollected: " << oldStorageInUse - storageInUse << " (" << cp.first << " " << cp.second << ")" <<
	  "\tNow: " << storageInUse << " (" << up.first << " " << up.second << ")\n";
      }
      maybeShowResources();
    }
  if (gcCount == earlyQuit)
    exit(0);
  double slopFactor = BIG_MODEL_SLOP;  // if we are using lots of nodes
  if (nrNodesInUse <= LOWER_BOUND)
    slopFactor = SMALL_MODEL_SLOP;  // if we are using few nodes
  else if (nrNodesInUse < UPPER_BOUND)
    slopFactor += ((UPPER_BOUND - nrNodesInUse) * (SMALL_MODEL_SLOP - BIG_MODEL_SLOP)) / (UPPER_BOUND - LOWER_BOUND);
  //
  //	Allocate new arenas so that we have at least slopFactor times the actually used nodes.
  //
  int neededArenas = ceil((slopFactor * nrNodesInUse) / ARENA_SIZE);
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

void
MemoryCell::showResources(ostream& s)
{
  rusage usage;
  if (getrusage(RUSAGE_SELF, &usage) == 0)
    {
      double userTime = usage.ru_utime.tv_usec / 1000000.0 + usage.ru_utime.tv_sec;
      double sysTime = usage.ru_stime.tv_usec / 1000000.0 + usage.ru_stime.tv_sec;
      pair<double, const char*> p = memConvert(1024 * static_cast<uint_fast64_t>(usage.ru_maxrss));
      s << "Time: " << userTime << "s user / " <<  sysTime << "s system";
      s << "\t\tContext switches: " << usage.ru_nvcsw << " voluntary / " << usage.ru_nivcsw << " involuntary" << endl;
      s << "Maximum resident size: " << p.first << " " << p.second;
      s << "\t\tPage faults: " << usage.ru_majflt << " major / " << usage.ru_minflt << " minor" << endl;
    }
  else
    CantHappen("getrusage() failed: " << strerror(errno));
}

#ifdef GC_DEBUG
void
MemoryCell::stompArenas()
{
  for (Arena* a = firstArena; a != 0; a = a->nextArena)
    {
      MemoryCell* d = a->firstNode();
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
      MemoryCell* d = a->firstNode();
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
      MemoryCell* d = a->firstNode();
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
