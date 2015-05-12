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
//	Implementation for class NullInstruction.
//

//	utility stuff
#include "macros.hh"
//#include "vector.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "symbol.hh"

//      core class definitions
//#include "frame.hh"
//#include "stackMachine.hh"
#include "nullInstruction.hh"

NullInstruction::NullInstruction()
{
}

const Instruction*
NullInstruction::getNullInstruction()
{
  static const NullInstruction nullInstruction;

  return &nullInstruction;
}

void 
NullInstruction::execute(StackMachine* /* machine */) const
{
}

void
NullInstruction::setActiveSlots(const NatSet& /* slots */)
{
  CantHappen("shouldn't be called");
}

void
NullInstruction::markActiveSlots(const Frame* /* frame*/) const
{
}

#ifdef DUMP

void 
NullInstruction::dump(ostream& s, int indentLevel)
{
  s << Indent(indentLevel) << "null instruction" << endl;
}

#endif
