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
//      Implementation for class AU_RhsAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "AU_Theory.hh"

//      core class definitions
#include "substitution.hh"
#include "variableInfo.hh"

//      AU theory class definitions
#include "AU_Symbol.hh"
#include "AU_DagNode.hh"
#include "AU_RhsAutomaton.hh"

AU_RhsAutomaton::AU_RhsAutomaton(AU_Symbol* symbol, int nrArgs)
  : topSymbol(symbol), arguments(0, nrArgs)
{
}

void
AU_RhsAutomaton::buildArguments(ArgVec<DagNode*>& argArray, Substitution& matcher) const
{
  int nrArgs = arguments.length();
  for (int i = 0; i < nrArgs; i++)
    argArray[i] = matcher.value(arguments[i]);
}

void
AU_RhsAutomaton::remapIndices(VariableInfo& variableInfo)
{
  destination = variableInfo.remapIndex(destination);
  int nrArgs = arguments.length();
  for (int i = 0; i < nrArgs; i++)
    arguments[i] = variableInfo.remapIndex(arguments[i]);
}

DagNode*
AU_RhsAutomaton::construct(Substitution& matcher)
{
  AU_DagNode* n = new AU_DagNode(topSymbol, arguments.length());
  buildArguments(n->argArray, matcher);
  matcher.bind(destination, n);
  return n;
}

void
AU_RhsAutomaton::replace(DagNode* old, Substitution& matcher)
{
  buildArguments((new(old) AU_DagNode(topSymbol, arguments.length()))->argArray, matcher);
}

#ifdef DUMP
void
AU_RhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{AU_RhsAutomaton}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "[" << destination << "] <= " << topSymbol << '(';
  int nrArgs = arguments.length();
  for (int i = 0; i < nrArgs; i++)
    {
      s << '[' << arguments[i] << "]";
      if (i + 1 < nrArgs)
	s << ", ";
    }
  s << ")\n";
  s << Indent(indentLevel - 1) << "End{AU_RhsAutomaton}\n";
}
#endif
