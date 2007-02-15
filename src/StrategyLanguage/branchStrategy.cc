/*

    This file is part of the Maude 2 interpreter.

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
//      Implementation for class BranchStrategy.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"

//	strategy language class definitions
#include "branchStrategy.hh"
#include "branchTask.hh"
#include "decompositionProcess.hh"
#include "strategicSearch.hh"

BranchStrategy::BranchStrategy(StrategyExpression* initialStrategy,
			       Action successAction,
			       StrategyExpression* successStrategy,
			       Action failureAction,
			       StrategyExpression* failureStrategy)
  : initialStrategy(initialStrategy),
    successStrategy(successStrategy),
    failureStrategy(failureStrategy),
    successAction(successAction),
    failureAction(failureAction)
{
  Assert(successAction >= FAIL && successAction <= ITERATE, "bad success action");
  Assert((successAction == NEW_STRATEGY) == (successStrategy != 0), "success inconsistancy");
  Assert(failureAction == FAIL || failureAction == IDLE || failureAction == NEW_STRATEGY,
	 "bad failure action");
  Assert((failureAction == NEW_STRATEGY) == (failureStrategy != 0), "failure inconsistancy");
}

BranchStrategy::~BranchStrategy()
{
  delete initialStrategy;
  delete successStrategy;
  delete failureStrategy;
}

StrategicExecution::Survival
BranchStrategy::decompose(StrategicSearch& searchObject, DecompositionProcess* remainder)
{
  (void) new BranchTask(searchObject,
			remainder,
			remainder->getDag(),
			initialStrategy,
			successAction,
			successStrategy,
			failureAction,
			failureStrategy,
			remainder->getPending(),
			remainder);
  return StrategicExecution::DIE;  //  request deletion of DecompositionProcess
}
