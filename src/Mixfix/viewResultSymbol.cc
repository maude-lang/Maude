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
//      Implementation for class ViewResultSymbol.
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
#include "viewResultSymbol.hh"

ViewResultSymbol::ViewResultSymbol(int id)
  : FreeSymbol(id, 2)
{
}

bool
ViewResultSymbol::attachData(const Vector<Sort*>& opDeclaration,
				  const char* purpose,
				  const Vector<const char*>& data)
{
  if (strcmp(purpose, "ViewResultSymbol") == 0)
    return true;
  return FreeSymbol::attachData(opDeclaration, purpose, data);
}

void
ViewResultSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
					    Vector<const char*>& purposes,
					    Vector<Vector<const char*>>& data)
{
  purposes.push_back("ViewResultOpSymbol");
  data.resize(data.size() + 1);
  FreeSymbol::getDataAttachments(opDeclaration, purposes, data);
}

bool
ViewResultSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  Assert(symbol != nullptr, "null symbol for " << purpose);
#define MACRO(SymbolName, SymbolClass) \
  BIND_SYMBOL(purpose, symbol, SymbolName, SymbolClass*)
#include "viewResultSignature.cc"
#undef MACRO
  return FreeSymbol::attachSymbol(purpose, symbol);
}

void
ViewResultSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					    Vector<Symbol*>& symbols)
{
#define MACRO(SymbolName, SymbolClass) \
  APPEND_SYMBOL(purposes, symbols, SymbolName)
#include "viewResultSignature.cc"
#undef MACRO
  FreeSymbol::getSymbolAttachments(purposes, symbols);
}

void
ViewResultSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  ViewResultSymbol* orig = safeCastNonNull<ViewResultSymbol*>(original);
#define MACRO(SymbolName, SymbolClass) \
  COPY_SYMBOL(orig, SymbolName, map, SymbolClass*)
#include "viewResultSignature.cc"
#undef MACRO
  FreeSymbol::copyAttachments(original, map);
}

