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
//      Implementation for class IterationStrategy.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"

//	strategy language class definitions
#include "iterationStrategy.hh"
#include "decompositionProcess.hh"
#include "strategicSearch.hh"

IterationStrategy::IterationStrategy(StrategyExpression* child, bool zeroAllowed)
  : child(child),
    star(zeroAllowed ? 0 : new IterationStrategy(child, true))
{
}

IterationStrategy::~IterationStrategy()
{
  delete (star != 0 ? star : child);
}

bool
IterationStrategy::check(VariableInfo& indices, const TermSet& boundVars)
{
  return child->check(indices, boundVars);
}

void
IterationStrategy::process()
{
  child->process();
}

StrategicExecution::Survival
IterationStrategy::decompose(StrategicSearch& searchObject, DecompositionProcess* remainder)
{
  if (star)  // + case; push * subexpression
    remainder->pushStrategy(searchObject, star);
  else  // * case
    {
      (void) new DecompositionProcess(remainder);  // idle alternative
      remainder->pushStrategy(searchObject, this);
    }
  remainder->pushStrategy(searchObject, child);
  return StrategicExecution::SURVIVE;  // remainder should not request deletion
}
