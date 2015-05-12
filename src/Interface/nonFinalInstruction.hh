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
#ifndef _nonFinalInstruction_hh_
#define _nonFinalInstruction_hh_
#include "regularInstruction.hh"
//#include "stackMachine.hh"
//#include "frame.hh"

class NonFinalInstruction : public RegularInstruction
{
  NO_COPYING(NonFinalInstruction);

public:
  //
  //	Non-final instructions have a destination slot to deposit their result in and a
  //	pointer to the next instruction.
  //
  NonFinalInstruction(int destinationIndex, Instruction* nextInstruction);
  ~NonFinalInstruction();
   Instruction* getNextInstruction() const;
  SlotIndex getDestinationIndex() const;

private:
  const SlotIndex destinationIndex;
  Instruction* nextInstruction;
};

inline
NonFinalInstruction::NonFinalInstruction(int destinationIndex, Instruction* nextInstruction)
  : destinationIndex(destinationIndex),
    nextInstruction(nextInstruction)
{
}

inline Instruction*
NonFinalInstruction::getNextInstruction() const
{
  return nextInstruction;
}

inline Instruction::SlotIndex
NonFinalInstruction::getDestinationIndex() const
{
  return destinationIndex;
}

#endif
