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
//      Implementation for class S_DagArgumentIterator.
//
 
//	utility stuff
#include "macros.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "S_Theory.hh"

//	S theory class definitions
#include "S_DagArgumentIterator.hh"

bool
S_DagArgumentIterator::valid() const
{
  return arg != 0;
}
 
DagNode*
S_DagArgumentIterator::argument() const
{
  Assert(valid(), "no args left");
  return arg;
}

void
S_DagArgumentIterator::next()
{
  Assert(valid(), "no args left");
  arg = 0;
}
