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
//      Implementation for class SubtermTask.
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

//	strategy language class definitions
#include "subtermTask.hh"
#include "strategicSearch.hh"
#include "subtermStrategy.hh"
#include "decompositionProcess.hh"

typedef StrategicExecution::Survival Survival;

//
// Child task definition and implementation.
//
// The child tasks are needed to know from which pattern solutions come
// from, because there may be slaves rewritting different subpatterns
// of the matchrew strategy at the same time.
//

class SubtermTask::ChildTask : public StrategicTask
{
  NO_COPYING(ChildTask);

  // Index of the subpattern of matchrew strategy their children are rewriting
  SubtermTask* parent;
  int index;

public:
  ChildTask(SubtermTask* parent, int index)
    : StrategicTask(parent->getDummyExecution()),
      parent(parent),
      index(index) {}

  using StrategicTask::getDummyExecution;

  //
  //	Virtual in StrategicTask.
  //
  Survival executionSucceeded(int, StrategicProcess*);
  Survival executionsExhausted(StrategicProcess*);
};

Survival
SubtermTask::ChildTask::executionsExhausted(StrategicProcess*)
{
  return StrategicExecution::DIE;
}

Survival
SubtermTask::ChildTask::executionSucceeded(int resultIndex, StrategicProcess* insertionPoint)
{
  // Maintains emptyChildren variable to know if a global solution is available
  if (parent->results[index].empty())
    parent->emptyChildren--;

  // Pushes the new subpattern solution into the store
  parent->results[index].push_back(parent->searchObject.getCanonical(resultIndex));

  // matchrew solutions are available
  if (parent->emptyChildren == 0)
    parent->executionSucceeded(index, insertionPoint);

  return StrategicExecution::SURVIVE;
}

//
// Remaining solutions process declaration and implementation.
//
// SubtermTask rewrites all the subpatterns' terms in parallel, as they are
// independent. Consecuently, when a subpattern succeeds, the new solutions are
// the combinations of the new one with the previously known solutions for
// the other subpatterns.
//
// Processes can only provide one solution at a time so we need to create
// another process to generate the combinatorial solutions and pass them to
// our owner.
//

class SubtermTask::RemainingProcess : public StrategicProcess
{
  NO_COPYING(RemainingProcess);

  SubtermTask* parent;
  size_t subpatternIndex;	// Index of the subpattern for which the last solution has been found
  Vector<int> limits;		// Index of the last solutions for each subpattern at
  // the time this process was issued

  Vector<int> indices;		// Index of the currently selected solution for each subpattern
  Vector<DagNode*> rewNodes;	// Currently selected node for each subpattern (maybe unnecessary)

  int i;			// Subpattern index whose solution index to increment next

public:
  RemainingProcess(SubtermTask* parent,
		   size_t lastSlaveIndex,
		   Vector<int> limits,
		   StrategicProcess* insertionPoint);

  Survival run(StrategicSearch& searchObject);
};

SubtermTask::RemainingProcess::RemainingProcess(SubtermTask* parent, size_t spIdx, Vector<int> limits, StrategicProcess* insertionPoint)
  : StrategicProcess(parent->getDummyExecution(), insertionPoint),
    parent(parent),
    subpatternIndex(spIdx),
    limits(limits),
    // If the subpatern index is the last, we can take a shorter vector
    indices(limits.size() == spIdx + 1 ? spIdx : limits.size()),
    rewNodes(limits.size()),
    i(indices.size() - 1)
{
  // Indices initialization to 0
  for (size_t i = 0; i < indices.size(); i++)
    indices[i] = 0;

  // Initialization of the vector of rewritten nodes

  for (size_t i = 0; i < subpatternIndex; i++)
    rewNodes[i] = parent->results[i][0];

  rewNodes[subpatternIndex] = parent->results[subpatternIndex][limits[subpatternIndex]];

  for (size_t i = subpatternIndex + 1; i < rewNodes.size(); i++)
    rewNodes[i] = parent->results[i][0];
}

