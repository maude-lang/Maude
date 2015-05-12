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
//	Implementation for class FreeGeneralExtor.
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

//	free theory class definitions
#include "freeSymbol.hh"
#include "freeDagNode.hh"
#include "freeNetExec.hh"
#include "freeRemainder.hh"
#include "freeGeneralExtor.hh"
	
FreeGeneralExtor::FreeGeneralExtor(FreeSymbol* symbol, const Vector<int>& argumentSlots, int destinationIndex, Instruction* nextInstruction)
  : NonFinalExtor(destinationIndex, nextInstruction),
    symbol(symbol),
    args(argumentSlots.size())
{
  int nrArgs = argumentSlots.size();
  for (int i = 0; i < nrArgs; ++i)
    args[i] = argumentSlots[i];
}

void
FreeGeneralExtor::execute(StackMachine* machine) const
{
  //
  //    Get the stack frame containing the pointer to us.
  //
  Frame* frame = machine->getTopFrame();
  //
  //	Assemble arguments from stack into temporary storage.
  //
  int nrArgs = args.size();
  DagNode** savedArgs = machine->getProtectedScratchpad();
  for (int i = 0; i < nrArgs; ++i)
    savedArgs[i] = frame->getSlot(args[i]);
  //
  //	Run the discrimination net.
  //
  FREE_NET& net = symbol->GET_NET();
  long index = net.findRemainderListIndex(savedArgs);
  //
  //	If we found a list of applicable equations, try to complete each one until we succeed.
  //
  if (index >= 0)
    {
      Frame* nextFrame = fastPushFrame(frame);
      DagNode** binding = nextFrame->getArgumentListPtr();
      Vector<FreeRemainder*>::const_iterator p = net.fastApplicable[index].begin();
      const FreeRemainder* r = *p;
      do
	{
	  if (r->generalCheckAndBind(binding, net.stack))
	    {
	      frame->setNextInstruction(getNextInstruction());
	      nextFrame->setAncestorWithValidNextInstruction(frame);
	      nextFrame->setReturnAddress(frame, getDestinationIndex());
	      nextFrame->setNextInstruction(r->getFirstInstruction());
	      machine->setTopFrame(nextFrame);
	      machine->incrementEqCount();
	      return;  // more agressive thing would be to attempt a tail call and not store the instruction
	    }
	}
      while ((r = *(++p)) != 0);
    }
  //
  //    Use the special new(NONE) allocator that sets the reduced flag at creation.
  //
  FreeDagNode* d = new(NONE) FreeDagNode(symbol);
  //
  //	Fill out arguments and sort.
  //
  DagNode** argArray = d->argArray();
  int state = 0;
  for (int i = 0; i < nrArgs; ++i)
    {
      DagNode* a = savedArgs[i];
      int t = a->getSortIndex();
      state = symbol->traverse(state, t);
      argArray[i] = a;
    }
  d->setSortIndex(state);
  //
  //	Save new result in frame.
  //
  frame->setSlot(getDestinationIndex(), d);
  //
  //	Execute next instruction.
  //	This should be subject to tail call optimization in the calling execute() function, after we
  //	are inlined, so the callers stack frame will disappear, avoiding accumulation of stack frames.
  //	We don't bother storing a pointer to the next instruction in the our frame, relying on the
  //	instruction we execute to do this (or pass on the obligation).
  //
  getNextInstruction()->execute(machine);
}
