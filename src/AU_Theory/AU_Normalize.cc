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
//	Normalization for AU_DagNodes.
//

bool
AU_DagNode::disappear(AU_Symbol* s, ArgVec<DagNode*>::const_iterator i)
{
  return (s->rightId() && i != argArray.begin()) ||
    (s->leftId() && i + 1 != argArray.end());
}

AU_DagNode::NormalizationResult
AU_DagNode::normalizeAtTop(bool dumb)
{
  AU_Symbol* s = symbol();
  Term* identity = s->getIdentity();
  int expansion = 0;
  int nrIdentities = 0;
  int maxDeque = 0;
  ArgVec<DagNode*>::const_iterator maxDequeIter;  // gcc gives uninitialized warning
  //
  //	First examine the argument list looking for either our top symbol
  //	or our identity.
  //
  {
    FOR_EACH_CONST(i, ArgVec<DagNode*>, argArray)
      {
	DagNode* d = *i;
	if (d->symbol() == s)
	  {
	    if (safeCast(AU_BaseDagNode*, d)->isDeque())
	      {
		//
		//	Since we have at least one deque we won't use
		//	expansion and therefore don't waste time
		//	updating it.
		//
		int nrArgs = safeCast(AU_DequeDagNode*, d)->nrArgs();
		if (nrArgs > maxDeque)
		  {
		    maxDeque = nrArgs;
		    maxDequeIter = i;
		  }
	      }
	    else
	      expansion += safeCast(AU_DagNode*, d)->argArray.length() - 1;
	  }
	else if (identity != 0 && identity->equal(d) && disappear(s, i))
	  ++nrIdentities;
      }
  }
  //
  //	Now deal efficiently with all the special cases.
  //
  if (maxDeque > 0)
    {
      //
      //	We had at least 1 deque in argument list. We push
      //	all the remaining arguments onto the largest such
      //	deque. We never deal with one sided identities in this
      //	case.
      //
      AU_Deque flat(safeCast(AU_DequeDagNode*, *maxDequeIter)->getDeque());
      {
	//
	//	Push stuff on the left of the largest deque.
	//
	const ArgVec<DagNode*>::const_iterator b = argArray.begin();
	for (ArgVec<DagNode*>::const_iterator i = maxDequeIter; i != b;)
	  {
	    --i;
	    DagNode* d = *i;
	    if (d->symbol() == s)
	      {
		if (safeCast(AU_BaseDagNode*, d)->isDeque())
		  flat.pushLeft(safeCast(AU_DequeDagNode*, d)->getDeque());
		else
		  flat.pushLeft(safeCast(AU_DagNode*, d)->argArray);
	      }
	    else if (!(nrIdentities > 0 && identity->equal(d)))
	      flat.pushLeft(d);
	  }
      }
      {
	//
	//	Push stuff on the right of the largest deque.
	//
	const ArgVec<DagNode*>::const_iterator e = argArray.end();
	for (ArgVec<DagNode*>::const_iterator i = maxDequeIter + 1; i != e; ++i)
	  {
	    DagNode* d = *i;
	    if (d->symbol() == s)
	      {
		if (safeCast(AU_BaseDagNode*, d)->isDeque())
		  flat.pushRight(safeCast(AU_DequeDagNode*, d)->getDeque());
		else
		  flat.pushRight(safeCast(AU_DagNode*, d)->argArray);
	      }
	    else if (!(nrIdentities > 0 && identity->equal(d)))
	      flat.pushRight(d);
	  }
      }
      (void) new (this) AU_DequeDagNode(symbol(), flat);
      Assert(!dumb, "shouldn't be here if we're in dumb mode");
      MemoryCell::okToCollectGarbage();  // needed because of pathological nesting
      return DEQUED;
    }
  if (expansion == 0)
    {
      //
      //	No flattening.
      //
      if (nrIdentities != 0)
	{
	  //
	  //	But have to remove identities.
	  //
	  int p = 0;
	  FOR_EACH_CONST(i, ArgVec<DagNode*>, argArray)
	    {
	      DagNode* d = *i;
	      if (!(identity->equal(d) && disappear(s, i)))
		argArray[p++] = d;
	    }
	  if (p < 2)
	    {
	      //
	      //	Eliminating identity causes AU dag node to collapse to its
	      //	remaining argument or 1st argument of all arguments were
	      //	identity.
	      //
	      DagNode* remaining = (s->getPermuteStrategy() == BinarySymbol::EAGER) ?
		argArray[0] : argArray[0]->copyReducible();
	      remaining->overwriteWithClone(this);
	      return COLLAPSED;
	    }
	  argArray.contractTo(p);
	}
      return NORMAL;
    }
  //
  //	We need to flatten.
  //
  int nrArgs = argArray.length();
  if (nrIdentities == 0)
    {
      //
      //	No identities: in place flattening.
      //
      argArray.expandBy(expansion);
      int p = nrArgs + expansion - 1;
      for (int i = nrArgs - 1; i >= 0; i--)
	{
	  Assert(p >= i, "loop invarient failed");
	  DagNode* d = argArray[i];
	  if (d->symbol() == s)
	    {
	      ArgVec<DagNode*>& argArray2 = safeCast(AU_DagNode*, d)->argArray;
	      for (int j = argArray2.length() - 1; j >= 0; j--)
		argArray[p--] = argArray2[j];
	    }
	  else
	    argArray[p--] = argArray[i];
	}
      Assert(p == -1, "bad argArray length");
    }
  else
    {
      //
      //	Identities: flatten into auxiliary buffer.
      //
      ArgVec<DagNode*> buffer(nrArgs + expansion - nrIdentities);
      int p = 0;
      FOR_EACH_CONST(i, ArgVec<DagNode*>, argArray)
	{
	  DagNode* d = *i;
	  if (d->symbol() == s)
	    {
	      ArgVec<DagNode*>& argArray2 = safeCast(AU_DagNode*, d)->argArray;
	      FOR_EACH_CONST(j, ArgVec<DagNode*>, argArray2)
		buffer[p++] = *j;
	    }
	  else if (!(identity->equal(d) && disappear(s, i)))
	    buffer[p++] = d;
	}
      Assert(p == nrArgs + expansion - nrIdentities, "bad buffer size");
      argArray.swap(buffer);
    }
  //
  //	If we're in dumb mode then there may be a lot of partially built
  //	instantiated dags lying around that are vunerable to garbage collection.
  //
  if (!dumb)
    MemoryCell::okToCollectGarbage();  // needed because of pathological nesting
  //
  //	This is the one place where deques can come into existence.
  //
  if (!dumb && s->useDeque())
    {
      AU_Deque flat(argArray);
      (void) new (this) AU_DequeDagNode(symbol(), flat);
      return DEQUED;
    }
  return FLATTENED;
}
