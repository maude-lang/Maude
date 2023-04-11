/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class ImportTranslation.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "term.hh"

//      core class definitions
#include "argumentIterator.hh"
#include "rewriteStrategy.hh"

//	variable class definitions
#include "variableSymbol.hh"

//	built in class definitions
#include "sortTestSymbol.hh"

//	front end class definitions
#include "importModule.hh"
#include "importTranslation.hh"
#include "renaming.hh"

//	strategy class definitions
#include "callStrategy.hh"

ImportTranslation::ImportTranslation(ImportModule* target, Renaming* renaming, bool preserveVariableIndicesFlag)
  : preserveVariableIndicesFlag(preserveVariableIndicesFlag)
{
  push(renaming, target);
}

bool
ImportTranslation::preserveVariableIndices()
{
  return preserveVariableIndicesFlag;
}

Sort*
ImportTranslation::translate(const Sort* sort)
{
  if (sort->index() == Sort::KIND)
    return translate(sort->component())->sort(Sort::KIND);

  int id = sort->id();
  for (Renaming* r : renamings)
    {
      if (r != 0)
	id = r->renameSort(id);
    }
  Sort* s = targets.back()->findSort(id);
  Assert(s != 0, "no translation for sort " << sort <<
	 " in " << targets.back());
  return s;
}

int
ImportTranslation::translateLabel(int id)
{
  if (id != NONE)
    {
      for (Renaming* r : renamings)
	{
	  if (r != 0)
	    id = r->renameLabel(id);
	}
    }
  return id;
}

Symbol*
ImportTranslation::translate(Symbol* symbol)
{
  auto i = symbolMap.find(symbol);
  if (i != symbolMap.end())
    return i->second;

  Symbol* s = 0;
  switch (safeCast(MixfixModule*, symbol->getModule())->
	  getSymbolType(symbol).getBasicType())
    {
    case SymbolType::VARIABLE:
      {
	Sort* sort = translate(safeCast(VariableSymbol*, symbol)->getSort());
	s = targets.back()->instantiateVariable(sort);
	break;
      }
    case SymbolType::SORT_TEST:
      {
	SortTestSymbol* t = safeCast(SortTestSymbol*, symbol);
	s = targets.back()->instantiateSortTest(translate(t->sort()), t->eager());
	break;
      }
    default:
      {
	RenamingList::const_iterator dummyIterator;
	int dummyIndex;
	s = translateRegularSymbol(symbol, dummyIterator, dummyIndex);
	break;
      }
    }
  if (s != 0)
    symbolMap.insert({symbol, s});
  return s;
}

RewriteStrategy*
ImportTranslation::translate(RewriteStrategy* strat)
{
  auto i = strategyMap.find(strat);

  // If we have already translated it
  if (i != strategyMap.end())
    return i->second;

  RenamingList::const_iterator dummyIterator;
  int dummyIndex;
  RewriteStrategy* s = translateStrategy(strat, dummyIterator, dummyIndex);

  // Saves the translated strategy for other runs
  if (s != 0)
    strategyMap.insert({strat, s});

  return s;
}

RewriteStrategy*
ImportTranslation::translateStrategy(RewriteStrategy* strat,
				     RenamingList::const_iterator& stratToExpr,
				     int& stratToExprIndex) const
{
  int nrArgs = strat->arity();
  int label = strat->id();

  const Vector<Sort*>& domain = strat->getDomain();
  Vector<int> sortNames(nrArgs);
  for (int i = 0; i < nrArgs; i++)
    sortNames[i] = domain[i]->id();

  FOR_EACH_CONST(i, RenamingList, renamings)
    {
      Renaming* r =  *i;
      if (r != 0)
	{
	  //
	  //	Translate name.
	  //
	  int index = r->renameStrat(label, sortNames);
	  if (index != NONE)
	    {
	      label = r->getStratTo(index);
	      if (label == NONE)
		{
		  stratToExpr = i;
		  stratToExprIndex = index;
		  return 0;  // must be an strat->expr mapping
		}
	    }
	  //
	  //	Translate domain sorts.
	  //
	  for (int j = 0; j < nrArgs; ++j)
	    sortNames[j] = r->renameSort(sortNames[j]);
	}
    }

  // Looks for the strategy in the module
  ImportModule* target = targets.back();
  Vector<ConnectedComponent*> domainComponents(nrArgs);

  for (int i = 0; i < nrArgs; i++)
    domainComponents[i] = target->findSort(sortNames[i])->component();

  RewriteStrategy* s = target->findStrategy(label, domainComponents);

  Assert(s, "no translation for strategy " << strat <<
	      " in " << targets.back());

  return s;
}

