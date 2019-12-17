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
//      Implementation for class AU_StackNode.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "AU_Persistent.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	AU stack class definitions
#include "AU_StackNode.hh"

//	our stuff
#include "AU_StackArgVec.cc"
#include "AU_StackSort.cc"

#ifdef CHECK_DEQUE

int
AU_StackNode::checkIntegrity(AU_StackNode* p)
{
  int count = 0;
  if (p != 0)
    {
      int i = 0;
      while (p->args[i] == 0)
	{
	  Assert(i < ELEMENTS_PER_NODE - 1, "empty AU_StackNode");
	  ++i;
	}
      while (p != 0)
	{
	  while (i < ELEMENTS_PER_NODE)
	    {
	      DagNode* d = p->args[i];
	      Assert(d != 0, "zero DagNode*");
	      Symbol* s= d->symbol();
	      Assert(s != 0, "zero Symbol*");
	      int nrArgs = s->arity();
	      Assert(nrArgs >= 0 && nrArgs < 20, "bad symbol");
	      ++count;
	      ++i;
	    }
	  i = 0;
	  p = p->next;
	}
    }
  return count;
}

#endif

void
AU_StackNode::mark()
{
  if (isMarked())
    return;
  setMarked();
  for (int i = firstUsed(); i < ELEMENTS_PER_NODE; ++i)
    args[i]->mark();
  for (AU_StackNode* p = next; p != 0 && !(p->isMarked()); p = p->next)
    {
      p->setMarked();
      for (int i = 0; i < ELEMENTS_PER_NODE; ++i)
	p->args[i]->mark();
    }
}	

local_inline AU_StackNode*
AU_StackNode::fwdCopy(int nrElements,
		      AU_StackNode* tail,
		      AU_StackNode const*& sp,
		      int& si)
{
  Assert(nrElements >= 1, "no elements to copy");
  //
  //	Return a stack containing first nrElements elements from (sp, si)
  //	with last next pointer pointing at tail. Return next
  //	element via (sp, si) pair with sp = 0 if no next element.
  //
  int nrElementsInFirstNode = 1 + (nrElements - 1) % ELEMENTS_PER_NODE;
  nrElements -= nrElementsInFirstNode;
  AU_StackNode* start = new AU_StackNode;
  AU_StackNode* dp = start;
  int di = ELEMENTS_PER_NODE - nrElementsInFirstNode;
  for (int i = 0; i < di; i++)
    dp->args[i] = 0;
  for(;;)
    {
      dp->args[di] = sp->args[si];
      if (++si == ELEMENTS_PER_NODE)
	{
	  sp = sp->next;
	  si = 0;
	}
      if (++di == ELEMENTS_PER_NODE)
	{
	  if (nrElements == 0)
	    break;
	  nrElements -= ELEMENTS_PER_NODE;
	  Assert(nrElements >= 0, "not enough elements for new stack node");
	  dp = dp->next = new AU_StackNode;
	  di = 0;
	}
    }
  dp->next = tail;
  return start;
}

local_inline AU_StackNode*
AU_StackNode::revCopy(const AU_StackNode* sp, int si)
{
  Assert(sp != 0, "no elements to copy");
  //
  //	Return a stack containing all the elements from (sp, si)
  //	in reverse order.
  //
  AU_StackNode* dp = 0;
  int di;
  goto start;

  for(;;)
    {
      if (--di < 0)
	{
	start:
	  AU_StackNode* t = new AU_StackNode;
	  t->next = dp;
	  dp = t;
	  di = ELEMENTS_PER_NODE - 1;
	}
      dp->args[di] = sp->args[si];
      if (++si == ELEMENTS_PER_NODE)
	{
	  sp = sp->next;
	  if (sp == 0)
	    break;
	  si = 0;
	}
    }
  while (--di >= 0)
    dp->args[di] = 0;
  return dp;
}

AU_StackNode*
AU_StackNode::split(int nrElements, AU_StackNode*& rev) const
{
  Assert(nrElements >= 2, "need at least 2 elements to split");
  const AU_StackNode* sp = this;
  int si = firstUsed();
  AU_StackNode* fwd = fwdCopy(nrElements / 2, 0, sp, si);
  rev = revCopy(sp, si);
  return fwd;
}

