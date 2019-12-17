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
//      Stack machine final instruction free symbols having > 3 arguments.
//
#ifndef _freeGeneralCtorFinal_hh_
#define _freeGeneralCtorFinal_hh_
#include "finalInstruction.hh"

class FreeGeneralCtorFinal : public FinalInstruction
{
  NO_COPYING(FreeGeneralCtorFinal);

public:
  FreeGeneralCtorFinal(FreeSymbol* symbol, const Vector<int>& argumentSlots);

  void execute(StackMachine* machine) const;

#ifdef DUMP
  //void dump(ostream& s, int indentLevel);
#endif

private:
  FreeSymbol* const symbol;

  Vector<SlotIndex> args;
};

#endif
