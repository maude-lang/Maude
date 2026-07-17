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
#include "meta.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      free class definitions
#include "freeDagNode.hh"

//      AU class definitions
#include "AU_Symbol.hh"

//      MetaLevel class definitions
#include "metaLevel.hh"

//	mixfix class definitions
#include "transformResultSymbol.hh"

TransformResultSymbol::TransformResultSymbol(int id)
  : ResultSymbol(id, 2)
{
}

bool
TransformResultSymbol::attachData(const Vector<Sort*>& opDeclaration,
				  const char* purpose,
				  const Vector<const char*>& data)
{
  if (strcmp(purpose, "TransformResultSymbol") == 0)
    return true;
  return ResultSymbol::attachData(opDeclaration, purpose, data);
}

void
TransformResultSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
					    Vector<const char*>& purposes,
					    Vector<Vector<const char*>>& data)
{
  purposes.push_back("TransformResultSymbol");
  data.resize(data.size() + 1);
  ResultSymbol::getDataAttachments(opDeclaration, purposes, data);
}

ImportModule*
TransformResultSymbol::makeTransformation(int newModuleName,
					  const Vector<ImportModule*>& inputModules,
					  const Vector<int>& optionVec,
					  Interpreter* owner)
{
  ImportModule* transformModule = safeCastNonNull<ImportModule*>(getModule());
  MetaLevel* metaLevel = getMetaLevel();
  PointerMap qidMap;
  DagNode* metaOptions = metaLevel->upQidList(optionVec, qidMap);
  const ConnectedComponent* qidKind = metaOptions->symbol()->rangeComponent();
  if (cachedTransformOp == nullptr)
    {
      //
      //	Need to find suitable operator.
      //
      const ConnectedComponent* moduleKind = domainComponent(0);
      const ConnectedComponent* transformResultKind = rangeComponent();
      for (Symbol* s : transformModule->getSymbols())
	{
	  if (s->arity() == 2 && s->rangeComponent() == transformResultKind)
	    {
	      if ((s->domainComponent(0) == moduleKind &&
		  s->domainComponent(1) == qidKind) ||
		  (s->domainComponent(0) == qidKind &&
		   s->domainComponent(1) == moduleKind))
		{
		  if (cachedTransformOp == nullptr)
		    cachedTransformOp = s;
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
	}
      if (cachedTransformOp == nullptr)
	{
	  IssueWarning("didn't find suitable module transform operator in module " <<
		       QUOTE(transformModule) << ".");
	  return nullptr;
	}
    }
  Verbose("Attempting to make transformed module " << Token::name(newModuleName));
  //
  //	Make dag to be reduced.
  //
  //	Put options first to use flattened modules.
  //
  bool flat = cachedTransformOp->domainComponent(0) == qidKind;
  Vector<DagNode*> args(2);
  args[0] = upModuleList(flat, inputModules, qidMap);
  args[1] = metaOptions;
  if (flat)
    swap(args[0], args[1]);
  DagNode* startDag = cachedTransformOp->makeDagNode(args);
  DebugAdvisory("Start dag = " << startDag);
  //
  //	Reduce it using user's code.
  //
  transformModule->finishFlattening();
  UserLevelRewritingContext context(startDag);
  transformModule->protect();  // in case it gets replaced in debugger
  context.reduce();
  if (UserLevelRewritingContext::aborted())
    {
      (void) transformModule->unprotect();
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
      handleMessageList(r->getArgument(1));
      //
      //	Deal with module list.
      //
      DagNode* modules = r->getArgument(0);
      Symbol* topSymbol = modules->symbol();
      if (multipleModules(topSymbol))
	IssueWarning("expected module transformer to return a single module.");
      else if (noModules(topSymbol))
	{
	  //
	  //	We assume module transformer will have returned an appropriate warning.
	  //
	  Verbose("Failed to make transformed module " << Token::name(newModuleName));
	}
      else 
	{
	  Verbose("Made transformed module " << Token::name(newModuleName));
	  if (ImportModule* resultModule =
	      metaLevel->downSignature(modules,
				       owner,
				       ImportModule::TRANSFORMATION,
				       newModuleName))
	    {
	      //
	      //	Our result module becomes a user of all the input modules and
	      //	the transform module. If any of these change, it becomes stale.
	      //
	      for (ImportModule* m : inputModules)
		m->addUser(resultModule);
	      transformModule->addUser(resultModule);
	      //
	      //	We pass it all the information used to construct it, so it
	      //	can remove itself as a dependency if it becomes stale and
	      //	calls deepSelfDestruct(). This also enables us to reconstruct
	      //	the module expression that built it.
	      //
	      resultModule->setTransformInfo(transformModule, inputModules, optionVec);
	      (void) transformModule->unprotect();
	      return resultModule;
	    }
	  IssueWarning("module transformer returned bad module.");
	}
    }
  else
    IssueWarning("module generator returned bad result term.");
  (void) transformModule->unprotect();
  return nullptr;
}
