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
#include "pointerMap.hh"

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
#include "dagArgumentIterator.hh"

//      free class definitions
#include "freeDagNode.hh"

//      AU class definitions
#include "AU_Symbol.hh"

//	built-in macros
#include "bindingMacros.hh"

//      MetaLevel class definitions
#include "metaLevelOpSymbol.hh"
#include "metaLevel.hh"

//	mixfix class definitions
#include "transformResultSymbol.hh"
//#include "quotedIdentifierSymbol.hh"

TransformResultSymbol::TransformResultSymbol(int id)
  : FreeSymbol(id, 2)
{
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

ImportModule*
TransformResultSymbol::makeTransformation(int newModuleName,
					  int opName,
					  const Vector<ImportModule*>& inputModules,
					  const Vector<int>& optionVec)
{
  if (shareWith == nullptr)
    {
      IssueWarning(*this << ": transformer operator " << QUOTE(this) <<
		   " has no metalevel attached.");
      return nullptr;
    }
  MetaLevel* metaLevel = shareWith->getMetaLevel();
  DagNode* metaOptions = metaLevel->upQidList(optionVec);
  Symbol* transformOp = nullptr;
  if (opName == NONE &&  cachedTransformOp != nullptr)
    transformOp = cachedTransformOp;
  else
    {
      //
      //	Need to find suitable operator.
      //
      const ConnectedComponent* moduleKind = domainComponent(0);
      ConnectedComponent* qidKind = metaOptions->symbol()->rangeComponent();
      ConnectedComponent* transformResultKind = rangeComponent();
      MixfixModule* m = safeCastNonNull<MixfixModule*>(getModule());
      if (opName == NONE)
	{
	  for (Symbol* s : m->getSymbols())
	    {
	      if (s->arity() == 2 &&
		  s->domainComponent(0) == moduleKind &&
		  s->domainComponent(1) == qidKind &&
		  s->rangeComponent() == transformResultKind)
		{
		  if (transformOp == nullptr)
		    transformOp = cachedTransformOp = s;
		  else
		    {
		      IssueWarning("multiple transform operators " <<
				   QUOTE(cachedTransformOp) << " and " <<
				   QUOTE(s) << ". Using " << QUOTE(cachedTransformOp));
		      break;
		    }
		}
	    }
	  if (transformOp == nullptr)
	    {
	      IssueWarning("didn't find suitable implicit transform operator in module " <<
			   QUOTE(m) << ".");
	      return nullptr;
	    }
	}
      else
	{
	  Vector<ConnectedComponent*> domainComponents(2);
	  domainComponents[0] = const_cast<ConnectedComponent*>(moduleKind);
	  domainComponents[1] = qidKind;
	  
	  transformOp = m->findSymbol(opName, domainComponents, transformResultKind);
	  if (transformOp == nullptr)
	    {
	      IssueWarning("didn't find transform operator " <<
			   QUOTE(Token::name(opName)) << " in module " <<
			   QUOTE(m) << ".");
	      return nullptr;
	    }
	}
    }

  Vector<DagNode*> args(2);
  if (inputModules.empty())
    args[0] = nilModuleListSymbol->makeDagNode();
  else
    {
      PointerMap qidMap;
      Index index = 0;
      Vector<DagNode*> metaModules(inputModules.size());
      for (ImportModule* m : inputModules)
	{
	  Rope name("I");
	  name += int64ToString(index + 1);
	  int newName = Token::ropeToCode(name);
	  metaModules[index] = metaLevel->upModule(true, m, qidMap, newName);
	}
      args[0] = (inputModules.size() == 1) ? metaModules[0] :
	moduleListSymbol->makeDagNode(metaModules);
    }
  args[1] = metaOptions;
  DagNode* startDag = transformOp->makeDagNode(args);
  IssueWarning("Start dag = " << startDag);
  UserLevelRewritingContext context(startDag);
  context.reduce();
  if (UserLevelRewritingContext::aborted())
    return nullptr;
  DagNode* result = context.root();
  IssueWarning("result dag = " << result);
  if (result->symbol() == this)
    {
      FreeDagNode* r = safeCastNonNull<FreeDagNode*>(result);
      //
      //	Deal with message list.
      //
      DagNode* messages = r->getArgument(1);
      if (messages->symbol() == transformMessageListSymbol)
	{
	  for (DagArgumentIterator i(messages); i.valid(); i.next())
	    handleMessage(i.argument());
	}
      else if (messages->symbol() != nilTransformMessageListSymbol)
	handleMessage(messages);
      //
      //	Deal with module list.
      //
      DagNode* modules = r->getArgument(0);
      if (modules->symbol() == moduleListSymbol)
	{
	  IssueWarning("only expected one module returned");
	  return nullptr;
	}
      else if (modules->symbol() == nilModuleListSymbol)
	return nullptr;  // normal failure
      
    }
  return nullptr;
}

void
TransformResultSymbol::handleMessage(DagNode* message)
{
  Symbol* s = message->symbol();
  if (s == advisorySymbol)
    {
      DagNode* m = safeCastNonNull<FreeDagNode*>(message)->getArgument(0);
      IssueAdvisory(m);
    }
  else if (s == warningSymbol)
    {
      DagNode* m = safeCastNonNull<FreeDagNode*>(message)->getArgument(0);
      IssueWarning(m);
    }
  else if (s == verboseSymbol)
    {
      DagNode* m = safeCastNonNull<FreeDagNode*>(message)->getArgument(0);
      Verbose(m);
    }
}
