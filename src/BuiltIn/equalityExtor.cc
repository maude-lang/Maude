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
//	Implementation for class EqualityExtor.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "freeTheory.hh"

//      core class definitions
#include "frame.hh"
#include "stackMachine.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//	free theory class definitions
#include "equalitySymbol.hh"
#include "equalityExtor.hh"

EqualityExtor::EqualityExtor(EqualitySymbol* symbol,  int argIndex0, int argIndex1, int destinationIndex, Instruction* nextInstruction)
  : NonFinalExtor(destinationIndex, nextInstruction),
    symbol(symbol),
    argIndex0(argIndex0),
    argIndex1(argIndex1)
{
  //
  //	Since EqualitySymbols are generated as needed and inserted as late symbols, we compile on demand.
  //	It is safe to call stackMachineCompile() multiple times on the same EqualitySymbol.
  //
  symbol->stackMachineCompile();
}

void
EqualityExtor::execute(StackMachine* machine) const
{
  //
  //    Get the stack frame containing the pointer to us.
  //
  Frame* frame = machine->getTopFrame();
  //
  //	Check equality of arguments.
  //
  bool equal = frame->getSlot(argIndex0)->equal(frame->getSlot(argIndex1));
  //
  //	Get approriate instruction sequence.
  //
  Instruction* ni = equal ? symbol->getEqualInstructionSequence() : symbol->getNotEqualInstructionSequence();
  //
  //	Make a new stack frame without reloading old frame or recording new frame in machine.
  //
  Frame* nextFrame = fastPushFrame(frame);
  //
  //	Move instruction pointer in old frame to next instruction.
  //
  frame->setNextInstruction(getNextInstruction());
  //
  //	Set return frame and addres for new frame.
  //
  nextFrame->setAncestorWithValidNextInstruction(frame);
  nextFrame->setReturnAddress(frame, getDestinationIndex());
  //
  //	Record new frame, and increment rewrite count.
  //
  machine->setTopFrame(nextFrame);
  machine->incrementEqCount();
  ni->execute(machine);  // tail call (should we worry about collecting garbage?)
}
