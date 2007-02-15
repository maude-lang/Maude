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
//	Implementation for class PositionState.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "extensionInfo.hh"

//	core class definitions
#include "positionState.hh"

PositionState::PositionState(DagNode* top, int flags, int minDepth, int maxDepth)
  : flags(flags),
    minDepth(minDepth),
    maxDepth(maxDepth)
{
  positionQueue.append(RedexPosition(top, UNDEFINED, UNDEFINED));
  depth.append(0);
  extensionInfo = 0;
  extensionInfoValid = true;  // in case maxDepth = -1 for no extension
  nextToReturn = -1;
  nextToExplore = -1;
}

PositionState::~PositionState()
{
  delete extensionInfo;
}

local_inline bool
PositionState::exploreNextPosition()
{
  int finish = positionQueue.length();
  for(;;)
    {
      if (++nextToExplore >= finish)
	return false;
      int ourDepth = depth[nextToExplore];
      if (ourDepth >= maxDepth)
	return false;
      DagNode* d = positionQueue[nextToExplore].node();
      d->stackArguments(positionQueue, nextToExplore, flags & RESPECT_FROZEN);
      int newFinish = positionQueue.length();
      if (finish < newFinish)
	{
	  ++ourDepth;
	  depth.expandTo(newFinish);
	  for (; finish < newFinish; finish++)
	    depth[finish] = ourDepth;
	  break;
	}
    }
  return true;
}

bool
PositionState::findNextPosition()
{
  do
    {
      if (++nextToReturn >= positionQueue.length() && !exploreNextPosition())
	return false;
    }
  while (depth[nextToReturn] < minDepth);
  if (maxDepth >= 0)
    {
      delete extensionInfo;
      extensionInfo = 0;
      //
      //	Force makeExtensionInfo() if getExtensionInfo() called.
      //
      extensionInfoValid = false;
    }
  return true;
}

PositionState::DagPair
PositionState::rebuildDag(DagNode* replacement, ExtensionInfo* extInfo, PositionIndex index) const
{
  //
  //	Extend the replacement term if needed.
  //
  if (extInfo != 0 && !(extInfo->matchedWhole()))
    replacement = positionQueue[index].node()->partialConstruct(replacement, extInfo);
  //
  //	Walk up the stack rebuilding.
  //
  DagNode* newDag = replacement;
  int argIndex = positionQueue[index].argIndex();
  for (PositionIndex i = positionQueue[index].parentIndex(); i != UNDEFINED;)
    {
      const RedexPosition& rp = positionQueue[i];
      newDag = rp.node()->copyWithReplacement(argIndex, newDag);
      argIndex = rp.argIndex();
      i = rp.parentIndex();
    }
  //
  //	We return the rebuilt dag, and the extended replacement term since the caller may
  //	need the later for tracing purposes.
  //
  return DagPair(newDag, replacement);
}
