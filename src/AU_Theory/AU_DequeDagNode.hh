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
//	Class for A and AU dag nodes based on persistent deques.
//
#ifndef _AU_DequeDagNode_hh_
#define _AU_DequeDagNode_hh_
#include "AU_BaseDagNode.hh"
#include "AU_Deque.hh"

class AU_DequeDagNode : public AU_BaseDagNode
{
  NO_COPYING(AU_DequeDagNode);

public:
  AU_DequeDagNode(AU_Symbol* symbol, const AU_Deque& original);
  //
  //	Functions required by theory interface.
  //
  RawDagArgumentIterator* arguments();
  size_t getHashValue();
  int compareArguments(const DagNode* other) const;
  void overwriteWithClone(DagNode* old);
  DagNode* makeClone();
  DagNode* copyWithReplacement(int argIndex, DagNode* replacement);
  DagNode* copyWithReplacement(Vector<RedexPosition>& redexStack,
			       int first,
			       int last);
  //
  //	Functions required to handle extension information.
  //
  ExtensionInfo* makeExtensionInfo();

  //
  //	Interface for narrowing.
  //
  //	This is needed to remove any deque nodes from dag being narrowed since
  //	unification and narrowing functionality doesn't support them.
  //
  bool indexVariables2(NarrowingVariableInfo& indices, int baseIndex);
  //
  //	Functions particular to AU_DequeDagNode.
  //
  int nrArgs() const;
  const AU_Deque& getDeque() const;
  static AU_DagNode* dequeToArgVec(AU_DequeDagNode* original);

private:
  //
  //	Functions required by theory interface.
  //
  DagNode* markArguments();
  DagNode* copyEagerUptoReduced2();
  DagNode* copyAll2();
  void clearCopyPointers2();

  AU_Deque deque;
};

inline
AU_DequeDagNode::AU_DequeDagNode(AU_Symbol* symbol, const AU_Deque& original)
  : AU_BaseDagNode(symbol),
    deque(original)
{
  setNormalizationStatus(DEQUE);
}

inline int
AU_DequeDagNode::nrArgs() const
{
  return deque.length();
}

inline const AU_Deque&
AU_DequeDagNode::getDeque() const
{
  return deque;
}

#endif
