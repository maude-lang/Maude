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
//	Class for deques of arguments with constant time copy.
//
#ifndef _AU_Deque_hh_
#define _AU_Deque_hh_
#include "argVec.hh"
#include "AU_StackNode.hh"

class AU_Deque
{
public:
  AU_Deque(const ArgVec<DagNode*>& source);

  int length() const;
  void copyToArgVec(ArgVec<DagNode*>& dest) const;

  DagNode* topLeft();
  void popLeft();
  void pushLeft(DagNode* dagNode);
  void pushLeft(const AU_Deque& other);
  void pushLeft(const ArgVec<DagNode*>& argArray);

  DagNode* topRight();
  void popRight();
  void pushRight(DagNode* dagNode);
  void pushRight(const AU_Deque& other);
  void pushRight(const ArgVec<DagNode*>& argArray);

  int computeBaseSort(Symbol* symbol) const;
  void mark();

#ifdef CHECK_DEQUE
  void checkIntegrity() const;
#endif

private:
  int nrElements;
  AU_StackNode* left;
  AU_StackNode* right;

  friend class AU_DequeIter;
};

inline
AU_Deque::AU_Deque(const ArgVec<DagNode*>& source)
{
  nrElements = source.length();
  Assert(nrElements >= 2, "need at least 2 arguments");
  int nrLeftElements = nrElements / 2;
  ArgVec<DagNode*>::const_iterator splitPoint = source.begin() + nrLeftElements;
  left = AU_StackNode::fwdMake(nrLeftElements, splitPoint);
  right = AU_StackNode::revMake(nrElements - nrLeftElements, splitPoint);

#ifdef CHECK_DEQUE
  checkIntegrity();
#endif
}

inline int
AU_Deque::length() const
{
  return nrElements;
}

inline void
AU_Deque::mark()
{
#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

  if (left != 0)
    left->mark();
  if (right != 0)
    right->mark();
}

inline void
AU_Deque::copyToArgVec(ArgVec<DagNode*>& dest) const
{
#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

  Assert(dest.length() == nrElements, "size mismatch()");
  if (left != 0)
    left->fwdCopy(dest.begin());
  if (right != 0)
    right->revCopy(dest.end());
}

inline DagNode*
AU_Deque::topLeft()
{
#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

  Assert(nrElements > 0, "empty deque");
  if (left == 0)
    {
      if (nrElements == 1)
	{
	  left = right;
	  right = 0;
	}
      else
	right = right->split(nrElements, left);

#ifdef CHECK_DEQUE
      checkIntegrity();
#endif
    }
  return left->first();
}

inline void
AU_Deque::popLeft()
{
#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

  Assert(left != 0, "no elements on left stack");
  left = AU_StackNode::pop(left);
  --nrElements;

#ifdef CHECK_DEQUE
  checkIntegrity();
#endif
}

inline void
AU_Deque::pushLeft(DagNode* dagNode)
{
#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

  left = AU_StackNode::push(left, dagNode);
  ++nrElements;

#ifdef CHECK_DEQUE
  checkIntegrity();
#endif
}

inline void
AU_Deque::pushLeft(const AU_Deque& other)
{
#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

  left = AU_StackNode::prepend(left, other.nrElements, other.left, other.right);
  nrElements += other.nrElements;  // other could be us!

#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

}

inline void
AU_Deque::pushLeft(const ArgVec<DagNode*>& argArray)
{
#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

  int n = argArray.length();
  left = AU_StackNode::fwdJoin(n, argArray.end(), left);
  nrElements += n;

#ifdef CHECK_DEQUE
  checkIntegrity();
#endif
}

inline DagNode*
AU_Deque::topRight()
{
#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

  Assert(nrElements > 0, "empty deque");
  if (right == 0)
    {
      if (nrElements == 1)
	{
	  right = left;
	  left = 0;
	}
      else
	left = left->split(nrElements, right);

#ifdef CHECK_DEQUE
      checkIntegrity();
#endif
    }
  return right->first();
}

inline void
AU_Deque::popRight()
{
#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

  Assert(right != 0, "no elements on right stack");
  right = AU_StackNode::pop(right);
  --nrElements;

#ifdef CHECK_DEQUE
  checkIntegrity();
#endif
}

inline void
AU_Deque::pushRight(DagNode* dagNode)
{
#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

  right = AU_StackNode::push(right, dagNode);
  ++nrElements;

#ifdef CHECK_DEQUE
  checkIntegrity();
#endif
}

inline void
AU_Deque::pushRight(const AU_Deque& other)
{
#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

  right = AU_StackNode::prepend(right, other.nrElements, other.right, other.left);
  nrElements += other.nrElements;  // other could be us!

#ifdef CHECK_DEQUE
  checkIntegrity();
#endif
}

inline void
AU_Deque::pushRight(const ArgVec<DagNode*>& argArray)
{
#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

  int n = argArray.length();
  right = AU_StackNode::revJoin(n, argArray.begin(), right);
  nrElements += n;

#ifdef CHECK_DEQUE
  checkIntegrity();
#endif
}

#endif
