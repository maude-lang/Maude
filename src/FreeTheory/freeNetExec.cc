/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2026 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for execution of free theory discrimination net.
//
//	We no longer inline this because it would make for large stack frames
//	in eqRewrite() which is highly recursive.
//

bool
FreeNet::applyReplace2(DagNode* subject, RewritingContext& context)
{ 
  //
  //	First traverse discrimination net to find the sequence of equations
  //	(actually remainder pointers) which match the free symbol skeleton in the
  //	subject.
  //
  long i;
  if (!(net.isNull()))  // at least one pattern has free symbols
    {
      DagNode** topArgArray = static_cast<FreeDagNode*>(subject)->argArray();
      Vector<TestNode>::const_iterator netBase = net.begin();
      Vector<TestNode>::const_iterator n = netBase;
      Vector<DagNode**>::iterator stackBase = stack.begin();
      DagNode* d = topArgArray[n->argIndex];
      int symbolIndex = d->symbol()->getIndexWithinModule();
      stack[0] = topArgArray;
      for (;;)
	{
	  long p;
	  int diff = symbolIndex - n->symbolIndex;
	  if (diff != 0)
	    {
	      i = n->notEqual[diff < 0];
	      if (i <= 0)
		{
		  if (i == 0)
		    return false;
		  break;
		}
	      n = netBase + i;
	      p = n->position;
	      if (p < 0)
		continue;
	    }
	  else
	    {
	      long s = n->slot;
	      if (s >= 0)
		stackBase[s] = static_cast<FreeDagNode*>(d)->argArray();
	      i = n->equal;
	      if (i <= 0)
		break;
	      n = netBase + i;
	      p = n->position;
	    }
	  d = stackBase[p][n->argIndex];
	  symbolIndex = d->symbol()->getIndexWithinModule();
	}
      i = ~i;
    }
  else
    {
      if (subject->symbol()->arity() != 0)  // top symbol is not a constant
	stack[0] = static_cast<FreeDagNode*>(subject)->argArray();
      i = 0;
    }
  //
  //	Now go through the sequence of remainders, trying to finish the
  //	matching process for each one in turn.
  //
  Vector<FreeRemainder*>::const_iterator p = fastApplicable[i].begin();
  const FreeRemainder* r = *p;
  do
    {
      if (r->generalMatchReplace(subject, context, stack))
	return true;
    }
  while ((r = *(++p)) != 0);
  return false;
}

bool
FreeNet::lowArityApplyReplace(DagNode* subject, RewritingContext& context)
{
  //
  //	Optimized version of the above that requires disciminiation
  //	net to be non-empty and top symbol and all free symbols appearing
  //	in the net to be low arity.
  //
  long i;
  Vector<TestNode>::const_iterator netBase = net.begin();
  Vector<DagNode**>::iterator stackBase = stack.begin();
  DagNode** topArgArray = static_cast<FreeDagNode*>(subject)->internal;
  Vector<TestNode>::const_iterator n = netBase;
  DagNode* d = topArgArray[n->argIndex];
  int symbolIndex = d->symbol()->getIndexWithinModule();
  stack[0] = topArgArray;
  for (;;)
    {
      long p;
      int diff = symbolIndex - n->symbolIndex;
      if (diff != 0)
	{
	  i = n->notEqual[diff < 0];
	  if (i <= 0)
	    {
	      if (i == 0)
		return false;
	      break;
	    }
	  n = netBase + i;
	  p = n->position;
	  if (p < 0)
	    continue;
	}
      else
	{
	  long s = n->slot;
	  if (s >= 0)
	    stackBase[s] = static_cast<FreeDagNode*>(d)->internal;
	  i = n->equal;
	  if (i <= 0)
	    break;
	  n = netBase + i;
	  p = n->position;
	}
      d = stackBase[p][n->argIndex];
      symbolIndex = d->symbol()->getIndexWithinModule();
    }
  //
  //	Now go through the sequence of remainders, trying to finish the
  //	matching process for each one in turn.
  //
  Vector<FreeRemainder*>::const_iterator p = fastApplicable[~i].begin();
  const FreeRemainder* r = *p;
  do
    {
      if (r->generalMatchReplace(subject, context, stack))
	return true;
    }
  while ((r = *(++p)) != nullptr);
  return false;
}

bool
FreeNet::fastApplyReplace(DagNode* subject, RewritingContext& context)
{
  //
  //	Optimized version of above that further requires that each
  //	fastApplicable list constain a single remainder that is FAST or
  //	SUPER-FAST, which implies no aliens.
  //
  long i;
  Vector<TestNode>::const_iterator netBase = net.begin();
  Vector<DagNode**>::iterator stackBase = stack.begin();
  DagNode** topArgArray = static_cast<FreeDagNode*>(subject)->internal;
  Vector<TestNode>::const_iterator n = netBase;
  DagNode* d = topArgArray[n->argIndex];
  int symbolIndex = d->symbol()->getIndexWithinModule();
  stack[0] = topArgArray;
  for (;;)
    {
      long p;
      int diff = symbolIndex - n->symbolIndex;
      if (diff != 0)
	{
	  i = n->notEqual[diff < 0];
	  if (i <= 0)
	    {
	      if (i == 0)
		return false;
	      break;
	    }
	  n = netBase + i;
	  p = n->position;
	  if (p < 0)
	    continue;
	}
      else
	{
	  long s = n->slot;
	  if (s >= 0)
	    stackBase[s] = static_cast<FreeDagNode*>(d)->internal;
	  i = n->equal;
	  if (i <= 0)
	    break;
	  n = netBase + i;
	  p = n->position;
	}
      d = stackBase[p][n->argIndex];
      symbolIndex = d->symbol()->getIndexWithinModule();
    }
  return fastApplicable[~i][0]->fastMatchReplace(subject, context, stack);
}

