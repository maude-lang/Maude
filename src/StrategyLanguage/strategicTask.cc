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
#include "strategicTask.hh"
#include "decompositionProcess.hh"

StrategicTask::StrategicTask(StrategicTask* master)
  : StrategicExecution(master),
    pending(StrategyStackManager::EMPTY_STACK),
    slaveList(this),
    varsContext(master ? master->getVarsContext()
		       : VariableBindingsManager::EMPTY_CONTEXT),
    transitionGraph(master ? master->transitionGraph : nullptr),
    enclosingSubtermTask(master ? master->enclosingSubtermTask : nullptr),
    taskInfo(nullptr)
{
}

StrategicTask::StrategicTask(StrategicExecution* sibling)
  : StrategicExecution(sibling),
    pending(StrategyStackManager::EMPTY_STACK),
    slaveList(this),
    varsContext(sibling->getOwner()->getVarsContext()),
    transitionGraph(sibling->getOwner()->transitionGraph),
    enclosingSubtermTask(sibling->getOwner()->enclosingSubtermTask),
    taskInfo(nullptr)
{
}


StrategicTask::StrategicTask(StrategicExecution* sibling, VariableBindingsManager::ContextId ctx)
  : StrategicExecution(sibling),
    pending(StrategyStackManager::EMPTY_STACK),
    slaveList(this),
    varsContext(ctx),
    transitionGraph(sibling->getOwner()->transitionGraph),
    enclosingSubtermTask(sibling->getOwner()->enclosingSubtermTask),
    taskInfo(nullptr)
{
}

StrategicTask::~StrategicTask()
{
  //
  //	Once we disappear, our slaves will have no place to report to, so
  //	we must delete them first.
  //
  StrategicExecution* next;
  for (StrategicExecution* i = slaveList.getNextSlave(); i != &slaveList; i = next)
    {
      next = i->getNextSlave();
      delete i;
    }

  if (taskInfo != nullptr)
    deleteTaskInfo(taskInfo);
}

void StrategicTask::resumeOwner(int dagNode, int pending, StrategicProcess* insertionPoint)
{
  // Resume the strategy execution in the owner task.
  // When model checking, some extra work is done and perhaps resuming is cancelled.
  if (transitionGraph == nullptr || transitionGraph->onCheckpoint(dagNode, this, pending, insertionPoint))
    (void) new DecompositionProcess(dagNode, pending, this, insertionPoint);
}

/*
bool
StrategicTask::alreadySeen(int dagIndex, StrategyStackManager::StackId stackId)
{
  State s(dagIndex, stackId);
  return !(seenSet.insert(s).second);
}
*/

bool
StrategicTask::alreadySeen(int dagIndex, StrategyStackManager::StackId stackId)
{
  DebugAdvisory("alreadySeen( " << dagIndex <<
		", " << stackId <<
		") - seenSet.size() " << seenSet.size());
  State s(dagIndex, stackId);
  pair<SeenSet::iterator, bool> p = seenSet.insert(s);  // to avoid UMR in purify
  return !(p.second);
}
