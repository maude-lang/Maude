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
//	Note that position indicies are persistent and can be saved, along with
//	a copy of extension info for a rebuildDag() after more searching.
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

  typedef int PositionIndex;
  typedef pair<DagNode*, DagNode*> DagPair;

  //
  //	maxDepth = -1		means at top, no extension
  //	maxDepth = 0		means at top, with extension
  //	maxDepth = UNBOUNDED	means all the way down to the leaf nodes, with extension
  //
  PositionState(DagNode* top, int flags = 0, int minDepth = 0, int maxDepth = -1);
  ~PositionState();

  bool findNextPosition();  // should this be protected?
  DagNode* getDagNode() const;
  DagNode* getDagNode(PositionIndex index) const;
  ExtensionInfo* getExtensionInfo();
  PositionIndex getPositionIndex() const;
  DagPair rebuildDag(DagNode* replacement) const;
  DagPair rebuildDag(DagNode* replacement, ExtensionInfo* extInfo, PositionIndex index) const;

  int getFlags() const;

private:
  bool exploreNextPosition();

  const short flags;
  const int minDepth;
  const int maxDepth;
  ExtensionInfo* extensionInfo;
  bool extensionInfoValid;	// need separate flag because 0 is a valid extensionInfo value
  //
  //	For breathfirst traversal over positions.
  //
  Vector<RedexPosition> positionQueue;
  Vector<int> depth;
  PositionIndex nextToExplore;
  PositionIndex nextToReturn;
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

inline DagNode*
PositionState::getDagNode(PositionIndex index) const
{
  Assert(index >= 0 && index <= nextToReturn, "bad index");
  return positionQueue[index].node();
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

inline PositionState::PositionIndex
PositionState::getPositionIndex() const
{
  Assert(nextToReturn >= 0, "findNextPosition() not called");
  return nextToReturn;
}

inline PositionState::DagPair
PositionState::rebuildDag(DagNode* replacement) const
{
  Assert(nextToReturn >= 0, "findNextPosition() not called");
  return rebuildDag(replacement, extensionInfo, nextToReturn);
}

#endif
