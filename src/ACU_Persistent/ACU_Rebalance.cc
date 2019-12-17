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
//	Copy spine of a red-black tree, rebalancing.
//
//	We use the rebalancing method given in chapter 14 of
//	  Thomas H. Cormen, Charles E. Leierson and Ronald L. Rivest,
//	  "Introduction to Algorithms",
//	  MIT Press and McGraw-Hill, 1990.
//
//	We need to handle 2 * 2 * 3 = 12 distinct cases.
//

ACU_RedBlackNode*
ACU_RedBlackNode::copyRebalance(ACU_Stack& path,
				ACU_RedBlackNode* a,
				ACU_RedBlackNode* old,
				ACU_RedBlackNode* victim,
				ACU_RedBlackNode* surrogate)
{
  //
  //	Swap surrogate for victim and increase black length by 1.
  //	a is a new black node or possibly null.
  //	old is the node is it is replacing in the rebuilt tree.
  //
  ACU_RedBlackNode* b;
  for(;;)
    {
      b = path.pop();  // old's parent
      ACU_RedBlackNode* b2 = (b == victim) ? surrogate : b;
      
      if (b->getLeft() == old)
	{
	  //
	  //	a will replace b->getLeft() in the rebuilt tree. Thus every
	  //	path from b->getRight() must contain at least 1 black node.
	  //	Therefore d = b->getRight() exists.
	  //
	  ACU_RedBlackNode* d = b->getRight();  // old a's sibling
	  ACU_RedBlackNode* c = d->getLeft();  // might be null
	  ACU_RedBlackNode* e = d->getRight();  // might be null
	  if (d->isRed())
	    {
	      //
	      //	CLR case 1; we consider 3 subcases.
	      //
	      //	Since d is red, every path from d must contain
	      //	at least 1 black node; so c = d->getLeft()
	      //	and e = d->getRight() exist.
	      //
	      ACU_RedBlackNode* p = c->getLeft();  // might be null
	      ACU_RedBlackNode* q = c->getRight();  // might be null
	      ACU_RedBlackNode* t;
	      if (isRed(q))
		{
		  //
		  //	Transformation 1 followed by transformation 4.
		  //
		  t = redNode(c,
			      blackNode(b2, a, p),
			      blackNode(q, q->getLeft(), q->getRight()));
		}
	      else if (isRed(p))
		{
		  //
		  //	Transformation 1 followed by transformation 3
		  //	followed by transformation 4.
		  //
		  t = redNode(p,
			      blackNode(b2, a, p->getLeft()),
			      blackNode(c, p->getRight(), q));
		}
	      else
		{
		  //
		  //	Transformation 1 followed by transformation 2.
		  //
		  t = blackNode(b2, a, redNode(c, p, q));
		}
	      a = blackNode(d, t, e);
	      break;
	    }
	  else // d is black
	    {
	      if (isRed(e))
		{
		  //
		  //	CLR case 4.
		  //
		  a = blackNode(d,
				blackNode(b2, a, c),
				blackNode(e, e->getLeft(), e->getRight()));
		}
	      else if (isRed(c))
		{
		  //
		  //	CLR case 3; we do a combined 3 then 4 transformation.
		  //
		  a = blackNode(c,
				blackNode(b2, a, c->getLeft()),
				blackNode(d, c->getRight(), e));
		}
	      else
		{
		  //
		  //	CLR case 2.
		  //
		  a = blackNode(b2, a, redNode(d, c, e));
		  goto up;
		}
	      a->makeRedIfRed(b);
	      break;
	    }
	}
      else
	{
	  //
	  //	Reflection of previous branch.
	  //
	  //
	  //	a will replace b->getRight() in the rebuilt tree. Thus every
	  //	path from b->getLeft() must contain at least 1 black node.
	  //	Therefore d = b->getLeft() exists.
	  //
	  ACU_RedBlackNode* d = b->getLeft();  // old a's sibling
	  ACU_RedBlackNode* c = d->getRight();  // might be null
	  ACU_RedBlackNode* e = d->getLeft();  // might be null
	  if (d->isRed())
	    {
	      //
	      //	CLR case 1; we consider 3 subcases.
	      //
	      //	Since d is red, every path from d must contain
	      //	at least 1 black node; so c = d->getRight()
	      //	and e = d->getLeft() exist.
	      //
	      ACU_RedBlackNode* p = c->getRight();  // might be null
	      ACU_RedBlackNode* q = c->getLeft();  // might be null
	      ACU_RedBlackNode* t;
	      if (isRed(q))
		{
		  //
		  //	Transformation 1 followed by transformation 4.
		  //
		  t = redNode(c,
			      blackNode(q, q->getLeft(), q->getRight()),
			      blackNode(b2, p, a));
		}
	      else if (isRed(p))
		{
		  //
		  //	Transformation 1 followed by transformation 3
		  //	followed by transformation 4.
		  //
		  t = redNode(p,
			      blackNode(c, q, p->getLeft()),
			      blackNode(b2, p->getRight(), a));
		}
	      else
		{
		  //
		  //	Transformation 1 followed by transformation 2.
		  //
		  t = blackNode(b2, redNode(c, q, p), a);
		}
	      a = blackNode(d, e, t);
	      break;
	    }
	  else // d is black
	    {
	      if (isRed(e))
		{
		  //
		  //	CLR case 4.
		  //
		  a = blackNode(d,
				blackNode(e, e->getLeft(), e->getRight()),
				blackNode(b2, c, a));
		}
	      else if (isRed(c))
		{
		  //
		  //	CLR case 3; we do a combined 3 then 4 transformation.
		  //
		  a = blackNode(c,
				blackNode(d, e, c->getLeft()),
				blackNode(b2, c->getRight(), a));
		}
	      else
		{
		  //
		  //	CLR case 2.
		  //
		  a = blackNode(b2, redNode(d, e, c), a);
		  goto up;
		}
	      a->makeRedIfRed(b);
	      break;
	    }
	}
    up:
      if (path.empty())
	return a;
      if (b->isRed())
	return copy(path, a, b, victim, surrogate);
      old = b;
    }
  if (path.empty())
    return a;
  return copy(path, a, b, victim, surrogate);
}
