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
//      Implementation for the rewriting of subterms strategy.
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

//	variable class definitions
#include "variableDagNode.hh"

//	strategy language class definitions
#include "decompositionProcess.hh"
#include "strategicSearch.hh"
#include "subtermStrategy.hh"
#include "subtermProcess.hh"
#include "termBag.hh"

SubtermStrategy::SubtermStrategy(Term* patternTerm,
				 int depth,
				 const Vector<ConditionFragment*>& condition,
				 const Vector<Term*>& patterns,
				 const Vector<StrategyExpression*>& strategies)
  : mainPattern(patternTerm, depth >= 0, condition, true),
    subpatterns(patterns),
    subtermStrats(strategies),
    depth(depth)
{
  Assert(patterns.size() == strategies.size(),
    "the number of patterns and strategies for the matchrew operator must coincide.");

  // Indexes subpattern variables with mainPattern's indices
  // (will make subterm checking and subpattern instantion easier)
  for (size_t i = 0; i < subpatterns.size(); i++)
    subpatterns[i]->indexVariables(mainPattern);
}

void
SubtermStrategy::process()
{
  size_t nrSubpatterns = subpatterns.size();
  for (size_t i = 0; i < nrSubpatterns; i++)
    {
      subpatterns[i]->normalize(true);
      subpatterns[i]->symbol()->fillInSortInfo(subpatterns[i]);

      subtermStrats[i]->process();
    }

  // Compiles RhsBuilder's automata
  TermBag dummy;

  mainPattern.prepare();
  mainPattern.getLhs()->compileTopRhs(rhsBuilder, mainPattern, dummy);
  mainPattern.computeIndexRemapping();
  rhsBuilder.remapIndices(mainPattern);
}

SubtermStrategy::~SubtermStrategy()
{
  size_t nrSubpatterns = subtermStrats.length();
  for (size_t i = 0; i < nrSubpatterns; i++)
    {
      delete subtermStrats[i];
      subpatterns[i]->deepSelfDestruct();
    }
}

bool
SubtermStrategy::check(VariableInfo& indices, const TermSet& boundVars)
{
  if (!checkSubterms())
    return false;

  // Check that the unbound variables are defined in the context and
  // builds a translation map between context indices and pattern indices
  const NatSet& unboundSet = mainPattern.getUnboundVariables();
  size_t nrVars = mainPattern.getNrProtectedVariables();

  indexTranslation.resize(0);

  for (size_t k = 0; k < nrVars; k++)
    {
      Term* var = mainPattern.index2Variable(k);

      // var is not bound outside the pattern
      if (boundVars.term2Index(var) == NONE)
	{
	  if (unboundSet.contains(k))
	    {
	      IssueWarning(*mainPattern.getLhs() << ": variable " << QUOTE(var) <<
		" is used before it is bound in condition of matchrew strategy.");

	      return false;
	    }
	}
      else
	{
	  int outerIndex = indices.variable2Index(static_cast<VariableTerm*>(var));
	  indexTranslation.append(make_pair(k, outerIndex));
	}
    }

  // Deal with the substrategies. They are a different static context,
  // so we need to produce a context construction vector.
  VariableInfo vinfo;
  TermSet boundNew = boundVars;

  // Include the pattern and condition variables in the
  // set of bound variables for the recursive check
  size_t nrSubpatterns = subtermStrats.length();

  for (size_t index = 0; index < nrVars; index++)
    boundNew.insert(mainPattern.index2Variable(index));

  for (size_t i = 0; i < nrSubpatterns; i++)
    if (!subtermStrats[i]->check(vinfo, boundNew))
      return false;

  // Make the context translation vector
  // The inner context must contain the variables there but
  // also the not rewritten variables in the pattern
  NatSet patternVars = mainPattern.getLhs()->occursBelow();

  for (size_t i = 0; i < nrSubpatterns; i++)
    patternVars.subtract(subpatterns[i]->occursBelow());

  int nrInnerContextVars = vinfo.getNrRealVariables();
  contextSpec.expandTo(nrInnerContextVars);

  for (int i = 0; i < nrInnerContextVars; i++)
    {
      VariableTerm* var = static_cast<VariableTerm*>(vinfo.index2Variable(i));
      int innerIndex = mainPattern.variable2Index(static_cast<VariableDagNode*>(var->term2Dag()));

      if (innerIndex != NONE)
	{
	  contextSpec[i] = innerIndex;
	  if (variableMapping[innerIndex] < 0)
	    variableMapping[innerIndex] = -(2 + i);
	  patternVars.subtract(innerIndex);
	}
      else
	contextSpec[i] = - indices.variable2Index(var) - 1;	// the complemented outer index
    }

  if (!patternVars.empty())
    {
      contextSpec.expandBy(patternVars.size());

      int i = nrInnerContextVars;

      for (int index : patternVars)
	{
	  variableMapping[index] = -(2 + i);
	  contextSpec[i++] = index;
	}
    }

  return true;
}

