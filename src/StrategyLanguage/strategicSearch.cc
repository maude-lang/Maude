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
//      Implementation for class StrategicSearch.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"

//      core class definitions
#include "rewritingContext.hh"

//	strategy language class definitions
#include "strategyExpression.hh"
#include "strategicProcess.hh"
#include "strategicSearch.hh"
#include "decompositionProcess.hh"

StrategicSearch::StrategicSearch(RewritingContext* initial, StrategyExpression* strategy)
  : VariableBindingsManager(initial->root()->symbol()->getModule()->getMinimumSubstitutionSize()),
    StrategicTask(static_cast<StrategicTask*>(0)),
    strategy(strategy),
    initial(initial),
    exhausted(false),
    solutionIndex(NONE)
{
  Assert(initial != 0, "null context");
  Assert(initial->root() != 0, "null root");
  Assert(strategy != 0, "null strategy");
}

StrategicSearch::~StrategicSearch()
{
  delete initial;
  delete strategy;
}

StrategicExecution::Survival
StrategicSearch::executionSucceeded(int resultIndex, StrategicProcess* /* insertionPoint */)
{
  Assert(solutionIndex == NONE, "multiple solutions from a single step");
  solutionIndex = resultIndex;
  return SURVIVE;  // still want more solutions
}

StrategicExecution::Survival
StrategicSearch::executionsExhausted(StrategicProcess* /* insertionPoint */)
{
  exhausted = true;
  return SURVIVE;  // root task cannot die in the usual way
}