void
ImportTranslation::splitTranslation(RenamingList::const_iterator firstMapping,
				    ImportTranslation*& prefix,
				    ImportTranslation*& suffix)
{
  prefix = this;
  suffix = 0;

  ++firstMapping;

  const RenamingList::const_iterator end = renamings.end();
  if (firstMapping != end)
    {
      //
      //	The problem renaming wasn't the last one so we need a real
      //	split into prefix and suffix import translations.
      //
      prefix = new ImportTranslation;
      RenamingList::const_iterator i = renamings.begin();
      ModuleList::const_iterator j = targets.begin();
      for (; i != firstMapping; ++i, ++j)
	{
	  prefix->renamings.push_back(*i);
	  prefix->targets.push_back(*j);
	}
      DebugAdvisory("prefix has size " << prefix->renamings.size());
      suffix = new ImportTranslation;
      for (; i != end; ++i, ++j)
	{
	  suffix->renamings.push_back(*i);
	  suffix->targets.push_back(*j);
	}
      DebugAdvisory("suffix has size " << suffix->renamings.size());
    }
}

StrategyExpression*
ImportTranslation::translateExpr(const CallStrategy* cs)
{
  DebugAdvisory("translateExpr() on " << cs << " we have size " << renamings.size());

  RewriteStrategy* strat = cs->getStrategy();
  RenamingList::const_iterator firstStratToExpr;
  int index = NONE;  // avoid compiler warning
  (void) translateStrategy(strat, firstStratToExpr, index);

  const Vector<int> vars = (*firstStratToExpr)->getStratVarIndices(index);
  StrategyExpression* toExpr = (*firstStratToExpr)->getStratTargetExpr(index);

  ImportTranslation *prefix, *suffix;
  splitTranslation(firstStratToExpr, prefix, suffix);

  //
  //	Collect the arguments under the top symbol, and use them to instantiate
  //	toTerm via the prefix importTranslation.
  //
  Vector<Term*> varBindings(strat->arity());
  int j = 0;
  for (ArgumentIterator i(*(const_cast<Term*>(cs->getTerm()))); i.valid(); i.next(), ++j)
    varBindings[j] = i.argument();

  StrategyExpression* instantiated =
    ImportModule::instantiateExpression(toExpr, vars, varBindings, prefix);
  //
  //	If there are more renamings to apply, apply them and tidy up.
  //
  if (suffix != 0)
    {
      StrategyExpression* s = ImportModule::deepCopyStrategyExpression(suffix, instantiated);
      delete instantiated;
      delete prefix;
      delete suffix;
      instantiated = s;
    }
  return instantiated;
}

Term*
ImportTranslation::translateTerm(const Term* term)
{
  DebugAdvisory("translateTerm() on " << term << " we have size " << renamings.size());
  Symbol* symbol = term->symbol();
  //
  //	We are called because the translation for the top symbol of term
  //	involves at least one op->term mapping. We locate that first by rerunning
  //	the translateRegularSymbol() and using the information passed back by the
  //	extra arguments.
  //
  RenamingList::const_iterator firstOpToTerm;  // this will be the Renaming with the op->term mappng
  int index = NONE;  // this will be the index of the op->term mapping withing the Renaming; initialize to NONE to avoid compiler warning
  (void) translateRegularSymbol(symbol, firstOpToTerm, index);
  Term* toTerm = (*firstOpToTerm)->getOpTargetTerm(index);
  //
  //	Usually there will be more renamings after the first op->term mapping;
  //	If this is so we need to split our import translation into prefix and
  //	suffix parts. We can only do the prefix portion before we need to instantiate
  //	the term part of the op->term mapping. Only then can we do the suffix portion.
  //
  ImportTranslation *prefix, *suffix;
  splitTranslation(firstOpToTerm, prefix, suffix);
  //
  //	Collect the arguments under the top symbol. Note that these
  //	will need to undergo translation by prefix. However we can't
  //	do this here since we cannot have subterm sharing. Rather
  //	translations will be done on demand in the variable instantiation
  //	code.
  //
  Vector<Term*> varBindings(symbol->arity());
  int j = 0;
  for (ArgumentIterator i(*(const_cast<Term*>(term))); i.valid(); i.next(), ++j)
    varBindings[j] = i.argument();
  //
  //	Now we can instantiate the toTerm on copies of these
  //	made for each variable occurance using using prefix translator.
  //
  //	Note that the toTerm lives in the target module TM of a view V
  //	whereas the module M{...V...} we are translating into is an instantiation
  //	of some other module M, that imports TM via V. So translating sorts and
  //	operators to the final module in prefix during the instantiation of
  //	toTerm is mandatory.
  //
  Term* translatedTerm = toTerm->instantiate(varBindings, prefix);
  //
  //	If there are more renamings to apply, apply them and tidy up.
  //
  if (suffix != 0)
    {
      Term* t = translatedTerm->deepCopy(suffix);
      translatedTerm->deepSelfDestruct();
      translatedTerm = t;
      delete prefix;
      delete suffix;
    }
  return translatedTerm;
}

