/*

    This file is part of the Maude 3 interpreter.

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
//      Abstract base class for stack machine instructions that are the last instruction of their sequence.
//
#ifndef _finalInstruction_hh_
#define _finalInstruction_hh_
#include "regularInstruction.hh"
#include "stackMachine.hh"

class FinalInstruction : public RegularInstruction
{
  NO_COPYING(FinalInstruction);

public:
  FinalInstruction() {}

protected:
  //
  //	This shared code for finishing up the execution of a final ctor is the sole
  //	reason for this class to exist. It is not placed in a class FinalCtor, since
  //	it is also usable by return instructions.
  //
  //	It's not safe to call from any function that leak pointers to their stack frame
  //	since the tail call won't optimize correctly.
  //
  void returnResultAndContinue(StackMachine* machine, Frame* frame, DagNode* result) const;
};

inline void
FinalInstruction::returnResultAndContinue(StackMachine* machine, Frame* frame, DagNode* result) const
{
  frame->returnValue(result);
  machine->popFrame(frame);
  Frame* liveFrame = machine->getTopFrame();
  const Instruction* ni = liveFrame->getNextInstruction();
  MemoryCell::okToCollectGarbage();
  ni->execute(machine);
}

#endif
