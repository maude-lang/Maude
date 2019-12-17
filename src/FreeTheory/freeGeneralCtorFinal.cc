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
//	Implementation for class FreeGeneralCtorFinal.
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
#include "freeGeneralCtorFinal.hh"
	
FreeGeneralCtorFinal::FreeGeneralCtorFinal(FreeSymbol* symbol, const Vector<int>& argumentSlots)
  : symbol(symbol),
    args(argumentSlots.size())
{
  int nrArgs = argumentSlots.size();
  for (int i = 0; i < nrArgs; ++i)
    args[i] = argumentSlots[i];
}

void
FreeGeneralCtorFinal::execute(StackMachine* machine) const
{
  //
  //    Get the stack frame containing the pointer to us.
  //
  Frame* frame = machine->getTopFrame();
  //
  //    Use the special new(NONE) allocator that sets the reduced flag at creation.
  //
  FreeDagNode* d = new(NONE) FreeDagNode(symbol);
  //
  //	Fill out arguments and sort.
  //
  int nrArgs = d->symbol()->arity();
  DagNode** argArray = d->argArray();
  
  int state = 0;
  for (int i = 0; i < nrArgs; ++i)
    {
      DagNode* a = frame->getSlot(args[i]);
      int t = a->getSortIndex();
      state = symbol->traverse(state, t);
      argArray[i] = a;
    }
  d->setSortIndex(state);
  //
  //    Finish up using parent class code.
  //
  returnResultAndContinue(machine, frame, d);
}
