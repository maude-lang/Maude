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
//      Implementation for VariableLhsAutomaton.
//
 
//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
 
//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "lhsAutomaton.hh"
 
//      core class definitions
#include "variableSymbol.hh"
#include "variableInfo.hh"
#include "substitution.hh"
#include "variableLhsAutomaton.hh"
 
VariableLhsAutomaton::VariableLhsAutomaton(int index,
					   const Sort* sort,
					   bool copyToAvoidOverwriting)
  : index(index),
    sort(sort),
    copyToAvoidOverwriting(copyToAvoidOverwriting)
{
}

bool
VariableLhsAutomaton::match(DagNode* subject,
			    Substitution& solution,
			    Subproblem*& returnedSubproblem,
			    ExtensionInfo* extensionInfo)
{
  return subject->matchVariable(index,
				sort,
				copyToAvoidOverwriting,
				solution,
				returnedSubproblem,
				extensionInfo);
}

#ifdef DUMP
void
VariableLhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{VariableLhsAutomaton}\n";
  s << Indent(indentLevel + 1) << "index = " << index <<
    " \"" << variableInfo.index2Variable(index) <<
    "\"\tsort = " <<  sort <<
    "\tcopyToAvoidOverwriting = " << copyToAvoidOverwriting << '\n';
  s << Indent(indentLevel) << "End{VariableLhsAutomaton}\n";
}
#endif
