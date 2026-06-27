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

TransformResultSymbol::TransformResultSymbol(int id)
  : FreeSymbol(id, 2)
{
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

void
TransformResultSymbol::flattenModule(ImportModule* m)
{
  //
  //	Both transform module and input modules maybe be generated
  //	from a module expression rather than a PreModule, so we have
  //	to do the flattening here.
  //
  //	We don't need to compile input modules, but we don't have
  //	a way of flagging that statements have been imported but not
  //	compiled so we do the full thing. If we imported the statements
  //	but didn't close the theory, we might import them a second time,
  //	if the module corresponds to a text module, or is used as an
  //	input module another time.
  //
  //	We know that all modules will have come via the module expression
  //	evaluation mechanism so signatures will already have been flattened.
  //
  if (m->getStatus() < Module::THEORY_CLOSED)
    {
      //
      //	Need to flatten in statements and compile.
      //
      m->importStatements();
      Assert(!(m->isBad()), "importStatements() unexpectedly set bad flag in " << *m);
      m->resetImports();
      //
      //	Compile  module.
      //
      m->closeTheory();
      //
      //	We don't allow reserved fresh variable names in variant
      //	equations or narrowing rules. We can't do this until statements
      //	have been compiled since it relied on VariableInfo being filled out.
      //
      m->checkFreshVariableNames();
    }
}

ImportModule*
TransformResultSymbol::makeTransformation(int newModuleName,
					  const Vector<ImportModule*>& inputModules,
					  const Vector<int>& optionVec,
					  Interpreter* owner)
{
  if (shareWith == nullptr)
    {
      IssueWarning(*this << ": transformer operator " << QUOTE(this) <<
		   " has no metalevel attached.");
      return nullptr;
    }
  ImportModule* transformModule = safeCastNonNull<ImportModule*>(getModule());
  MetaLevel* metaLevel = shareWith->getMetaLevel();
  DagNode* metaOptions = metaLevel->upQidList(optionVec);
  Symbol* transformOp = nullptr;
  if (cachedTransformOp != nullptr)
    transformOp = cachedTransformOp;
  else
    {
      //
      //	Need to find suitable operator.
      //
      const ConnectedComponent* moduleKind = domainComponent(0);
      ConnectedComponent* qidKind = metaOptions->symbol()->rangeComponent();
      ConnectedComponent* transformResultKind = rangeComponent();
      for (Symbol* s : transformModule->getSymbols())
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
			       QUOTE(s) << "in module " << QUOTE(transformModule) <<
			       ". Using " << QUOTE(cachedTransformOp));
		  break;
		}
	    }
	}
      if (transformOp == nullptr)
	{
	  IssueWarning("didn't find suitable implicit transform operator in module " <<
		       QUOTE(transformModule) << ".");
	  return nullptr;
	}
    }
  //
  //	Make dag to be reduced.
  //
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
	  flattenModule(m);
	  metaModules[index] = metaLevel->upModule(true, m, qidMap, newName);
	}
      args[0] = (inputModules.size() == 1) ? metaModules[0] :
	moduleListSymbol->makeDagNode(metaModules);
    }
  args[1] = metaOptions;
  DagNode* startDag = transformOp->makeDagNode(args);
  IssueWarning("Start dag = " << startDag);
  //
  //	Reduce it using user's code.
  //
  flattenModule(transformModule);
  UserLevelRewritingContext context(startDag);
  transformModule->protect();  // in case it gets replaced in debugger
  context.reduce();
  if (UserLevelRewritingContext::aborted())
    {
      (void) transformModule->unprotect();
      return nullptr;
    }
  DagNode* result = context.root();
  IssueWarning("result dag = " << result);
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
      //	Deal with module list.
      //
      DagNode* modules = r->getArgument(0);
      if (modules->symbol() == moduleListSymbol)
	IssueWarning("only expected one module returned");
      else if (modules->symbol() != nilModuleListSymbol)
	{
	  ImportModule* resultModule = metaLevel->downSignature(modules, owner, newModuleName);
	  if (resultModule != nullptr)
	    {
	      //
	      //	Our result module becomes a user of all the input modules and
	      //	the transform module. If any of these change, it becomes stale.
	      //	We must also add these modules to our used modules so we can
	      //	removed ourselves if we deepSelfDestruct() to avoid getting
	      //	a regretToInform() message sent to a deleted object.
	      //
	      for (ImportModule* m : inputModules)
		{
		  m->addUser(resultModule);
		  resultModule->addUsedModule(m);
		}
	      transformModule->addUser(resultModule);
	      resultModule->addUsedModule(transformModule);
	    }
	  (void) transformModule->unprotect();
	  return resultModule;
	}
    }
  (void) transformModule->unprotect();
  return nullptr;
}

void
TransformResultSymbol::handleMessage(DagNode* message)
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
  IssueWarning("Unexpected message value returned from module transformation: " <<
	       QUOTE(message));
}
