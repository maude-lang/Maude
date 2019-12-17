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
  : top(false),
    label(label),
    variables(variables),
    valueDags(values.size()),
    strategies(strategies),
    subsDagsAreReduced(false)
{
  Assert(label != NONE || (variables.empty() && strategies.empty()),
	 "substitutions and condition strategies aren't allowed without a label");

  size_t nrValues = values.size();
  Assert(variables.size() == nrValues, "bad substitution");

  for (size_t i = 0; i < nrValues; ++i)
    valueDags[i].setTerm(values[i]);
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

bool
ApplicationStrategy::check(VariableInfo& indices, const TermSet& boundVars)
{
  // Index and check variables in the substitution

  int subsSize = valueDags.length();
  for (int i = 0; i < subsSize; i++)
    {
      valueDags[i].getTerm()->indexVariables(indices);
      valueDags[i].normalize();

      const NatSet& occurSet = valueDags[i].getTerm()->occursBelow();

      // NOTE Free variables can be used in substitutions?
      // It may have some sense.

      for (int index : occurSet)
        {
	  Term* var = indices.index2Variable(index);

	  if (boundVars.term2Index(var) == NONE)
            {
	      IssueWarning(*var << ": unbound variable " << QUOTE(var) <<
			   " in application strategy substitution for " <<
			   QUOTE(variables[i]) << ".");
	      return false;
            }
        }
    }

  // Index and check variables in the rewriting condition strategies

  size_t nrStrategies = strategies.length();
  for (size_t i = 0; i < nrStrategies; i++)
    if (!strategies[i]->check(indices, boundVars))
      return false;

  return true;
}

void
ApplicationStrategy::process()
{
  // Fills in sort info for variables, and prepare values
  int subsSize = variables.length();

  for (int i = 0; i < subsSize; i++)
    {
      valueDags[i].prepare();
      variables[i]->symbol()->fillInSortInfo(variables[i]);
      valueDags[i].getDag()->computeBaseSortForGroundSubterms(false);
    }

  // And follows recusively
  int nrStrategies = strategies.length();
  for (int i = 0; i < nrStrategies; i++)
    strategies[i]->process();
}

StrategicExecution::Survival
ApplicationStrategy::decompose(StrategicSearch& searchObject, DecompositionProcess* remainder)
{
  (void) new ApplicationProcess(searchObject,
				remainder->getDagIndex(),
				this,
				remainder->getPending(),
				remainder, // working for same task
				remainder);  // place in process queue ahead of old process
  return StrategicExecution::DIE;  // request deletion of DecompositionProcess
}
