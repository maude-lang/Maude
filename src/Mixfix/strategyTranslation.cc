/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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
//      Strategy translation for ImportModule.
//
//      Translation is not done recursively in the StrategyExpression class for the same
//      reason that condition fragments are copied the same way.
//

//	utility stuff
#include "macros.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//	implementing class
#include "importModule.hh"

//	interface class
#include "rawArgumentIterator.hh"

//	core classes
#include "term.hh"
#include "conditionFragment.hh"
#include "rewriteStrategy.hh"
#include "substitution.hh"

//	mixfix classes
#include "importTranslation.hh"

//	strategy language classes
#include "strategicSearch.hh"
#include "strategyExpression.hh"
#include "trivialStrategy.hh"
#include "unionStrategy.hh"
#include "concatenationStrategy.hh"
#include "callStrategy.hh"
#include "iterationStrategy.hh"
#include "branchStrategy.hh"
#include "applicationStrategy.hh"
#include "testStrategy.hh"
#include "subtermStrategy.hh"
#include "oneStrategy.hh"

//	higuer language classes
#include "assignmentConditionFragment.hh"
#include "equalityConditionFragment.hh"
#include "rewriteConditionFragment.hh"
#include "sortTestConditionFragment.hh"

StrategyExpression*
ImportModule::deepCopyStrategyExpression(ImportTranslation* importTranslation,
					 StrategyExpression* original)
{
  if (TrivialStrategy* trs = dynamic_cast<TrivialStrategy*>(original))
    return new TrivialStrategy(trs->getResult());

  else if (CallStrategy* cs = dynamic_cast<CallStrategy*>(original))
    {
      RewriteStrategy* strategy = importTranslation->translate(cs->getStrategy());
      // If strategy is null (to expr mappings), the call must be translated
      // to a strategy expression
      if (strategy == 0)
	{
	  return importTranslation->translateExpr(cs);
	}
      // Otherwise, the translation is another call strategy expression
      return new CallStrategy(strategy,
			      strategy->copyAuxiliaryTerm(cs->getTerm(), importTranslation));
    }

  else if (IterationStrategy* is = dynamic_cast<IterationStrategy*>(original))
    return new IterationStrategy(deepCopyStrategyExpression(importTranslation, is->getStrategy()),
				 is->getZeroAllowed());

  else if (UnionStrategy* us = dynamic_cast<UnionStrategy*>(original))
    {
      const Vector<StrategyExpression*>& strategies = us->getStrategies();
      Vector<StrategyExpression*> strategiesCopy(strategies.size());

      for (size_t i = 0; i < strategies.size(); i++)
	strategiesCopy[i] = deepCopyStrategyExpression(importTranslation, strategies[i]);

      return new UnionStrategy(strategiesCopy);
    }

  else if (ConcatenationStrategy* cs = dynamic_cast<ConcatenationStrategy*>(original))
    {
      const Vector<StrategyExpression*>& strategies = cs->getStrategies();
      Vector<StrategyExpression*> strategiesCopy(strategies.size());

      for (size_t i = 0; i < strategies.size(); i++)
	strategiesCopy[i] = deepCopyStrategyExpression(importTranslation, strategies[i]);

      return new ConcatenationStrategy(strategiesCopy);
    }

  else if (BranchStrategy* bs = dynamic_cast<BranchStrategy*>(original))
    {
      return new BranchStrategy(deepCopyStrategyExpression(importTranslation, bs->getInitialStrategy()),
				bs->getSuccessAction(),
				bs->getSuccessStrategy() ? deepCopyStrategyExpression(importTranslation, bs->getSuccessStrategy()) : 0,
				bs->getFailureAction(),
				bs->getFailureStrategy() ? deepCopyStrategyExpression(importTranslation, bs->getFailureStrategy()) : 0);
    }

  else if (ApplicationStrategy* as = dynamic_cast<ApplicationStrategy*>(original))
    {
      const Vector<StrategyExpression*>& strategies = as->getStrategies();
      const Vector<Term*>& variables = as->getVariables();
      const Vector<CachedDag>& values = as->getValues();

      Vector<StrategyExpression*> strategiesCopy(strategies.size());
      Vector<Term*> variablesCopy(variables.size());
      Vector<Term*> valuesCopy(values.size());

      for (size_t i = 0; i < strategies.size(); i++)
	strategiesCopy[i] = deepCopyStrategyExpression(importTranslation, strategies[i]);

      for (size_t i = 0; i < variables.size(); i++)
	{
	  variablesCopy[i] = variables[i]->deepCopy(importTranslation);
	  valuesCopy[i] = values[i].getTerm()->deepCopy(importTranslation);
	}

      int label = importTranslation->translateLabel(as->getLabel());

      ApplicationStrategy* copy = new ApplicationStrategy(label, variablesCopy, valuesCopy, strategiesCopy);
      if (as->getTop())
	copy->setTop();

      return copy;
    }

  else if (TestStrategy* te = dynamic_cast<TestStrategy*>(original))
    {
      Vector<ConditionFragment*> conditionCopy(te->getCondition().size());

      deepCopyCondition(importTranslation, te->getCondition(), conditionCopy);

      return new TestStrategy(te->getPatternTerm()->deepCopy(importTranslation),
			      te->getDepth(),
			      conditionCopy);
    }

  else if (SubtermStrategy* mrs = dynamic_cast<SubtermStrategy*>(original))
    {
      const Vector<StrategyExpression*>& strategies = mrs->getStrategies();
      const Vector<Term*>& patterns = mrs->getSubterms();

      Vector<StrategyExpression*> strategiesCopy(strategies.size());
      Vector<Term*> patternsCopy(patterns.size());
      Vector<ConditionFragment*> conditionCopy(mrs->getCondition().size());

      for (size_t i = 0; i < strategies.size(); i++)
	{
	  strategiesCopy[i] = deepCopyStrategyExpression(importTranslation, strategies[i]);
	  patternsCopy[i] = patterns[i]->deepCopy(importTranslation);
	}

      deepCopyCondition(importTranslation, mrs->getCondition(), conditionCopy);

      return new SubtermStrategy(mrs->getPatternTerm()->deepCopy(importTranslation),
				 mrs->getDepth(),
				 conditionCopy,
				 patternsCopy,
				 strategiesCopy);
    }
  else if (OneStrategy* os = dynamic_cast<OneStrategy*>(original))
    return new OneStrategy(deepCopyStrategyExpression(importTranslation, os->getStrategy()));

  CantHappen("unknown strategy expression");
  return 0;
}

