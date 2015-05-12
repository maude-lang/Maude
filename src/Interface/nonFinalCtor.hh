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
//      Abstract base class for stack machine instructions that are the not last instruction of their sequence.
//
#ifndef _nonFinalCtor_hh_
#define _nonFinalCtor_hh_
#include "nonFinalInstruction.hh"
#include "frame.hh"

class NonFinalCtor : public NonFinalInstruction
{
  NO_COPYING(NonFinalCtor);

public:
  NonFinalCtor(int destinationIndex, Instruction* nextInstruction);

protected:
  //
  //	This shared code for finishing up the execution of a non-final ctor is the sole
  //	reason for this class to exist.
  //
  //	It's not safe to call this from functions that leak pointers to their stack
  //	frame since the compiler will not be able to perform tail recursion properly
  //	and an accumulation of garbage stack frame will occur. This is why this
  //	function lives here, as opposed to in the parent class.
  //
  void saveResultAndContinue(StackMachine* machine, Frame* frame, DagNode* result) const;
};

inline
NonFinalCtor::NonFinalCtor(int destinationIndex, Instruction* nextInstruction)
  : NonFinalInstruction(destinationIndex, nextInstruction)
{
}

inline void
NonFinalCtor::saveResultAndContinue(StackMachine* machine, Frame* frame, DagNode* result) const
{
  //
  //	Save new result in frame.
  //
  frame->setSlot(getDestinationIndex(), result);
  //
  //	Execute next instruction.
  //	This should be subject to tail call optimization in the calling execute() function, after we
  //	are inlined, so the callers stack frame will disappear, avoiding accumulation of stack frames.
  //	We don't bother storing a pointer to the next instruction in the our frame, relying on the
  //	instruction we execute to do this (or pass on the obligation).
  //
  getNextInstruction()->execute(machine);
}

#endif
