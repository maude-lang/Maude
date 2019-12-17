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
//      Implementation for class MetaModuleCache.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "pointerMap.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "freeTheory.hh"
#include "higher.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      core class definitions
#include "dagRoot.hh"

//	higher class definitions
#include "cacheableState.hh"

//      free theory class definitions
#include "freeSymbol.hh"
#include "freeDagNode.hh"

//      mixfix class definitions
#include "userLevelRewritingContext.hh"

//      metalevel class definitions
#include "metaOpCache.hh"

local_inline void
MetaOpCache::Item::clear()
{
  delete metaOp;
  delete state;
}

MetaOpCache::MetaOpCache(int maxSize)
  : maxSize(maxSize)
{
}

MetaOpCache::~MetaOpCache()
{
  for (int i = cache.length() - 1; i >= 0; i--)
    cache[i].clear();
}

bool
MetaOpCache::sameProblem(FreeDagNode* m1, DagNode* m2, int nrArgumentsToIgnore)
{
  Symbol* s = m1->symbol();
  if (s == m2->symbol())
    {
      //
      //	We don't look at the first argument since that's our module which
      //	must be equal, and we don't look at the last nrArgumentsToIgnore
      //	arguments from the tail of the arguments list because the user
      //	decided not to care about those for deciding if a cached state
      //	is interesting. Often the last argument holds a solution number
      //	but that is not required for insert() and remove().
      //
      int nrUsefulArgs = s->arity() - nrArgumentsToIgnore;
      FreeDagNode* m3 = static_cast<FreeDagNode*>(m2);
      for (int i = 1; i < nrUsefulArgs; i++)
	{
	  if (!(m1->getArgument(i)->equal(m3->getArgument(i))))
	    return false;
	}
      return true;
    }
  return false;
}

void
MetaOpCache::insert(FreeDagNode* metaOp, CacheableState* state, Int64 lastSolutionNr)
{
  if (cache.length() < maxSize)
    cache.expandBy(1);
  else
    cache[cache.length() - 1].clear();
      
  for (int i = cache.length() - 1; i >= 1; i--)
    cache[i] = cache[i - 1];

  cache[0].metaOp = new DagRoot(metaOp->makeClone());
  cache[0].state = state;
  cache[0].lastSolutionNr = lastSolutionNr;
}

bool
MetaOpCache::remove(FreeDagNode* metaOp,
		    CacheableState*& state,
		    Int64& lastSolutionNr,
		    int nrArgumentsToIgnore)
{
  int nrEntries = cache.length();
  for (int i = 0; i < nrEntries; i++)
    {
      if (sameProblem(metaOp, cache[i].metaOp->getNode(), nrArgumentsToIgnore))
	{
	  delete cache[i].metaOp;
	  state = cache[i].state;
	  lastSolutionNr = cache[i].lastSolutionNr;
	  for (i++; i < nrEntries; i++)
	    cache[i - 1] = cache[i];
	  cache.contractTo(nrEntries - 1);
	  return true;
	}
    }
  return false;
}
