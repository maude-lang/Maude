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
//      Implementation for class NA_LhsAutomaton.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "NA_Theory.hh"

//      NA theory class definitions
#include "NA_Symbol.hh"
#include "NA_Term.hh"
#include "NA_DagNode.hh"
#include "NA_LhsAutomaton.hh"

NA_LhsAutomaton::NA_LhsAutomaton(NA_Term* term) : term(term)
{
}

bool
NA_LhsAutomaton::match(DagNode* subject,
		       Substitution& /* solution */,
		       Subproblem*& returnedSubproblem,
		       ExtensionInfo* /* extensionInfo */)
{
  returnedSubproblem = 0;
  return term->equal(subject);
}

#ifdef DUMP
void
NA_LhsAutomaton::dump(ostream& s, const VariableInfo& /* variableInfo */, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{NA_LhsAutomaton}\n";
  s << Indent(indentLevel + 1) << "term = " << term << '\n';
  s << Indent(indentLevel) << "End{NA_LhsAutomaton}\n";
}
#endif
