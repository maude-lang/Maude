/*

    This file is part of the Maude 2 interpreter.

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
//      Implementation for class ACU_TreeDagArgumentIterator.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "ACU_Persistent.hh"

//	ACU theory class definitions
#include "ACU_TreeDagArgumentIterator.hh"

bool
ACU_TreeDagArgumentIterator::valid() const
{
  return multiplicityRemaining > 0;
}
 
DagNode*
ACU_TreeDagArgumentIterator::argument() const
{
  Assert(valid(), "no args left");
  return iter.getDagNode();
}

void
ACU_TreeDagArgumentIterator::next()
{
  Assert(valid(), "no args left");
  --multiplicityRemaining;
  if (multiplicityRemaining == 0)
    {
      iter.next();
      if (iter.valid())
        multiplicityRemaining = iter.getMultiplicity();
    }
}
