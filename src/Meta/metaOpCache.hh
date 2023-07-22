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
//      Class for cache of meta-operations.
//
#ifndef _metaOpCache_hh_
#define _metaOpCache_hh_
#include "userLevelRewritingContext.hh"
#include "cacheableState.hh"

class MetaOpCache
{
  NO_COPYING(MetaOpCache);

public:
  MetaOpCache(int maxSize = 4);
  ~MetaOpCache();

  //
  //	Each state is recorded and restrieved with the solution number it
  //	corresponds to. There is no requirement that a (meta) solution number
  //	is stored in the last argument under the top symbol of metaOp.
  //
  //	Indeed the notion of solution number need not make sense for the
  //	user, and more than one argument from tail of the argument list
  //	may be ignored for deciding if a cached state is of interest.
  //	
  //	Since each user starts by pulling out any corresponding state, there
  //	can only be at most one corresponding state to any descent function
  //	call or metaInterpreter request message.
  //
  void insert(FreeDagNode* metaOp, CacheableState* state, Int64 lastSolutionNr);
  bool remove(FreeDagNode* metaOp,
	      CacheableState*& state,
	      Int64& lastSolutionNr,
	      int nrArgumentsToIgnore = 1);
  //
  //	These templates provide slightly higher-level functionality
  //	above remove() for those users that fit certain usage patterns.
  //
  //	For all objects that live in a MetaModule cache and for
  //	objects without a RewritingContext that live in a
  //	MetaLevelOpSymbol cache.
  //
  template<class T>
  bool getCachedStateObject(FreeDagNode* subject,
			    Int64 solutionNr,
			    T*& state,
			    Int64& lastSolutionNr);
  //
  //	For objects that have a RewritingContext (whose parent pointer
  //	may be stale) that live in a MetaLevelOpSymbol cache.
  //	Here we pass a RewritingContext that becomes the new parent.
  //
  template<class T>
  bool getCachedStateObject(FreeDagNode* subject,
			    RewritingContext& context,
			    Int64 solutionNr,
			    T*& state,
			    Int64& lastSolutionNr);
  //
  //	Attempt to purge a single state derived from class T.
  //
  template<class T>
  bool purge();

private:
  bool sameProblem(FreeDagNode* m1, DagNode* m2, int nrArgumentsToIgnore);

  struct Item
  {
    void clear();

    DagRoot* metaOp;
    CacheableState* state;
    Int64 lastSolutionNr;
  };

  const int maxSize;
  Vector<Item> cache;
};

inline void
MetaOpCache::Item::clear()
{
  delete metaOp;
  delete state;
}

template<class T>
inline bool
MetaOpCache::getCachedStateObject(FreeDagNode* subject,
				  Int64 solutionNr,
				  T*& state,
				  Int64& lastSolutionNr)
{
  CacheableState* cachedState;
  if (remove(subject, cachedState, lastSolutionNr))
    {
      DebugAdvisory("looking for solution #" << solutionNr << " and found cached solution #" << lastSolutionNr);
      if (lastSolutionNr <= solutionNr)
	{
	  state = safeCast(T*, cachedState);
	  return true;
	}
      delete cachedState;
    }
  return false;
}

template<class T>
inline bool
MetaOpCache::getCachedStateObject(FreeDagNode* subject,
				  RewritingContext& context,
				  Int64 solutionNr,
				  T*& state,
				  Int64& lastSolutionNr)
{
  CacheableState* cachedState;
  if (remove(subject, cachedState, lastSolutionNr))
    {
      DebugAdvisory("looking for solution #" << solutionNr << " and found cached solution #" << lastSolutionNr);
      if (lastSolutionNr <= solutionNr)
	{
	  state = safeCastNonNull<T*>(cachedState);
	  //
	  //	The parent context pointer of the root context in the
	  //	state object is possibly stale.
	  //	So we replace it with a pointer to the current context.
	  //
	  safeCastNonNull<UserLevelRewritingContext*>(state->getContext())->
	    beAdoptedBy(safeCastNonNull<UserLevelRewritingContext*>(&context));
	  return true;
	}
      delete cachedState;
    }
  return false;
}

template<class T>
inline bool
MetaOpCache::purge()
{
  Index nrEntries = cache.size();
  for (int i = 0; i < nrEntries; i++)
    {
      if (dynamic_cast<T*>(cache[i].state) != 0)
	{
	  cache[i].clear();
	  for (i++; i < nrEntries; i++)
	    cache[i - 1] = cache[i];
	  --nrEntries;
	  cache.contractTo(nrEntries);
	  return true;
	}
    }
  return false;
}

#endif
