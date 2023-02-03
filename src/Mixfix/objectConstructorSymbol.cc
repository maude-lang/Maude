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
//      Implementation for class ObjectConstructorSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "ACU_Theory.hh"

//      interface class definitions
//#include "term.hh"
//#include "argumentIterator.hh"

//      core class definitions
//#include "rewritingContext.hh"
#include "symbolMap.hh"

//      free theory class definitions
//#include "freeNet.hh"
//#include "freeDagNode.hh"

//      ACU theory class definitions
#include "ACU_Symbol.hh"

//      built in stuff
#include "bindingMacros.hh"
#include "objectConstructorSymbol.hh"

ObjectConstructorSymbol::ObjectConstructorSymbol(int id)
  : FreeSymbol(id, 3)
{
}

bool
ObjectConstructorSymbol::attachData(const Vector<Sort*>& opDeclaration,
			   const char* purpose,
			   const Vector<const char*>& data)
{
  NULL_DATA(purpose, ObjectConstructorSymbol, data)
  return FreeSymbol::attachData(opDeclaration, purpose, data);
}

bool
ObjectConstructorSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  BIND_SYMBOL(purpose, symbol, attributeSetSymbol, ACU_Symbol*);
  return FreeSymbol::attachSymbol(purpose, symbol);
}

void
ObjectConstructorSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  ObjectConstructorSymbol* orig = safeCast(ObjectConstructorSymbol*, original);
  COPY_SYMBOL(orig, attributeSetSymbol, map, ACU_Symbol*);
  FreeSymbol::copyAttachments(original, map);
}

void
ObjectConstructorSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
					    Vector<const char*>& purposes,
					    Vector<Vector<const char*> >& data)
{
  APPEND_DATA(purposes, data, ObjectConstructorSymbol);
  FreeSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
ObjectConstructorSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					      Vector<Symbol*>& symbols)
{
  APPEND_SYMBOL(purposes, symbols, attributeSetSymbol);
  FreeSymbol::getSymbolAttachments(purposes, symbols);
}
