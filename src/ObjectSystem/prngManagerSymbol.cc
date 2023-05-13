/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class PrngManagerSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "NA_Theory.hh"
#include "S_Theory.hh"
#include "builtIn.hh"
#include "objectSystem.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "symbolMap.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//      built in class definitions
#include "succSymbol.hh"
#include "stringSymbol.hh"
#include "stringDagNode.hh"
#include "bindingMacros.hh"

//	object system class definitions
#include "objectSystemRewritingContext.hh"
#include "prngManagerSymbol.hh"

//	our stuff
#include "prngActions.cc"

PrngManagerSymbol::PrngManagerSymbol(int id)
  : ExternalObjectManagerSymbol(id)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolName = 0;
#include "prngSignature.cc"
#undef MACRO
}

bool
PrngManagerSymbol::attachData(const Vector<Sort*>& opDeclaration,
				const char* purpose,
				const Vector<const char*>& data)
{
  NULL_DATA(purpose, PrngManagerSymbol, data);
  return ExternalObjectManagerSymbol::attachData(opDeclaration, purpose, data);
}

bool
PrngManagerSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  Assert(symbol != 0, "null symbol for " << purpose);
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  BIND_SYMBOL(purpose, symbol, SymbolName, SymbolClass*)
#include "prngSignature.cc"
#undef MACRO
  return ExternalObjectManagerSymbol::attachSymbol(purpose, symbol);
}

void
PrngManagerSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  PrngManagerSymbol* orig = safeCast(PrngManagerSymbol*, original);
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  COPY_SYMBOL(orig, SymbolName, map, SymbolClass*)
#include "prngSignature.cc"
#undef MACRO
  ExternalObjectManagerSymbol::copyAttachments(original, map);
}

void
PrngManagerSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				      Vector<const char*>& purposes,
				      Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "PrngManagerSymbol";
  data.resize(nrDataAttachments + 1);
  ExternalObjectManagerSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
PrngManagerSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					  Vector<Symbol*>& symbols)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  APPEND_SYMBOL(purposes, symbols, SymbolName)
#include "prngSignature.cc"
#undef MACRO
  ExternalObjectManagerSymbol::getSymbolAttachments(purposes, symbols);
}

bool
PrngManagerSymbol::handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  DebugEnter(message);
  Symbol* s = message->symbol();
  if (s == createPrngMsg)
    {
      createPrng(safeCastNonNull<FreeDagNode*>(message), context);
      return true;
    }
  return false;
}

bool
PrngManagerSymbol::handleMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  DebugEnter(message);
  Symbol* s = message->symbol();
  if (s == setSeedMsg)
    setSeed(safeCast(FreeDagNode*, message), context);
  else if (s == getNextMsg)
    getNext(safeCast(FreeDagNode*, message), context);
  else if (s == deletePrngMsg)
    deletePrng(safeCast(FreeDagNode*, message), context);
  else
    return false;
  return true;
}

void
PrngManagerSymbol::cleanUp(DagNode* objectId)
{
  DebugEnter(objectId);
  int id = getPrng(objectId);
  switch (prngs[id].algorithm)
    {
    case PrngAlgorithm::MT32:
      {
	delete prngs[id].mt32;
	prngs[id].mt32 = nullptr;
	break;
      }
    case PrngAlgorithm::MT64:
      {
	delete prngs[id].mt64;
	prngs[id].mt64 = nullptr;
	break;
      }
    case PrngAlgorithm::FREE:
      {
	CantHappen("algorithm is FREE");
	break;
      }
    }
  prngs[id].algorithm = PrngAlgorithm::FREE;
}
