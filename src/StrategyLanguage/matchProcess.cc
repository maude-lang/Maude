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
//      Implementation for class for processes that solve matching problems
//      associated with condition fragments.
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
#include "matchProcess.hh"

MatchProcess::MatchProcess(const shared_ptr<RewriteSearchState>& rewriteState,
			   PositionState::PositionIndex redexIndex,
			   ExtensionInfo* extensionInfo,
			   RewritingContext* matchContext,
			   Subproblem* subproblem,
			   Rule* rule,
			   int fragmentNr,
			   const Vector<StrategyExpression*>& strategies,
			   int strategyNr,
			   StrategyStackManager::StackId pending,
			   StrategicExecution* taskSibling,
			   StrategicProcess* insertionPoint)
  : StrategicProcess(taskSibling, insertionPoint),
    rewriteState(rewriteState),  // share rewrite state
    redexIndex(redexIndex),  // copy redex index
    extensionInfoCopy((extensionInfo == 0) ? 0 : extensionInfo->makeClone()),  // clone extension info
    matchContext(matchContext),  // take over ownership of matchContext
    subproblem(subproblem),  // take over ownership of subproblem
    rule(rule),  // copy rule pointer
    fragmentNr(fragmentNr),  // copy fragment number
    strategies(strategies),  // share reference to strategies vector
    strategyNr(strategyNr),  // copy strategy number
    pending(pending)  // copy pending stack
{
  DebugAdvisory("created MatchProcess, matchContext has nrFragileBindings = " << matchContext->nrFragileBindings());
  findFirst = true;
}

MatchProcess::~MatchProcess()
{
  delete extensionInfoCopy;
  delete matchContext;
  delete subproblem;
}

StrategicExecution::Survival
MatchProcess::run(StrategicSearch& searchObject)
{
  //
  //	Each time we run we look for a new solution to the matching problem
  //	If we find one we call resolveRemainingConditionFragments() which will
  //	fork off a new process to continue the seach. If we don't find one,
  //	there is no more work to do and we die.
  //
  bool success;
  if (subproblem)
    {
      success = subproblem->solve(findFirst, *matchContext);
      //
      //	solve() could perform rewrites to apply cmbs.
      //
      searchObject.getContext()->transferCountFrom(*matchContext);
    }
  else
    success = findFirst;
  if (success)
    {
      findFirst = false;
      if (ApplicationProcess::resolveRemainingConditionFragments(searchObject,
								 rewriteState,
								 redexIndex,
								 extensionInfoCopy,
								 matchContext,
								 rule,
								 fragmentNr + 1,
								 strategies,
								 strategyNr,
								 pending,
								 this,
								 this) == SURVIVE)
	return SURVIVE;
    }
  finished(this);
  return DIE;
}
