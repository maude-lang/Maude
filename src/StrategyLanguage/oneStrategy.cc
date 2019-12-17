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
//      Implementation for the one strategy.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"

//	strategy language class definitions
#include "oneStrategy.hh"
#include "oneTask.hh"
#include "decompositionProcess.hh"

OneStrategy::OneStrategy(StrategyExpression* strat)
  : strategy(strat)
{
}

OneStrategy::~OneStrategy()
{
  delete strategy;
}

bool
OneStrategy::check(VariableInfo& indices, const TermSet& boundVars)
{
  return strategy->check(indices, boundVars);
}

void
OneStrategy::process()
{
  strategy->process();
}

StrategicExecution::Survival
OneStrategy::decompose(StrategicSearch& searchObject, DecompositionProcess* remainder)
{
  (void) new OneTask(searchObject,
		     remainder->getDagIndex(),
		     strategy,
		     remainder->getPending(),
		     remainder,
		     remainder);

  return StrategicExecution::DIE;
}
