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
//	Class for executing the matchrew subterm rewriting.
//
#ifndef _subtermTask_hh_
#define _subtermTask_hh_

#include <vector>

//	core class definitions
#include "dagNode.hh"

//	higher class definitions
#include "pattern.hh"
#include "matchSearchState.hh"

//	strategy language definitions
#include "sharedValue.hh"
#include "strategyLanguage.hh"
#include "strategicTask.hh"
#include "variableBindingsManager.hh"

class SubtermTask : public StrategicTask
{
  NO_COPYING(SubtermTask);

public:
  //
  // Creates a task for the matchrew strategy execution.
  //
  // @param searchObject The global search object.
  // @param strategy Matchrew strategy in execution.
  // @param searchState Main pattern match state.
  // @param extensionInfo Extension information, whenever the match is with extension.
  // @param searchPosition This match position in the (reusable) searchState.
  // @param pending Stack of pending strategies.
  // @param sibling Task's sibling.
  // @param varBinds Variable bindings.
  // @param insertionPoint Insertion point in the list of processes.
  //
  SubtermTask(StrategicSearch& searchObject,
	      SubtermStrategy* strategy,
	      SharedValue<MatchSearchState> searchState,
	      ExtensionInfo* extensionInfo,
	      MatchSearchState::PositionIndex searchPosition,
	      StrategyStackManager::StackId pending,
	      VariableBindingsManager::ContextId varBinds,
	      StrategicExecution* sibling,
	      StrategicProcess* insertionPoint);

  ~SubtermTask();

private:
  //
  //	Virtual in StrategicTask.
  //
  Survival executionSucceeded(int resultIndex, StrategicProcess* insertionPoint); // used in a non-standard way
  Survival executionsExhausted(StrategicProcess* insertionPoint);

  // Builds a solution with the given results and pushes it (as a new descomposition process)
  void pushSolution(const Vector<DagNode*>& rewNodes, StrategicProcess* insertionPoint);

  StrategicSearch& searchObject;
  SubtermStrategy* strategy;

  // The following 3 contain the information about where the main pattern has been found
  // (and allow to replace the main pattern match with the result)
  SharedValue<MatchSearchState> searchState;
  ExtensionInfo* extensionInfo;
  MatchSearchState::PositionIndex searchIndex;

  // Store of the results of rewriting the subterms according to their strategies
  // (the first coordinate selects the subpattern, the second the result)
  std::vector<std::vector<DagNode*> > results;		// TODO Change to Vector?

  // Number of empty children (without solutions yet)
  // (it is the count of results[i] s.t. result[i].empty())
  size_t emptyChildren;

  StrategyStackManager::StackId pending;

  class ChildTask;		// Nested child task to get in charge of a (Pm, Em) rewriting
  class RemainingProcess;	// Nested child process to generate subterm solution combinations
};

#endif
