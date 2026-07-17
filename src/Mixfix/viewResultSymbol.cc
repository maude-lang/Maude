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
#include "meta.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      free class definitions
#include "freeDagNode.hh"

//      MetaLevel class definitions
#include "metaLevel.hh"

//	mixfix class definitions
#include "viewResultSymbol.hh"

ViewResultSymbol::ViewResultSymbol(int id)
  : ResultSymbol(id, 2)
{
}

bool
ViewResultSymbol::attachData(const Vector<Sort*>& opDeclaration,
			     const char* purpose,
			     const Vector<const char*>& data)
{
  if (strcmp(purpose, "ViewResultSymbol") == 0)
    return true;
  return ResultSymbol::attachData(opDeclaration, purpose, data);
}

void
ViewResultSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				     Vector<const char*>& purposes,
				     Vector<Vector<const char*>>& data)
{
  purposes.push_back("ViewResultSymbol");
  data.resize(data.size() + 1);
  ResultSymbol::getDataAttachments(opDeclaration, purposes, data);
}

View*
ViewResultSymbol::generateView(int newViewName,
			       const Vector<ImportModule*>& inputModules,
			       const Vector<int>& optionVec,
			       Interpreter* owner,
			       LineNumber lineNumber)
{
  ImportModule* generatorModule = safeCastNonNull<ImportModule*>(getModule());
  MetaLevel* metaLevel = getMetaLevel();
  PointerMap qidMap;
  DagNode* metaOptions = metaLevel->upQidList(optionVec, qidMap);
  if (cachedGeneratorOp == nullptr)
    {
      //
      //	Need to find suitable operator.
      //
      const ConnectedComponent* moduleKind = getModuleKind();
      const ConnectedComponent* qidKind = metaOptions->symbol()->rangeComponent();
      const ConnectedComponent* viewResultKind = rangeComponent();
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
		      IssueWarning(*generatorModule << ": multiple view generator operators " <<
				   QUOTE(cachedGeneratorOp) << " and " <<
				   QUOTE(s) << "in module " << QUOTE(generatorModule) << ".");
		      cachedGeneratorOp = nullptr;
		      return nullptr;
		    }
		}
	    }
	}
      if (cachedGeneratorOp == nullptr)
	{
	  IssueWarning(*generatorModule <<
		       ": didn't find suitable view generator operator in module " <<
		       QUOTE(generatorModule) << ".");
	  return nullptr;
	}
    }
  Verbose("Attempting to generate view " << Token::name(newViewName));
  //
  //	Make dag to be reduced.
  //
  Index nrArgs = cachedGeneratorOp->arity();
  Vector<DagNode*> args(nrArgs);
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
      handleMessageList(r->getArgument(1), lineNumber);
      //
      //	Deal with view? result.
      //
      DagNode* metaView = r->getArgument(0);
      if (viewFailure(metaView->symbol()))
	{
	  //
	  //	We assume view generator will have returned an appropriate warning.
	  //
	  Verbose("Failed to make generate view " << Token::name(newViewName));
	}
      else
	{
	  Verbose("Generated view " << Token::name(newViewName));
	  if (View* resultView = metaLevel->downView(metaView, owner, newViewName))
	    {
	      //
	      //	Our result module becomes a user of all the input modules and
	      //	the generator module. If any of these change, it becomes stale.
	      //
	      for (ImportModule* m : inputModules)
		m->addUser(resultView);
	      generatorModule->addUser(resultView);
	      //
	      //	We pass it all the information used to construct it, so it
	      //	can remove itself as a dependency if it becomes stale and
	      //	calls deepSelfDestruct(). This also enables us to reconstruct
	      //	the view expression that built it.
	      //
	      resultView->setGenerationInfo(generatorModule, inputModules, optionVec);
	      (void) generatorModule->unprotect();
	      return resultView;
	    }
	  IssueWarning(*cachedGeneratorOp << ": view generator " << QUOTE(cachedGeneratorOp) <<
		       " returned bad view " << QUOTE(metaView) << ".");
	}
    }
  else
    IssueWarning(*cachedGeneratorOp << ": view generator " << QUOTE(cachedGeneratorOp) <<
		 " returned bad result term " << QUOTE(result) << ".");
  (void) generatorModule->unprotect();
  return nullptr;
}
