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
//	Class for Red-Black Tree based DAG nodes in the AC and ACU theories.
//
#ifndef _ACU_TreeDagNode_hh_
#define _ACU_TreeDagNode_hh_
#include "ACU_BaseDagNode.hh"
#include "ACU_Tree.hh"

class ACU_TreeDagNode : public ACU_BaseDagNode
{
public:
  ACU_TreeDagNode(ACU_Symbol* symbol, const ACU_Tree& tree);
   //
  //	Member functions required by theory interface.
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
  void stackArguments(Vector<RedexPosition>& stack,
		      int parentIndex,
		      bool respectFrozen);
  //
  //	Member functions required to handle extension information.
  //
  bool matchVariableWithExtension(int index,
				  const Sort* sort,
				  Substitution& solution,
				  Subproblem*& returnedSubproblem,
				  ExtensionInfo* extensionInfo);
  void partialReplace(DagNode* replacement, ExtensionInfo* extensionInfo);
  DagNode* partialConstruct(DagNode* replacement, ExtensionInfo* extensionInfo);
  //
  //	ACU_TreeDagNode -> ACU_DagNode in-place conversion function.
  //
  static ACU_DagNode* treeToArgVec(ACU_TreeDagNode* original);

  const ACU_Tree& getTree() const;

private:
 //
  //	Theory interface functions.
  //
  DagNode* markArguments();
  DagNode* copyEagerUptoReduced2();
  void clearCopyPointers2();
  //
  //	Arguments under ACU symbol.
  //
  ACU_Tree tree;
  //
  //	Slot for caching hash value.
  //
  size_t hashCache;
};

inline
ACU_TreeDagNode::ACU_TreeDagNode(ACU_Symbol* symbol, const ACU_Tree& tree)
  : ACU_BaseDagNode(symbol), tree(tree)
{
  Assert(tree.getSize() > 1 || tree.getMaxMult() > 1,
  	 "tried to make ACU_TreeDagNode with single argument");
  setNormalizationStatus(TREE);
}

inline
const ACU_Tree&
ACU_TreeDagNode::getTree() const
{
  return tree;
}

#endif
