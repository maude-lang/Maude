/*

    This file is part of the Maude 3 interpreter.

    Copyright 2013-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class RegularInstruction.
//

//	utility stuff
#include "macros.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "dagNode.hh"
#include "regularInstruction.hh"

//      core class definitions
#include "frame.hh"

void
RegularInstruction::setActiveSlots(const NatSet& slots)
{
  activeSlots = 0;
  for (int index : slots)
    activeSlots |= (1 << index);
}

void
RegularInstruction::markActiveSlots(const Frame* frame) const
{
  BitVec b = activeSlots;
  DagNode* const* p = frame->getArgumentListPtr();
  do
    {
      if ((b & 1) != 0)
	{
	  DagNode* d = *p;
	  if (d != 0)
	    d->mark();
	}
      ++p;
    }
  while ((b >>= 1) != 0);
}

#ifdef DUMP

void
RegularInstruction::dumpActiveSlots(ostream& s, const Frame* frame) const
{
  s << "frame ptr = " << static_cast<const void*>(frame) <<
    "  ancestor = " << static_cast<void*>(frame->getAncestorWithValidNextInstruction()) << endl;
  BitVec b = activeSlots;
  DagNode* const* p = frame->getArgumentListPtr();
  do
    {
      if ((b & 1) != 0)
	{
	  DagNode* d = *p;
	  s << static_cast<void*>(d) << " : ";
	  if (d != 0)
	    s << '(' << d << ')';
	  else
	    s << "(null)";
	}
      else
	s << "???";
      s << '\t';
      ++p;
    }
  while ((b >>= 1) != 0);
  s << endl;
}

#endif
