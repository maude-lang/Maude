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
//      Implementation for class MetaModuleCache.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
 
//      core class definitions
#include "dagRoot.hh"

//	front end class definitions
#include "metaModule.hh"
#include "metaModuleCache.hh"

MetaModuleCache::MetaModuleCache(int maxSize)
  : maxSize(maxSize)
{
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
  for (int i = 0; i < nrPairs; i++)
    {
      if (dag == cache[i].dag->getNode())
	return moveToFront(i);
    }
  for (int i = 0; i < nrPairs; i++)
    {
      if (dag->equal(cache[i].dag->getNode()))
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
  cache.contractTo(0);
}

void
MetaModuleCache::regretToInform(ImportModule* doomedModule)
{
  //
  //	Must remove doomed module from cache if it's there.
  //
  int nrPairs = cache.length();
  for (int i = 0; i < nrPairs; i++)
    {
      if (doomedModule == cache[i].module)
	{
	  delete cache[i].dag;
	  for (int j = nrPairs - 1; j > i; j--)
	    cache[j] = cache[j - 1];
	  cache.contractTo(nrPairs - 1);
	  break;
	}
    }
}
