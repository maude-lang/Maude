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
//      Implementation for class S_RhsAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "S_Theory.hh"

//      core class definitions
#include "substitution.hh"
#include "variableInfo.hh"

//      S theory class definitions
#include "S_Symbol.hh"
#include "S_DagNode.hh"
#include "S_RhsAutomaton.hh"

S_RhsAutomaton::S_RhsAutomaton(S_Symbol* symbol,
			       mpz_class& number,
			       int source,
			       int destination)
  : topSymbol(symbol),
    number(number),
    source(source),
    destination(destination)
{
}

void
S_RhsAutomaton::remapIndices(VariableInfo& variableInfo)
{
  source = variableInfo.remapIndex(source);
  destination = variableInfo.remapIndex(destination);
}

DagNode*
S_RhsAutomaton::construct(Substitution& matcher)
{
  S_DagNode* n = new S_DagNode(topSymbol, number, matcher.value(source));
  matcher.bind(destination, n);
  return n;
}

void
S_RhsAutomaton::replace(DagNode* old, Substitution& matcher)
{
  (void) new(old) S_DagNode(topSymbol, number, matcher.value(source));
}

#ifdef DUMP
void
S_RhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{S_RhsAutomaton}\n";
  s << Indent(indentLevel + 1) << "[" << destination << "] <= " << topSymbol <<
    '^' << number << '(' << '[' << source << "])\n";
  s << Indent(indentLevel) << "End{S_RhsAutomaton}\n";
}
#endif
