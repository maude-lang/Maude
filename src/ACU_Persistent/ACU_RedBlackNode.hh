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
//	Class for nodes in persistent red-black trees.
//
#ifndef _ACU_RedBlackNode_hh_
#define _ACU_RedBlackNode_hh_
#include "symbol.hh"  // HACK
#include "dagNode.hh"  // HACK
#include "memoryCell.hh"

class ACU_RedBlackNode
{
  NO_COPYING(ACU_RedBlackNode);

public:
  enum PublicValues
  {
    MAX_TREE_HEIGHT = 64	// must be height of largest red-black tree
  };

  ACU_RedBlackNode(DagNode* dagNode,
		   int multiplicity,
		   ACU_RedBlackNode* left,
		   ACU_RedBlackNode* right);

  ACU_RedBlackNode(DagNode* dagNode,
		   int multiplicity,
		   ACU_RedBlackNode* left,
		   ACU_RedBlackNode* right,
		   int maxMult);

  DagNode* getDagNode() const;
  int getMultiplicity() const;
  ACU_RedBlackNode* getLeft() const;
  ACU_RedBlackNode* getRight() const;
  int getMaxMult() const;

  void setSortIndex(int index);
  int getSortIndex() const;

  void makeRed();  // need for conversion from array

  void* operator new(size_t size);
  void setMarked();
  bool isMarked();

  static bool find(ACU_RedBlackNode* root,
		   DagNode* dagNode,
		   ACU_Stack& path);

  static bool find(ACU_RedBlackNode* root,
		   Term* term,
		   ACU_Stack& path);

  static bool findFirstPotentialMatch(ACU_RedBlackNode* root,
				      Term* key,
				      const Substitution& partial,
				      ACU_Stack& path);

  static bool findGeqMult(ACU_RedBlackNode* root,
			  int multiplicity,
			  ACU_Stack& path);

  static ACU_RedBlackNode* consInsert(ACU_RedBlackNode* root,
				      DagNode* dagNode,
				      int multiplicity,
				      int& delta);

  static ACU_RedBlackNode* consDelete(ACU_Stack& path,
				      int multiplicity,
				      int& delta);

  //
  //	Experimental hash cons code.
  //
  ACU_RedBlackNode* canonicalRebuild(HashConsSet* hcs);

  void dump(ostream& s, int indent);

#ifdef CHECK_TREE
  //
  //	Extra functions for strict checking of red-black property
  //	after each operation.
  //
  bool checkRedBlackProperty();
  bool checkRedBlackProperty(int& height);
  static ACU_RedBlackNode* consInsert2(ACU_RedBlackNode* root,
				      DagNode* dagNode,
				      int multiplicity);

  static ACU_RedBlackNode* consDelete2(ACU_Stack& path, int multiplicity);

#endif

private:
  enum Values
  {
    LEFT_INDEX = 1,
    RIGHT_INDEX = 0
  };

  enum Flags
  {
    RED = 1
  };

  //
  //	Returns left child if negative, right child otherwise.
  //
  ACU_RedBlackNode* getChild(int sign) const;

  static ACU_RedBlackNode* copy(ACU_Stack& path,
				ACU_RedBlackNode* n,
				ACU_RedBlackNode* old);

  static ACU_RedBlackNode* copy(ACU_Stack& path,
				ACU_RedBlackNode* n,
				ACU_RedBlackNode* old,
				ACU_RedBlackNode* victim,
				ACU_RedBlackNode* surrogate);

  static ACU_RedBlackNode* copyRebalance(ACU_Stack& path,
					 ACU_RedBlackNode* n,
					 ACU_RedBlackNode* old,
					 ACU_RedBlackNode* victim,
					 ACU_RedBlackNode* surrogate);

  static ACU_RedBlackNode* blackNode(ACU_RedBlackNode* key,
				     ACU_RedBlackNode* left,
				     ACU_RedBlackNode* right);

  static ACU_RedBlackNode* redNode(ACU_RedBlackNode* key,
				   ACU_RedBlackNode* left,
				   ACU_RedBlackNode* right);

  bool isRed() const;
  static bool isRed(const ACU_RedBlackNode* mightBeNull);
  void makeRedIfRed(const ACU_RedBlackNode* other);
  //
  //	Nasty cross casting stuff.
  //
  MemoryCell* getMemoryCell();
  const MemoryCell* getMemoryCell() const;

