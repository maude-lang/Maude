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
//      Implementation for class StackMachine.
//

//	utility stuff
#include "macros.hh"
//#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	core declarations
#include "stackMachine.hh"
#include "instruction.hh"
#include "nullInstruction.hh"

StackMachine::StackMachine()
{
  memoryBase = static_cast<char*>(operator new[](STACK_SIZE));
  topFrame = reinterpret_cast<Frame*>(memoryBase);

  protectedScratchpad = new DAG_POINTER[10000];  // HACK
  //rawScratchpad = new DAGP_POINTER[1000]; // HACK

  rewriteCount = 0;

  //nrFramesMade = 0;
  //maxFrameUse = 0;
}

StackMachine::~StackMachine()
{
  delete[] memoryBase;
  delete[] protectedScratchpad;
}

/*
DagNode*
StackMachine::execute(Instruction* instructionSequence)
{
  //
  //	Create initial frame to kick up computation.
  //
  Frame* initFrame = topFrame;
  initFrame->setNextInstruction(instructionSequence);
  initFrame->setReturnAddress(&realResult);
  initFrame->setAncestorWithValidNextInstruction(0);

  while (topFrame != 0)
    {
      topFrame->getNextInstruction()->execute(this);
      //MemoryCell::okToCollectGarbage();
    }

  return realResult;
}
*/

DagNode*
StackMachine::execute(Instruction* instructionSequence)
{
  //
  //	Create a dummy frame to soak up extra execute() calls.
  //
  DagNode* dummyResult;
  Frame* dummyFrame = topFrame;
  dummyFrame->setNextInstruction(NullInstruction::getNullInstruction());
  dummyFrame->setReturnAddress(&dummyResult);  // needs to be a valid address for GC
  dummyFrame->setAncestorWithValidNextInstruction(0);
  //
  //	Create initial frame to kick off computation.
  //
  Frame* initFrame = pushFrame(makeFrameLift(0));
  initFrame->setNextInstruction(instructionSequence);
  initFrame->setReturnAddress(&realResult);
  initFrame->setAncestorWithValidNextInstruction(dummyFrame);
  //
  //	Make safe for GC.
  //
  realResult = 0;
  //
  //	Loop until we get back to dummy frame.
  //
  do
    {
      //
      //	Pick 12 as the lcm(2, 3, 4, 6)
      //
      topFrame->getNextInstruction()->execute(this);
      
      topFrame->getNextInstruction()->execute(this);
      /*
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
 
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
 
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);

      //
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
 
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
 
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);

      //
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
 
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
 
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);

      //
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
 
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
 
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);

      //
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
 
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
 
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);
      topFrame->getNextInstruction()->execute(this);

      //cout << (reinterpret_cast<char*>(topFrame) - memoryBase) << endl;
      */
      MemoryCell::okToCollectGarbage();
    }
  while (topFrame != dummyFrame);

  return realResult;
}

void
StackMachine::execute()
{
  //
  //	Need to figure out termination condition for when last frame is pop'd
  //
  //  for(;;)
  while (topFrame != 0)
    {
      topFrame->getNextInstruction()->execute(this);
      MemoryCell::okToCollectGarbage();
    }
}

void
StackMachine::markReachableNodes()
{
  for (Frame* i = topFrame; i != 0; i = i->getAncestorWithValidNextInstruction())
    {
      i->returnValue(0);  // need to clear garbage pointer from slot
      i->markActiveSlots();
    }
  if (realResult != 0)
    realResult->mark();
}
