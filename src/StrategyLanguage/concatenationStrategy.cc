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
//      Implementation for abstract class Strategy.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"

//	strategy language class definitions
#include "concatenationStrategy.hh"
#include "decompositionProcess.hh"
#include "strategicSearch.hh"

ConcatenationStrategy::ConcatenationStrategy(const Vector<StrategyExpression*>& strategies)
  : strategies(strategies)
{
  Assert(!strategies.empty(), "no strategies");
}

ConcatenationStrategy::~ConcatenationStrategy()
{
  int nrStrategies = strategies.size();
  for (int i = 0; i < nrStrategies; ++i)
    delete strategies[i];
}

bool
ConcatenationStrategy::check(VariableInfo& indices, const TermSet& boundVars)
{
  int nrStrategies = strategies.length();
  for (int i = 0; i < nrStrategies; i++)
    if (!strategies[i]->check(indices, boundVars))
      return false;

  return true;
}

void
ConcatenationStrategy::process()
{
  int nrStrategies = strategies.length();
  for (int i = 0; i < nrStrategies; i++)
    strategies[i]->process();
}

StrategicExecution::Survival
ConcatenationStrategy::decompose(StrategicSearch& searchObject, DecompositionProcess* remainder)
{
  for (int i = strategies.size() - 1; i >= 0; --i)
    remainder->pushStrategy(searchObject, strategies[i]);
  return StrategicExecution::SURVIVE;  // remainder should not request deletion
}
