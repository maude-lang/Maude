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
//	Another class for executing the matchrew subterm rewriting.
//
//	Intented to be used by the model checker only, it generates
//	rewriting traces that are complete whenever the order in which
//	the different subterms are rewritten is irrelevant (partial
//	order reduction).
//
#ifndef _biasedSubtermTask_hh_
#define _biasedSubtermTask_hh_

//	strategy language definitions
#include "modelCheckerSubtermTask.hh"

class BiasedSubtermTask : public ModelCheckerSubtermTask
{
  NO_COPYING(BiasedSubtermTask);

public:
  BiasedSubtermTask(
    StrategicSearch& searchObject,
    SubtermStrategy* strategy,
    shared_ptr<MatchSearchState> searchState,
    ExtensionInfo* extensionInfo,
    MatchSearchState::PositionIndex searchPosition,
    StrategyStackManager::StackId pending,
    VariableBindingsManager::ContextId varBinds,
    StrategicExecution* sibling,
    StrategicProcess* insertionPoint
  );

  BiasedSubtermTask(BiasedSubtermTask &subterm,
		    size_t currentSubterm,
		    StrategicProcess* insertionPoint);

  int onCommitState(int dagNode,
		    StrategyStackManager::StackId stackId,
		    StrategicExecution* taskSibling,
		    const StrategyTransitionGraph::Transition& transition) override;

private:
  //
  //	Virtual in StrategicTask.
  //
  Survival executionSucceeded(int resultIndex, StrategicProcess* insertionPoint) override;

  // The index of the current subterm being rewritten
  size_t currentSubterm;
};

#endif
