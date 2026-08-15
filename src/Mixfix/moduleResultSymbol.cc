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
//      Implementation for class ModuleResultSymbol.
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
#include "moduleResultSymbol.hh"

ModuleResultSymbol::ModuleResultSymbol(int id)
  : ResultSymbol(id, 2)
{
}

bool
ModuleResultSymbol::attachData(const Vector<Sort*>& opDeclaration,
				  const char* purpose,
				  const Vector<const char*>& data)
{
  if (strcmp(purpose, "ModuleResultSymbol") == 0)
    return true;
  return ResultSymbol::attachData(opDeclaration, purpose, data);
}

void
ModuleResultSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
					    Vector<const char*>& purposes,
					    Vector<Vector<const char*>>& data)
{
  purposes.push_back("ModuleResultSymbol");
  data.resize(data.size() + 1);
  ResultSymbol::getDataAttachments(opDeclaration, purposes, data);
}

ImportModule*
ModuleResultSymbol::makeTransformation(int newModuleName,
				       const Vector<ImportModule*>& inputModules,
				       const Vector<int>& optionVec,
				       Interpreter* owner,
				       LineNumber lineNumber)
{
  ImportModule* transformModule = safeCastNonNull<ImportModule*>(getModule());
  MetaLevel* metaLevel = getMetaLevel();
  PointerMap qidMap;
  DagNode* metaOptions = metaLevel->upQidList(optionVec, qidMap);
  Symbol* transformOp = getTransformOp(metaOptions->symbol()->rangeComponent());
  if (transformOp == nullptr)
    return nullptr;
  Verbose("Attempting to make transformed module " << Token::name(newModuleName));
  //
  //	Make dag to be reduced.
  //
  Vector<View*> inputViews;
  DagNode* startDag = makeStartDag(inputModules, metaOptions, inputViews, qidMap);
  DebugAdvisory("Start dag = " << startDag);
  //
  //	Reduce it using user's code.
  //
  transformModule->finishFlattening();
  UserLevelRewritingContext context(startDag);
  context.reduce();
  if (UserLevelRewritingContext::aborted())
    return nullptr;
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
      //	Deal with module list.
      //
      DagNode* modules = r->getArgument(0);
      Symbol* topSymbol = modules->symbol();
      if (multipleModules(topSymbol))
	IssueWarning(*transformOp <<
		     ": expected module transformer " << QUOTE(transformOp) <<
		     " to return a single module.");
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
	      return resultModule;
	    }
	  IssueWarning(*transformOp << ": module transformer " << QUOTE(transformOp) <<
		       " returned bad module " << QUOTE(modules) << ".");
	}
    }
  else
    IssueWarning(*transformOp << ": module transformer " << QUOTE(transformOp) <<
		 " returned bad result term " << QUOTE(result) << ".");
  return nullptr;
}
