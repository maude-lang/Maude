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
//      Implementation for class AU_DequeIter.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "AU_Persistent.hh"

//	AU persistent class definitions
#include "AU_DequeIter.hh"

void
AU_DequeIter::reverseRight()
{
  revIndex = (nrElementsRemaining - 1) / AU_StackNode::ELEMENTS_PER_NODE;
  reversed.resize(revIndex);
  AU_StackNode* p = right;
  for (Vector<AU_StackNode*>::iterator b = reversed.begin();; ++b)
    {
      AU_StackNode* n = p->next;
      if (n == 0)
	break;
      *b = p;
      p = n;
    }
  current = p;
  index = AU_StackNode::ELEMENTS_PER_NODE - 1;
}
