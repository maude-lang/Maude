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
//      Implementation for class FullSubtermTask.
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

//	higher class definitions
#include "strategyTransitionGraph.hh"

//	strategy language class definitions
#include "fullSubtermTask.hh"
#include "strategicSearch.hh"
#include "subtermStrategy.hh"
#include "decompositionProcess.hh"

using Survival = StrategicExecution::Survival;

//
// Child task definition and implementation.
//
// Child task are in charge of rewriting a specific
// subterm.
//

class FullSubtermTask::ChildTask : public StrategicTask
{
  NO_COPYING(ChildTask);

  // Parent matchrew task
  FullSubtermTask* parent;
  // The subterm of the matchrew this task is rewritting
  size_t subtermIndex;
  // Subgraph identifier
  int subgraphId;
  // Current node being whose processes are being run
  size_t currentNode;

public:
  ChildTask(FullSubtermTask* parent, size_t index, int dagIndex, int pending)
  : StrategicTask(parent->getDummyExecution()),
    parent(parent), subtermIndex(index),
    currentNode(0)
  {
    // When a state is commited reports must arrive to the parent
    setEnclosingSubtermTask(nullptr);
    subgraphId = getTransitionGraph()->newSubgraph(dagIndex, pending, getDummyExecution());
  }

  int selectSubgraph();
  void closeSubgraph();

  //
  //	Virtual in StrategicTask.
  //
  Survival executionSucceeded(int, StrategicProcess*) override;
  Survival executionsExhausted(StrategicProcess*) override;
};

int FullSubtermTask::ChildTask::selectSubgraph()
{
  return parent->getTransitionGraph()->selectSubgraph(subgraphId);
}

void FullSubtermTask::ChildTask::closeSubgraph()
{
  parent->getTransitionGraph()->closeSubgraph(subgraphId);
}

Survival FullSubtermTask::ChildTask::executionsExhausted(StrategicProcess*)
{
  return StrategicExecution::SURVIVE;
}

Survival FullSubtermTask::ChildTask::executionSucceeded(int, StrategicProcess*)
{
  parent->solutionReported = true;
  return StrategicExecution::SURVIVE;
}

//
//	Interleaver process declaration and implementation
//

class FullSubtermTask::InterleaverProcess : public StrategicProcess
{
  NO_COPYING(InterleaverProcess);

  FullSubtermTask* parent;
  // std::vector is used instead of Vector because it implements lexicographic comparison
  vector<int> subgraphStates;
  size_t subtermIndex;
  size_t childIndex;

public:
  InterleaverProcess(FullSubtermTask* parent,
		     size_t subtermIndex,
		     StrategicProcess* insertionPoint);
  InterleaverProcess(const InterleaverProcess &base,
		     size_t subtermIndex,
		     StrategicProcess* insertionPoint);
  InterleaverProcess(const InterleaverProcess &base,
		     size_t subtermIndex,
		     int childIndex,
		     StrategicProcess* insertionPoint);	// HACK

  Survival run(StrategicSearch& searchObject) override;
};

FullSubtermTask::InterleaverProcess::InterleaverProcess(FullSubtermTask* parent,
							size_t subtermIndex,
							StrategicProcess* insertionPoint)
 : StrategicProcess(parent->getDummyExecution(), insertionPoint),
   parent(parent),
   subgraphStates(parent->childTasks.size()),
   subtermIndex(subtermIndex),
   childIndex(0)
{
  for (int& state : subgraphStates)
    state = 0;
}

FullSubtermTask::InterleaverProcess::InterleaverProcess(const InterleaverProcess &base,
							size_t subtermIndex,
							StrategicProcess* insertionPoint)
  : StrategicProcess(base.parent->getDummyExecution(), insertionPoint),
    parent(base.parent),
    subgraphStates(base.subgraphStates),
    subtermIndex(subtermIndex),
    childIndex(0)
{
}

FullSubtermTask::InterleaverProcess::InterleaverProcess(const InterleaverProcess &base,
							size_t subtermIndex,
							int childIndex,
							StrategicProcess* insertionPoint)
  : StrategicProcess(base.parent->getDummyExecution(), insertionPoint),
    parent(base.parent),
    subgraphStates(base.subgraphStates),
    subtermIndex(subtermIndex),
    childIndex(childIndex)
{
}

