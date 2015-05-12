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
//      Abstract base class for stack machine instructions.
//
#ifndef _instruction_hh_
#define _instruction_hh_
#include "natSet.hh"
#include "dagNode.hh"

class Instruction
{
  NO_COPYING(Instruction);

public:
  enum OpCodes
    {
      FREE_THEORY_BASE = 0,

      RETURN = 32,
      //
      //	Default for any theory that isn't going to check opCode.
      //
      OTHER = 127
    };

  Instruction();
  virtual ~Instruction(){}
  //
  //	Slot index type is performance critical; make soft choice.
  //
  typedef size_t SlotIndex;
  typedef size_t OpCode;

  void setOpCode(OpCode code);
  OpCode getOpCode() const;
  //
  //	We will specialize this class as much as possible so as much unpredicable branching as possible
  //	can be in this single unpredicable virtual function call.
  //
  virtual void execute(StackMachine* machine) const = 0;

  virtual void setActiveSlots(const NatSet& slots) = 0;
  virtual void markActiveSlots(const Frame* frame) const = 0;

#ifdef DUMP
  virtual void dump(ostream& s, int indentLevel = 0) {}
  virtual void dumpActiveSlots(ostream& s, const Frame* frame) const {}
#endif

private:
  OpCode opCode;
};

inline
Instruction::Instruction()
{
  opCode = OTHER;
}

inline void
Instruction::setOpCode(OpCode code)
{
  opCode = code;
}

inline Instruction::OpCode
Instruction::getOpCode() const
{
  return opCode;
}

#endif
