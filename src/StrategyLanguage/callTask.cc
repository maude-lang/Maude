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
//      Implementation for CallTask.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"

//	strategy language class definitions
#include "callTask.hh"
#include "decompositionProcess.hh"
#include "strategicSearch.hh"

CallTask::CallTask(StrategicSearch& searchObject,
		   int startIndex,
		   StrategyExpression* strategy,
		   StrategyStackManager::StackId pending,
		   VariableBindingsManager::ContextId varBinds,
		   StrategicExecution* sibling,
		   StrategicProcess* insertionPoint)
  : StrategicTask(sibling, varBinds),
    searchObject(searchObject),
    pending(pending)
{
  (void) new DecompositionProcess(startIndex,
				  searchObject.push(StrategyStackManager::EMPTY_STACK, strategy),
				  getDummyExecution(),
				  insertionPoint);
}

CallTask::~CallTask()
{
  searchObject.closeContext(getVarsContext());
}

StrategicExecution::Survival
CallTask::executionSucceeded(int resultIndex, StrategicProcess* insertionPoint)
{
  (void) new DecompositionProcess(resultIndex,
				  pending,		// Recovers the original strategy stack
				  this,			// Will be slave of the parent task (closes context)
  insertionPoint);

  return SURVIVE;
}

StrategicExecution::Survival
CallTask::executionsExhausted(StrategicProcess*)
{
  return DIE;
}