  DagNode* const dagNode;
  int const multiplicity;
  ACU_RedBlackNode* children[2];  // no way to initialize const
  int maxMult;
};

inline MemoryCell*
ACU_RedBlackNode::getMemoryCell()
{
  return static_cast<MemoryCell*>(static_cast<void*>(this));
}

inline const MemoryCell*
ACU_RedBlackNode::getMemoryCell() const
{
  return static_cast<const MemoryCell*>(static_cast<const void*>(this));
}

inline void*
ACU_RedBlackNode::operator new(size_t size)
{
  //
  //	We rely on MemoryCell::allocateMemoryCell() setting the half word to
  //	Sort::SORT_UNKNOWN.
  //
  Assert(size <= sizeof(MemoryCell), "red-black node too big");
  return MemoryCell::allocateMemoryCell();
}

inline void
ACU_RedBlackNode::setMarked()
{
  getMemoryCell()->setMarked();
}

inline bool
ACU_RedBlackNode::isMarked()
{
  return getMemoryCell()->isMarked();
}

inline int
ACU_RedBlackNode::getMaxMult() const
{
  return maxMult;
}

inline
ACU_RedBlackNode::ACU_RedBlackNode(DagNode* dagNode,
				   int multiplicity,
				   ACU_RedBlackNode* left,
				   ACU_RedBlackNode* right)
  : dagNode(dagNode),
    multiplicity(multiplicity)
{
  int mm = multiplicity;
  if (left != 0)
    {
      int t = left->maxMult;
      if (t > mm)
	mm = t;
    }
  if (right != 0)
    {
      int t = right->maxMult;
      if (t > mm)
	mm = t;
    }
  maxMult = mm;
  children[LEFT_INDEX] = left;
  children[RIGHT_INDEX] = right;
}

inline
ACU_RedBlackNode::ACU_RedBlackNode(DagNode* dagNode,
				   int multiplicity,
				   ACU_RedBlackNode* left,
				   ACU_RedBlackNode* right,
				   int maxMult)
  : dagNode(dagNode),
    multiplicity(multiplicity),
    maxMult(maxMult)
{
  Assert(maxMult ==
	 max(multiplicity,
	     max((left == 0) ? 0 : left->maxMult,
		 (right == 0) ? 0 : right->maxMult)),
	  "bad maxMult");
  children[LEFT_INDEX] = left;
  children[RIGHT_INDEX] = right;
}

inline DagNode*
ACU_RedBlackNode::getDagNode() const
{
  return dagNode;
}

inline int
ACU_RedBlackNode::getMultiplicity() const
{
  return multiplicity;
}

inline ACU_RedBlackNode*
ACU_RedBlackNode::getLeft() const
{
  return children[LEFT_INDEX];
}

inline ACU_RedBlackNode*
ACU_RedBlackNode::getRight() const
{
  return children[RIGHT_INDEX];
}

inline ACU_RedBlackNode*
ACU_RedBlackNode::getChild(int sign) const
{
  return children[sign < 0];
}

inline void
ACU_RedBlackNode::setSortIndex(int index)
{
  getMemoryCell()->setHalfWord(index);
}

inline int
ACU_RedBlackNode::getSortIndex() const
{
  return getMemoryCell()->getHalfWord();
}

inline bool
ACU_RedBlackNode::isRed() const
{
  return getMemoryCell()->getFlag(RED);
}

inline bool
ACU_RedBlackNode::isRed(const ACU_RedBlackNode* mightBeNull)
{
  return mightBeNull != 0 && mightBeNull->getMemoryCell()->getFlag(RED);
}

inline void
ACU_RedBlackNode::makeRed()
{
  getMemoryCell()->setFlag(RED);
}

inline void
ACU_RedBlackNode::makeRedIfRed(const ACU_RedBlackNode* other)
{
  getMemoryCell()->copySetFlags(RED, other->getMemoryCell());
}

inline ACU_RedBlackNode*
ACU_RedBlackNode::redNode(ACU_RedBlackNode* key,
			  ACU_RedBlackNode* left,
			  ACU_RedBlackNode* right)
{
  ACU_RedBlackNode* n = blackNode(key, left, right);
  n->makeRed();
  return n;
}

#endif
