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
//      Stack machine instruction for a free symbols that have 0 to 3 arguments and only fast equations (if any).
//	This might be combined with FreeInstruction at some point.
//
#ifndef _freeFastInstruction_hh_
#define _freeFastInstruction_hh_
#include "freeInstruction.hh"

class FreeFastInstruction : public FreeInstruction
{
  NO_COPYING(FreeFastInstruction);

public:
  FreeFastInstruction(FreeSymbol* symbol, int destIndex, Instruction* nextInstruction);
  FreeFastInstruction(FreeSymbol* symbol, int argIndex0, int destIndex, Instruction* nextInstruction);
  FreeFastInstruction(FreeSymbol* symbol, int argIndex0, int argIndex1, int destIndex, Instruction* nextInstruction);
  FreeFastInstruction(FreeSymbol* symbol, int argIndex0, int argIndex1, int argIndex2, int destIndex, Instruction* nextInstruction);

#ifdef DUMP
  //void dump(ostream& s, int indentLevel);
#endif

private:
  const SlotIndex argIndex0;
  const SlotIndex argIndex1;
  const SlotIndex argIndex2;

  friend class FreeInstruction;
};

#endif
