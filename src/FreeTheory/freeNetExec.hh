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
//	Code for execution of free theory discrimination net from an explicit stack machine.
//
//	Since we don't make recursive calls, it's OK to have to inline this code and have big
//	C++ stack frames.
//

#ifndef _freeNetExec_hh_
#define _freeNetExec_hh_

#if 1

inline long
FreeNet::findRemainderListIndex(DagNode** argumentList)
{
  long i;
  stack[0] = argumentList;
  if (!(net.isNull()))  // at least one pattern has free symbols
    {
      Vector<TestNode>::const_iterator netBase = net.begin();
      Vector<TestNode>::const_iterator n = netBase;
      Vector<DagNode**>::iterator stackBase = stack.begin();
      DagNode* d = argumentList[n->argIndex];
      int symbolIndex = d->symbol()->getIndexWithinModule();
      for (;;)
	{
	  //
	  //	The sole reason for unrolling this loop is to generate copies of branch instructions
	  //	in the hope of better branch prediction.
	  //
	one:
	  {
	    long p;
	    long diff = symbolIndex - n->symbolIndex;
	    if (diff != 0)
	      {
		i = n->notEqual[diff < 0];
		if (i <= 0)
		  {
		    //
		    //	If i == 0 we want to return NONE. But since NONE = -1 = ~i we
		    //	cheat, by skipping the test and letting the main return statement
		    //	negate 0 into -1.
		    //
		    break;
		  }
		n = netBase + i;
		p = n->position;
		if (p < 0)
		  goto two;
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
	two:
	  {
	    long p;
	    long diff = symbolIndex - n->symbolIndex;
	    if (diff != 0)
	      {
		i = n->notEqual[diff < 0];
		if (i <= 0)
		  {
		    //
		    //	If i == 0 we want to return NONE. But since NONE = -1 = ~i we
		    //	cheat, by skipping the test and letting the main return statement
		    //	negate 0 into -1.
		    //
		    break;
		  }
		n = netBase + i;
		p = n->position;
		if (p < 0)
		  goto one;
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
	}
      return ~i;
    }
  return 0;  // single list of remainders
}

#else

inline long
FreeNet::findRemainderListIndex(DagNode** argumentList)
{
  long i;
  stack[0] = argumentList;
  if (!(net.isNull()))  // at least one pattern has free symbols
    {
      Vector<TestNode>::const_iterator netBase = net.begin();
      Vector<TestNode>::const_iterator n = netBase;
      Vector<DagNode**>::iterator stackBase = stack.begin();
      DagNode* d = argumentList[n->argIndex];
      int symbolIndex = d->symbol()->getIndexWithinModule();
      for (;;)
	{
	  long p;
	  int diff = symbolIndex - n->symbolIndex;
	  if (diff != 0)
	    {
	      i = n->notEqual[diff < 0];
	      if (i <= 0)
		{
		  //
		  //	If i == 0 we want to return NONE. But since NONE = -1 = ~i we
		  //	cheat, by skipping the test and letting the main return statement
		  //	negate 0 into -1.
		  //
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
      return ~i;
    }
  return 0;  // single list of remainders
}

#endif

#endif
