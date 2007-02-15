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
#include "searchState.hh"
#include "rewriteSequenceSearch.hh"
#include "unificationProblem.hh"

//      free theory class definitions
#include "freeSymbol.hh"
#include "freeDagNode.hh"

//      mixfix class definitions
#include "userLevelRewritingContext.hh"

//      metalevel class definitions
#include "metaOpCache.hh"

MetaOpCache::MetaOpCache(int maxSize)
  : maxSize(maxSize)
{
}

MetaOpCache::~MetaOpCache()
{
  for (int i = cache.length() - 1; i >= 0; i--)
    cache[i].clear();
}

void
MetaOpCache::Item::clear()
{
  delete metaOp;
  delete state;
  delete search;
  delete unification;
}

bool
MetaOpCache::sameProblem(FreeDagNode* m1, DagNode* m2)
{
  Symbol* s = m1->symbol();
  if (s == m2->symbol())
    {
      int nrUsefulArgs = s->arity() - 1;
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
MetaOpCache::insert(FreeDagNode* metaOp, SearchState* state, Int64 lastSolutionNr)
{
  if (cache.length() < maxSize)
    cache.expandBy(1);
  else
    cache[cache.length() - 1].clear();
      
  for (int i = cache.length() - 1; i >= 1; i--)
    cache[i] = cache[i - 1];

  cache[0].metaOp = new DagRoot(metaOp->makeClone());
  cache[0].state = state;
  cache[0].search = 0;
  cache[0].unification = 0;
  cache[0].lastSolutionNr = lastSolutionNr;
}

bool
MetaOpCache::remove(FreeDagNode* metaOp,
		    RewritingContext& parentContext,
		    SearchState*& state,
		    Int64& lastSolutionNr)
{
  int nrEntries = cache.length();
  for (int i = 0; i < nrEntries; i++)
    {
      if (sameProblem(metaOp, cache[i].metaOp->getNode()))
	{
	  delete cache[i].metaOp;
	  state = cache[i].state;
	  safeCast(UserLevelRewritingContext*, state->getContext())->
	    beAdoptedBy(safeCast(UserLevelRewritingContext*, &parentContext));
	  lastSolutionNr = cache[i].lastSolutionNr;
	  for (i++; i < nrEntries; i++)
	    cache[i - 1] = cache[i];
	  cache.contractTo(nrEntries - 1);
	  return true;
	}
    }
  return false;
}

void
MetaOpCache::insert(FreeDagNode* metaOp, RewriteSequenceSearch* search, Int64 lastSolutionNr)
{
  if (cache.length() < maxSize)
    cache.expandBy(1);
  else
    cache[cache.length() - 1].clear();
      
  for (int i = cache.length() - 1; i >= 1; i--)
    cache[i] = cache[i - 1];

  cache[0].metaOp = new DagRoot(metaOp->makeClone());
  cache[0].state = 0;
  cache[0].search = search;
  cache[0].unification = 0;
  cache[0].lastSolutionNr = lastSolutionNr;
}

bool
MetaOpCache::remove(FreeDagNode* metaOp,
		    RewritingContext& parentContext,
		    RewriteSequenceSearch*& search,
		    Int64& lastSolutionNr)
{
  int nrEntries = cache.length();
  for (int i = 0; i < nrEntries; i++)
    {
      if (sameProblem(metaOp, cache[i].metaOp->getNode()))
	{
	  delete cache[i].metaOp;
	  search = cache[i].search;
	  safeCast(UserLevelRewritingContext*, search->getContext())->
	    beAdoptedBy(safeCast(UserLevelRewritingContext*, &parentContext));
	  lastSolutionNr = cache[i].lastSolutionNr;
	  for (i++; i < nrEntries; i++)
	    cache[i - 1] = cache[i];
	  cache.contractTo(nrEntries - 1);
	  return true;
	}
    }
  return false;
}

void
MetaOpCache::insert(FreeDagNode* metaOp, UnificationProblem* unification, Int64 lastSolutionNr)
{
  if (cache.length() < maxSize)
    cache.expandBy(1);
  else
    cache[cache.length() - 1].clear();
      
  for (int i = cache.length() - 1; i >= 1; i--)
    cache[i] = cache[i - 1];

  cache[0].metaOp = new DagRoot(metaOp->makeClone());
  cache[0].state = 0;
  cache[0].search = 0;
  cache[0].unification = unification;
  cache[0].lastSolutionNr = lastSolutionNr;
}

bool
MetaOpCache::remove(FreeDagNode* metaOp,
		    UnificationProblem*& unification,
		    Int64& lastSolutionNr)
{
  int nrEntries = cache.length();
  for (int i = 0; i < nrEntries; i++)
    {
      if (sameProblem(metaOp, cache[i].metaOp->getNode()))
	{
	  delete cache[i].metaOp;
	  unification = cache[i].unification;
	  lastSolutionNr = cache[i].lastSolutionNr;
	  for (i++; i < nrEntries; i++)
	    cache[i - 1] = cache[i];
	  cache.contractTo(nrEntries - 1);
	  return true;
	}
    }
  return false;
}
