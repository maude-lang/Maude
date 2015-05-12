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
//	Implementation for class FreeFastInstruction
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

//	free theory class definitions
#include "freeSymbol.hh"
#include "freeFastInstruction.hh"
	
FreeFastInstruction::FreeFastInstruction(FreeSymbol* symbol, int destinationIndex, Instruction* nextInstruction)
  : FreeInstruction(symbol, destinationIndex, nextInstruction),
    argIndex0(0),
    argIndex1(0),
    argIndex2(0)
{
  setOpCode(4 * (symbol->equationFree() == false) + 8 * (nextInstruction == 0));
}

FreeFastInstruction::FreeFastInstruction(FreeSymbol* symbol, int argIndex0, int destinationIndex, Instruction* nextInstruction)
  : FreeInstruction(symbol, destinationIndex, nextInstruction),
    argIndex0(argIndex0),
    argIndex1(0),
    argIndex2(0)
{
  int opCode = 1 + 4 * (symbol->equationFree() == false) + 8 * (nextInstruction == 0);
  //  if (symbol->equationFree() && symbol->getUniqueSortIndex() > 0)
  //    opCode += 16;
  setOpCode(opCode);
}

FreeFastInstruction::FreeFastInstruction(FreeSymbol* symbol, int argIndex0, int argIndex1, int destinationIndex, Instruction* nextInstruction)
  : FreeInstruction(symbol, destinationIndex, nextInstruction),
    argIndex0(argIndex0),
    argIndex1(argIndex1),
    argIndex2(0)
{
  int opCode = 2 + 4 * (symbol->equationFree() == false) + 8 * (nextInstruction == 0);
  //  if (symbol->equationFree() && symbol->getUniqueSortIndex() > 0)
  //    opCode += 16;
  setOpCode(opCode);
}

FreeFastInstruction::FreeFastInstruction(FreeSymbol* symbol, int argIndex0, int argIndex1, int argIndex2, int destinationIndex, Instruction* nextInstruction)
  : FreeInstruction(symbol, destinationIndex, nextInstruction),
    argIndex0(argIndex0),
    argIndex1(argIndex1),
    argIndex2(argIndex2)
{
  int opCode = 3 + 4 * (symbol->equationFree() == false) + 8 * (nextInstruction == 0);
  //  if (symbol->equationFree() && symbol->getUniqueSortIndex() > 0)
  //    opCode += 16;
  setOpCode(opCode);
}
