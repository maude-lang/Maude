/*

    This file is part of the Maude 2 interpreter.

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
  Vector<DagNode*> args(5);

  args[0] = upQid(view->id(), qidMap);
  args[1] = upModuleExpression(view->getFrom(), qidMap);
  args[2] = upModuleExpression(view->getTo(), qidMap);
  args[3] = upSortMappings(view, qidMap);
  args[4] = upOpMappings(view, qidMap);
  return viewSymbol->makeDagNode(args);
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