StrategyExpression*
ImportModule::instantiateExpression(StrategyExpression* expr,
				    const Vector<int>& varMap,
				    const Vector<Term*>& values,
				    ImportTranslation* translation)
{
  int nrContextSize = varMap.length();
  Vector<Term*> subs(nrContextSize);

  for (int i = 0; i < nrContextSize; i++)
    subs[i] = values[varMap[i]];

  return instantiateExpression(expr, subs, translation);
}

Term*
ImportModule::instantiateCall(Term* term,
			      RewriteStrategy* strat,
			      const Vector<Term*>& subs,
			      ImportTranslation* translation)
{
  //
  // Instantiate the arguments and rebuild the call term
  //
  Symbol* symbol = term->symbol();
  Vector<Term*> args(symbol->arity());

  RawArgumentIterator* it = term->arguments();
  for (size_t i = 0; i < args.size(); i++)
    {
      Assert(it->valid(), "non-valid iterator");

      args[i] = it->argument()->instantiate(subs, translation);
      it->next();
    }
  delete it;

  return strat->makeAuxiliaryTerm(args);
}

StrategyExpression*
ImportModule::instantiateExpression(StrategyExpression* original,
				    const Vector<Term*>& subs,
				    ImportTranslation* translation)
{
  //
  // We return an instantiated copy
  //
  if (TrivialStrategy* trs = dynamic_cast<TrivialStrategy*>(original))
    return new TrivialStrategy(trs->getResult());

  else if (CallStrategy* cs = dynamic_cast<CallStrategy*>(original))
    {
      RewriteStrategy* strategy = translation->translate(cs->getStrategy());
      Term* callTerm = cs->getTerm();

      Term* instantiated = instantiateCall(callTerm, strategy, subs, translation);
      Assert(instantiated, "instantiated is null");

      return new CallStrategy(strategy, instantiated);
    }

  else if (IterationStrategy* is = dynamic_cast<IterationStrategy*>(original))
    return new IterationStrategy(instantiateExpression(is->getStrategy(), subs, translation),
				 is->getZeroAllowed());

  else if (UnionStrategy* us = dynamic_cast<UnionStrategy*>(original))
    {
      const Vector<StrategyExpression*>& strategies = us->getStrategies();
      Vector<StrategyExpression*> instantiatedStrats(strategies.size());

      for (size_t i = 0; i < strategies.size(); i++)
	instantiatedStrats[i] = instantiateExpression(strategies[i], subs, translation);

      return new UnionStrategy(instantiatedStrats);
    }

  else if (ConcatenationStrategy* cs = dynamic_cast<ConcatenationStrategy*>(original))
    {
      const Vector<StrategyExpression*>& strategies = cs->getStrategies();
      Vector<StrategyExpression*> instantiatedStrats(strategies.size());

      for (size_t i = 0; i < strategies.size(); i++)
	instantiatedStrats[i] = instantiateExpression(strategies[i], subs, translation);

      return new ConcatenationStrategy(instantiatedStrats);
    }

  else if (BranchStrategy* bs = dynamic_cast<BranchStrategy*>(original))
    {
      return new BranchStrategy(instantiateExpression(bs->getInitialStrategy(), subs, translation),
				bs->getSuccessAction(),
				bs->getSuccessStrategy() ? instantiateExpression(bs->getSuccessStrategy(), subs, translation) : 0,
				bs->getFailureAction(),
				bs->getFailureStrategy() ? instantiateExpression(bs->getFailureStrategy(), subs, translation) : 0);
    }

  else if (ApplicationStrategy* as = dynamic_cast<ApplicationStrategy*>(original))
    {
      const Vector<StrategyExpression*>& strategies = as->getStrategies();
      const Vector<Term*>& variables = as->getVariables();
      const Vector<CachedDag>& values = as->getValues();

      Vector<StrategyExpression*> strategiesCopy(strategies.size());
      Vector<Term*> variablesCopy(variables.size());
      Vector<Term*> instantiatedValues(values.size());

      for (size_t i = 0; i < strategies.size(); i++)
	strategiesCopy[i] = instantiateExpression(strategies[i], subs, translation);

      for (size_t i = 0; i < variables.size(); i++)
	{
	  variablesCopy[i] = variables[i]->deepCopy(translation);
	  VariableInfo vinfo;
	  instantiatedValues[i] = values[i].getTerm()->instantiate(subs, translation);
	}

      int label = translation->translateLabel(as->getLabel());

      ApplicationStrategy* copy = new ApplicationStrategy(label,
							  variablesCopy,
							  instantiatedValues,
							  strategiesCopy);
      if (as->getTop())
	copy->setTop();

      return copy;
    }

  else if (TestStrategy* te = dynamic_cast<TestStrategy*>(original))
    {
      Vector<ConditionFragment*> conditionCopy(te->getCondition().size());

      // Builds and applies the substitution for the condition
      {
	const Vector<pair<int, int> >& indexTranslation = te->getIndexTranslation();
	int contextSize = te->getPattern().getNrRealVariables();
	Vector<Term*> conditionSubs(contextSize);
	for (int i = 0; i < contextSize; i++)
	  conditionSubs[i] = te->getPattern().index2Variable(i);
	size_t translationSize = indexTranslation.length();
	for (size_t i = 0; i < translationSize; i++)
	  conditionSubs[indexTranslation[i].first] = subs[indexTranslation[i].second];

	instantiateCondition(te->getCondition(), conditionCopy, conditionSubs, translation);
      }

      return new TestStrategy(te->getPatternTerm()->deepCopy(translation), te->getDepth(), conditionCopy);
    }

  else if (SubtermStrategy* mrs = dynamic_cast<SubtermStrategy*>(original))
    {
      const Vector<StrategyExpression*>& strategies = mrs->getStrategies();
      const Vector<Term*>& patterns = mrs->getSubterms();

      Vector<StrategyExpression*> strategiesCopy(strategies.size());
      Vector<Term*> patternsCopy(patterns.size());
      Vector<ConditionFragment*> conditionCopy(mrs->getCondition().size());

      // Builds and applies the substitution for inside the matchrew
      {
	const Vector<int>& contextSpec = mrs->getContextSpec();
	int innerContextSize = contextSpec.size();
	Vector<Term*> innerSubs(innerContextSize);
	for (int i = 0; i < innerContextSize; i++)
	  innerSubs[i] = contextSpec[i] < 0 ? subs[-(1 + contextSpec[i])]
			  : mrs->getPattern().index2Variable(contextSpec[i]);

	for (size_t i = 0; i < strategies.size(); i++)
	  {
	    strategiesCopy[i] = instantiateExpression(strategies[i], innerSubs, translation);
	    patternsCopy[i] = patterns[i]->deepCopy(translation);
	  }
      }

      // Builds and applies the substitution for the condition
      {
	const Vector<pair<int, int> >& indexTranslation = mrs->getIndexTranslation();
	int contextSize = mrs->getPattern().getNrRealVariables();
	Vector<Term*> conditionSubs(contextSize);
	for (int i = 0; i < contextSize; i++)
	  conditionSubs[i] = mrs->getPattern().index2Variable(i);
	size_t translationSize = indexTranslation.length();
	for (size_t i = 0; i < translationSize; i++)
	  conditionSubs[indexTranslation[i].first] = subs[indexTranslation[i].second];

	instantiateCondition(mrs->getCondition(), conditionCopy, conditionSubs, translation);
      }

      return new SubtermStrategy(mrs->getPatternTerm()->deepCopy(translation),
				 mrs->getDepth(),
				 conditionCopy,
				 patternsCopy,
				 strategiesCopy);
    }
  else if (OneStrategy* os = dynamic_cast<OneStrategy*>(original))
    return new OneStrategy(instantiateExpression(os->getStrategy(), subs, translation));

  CantHappen("unknow strategy expression");
  return 0;
}