bool
FreeNet::superFastApplyReplace(DagNode* subject, RewritingContext& context)
{
  //
  //	Optimized version of the above that further requires that each
  //	remainder be SUPER-FAST, which implies no aliens.
  //
  long i;
  Vector<TestNode>::const_iterator netBase = net.begin();
  Vector<DagNode**>::iterator stackBase = stack.begin();
  DagNode** topArgArray = static_cast<FreeDagNode*>(subject)->internal;
  Vector<TestNode>::const_iterator n = netBase;
  DagNode* d = topArgArray[n->argIndex];
  int symbolIndex = d->symbol()->getIndexWithinModule();
  stack[0] = topArgArray;
  for (;;)
    {
      long p;
      int diff = symbolIndex - n->symbolIndex;
      if (diff != 0)
	{
	  i = n->notEqual[diff < 0];
	  if (i <= 0)
	    {
	      if (i == 0)
		return false;
	      break;
	    }
	  n = netBase + i;
	  p = n->position;
	  if (p < 0)
	    continue;
	}
      else
	{
	  long s = n->slot;
	  if (s >= 0)
	    stackBase[s] = static_cast<FreeDagNode*>(d)->internal;
	  i = n->equal;
	  if (i <= 0)
	    break;
	  n = netBase + i;
	  p = n->position;
	}
      d = stackBase[p][n->argIndex];
      symbolIndex = d->symbol()->getIndexWithinModule();
    }
  return fastApplicable[~i][0]->superFastMatchReplace(subject, context, stack);
}

bool
FreeNet::generalNullNet(DagNode* subject, RewritingContext& context)
{
  //
  //	Case where the discrimination net is empty and the top symbol has
  //	low arity.
  //
  stack[0] = static_cast<FreeDagNode*>(subject)->internal;
  Vector<FreeRemainder*>::const_iterator p = fastApplicable[0].begin();
  const FreeRemainder* r = *p;
  do
    {
      if (r->generalMatchReplace(subject, context, stack))
	return true;
    }
  while ((r = *(++p)) != nullptr);
  return false;
}

bool
FreeNet::fastNullNet(DagNode* subject, RewritingContext& context)
{
  //
  //	Optimized version of the above for a single fast remainder.
  //
  stack[0] = static_cast<FreeDagNode*>(subject)->internal;
  return fastApplicable[0][0]->fastMatchReplace(subject, context, stack);
}

bool
FreeNet::superFastNullNet(DagNode* subject, RewritingContext& context)
{
  //
  //	Optimized version of the above for a single super-fast remainder.
  //
  stack[0] = static_cast<FreeDagNode*>(subject)->internal;
  return fastApplicable[0][0]->superFastMatchReplace(subject, context, stack);
}

bool
FreeNet::applyReplaceNoOwise2(DagNode* subject, RewritingContext& context)
{
  //
  //	This is needed for non-final 0s in complex strategies.
  //
  //	First traverse discrimination net to find the sequence of equations
  //	(actually remainder pointers) which match the free symbol skeleton in the
  //	subject.
  //
  long i;
  if (!(net.isNull()))  // at least one pattern has free symbols
    {
      DagNode** topArgArray = static_cast<FreeDagNode*>(subject)->argArray();
      Vector<TestNode>::const_iterator netBase = net.begin();
      Vector<TestNode>::const_iterator n = netBase;
      Vector<DagNode**>::iterator stackBase = stack.begin();
      DagNode* d = topArgArray[n->argIndex];
      int symbolIndex = d->symbol()->getIndexWithinModule();
      stack[0] = topArgArray;
      for (;;)
	{
	  long p;
	  int diff = symbolIndex - n->symbolIndex;
	  if (diff != 0)
	    {
	      i = n->notEqual[diff < 0];
	      if (i <= 0)
		{
		  if (i == 0)
		    return false;
		  break;
		}
	      n = netBase + i;
	      p = n->position;
	      if (p < 0)
		continue;
	    }
	  else
	    {
	      long s = n->slot;
	      if (s >= 0)
		stackBase[s] = static_cast<FreeDagNode*>(d)->argArray();
	      i = n->equal;
	      if (i <= 0)
		break;
	      n = netBase + i;
	      p = n->position;
	    }
	  d = stackBase[p][n->argIndex];
	  symbolIndex = d->symbol()->getIndexWithinModule();
	}
      i = ~i;
    }
  else
    {
      if (subject->symbol()->arity() != 0)  // top symbol is not a constant
	stack[0] = static_cast<FreeDagNode*>(subject)->argArray();
      i = 0;
    }
  //
  //	Now go through the sequence of remainders, trying to finish the
  //	matching process for each one in turn; if we encounter a remainder
  //	belonging to an owise equation we quit.
  //
  Vector<FreeRemainder*>::const_iterator p = fastApplicable[i].begin();
  const FreeRemainder* r = *p;
  do
    {
      if (r->isOwise())
	break;
      if (r->generalMatchReplace(subject, context, stack))
	return true;
    }
  while ((r = *(++p)) != 0);
  return false;
}
