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
//      Class for smart pointers that form DAG roots for garbage collection.
//	They have zero impact on performance of GC when null.
//
#ifndef _dagRoot_hh_
#define _dagRoot_hh_
#include "rootContainer.hh"

class DagRoot : private RootContainer
{
public:
  DagRoot(DagNode* initial = 0);
  ~DagRoot();
  DagNode* getNode() const;
  void setNode(DagNode* node);
 
private:
  void markReachableNodes();

  DagNode* nodePointer;
};

inline
DagRoot::DagRoot(DagNode* initial)
{
  nodePointer = initial;
  if (initial != 0)
    link();
}

inline
DagRoot::~DagRoot()
{
  if (nodePointer != 0)
    unlink();
}

inline DagNode*
DagRoot::getNode() const
{
  return nodePointer;
}

inline void
DagRoot::setNode(DagNode* node)
{
  if (node == 0)
    {
      if (nodePointer != 0)
	unlink();
    }
  else
    {
      if (nodePointer == 0)
	link();
    }
  nodePointer = node;
}

#endif