bool
SubtermStrategy::checkSubterms()
{
  // Make sure the subterms appear in the main pattern.
  // At the same time it saves the origins from where variables should be
  // substitued when rebuilding the term (negative indices mean from the
  // original match, otherwise from the rewriting results)

  // May have extra useless entries (for the variables in the condition)
  variableMapping.expandTo(mainPattern.getNrRealVariables());

  // Not all indexed variables appear in the pattern
  for (size_t i = 0; i < variableMapping.size(); i++)
    variableMapping[i] = -1;

  // Finds the subterm in the main pattern variables
  const Term* patternTerm = mainPattern.getLhs();
  const NatSet& vars = patternTerm->occursBelow();
  NatSet alreadySeen;

  bool ok = true;

  for (size_t i = 0; i < subpatterns.size(); i++)
    {
      VariableTerm* var = dynamic_cast<VariableTerm*>(subpatterns[i]);

      // Checks the subpattern is a variable
      if (!var)
	{
	  IssueWarning(*patternTerm << ": subpattern " << QUOTE(subpatterns[i]) << " ("
	    << i+1 << ") is not a variable in matchrew strategy.");

	  ok = false;
	  continue;
	}

      if (!vars.contains(var->getIndex()))
	{
	  IssueWarning(*patternTerm << ": variable " << QUOTE(subpatterns[i]) << " ("
	    << i+1 << ") is not a subterm of the main pattern in matchrew strategy.");

	  ok = false;
	}
      else if (alreadySeen.contains(var->getIndex()))
	{
	  IssueWarning(*patternTerm << ": subpattern " << QUOTE(subpatterns[i]) << " ("
	    << i+1 << ") is repeated in matchrew strategy.");

	  ok = false;
	}
      else
	{
	  alreadySeen.insert(var->getIndex());
	  variableMapping[var->getIndex()] = i;
	}
    }

  return ok;
}

DagNode*
SubtermStrategy::rebuild(const Vector<DagNode*>& matchSubst, const Vector<DagNode*>& results) const
{
  // A substitution is build according to the rhs term indices
  Substitution subs(variableMapping.size()
    + mainPattern.getNrProtectedVariables());

  // See the constructor for explanations on the variable mapping
  for (size_t i = 0; i < variableMapping.size(); i++)
    {
      int from = variableMapping[i];
      if (from != -1)
	subs.bind(i, from < 0 ? matchSubst[-(2+from)] : results[from]);
    }

  return rhsBuilder.construct(subs);
}

StrategicExecution::Survival
SubtermStrategy::decompose(StrategicSearch& searchObject, DecompositionProcess* remainder)
{
  // Instantiates a search for the main pattern into the subject DAG node
  RewritingContext* context = searchObject.getContext();
  RewritingContext* newContext = context->makeSubcontext(searchObject.getCanonical(remainder->getDagIndex()));
  MatchSearchState* state = new MatchSearchState(newContext, &mainPattern, MatchSearchState::GC_CONTEXT |
						 MatchSearchState::GC_SUBSTITUTION, 0, depth);
  // Applies the variable bindings
  if (!indexTranslation.isNull())
    {
      VariableBindingsManager::ContextId varBinds = remainder->getOwner()->getVarsContext();
      Vector<Term*> vars;
      Vector<DagRoot*> values;
      searchObject.buildInitialSubstitution(varBinds, mainPattern, indexTranslation, vars, values);
      state->setInitialSubstitution(vars, values);
    }

  (void) new SubtermProcess(state, this, remainder->getPending(), remainder, remainder);

  // remainder should die because the process above gets in charge of its pending work
  // (the pending stack execution will be resumed after its processing)
  return StrategicExecution::DIE;
}
