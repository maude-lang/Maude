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
//	Implementation for class SortTestConditionFragment.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

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
#include "sortTestConditionFragment.hh"

SortTestConditionFragment::SortTestConditionFragment(Term* lhs, Sort* sort)
  : lhs(lhs),
    sort(sort)
{
  lhsIndex = NONE;
}

SortTestConditionFragment::~SortTestConditionFragment()
{
  lhs->deepSelfDestruct();
}

void
SortTestConditionFragment::check(VariableInfo& variableInfo, NatSet& boundVariables)
{
  NatSet unboundVariables;

  lhs->normalize(false);
  lhs->indexVariables(variableInfo);
  variableInfo.addConditionVariables(lhs->occursBelow());
  unboundVariables.insert(lhs->occursBelow());
  unboundVariables.subtract(boundVariables);
  variableInfo.addUnboundVariables(unboundVariables);
}

void
SortTestConditionFragment::preprocess()
{
  lhs->symbol()->fillInSortInfo(lhs);
  Assert(lhs->getComponent() == sort->component(), "component clash");
}

void
SortTestConditionFragment::compileBuild(VariableInfo& variableInfo, TermBag& availableTerms)
{
  lhsIndex = lhs->compileRhs(builder, variableInfo, availableTerms, true);
  variableInfo.useIndex(lhsIndex);
  variableInfo.endOfFragment();
}

void
SortTestConditionFragment::compileMatch(VariableInfo& variableInfo, NatSet& boundUniquely)
{
  builder.remapIndices(variableInfo);
  lhsIndex = variableInfo.remapIndex(lhsIndex);
}

bool
SortTestConditionFragment::solve(bool findFirst,
				 RewritingContext& solution,
				 stack<ConditionState*>& state)
{
  if (!findFirst)
    return false;
  builder.safeConstruct(solution);
  RewritingContext* lhsContext =
    solution.makeSubcontext(solution.value(lhsIndex), RewritingContext::CONDITION_EVAL);
  lhsContext->reduce();
  solution.addInCount(*lhsContext);
  //
  //	Check for low enough sort axioms.
  //
  bool success = lhsContext->root()->leq(sort);
  delete lhsContext;
  return success;
}
