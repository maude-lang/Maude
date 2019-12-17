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
//      Implementation for class BindingLhsAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      core class definitions
#include "substitution.hh"
#include "bindingLhsAutomaton.hh"

BindingLhsAutomaton::BindingLhsAutomaton(int variableIndex, LhsAutomaton* realLhsAutomaton)
  : variableIndex(variableIndex),
    realLhsAutomaton(realLhsAutomaton)
{
}

BindingLhsAutomaton::~BindingLhsAutomaton()
{
  delete realLhsAutomaton;
}

bool
BindingLhsAutomaton::match(DagNode* subject,
			   Substitution& solution,
			   Subproblem*& returnedSubproblem,
			   ExtensionInfo* extensionInfo)
{
  if (realLhsAutomaton->match(subject, solution, returnedSubproblem, extensionInfo))
    {
      solution.bind(variableIndex, subject);
      return true;
    }
  return false;
}

#ifdef DUMP
void
BindingLhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{BindingLhsAutomaton}\n";
  s << Indent(indentLevel + 1) << "variableIndex = " << variableIndex << '\n';
  s << Indent(indentLevel + 1) << "realLhsAutomaton =\n";
  realLhsAutomaton->dump(s, variableInfo, indentLevel + 2);
  s << Indent(indentLevel) << "End{BindingLhsAutomaton}\n";
}
#endif
