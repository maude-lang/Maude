/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2006 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for the process controlling the matchrew main pattern search.
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

//	higher class definitions
#include "matchSearchState.hh"

//	strategy language class definitions
#include "strategicSearch.hh"
#include "subtermProcess.hh"
#include "subtermTask.hh"
#include "biasedSubtermTask.hh"
#include "fullSubtermTask.hh"
#include "subtermStrategy.hh"

SubtermProcess::SubtermProcess(MatchSearchState* matchState,
			       SubtermStrategy* strategy,
			       StrategyStackManager::StackId pending,
			       StrategicExecution* taskSibling,
			       StrategicProcess* insertionPoint)
  : StrategicProcess(taskSibling, insertionPoint),
    matchState(matchState),
    strategy(strategy),
    pending(pending)
{
}

StrategicExecution::Survival
SubtermProcess::run(StrategicSearch& searchObject)
{
  bool matched = matchState->findNextMatch();

  RewritingContext* context = searchObject.getContext();
  context->transferCountFrom(*matchState->getContext());

  if (matched)
    {
      ExtensionInfo* extensionInfo = matchState->getExtensionInfo();

      // If this is a extended match the extension information should be cloned
      // (because matchSearchState will reuse when searching again). And if the
      // whole term was matched, we can get rid of it.
      if (extensionInfo)
	extensionInfo = !extensionInfo->matchedWhole() ? extensionInfo->makeClone() : nullptr;

      VariableBindingsManager::ContextId outerBinds = getOwner()->getVarsContext();
      const Vector<int>& contextSpec = strategy->getContextSpec();

      // The variable inside the matchrew context mix those of the outer
      // context with those from the matching
      VariableBindingsManager::ContextId innerBinds = contextSpec.empty()
		? VariableBindingsManager::EMPTY_CONTEXT
		: searchObject.openContext(outerBinds, *matchState->getContext(), contextSpec);

      // If we are model checking, we use a specific implementation
      // of the matchrew operator. Otherwise, we use another one
      // optimized for execution.
      StrategyTransitionGraph* graph = getOwner()->getTransitionGraph();
      if (graph != nullptr)
	if (strategy->getSubterms().size() == 1 || graph->useBiasedMatchrew())
	  (void) new BiasedSubtermTask(searchObject, strategy,
				      matchState, extensionInfo,
				      matchState->getPositionIndex(),
				      pending, innerBinds, this, this);
	else
	  (void) new FullSubtermTask(searchObject, strategy,
				      matchState, extensionInfo,
				      matchState->getPositionIndex(),
				      pending, innerBinds, this, this);
      else
	(void) new SubtermTask(searchObject, strategy,
			       matchState, extensionInfo,
			       matchState->getPositionIndex(),
			       pending, innerBinds, this, this);

      return StrategicExecution::SURVIVE;
    }

  // Neither solution has been found nor will be found
  finished(this);
  return StrategicExecution::DIE;
}
