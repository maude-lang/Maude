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
#include "mixfix.hh"

//      interface class definitions
#include "term.hh"

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
  if (sort->index() == Sort::ERROR_SORT)
    return translate(sort->component())->sort(Sort::ERROR_SORT);

  int id = sort->id();
  FOR_EACH_CONST(i, list<Renaming*>, renamings)
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
      FOR_EACH_CONST(i, list<Renaming*>, renamings)
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
	int nrArgs = symbol->arity();
	Vector<ConnectedComponent*> domainComponents(nrArgs);
	s = symbol;
	list<ImportModule*>::const_iterator i2 = targets.begin();
	FOR_EACH_CONST(i, list<Renaming*>, renamings)
	  {
	    Renaming* r =  *i;
	    int id = s->id();
	    //
	    //	Change name if needed.
	    //
	    if (r != 0)
	      {
		int index = r->renameOp(s);
		if (index != NONE)
		  id = r->getOpTo(index);
	      }
	    //
	    //	Now translate domain and range components.
	    //
	    for (int j = 0; j < nrArgs; j++)
	      domainComponents[j] = translate(r, *i2, s->domainComponent(j));
	    ConnectedComponent* rangeComponent = translate(r, *i2, s->rangeComponent());
	    //
	    //	Look up symbol in target module.
	    //
	    s = (*i2)->findSymbol(id, domainComponents, rangeComponent);
	    Assert(s != 0, "no translation for renamed " << s  << " in " << *i2);
	    ++i2;
	  }
	break;
      }
    }
  Assert(s != 0, "no translation for " << symbol << " in " << targets.back());
  directMap.setMap(symbol, s);
  return s;
}

ConnectedComponent*
ImportTranslation::translate(Renaming* renaming,
			     ImportModule* target,
			     const ConnectedComponent* old)
{
  Sort* sort = old->sort(1);
  int id = sort->id();
  if (renaming != 0)
    id = renaming->renameSort(id);
  Sort* s = target->findSort(id);
  Assert(s != 0, "no translation for sort " << sort << " in " << target);
  return s->component();
}