Symbol*
ImportTranslation::translateRegularSymbol(Symbol* symbol,
					  RenamingList::const_iterator& opToTerm,
					  int& opToTermIndex) const
{
  int nrArgs = symbol->arity();
  int id = symbol->id();
  Vector<int> sortNames(nrArgs + 1);
  for (int i = 0; i < nrArgs; ++i)
    sortNames[i] = symbol->domainComponent(i)->sort(1)->id();
  sortNames[nrArgs] = symbol->rangeComponent()->sort(1)->id();

  FOR_EACH_CONST(i, RenamingList, renamings)
    {
      Renaming* r =  *i;
      if (r != 0)
	{
	  //
	  //	Translate name.
	  //
	  int index = r->renameOp(id, sortNames);
	  if (index != NONE)
	    {
	      id = r->getOpTo(index);
	      if (id == NONE)
		{
		  opToTerm = i;
		  opToTermIndex = index;
		  return 0;  // must be an op->term mapping
		}
	    }
	  //
	  //	Translate domain and range sorts.
	  //
	  for (int j = 0; j <= nrArgs; ++j)
	    sortNames[j] = r->renameSort(sortNames[j]);
	}
    }
  //
  //	Look up symbol in target module.
  //
  ImportModule* target = targets.back();
  Vector<ConnectedComponent*> domainComponents(nrArgs);
  for (int i = 0; i < nrArgs; ++i)
    domainComponents[i] = target->findSort(sortNames[i])->component();
  Symbol* s = target->findSymbol(id, domainComponents, target->findSort(sortNames[nrArgs])->component());
  //
  //	Because we use a return value of 0 to indicate an op->term mapping we never
  //	want to return 0 due to an internal error - even in the optimized build.
  //
  AlwaysAssert(s != 0, "no translation for " << symbol << " with " << nrArgs << " args in " << target << " was looking for " <<
	       Token::name(id));
  return s;
}

Symbol*
ImportTranslation::findTargetVersionOfSymbol(Symbol* symbol)
{
  ImportModule *target = targets.back();
  MixfixModule *fromModule = safeCastNonNull<MixfixModule*>(symbol->getModule());
  switch (fromModule->getSymbolType(symbol).getBasicType())
    {
    case SymbolType::VARIABLE:
      {
	//
	//	The point of findTargetVersionOfSymbol() is to translate the symbols from the toTerm of
	//	an op to term mapping into the last module. But variables are intended to be instantiated
	//	rather than translated so we should never be asked for a translation.
	//
	DebugAdvisory("unexpected request for target version of a variable symbol " << symbol << " in " << target);
	//
	//	But to maintain the generality of the code in case we find other uses for it, we do
	//	the translation anyway.
	//
	VariableSymbol* v = safeCastNonNull<VariableSymbol*>(symbol);
	Sort* vs = v->getSort();
	DebugInfo("mapping variable symbol " << v << " of sort " << vs << " in " << vs->getModule());
	Sort* targetSort = target->findSort(vs->id());
	Assert(targetSort != 0, "couldn't find " << vs << " in " << target);
	DebugInfo("mapped to sort " << targetSort << " in " << target);
	return target->instantiateVariable(targetSort);
      }
    case SymbolType::SORT_TEST:
      {
	SortTestSymbol* t = safeCastNonNull<SortTestSymbol*>(symbol);
	Sort* targetSort = target->findSort(t->sort()->id());
	Assert(targetSort != 0, "couldn't find " << t->sort() << " in " << target);
	return target->instantiateSortTest(targetSort, t->eager());
      }
    }
  int nrArgs = symbol->arity();
  Vector<ConnectedComponent*> domainComponents(nrArgs);
  for (int i = 0; i < nrArgs; ++i)
    {
      Sort* ds = symbol->domainComponent(i)->sort(1);
      Sort* targetSort = target->findSort(ds->id());
      Assert(targetSort != 0, "couldn't find sort " << ds << " in " << target);
      domainComponents[i] = targetSort->component();
    }
  Sort* rs = symbol->rangeComponent()->sort(1);
  Sort* targetSort = target->findSort(rs->id());
  Assert(targetSort != 0, "couldn't find sort  " << rs << " in " << target);
  ConnectedComponent* rangeComponent = targetSort->component();
  Symbol* targetSymbol = target->findSymbol(symbol->id(), domainComponents, rangeComponent);
  Assert(targetSymbol != 0, "couldn't find symbol " << symbol << " in " << target);
  return targetSymbol;
}

ConnectedComponent*
ImportTranslation::translate(Renaming* renaming,
			     ImportModule* target,
			     const ConnectedComponent* old)
{
  Sort* sort = old->sort(Sort::FIRST_USER_SORT);
  int id = sort->id();
  if (renaming != 0)
    id = renaming->renameSort(id);
  Sort* s = target->findSort(id);
  Assert(s != 0, "no translation for sort " << sort << " in " << target);
  return s->component();
}
