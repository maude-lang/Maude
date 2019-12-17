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
  // Each task has a variable bindings context associated
  //
  VariableBindingsManager::ContextId getVarsContext() const;

protected:
  StrategicExecution* getDummyExecution();

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

  VariableBindingsManager::ContextId varsContext;
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

#endif
