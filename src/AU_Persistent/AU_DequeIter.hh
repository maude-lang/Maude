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
//	Class for iterators for AU deques.
//
#ifndef _AU_DequeIter_hh_
#define _AU_DequeIter_hh_
#include "AU_Deque.hh"

class AU_DequeIter
{
  NO_COPYING(AU_DequeIter);

public:
  AU_DequeIter(const AU_Deque& deque);
  
  bool valid() const;
  DagNode* getDagNode() const;
  void next();

private:
  void reverseRight();

  int nrElementsRemaining;
  AU_StackNode* current;
  AU_StackNode* right;
  int index;
  Vector<AU_StackNode*> reversed;
  int revIndex;
};

inline
AU_DequeIter::AU_DequeIter(const AU_Deque& deque)
{
  nrElementsRemaining = deque.nrElements;
  right = deque.right;

  current = deque.left;
  if (current == 0)
    reverseRight();
  else
    {
      revIndex = -1;
      index = current->firstUsed();
    }
}

inline bool
AU_DequeIter::valid() const
{
  return nrElementsRemaining > 0;
}

inline DagNode*
AU_DequeIter::getDagNode() const
{
  return current->args[index];
}

inline void
AU_DequeIter::next()
{
  if (--nrElementsRemaining > 0)
    {
      if (revIndex < 0)
	{
	  if (++index == AU_StackNode::ELEMENTS_PER_NODE)
	    {
	      current = current->next;
	      if (current == 0)
		reverseRight();
	      else
		index = 0;
	    }
	}
      else
	{
	  if (--index < 0)
	    {
	      current = reversed[--revIndex];
	      index = AU_StackNode::ELEMENTS_PER_NODE - 1;
	    }
	}
    }
}

#endif
