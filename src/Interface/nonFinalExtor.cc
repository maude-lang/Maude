/*

    This file is part of the Maude 2 interpreter.

    Copyright 2013 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class NonFinalExtor.
//

//	utility stuff
#include "macros.hh"
#include "natSet.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "nonFinalExtor.hh"

void
NonFinalExtor::setActiveSlots(const NatSet& slots)
{
  //
  //	We need to calculate our frameLift to preserve all the slots we protect from GC
  //	together with our destinationIndex, since the latter will be zeroed out by the
  //	mark phase, destroying any overlapped data.
  //
  int maxSlotToPreserve = slots.max();
  if (maxSlotToPreserve == NONE)
    maxSlotToPreserve = 0;
  SlotIndex destinationIndex = getDestinationIndex();
  DebugAdvisory("maxSlotToPreserve = " << maxSlotToPreserve << " destinationIndex " << destinationIndex);
  if (destinationIndex > static_cast<SlotIndex>(maxSlotToPreserve))
    maxSlotToPreserve = destinationIndex;
  frameLift = StackMachine::makeFrameLift(maxSlotToPreserve + 1);
  //
  //	Sill need standard active slot processing by ancestor class.
  //
  NonFinalInstruction::setActiveSlots(slots);
}