Survival FullSubtermTask::InterleaverProcess::run(StrategicSearch& searchObject)
{
  StrategyTransitionGraph* graph = parent->getTransitionGraph();
  const size_t nrSubterms = parent->childTasks.size();

  // We use subtermIndex = -1 to decompose solutions. The reason not to do it
  // directly is to ensure that they are processed in the correct state and
  // that the 'blank' assumptions in onCheckpoint are respected.
  if (subtermIndex == size_t(-1))
    {
      parent->resumeOwner(childIndex, parent->pending, this);
      finished(this);
      return StrategicExecution::DIE;
    }

  int originalState = subgraphStates[subtermIndex];
  bool initialState = zeroVector(subgraphStates);

  // This process adding transitions from the subtermIndex subterm
  ChildTask* childTask = parent->childTasks[subtermIndex];

  // Obtains the next state for the subtermIndex subterm
  int originalGraph = childTask->selectSubgraph();
  parent->solutionReported = false;
  int nextState = graph->getNextState(originalState, childIndex);

  // Marks the state in the subgraph as solution if it has been reached
  if (!graph->isSolutionState(originalState) && parent->solutionReported)
    graph->markSolution(originalState);

  // Checks whether the current composed state is a solution
  StateMap::iterator stateIt = parent->stateMap.find(subgraphStates);

  if (graph->isSolutionState(originalState) &&
      (initialState || !stateIt->second.second))
    {
      bool isSolution = true;
      for (size_t i = 0; i < nrSubterms && isSolution; ++i)
	if (i != subtermIndex)
	  {
	     parent->childTasks[i]->selectSubgraph();
	     isSolution = graph->isSolutionState(subgraphStates[i]);
	  }
      if (isSolution)
	{
	  graph->selectSubgraph(originalGraph);
	  (void) new InterleaverProcess(*this, size_t(-1),
		       initialState ? parent->initialStateDag :
			 graph->getStatePoint(stateIt->second.first).first, this);
	  stateIt->second.second = true;
	}
      childTask->selectSubgraph();
    }

  // The current state has no more successors, so the process must die
  if (nextState == NONE)
    {
	graph->selectSubgraph(originalGraph);
	finished(this);
	return StrategicExecution::DIE;
    }

  // Recovers the transition and the next state information
  const StrategyTransitionGraph::Transition &transition =
	*graph->getStateFwdArcs(originalState).find(nextState)->second.begin();

  // This function returns the (dag node index, stack id) pair
  pair<int,int> stateInfo = graph->getStatePoint(nextState);

  // Temporarily changes the state we are rewritting by its successor
  subgraphStates[subtermIndex] = nextState;

  // Looks for the combination of substates to detect if it has been already explored
  StateMap::const_iterator it = parent->stateMap.find(subgraphStates);

  // In the positive case, the current state is linked with that state
  if (it != parent->stateMap.end())
    {
      graph->selectSubgraph(originalGraph);
      graph->linkState(it->second.first, transition);
    }

  // Otherwise, a new state is created, but without calling commitState
  else
    {
      // The subterms vector is filled with the rewritten subterms at the
      // current positions...
      bool isSolution = true;
      for (size_t i = 0; i < nrSubterms; i++)
	{
	  parent->childTasks[i]->selectSubgraph();
	  parent->subterms[i] = graph->getStateDag(subgraphStates[i]);

	  // Also checks if the current combination is a solution
	  if (isSolution && !graph->isSolutionState(subgraphStates[i]))
	    isSolution = false;
	}
      // ...to rebuild the whole term
      int completeDagNode = parent->rebuild(parent->subterms);

      // Sets the StrategyTransitionGraph to its original state
      graph->selectSubgraph(originalGraph);

      // Interleaver processes will be created to extend from here
      StrategicProcess* firstProcess = new InterleaverProcess(*this, 0, nullptr);

      // Creates a new state
      int newStateNr = graph->newState(stateInfo.first,
				       completeDagNode,
				       stateInfo.second,
				       firstProcess,
				       transition);

      // Maps the current subterm combination to the new state
      parent->stateMap[subgraphStates] = make_pair(newStateNr, isSolution);

      // Creates interleaver processes for all other subterm
      for (size_t i = 1; i < nrSubterms; i++)
	(void) new InterleaverProcess(*this, i, firstProcess);

      // If it is a solution, creates a decomposition process to
      // exit the matchrew
      if (isSolution)
	(void) new InterleaverProcess(*this, size_t(-1), completeDagNode, firstProcess);
    }

  // Recovers the initial positions in the rewriting tree
  subgraphStates[subtermIndex] = originalState;
  // The next children will be generated in the next turn
  childIndex++;

  return StrategicExecution::SURVIVE;
}


//
//	FullSubtermTask implementation
//

FullSubtermTask::FullSubtermTask(StrategicSearch &searchObject,
	SubtermStrategy* strategy,
	shared_ptr<MatchSearchState> searchState,
	ExtensionInfo* extensionInfo,
	MatchSearchState::PositionIndex searchPosition,
	StrategyStackManager::StackId pending,
	VariableBindingsManager::ContextId varBinds,
	StrategicExecution* sibling,
	StrategicProcess* insertionPoint
)
  : ModelCheckerSubtermTask(searchObject, strategy,
			    searchState, extensionInfo, searchPosition,
			    pending, varBinds, sibling),
    childTasks(subterms.size())
{
  const size_t nrSubterms = subterms.size();
  const Vector<StrategyExpression*> &strategies = strategy->getStrategies();

  // For each subterm, creates a child task and an interleaver process
  for (size_t subtermIndex = 0; subtermIndex < nrSubterms; subtermIndex++)
    {
      childTasks[subtermIndex] = new ChildTask(this,
	subtermIndex,
	searchObject.insert(subterms[subtermIndex]),
	searchObject.push(StrategyStackManager::EMPTY_STACK, strategies[subtermIndex]));

      (void) new InterleaverProcess(this, subtermIndex, insertionPoint);
    }

  initialStateDag = searchObject.insert(searchState->getDagNode());
}

Survival FullSubtermTask::executionSucceeded(int, StrategicProcess*)
{
  CantHappen("FullSubtermTask::executionSuceeded must not be called");
  return StrategicExecution::DIE;	// just to avoid warnings
}

StrategicExecution::Survival FullSubtermTask::executionsExhausted(StrategicProcess* insertionPoint)
{
  for (ChildTask* task : childTasks)
    task->closeSubgraph();
  return StrategicExecution::DIE;
}

int FullSubtermTask::onCommitState(int dagIndex,
				   StrategyStackManager::StackId stackId,
				   StrategicExecution* taskSibling,
				   const StrategyTransitionGraph::Transition& transition)
{
  CantHappen("onCommitState called for FullSubtermTask");
  return NONE;
}

bool FullSubtermTask::zeroVector(const vector<int>& v)
{
  for (int value : v)
    if (value != 0)
      return false;
  return true;
}
