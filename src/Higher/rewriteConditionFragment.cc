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
//	Implementation for class ConditionFragment.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"

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
#include "rewriteConditionState.hh"
#include "rewriteConditionFragment.hh"

RewriteConditionFragment::RewriteConditionFragment(Term* lhs, Term* rhs)
  : lhs(lhs),
    rhs(rhs)
{
  lhsIndex = NONE;
  rhsMatcher = 0;
}

RewriteConditionFragment::~RewriteConditionFragment()
{
  lhs->deepSelfDestruct();
  rhs->deepSelfDestruct();
  delete rhsMatcher;
}

void
RewriteConditionFragment::check(VariableInfo& variableInfo, NatSet& boundVariables)
{
  NatSet unboundVariables;

  lhs->normalize(false);
  lhs->indexVariables(variableInfo);
  variableInfo.addConditionVariables(lhs->occursBelow());
  unboundVariables.insert(lhs->occursBelow());

  rhs->normalize(true);
  rhs->indexVariables(variableInfo);
  variableInfo.addConditionVariables(rhs->occursBelow());
      
  unboundVariables.subtract(boundVariables);
  variableInfo.addUnboundVariables(unboundVariables);
  boundVariables.insert(rhs->occursBelow());
}

void
RewriteConditionFragment::preprocess()
{
  lhs->symbol()->fillInSortInfo(lhs);
  rhs->symbol()->fillInSortInfo(rhs);
  Assert(lhs->getComponent() == rhs->getComponent(), "component clash");
  rhs->analyseCollapses();
}

void
RewriteConditionFragment::compileBuild(VariableInfo& variableInfo, TermBag& availableTerms)
{

  lhsIndex = lhs->compileRhs(builder, variableInfo, availableTerms, true);
  variableInfo.useIndex(lhsIndex);
  rhs->findAvailableTerms(availableTerms, true);
  rhs->determineContextVariables();
  rhs->insertAbstractionVariables(variableInfo);
  variableInfo.endOfFragment();
}

void
RewriteConditionFragment::compileMatch(VariableInfo& variableInfo, NatSet& boundUniquely)
{
  builder.remapIndices(variableInfo);
  lhsIndex = variableInfo.remapIndex(lhsIndex);
  bool subproblemLikely;
  rhsMatcher = rhs->compileLhs(false, variableInfo, boundUniquely, subproblemLikely);
  boundUniquely.insert(rhs->occursBelow());
}

bool
RewriteConditionFragment::solve(bool findFirst,
				RewritingContext& solution,
				stack<ConditionState*>& state)
{
  if (findFirst)
    {
      builder.safeConstruct(solution);
      RewriteConditionState* cs =
	new RewriteConditionState(solution, solution.value(lhsIndex), rhsMatcher);
      if (cs->solve(true, solution))
	{
	  state.push(cs);
	  return true;
	}
      delete cs;
    }
  else
    {
      RewriteConditionState* cs = safeCast(RewriteConditionState*, state.top());
      if (cs->solve(false, solution))
	return true;
      delete cs;
      state.pop();
    }
  return false;
}

DagNode*
RewriteConditionFragment::makeLhsInstance(Substitution& solution)
{
  builder.safeConstruct(solution);
  return solution.value(lhsIndex);
}

bool
RewriteConditionFragment::matchRoot(RewritingContext& context, Subproblem*& subproblem)
{
  subproblem = 0;
  return rhsMatcher->match(context.root(), context, subproblem);
}
