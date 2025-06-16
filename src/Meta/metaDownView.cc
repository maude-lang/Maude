/*

    This file is part of the Maude 3 interpreter.

    Copyright 2018-2025 SRI International, Menlo Park, CA 94025, USA.

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

View*
MetaLevel::downView(DagNode* metaView, Interpreter* owner)
{
  if (metaView->symbol() == viewSymbol)
    {
      //
      //	op view_from_to_is__endv : Header ModuleExpression ModuleExpression
      //	SortMappingSet OpMappingSet StratMappingSet -> View .
      //
      FreeDagNode* f = safeCast(FreeDagNode*, metaView);
      int id;
      DagNode* metaParameterDeclList;
      if (downHeader(f->getArgument(0), id, metaParameterDeclList))
	{
	  if (Token::isValidViewName(id))
	    {
	      if (ModuleExpression* fromTheory = downModuleExpression(f->getArgument(1)))
		{
		  if (ModuleExpression* toModule = downModuleExpression(f->getArgument(2)))
		    {
		      DagNode* metaOpMappings = f->getArgument(4);
		      DagNode* metaStratMappings = f->getArgument(5);
		      Token name;
		      name.tokenize(id, FileTable::META_LEVEL_CREATED);
		      MetaView* view =
			new MetaView(name, metaOpMappings, metaStratMappings, this, owner);
		      if (downParameterDeclList(metaParameterDeclList, view))
			{
			  view->addFrom(fromTheory);
			  view->addTo(toModule);
			  if (downSortMappingSet(f->getArgument(3), view) &&
			      downOpMappingSet(metaOpMappings, view) &&
			      downStratMappingSet(metaStratMappings, view))
			    return view;
			}
		      delete view;
		    }
		  fromTheory->deepSelfDestruct();
		}
	    }
	}
    }
  return 0;
}

bool
MetaLevel::downParameterDeclList(DagNode* metaParameterDeclList, MetaView* v)
{
  if (metaParameterDeclList == 0)
    return true;
  if (metaParameterDeclList->symbol() == parameterDeclListSymbol)
    {
      for (DagArgumentIterator i(metaParameterDeclList); i.valid(); i.next())
	{
	  if (!downParameterDecl(i.argument(), v))
	    return false;
	}
      return true;
    }
  return downParameterDecl(metaParameterDeclList, v);
}

bool
MetaLevel::downParameterDecl(DagNode* metaParameterDecl, MetaView* v)
{
  if (metaParameterDecl->symbol() == parameterDeclSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaParameterDecl);
      int name;
      if (downQid(f->getArgument(0), name))
	{
	  if (ModuleExpression* expr = downModuleExpression(f->getArgument(1)))
	    {
	      v->addParameter(name, expr);
	      return true;
	    }
	}
    }
  return false;
}

bool
MetaLevel::downSortMappingSet(DagNode* metaSortMappings, View* view)
{
  Symbol* ms = metaSortMappings->symbol();
  if (ms == sortMappingSetSymbol)
    {
      for (DagArgumentIterator i(metaSortMappings); i.valid(); i.next())
	{
	  if (!downSortMapping(i.argument(), view))
	    return false;
	}
    }
  else if (ms != emptySortMappingSetSymbol)
    return downSortMapping(metaSortMappings, view);
  return true;
}

bool
MetaLevel::downSortMapping(DagNode* metaSortMapping, View* view)
{
  Symbol* ms = metaSortMapping->symbol();
  if (ms == sortMappingSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaSortMapping);
      Token from;
      Token to;
      if (downToken(f->getArgument(0), from) && downToken(f->getArgument(1), to))
	{
	  view->addSortMapping(from, to);
	  return true;
	}
    }
  return false;
}

bool
MetaLevel::downOpMappingSet(DagNode* metaOpMappings, View* view)
{
  Symbol* mo = metaOpMappings->symbol();
  if (mo == opMappingSetSymbol)
    {
      for (DagArgumentIterator i(metaOpMappings); i.valid(); i.next())
	{
	  if (!downOpMapping(i.argument(), view))
	    return false;
	}
    }
  else if (mo != emptyOpMappingSetSymbol)
    return downOpMapping(metaOpMappings, view);
  return true;
}

bool
MetaLevel::downOpMapping(DagNode* metaOpMapping, View* view)
{
  Symbol* mo = metaOpMapping->symbol();
  if (mo == opMappingSymbol)
    {
      //
      //	  op (op_to_.) : Qid Qid -> OpMapping [ctor] .
      //
      FreeDagNode* f = safeCast(FreeDagNode*, metaOpMapping);
      Vector<Token> from(1);
      Vector<Token> to(1);
      if (downToken(f->getArgument(0), from[0]) && downToken(f->getArgument(1), to[0]))
	{
	  view->addOpMapping(from);
	  view->addOpTarget(to);
	  return true;
	}
    }
  else if (mo == opSpecificMappingSymbol)
    {
      //
      //	op (op_:_->_to_.) : Qid TypeList Type Qid -> OpMapping [ctor] .
      //
      FreeDagNode* f = safeCast(FreeDagNode*, metaOpMapping);
      Vector<Token> from(1);
      Vector<Token> to(1);
      if (downToken(f->getArgument(0), from[0]) && downToken(f->getArgument(3), to[0]))
	{
	  view->addOpMapping(from);
	  view->addOpTarget(to);
	  return downRenamingTypes(f->getArgument(1), view) &&
	    downRenamingType(f->getArgument(2), view);
	}
    }
  else if (mo == opTermMappingSymbol)
    return true;  // handle these in MetaView
  return false;
}

bool
MetaLevel::downOpTermMappings(DagNode* metaOpMappings,
			      MixfixModule* fromTheory,
			      MixfixModule* toModule,
			      Vector<Term*>& fromTerms,
			      Vector<Term*>& toTerms)
{
  Symbol* mo = metaOpMappings->symbol();
  if (mo == opMappingSetSymbol)
    {
      for (DagArgumentIterator i(metaOpMappings); i.valid(); i.next())
	{
	  if (!downOpTermMapping(i.argument(), fromTheory, toModule, fromTerms, toTerms))
	    return false;
	}
    }
  else if (mo != emptyOpMappingSetSymbol)
    return downOpTermMapping(metaOpMappings, fromTheory, toModule, fromTerms, toTerms);
  return true;
}

bool
MetaLevel::downOpTermMapping(DagNode* metaOpMapping,
			     MixfixModule* fromTheory,
			     MixfixModule* toModule,
			     Vector<Term*>& fromTerms,
			     Vector<Term*>& toTerms)
{
  if (metaOpMapping->symbol() == opTermMappingSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaOpMapping);
      if (Term* lhs = downTerm(f->getArgument(0), fromTheory))
	{
	  if (Term* rhs = downTerm(f->getArgument(1), toModule))
	    {
	      fromTerms.append(lhs);
	      toTerms.append(rhs);
	      return true;
	    }
	  lhs->deepSelfDestruct();
	}
      return false;
    }
  //
  //	Because it is not an op->term mapping We assume that metaOpMapping was
  //	previously processed without issue so we don't need to check it here.
  //
  return true;
}

bool
MetaLevel::downStratMappingSet(DagNode* metaStratMappings, View* view)
{
  Symbol* mo = metaStratMappings->symbol();
  if (mo == stratMappingSetSymbol)
    {
      for (DagArgumentIterator i(metaStratMappings); i.valid(); i.next())
	{
	  if (!downStratMapping(i.argument(), view))
	    return false;
	}
    }
  else if (mo != emptyStratMappingSetSymbol)
    return downStratMapping(metaStratMappings, view);
  return true;
}

bool
MetaLevel::downStratMapping(DagNode* metaStratMapping, View* view)
{
  Symbol* mo = metaStratMapping->symbol();
  if (mo == stratMappingSymbol)
    {
      //
      //	  op (strat_to_.) : Qid Qid -> StratMapping [ctor] .
      //
      FreeDagNode* f = safeCast(FreeDagNode*, metaStratMapping);
      Token from;
      Token to;
      if (downToken(f->getArgument(0), from) && downToken(f->getArgument(1), to))
	{
	  view->addStratMapping(from);
	  view->addStratTarget(to);
	  return true;
	}
    }
  else if (mo == stratSpecificMappingSymbol)
    {
      //
      //	op (strat_:_@_to_.) : Qid TypeList Type Qid -> StratMapping [ctor] .
      //
      FreeDagNode* f = safeCast(FreeDagNode*, metaStratMapping);
      Token from;
      Token to;
      if (downToken(f->getArgument(0), from) && downToken(f->getArgument(3), to))
	{
	  view->addStratMapping(from);
	  view->addStratTarget(to);
	  return downRenamingTypes(f->getArgument(1), view) &&
	    downRenamingType(f->getArgument(2), view);
	}
    }
  else if (mo == stratExprMappingSymbol)
    return true;  // handle these in MetaView
  return false;
}

bool
MetaLevel::downStratExprMappings(DagNode* metaStratMappings,
				 MixfixModule* fromTheory,
				 MixfixModule* toModule,
				 Vector<CallStrategy*>& fromExprs,
				 Vector<StrategyExpression*>& toExprs)
{
  Symbol* mo = metaStratMappings->symbol();
  if (mo == stratMappingSetSymbol)
    {
      for (DagArgumentIterator i(metaStratMappings); i.valid(); i.next())
	{
	  if (!downStratExprMapping(i.argument(), fromTheory, toModule, fromExprs, toExprs))
	    return false;
	}
    }
  else if (mo != emptyStratMappingSetSymbol)
    return downStratExprMapping(metaStratMappings, fromTheory, toModule, fromExprs, toExprs);
  return true;
}

bool
MetaLevel::downStratExprMapping(DagNode* metaStratMapping,
				MixfixModule* fromTheory,
				MixfixModule* toModule,
				Vector<CallStrategy*>& fromExprs,
				Vector<StrategyExpression*>& toExprs)
{
  if (metaStratMapping->symbol() == stratExprMappingSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaStratMapping);
      if (CallStrategy* lhs = safeCast(CallStrategy*,
				       downStratExpr(f->getArgument(0), fromTheory)))
	{
	  if (StrategyExpression* rhs = downStratExpr(f->getArgument(1), toModule))
	    {
	      fromExprs.append(lhs);
	      toExprs.append(rhs);
	      return true;
	    }
	  delete lhs;
	}
      return false;
    }
  //
  //	Because it is not an strat->expr mapping We assume that metaStratMapping was
  //	previously processed without issue so we don't need to check it here.
  //
  return true;
}

ModuleExpression*
MetaLevel::downModuleExpression(DagNode* metaExpr)
{
  //
  //	Named module.
  //
  Token moduleName;
  if (downToken(metaExpr, moduleName))
    return new ModuleExpression(moduleName);

  Symbol* me = metaExpr->symbol();
  if (me == sumSymbol)
    {
      //
      //	<ModExpr> + ... + <ModExpr>
      //
      ModuleExpression* sum = 0;
      for (DagArgumentIterator i(metaExpr); i.valid(); i.next())
	{
	  if (ModuleExpression* summand = downModuleExpression(i.argument()))
	    sum = (sum == 0) ? summand : new ModuleExpression(sum, summand);
	  else
	    {
	      if (sum != 0)
		sum->deepSelfDestruct();
	      return 0;
	    }
	}
      return sum;
    }
  if (me == renamingSymbol)
    {
      //
      //	<ModExpr> * <Renaming>
      //
      FreeDagNode* f = safeCast(FreeDagNode*, metaExpr);
      Renaming* renaming = new Renaming();
      if (downRenamings(f->getArgument(1), renaming))
	{
	  if (ModuleExpression* renamed = downModuleExpression(f->getArgument(0)))
	    return new ModuleExpression(renamed, renaming);
	}
      delete renaming;
      return 0;
    }
  if (me == instantiationSymbol)
    {
      //
      //	Module instantiation.
      //
      FreeDagNode* f = safeCast(FreeDagNode*, metaExpr);
      if (ModuleExpression* instantiated = downModuleExpression(f->getArgument(0)))
	{
	  Vector<ViewExpression*> arguments;
	  if (downInstantiationArguments(f->getArgument(1), arguments))
	    return new ModuleExpression(instantiated, arguments);
	  instantiated->deepSelfDestruct();
	}
    }
  return 0;
}

bool
MetaLevel::downInstantiationArguments(DagNode* metaArguments, Vector<ViewExpression*>& arguments)
{
  Assert(arguments.empty(), "nonempty arg list");
  if (metaArguments->symbol() == metaArgSymbol)
    {
      for (DagArgumentIterator i(metaArguments); i.valid(); i.next())
	{
	  if (ViewExpression* argument = downViewExpression(i.argument()))
	    arguments.append(argument);
	  else
	    {
	      for (ViewExpression* ve : arguments)
		ve->deepSelfDestruct();
	      return false;
	    }
	}
      return true;
    }
  else if (ViewExpression* argument = downViewExpression(metaArguments))
    {
      arguments.append(argument);
      return true;
    }
  return false;
}

ViewExpression*
MetaLevel::downViewExpression(DagNode* metaViewExpr)
{
  Token name;
  if (downToken(metaViewExpr, name))
    {
      //
      //	Parameter or uninstantiated view.
      //
      return new ViewExpression(name);
    }
  Symbol* ma = metaViewExpr->symbol();
  if (ma == instantiationSymbol)
    {
      //
      //	View instantiation.
      //
      FreeDagNode* f = safeCast(FreeDagNode*, metaViewExpr);
      if (ViewExpression* instantiated = downViewExpression(f->getArgument(0)))
	{
	  Vector<ViewExpression*> arguments;
	  if (downInstantiationArguments(f->getArgument(1), arguments))
	    return new ViewExpression(instantiated, arguments);
	  instantiated->deepSelfDestruct();
	}
    }
  return 0;
}