StrategicExecution::Survival
SubtermTask::RemainingProcess::run(StrategicSearch&)
{
  // Last index (the one to be incremented more often)
  const int last = indices.size() - 1;

  // (Invariant) Indices are always in bound at this point

  parent->pushSolution(rewNodes, this);

  // This code will generate all possible combinations of subterm's solutions
  // An iterative algorithm for this may have indices for each solution source
  // and go over these incrementing them like a counter or an odometer

  // The current index can be incremented
  if (indices[i] < limits[i])
    {
      indices[i]++;
      rewNodes[i] = parent->results[i][indices[i]];

      if (i != last) i = last;	// Let's change the less significant
    }
  else
    {
      indices[i] = 0;
      rewNodes[i] = parent->results[i][0];

      // We jump to most significant positions and
      // over the subpatternIndex's index, which is fixed
      i = (i-1 == (int) subpatternIndex) ? i-2 : i-1;
    }

  // In this case all solution have been visited, we finish
  if (i < 0)
    finished(this);

  return i >= 0 ? SURVIVE : DIE;
}

//
// SubtermTask implementation
//

SubtermTask::SubtermTask(StrategicSearch& searchObject,
			 SubtermStrategy* strategy,
			 shared_ptr<MatchSearchState> searchState,
			 ExtensionInfo* extensionInfo,
			 MatchSearchState::PositionIndex searchPosition,
			 StrategyStackManager::StackId pending,
			 VariableBindingsManager::ContextId varBinds,
			 StrategicExecution* sibling,
			 StrategicProcess* insertionPoint)
  : StrategicTask(sibling, varBinds),
    searchObject(searchObject),
    strategy(strategy),
    searchState(searchState),
    extensionInfo(extensionInfo),
    searchIndex(searchPosition),
    results(strategy->getStrategies().size()),
    emptyChildren(results.size()),
    pending(pending)
{
  Assert(strategy, "null strategy");

  // Uses the substitution from the search to instantiate the subpatterns
  Substitution& subst = *searchState->getContext();

  const Vector<StrategyExpression*>& strategies = strategy->getStrategies();
  const Vector<Term*>& subterms = strategy->getSubterms();

  for (size_t subtermIndex = 0; subtermIndex < results.size(); subtermIndex++)
    {
      //
      // Creates a decomposition process under a child task to apply the corresponding
      // strategy to the selected term
      //

      ChildTask* child = new ChildTask(this, subtermIndex);

      DagNode* subtermVariable = subterms[subtermIndex]->term2Dag(true);
      DagNode* subterm = subtermVariable->instantiate(subst, true);  // passing true for safety

      (void) new DecompositionProcess(searchObject.insert(subterm),
				      searchObject.push(StrategyStackManager::EMPTY_STACK,
							strategies[subtermIndex]),
				      child->getDummyExecution(),
				      insertionPoint);
    }
}

SubtermTask::~SubtermTask()
{
  searchObject.closeContext(getVarsContext());
  delete extensionInfo;
}

void
SubtermTask::pushSolution(const Vector<DagNode*>& rewNodes, StrategicProcess* insertionPoint)
{
  // Rebuils the main pattern from rewritten terms and matched variables
  DagNode* result = strategy->rebuild(searchObject.getValues(getVarsContext()),
				      rewNodes);

  // Rebuilds the node from the pattern match to the root
  result = searchState->rebuildDag(result, extensionInfo, searchIndex).first;

  // Reduces it
  RewritingContext* newContext =
	  searchObject.getContext()->makeSubcontext(result);

  newContext->reduce();
  searchObject.getContext()->transferCountFrom(*newContext);

  // Creates a descomposition process independent of this task
  // to apply the remaining strategy
  int ourResultIndex = searchObject.insert(newContext->root());
  delete newContext;

  (void) new DecompositionProcess(ourResultIndex,
				  pending,
				  this,
				  insertionPoint);
}

Survival
SubtermTask::executionSucceeded(int slaveIndex, StrategicProcess* insertionPoint)
{
  Assert(emptyChildren == 0, "empty results for some subterm rewriting tasks.");

  // Only to optimize this case (can be removed without effect)
  if (results.size() == 1)
    {
      // Rewritten node to be passed to the substitution maker
      Vector<DagNode*> rewNodes(1);
      rewNodes[0] = results[slaveIndex].back();

      pushSolution(rewNodes, insertionPoint);

      return StrategicExecution::SURVIVE;  // still want more solutions if available
    }

  Vector<int> limits(results.size());

  // Saves the current limits for the solution vectors
  for (size_t i = 0; i < limits.size(); i++)
    limits[i] = results[i].size() - 1;

  // General case: when the subpatterns are at least two
  // => all the combinations of the previous results will be generated
  (void) new RemainingProcess(this, slaveIndex, limits, insertionPoint);

  return StrategicExecution::SURVIVE;  // still want more solutions if available
}

Survival
SubtermTask::executionsExhausted(StrategicProcess*)
{
  return StrategicExecution::DIE;  // nothing more to do
}
