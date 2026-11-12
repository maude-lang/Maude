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
//      Implementation for abstract class StrategicTask.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"

//	higher class definitions
#include "strategyTransitionGraph.hh"

//	strategy language class definitions
#include "branchTask.hh"
#include "decompositionProcess.hh"

BranchTask::BranchTask(StrategyStackManager& strategyStackManager,
		       StrategicExecution* sibling,
		       int startIndex,
		       StrategyExpression* initialStrategy,
		       BranchStrategy::Action successAction,
		       StrategyExpression* successStrategy,
		       BranchStrategy::Action failureAction,
		       StrategyExpression* failureStrategy,
		       StrategyStackManager::StackId pending,
		       StrategyStackManager::StackId iterationCheckpoint,
		       StrategicProcess* insertionPoint)
  : StrategicTask(sibling),
    strategyStackManager(strategyStackManager),
    startIndex(startIndex),
    iterationCheckpoint(iterationCheckpoint),
    initialStrategy(initialStrategy),
    successAction(successAction),
    successStrategy(successStrategy),
    failureAction(failureAction),
    failureStrategy(failureStrategy)
{
  success = false;
  (void) new DecompositionProcess(startIndex,
				  strategyStackManager.push(StrategyStackManager::EMPTY_STACK, initialStrategy),
				  getDummyExecution(),
				  insertionPoint);

  StrategicTask::pending = pending;

  // Strategies not and test are opaque for the model checker
  if (successAction == BranchStrategy::FAIL || successAction == BranchStrategy::IDLE)
    setTransitionGraph(nullptr);
}

StrategicExecution::Survival
BranchTask::executionSucceeded(int resultIndex, StrategicProcess* insertionPoint)
{
  success = true;
  switch (successAction)
    {
    case BranchStrategy::FAIL:
      return DIE;
    case BranchStrategy::IDLE:
      {
	resumeOwner(startIndex, pending, insertionPoint);
	return DIE;
      }
    case BranchStrategy::PASS_THRU:
      {
	resumeOwner(resultIndex, pending, insertionPoint);
	break;
      }
    case BranchStrategy::NEW_STRATEGY:
      {
	//
	//	Start a new process that applies the success strategy followed by the pending
	//	strategies to the result. It will report to our owner.
	//
	StrategyStackManager::StackId newPending = strategyStackManager.push(pending, successStrategy);
	resumeOwner(resultIndex, newPending, insertionPoint);
	break;
      }
    case BranchStrategy::ITERATE:
      {
	//
	//	Do not iterate if we have already iterated from this term. We check this
	//	by looking for the stack position of e ! in the seen set of the parent task.
	//
	StrategyTransitionGraph* transitionGraph = getTransitionGraph();
	if (transitionGraph == nullptr ? getOwner()->alreadySeen(resultIndex, iterationCheckpoint)
	    // onCheckpoint may create a substate in the current branch task for the
	    // iterationCheckpoint instead of in the new branch
	    : !transitionGraph->onCheckpoint(resultIndex, this, iterationCheckpoint, insertionPoint))
	  return SURVIVE;
	//
	//	We set up another branch task on the new result and we stay alive to
	//	process any new results.
	//
	(void) new BranchTask(strategyStackManager,
				this,
				resultIndex,
				initialStrategy,
				successAction,
				successStrategy,
				failureAction,
				failureStrategy,
				pending,
				iterationCheckpoint,
				insertionPoint);
	  break;
      }
    default:
      CantHappen("bad success action");
    }
  return SURVIVE;
}

StrategicExecution::Survival
BranchTask::executionsExhausted(StrategicProcess* insertionPoint)
{
  if (!success)
    {
      //
      //	We didn't have any successes with initial strategy from the original term.
      //
      switch (failureAction)
	{
	case BranchStrategy::FAIL:
	  break;
	case BranchStrategy::IDLE:
	  {
	    resumeOwner(startIndex, pending, insertionPoint);
	    break;
	  }
	case BranchStrategy::NEW_STRATEGY:
	  {
	    //
	    //	Start a new process that applies the failure strategy followed by the pending
	    //	strategies to the original term. It will report to our owner.
	    //
	    StrategyStackManager::StackId newPending = strategyStackManager.push(pending, failureStrategy);
	    resumeOwner(startIndex, newPending, insertionPoint);
	    break;
	  }
	default:
	  CantHappen("bad failure action");
	}
    }
  //
  //	We don't have any more slave executions so we can pack up and go home.
  //
  return DIE;
}
