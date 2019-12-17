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
//      Implementation for class NA_RhsAutomaton.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "NA_Theory.hh"

//      core class definitions
#include "substitution.hh"
#include "variableInfo.hh"

//      NA theory class definitions
#include "NA_Symbol.hh"
#include "NA_Term.hh"
#include "NA_DagNode.hh"
#include "NA_RhsAutomaton.hh"

NA_RhsAutomaton::NA_RhsAutomaton(NA_Term* term, int destination)
  : term(term),
    destination(destination)
{
}

void
NA_RhsAutomaton::remapIndices(VariableInfo& variableInfo)
{
  destination = variableInfo.remapIndex(destination);
}

DagNode*
NA_RhsAutomaton::construct(Substitution& matcher)
{
  NA_DagNode* d = term->makeDagNode();
  matcher.bind(destination, d);
  return d;
}

void
NA_RhsAutomaton::replace(DagNode* old, Substitution& /* matcher */)
{
  term->overwriteWithDagNode(old);
}

#ifdef DUMP
void
NA_RhsAutomaton::dump(ostream& s, const VariableInfo& /* variableInfo */, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{NA_RhsAutomaton}\n";
  s << Indent(indentLevel + 1) << "[" << destination << "] <= " << term << '\n';
  s << Indent(indentLevel) << "End{NA_RhsAutomaton}\n";
}
#endif
