/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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

void
RewritingContext::ruleRewrite(Int64 limit)
{
  Vector<RedexPosition> redexStack;
  for (Int64 nrRewrites = 0;; nrRewrites++)
    {
      reduce();
      if (nrRewrites == limit)
	return;
      redexStack.clear();
      redexStack.append(RedexPosition(rootNode, UNDEFINED, UNDEFINED, true));

      int nextToExplore = 0;
      int finish = redexStack.length();
      for (int nextToRewrite = 0;; nextToRewrite++)
	{
	  if (nextToRewrite == finish)
	    {
	      //
	      //	No dag nodes to rewrite so stack the arguments of a dag node.
	      //
	      for (;;)
		{
		  if (nextToExplore == finish)
		    return;
		  DagNode* d = redexStack[nextToExplore].node();
		  /*
		  DebugAdvisory("Exploring " << d <<
				" unrewritable = " << d->isUnrewritable() <<
				" unstackable = " << d->isUnstackable());
		  */
		  //
		  //	Only want to try rewriting one copy of repeated arguments
		  //	where possible.
		  //
		  d->symbol()->stackPhysicalArguments(d, redexStack, nextToExplore);
		  ++nextToExplore;
		  int len = redexStack.length();
		  if (len > finish)
		    {
		      finish = len;
		      break;
		    }
		  //
		  //	Because redexStack didn't grow, all of d's arguments must
		  //	be unstackable. We might assume that d is unrewritable otherwise
		  //	we would have already rewritten it; but for safety in the future
		  //	(say if we use this routine in conjunction with objects that
		  //	couldn't rewrite at some point but might become rewritable in the
		  //	future) we do an explicit check.
		  //
		  if (d->isUnrewritable())
		    d->setUnstackable();
		}
	    }

	  DagNode* d = redexStack[nextToRewrite].node();
	  if (d->isUnrewritable())
	    continue;
	  DagNode* r = d->symbol()->ruleRewrite(d, *this);
	  if (r != 0)
	    {
	      int argIndex = redexStack[nextToRewrite].argIndex();
	      DagNode* p = r;
	      for (int i = redexStack[nextToRewrite].parentIndex(); i != UNDEFINED;)
		{
		  RedexPosition& rp = redexStack[i];
		  p = rp.node()->copyWithReplacement(argIndex, p);
		  argIndex = rp.argIndex();
		  i = rp.parentIndex();
		}
	      rootNode = p;
	      if (traceFlag)
		{
		  if (traceAbort())
		    return;
		  tracePostRuleRewrite(r);
		}
	      MemoryCell::okToCollectGarbage();
	      break;
	    }
	}
    }
}

void
RewritingContext::fairRewrite(Int64 limit, Int64 gas)
{
  rewriteLimit = limit;
  gasPerNode = gas;
  currentIndex = 0;
  lazyMarker = NONE;

  reduce();
  redexStack.clear();
  redexStack.append(RedexPosition(rootNode, UNDEFINED, UNDEFINED, true));

  do
    {
      progress = false;
      if (fairTraversal())
	return ;  // we hit our rewrite limit
    }
  while(progress);
  return;  // no more redexes
}

void
RewritingContext::fairContinue(Int64 limit)
{
  if (progress)
    {
      rewriteLimit = limit;
      if (fairTraversal())
	return;  // we hit our rewrite limit
      do
	{
	  progress = false;
	  if (fairTraversal())
	    return;  // we hit our rewrite limit
	}
      while (progress);
    }
  return;  // no more redexes 
}

void
RewritingContext::fairStart(Int64 limit, Int64 gas)
{
  rewriteLimit = limit;
  gasPerNode = gas;
  currentIndex = 0;
  lazyMarker = NONE;

  reduce();
  redexStack.clear();
  redexStack.append(RedexPosition(rootNode, UNDEFINED, UNDEFINED, true));
}

bool
RewritingContext::fairTraversal()
{
  //
  //	Return true if we stop mid-traversal because of rewrite limit
  //	and false if traversal completed.
  //
  bool argsUnstackable;
  if (currentIndex == 0) 
    {
      //
      //	Either this is our first traversal or we just completed a traversal.
      //	Either way we descend the leftmost path to start a new traveral.
      //
      descend();
      argsUnstackable = true;
      currentGas = gasPerNode;
      progress = false;
    }
  else
    argsUnstackable = (redexStack[currentIndex].node()->symbol()->arity() == 0);

  while (!doRewriting(argsUnstackable))  // until we hit rewrite limit
    {
      if (currentIndex == 0)
	return false;  // end of traversal
      //
      //	Check if we are leaving the node pointed at by the
      //	lazy marker.
      //
      if (currentIndex == lazyMarker)
	lazyMarker = NONE;
      //
      //	Find next potentially rewritable node using depth-first
      //	traversal by checking for a right sibling on the stack,
      //	and failing that ascending.
      //
      if (currentIndex + 1 < redexStack.length())
	{
	  Assert(redexStack[currentIndex].parentIndex() ==
		 redexStack[currentIndex + 1].parentIndex(),
		 "redexStack corrupt");
	  ++currentIndex;  // across to sibling
	  descend();
	  argsUnstackable = true;
	}
      else
	argsUnstackable = ascend();
      currentGas = gasPerNode;
    }
  return true;
}

