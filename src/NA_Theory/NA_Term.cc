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
//      Implementation for class NA_Term.
//
 
//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "NA_Theory.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"

//	core class definitions
#include "variableInfo.hh"
#include "termBag.hh"
#include "rhsBuilder.hh"

//      NA theory class definitions
#include "NA_Symbol.hh"
#include "NA_DagNode.hh"
#include "NA_Term.hh"
#include "NA_LhsAutomaton.hh"
#include "NA_RhsAutomaton.hh"

NA_Term::NA_Term(NA_Symbol* symbol) : Term(symbol)
{
}

RawArgumentIterator*
NA_Term::arguments()
{
  return 0;
}

void
NA_Term::deepSelfDestruct()
{
  delete this;
}

void
NA_Term::findEagerVariables(bool /* atTop */, NatSet& /* eagerVariables */) const
{
}

void
NA_Term::markEagerArguments(int /* nrVariables */,
			    const NatSet& /* eagerVariables */,
			    Vector<int>& /* problemVariables */)
{
}

void
NA_Term::analyseConstraintPropagation(NatSet& /* BoundUniquely */) const
{
}

LhsAutomaton*
NA_Term::compileLhs2(bool /* matchAtTop */,
		    const VariableInfo& /* variableInfo */,
		    NatSet& /* boundUniquely */,
		    bool& subproblemLikely)
{
  subproblemLikely = false;
  return new NA_LhsAutomaton(this);
}

DagNode*
NA_Term::dagify2()
{
  return makeDagNode();
}

void
NA_Term::findAvailableTerms(TermBag& /* availableTerm */,
			    bool /* eagerContext */,
			    bool /* atTop */)
{
}

int
NA_Term::compileRhs2(RhsBuilder& rhsBuilder,
		     VariableInfo& variableInfo,
		     TermBag& /* availableTerms */,
		     bool /* eagerContext */)
{
  int index = variableInfo.makeConstructionIndex();
  rhsBuilder.addRhsAutomaton(new NA_RhsAutomaton(this, index));
  return index;
}
