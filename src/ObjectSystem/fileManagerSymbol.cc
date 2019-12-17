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
//      Implementation for class FileManagerSymbol.
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
#include "minusSymbol.hh"
#include "stringSymbol.hh"
#include "stringDagNode.hh"
#include "bindingMacros.hh"

//	object system class definitions
#include "objectSystemRewritingContext.hh"
#include "fileManagerSymbol.hh"

//	our stuff
#include "fileActions.cc"
#include "fileOutcomes.cc"

FileManagerSymbol::FileManagerSymbol(int id)
  : ExternalObjectManagerSymbol(id)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolName = 0;
#include "fileSignature.cc"
#undef MACRO
}

bool
FileManagerSymbol::attachData(const Vector<Sort*>& opDeclaration,
				const char* purpose,
				const Vector<const char*>& data)
{
  NULL_DATA(purpose, FileManagerSymbol, data);
  return ExternalObjectManagerSymbol::attachData(opDeclaration, purpose, data);
}

bool
FileManagerSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  Assert(symbol != 0, "null symbol for " << purpose);
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  BIND_SYMBOL(purpose, symbol, SymbolName, SymbolClass*)
#include "fileSignature.cc"
#undef MACRO
  return ExternalObjectManagerSymbol::attachSymbol(purpose, symbol);
}

void
FileManagerSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  FileManagerSymbol* orig = safeCast(FileManagerSymbol*, original);
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  COPY_SYMBOL(orig, SymbolName, map, SymbolClass*)
#include "fileSignature.cc"
#undef MACRO
  ExternalObjectManagerSymbol::copyAttachments(original, map);
}

void
FileManagerSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				      Vector<const char*>& purposes,
				      Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "FileManagerSymbol";
  data.resize(nrDataAttachments + 1);
  ExternalObjectManagerSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
FileManagerSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					  Vector<Symbol*>& symbols)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  APPEND_SYMBOL(purposes, symbols, SymbolName)
#include "fileSignature.cc"
#undef MACRO
  ExternalObjectManagerSymbol::getSymbolAttachments(purposes, symbols);
}

bool
FileManagerSymbol::handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  //cerr << "FileManagerSymbol::handleManagerMessage(): saw " << message << endl;
  Symbol* s = message->symbol();
  if (s == openFileMsg)
    return openFile(safeCast(FreeDagNode*, message), context);
  return false;
}

bool
FileManagerSymbol::handleMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  //cerr << "FileManagerSymbol::handleMessage(): saw " << message << endl;
  Symbol* s = message->symbol();
  if (s == getLineMsg)
    return getLine(safeCast(FreeDagNode*, message), context);
  if (s == getCharsMsg)
    return getChars(safeCast(FreeDagNode*, message), context);
  if (s == writeMsg)
    return write(safeCast(FreeDagNode*, message), context);
  if (s == flushMsg)
    return flush(safeCast(FreeDagNode*, message), context);
  if (s == getPositionMsg)
    return getPosition(safeCast(FreeDagNode*, message), context);
  if (s == setPositionMsg)
    return setPosition(safeCast(FreeDagNode*, message), context);
  if (s == closeFileMsg)
    return closeFile(safeCast(FreeDagNode*, message), context);
  return false;
}