View*
ViewResultSymbol::generateView(int viewName,
			       const Vector<ImportModule*>& inputModules,
			       const Vector<int>& optionVec,
			       Interpreter* owner)
{
  if (shareWith == nullptr)
    {
      IssueWarning(*this << ": view result operator " << QUOTE(this) <<
		   " has no metalevel attached.");
      return nullptr;
    }
  ImportModule* generatorModule = safeCastNonNull<ImportModule*>(getModule());
  MetaLevel* metaLevel = shareWith->getMetaLevel();
  DagNode* metaOptions = metaLevel->upQidList(optionVec);
  ConnectedComponent* qidKind = metaOptions->symbol()->rangeComponent();
  if (cachedGeneratorOp == nullptr)
    {
      //
      //	Need to find suitable operator.
      //
      //	We require that we share the metalevel with an operator
      //	that takes a module as its first argument, which is most
      //	descent functions apart from the up*() family and downTerm().
      //
      const ConnectedComponent* moduleKind = shareWith->domainComponent(0);
      ConnectedComponent* viewResultKind = rangeComponent();
      for (Symbol* s : generatorModule->getSymbols())
	{
	  Index nrArgs = s->arity();
	  if (nrArgs == 3 || nrArgs == 4)
	    {
	      if (s->domainComponent(0) == moduleKind &&
		  s->domainComponent(1) == qidKind &&
		  s->domainComponent(2) == qidKind &&
		  s->rangeComponent() == viewResultKind &&
		  (nrArgs == 3 || s->domainComponent(3) == moduleKind))
		{
		  if (cachedGeneratorOp == nullptr)
		    cachedGeneratorOp = s;
		  else
		    {
		      IssueWarning("multiple view generator operators " <<
				   QUOTE(cachedGeneratorOp) << " and " <<
				   QUOTE(s) << "in module " << QUOTE(generatorModule) <<
				   ". Using " << QUOTE(cachedGeneratorOp));
		      break;
		    }
		}
	    }
	}
      if (cachedGeneratorOp == nullptr)
	{
	  IssueWarning("didn't find suitable view generator operator in module " <<
		       QUOTE(generatorModule) << ".");
	  return nullptr;
	}
    }
  Verbose("Attempting to generate view " << Token::name(viewName));
  //
  //	Make dag to be reduced.
  //
  Index nrArgs = cachedGeneratorOp->arity();
  Vector<DagNode*> args(nrArgs);
  PointerMap qidMap;
  args[0] = upModuleList(false, inputModules, qidMap);
  args[1] = metaOptions;
  args[2] = metaLevel->upModuleExpressionList(inputModules, qidMap);
  if (nrArgs == 4)
    args[3] = upModuleList(true, inputModules, qidMap);
  DagNode* startDag = cachedGeneratorOp->makeDagNode(args);
  DebugAdvisory("Start dag = " << startDag);
  //
  //	Reduce it using user's code.
  //
  generatorModule->finishFlattening();
  UserLevelRewritingContext context(startDag);
  generatorModule->protect();  // in case it gets replaced in debugger
  context.reduce();
  if (UserLevelRewritingContext::aborted())
    {
      (void) generatorModule->unprotect();
      return nullptr;
    }
  DagNode* result = context.root();
  DebugAdvisory("result dag = " << result);
  //
  //	Process result.
  //
  if (result->symbol() == this)
    {
      FreeDagNode* r = safeCastNonNull<FreeDagNode*>(result);
      //
      //	Deal with message list.
      //
      DagNode* messages = r->getArgument(1);
      if (messages->symbol() == systemMsgListSymbol)
	{
	  for (DagArgumentIterator i(messages); i.valid(); i.next())
	    handleMessage(i.argument());
	}
      else if (messages->symbol() != nilSystemMsgListSymbol)
	handleMessage(messages);
      //
      //	Deal with view? result.
      //
      DagNode* metaView = r->getArgument(0);
      if (metaView->symbol() != viewFailureSymbol)
	{
	  Verbose("Generated view " << Token::name(viewName));
	  View* resultView = metaLevel->downView(metaView, owner, viewName);
	  if (resultView != nullptr)
	    {
	      //
	      //	Our result view becomes a user of the view generator module.
	      //
	      generatorModule->addUser(resultView);
	      //
	      //	We pass it all the information used to construct it, so it
	      //	can remove itself as a dependency if it becomes stale and
	      //	calls deepSelfDestruct(). This also enables us to reconstruct
	      //	the view expression that built it.
	      //
	      resultView->setGenerationInfo(generatorModule, inputModules, optionVec);
	    }
	  (void) generatorModule->unprotect();
	  return resultView;
	}
      else
	Verbose("Failed to make generate view " << Token::name(viewName));
    }
  (void) generatorModule->unprotect();
  return nullptr;
}

void
ViewResultSymbol::handleMessage(DagNode* message)
{
  Symbol* s = message->symbol();
  DagArgumentIterator i(message);
  if (i.valid())
    {
      Rope text;
      if (shareWith->getMetaLevel()->downString(i.argument(), text))
	{
	  if (s == advisorySymbol)
	    {
	      IssueAdvisory(text);
	      return;
	    }
	  else if (s == warningSymbol)
	    {
	      IssueWarning(text);
	      return;
	    }
	  else if (s == verboseSymbol)
	    {
	      Verbose(text);
	      return;
	    }
	}
    }
  IssueWarning("Unexpected message value returned from view generation: " << QUOTE(message));
}

DagNode*
ViewResultSymbol::upModuleList(bool flat,
			       const Vector<ImportModule*>& inputModules,
			       PointerMap& qidMap)
{
  if (inputModules.empty())
    return nilModuleListSymbol->makeDagNode();
  Index index = 0;
  Vector<DagNode*> metaModules(inputModules.size());
  for (ImportModule* m : inputModules)
    {
      Rope name("I");
      name += int64ToString(index + 1);
      int newName = Token::ropeToCode(name);
      m->finishFlattening();
      metaModules[index] = metaLevel->upModule(flat, m, qidMap, newName);
    }
  return (inputModules.size() == 1) ? metaModules[0] :
    moduleListSymbol->makeDagNode(metaModules);
}
