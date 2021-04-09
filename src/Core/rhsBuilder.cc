/*

    This file is part of the Maude 3 interpreter.

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
//      Implementation for class RhsBuilder.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      core class definitions
#include "rhsBuilder.hh"

RhsBuilder::~RhsBuilder()
{
  int nrAutomata = automata.length();
  for (int i = 0; i < nrAutomata; i++)
    delete automata[i];
  delete lastAutomaton;
}

void
RhsBuilder::remapIndices(VariableInfo& variableInfo)
{
  int nrAutomata = automata.length();
  for (int i = 0; i < nrAutomata; i++)
    automata[i]->remapIndices(variableInfo);
  if (lastAutomaton != 0)
    lastAutomaton->remapIndices(variableInfo);
}

bool
RhsBuilder::recordInfo(StackMachineRhsCompiler& compiler)
{
  for (RhsAutomaton* ra : automata)
    {
      if (!(ra->recordInfo(compiler)))
	return false;
    }
  return (lastAutomaton == 0) ? true : lastAutomaton->recordInfo(compiler);
}

#ifdef DUMP
void
RhsBuilder::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{FreeRhsAutomaton}\n";
  s << Indent(indentLevel + 1) << "automata:\n";
  int nrAutomata = automata.length();
  for (int i = 0; i < nrAutomata; i++)
    automata[i]->dump(s, variableInfo, indentLevel + 2);
  s << Indent(indentLevel + 1) << "lastAutomata:\n";
  lastAutomaton->dump(s, variableInfo, indentLevel + 2);
  s << Indent(indentLevel) << "End{FreeRhsAutomaton}\n";
}
#endif
