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

ImportTranslation::ImportTranslation(ImportModule* importer)
  : importer(importer)
{
}

Sort*
ImportTranslation::translate(const Sort* sort)
{
  if (sort->index() == Sort::ERROR_SORT)
    return translate(sort->component())->sort(Sort::ERROR_SORT);
  Sort* s = importer->findSort(sort->id());
  Assert(s != 0, "no translation for sort " << sort <<
	 " in " << importer);
  return s;
}

Symbol*
ImportTranslation::translate(Symbol* symbol)
{
  Symbol* s = static_cast<Symbol*>(directMap.getMap(symbol));
  if (s != 0)
    return s;
  Symbol* importerVersion = 0;
  switch (static_cast<MixfixModule*>(symbol->getModule())->getSymbolType(symbol).getBasicType())
    {
    case SymbolType::VARIABLE:
      {
	Sort* sort = translate(safeCast(VariableSymbol*, symbol)->getSort());
	importerVersion = importer->instantiateVariable(sort);
	break;
      }
    case SymbolType::SORT_TEST:
      {
	SortTestSymbol* t = safeCast(SortTestSymbol*, symbol);
	importerVersion = importer->instantiateSortTest(translate(t->sort()), t->eager());
	break;
      }
    default:
      {
	int nrArgs = symbol->arity();
	static Vector<ConnectedComponent*> domainComponents;
	domainComponents.resize(nrArgs);
	for (int i = 0; i < nrArgs; i++)
	  domainComponents[i] = translate(symbol->domainComponent(i));
	ConnectedComponent* rangeComponent = translate(symbol->rangeComponent());
	importerVersion = importer->findSymbol(symbol->id(), domainComponents, rangeComponent);
	break;
      }
    }
  Assert(importerVersion != 0, "no translation for " << symbol <<
	 " in " << importer);
  directMap.setMap(symbol, importerVersion);
  return importerVersion;
}
