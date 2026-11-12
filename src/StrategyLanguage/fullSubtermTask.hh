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
//	the full set of rewriting traces for the matchrew, i.e. all
//	rewrites in different subterms appear in any order. It is
//	computationally expensive but semantically accurate.
//
#ifndef _fullSubtermTask_hh_
#define _fullSubtermTask_hh_

//	strategy language definitions
#include "modelCheckerSubtermTask.hh"

class FullSubtermTask : public ModelCheckerSubtermTask
{
  NO_COPYING(FullSubtermTask);

public:
  FullSubtermTask(
    StrategicSearch &searchObject,
    SubtermStrategy* strategy,
    shared_ptr<MatchSearchState> searchState,
    ExtensionInfo* extensionInfo,
    MatchSearchState::PositionIndex searchPosition,
    StrategyStackManager::StackId pending,
    VariableBindingsManager::ContextId varBinds,
    StrategicExecution* sibling,
    StrategicProcess* insertionPoint
  );

  int onCommitState(int dagNode, StrategyStackManager::StackId stackId,
		    StrategicExecution* taskSibling,
		    const StrategyTransitionGraph::Transition& transition) override;

private:
  //
  //	Virtual in StrategicTask.
  //
  Survival executionSucceeded(int resultIndex, StrategicProcess* insertionPoint) override; // not used
  Survival executionsExhausted(StrategicProcess* insertionPoint) override;

  static bool zeroVector(const vector<int> &v);

  class ChildTask;		// Nested child task to get in charge of a (Pm, Em) rewriting
  class InterleaverProcess;

  using StateMap = std::map<std::vector<int>, std::pair<int, bool>>;
  StateMap stateMap;
  int initialStateDag;

  Vector<ChildTask*> childTasks;
  bool solutionReported;

  size_t currentSubterm;
};

#endif
