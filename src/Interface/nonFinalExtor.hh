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
//      Abstract base class for stack machine extors that are the not last instruction of their sequence.
//
#ifndef _nonFinalExtor_hh_
#define _nonFinalExtor_hh_
#include "nonFinalInstruction.hh"
#include "stackMachine.hh"

class NonFinalExtor : public NonFinalInstruction
{
  NO_COPYING(NonFinalExtor);

public:
  //
  //	We add capability the capability to push new stack frames,
  //    by knowing how much of the current stack frame needs to be preserved.
  //
  NonFinalExtor(int destinationIndex, Instruction* nextInstruction);

  Frame* pushFrame(StackMachine* machine) const;
  Frame* fastPushFrame(Frame* frame) const;

  void setActiveSlots(const NatSet& slots);

private:
  StackMachine::FrameLift frameLift;
};

inline
NonFinalExtor::NonFinalExtor(int destinationIndex, Instruction* nextInstruction)
  : NonFinalInstruction(destinationIndex, nextInstruction)
{
}

inline Frame*
NonFinalExtor::pushFrame(StackMachine* machine) const
{
  return machine->pushFrame(frameLift);
}

inline Frame*
NonFinalExtor::fastPushFrame(Frame* frame) const
{
  return StackMachine::fastPushFrame(frame, frameLift);
}

#endif
