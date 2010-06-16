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
//      Implementation for class SocketOidSymbol.
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
//#include "rewritingContext.hh"
#include "symbolMap.hh"

//      free theory class definitions
//#include "freeNet.hh"
#include "freeDagNode.hh"

//      built in class definitions
#include "succSymbol.hh"
#include "stringSymbol.hh"
#include "stringDagNode.hh"
//#include "stringOpSymbol.hh"
#include "bindingMacros.hh"

//	object system class definitions
#include "objectSystemRewritingContext.hh"

//	socket class definitions
#include "socketManagerSymbol.hh"

//	our stuff
#include "socketStuff.cc"
#include "socketAsync.cc"
#include "socketOutcomes.cc"

SocketManagerSymbol::SocketManagerSymbol(int id)
  : ExternalObjectManagerSymbol(id)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolName = 0;
#include "socketSignature.cc"
#undef MACRO
}

bool
SocketManagerSymbol::attachData(const Vector<Sort*>& opDeclaration,
				const char* purpose,
				const Vector<const char*>& data)
{
  NULL_DATA(purpose, SocketManagerSymbol, data);
  return ExternalObjectManagerSymbol::attachData(opDeclaration, purpose, data);
}

bool
SocketManagerSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  Assert(symbol != 0, "null symbol for " << purpose);
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  BIND_SYMBOL(purpose, symbol, SymbolName, SymbolClass*)
#include "socketSignature.cc"
#undef MACRO
  return ExternalObjectManagerSymbol::attachSymbol(purpose, symbol);
}

void
SocketManagerSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  SocketManagerSymbol* orig = safeCast(SocketManagerSymbol*, original);
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  COPY_SYMBOL(orig, SymbolName, map, SymbolClass*)
#include "socketSignature.cc"
#undef MACRO
  ExternalObjectManagerSymbol::copyAttachments(original, map);
}

void
SocketManagerSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
					Vector<const char*>& purposes,
					Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "SocketManagerSymbol";
  data.resize(nrDataAttachments + 1);
  ExternalObjectManagerSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
SocketManagerSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					  Vector<Symbol*>& symbols)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  APPEND_SYMBOL(purposes, symbols, SymbolName)
#include "socketSignature.cc"
#undef MACRO
  ExternalObjectManagerSymbol::getSymbolAttachments(purposes, symbols);
}

bool
SocketManagerSymbol::handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  //cerr << "SocketManagerSymbol::handleManagerMessage(): saw " << message << endl;
  Symbol* s = message->symbol();
  if (s == createClientTcpSocketMsg)
    return createClientTcpSocket(safeCast(FreeDagNode*, message), context);
  if (s == createServerTcpSocketMsg)
    return createServerTcpSocket(safeCast(FreeDagNode*, message), context);
  return false;
}

bool
SocketManagerSymbol::handleMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  //cerr << "SocketManagerSymbol::handleMessage(): saw " << message << endl;
  Symbol* s = message->symbol();
  if (s == acceptClientMsg)
    return acceptClient(safeCast(FreeDagNode*, message), context);
  if (s == sendMsg)
    return send(safeCast(FreeDagNode*, message), context);
  if (s == receiveMsg)
    return receive(safeCast(FreeDagNode*, message), context);
  if (s == closeSocketMsg)
    return closeSocket(safeCast(FreeDagNode*, message), context);
  return false;
}
