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
//      Stack machine instruction for a free symbol with only "fast" equations.
//	It can be final or non-final, extor or ctor but we inherit from NonFinalExtor to get
//	the extra non-final and extor stuff in case we need it.
//
#ifndef _freeInstruction_hh_
#define _freeInstruction_hh_
#include "nonFinalExtor.hh"

class FreeInstruction : public NonFinalExtor
{
  NO_COPYING(FreeInstruction);

public:
  enum FreeOpCodes
    {
      FREE_NULLARY_CTOR,
      FREE_UNARY_CTOR,
      FREE_BINARY_CTOR,
      FREE_TERNARY_CTOR,

      FREE_NULLARY_EXTOR,
      FREE_UNARY_EXTOR,
      FREE_BINARY_EXTOR,
      FREE_TERNARY_EXTOR,

      FREE_NULLARY_CTOR_FINAL,
      FREE_UNARY_CTOR_FINAL,
      FREE_BINARY_CTOR_FINAL,
      FREE_TERNARY_CTOR_FINAL,

      FREE_NULLARY_EXTOR_FINAL,
      FREE_UNARY_EXTOR_FINAL,
      FREE_BINARY_EXTOR_FINAL,
      FREE_TERNARY_EXTOR_FINAL,
    };

  FreeInstruction(FreeSymbol* symbol, int destIndex = 0, Instruction* nextInstruction = 0);

  void execute(StackMachine* machine) const;

#ifdef DUMP
  void dump(ostream& s, int indentLevel);
#endif

private:
  FreeSymbol* const symbol;  // our symbol
};

#endif
