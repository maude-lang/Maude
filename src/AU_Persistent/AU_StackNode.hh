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
//	Class for nodes in persistent stacks.
//
#ifndef _AU_StackNode_hh_
#define _AU_StackNode_hh_
#include "argVec.hh"
#include "symbol.hh"  // HACK
#include "dagNode.hh"  // HACK
#include "memoryCell.hh"

class AU_StackNode
{
  NO_COPYING(AU_StackNode);

public:
  AU_StackNode(){}

  DagNode* first() const;
  int firstUsed() const;

  AU_StackNode* split(int nrElements, AU_StackNode*& rev) const;
  static AU_StackNode* pop(AU_StackNode* base);
  static AU_StackNode* push(AU_StackNode* base, DagNode* dagNode);
  static AU_StackNode* prepend(AU_StackNode* base,
			       int nrElements,
			       const AU_StackNode* fwd,
			       const AU_StackNode* rev);

  void fwdCopy(ArgVec<DagNode*>::iterator s) const;
  void revCopy(ArgVec<DagNode*>::iterator s) const;
  static AU_StackNode* fwdJoin(int nrElements,
			       ArgVec<DagNode*>::const_iterator s,
			       AU_StackNode* d);
  static AU_StackNode* revJoin(int nrElements,
			       ArgVec<DagNode*>::const_iterator s,
			       AU_StackNode* d);
  static AU_StackNode* fwdMake(int nrElements,
			       ArgVec<DagNode*>::const_iterator s,
			       AU_StackNode* d = 0);
  static AU_StackNode* revMake(int nrElements,
			       ArgVec<DagNode*>::const_iterator s,
			       AU_StackNode* d = 0);
  int fwdComputeBaseSort(Symbol* symbol);
  int revComputeBaseSort(Symbol* symbol);

  int getSortIndex() const;
  void mark();

  void* operator new(size_t size);

#ifdef CHECK_DEQUE
  static int checkIntegrity(AU_StackNode* p);
#endif

private:
  enum Values
  {
    ELEMENTS_PER_NODE = 4
  };

  static AU_StackNode* fwdCopy(int nrElements,
			       AU_StackNode* tail,
			       AU_StackNode const*& sp,
			       int& si);
  static AU_StackNode* revCopy(const AU_StackNode* sp, int si);

  void setMarked();
  bool isMarked() const;
  void setSortIndex(int sortIndex);
  AU_StackNode* partialClone(int f);
  //
  //    Get pointer to MemoryInfo object associated with us.
  //
  MemoryInfo* getMemoryInfo();
  const MemoryInfo* getMemoryInfo() const;

  DagNode* args[ELEMENTS_PER_NODE];
  AU_StackNode* next;

  friend class AU_DequeIter;
};

inline void*
AU_StackNode::operator new(size_t size)
{
  Assert(size <= sizeof(MemoryCell), "stack node too big");
  void* m = MemoryCell::allocateMemoryCell();
  //
  //	MemoryCell::allocateMemoryCell() no longer sets the half word to
  //	Sort::SORT_UNKNOWN. This responsibility is shifted to us.
  //
  MemoryCell::getMemoryInfo(m)->setHalfWord(Sort::SORT_UNKNOWN);
  //
  //	MemoryCell::allocateMemoryCell() no longer clears the memory
  //	cell flags. This responsibility is shifted to us.
  //
  MemoryCell::getMemoryInfo(m)->clearAllFlags();
  return m;
}

inline MemoryInfo*
AU_StackNode::getMemoryInfo()
{
  return MemoryCell::getMemoryInfo(this);
}

inline const MemoryInfo*
AU_StackNode::getMemoryInfo() const
{
  return MemoryCell::getMemoryInfo(this);
}

inline void
AU_StackNode::setMarked()
{
  getMemoryInfo()->setMarked();
}

inline bool
AU_StackNode::isMarked() const
{
  return getMemoryInfo()->isMarked();
}

inline void 
AU_StackNode::setSortIndex(int sortIndex)
{
  getMemoryInfo()->setHalfWord(sortIndex);
}

inline int
AU_StackNode::getSortIndex() const
{
  return getMemoryInfo()->getHalfWord();
}

inline DagNode*
AU_StackNode::first() const
{
  for (DagNode* const* p = args;; ++p)
    {
      Assert(p - args < ELEMENTS_PER_NODE, "didn't find non-null pointer");
      DagNode* t = *p;
      if (t != 0)
	return t;
    }
}

inline int
AU_StackNode::firstUsed() const
{
  for (int i = 0;; ++i)
    {
      Assert(i < ELEMENTS_PER_NODE, "didn't find non-null pointer");
      if (args[i] != 0)
	return i;
    }
}

inline AU_StackNode*
AU_StackNode::partialClone(int f)
{
  AU_StackNode* d = new AU_StackNode;
  do
    d->args[f] = args[f];
  while (++f < ELEMENTS_PER_NODE);
  d->next = next;
  return d;
}

#endif
