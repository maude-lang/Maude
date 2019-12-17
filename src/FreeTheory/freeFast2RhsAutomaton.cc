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
#include "freeFast2RhsAutomaton.hh"

local_inline void
FreeFast2RhsAutomaton::fillOutArgs(const FastInstruction& instr,
				  Substitution& matcher,
				  FreeDagNode* d)
{
  DagNode* d0 = matcher.value(instr.sources[0]);
  DagNode* d1 = matcher.value(instr.sources[1]);
  d->internal[0] = d0;
  d->internal[1] = d1;
}

DagNode*
FreeFast2RhsAutomaton::construct(Substitution& matcher)
{
  FreeDagNode* d;
  Vector<FastInstruction>::const_iterator i = fastInstructions.begin();
  for (Vector<FastInstruction>::size_type instructionCount = nrInstructions;; ++i)
    {
      d = new FREE_DAG_NODE_LOOP(i->symbol);
      fillOutArgs(*i, matcher, d);
      matcher.bind(i->destination, d);
      if (--instructionCount == 0)
	break;
    }
  return d;
}

void
FreeFast2RhsAutomaton::replace(DagNode* old, Substitution& matcher)
{
  Vector<FastInstruction>::const_iterator i = fastInstructions.begin();
  for (Vector<FastInstruction>::size_type instructionCount = nrInstructions; --instructionCount != 0; ++i)
    {
      FreeDagNode* d = new FREE_DAG_NODE_LOOP(i->symbol);
      fillOutArgs(*i, matcher, d);
      matcher.bind(i->destination, d);
    }
  FreeDagNode* d = new(old) FREE_DAG_NODE(i->symbol);
  fillOutArgs(*i, matcher, d);
}

void
FreeFast2RhsAutomaton::remapIndices(VariableInfo& variableInfo)
{
  //
  //	Standard processing.
  //
  FreeRhsAutomaton::remapIndices(variableInfo);
  //
  //	Make fast copy.
  //
  nrInstructions = instructions.size();
  fastInstructions.resize(nrInstructions);
  for (Vector<FastInstruction>::size_type i = 0; i < nrInstructions; i++)
    {
      FastInstruction& f = fastInstructions[i];
      const Instruction& instr = instructions[i];
      f.symbol = instr.symbol;
      f.sources[0] = f.sources[1] = 0;
      f.destination = instr.destination;
      int nrArgs = instr.sources.length();
      for (int j = 0; j < nrArgs; ++j)
	f.sources[j] = instr.sources[j];
    }
}
