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
//	Class for storing position and extension information during a search.
//
#ifndef _positionState_hh_
#define _positionState_hh_
#include <stack>
#include "redexPosition.hh"

class PositionState
{
  NO_COPYING(PositionState);

public:
  enum Flags
  {
    RESPECT_FROZEN = 1
  };

  PositionState(DagNode* top, int flags = 0, int minDepth = 0, int maxDepth = -1);
  ~PositionState();

  bool findNextPosition();  // should this be protected?
  DagNode* getDagNode() const;
  ExtensionInfo* getExtensionInfo();
  DagNode* rebuildDag(DagNode* replacement) const;
  int getFlags() const;

private:
  bool exploreNextPosition();

  const short flags;
  const int minDepth;
  const int maxDepth;
  ExtensionInfo* extensionInfo;
  bool extensionInfoValid;
  //
  //	For breathfirst traversal over positions.
  //
  Vector<RedexPosition> positionQueue;
  Vector<int> depth;
  int nextToExplore;
  int nextToReturn;
};

inline int
PositionState::getFlags() const
{
  return flags;
}

inline DagNode*
PositionState::getDagNode() const
{
  Assert(nextToReturn >= 0, "findNextPosition() not called");
  return positionQueue[nextToReturn].node();
}

inline ExtensionInfo* 
PositionState::getExtensionInfo()
{
  Assert(nextToReturn >= 0, "findNextPosition() not called");
  if (!extensionInfoValid)
    {
      extensionInfo = getDagNode()->makeExtensionInfo();
      extensionInfoValid = true;
    }
  return extensionInfo;
}

#endif
