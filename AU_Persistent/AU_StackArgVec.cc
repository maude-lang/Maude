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
//	Stack <-> ArgVec conversion.
//

AU_StackNode*
AU_StackNode::fwdMake(int nrElements,
		      ArgVec<DagNode*>::const_iterator s,
		      AU_StackNode* d)
{
  Assert(nrElements >= 1, "no elements");
  for (;;)
    {
      AU_StackNode* t = d;
      d = new AU_StackNode;
      d->next = t;
      int i = ELEMENTS_PER_NODE - 1;
      if (nrElements < ELEMENTS_PER_NODE)
	{
	  do
	    {
	      --s;
	      d->args[i] = *s;
	      --i;
	    }
	  while (--nrElements > 0);
	  do
	    d->args[i] = 0;
	  while (--i >= 0);
	  break;
	}
      else
	{
	  do
	    {
	      --s;
	      d->args[i] = *s;
	    }
	  while (--i >= 0);
	  nrElements -= ELEMENTS_PER_NODE;
	  if (nrElements == 0)
	    break;
	}
    }
  return d;
}

AU_StackNode*
AU_StackNode::fwdJoin(int nrElements,
		      ArgVec<DagNode*>::const_iterator s,
		      AU_StackNode* d)
{
  Assert(nrElements >= 1, "no elements");
  if (d != 0)
    {
      int f = d->firstUsed();
      if (f > 0)
	{
	  d = d->partialClone(f);
	  do
	    {
	      --s;
	      d->args[--f] = *s;
	      if (--nrElements == 0)
		{
		  while (f > 0)
		    d->args[--f] = 0;
		  return d;
		}
	    }
	  while (f > 0);
	}
    }
  return fwdMake(nrElements, s, d);
}

AU_StackNode*
AU_StackNode::revMake(int nrElements,
		      ArgVec<DagNode*>::const_iterator s,
		      AU_StackNode* d)
{
  Assert(nrElements >= 1, "no elements");
  for (;;)
    {
      AU_StackNode* t = d;
      d = new AU_StackNode;
      d->next = t;
      int i = ELEMENTS_PER_NODE - 1;
      if (nrElements < ELEMENTS_PER_NODE)
	{
	  do
	    {
	      d->args[i] = *s;
	      ++s;
	      --i;
	    }
	  while (--nrElements > 0);
	  do
	    d->args[i] = 0;
	  while (--i >= 0);
	  break;
	}
      else
	{
	  do
	    {
	      d->args[i] = *s;
	      ++s;
	    }
	  while (--i >= 0);
	  nrElements -= ELEMENTS_PER_NODE;
	  if (nrElements == 0)
	    break;
	}
    }
  return d;
}

AU_StackNode*
AU_StackNode::revJoin(int nrElements,
		      ArgVec<DagNode*>::const_iterator s,
		      AU_StackNode* d)
{
  Assert(nrElements >= 1, "no elements");
  if (d != 0)
    {
      int f = d->firstUsed();
      if (f > 0)
	{
	  d = d->partialClone(f);
	  do
	    {
	      
	      d->args[--f] = *s;
	      ++s;
	      if (--nrElements == 0)
		{
		  while (f > 0)
		    d->args[--f] = 0;
		  return d;
		}
	    }
	  while (f > 0);
	}
    }
  return revMake(nrElements, s, d);
}

void
AU_StackNode::fwdCopy(ArgVec<DagNode*>::iterator s) const
{
  const AU_StackNode* d = this;
  int i = firstUsed();
  for(;;)
    {
      *s = d->args[i];
      if (++i == ELEMENTS_PER_NODE)
	{
	  d = d->next;
	  if (d == 0)
	    break;
	  i = 0;
	}
      ++s;
    }
}

void
AU_StackNode::revCopy(ArgVec<DagNode*>::iterator s) const
{
  const AU_StackNode* d = this;
  int i = firstUsed();
  for(;;)
    {
      --s;
      *s = d->args[i];
      if (++i == ELEMENTS_PER_NODE)
	{
	  d = d->next;
	  if (d == 0)
	    break;
	  i = 0;
	}
    }
}
