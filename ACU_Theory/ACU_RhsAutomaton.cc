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
//      Implementation for class ACU_RhsAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "ACU_Persistent.hh"
#include "ACU_Theory.hh"

//      core class definitions
#include "substitution.hh"
#include "variableInfo.hh"

//      ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_RhsAutomaton.hh"

ACU_RhsAutomaton::ACU_RhsAutomaton(ACU_Symbol* symbol, int nrArgs)
  : topSymbol(symbol),
    arguments(0, nrArgs)
{
}

void
ACU_RhsAutomaton::addArgument(int index, int multiplicity)
{
  int nrArgs = arguments.length();
  arguments.expandBy(1);
  arguments[nrArgs].index = index;
  arguments[nrArgs].multiplicity = multiplicity;
}

void
ACU_RhsAutomaton::close(int destinationIndex)
{
  destination = destinationIndex;
}

void
ACU_RhsAutomaton::remapIndices(VariableInfo& variableInfo)
{
  destination = variableInfo.remapIndex(destination);
  int nrArgs = arguments.length();
  for (int i = 0; i < nrArgs; i++)
    arguments[i].index = variableInfo.remapIndex(arguments[i].index);
}

local_inline void
ACU_RhsAutomaton::buildArguments(ArgVec<ACU_DagNode::Pair>& argArray, Substitution& matcher) const
{
  Vector<Argument>::const_iterator j = arguments.begin();
  const Vector<Argument>::const_iterator e = arguments.end();
  Assert(j != e, "no args");
  ArgVec<ACU_DagNode::Pair>::iterator i = argArray.begin();
  do
    {
      i->dagNode = matcher.value(j->index);
      i->multiplicity = j->multiplicity;
      ++i;
      ++j;
    }
  while (j != e);
  Assert(i == argArray.end(), "iterator inconsistent");
}

DagNode*
ACU_RhsAutomaton::construct(Substitution& matcher)
{
  ACU_DagNode* n = new ACU_DagNode(topSymbol, arguments.length());
  buildArguments(n->argArray, matcher);
  matcher.bind(destination, n);
  return n;
}

void
ACU_RhsAutomaton::replace(DagNode* old, Substitution& matcher)
{
  buildArguments((new(old) ACU_DagNode(topSymbol, arguments.length()))->argArray, matcher);
}

#ifdef DUMP
void
ACU_RhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{ACU_RhsAutomaton}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "[" << destination << "] <= " << topSymbol << '(';
  int nrArgs = arguments.length();
  for (int i = 0; i < nrArgs; i++)
    {
      s << '[' << arguments[i].index << "]^" << arguments[i].multiplicity;
      if (i + 1 < nrArgs)
	s << ", ";
    }
  s << ")\n";
  s << Indent(indentLevel - 1) << "End{ACU_RhsAutomaton}\n";
}
#endif
