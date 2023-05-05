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
//      Implementation for class MetaModuleCache.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
 
//      core class definitions
#include "dagRoot.hh"

//      free theory class definitions
#include "freeSymbol.hh"
#include "freeDagNode.hh"

//	front end class definitions
#include "metaModule.hh"
#include "metaModuleCache.hh"

int MetaModuleCache::maxSize = UNDEFINED;

MetaModuleCache::MetaModuleCache()
{
  if (maxSize == UNDEFINED)
    {
      maxSize = DEFAULT_MAX_SIZE;
      if (const char* value = getenv("MAUDE_META_MODULE_CACHE_SIZE"))
	{
	  int size = atoi(value);
	  if (size >= MIN_MAX_SIZE && size <= MAX_MAX_SIZE)
	    maxSize = size;
	}
    }
}

MetaModuleCache::~MetaModuleCache()
{
  flush();
}

void
MetaModuleCache::Pair::clear()
{
  delete dag;
  MetaModule* t = module;
  //
  //	It's important to zero module before calling deepSelfDestruct()
  //	so that the regretToInform() call back will not find the doomed module
  //	in the cache and shuffle things around.
  //
  module = 0;
  t->deepSelfDestruct();
}


MetaModule*
MetaModuleCache::find(DagNode* dag)
{
  int nrPairs = cache.length();
  //
  //	First check if we've downed this is exact dag.
  //
  for (int i = 0; i < nrPairs; i++)
    {
      if (dag == cache[i].dag->getNode())
	return moveToFront(i);
    }
  //
  //	Then check if we've downed an equal dag. Surprisingly this is a very common case.
  //	It's tempting to compare on getHashValue() as a filter before the expensive equal() 
  //	call but benchmarking shows that it saves too few calls to pay for itself.
  //
  for (int i = 0; i < nrPairs; i++)
    {
      DagNode* cached = cache[i].dag->getNode();
      if (dag->equal(cached))
	return moveToFront(i);
    }
  return 0;
}

MetaModule*
MetaModuleCache::moveToFront(int chosen)
{
  DebugAdvisory("cache hit (" << cache[chosen].module <<
		") for #" << chosen << " cache size = " << cache.length());
  if (chosen == 0)
    return cache[0].module;
  Pair p = cache[chosen];
  for (int i = chosen; i > 0; i--)
    cache[i] = cache[i - 1];
  cache[0] = p;
  return p.module;
}

void 
MetaModuleCache::insert(DagNode* dag, MetaModule* module)
{
  DebugAdvisory("cache miss (" << module <<
		"), cache size = " << cache.length());
  int size = cache.length();
  if (size == maxSize)
    cache[--size].clear();
  else
    cache.expandTo(size + 1);
  for (int i = size; i > 0; i--)
    cache[i] = cache[i - 1];
  cache[0].dag = new DagRoot(dag);
  cache[0].module = module;
}

void 
MetaModuleCache::flush()
{
  int nrPairs = cache.length();
  for (int i = 0; i < nrPairs; i++)
    cache[i].clear();
  cache.clear();
}

void
MetaModuleCache::regretToInform(Entity* doomedEntity)
{
  ImportModule* doomedModule = static_cast<ImportModule*>(doomedEntity);
  //
  //	Must remove doomed module from cache if it's there.
  //
  int nrPairs = cache.length();
  for (int i = 0; i < nrPairs; i++)
    {
      if (doomedModule == cache[i].module)
	{
	  delete cache[i].dag;
	  for (++i; i < nrPairs; ++i)
	    cache[i - 1] = cache[i];
	  cache.contractTo(nrPairs - 1);
	  return;
	}
    }
  //
  //	This can occur because the metamodule was not successfully
  //	completed and was destructed rather than added to the cache,
  //	or becasue we zeroed the cache entry in clear().
  //
  DebugAdvisory("doomed module " << doomedModule << " not in cache");
}
