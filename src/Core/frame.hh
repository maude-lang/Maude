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
//      Class for excution frames on the stack machine.
//
#ifndef _frame_hh_
#define _frame_hh_
#include "instruction.hh"

class Frame
{
  //
  //	Frames can't have ctors, dtors or virtual functions because of the way we
  //	allocate and deallocate them.
  //
  NO_COPYING(Frame);

public:
  //
  //	Setting and getting slot values.
  //
  void setSlot(Instruction::SlotIndex index, DagNode* value);
  DagNode* getSlot(Instruction::SlotIndex index) const;
  DagNode** getArgumentListPtr();
  DagNode* const* getArgumentListPtr() const;

  //
  //	Setting a return address and returning values.
  //
  void setReturnAddress(Frame* ancestor, Instruction::SlotIndex index);
  void setReturnAddress(DagNode** absoluteAddress);
  void returnValue(DagNode* value) const;
  //
  //	Setting and getting the next instruction pointer.
  //
  void setNextInstruction(const Instruction* instruction);
  const Instruction* getNextInstruction() const;
  //
  //	Setting and getting the ancestor frame.
  //
  void setAncestorWithValidNextInstruction(Frame* ancestor);
  Frame* getAncestorWithValidNextInstruction() const;
  Frame* fastPop() const;
  //
  //	Copying return information from a deadFrame into a new frame.
  //
  void copyReturnInfo(const Frame* deadFrame);
  //
  //	Mark active slots, depending on our instruction.
  //
  void markActiveSlots() const;

private:
  //
  //	Each instruction is responsible for either disposing of the frame or updating the
  //	nextInstruction pointer.
  //
  const Instruction* nextInstruction;
  //
  //	In the case that the frame is to be disposed of, the instruction has to return a
  //	value by writting it to a slot located in some earlier frame.
  //
  DagNode** returnAddress;
  //
  //	When a frame is finished we need to go to the ancestor frame that still has
  //	a valid next instructuion.
  //
  Frame* ancestorWithValidNextInstruction;
  //
  //	The main contents of a frame is a block of slots for holding substitutions and
  //	partly built dags. The size of this block depends on the amount of space allocated
  //	for the frame.
  //
  DagNode* slots[0];
};

inline void
Frame::setSlot(Instruction::SlotIndex index, DagNode* value)
{
  //  Assert(((void*) value) > ((void*) this) + 10000 ||
  //	  ((void*) value) + 10000 < ((void*) this), "dag node point shouldn't be close to fame pointer");
  slots[index] = value;
}

inline DagNode*
Frame::getSlot(Instruction::SlotIndex index) const
{
  return slots[index];
}

inline DagNode**
Frame::getArgumentListPtr()
{
  return slots;
}

inline DagNode* const*
Frame::getArgumentListPtr() const
{
  return slots;
}

inline void
Frame::setReturnAddress(Frame* ancestor, Instruction::SlotIndex index)
{
  returnAddress = &(ancestor->slots[index]);
}

inline void
Frame::setReturnAddress(DagNode** absoluteAddress)
{
  returnAddress = absoluteAddress;
}

inline void
Frame::returnValue(DagNode* value) const
{
  //  Assert(((void*) value) > ((void*) this) + 10000 ||
  //	  ((void*) value) + 10000 < ((void*) this), "dag node point shouldn't be close to fame pointer");
  *returnAddress = value;
}

inline void
Frame::setNextInstruction(const Instruction* instruction)
{
  nextInstruction = instruction;
}

inline const Instruction*
Frame::getNextInstruction() const
{
  return nextInstruction;
}

inline void
Frame::setAncestorWithValidNextInstruction(Frame* ancestor)
{
  ancestorWithValidNextInstruction = ancestor;
}

inline Frame*
Frame::getAncestorWithValidNextInstruction() const
{
  return ancestorWithValidNextInstruction;
}

inline Frame*
Frame::fastPop() const
{
  return ancestorWithValidNextInstruction;
}

inline void
Frame::copyReturnInfo(const Frame* deadFrame)
{
  //
  //	We copy the return information from a dead frame into a new frame.
  //
  returnAddress = deadFrame->returnAddress;
  ancestorWithValidNextInstruction = deadFrame->ancestorWithValidNextInstruction;
}

inline void
Frame::markActiveSlots() const
{
  nextInstruction->markActiveSlots(this);
}

#define FRAME_SIZE_HACK 5

#endif
