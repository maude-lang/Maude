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
//	Class for stack of pointers to red-black nodes.
//
#ifndef _ACU_Stack_hh_
#define _ACU_Stack_hh_
#include "ACU_RedBlackNode.hh"

class ACU_Stack
{
  NO_COPYING(ACU_Stack);
  
public:
  ACU_Stack();
  
  bool empty() const;
  ACU_RedBlackNode* top() const;
  
  void clear();
  void push(ACU_RedBlackNode* node);
  ACU_RedBlackNode* pop();
  void unpop();  // only valid if no clear() or push() since last pop()
  void multiPop(int nr);
  void stackLeftmostPath(ACU_RedBlackNode* n);
  void save();
  void restore();
 
private:
  enum Sizes
  {
    STACK_SIZE = ACU_RedBlackNode::MAX_TREE_HEIGHT
  };

  ACU_RedBlackNode** ptr;
  ACU_RedBlackNode** old;
  ACU_RedBlackNode* base[STACK_SIZE];
};

inline
ACU_Stack::ACU_Stack()
{
  ptr = base;
}

inline bool
ACU_Stack::empty() const
{
  return ptr == base;
}

inline ACU_RedBlackNode*
ACU_Stack::top() const
{
  Assert(ptr > base, "ACU stack underflow");
  return *(ptr - 1);
}

inline void
ACU_Stack::clear()
{
  ptr = base;
}

inline void
ACU_Stack::push(ACU_RedBlackNode* node)
{
  Assert(ptr < base + STACK_SIZE, "ACU stack overflow");
  *ptr++ = node;
}

inline ACU_RedBlackNode*
ACU_Stack::pop()
{
  Assert(ptr > base, "ACU stack underflow");
  return *(--ptr);
}

inline void
ACU_Stack::unpop()
{
  Assert(ptr < base + STACK_SIZE, "ACU stack overflow");
  ++ptr;
}

inline void
ACU_Stack::multiPop(int nr)
{
  Assert(ptr - nr >= base, "ACU stack underflow");
  ptr -= nr;
}

inline void
ACU_Stack::stackLeftmostPath(ACU_RedBlackNode* n)
{
  while (n != 0)  // might be optimized
    {
      push(n);
      n = n->getLeft();
    }
}

inline void
ACU_Stack::save()
{
  old = ptr;
}

inline void
ACU_Stack::restore()
{
  ptr = old;
}

#endif
