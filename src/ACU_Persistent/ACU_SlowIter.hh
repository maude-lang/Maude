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
//	Class for slow iterators for ACU red-black trees.
//
//	They are slow in the sense that we always keep the full
//	path on the stack.
//
#ifndef _ACU_SlowIter_hh_
#define _ACU_SlowIter_hh_
#include "ACU_Stack.hh"
#include "ACU_Tree.hh"

class ACU_SlowIter : public ACU_Stack
{
  NO_COPYING(ACU_SlowIter);

public:
  ACU_SlowIter();
  ACU_SlowIter(const ACU_Tree& tree);
  
  bool valid() const;
  DagNode* getDagNode() const;
  int getMultiplicity() const;
  void next();
};

inline
ACU_SlowIter::ACU_SlowIter()
{
}

inline
ACU_SlowIter::ACU_SlowIter(const ACU_Tree& tree)
{
  stackLeftmostPath(tree.root);
}

inline bool
ACU_SlowIter::valid() const
{
  return !empty();
}

inline DagNode*
ACU_SlowIter::getDagNode() const
{
  return top()->getDagNode();
}

inline int
ACU_SlowIter::getMultiplicity() const
{
  return top()->getMultiplicity();
}

inline void
ACU_SlowIter::next()
{
  ACU_RedBlackNode* t = top()->getRight();
  if (t != 0)
    stackLeftmostPath(t);
  else
    {
      do
	t = pop();
      while (!empty() && top()->getRight() == t);
    }
}

#endif
