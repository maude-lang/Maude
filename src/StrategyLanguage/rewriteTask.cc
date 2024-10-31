/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class RewriteTask.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "strategyLanguage.hh"

//	interface class definitions
#include "term.hh"
#include "extensionInfo.hh"
#include "subproblem.hh"

//	core class definitions
#include "cachedDag.hh"
#include "rule.hh"

//	higher class definitions
#include "assignmentConditionFragment.hh"
#include "rewriteConditionFragment.hh"

//	strategy language class definitions
#include "strategicSearch.hh"
#include "strategyExpression.hh"
#include "applicationStrategy.hh"
#include "applicationProcess.hh"
#include "decompositionProcess.hh"
#include "matchProcess.hh"
#include "rewriteTask.hh"

RewriteTask::RewriteTask(StrategicSearch& searchObject,
			 shared_ptr<RewriteSearchState> rewriteState,
			 PositionState::PositionIndex redexIndex,
			 ExtensionInfo* extensionInfo,
			 Substitution* substitutionSoFar,
			 Rule* rule,
			 int fragmentNr,
			 const Vector<StrategyExpression*>& strategies,
			 int strategyNr,
			 StrategyStackManager::StackId pending,
			 StrategicExecution* taskSibling,
			 StrategicProcess* insertionPoint)
  : StrategicTask(taskSibling),
    hashConsSet(searchObject),
    rewriteState(rewriteState),  // share rewrite state
    redexIndex(redexIndex),  // copy redex index
    extensionInfoCopy((extensionInfo == 0) ? 0 : extensionInfo->makeClone()),  // clone extension info
    rule(rule),  // copy rule pointer
    fragmentNr(fragmentNr),  // copy fragment number
    strategies(strategies),  // share reference to strategies vector
    strategyNr(strategyNr),  // copy strategy number
    pending(pending)  // copy pending stack
{

  //
  //	Find the L => R fragment that we are going to test.
  //
  rcf = safeCastNonNull<RewriteConditionFragment*>((rule->getCondition())[fragmentNr]);
  //
  //	Make a subcontext, construct and evalutate the instance of L.
  //
  RewritingContext* baseContext = rewriteState->getContext();
  newContext = baseContext->makeSubcontext(rcf->makeLhsInstance(*substitutionSoFar), RewritingContext::CONDITION_EVAL);
  newContext->reduce();
  //
  //	We won't be doing anymore rewriting with newContext so it is safe to add in its rewrites here.
  //
  searchObject.getContext()->addInCount(*newContext);
  //
  //	We now reuse newContext to save a copy of the partial substitution (since we don't
  //	own substitutionSoFar an it will probably change between now and our call-backs being
  //	called.  We use clone() rather than copy() since the copy size will be wrong.
  //
  newContext->clone(*substitutionSoFar);
  //
  //	Start a new search from the result of evaluating L, using a substrategy.
  //
  StrategyExpression* substrategy = strategies[strategyNr];
  (void) new DecompositionProcess(searchObject.insert(newContext->root()),
				  searchObject.push(StrategyStackManager::EMPTY_STACK, substrategy),
				  getDummyExecution(),
				  insertionPoint);
}

RewriteTask::~RewriteTask()
{
  delete extensionInfoCopy;
  delete newContext;  // Can we delete this earlier?
}

StrategicExecution::Survival
RewriteTask::executionSucceeded(int resultIndex, StrategicProcess* insertionPoint)
{
  //
  //	We now need to match the result of a rewrite seach against our rhs pattern.
  //
  DagNode* result = hashConsSet.getCanonical(resultIndex);
  RewritingContext* matchContext = rewriteState->getContext()->makeSubcontext(result, RewritingContext::CONDITION_EVAL);
  //
  //	We need to use clone() rather than copy() since matchContext with have a copy size of zero.
  //
  matchContext->clone(*newContext);
  Subproblem* subproblem;
  if (rcf->matchRoot(*matchContext, subproblem))
    {
      (void) new MatchProcess(rewriteState,
			      redexIndex,
			      extensionInfoCopy,
			      matchContext,  // MatchProcess takes over ownership of matchContext
			      subproblem,  // MatchProcess takes over ownership of subproblem
			      rule,
			      fragmentNr,
			      strategies,
			      strategyNr + 1,
			      pending,
			      this,
			      insertionPoint);
    }
  else
    delete matchContext;
  return SURVIVE;
}

StrategicExecution::Survival
RewriteTask::executionsExhausted(StrategicProcess*)
{
  //
  //	We don't have any more slave executions so there are no more solutions to
  //	rewrite condition fragment.
  //
  return DIE;
}
