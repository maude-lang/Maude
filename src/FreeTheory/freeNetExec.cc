/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	We don't inline this because it would make for large stack frames
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
  Index i;
  if (!(net.isNull()))  // at least one pattern has free symbols
    {
      DagNode** const topArgArray = static_cast<FreeDagNode*>(subject)->argArray();
      const Vector<TestNode>::const_iterator netBase = net.begin();
      const Vector<DagNode**>::iterator stackBase = stack.begin();
      Vector<TestNode>::const_iterator n = netBase;
      DagNode* d = topArgArray[n->argIndex];
      stack[0] = topArgArray;
      for (;;)
	{
	  const TestNode action = n[d->symbol()->getMatchIndex()];  // load both fields
	  const Index s = action.slotIndex;
	  if (s >= 0)
	    stackBase[s] = static_cast<FreeDagNode*>(d)->argArray();
	  i = action.nodeIndex;
	  if (i <= 0)
	    {
	      if (i == 0)
		return false;
	      break;
	    }
	  n = netBase + i;
	  d = stackBase[n->slotIndex][n->argIndex];
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
      if (r->fastMatchReplace(subject, context, stack))
	return true;
    }
  while ((r = *(++p)) != 0);
  return false;
}

bool
FreeNet::applyReplaceFast2(DagNode* subject, RewritingContext& context)
{
  //
  //	Optimized version of the the above that only works for top
  //	symbols with at least one argument and internal storage.
  //
  Index i;
  DagNode** topArgArray = static_cast<FreeDagNode*>(subject)->internal;
  stack[0] = topArgArray;
  if (!(net.isNull()))  // at least one pattern has free symbols
    {
      const Vector<TestNode>::const_iterator netBase = net.begin();
      const Vector<DagNode**>::iterator stackBase = stack.begin();
      Vector<TestNode>::const_iterator n = netBase;
      DagNode* d = topArgArray[n->argIndex];
      for (;;)
	{
	  const TestNode action = n[d->symbol()->getMatchIndex()];  // load both fields
	  const Index s = action.slotIndex;
	  if (s >= 0)
	    stackBase[s] = static_cast<FreeDagNode*>(d)->argArray();
	  i = action.nodeIndex;
	  if (i <= 0)
	    {
	      if (i == 0)
		return false;
	      break;
	    }
	  n = netBase + i;
	  d = stackBase[n->slotIndex][n->argIndex];
	}
      i = ~i;
    }
  else
    i = 0;
  //
  //	Now go through the sequence of remainders, trying to finish the
  //	matching process for each one in turn.
  //
  Vector<FreeRemainder*>::const_iterator p = fastApplicable[i].begin();
  const FreeRemainder* r = *p;
  do
    {
      if (r->fastMatchReplace(subject, context, stack))
	return true;
    }
  while ((r = *(++p)) != 0);
  return false;
}

bool
FreeNet::applyReplaceNoOwise2(DagNode* subject, RewritingContext& context)
{
  //
  //	First traverse discrimination net to find the sequence of equations
  //	(actually remainder pointers) which match the free symbol skeleton in the
  //	subject.
  //
  Index i;
  if (!(net.isNull()))  // at least one pattern has free symbols
    {
      DagNode** const topArgArray = static_cast<FreeDagNode*>(subject)->argArray();
      const Vector<TestNode>::const_iterator netBase = net.begin();
      const Vector<DagNode**>::iterator stackBase = stack.begin();
      Vector<TestNode>::const_iterator n = netBase;
      DagNode* d = topArgArray[n->argIndex];
      stack[0] = topArgArray;
      for (;;)
	{
	  const TestNode action = n[d->symbol()->getMatchIndex()];  // load both fields
	  const Index s = action.slotIndex;
	  if (s >= 0)
	    stackBase[s] = static_cast<FreeDagNode*>(d)->argArray();
	  i = action.nodeIndex;
	  if (i <= 0)
	    {
	      if (i == 0)
		return false;
	      break;
	    }
	  n = netBase + i;
	  d = stackBase[n->slotIndex][n->argIndex];
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
      if (r->fastMatchReplace(subject, context, stack))
	return true;
    }
  while ((r = *(++p)) != 0);
  return false;
}
