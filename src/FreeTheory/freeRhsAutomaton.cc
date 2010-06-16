/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class FreeRhsAutomaton.
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
#include "term.hh"

//	core class definitions
#include "substitution.hh"
#include "variableInfo.hh"

//	free theory class definitions
#include "freeDagNode.hh"
#include "freeRhsAutomaton.hh"

void
FreeRhsAutomaton::addFree(Symbol* symbol,
			  int destination,
			  const Vector<int>& sources)
{
  int i = instructions.length();
  instructions.expandBy(1);
  instructions[i].symbol = symbol;
  instructions[i].destination = destination;
  instructions[i].sources = sources;  // deep copy
}

void
FreeRhsAutomaton::remapIndices(VariableInfo& variableInfo)
{
  int nrInstructions = instructions.length();
  for (int i = 0; i < nrInstructions; i++)
    {
      Instruction& instr = instructions[i];
      instr.destination = variableInfo.remapIndex(instr.destination);
      int nrArgs = instr.sources.length();
      for (int j = 0; j < nrArgs; j++)
	instr.sources[j] = variableInfo.remapIndex(instr.sources[j]);
    }
}

local_inline void
FreeRhsAutomaton::fillOutArgs(const Instruction& instr,
			      Substitution& matcher,
			      FreeDagNode* d)
{
  int nrArgs = d->symbol()->arity();
  if (nrArgs != 0)
    {
      DagNode** args = d->argArray();
      for (Vector<int>::const_iterator j = instr.sources.begin();; ++args, ++j)
	{
	  *args = matcher.value(*j);
	  if (--nrArgs == 0)
	    break;
	}
    }
}

DagNode*
FreeRhsAutomaton::construct(Substitution& matcher)
{
  FreeDagNode* d;
  Vector<Instruction>::const_iterator i = instructions.begin();
  for (int nrInstructions = instructions.size();; ++i)
    {
      d = new FreeDagNode(i->symbol);
      fillOutArgs(*i, matcher, d);
      matcher.bind(i->destination, d);
      if (--nrInstructions == 0)
	break;
    }
  return d;
}

void
FreeRhsAutomaton::replace(DagNode* old, Substitution& matcher)
{
  Vector<Instruction>::const_iterator i = instructions.begin();
  for (int nrInstructions = instructions.size(); --nrInstructions != 0; ++i)
    {
      FreeDagNode* d = new FreeDagNode(i->symbol);
      fillOutArgs(*i, matcher, d);
      matcher.bind(i->destination, d);
    }
  FreeDagNode* d = new(old) FreeDagNode(i->symbol);
  fillOutArgs(*i, matcher, d);
}

#ifdef DUMP
void
FreeRhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{FreeRhsAutomaton}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "Instructions:\n";
  ++indentLevel;
  int nrInstructions = instructions.length();
  for (int i = 0; i < nrInstructions; i++)
    {
      s << Indent(indentLevel) << "[" << instructions[i].destination << "] <= "
	<< instructions[i].symbol << '(';
      int nrArgs = instructions[i].symbol->arity();
      for (int j = 0; j < nrArgs; j++)
	{
	  s << '[' << instructions[i].sources[j] << ']';
	  if (j + 1 < nrArgs)
	    s << ", ";
	}
      s << ")\n";
    }
  s << Indent(indentLevel - 2) << "End{FreeRhsAutomaton}\n";
}
#endif
