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
//	Implementation for class ReturnInstruction.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "symbol.hh"

//      core class definitions
#include "frame.hh"
#include "stackMachine.hh"
#include "returnInstruction.hh"

ReturnInstruction::ReturnInstruction(int returnSlot)
  : returnSlot(returnSlot)
{
  setOpCode(RETURN);
}

void
ReturnInstruction::execute(StackMachine* machine) const
{
  //
  //    Get the stack frame containing the pointer to us.
  //
  Frame* frame = machine->getTopFrame();
  //
  //    Get value to return.
  //
  DagNode* v = frame->getSlot(returnSlot);
  //
  //	Finish up using parent class code.
  //
  returnResultAndContinue(machine, frame, v);
}

#ifdef DUMP

void
ReturnInstruction::dump(ostream& s, int indentLevel)
{
  s << Indent(indentLevel) << "return " << returnSlot << " (ReturnInstruction)" << endl;
}

#endif
