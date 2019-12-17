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
#include "freeBinaryRhsAutomaton.hh"

local_inline void
FreeBinaryRhsAutomaton::fillOutArgs(Substitution& matcher,
				    FreeDagNode* d)
{
  DagNode* d0 = matcher.value(sources[0]);
  DagNode* d1 = matcher.value(sources[1]);
  d->internal[0] = d0;
  d->internal[1] = d1;
}

DagNode*
FreeBinaryRhsAutomaton::construct(Substitution& matcher)
{
  FreeDagNode* d = new FREE_DAG_NODE(symbol);
  fillOutArgs(matcher, d);
  matcher.bind(destination, d);
  return d;
}

void
FreeBinaryRhsAutomaton::replace(DagNode* old, Substitution& matcher)
{
  FreeDagNode* d = new(old) FREE_DAG_NODE(symbol);
  fillOutArgs(matcher, d);
}

void
FreeBinaryRhsAutomaton::remapIndices(VariableInfo& variableInfo)
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
  sources[0] = instr.sources[0];
  sources[1] = instr.sources[1];
  destination = instr.destination;
}
