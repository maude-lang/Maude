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
//      Implementation for class ModelCheckerSubtermTask.
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
#include "modelCheckerSubtermTask.hh"
#include "strategicSearch.hh"
#include "subtermStrategy.hh"
#include "decompositionProcess.hh"

using Survival = StrategicExecution::Survival;

ModelCheckerSubtermTask::ModelCheckerSubtermTask(StrategicSearch &searchObject,
	SubtermStrategy* strategy,
	shared_ptr<MatchSearchState> searchState,
	ExtensionInfo* extensionInfo,
	MatchSearchState::PositionIndex searchPosition,
	StrategyStackManager::StackId pending,
	VariableBindingsManager::ContextId varBinds,
	StrategicExecution* sibling
)
  : StrategicTask(sibling, varBinds),
    searchState(searchState),
    extensionInfo(extensionInfo),
    searchIndex(searchPosition),
    searchObject(searchObject),
    strategy(strategy),
    subterms(strategy->getStrategies().size())
{
  Assert(strategy, "null strategy");

  // Uses the substitution from the search to instantiate the subpatterns
  Substitution &subst = *searchState->getContext();
  const Vector<Term*> &subpatterns = strategy->getSubterms();

  size_t nrSubterms = strategy->getStrategies().size();

  for (size_t i = 0; i < nrSubterms; i++)
    {
      DagNode* variable = subpatterns[i]->term2Dag(true);
      subterms[i] = variable->instantiate(subst, true);
    }

  StrategicTask::pending = pending;
  setEnclosingSubtermTask(this);
}

ModelCheckerSubtermTask::ModelCheckerSubtermTask(ModelCheckerSubtermTask &task,
	const Vector<DagNode*> &subterms
)
  : StrategicTask(&task, task.getVarsContext()),
    searchState(task.searchState),
    extensionInfo(task.extensionInfo),
    searchIndex(task.searchIndex),
    searchObject(task.searchObject),
    strategy(task.strategy),
    subterms(subterms)
{
  setEnclosingSubtermTask(this);
  StrategicTask::pending = task.pending;
}

ModelCheckerSubtermTask::~ModelCheckerSubtermTask()
{
  searchObject.closeContext(getVarsContext());
  delete extensionInfo;
}

Survival ModelCheckerSubtermTask::executionsExhausted(StrategicProcess*)
{
  return StrategicExecution::DIE;  // nothing more to do
}

int ModelCheckerSubtermTask::rebuild(const Vector<DagNode*> &subterms)
{
  // Rebuils the main pattern from rewritten terms and matched variables
  DagNode * result = strategy->rebuild(
	  searchObject.getValues(getVarsContext()),
	  subterms
  );

  // Rebuilds the node from the pattern match to the root
  result = searchState->rebuildDag(result, extensionInfo, searchIndex).first;

  // Reduces it
  RewritingContext* newContext = searchObject.getContext()->makeSubcontext(result);

  newContext->reduce();
  searchObject.getContext()->transferCountFrom(*newContext);

  int resultIndex = searchObject.insert(newContext->root());
  delete newContext;

  return resultIndex;
}
