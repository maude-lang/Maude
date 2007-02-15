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
//      Implementation for abstract class Strategy.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"

//	interface class definitions
#include "term.hh"

//	strategy language class definitions
#include "strategicSearch.hh"
#include "applicationStrategy.hh"
#include "applicationProcess.hh"
#include "decompositionProcess.hh"

ApplicationStrategy::ApplicationStrategy(int label,
					 const Vector<Term*>& variables,
					 const Vector<Term*>& values,
					 const Vector<StrategyExpression*>& strategies)
  : label(label),
    variables(variables),
    valueDags(values.size()),
    strategies(strategies)
{
  Assert(label != NONE || (variables.empty() && strategies.empty()),
	 "substitutions and condition strategies aren't allowed without a label");
  top = false;
  int nrValues = values.size();
  Assert(variables.size() == nrValues, "bad substitution");
  for (int i = 0; i < nrValues; ++i)
    {
      valueDags[i].setTerm(values[i]);
      valueDags[i].normalize();
      valueDags[i].prepare();
    }
}

ApplicationStrategy::~ApplicationStrategy()
{
  int nrVariables = variables.size();
  for (int i = 0; i < nrVariables; ++i)
    variables[i]->deepSelfDestruct();
  int nrStrategies = strategies.size();
  for (int i = 0; i < nrStrategies; ++i)
    delete strategies[i];
}

StrategicExecution::Survival
ApplicationStrategy::decompose(StrategicSearch& searchObject, DecompositionProcess* remainder)
{
  (void) new ApplicationProcess(searchObject,
				remainder->getDag(),
				this,
				remainder->getPending(),
				remainder, // working for same task
				remainder);  // place in process queue ahead of old process
  return StrategicExecution::DIE;  // request deletion of DecompositionProcess
}
