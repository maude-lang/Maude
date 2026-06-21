/*

    This file is part of the Maude 3 interpreter.

    Copyright 2026 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class TransformResultSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "meta.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "AU_Theory.hh"
#include "mixfix.hh"
#include "higher.hh"
#include "SMT.hh"
#include "meta.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	core class definitions
#include "symbolMap.hh"

//      AU class definitions
#include "AU_Symbol.hh"

//	built-in macros
#include "bindingMacros.hh"

//      MetaLevel class definitions
#include "metaLevelOpSymbol.hh"

//	mixfix class definitions
#include "transformResultSymbol.hh"

TransformResultSymbol::TransformResultSymbol(int id)
  : FreeSymbol(id, 2)
{
#define MACRO(SymbolName, SymbolClass) \
  SymbolName = nullptr;
#include "transformResultSignature.cc"
#undef MACRO

  metaLevel = nullptr;
  cachedImplicitTransformer = nullptr;
}

TransformResultSymbol::~TransformResultSymbol()
{  // maybe we don't need this
}

bool
TransformResultSymbol::attachData(const Vector<Sort*>& opDeclaration,
				  const char* purpose,
				  const Vector<const char*>& data)
{
  if (strcmp(purpose, "TransformResultSymbol") == 0)
    return true;
  return FreeSymbol::attachData(opDeclaration, purpose, data);
}

void
TransformResultSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
					    Vector<const char*>& purposes,
					    Vector<Vector<const char*>>& data)
{
  purposes.push_back("TransformResultOpSymbol");
  data.resize(data.size() + 1);
  FreeSymbol::getDataAttachments(opDeclaration, purposes, data);
}

bool
TransformResultSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  Assert(symbol != nullptr, "null symbol for " << purpose);
#define MACRO(SymbolName, SymbolClass) \
  BIND_SYMBOL(purpose, symbol, SymbolName, SymbolClass*)
#include "transformResultSignature.cc"
#undef MACRO
  return FreeSymbol::attachSymbol(purpose, symbol);
}

void
TransformResultSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					    Vector<Symbol*>& symbols)
{
#define MACRO(SymbolName, SymbolClass) \
  APPEND_SYMBOL(purposes, symbols, SymbolName)
#include "transformResultSignature.cc"
#undef MACRO
  FreeSymbol::getSymbolAttachments(purposes, symbols);
}

void
TransformResultSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  TransformResultSymbol* orig = safeCastNonNull<TransformResultSymbol*>(original);
#define MACRO(SymbolName, SymbolClass) \
  COPY_SYMBOL(orig, SymbolName, map, SymbolClass*)
#include "transformResultSignature.cc"
#undef MACRO
  FreeSymbol::copyAttachments(original, map);
}
