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
//      Implementation for class MemoTable.
//
#include <map>

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "memoTable.hh"

struct MemoTable::dagNodeLt
{
  bool operator()(const DagNode* d1, const DagNode* d2)
    {
      return d1->compare(d2) < 0;
    }
};

class MemoTable::MemoMap
 : public map<DagNode*, DagNode*, MemoTable::dagNodeLt>,
   private SimpleRootContainer
{
  void markReachableNodes();
};

MemoTable::~MemoTable()
{
  delete memoMap;
}

void
MemoTable::clearMemo()
{
  delete memoMap;
  memoMap = 0;
}

bool
MemoTable::memoRewrite(SourceSet& sourceSet, DagNode* subject, RewritingContext& context)
{
  if (memoMap != 0)
    {
      MemoMap::iterator i = memoMap->find(subject);
      if (i != memoMap->end())
	{
	  bool trace = RewritingContext::getTraceStatus();
	  if (trace)
	    {
	      context.tracePreEqRewrite(subject, 0, RewritingContext::MEMOIZED);
	      if (context.traceAbort())
		return false;
	    }
	  (*i).second->overwriteWithClone(subject);
	  context.incrementEqCount();
	  if (trace)
	    context.tracePostEqRewrite(subject);
	  return true;
	}
    }
  sourceSet.sources.append(subject->copyReducible());
  return false;
}

void
MemoTable::memoEnter(SourceSet& sourceSet, DagNode* destination)
{
  int nrEntries = sourceSet.sources.length();
  if (nrEntries > 0)
    {
      if (memoMap == 0)
	memoMap = new MemoMap;
      for (int i = 0; i < nrEntries; i++)
	(*memoMap)[sourceSet.sources[i]] = destination;
    }
}

void
MemoTable::SourceSet::markReachableNodes()
{
  int nrEntries = sources.length();
  for (int i = 0; i < nrEntries; i++)
    sources[i]->mark();
}

void
MemoTable::MemoMap::markReachableNodes()
{
  for (MemoMap::iterator i = begin(); i != end(); i++)
    {
      (*i).first->mark();
      (*i).second->mark();
    }
}
