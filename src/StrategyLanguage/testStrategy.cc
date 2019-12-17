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
//      Implementation for class TestStrategy.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "strategyLanguage.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	higher class definitions
#include "matchSearchState.hh"

//	strategy language class definitions
#include "decompositionProcess.hh"
#include "strategicSearch.hh"
#include "testStrategy.hh"

TestStrategy::TestStrategy(Term* patternTerm, int depth, const Vector<ConditionFragment*>& condition)
  : pattern(patternTerm, depth >= 0, condition, true),
    depth(depth)
{
  // need to do something about recovery
}

bool
TestStrategy::check(VariableInfo& indices, const TermSet& boundVars)
{
  // Check that the unbound variables are defined in the context and
  // builds a translation map between context indices and pattern indices
  const NatSet& unboundSet = pattern.getUnboundVariables();
  size_t nrVars = pattern.getNrRealVariables();

  indexTranslation.resize(0);

  for (size_t k = 0; k < nrVars; k++)
    {
      Term* var = pattern.index2Variable(k);

      // var is not bound outside the pattern
      if (boundVars.term2Index(var) == NONE)
	{
	  if (unboundSet.contains(k))
	    {
	      IssueWarning(*pattern.getLhs() << ": variable " << QUOTE(var) <<
		" is used before it is bound in condition of test strategy.");

	      return false;
	    }
	}
      else
	{
	  int outerIndex = indices.variable2Index(static_cast<VariableTerm*>(var));
	  indexTranslation.append(make_pair(k, outerIndex));
	}
    }
  return true;
}

void
TestStrategy::process()
{
  pattern.prepare();
}

StrategicExecution::Survival
TestStrategy::decompose(StrategicSearch& searchObject, DecompositionProcess* remainder)
{
  RewritingContext* context = searchObject.getContext();
  RewritingContext* newContext = context->makeSubcontext(searchObject.getCanonical(remainder->getDagIndex()));
  MatchSearchState* state = new MatchSearchState(newContext, &pattern, MatchSearchState::GC_CONTEXT |
						 MatchSearchState::GC_SUBSTITUTION, 0, depth);

  // Applies the variable bindings
  if (!indexTranslation.isNull())
    {
      VariableBindingsManager::ContextId varBinds = remainder->getOwner()->getVarsContext();
      Vector<Term*> vars;
      Vector<DagRoot*> values;
      searchObject.buildInitialSubstitution(varBinds, pattern, indexTranslation, vars, values);
      state->setInitialSubstitution(vars, values);
    }

  bool result = state->findNextMatch();
  state->transferCountTo(*context);
  delete state;
  return result ? StrategicExecution::SURVIVE : StrategicExecution::DIE;
}
