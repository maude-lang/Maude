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
//      Implementation for class ACU_DagArgumentIterator.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "ACU_Persistent.hh"
#include "ACU_Theory.hh"

//	ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_DagArgumentIterator.hh"

bool
ACU_DagArgumentIterator::valid() const
{
  return multiplicityRemaining > 0;
}
 
DagNode*
ACU_DagArgumentIterator::argument() const
{
  Assert(multiplicityRemaining > 0, "no args left");
  return (*argArray)[position].dagNode;
}

void
ACU_DagArgumentIterator::next()
{
  Assert(multiplicityRemaining > 0, "no args left");
  --multiplicityRemaining;
  if (multiplicityRemaining == 0)
    {
      ++position;
      if (position < argArray->length())
        multiplicityRemaining = (*argArray)[position].multiplicity;
    }
}
