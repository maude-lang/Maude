/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2017 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class FreeFastRhsAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	core class definitions
#include "substitution.hh"

//	free theory class definitions
#include "freeDagNode.hh"
#include "freeUnaryRhsAutomaton.hh"

DagNode*
FreeUnaryRhsAutomaton::construct(Substitution& matcher)
{
  FreeDagNode* d = new FREE_DAG_NODE(symbol);
  d->internal[0] = matcher.value(source);
  matcher.bind(destination, d);
  return d;
}

void
FreeUnaryRhsAutomaton::replace(DagNode* old, Substitution& matcher)
{
  FreeDagNode* d = new(old) FREE_DAG_NODE(symbol);
  d->internal[0] = matcher.value(source);
}

void
FreeUnaryRhsAutomaton::remapIndices(VariableInfo& variableInfo)
{
  //
  //	Standard processing.
  //
  FreeRhsAutomaton::remapIndices(variableInfo);
  //
  //	Make fast copy.
  //
  const Instruction& instr = instructions[0];
  symbol = instr.symbol;
  source = instr.sources[0];
  destination = instr.destination;
}
