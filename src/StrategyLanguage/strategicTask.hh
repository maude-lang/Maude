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
//	Abstract base class for event triggered strategic execution.
//
#ifndef _stategicTask_hh_
#define _stategicTask_hh_
#include <set>
#include "strategicExecution.hh"
#include "strategyStackManager.hh"
#include "variableBindingsManager.hh"
#include "higher.hh"

class StrategicTask : public StrategicExecution
{
  NO_COPYING(StrategicTask);

public:
  StrategicTask(StrategicTask* master);
  StrategicTask(StrategicExecution* sibling);
  StrategicTask(StrategicExecution* sibling,
		VariableBindingsManager::ContextId ctx);
  ~StrategicTask();
  //
  //	Call-backs for interesting events.
  //
  virtual Survival executionSucceeded(int resultIndex, StrategicProcess* insertionPoint) = 0;
  virtual Survival executionsExhausted(StrategicProcess* insertionPoint) = 0;

  bool alreadySeen(int dagIndex, StrategyStackManager::StackId stackId);

  //
  // Tasks represent executions contexts, and so they are linked
  // to both a variable context and a continuation.
  //
  VariableBindingsManager::ContextId getVarsContext() const;
  StrategyStackManager::StackId getContinuation() const;

  //
  // Model checking stuff
  //

  // The strategy transition graph. It is the connection point between
  // the execution infrastructure and the model checker. This pointer
  // will be null in normal execution or when model checking is disabled.
  StrategyTransitionGraph* getTransitionGraph() const;
  // Gets the enclosing SubtermTask (if any) for DAG node reconstruction.
  ModelCheckerSubtermTask* getEnclosingSubtermTask() const;
  // Gets the task information associated to the task
  TaskInfo* getTaskInfo() const;

  // Transition graphs and enclosing tasks are inherited by default
  void setTransitionGraph(StrategyTransitionGraph* stg);
  void setEnclosingSubtermTask(ModelCheckerSubtermTask* subterm);
  void setTaskInfo(TaskInfo* taskInfo);

protected:
  StrategicExecution* getDummyExecution();
  StrategyStackManager::StackId pending;

  // Convenient functions to reduce the model checker interference
  void resumeOwner(int dagNode, int pending, StrategicProcess* insertionPoint);

private:
  //
  //	A state is an index to a dag and a stack identifier.
  //
  typedef pair<int, StrategyStackManager::StackId> State;
  typedef set<State> SeenSet;
  //
  //	To simplify coding, the head and tail of our list of slaves
  //	(processes and tasks working for us) is stored as a dummy
  //	execution, essentially forming a circular list.
  //
  StrategicExecution slaveList;
  SeenSet seenSet;

  //
  // Variable context
  //
  VariableBindingsManager::ContextId varsContext;

  //
  // Model checking stuff
  //
  StrategyTransitionGraph* transitionGraph;
  ModelCheckerSubtermTask* enclosingSubtermTask;
  TaskInfo* taskInfo;
};

inline StrategicExecution*
StrategicTask::getDummyExecution()
{
  return &slaveList;
}

inline VariableBindingsManager::ContextId
StrategicTask::getVarsContext() const
{
  return varsContext;
}

inline StrategyTransitionGraph*
StrategicTask::getTransitionGraph() const
{
  return transitionGraph;
}

inline void
StrategicTask::setTransitionGraph(StrategyTransitionGraph* stg)
{
  transitionGraph = stg;
}

inline ModelCheckerSubtermTask*
StrategicTask::getEnclosingSubtermTask() const
{
  return enclosingSubtermTask;
}

inline void
StrategicTask::setEnclosingSubtermTask(ModelCheckerSubtermTask* stsk)
{
  enclosingSubtermTask = stsk;
}

inline StrategyStackManager::StackId
StrategicTask::getContinuation() const
{
  return pending;
}

inline TaskInfo*
StrategicTask::getTaskInfo() const
{
  return taskInfo;
}

inline void
StrategicTask::setTaskInfo(TaskInfo* givenTaskInfo)
{
  Assert(taskInfo == nullptr, "overriding task information");
  taskInfo = givenTaskInfo;
}

#endif
