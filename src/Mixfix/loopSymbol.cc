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
//      Implementation for class LoopSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
//#include "stringTable.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "AU_Theory.hh"
#include "NA_Theory.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "rawDagArgumentIterator.hh"
#include "rawArgumentIterator.hh"
#include "term.hh"

//      core class definitions
#include "dagArgumentIterator.hh"
#include "argumentIterator.hh"
#include "substitution.hh"
#include "rewritingContext.hh"
#include "symbolMap.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//      AU theory class definitions
#include "AU_Symbol.hh"
#include "AU_DagNode.hh"

//      built in class definitions
#include "bindingMacros.hh"

//      front end class definitions
#include "token.hh"
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierDagNode.hh"
#include "loopSymbol.hh"
#include "token.hh"

const Vector<int> LoopSymbol::eagerStrategy;

LoopSymbol::LoopSymbol(int id)
  : FreeSymbol(id, 3, eagerStrategy)
{
  qidSymbol = 0;
  nilQidListSymbol = 0;
  qidListSymbol = 0;
}

bool
LoopSymbol::attachData(const Vector<Sort*>& opDeclaration,
		       const char* purpose,
		       const Vector<const char*>& data)
{
  if (strcmp(purpose, "LoopSymbol") == 0)
    return data.empty();
  return FreeSymbol::attachData(opDeclaration, purpose, data);
}

bool
LoopSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  BIND_SYMBOL(purpose, symbol, qidSymbol, QuotedIdentifierSymbol*);
  BIND_SYMBOL(purpose, symbol, nilQidListSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, qidListSymbol, AU_Symbol*);
  return FreeSymbol::attachSymbol(purpose, symbol);
}

void
LoopSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  LoopSymbol* orig = safeCast(LoopSymbol*, original);
  COPY_SYMBOL(orig, qidSymbol, map, QuotedIdentifierSymbol*);
  COPY_SYMBOL(orig, nilQidListSymbol, map, Symbol*);
  COPY_SYMBOL(orig, qidListSymbol, map, AU_Symbol*);
  FreeSymbol::copyAttachments(original, map);
}

void
LoopSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
			       Vector<const char*>& purposes,
			       Vector<Vector<const char*> >& data)
{
  APPEND_DATA(purposes, data, LoopSymbol);
  FreeSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
LoopSymbol::getSymbolAttachments(Vector<const char*>& purposes,
				 Vector<Symbol*>& symbols)
{
  APPEND_SYMBOL(purposes, symbols, qidSymbol);
  APPEND_SYMBOL(purposes, symbols, nilQidListSymbol);
  APPEND_SYMBOL(purposes, symbols, qidListSymbol);
  FreeSymbol::getSymbolAttachments(purposes, symbols);
}

void
LoopSymbol::injectInput(DagNode* loopNode, const Vector<Token>& bubble)
{
  FreeDagNode* f = static_cast<FreeDagNode*>(loopNode);
  Vector<DagNode*> args(3);
  args[0] = createQidList(bubble);
  args[1] = f->getArgument(1);
  args[2] = new FreeDagNode(nilQidListSymbol);
  DagNode* n =  makeDagNode(args);
  //
  //    We assume that loopNode is a root with no other users so
  //    we can overwrite it in place.
  //
  n->overwriteWithClone(loopNode);
}

DagNode*
LoopSymbol::createQidList(const Vector<Token>& ids)
{
  int nrIds = ids.length();
  if (nrIds == 0)
    return new FreeDagNode(nilQidListSymbol);
  if (nrIds == 1)
    return new QuotedIdentifierDagNode(qidSymbol, ids[0].code());
  Vector<DagNode*> args(nrIds);
  for (int i = 0; i < nrIds; i++)
    args[i] = new QuotedIdentifierDagNode(qidSymbol, Token::backQuoteSpecials(ids[i].code()));
  return qidListSymbol->makeDagNode(args);
}

bool
LoopSymbol::extractOutput(DagNode* loopNode, Vector<int>& bubble)
{
  FreeDagNode* f = static_cast<FreeDagNode*>(loopNode);
  return extractQidList(f->getArgument(2), bubble);
}

bool
LoopSymbol::extractQid(DagNode* metaQid, int& id)
{
  if (metaQid->symbol() == qidSymbol)
    {
      id = Token::unBackQuoteSpecials(static_cast<QuotedIdentifierDagNode*>(metaQid)->getIdIndex());
      return true;
    }
  return false;
}

bool
LoopSymbol::extractQidList(DagNode* metaQidList, Vector<int>& ids)
{
  ids.clear();
  Symbol* mq = metaQidList->symbol();
  int id;
  if (mq == qidListSymbol)
    {
      for (DagArgumentIterator i(metaQidList); i.valid(); i.next())
	{
	  if (!extractQid(i.argument(), id))
	    return false;
	  ids.append(id);
	}
    }
  else if (extractQid(metaQidList, id))
    ids.append(id);
  else if (mq != nilQidListSymbol)
    return false;
  return true;
}
