/*

    This file is part of the Maude 3 interpreter.

    Copyright 2021 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class DirectoryManagerSymbol.
//
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

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
#include "directoryManagerSymbol.hh"

//	our stuff
#include "directoryActions.cc"
#include "directoryOutcomes.cc"

bool DirectoryManagerSymbol::allowDir = false;

DirectoryManagerSymbol::DirectoryManagerSymbol(int id)
  : ExternalObjectManagerSymbol(id)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolName = 0;
#include "directorySignature.cc"
#undef MACRO
}

bool
DirectoryManagerSymbol::attachData(const Vector<Sort*>& opDeclaration,
				const char* purpose,
				const Vector<const char*>& data)
{
  NULL_DATA(purpose, DirectoryManagerSymbol, data);
  return ExternalObjectManagerSymbol::attachData(opDeclaration, purpose, data);
}

bool
DirectoryManagerSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  Assert(symbol != 0, "null symbol for " << purpose);
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  BIND_SYMBOL(purpose, symbol, SymbolName, SymbolClass*)
#include "directorySignature.cc"
#undef MACRO
  return ExternalObjectManagerSymbol::attachSymbol(purpose, symbol);
}

void
DirectoryManagerSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  DirectoryManagerSymbol* orig = safeCast(DirectoryManagerSymbol*, original);
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  COPY_SYMBOL(orig, SymbolName, map, SymbolClass*)
#include "directorySignature.cc"
#undef MACRO
  ExternalObjectManagerSymbol::copyAttachments(original, map);
}

void
DirectoryManagerSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				      Vector<const char*>& purposes,
				      Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "DirectoryManagerSymbol";
  data.resize(nrDataAttachments + 1);
  ExternalObjectManagerSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
DirectoryManagerSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					     Vector<Symbol*>& symbols)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  APPEND_SYMBOL(purposes, symbols, SymbolName)
#include "directorySignature.cc"
#undef MACRO
  ExternalObjectManagerSymbol::getSymbolAttachments(purposes, symbols);
}

bool
DirectoryManagerSymbol::handleManagerMessage(DagNode* message,
					     ObjectSystemRewritingContext& context)
{
  DebugInfo("DirectoryManagerSymbol::handleManagerMessage(): saw " << message);
  Symbol* s = message->symbol();
  if (s == openDirectoryMsg)
    openDirectory(safeCast(FreeDagNode*, message), context);
  else if (s == makeDirectoryMsg)
    makeDirectory(safeCast(FreeDagNode*, message), context);
  else if (s == removeDirectoryMsg)
    removeDirectory(safeCast(FreeDagNode*, message), context);
  else
    return false;
  return true;
}

bool
DirectoryManagerSymbol::handleMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  DebugInfo("DirectoryManagerSymbol::handleMessage(): saw " << message);
  Symbol* s = message->symbol();
  if (s == getDirectoryEntryMsg)
    getDirectoryEntry(safeCast(FreeDagNode*, message), context);
  else if (s == closeDirectoryMsg)
    closeDirectory(safeCast(FreeDagNode*, message), context);
  else
    return false;
  return true;
}
