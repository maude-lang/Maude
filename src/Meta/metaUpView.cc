/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2009 SRI International, Menlo Park, CA 94025, USA.

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

DagNode*
MetaLevel::upView(View* view, PointerMap& qidMap)
{
  Vector<DagNode*> args(6);

  view->evaluate();  // in case it became stale
  args[0] = upHeader(view, qidMap);
  args[1] = upModuleExpression(view->getFrom(), qidMap);
  args[2] = upModuleExpression(view->getTo(), qidMap);
  args[3] = upSortMappings(view, qidMap);
  args[4] = upOpMappings(view, qidMap);
  args[5] = upStratMappings(view, qidMap);
  return viewSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upHeader(View* view, PointerMap& qidMap)
{
  DagNode* name = upQid(view->id(), qidMap);
  if (view->getNrParameters() == 0)
    return name;
  Vector<DagNode*> args(2);
  args[0] = name;
  args[1] = upParameterDecls(view, qidMap);
  return headerSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upParameterDecls(View* view, PointerMap& qidMap)
{
  int nrParameters = view->getNrParameters();
  if (nrParameters == 1)
    return upParameterDecl(view, 0, qidMap);
  Vector<DagNode*> args(nrParameters);
  for (int i = 0; i < nrParameters; ++i)
    args[i] = upParameterDecl(view, i, qidMap);
  return parameterDeclListSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upParameterDecl(View* view, int index, PointerMap& qidMap)
{
  Vector<DagNode*> args(2);
  args[0] = upQid(view->getParameterName(index), qidMap);
  args[1] = upModuleExpression(view->getParameterTheoryExpression(index), qidMap);
  return parameterDeclSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upSortMappings(View* view, PointerMap& qidMap)
{
  int nrSortMappings = view->getNrSortMappings();
  Vector<DagNode*> args(nrSortMappings);
  Vector<DagNode*> args2(2);
  for (int i = 0; i < nrSortMappings; ++i)
    {
      args2[0] = upQid(view->getSortFrom(i), qidMap);
      args2[1] = upQid(view->getSortTo(i), qidMap);
      args[i] = sortMappingSymbol->makeDagNode(args2);
    }
  return upGroup(args, emptySortMappingSetSymbol, sortMappingSetSymbol);
}

DagNode*
MetaLevel::upOpMappings(View* view, PointerMap& qidMap)
{
  Vector<DagNode*> args;
  Vector<DagNode*> args2;
  Vector<DagNode*> args3;

  int nrSimpleOpMappings = view->getNrOpMappings();
  for (int i = 0; i < nrSimpleOpMappings; i++)
    {
      int nrTypes = view->getNrTypes(i);
      if (nrTypes == 0)
	{
	  args2.resize(2);
	  args2[0] = upQid(view->getOpFrom(i), qidMap);
	  args2[1] = upQid(view->getOpTo(i), qidMap);
	  args.append(opMappingSymbol->makeDagNode(args2));
	}
      else
	{
	  args2.resize(4);
	  args2[0] = upQid(view->getOpFrom(i), qidMap);
	  --nrTypes;
	  args3.resize(nrTypes);
	  for (int j = 0; j < nrTypes; j++)
	    args3[j] = upTypeSorts(view->getTypeSorts(i, j), qidMap);
	  args2[1] = upGroup(args3, nilQidListSymbol, qidListSymbol);
	  args2[2] = upTypeSorts(view->getTypeSorts(i, nrTypes), qidMap);
	  args2[3] = upQid(view->getOpTo(i), qidMap);
	  args.append(opSpecificMappingSymbol->makeDagNode(args2));
	}
      }

  const View::OpTermMap& opTermMap = view->getOpTermMap();
  FOR_EACH_CONST(i, View::OpTermMap, opTermMap)
    {
      Term* fromTerm = i->second.first;
      Term* toTerm = i->second.second;
      //
      //	Note that fromTerm and toTerm were parsed in special modules
      //	belonging to the view and we need to use those modules
      //	with upTerm().
      //
      MixfixModule* fromModule = safeCast(MixfixModule*, fromTerm->symbol()->getModule());
      MixfixModule* toModule = safeCast(MixfixModule*, toTerm->symbol()->getModule());
      args2.resize(2);
      args2[0] = upTerm(fromTerm, fromModule, qidMap);
      args2[1] = upTerm(toTerm, toModule, qidMap);
      args.append(opTermMappingSymbol->makeDagNode(args2));
    }

  return upGroup(args, emptyOpMappingSetSymbol, opMappingSetSymbol);
}

DagNode*
MetaLevel::upStratMappings(View* view, PointerMap& qidMap)
{
  Vector<DagNode*> args;
  Vector<DagNode*> args2;
  Vector<DagNode*> args3;

  int nrSimpleStratMappings = view->getNrStratMappings();
  for (int i = 0; i < nrSimpleStratMappings; i++)
    {
      int nrTypes = view->getNrStratTypes(i);
      if (nrTypes == 0)
	{
	  args2.resize(2);
	  args2[0] = upQid(view->getStratFrom(i), qidMap);
	  args2[1] = upQid(view->getStratTo(i), qidMap);
	  args.append(stratMappingSymbol->makeDagNode(args2));
	}
      else
	{
	  args2.resize(4);
	  args2[0] = upQid(view->getStratFrom(i), qidMap);
	  --nrTypes;
	  args3.resize(nrTypes);
	  for (int j = 0; j < nrTypes; j++)
	    args3[j] = upTypeSorts(view->getStratTypeSorts(i, j), qidMap);
	  args2[1] = upGroup(args3, nilQidListSymbol, qidListSymbol);
	  args2[2] = upTypeSorts(view->getStratTypeSorts(i, nrTypes), qidMap);
	  args2[3] = upQid(view->getStratTo(i), qidMap);
	  args.append(stratSpecificMappingSymbol->makeDagNode(args2));
	}
      }

  const View::StratExprMap& stratExprMap = view->getStratExprMap();
  MixfixModule* toModule = view->getToModule();
  for (const auto& mapping : stratExprMap)
    {
      CallStrategy* fromCall = mapping.second.call;
      StrategyExpression* toExpr = mapping.second.value;
      //
      //	fromCall and toExpr were parsed in special modules. Unlike in
      //	upOpMappings, it is difficult to obtain the module from toExpr
      //	so we use View::getTargetModule.
      //
      MixfixModule* fromModule = safeCast(MixfixModule*,
					  fromCall->getTerm()->symbol()->getModule());
      args2.resize(2);
      args2[0] = upStratExpr(fromCall, fromModule, qidMap);
      args2[1] = upStratExpr(toExpr, toModule, qidMap);
      args.append(stratExprMappingSymbol->makeDagNode(args2));
    }

  return upGroup(args, emptyStratMappingSetSymbol, stratMappingSetSymbol);
}
