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
//      Implementation for class CopyRhsAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      core class definitions
#include "substitution.hh"
#include "variableInfo.hh"
#include "copyRhsAutomaton.hh"

CopyRhsAutomaton::CopyRhsAutomaton(int originalIndex, int copyIndex)
  : originalIndex(originalIndex),
    copyIndex(copyIndex)
{
}

void
CopyRhsAutomaton::remapIndices(VariableInfo& variableInfo)
{
  originalIndex = variableInfo.remapIndex(originalIndex);
  copyIndex = variableInfo.remapIndex(copyIndex);
}

DagNode*
CopyRhsAutomaton::construct(Substitution& matcher)
{
  DagNode* orig = matcher.value(originalIndex);
  //cerr << "CopyRhsAutomaton::construct " << orig << endl;
  DagNode* n = orig->copyEagerUptoReduced();
  orig->clearCopyPointers();
  matcher.bind(copyIndex, n);
  return n;
}

void
CopyRhsAutomaton::replace(DagNode* old, Substitution& matcher)
{
  DagNode* orig = matcher.value(originalIndex);
  DagNode* n = orig->copyEagerUptoReduced();
  orig->clearCopyPointers();
  n->overwriteWithClone(old);
}

#ifdef DUMP
void
CopyRhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{CopyRhsAutomaton}\n";
  s << Indent(indentLevel + 1) << "originalIndex = " << originalIndex <<
    "\tcopyIndex = " << copyIndex << '\n';
  s << Indent(indentLevel) << "End{CopyRhsAutomaton}\n";
}
#endif
