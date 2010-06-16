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
//	Class for fast iterators for ACU red-black trees.
//
//	They are fast in the sense that we do the explicit
//	stack version of tail recursion elimination and therefore
//	may not have a full path on the stack.
//
#ifndef _ACU_FastIter_hh_
#define _ACU_FastIter_hh_
#include "ACU_Stack.hh"
#include "ACU_Tree.hh"

class ACU_FastIter : private ACU_Stack
{
  NO_COPYING(ACU_FastIter);

public:
  ACU_FastIter(const ACU_Tree& tree);
  
  bool valid() const;
  DagNode* getDagNode() const;
  int getMultiplicity() const;
  void next();
};

inline
ACU_FastIter::ACU_FastIter(const ACU_Tree& tree)
{
  stackLeftmostPath(tree.root);
}

inline bool
ACU_FastIter::valid() const
{
  return !empty();
}

inline DagNode*
ACU_FastIter::getDagNode() const
{
  return top()->getDagNode();
}

inline int
ACU_FastIter::getMultiplicity() const
{
  return top()->getMultiplicity();
}

inline void
ACU_FastIter::next()
{
  if (ACU_RedBlackNode* r = pop()->getRight())
    stackLeftmostPath(r);
}

#endif