AU_StackNode*
AU_StackNode::pop(AU_StackNode* base)
{
  int f = base->firstUsed();
  if (f == ELEMENTS_PER_NODE - 1)
    return base->next;
  AU_StackNode* p = new AU_StackNode;
  int i = 0;
  do
    p->args[i] =0;
  while (++i <= f);
  do
    p->args[i] = base->args[i];
  while (++i < ELEMENTS_PER_NODE);
  p->next = base->next;
  return p;
}

AU_StackNode*
AU_StackNode::push(AU_StackNode* base, DagNode* dagNode)
{
  AU_StackNode* dp = new AU_StackNode;
  dp->next = base;
  int di = ELEMENTS_PER_NODE - 1;
  if (base != 0)
    {
      int f = base->firstUsed();
      if (f > 0)
	{
	  do
	    dp->args[di] = base->args[di];
	  while (--di >= f);
	  dp->next = base->next;
	}
    }
  Assert(0 <= di && di < ELEMENTS_PER_NODE, "di bad " << di);
  dp->args[di] = dagNode;
  while (--di >= 0)
    dp->args[di] = 0;
  return dp;
}

AU_StackNode*
AU_StackNode::prepend(AU_StackNode* base,
		      int nrElements,
		      const AU_StackNode* fwd,
		      const AU_StackNode* rev)
{
  Assert(nrElements >= 1, "no elements to prepend");
  //
  //	Create a first new stack node and copy in any leftover
  //	element from our first node.
  //
  AU_StackNode* dp = new AU_StackNode;
  dp->next = base;
  int di = ELEMENTS_PER_NODE;
  if (base != 0)
    {
      int f = base->firstUsed();
      if (f > 0)
	{
	  do
	    {
	      --di;
	      dp->args[di] = base->args[di];
	    }
	  while (di > f);
	  dp->next = base->next;
	}
    }
  Assert(1 <= di && di <= ELEMENTS_PER_NODE, "di bad " << di);
  //
  //	There is room for di elements in *dp.
  //
  if (rev != 0)
    {
      //
      //	Prepend contents of rev stack in reverse order.
      //
      --di;
      int ri = rev->firstUsed();
      nrElements -= (ELEMENTS_PER_NODE - ri);
      for(;;)
	{
	  dp->args[di] = rev->args[ri];
	  if (++ri == ELEMENTS_PER_NODE)
	    {
	      rev = rev->next;
	      if (rev == 0)
		break;
	      ri = 0;
	      nrElements -= ELEMENTS_PER_NODE;
	    }
	  if (--di < 0)
	    {
	      AU_StackNode* t = dp;
	      dp = new AU_StackNode;
	      dp->next = t;
	      di = ELEMENTS_PER_NODE - 1;
	    }
	}
      Assert(0 <= di && di < ELEMENTS_PER_NODE, "di bad " << di);
      //
      //	di slots left in *dp; if fwd stack empty, clear them
      //	and return.
      //
      if (fwd == 0)
	{
	  while (--di >= 0)
	    dp->args[di] = 0;
	  return dp;
	}
    }
  Assert(nrElements >= 1, "nrElements bad " << nrElements);
  Assert(0 <= di && di <= ELEMENTS_PER_NODE, "di bad " << di);
  //
  //	We have nrElements elements on the fwd stack. We want to
  //	put the first (nrElements - di) elements into new nodes and
  //	the last di elements (0 padded if nrElements < di) into *dp.
  //
  AU_StackNode* start = dp;
  nrElements -= di;
  if (nrElements > 0)
    {
      int si = fwd->firstUsed();
      start = fwdCopy(nrElements, dp, fwd, si);
    }
  //
  //	Need to copy the last di elements from fwd to *dp; may include
  //	some nulls if fwd didn't contain at least di elements.
  //
  for (int fi = ELEMENTS_PER_NODE - 1; di > 0; --fi)
    {
      --di;
      dp->args[di] = fwd->args[fi];
    }

  return start;
}
