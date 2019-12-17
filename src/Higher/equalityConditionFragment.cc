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
//	Implementation for class EqualityConditionFragment.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
//#include "variable.hh"

//	interface class definitions
//#include "symbol.hh"
//#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"
#include "subproblem.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "variableInfo.hh"
//#include "conditionState.hh"

//	higher class definitions
#include "equalityConditionFragment.hh"

EqualityConditionFragment::EqualityConditionFragment(Term* lhs, Term* rhs)
  : lhs(lhs),
    rhs(rhs)
{
  lhsIndex = NONE;
  rhsIndex = NONE;
}

EqualityConditionFragment::~EqualityConditionFragment()
{
  lhs->deepSelfDestruct();
  rhs->deepSelfDestruct();
}

void
EqualityConditionFragment::check(VariableInfo& variableInfo, NatSet& boundVariables)
{
  NatSet unboundVariables;

  lhs = lhs->normalize(false);
  lhs->indexVariables(variableInfo);
  variableInfo.addConditionVariables(lhs->occursBelow());
  unboundVariables.insert(lhs->occursBelow());

  rhs = rhs->normalize(false);
  rhs->indexVariables(variableInfo);
  variableInfo.addConditionVariables(rhs->occursBelow());
  unboundVariables.insert(rhs->occursBelow());

  unboundVariables.subtract(boundVariables);
  variableInfo.addUnboundVariables(unboundVariables);
}

void
EqualityConditionFragment::preprocess()
{
  lhs->symbol()->fillInSortInfo(lhs);
  rhs->symbol()->fillInSortInfo(rhs);
  Assert(lhs->getComponent() == rhs->getComponent(), "component clash");
}

void
EqualityConditionFragment::compileBuild(VariableInfo& variableInfo, TermBag& availableTerms)
{
  lhsIndex = lhs->compileRhs(builder, variableInfo, availableTerms, true);
  rhsIndex = rhs->compileRhs(builder, variableInfo, availableTerms, true);
  variableInfo.useIndex(lhsIndex);
  variableInfo.useIndex(rhsIndex);
  variableInfo.endOfFragment();
}

void
EqualityConditionFragment::compileMatch(VariableInfo& variableInfo, NatSet& boundUniquely)
{
  builder.remapIndices(variableInfo);
  lhsIndex = variableInfo.remapIndex(lhsIndex);
  rhsIndex = variableInfo.remapIndex(rhsIndex);
}

bool
EqualityConditionFragment::solve(bool findFirst,
				 RewritingContext& solution,
				 Stack<ConditionState*>& /* state */)
{
  if (!findFirst)
    return false;
  builder.safeConstruct(solution);
  RewritingContext* lhsContext =
    solution.makeSubcontext(solution.value(lhsIndex), RewritingContext::CONDITION_EVAL);
  RewritingContext* rhsContext =
    solution.makeSubcontext(solution.value(rhsIndex), RewritingContext::CONDITION_EVAL);
  lhsContext->reduce();
  solution.addInCount(*lhsContext);
  rhsContext->reduce();
  solution.addInCount(*rhsContext);
  //
  //	Check for equality modulo axioms.
  //
  bool success = lhsContext->root()->equal(rhsContext->root());
  delete lhsContext;
  delete rhsContext;
  return success;
}

void
EqualityConditionFragment::buildInstances(Substitution& substitution, DagNode*& lhs, DagNode*& rhs)
{
  //
  //	Equality fragments have a very different operational semantics when rewriting module SMT.
  //	All we do here is return the instantiated lhs and rhs terms.
  //
  builder.safeConstruct(substitution);
  lhs = substitution.value(lhsIndex);
  rhs = substitution.value(rhsIndex);
}

#ifdef DUMP
void
EqualityConditionFragment::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{EqualityConditionFragment}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "lhs = " << lhs << '\n';
  s << Indent(indentLevel) << "rhs = " << rhs << '\n';
  s << Indent(indentLevel) << "lhsIndex = " << lhsIndex << '\n';
  s << Indent(indentLevel) << "rhsIndex = " << rhsIndex << '\n';
  s << Indent(indentLevel - 1) << "End{EqualityConditionFragment}\n";
}
#endif