bool
RewritingContext::ascend()
{
  //
  //	Go up one node.
  //	Return true if new current node is guaranteed to have all args unstackable.
  //
  //	Because we only ascend when we have no right sibling on the stack
  //	we can assume current node is the last stacked argument of its parent.
  //
  int last = currentIndex;
  //
  //	Parent becomes current.
  //
  currentIndex = redexStack[currentIndex].parentIndex();
  //
  //	Now we need to find first stacked argument of parent node.
  //
  int first = last;
  while (redexStack[first - 1].parentIndex() == currentIndex)
    --first;
  //
  //	Rebuild parent node if it is stale.
  //
  RedexPosition& rp = redexStack[currentIndex];
  if (staleMarker == currentIndex)
    {
      rp.replaceNode(rp.node()->copyWithReplacement(redexStack, first, last));
      staleMarker = rp.parentIndex();  // our parent now becomes stale
      MemoryCell::okToCollectGarbage();
    }
  //
  //	Reduce node if needed and check for unstackable args.
  //
  DagNode* d = rp.node();
  bool argsUnstackable;
  if (lazyMarker != NONE || d->isReduced())
    {
      argsUnstackable = true;
      for (int i = first; i <= last; i++)
	{
	  if (!(redexStack[i].node()->isUnstackable()))
	    {
	      argsUnstackable = false;
	      break;
	    }
	}
    }
  else
    {
      d->reduce(*this);
      argsUnstackable = (d->symbol()->arity() == 0);
    }
  //
  //	Remove arguments of new current node from the stack.
  //
  redexStack.contractTo(first);
  return argsUnstackable;
}

void
RewritingContext::descend()
{
  //
  //	Finds leftmost leaf (or quasi-leaf) below currentIndex.
  //	Updates redexStack, currentIndex and lazyMarker.
  //
  for (;;)
    {
      DagNode* d = redexStack[currentIndex].node();
      //
      //	Whenever we arrive at a non-eager node, if the lazy
      //	marker is not set, we point it at this node. Then
      //	until we leave this node and its descendents we must not
      //	do equational rewriting.
      //
      if (lazyMarker == NONE && !(redexStack[currentIndex].isEager()))
	lazyMarker = currentIndex;
      //
      //	We ask the dag nodes symbol to stack those arguments
      //	which are not either frozen or unstackable.
      //
      int first = redexStack.length();
      d->symbol()->stackArguments(d, redexStack, currentIndex);
      //
      //	If no new dag nodes were stacked we hit a quasi-leaf.
      //
      if (first == redexStack.length())
	break;
      //
      //	Otherwise set current node to the first (leftmost)
      //	stacked argument and continue down the term.
      //
      currentIndex = first;
    }
}

bool
RewritingContext::doRewriting(bool argsUnstackable)
{
  //
  //	Tries to rewrite node at currentIndex.
  //	Returns true if we stop because of rewrite limit.
  //	Return false we stop because we ran out of gas or because
  //	no more rewrites are possible.
  //	Updates redexStack, currentGas, nrRewritesAllowed, progress.
  //	Only routine in fair rewriting that sets unstackable flag.
  //
  DagNode* d = redexStack[currentIndex].node();
  if (d->isUnrewritable())
    {
      //
      //	*d's unrewritable flag could have been set in an
      //	earlier traversal when it had a stackable argument.
      //	If no rewrites occurred in this stackable argument it
      //	would eventually become unstackable and *d would never
      //	become stale and never be replaced. So we need to check
      //	for this case.
      //
      if (argsUnstackable)
	d->setUnstackable();
    }
  else
    {
      Symbol* s = d->symbol();
      while (currentGas > 0)
	{
	  DagNode* r = s->ruleRewrite(d, *this);
	  if (r == 0)
	    {
	      //
	      //	Just because we couldn't do any rewrites it doesn't follow
	      //	that we will never be able to do rewrites here - some external
	      //	agency might force a rewrite; so we check to see if the
	      //	unrewritable flag was set by ruleRewrite(). This virtual function
	      //	will be aware of any external agency that might affect its symbol
	      //	and will act accordingly.
	      //
	      if (argsUnstackable && d->isUnrewritable())
		d->setUnstackable();
	      break;
	    }
	  progress = true;
	  d = r;
	  redexStack[currentIndex].replaceNode(r);
	  staleMarker = redexStack[currentIndex].parentIndex();
	  MemoryCell::okToCollectGarbage();
	  if (traceFlag)
	    {
	      if (traceAbort())
		return true;  // pretend we hit rewrite limit
	      tracePostRuleRewrite(r);
	    }
	  if (lazyMarker == NONE)
	    d->reduce(*this);
	  --currentGas;
	  if (rewriteLimit != NONE && --rewriteLimit == 0)
	    return true;
	  s = d->symbol();
	  argsUnstackable = (s->arity() == 0);
	}
    }
  return false;
}
