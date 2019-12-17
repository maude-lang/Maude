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
#include "memoMap.hh"
#include "memoTable.hh"

bool
MemoTable::memoRewrite(SourceSet& sourceSet, DagNode* subject, RewritingContext& context)
{
#if 0
  DebugAdvisory("memoRewrite()  subject " << subject <<
		" at " << ((void*) subject) <<
		" has sort index " << subject->getSortIndex());
#endif
  MemoMap* memoMap = getModule()->getMemoMap();
  int subjectIndex = memoMap->getFromIndex(subject);

  if (DagNode* toDag = memoMap->getToDag(subjectIndex))
    {
#if 0
      DebugAdvisory("memoRewrite()  toDag " << subject << 
		    " at " << ((void*) toDag) <<
		    " has sort index " << subject->getSortIndex());
#endif
      bool trace = RewritingContext::getTraceStatus();
      if (trace)
	{
	  context.tracePreEqRewrite(subject, 0, RewritingContext::MEMOIZED);
	  if (context.traceAbort())
	    return false;
	}
      toDag->overwriteWithClone(subject);
      context.incrementEqCount();
      if (trace)
	context.tracePostEqRewrite(subject);
      return true;
    }
  sourceSet.append(subjectIndex);
  return false;
}

void
MemoTable::memoEnter(SourceSet& sourceSet, DagNode* destination)
{
#if 0
  DebugAdvisory("memoEnter()  destination " << destination <<
		" at " << ((void*) destination) <<
		" has sort index " << destination->getSortIndex());
#endif
  MemoMap* memoMap = getModule()->getMemoMap();
  FOR_EACH_CONST(i, SourceSet, sourceSet)
    memoMap->assignToDag(*i, destination);
}
