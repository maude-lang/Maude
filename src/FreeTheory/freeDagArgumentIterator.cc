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
//      Implementation for class FreeDagArgumentIterator
//

#include "macros.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"

//	interface class definitions
#include "rawDagArgumentIterator.hh"

//	free theory class definitions
#include "freeDagArgumentIterator.hh"

bool
FreeDagArgumentIterator::valid() const
{
  return nrRemaining > 0;
}

DagNode*
FreeDagArgumentIterator::argument() const
{
  Assert(nrRemaining > 0, "no args left");
  return *position;
}

void
FreeDagArgumentIterator::next()
{
  Assert(nrRemaining > 0, "no args left");
  ++position;
  --nrRemaining;
}
