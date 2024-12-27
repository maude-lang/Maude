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
//	Code for execution of free theory discrimination net from an explicit stack machine.
//
//	Since we don't make recursive calls, it's OK to have to inline this code and have big
//	C++ stack frames.
//

#ifndef _freeNetExec_hh_
#define _freeNetExec_hh_

inline long
FreeNet::findRemainderListIndex(DagNode** argumentList)
{
  Index i;
  stack[0] = argumentList;
  if (!(net.isNull()))  // at least one pattern has free symbols
    {
      const Vector<TestNode>::const_iterator netBase = net.begin();
      const Vector<DagNode**>::iterator stackBase = stack.begin();
      Vector<TestNode>::const_iterator n = netBase;
      DagNode* d = argumentList[n->argIndex];
      for (;;)
	{
	  const TestNode& action = n[1 + d->symbol()->getMatchIndex()];
	  const Index s = action.slotIndex;
	  if (s >= 0)
	    stackBase[s] = static_cast<FreeDagNode*>(d)->argArray();
	  i = action.nodeIndex;
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
	  d = stackBase[n->position][n->argIndex];
	}
      return ~i;
    }
  return 0;  // single list of remainders
}

#endif
