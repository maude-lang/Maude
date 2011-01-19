/*

    This file is part of the Maude 2 interpreter.

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

//	variable class definitions
#include "variableSymbol.hh"

//	built in class definitions
#include "sortTestSymbol.hh"

//	front end class definitions
#include "importModule.hh"
#include "importTranslation.hh"
#include "renaming.hh"

ImportTranslation::ImportTranslation(ImportModule* target, Renaming* renaming)
{
  push(renaming, target);
}

Sort*
ImportTranslation::translate(const Sort* sort)
{
  if (sort->index() == Sort::KIND)
    return translate(sort->component())->sort(Sort::KIND);

  int id = sort->id();
  FOR_EACH_CONST(i, RenamingList, renamings)
    {
      if (Renaming* r = *i)
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
      FOR_EACH_CONST(i, RenamingList, renamings)
	{
	  if (Renaming* r = *i)
	    id = r->renameLabel(id);
	}
    }
  return id;
}

Symbol*
ImportTranslation::translate(Symbol* symbol)
{
  Symbol* s = static_cast<Symbol*>(directMap.getMap(symbol));
  if (s != 0)
    return s;

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
    directMap.setMap(symbol, s);
  return s;
}

Term*
ImportTranslation::translateTerm(const Term* term)
{
  DebugAdvisory("translateTerm() on " << term << " we have size " << renamings.size());
  //
  //	We are called because the translation for the top symbol of term
  //	involves at least one op->term mapping. We locate that first by rerunning
  //	the translateRegularSymbol() and using the information passed back by the
  //	extra arguments.
  //
  Symbol* symbol = term->symbol();
  RenamingList::const_iterator firstOpToTerm;
  int index;
  (void) translateRegularSymbol(symbol, firstOpToTerm, index);
  Term* toTerm = (*firstOpToTerm)->getOpTargetTerm(index);
  //
  //	Usually there will be more renamings after the first op->term mapping;
  //	If this is so we need to split our import translation into prefix and
  //	suffix parts. We can only do the prefix portion before we need to instantiate
  //	the term part of the op->term mapping. Only then can we do the suffix portion.
  //
  ImportTranslation* prefix = this;
  ImportTranslation* suffix = 0;
  ++firstOpToTerm;
  const RenamingList::const_iterator e = renamings.end();
  if (firstOpToTerm != e)
    {
      prefix = new ImportTranslation;
      RenamingList::const_iterator i = renamings.begin();
      ModuleList::const_iterator j = targets.begin();
      for (; i != firstOpToTerm; ++i, ++j)
	{
	  prefix->renamings.push_back(*i);
	  prefix->targets.push_back(*j);
	}
      DebugAdvisory("prefix has size " << prefix->renamings.size());
      suffix = new ImportTranslation;
      for (; i != e;  ++i, ++j)
      	{
	  suffix->renamings.push_back(*i);
	  suffix->targets.push_back(*j);
	}
      DebugAdvisory("suffix has size " << suffix->renamings.size());
    }
  //
  //	Collect the arguments under the top symbol, and use them to instantiate
  //	toTerm via the prefix importTranslation.
  //
  Vector<Term*> varBindings(symbol->arity());
  int j = 0;
  for (ArgumentIterator i(*(const_cast<Term*>(term))); i.valid(); i.next(), ++j)
    varBindings[j] = i.argument();
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
  Assert(s != 0, "no translation for " << symbol << " in " << target);
  return s;
}

Symbol*
ImportTranslation::findTargetVersionOfSymbol(Symbol* symbol)
{
  ImportModule *target = targets.back();
  switch (safeCast(MixfixModule*, symbol->getModule())->
	  getSymbolType(symbol).getBasicType())
    {
    case SymbolType::VARIABLE:
      {
	Sort* sort = target->findSort(safeCast(VariableSymbol*, symbol)->getSort()->id());
	return target->instantiateVariable(sort);
      }
    case SymbolType::SORT_TEST:
      {
	SortTestSymbol* t = safeCast(SortTestSymbol*, symbol);
	return target->instantiateSortTest(target->findSort(t->sort()->id()), t->eager());
      }
    }
  int nrArgs = symbol->arity();
  Vector<ConnectedComponent*> domainComponents(nrArgs);
  for (int i = 0; i < nrArgs; ++i)
    domainComponents[i] = target->findSort(symbol->domainComponent(i)->sort(1)->id())->component();
  ConnectedComponent* rangeComponent = target->findSort(symbol->rangeComponent()->sort(1)->id())->component();
  return target->findSymbol(symbol->id(), domainComponents, rangeComponent);
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