void
ImportModule::instantiateCondition(const Vector<ConditionFragment*>& original,
				   Vector<ConditionFragment*>& copy,
				   const Vector<Term*>& mapping,
				   ImportTranslation* translation)
{
  int nrFragments = original.length();
  copy.expandTo(nrFragments);

  for (int i = 0; i < nrFragments; i++)
    {
      ConditionFragment* c = original[i];
      ConditionFragment* n;
      if (EqualityConditionFragment* e = dynamic_cast<EqualityConditionFragment*>(c))
	{
	  Term* lhs = e->getLhs()->instantiate(mapping, translation);
	  Term* rhs = e->getRhs()->instantiate(mapping, translation);
	  n = new EqualityConditionFragment(lhs, rhs);
	}
      else if (SortTestConditionFragment* s = dynamic_cast<SortTestConditionFragment*>(c))
	{
	  Term* lhs = s->getLhs()->instantiate(mapping, translation);
	  n = new SortTestConditionFragment(lhs, s->getSort());
	}
      else if (AssignmentConditionFragment* a = dynamic_cast<AssignmentConditionFragment*>(c))
	{
	  Term* lhs = a->getLhs()->instantiate(mapping, translation);
	  Term* rhs = a->getRhs()->instantiate(mapping, translation);
	  n = new AssignmentConditionFragment(lhs, rhs);
	}
      else if (RewriteConditionFragment* r = dynamic_cast<RewriteConditionFragment*>(c))
	{
	  Term* lhs = r->getLhs()->instantiate(mapping, translation);
	  Term* rhs = r->getRhs()->instantiate(mapping, translation);
	  n = new RewriteConditionFragment(lhs, rhs);
	}
      else
	{
	  CantHappen("bad condition fragment");
	  n = 0;  // avoid compiler warning
	}
      copy[i] = n;
    }
}
