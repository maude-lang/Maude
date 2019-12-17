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
//      Implementation for class AU_Deque.
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

//	AU stack class definitions
#include "AU_Deque.hh"

int
AU_Deque::computeBaseSort(Symbol* symbol) const
{
#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

  //cerr << "AU_Deque::computeBaseSort()\n";
  //cerr << "left = " << left << " right = " << right << endl;
  if (left != 0)
    {
      
      int index = left->getSortIndex();
      //cerr << "left index = " << index << endl;
      if (index == Sort::SORT_UNKNOWN)
	index = left->fwdComputeBaseSort(symbol);
      //cerr << "left index now = " << index << endl;

#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

      if (right == 0)
	return index;
      int index2 = right->getSortIndex();
      //cerr << "right index = " << index2 << endl;

      if (index2 == Sort::SORT_UNKNOWN)
	index2 = right->revComputeBaseSort(symbol);
      //cerr << "right index now = " << index2 << endl;

#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

      return symbol->traverse(symbol->traverse(0, index), index2);
    }
  Assert(right != 0, "no args");
  int index = right->getSortIndex();
  //cerr << "Right index = " << index << endl;
  if (index == Sort::SORT_UNKNOWN)
    index = right->revComputeBaseSort(symbol);
  //cerr << "Right index now = " << index << endl;

#ifdef CHECK_DEQUE
  checkIntegrity();
#endif

  return index;
}

#ifdef CHECK_DEQUE

void
AU_Deque::checkIntegrity() const 
{
  int l = AU_StackNode::checkIntegrity(left);
  int r = AU_StackNode::checkIntegrity(right);
  Assert(nrElements == l + r,
	 "bad deque size: " << nrElements << " != " << l << " + " << r);
}

#endif
