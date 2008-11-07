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
//	Implementation for class AssignmentConditionFragment.
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

//	higher class definitions
#include "assignmentConditionState.hh"
#include "assignmentConditionFragment.hh"

AssignmentConditionFragment::AssignmentConditionFragment(Term* lhs, Term* rhs)
  : lhs(lhs),
    rhs(rhs)
{
  lhsMatcher = 0;
  rhsIndex = NONE;
}

AssignmentConditionFragment::~AssignmentConditionFragment()
{
  lhs->deepSelfDestruct();
  rhs->deepSelfDestruct();
  delete lhsMatcher;
}

void
AssignmentConditionFragment::check(VariableInfo& variableInfo, NatSet& boundVariables)
{
  NatSet unboundVariables;

  lhs = lhs->normalize(true);
  lhs->indexVariables(variableInfo);
  variableInfo.addConditionVariables(lhs->occursBelow());

  rhs = rhs->normalize(false);
  rhs->indexVariables(variableInfo);
  variableInfo.addConditionVariables(rhs->occursBelow());
  unboundVariables.insert(rhs->occursBelow());

  unboundVariables.subtract(boundVariables);
  variableInfo.addUnboundVariables(unboundVariables);
  boundVariables.insert(lhs->occursBelow());
}

void
AssignmentConditionFragment::preprocess()
{
  lhs->symbol()->fillInSortInfo(lhs);
  lhs->analyseCollapses();
  rhs->symbol()->fillInSortInfo(rhs);
  Assert(lhs->getComponent() == rhs->getComponent(), "component clash");
}

void
AssignmentConditionFragment::compileBuild(VariableInfo& variableInfo, TermBag& availableTerms)
{
  rhsIndex = rhs->compileRhs(builder, variableInfo, availableTerms, true);
  variableInfo.useIndex(rhsIndex);
  lhs->findAvailableTerms(availableTerms, true);
  lhs->determineContextVariables();
  lhs->insertAbstractionVariables(variableInfo);
  variableInfo.endOfFragment();
}

void
AssignmentConditionFragment::compileMatch(VariableInfo& variableInfo, NatSet& boundUniquely)
{
  builder.remapIndices(variableInfo);
  rhsIndex = variableInfo.remapIndex(rhsIndex);
  bool subproblemLikely;
  lhsMatcher = lhs->compileLhs(false, variableInfo, boundUniquely, subproblemLikely);
  boundUniquely.insert(lhs->occursBelow());
}

bool
AssignmentConditionFragment::solve(bool findFirst,
				   RewritingContext& solution,
				   stack<ConditionState*>& state)
{
  if (findFirst)
    {
      builder.safeConstruct(solution);
      AssignmentConditionState* cs =
	new AssignmentConditionState(solution, lhsMatcher, solution.value(rhsIndex));
      if (cs->solve(true, solution))
	{
	  state.push(cs);
	  return true;
	}
      delete cs;
    }
  else
    {
      AssignmentConditionState* cs = safeCast(AssignmentConditionState*, state.top());
      if (cs->solve(false, solution))
	return true;
      delete cs;
      state.pop();
    }
  return false;
}

DagNode*
AssignmentConditionFragment::makeRhsInstance(Substitution& solution)
{
  builder.safeConstruct(solution);
  return solution.value(rhsIndex);
}

bool
AssignmentConditionFragment::matchRoot(RewritingContext& context, Subproblem*& subproblem)
{
  subproblem = 0;
  return lhsMatcher->match(context.root(), context, subproblem);
}
