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
//	Functions for rebuilding a the spine of a red-black tree.
//

ACU_RedBlackNode*
ACU_RedBlackNode::copy(ACU_Stack& path,
		       ACU_RedBlackNode* n,
		       ACU_RedBlackNode* old)
{
  int maxMult = 0;
  if (n != 0)
    maxMult = n->getMaxMult();
  //
  //	Rebuild spine.
  //
  while (!(path.empty()))
    {
      ACU_RedBlackNode* parent = path.pop();
      ACU_RedBlackNode* l = parent->getLeft();
      if (parent->getMaxMult() > maxMult)
	{
	  //
	  //	Slow case; need to recalculate maxMult.
	  //
	  int t = parent->getMultiplicity();
	  if (t > maxMult)
	    maxMult = t;
	  if (l == old)
	    {
	      l = n;
	      n = parent->getRight();
	      if (n != 0)
		{
		  t = n->getMaxMult();
		  if (t > maxMult)
		    maxMult = t;
		}
	    }
	  else
	    {
	      if (l != 0)
		{
		  t = l->getMaxMult();
		  if (t > maxMult)
		    maxMult = t;
		}
	    }
	}
      else
	{
	  //
	  //	Fast case.
	  //
	  if (l == old)
	    {
	      l = n;
	      n = parent->getRight();
	    }
	}
      
      n = new ACU_RedBlackNode(parent->getDagNode(),
			       parent->getMultiplicity(),
			       l,
			       n,
			       maxMult);
      n->makeRedIfRed(parent);
      old = parent;
    }
  return n;
}

ACU_RedBlackNode*
ACU_RedBlackNode::copy(ACU_Stack& path,
		       ACU_RedBlackNode* n,
		       ACU_RedBlackNode* old,
		       ACU_RedBlackNode* victim,
		       ACU_RedBlackNode* surrogate)
{
  int maxMult = 0;
  if (n != 0)
    maxMult = n->getMaxMult();
  //
  //	Rebuild spine replacing victim data with surrogate data.
  //
  while (!(path.empty()))
    {
      ACU_RedBlackNode* parent = path.pop();
      ACU_RedBlackNode* l = parent->getLeft();

      ACU_RedBlackNode* source = parent;
      if (source == victim)
	{
	  source = surrogate;
	  //
	  //	surrogate is a descendant of parent.
	  //	Thus parent->getMaxMult() will be at least as
	  //	large as surrogate->getMultiplicity().
	  //
	}

      if (parent->getMaxMult() > maxMult)
	{
	  //
	  //	Slow case; need to recalculate maxMult.
	  //
	  int t = source->getMultiplicity();
	  if (t > maxMult)
	    maxMult = t;
	  if (l == old)
	    {
	      l = n;
	      n = parent->getRight();
	      if (n != 0)
		{
		  t = n->getMaxMult();
		  if (t > maxMult)
		    maxMult = t;
		}
	    }
	  else
	    {
	      if (l != 0)
		{
		  t = l->getMaxMult();
		  if (t > maxMult)
		    maxMult = t;
		}
	    }
	}
      else
	{
	  //
	  //	Fast case.
	  //
	  if (l == old)
	    {
	      l = n;
	      n = parent->getRight();
	    }
	}

      n = new ACU_RedBlackNode(source->getDagNode(),
			       source->getMultiplicity(),
			       l,
			       n,
			       maxMult);
      n->makeRedIfRed(parent);
      old = parent;
    }
  return n;
}
