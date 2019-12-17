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
#ifndef _stackMachine_hh_
#define _stackMachine_hh_
#include "simpleRootContainer.hh"
#include "frame.hh"

class StackMachine : private SimpleRootContainer
{
  NO_COPYING(StackMachine);

public:
  typedef size_t FrameLift;

  StackMachine();
  ~StackMachine();

  Frame* getTopFrame() const;
  static FrameLift makeFrameLift(int nrSlotsToPreserve);
  Frame* pushFrame(FrameLift frameLift);
  void popDeadFrames();
  void popFrame(Frame* frame);
  void setTopFrame(Frame* frame);
  //
  //	To quit the top frame when it is not yet complete.
  //
  void popToFrame(Frame* frame);

  void execute();
  DagNode* execute(Instruction* instructionSequence);
  void incrementEqCount(Int64 amount = 1);
  Int64 getEqCount();

  static Frame* fastPushFrame(Frame* oldFrame, FrameLift frameLift);
  //
  //	Scratchpad manipulation.
  //
  DagNode** getProtectedScratchpad() const;
  //DagNode*** getRawScratchpad() const;

private:
  enum Private
    {
      STACK_SIZE = 1000000000
    };

  typedef DagNode** DAGP_POINTER;
  typedef DagNode* DAG_POINTER;

  void markReachableNodes();

  Frame* topFrame;
  char* memoryBase;

  Int64 rewriteCount;

  DagNode* realResult;  // so we can protect it from GC
  //
  //	Scratchpads.
  //
  DAG_POINTER* protectedScratchpad;
  //DAGP_POINTER* rawScratchpad;
  //
  //	stack statistics
  //
  //Int64 nrFramesMade;
  //size_t maxFrameUse;
};

inline Frame*
StackMachine::getTopFrame() const
{
  return topFrame;
}

inline StackMachine::FrameLift
StackMachine::makeFrameLift(int nrSlotsToPreserve)
{
  return sizeof(Frame) + sizeof(DagNode*) * nrSlotsToPreserve;
}

inline Frame*
StackMachine::pushFrame(FrameLift frameLift)
{
  topFrame = reinterpret_cast<Frame*>(reinterpret_cast<char*>(topFrame) + frameLift);
#if 0
  for (int i = 0; i < 64; ++i)
	 (reinterpret_cast<char*>(topFrame))[i] = 0x33;  // fill new frame with garbage
#endif
  return topFrame;
}

inline Frame*
StackMachine::fastPushFrame(Frame* oldFrame, FrameLift frameLift)
{
  //
  //	Static function that creates a new frame from the old frame without updating underlying machine.
  //
  return reinterpret_cast<Frame*>(reinterpret_cast<char*>(oldFrame) + frameLift);
}

inline void
StackMachine::popDeadFrames()
{
  topFrame = topFrame->getAncestorWithValidNextInstruction();
}

inline void
StackMachine::popFrame(Frame* frame)
{
  //
  //	This function exists simply to avoid reloading topFrame, when the caller already has
  //	it in a register.
  //
  Assert(frame == topFrame, "frame clash");
  topFrame = frame->getAncestorWithValidNextInstruction();
}

inline void
StackMachine::popToFrame(Frame* frame)
{
  topFrame = frame;
}

inline void
StackMachine::setTopFrame(Frame* frame)
{
  topFrame = frame;
}

inline void
StackMachine::incrementEqCount(Int64 amount)
{
  rewriteCount += amount;
}

inline Int64
StackMachine::getEqCount()
{
  return rewriteCount;
}

inline DagNode**
StackMachine::getProtectedScratchpad() const
{
  return protectedScratchpad;
}

/*
inline DagNode***
StackMachine::getRawScratchpad() const
{
  return rawScratchpad;
}
*/

#endif
