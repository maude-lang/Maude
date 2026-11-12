/*

    This file is part of the Maude 3 strategy-aware model checker.

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
//      Implementation for class BiasedSubtermTask.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "strategyLanguage.hh"
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "extensionInfo.hh"

//	higuer class definitions
#include "strategyTransitionGraph.hh"

//	strategy language class definitions
#include "biasedSubtermTask.hh"
#include "strategicSearch.hh"
#include "subtermStrategy.hh"
#include "decompositionProcess.hh"

using Survival = StrategicExecution::Survival;

BiasedSubtermTask::BiasedSubtermTask(StrategicSearch &searchObject,
	SubtermStrategy* strategy,
	shared_ptr<MatchSearchState> searchState,
	ExtensionInfo* extensionInfo,
	MatchSearchState::PositionIndex searchPosition,
	StrategyStackManager::StackId pending,
	VariableBindingsManager::ContextId varBinds,
	StrategicExecution* sibling,
	StrategicProcess* insertionPoint
)
  : ModelCheckerSubtermTask(searchObject, strategy, searchState,
			    extensionInfo, searchPosition,
			    pending, varBinds, sibling),
    currentSubterm(0)
{
  // Creates a descomposition process for the first subterm
  (void) new DecompositionProcess(searchObject.insert(subterms[0]),
				  searchObject.push(
					  StrategyStackManager::EMPTY_STACK,
					  strategy->getStrategies()[0]),
				  getDummyExecution(),
				  insertionPoint);
}

BiasedSubtermTask::BiasedSubtermTask(BiasedSubtermTask& task, size_t currentSubterm,
				     StrategicProcess* insertionPoint)
  : ModelCheckerSubtermTask(task, task.subterms),
    currentSubterm(currentSubterm)
{
  Assert(currentSubterm < strategy->getSubterms().size(), "current subterm out of bounds");

  // Creates a descomposition process for the given subterm
  (void) new DecompositionProcess(task.searchObject.insert(subterms[currentSubterm]),
				  task.searchObject.push(
					  StrategyStackManager::EMPTY_STACK,
					  strategy->getStrategies()[currentSubterm]),
				  getDummyExecution(),
				  insertionPoint);
}

Survival BiasedSubtermTask::executionSucceeded(int dagNode, StrategicProcess* insertionPoint)
{
  if (currentSubterm + 1 == subterms.size())
    {
      // This rebuild was already done when the rewrite has been issued (try to prevent it)
      subterms[currentSubterm] = searchObject.getCanonical(dagNode);
      resumeOwner(rebuild(subterms), pending, insertionPoint);
    }
  else
    {
      subterms[currentSubterm] = searchObject.getCanonical(dagNode);

      // Fork a subterm process to rewrite the next subterm
      (void) new BiasedSubtermTask(*this, currentSubterm + 1, insertionPoint);
    }

  return StrategicExecution::SURVIVE;  // still want more solutions if available
}

int BiasedSubtermTask::onCommitState(int rewrittenSubterm,
				     StrategyStackManager::StackId,
				     StrategicExecution*,
				     const StrategyTransitionGraph::Transition& transition)
{
  subterms[currentSubterm] = searchObject.getCanonical(rewrittenSubterm);
  return rebuild(subterms);
}
