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
//      Implementation for class CUI_RhsAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "CUI_Theory.hh"

//      core class definitions
#include "substitution.hh"
#include "variableInfo.hh"

//      CUI theory class definitions
#include "CUI_Symbol.hh"
#include "CUI_DagNode.hh"
#include "CUI_RhsAutomaton.hh"

CUI_RhsAutomaton::CUI_RhsAutomaton(CUI_Symbol* symbol)
  : topSymbol(symbol)
{
}

void
CUI_RhsAutomaton::remapIndices(VariableInfo& variableInfo)
{
  source0 = variableInfo.remapIndex(source0);
  source1 = variableInfo.remapIndex(source1);
  destination = variableInfo.remapIndex(destination);
}

void
CUI_RhsAutomaton::close(int sourceIndex0, int sourceIndex1, int destinationIndex)
{
  source0 = sourceIndex0;
  source1 = sourceIndex1;
  destination = destinationIndex;
}

DagNode*
CUI_RhsAutomaton::construct(Substitution& matcher)
{
  CUI_DagNode* n = new CUI_DagNode(topSymbol);
  n->argArray[0] = matcher.value(source0);
  n->argArray[1] = matcher.value(source1);
  matcher.bind(destination, n);
  return n;
}

void
CUI_RhsAutomaton::replace(DagNode* old, Substitution& matcher)
{
  CUI_DagNode* n = new(old) CUI_DagNode(topSymbol);
  n->argArray[0] = matcher.value(source0);
  n->argArray[1] = matcher.value(source1);
}

#ifdef DUMP
void
CUI_RhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{CUI_RhsAutomaton}\n";
  s << Indent(indentLevel + 1) << "[" << destination << "] <= " << topSymbol <<
    '(' << '[' << source0 << "], [" << source1 << "])\n";
  s << Indent(indentLevel) << "End{CUI_RhsAutomaton}\n";
}
#endif
