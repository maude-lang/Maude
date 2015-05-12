/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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
//	Code to move stuff down from meta-level to object level.
//

bool
MetaLevel::downQid(DagNode* metaQid, int& id)
{
  if (metaQid->symbol() == qidSymbol)
    {
      id = Token::unBackQuoteSpecials(static_cast<QuotedIdentifierDagNode*>(metaQid)->getIdIndex());
      return true;
    }
  return false;
}

bool
MetaLevel::downOpName(DagNode* metaQid, int& id)
{
  //
  //	Like downQid() but we convert things like '`, to `, rather than ,
  //
  if (metaQid->symbol() == qidSymbol)
    {
      id = static_cast<QuotedIdentifierDagNode*>(metaQid)->getIdIndex();
      return true;
    }
  return false;
}

bool
MetaLevel::downBound(DagNode* metaBound, int& bound) const
{
  if (succSymbol->getSignedInt(metaBound, bound))
    return true;
  if (metaBound->symbol() == unboundedSymbol || succSymbol->isNat(metaBound))
    {
      bound = NONE;  // treat out of range as unbounded
      return true;
    }
  return false;
}

bool
MetaLevel::downSaturate(DagNode* metaBound, int& bound) const
{
  if (succSymbol->getSignedInt(metaBound, bound))
    return true;
  if (succSymbol->isNat(metaBound))
    {
      bound = INT_MAX;  // saturate
      return true;
    }
  return false;
}

bool
MetaLevel::downBound64(DagNode* metaBound, Int64& bound) const
{
  if (succSymbol->getSignedInt64(metaBound, bound))
    return true;
  if (metaBound->symbol() == unboundedSymbol || succSymbol->isNat(metaBound))
    {
      bound = NONE;  // treat out of range as unbounded
      return true;
    }
  return false;
}

bool
MetaLevel::downSaturate64(DagNode* metaBound, Int64& bound) const
{
  if (succSymbol->getSignedInt64(metaBound, bound))
    return true;
  if (succSymbol->isNat(metaBound))
    {
      bound = INT64_MAX;  // saturate
      return true;
    }
  return false;
}

bool
MetaLevel::downBool(DagNode* metaBool, bool& value)
{
  if (trueTerm.getTerm()->equal(metaBool))
    {
      value = true;
      return true;
    }
  else if (falseTerm.getTerm()->equal(metaBool))
    {
      value = false;
      return true;
    }
  return false;
}

bool
MetaLevel::downHeader(DagNode* metaHeader, int& id, DagNode*& metaParameterDeclList)
{
  if (metaHeader->symbol() == headerSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaHeader);
      metaParameterDeclList = f->getArgument(1);
      return downQid(f->getArgument(0), id);
    }
  metaParameterDeclList = 0;
  return downQid(metaHeader, id);
}

bool
MetaLevel::downParameterDeclList(DagNode* metaParameterDeclList, ImportModule* m)
{
  if (metaParameterDeclList == 0)
    return true;
  if (m->isTheory())
    return false;
  if (metaParameterDeclList->symbol() == parameterDeclListSymbol)
    {
      for (DagArgumentIterator i(metaParameterDeclList); i.valid(); i.next())
	{
	  if (!downParameterDecl(i.argument(), m))
	    return false;
	}
      return true;
    }
  return downParameterDecl(metaParameterDeclList, m);
}

bool
MetaLevel::downParameterDecl(DagNode* metaParameterDecl, ImportModule* m)
{
  if (metaParameterDecl->symbol() == parameterDeclSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaParameterDecl);
      int name;
      ImportModule* theory;
      if (downQid(f->getArgument(0), name) &&
	  downModuleExpression(f->getArgument(1), m, theory) &&
	  theory->isTheory())
	{
	  Token t;
	  t.tokenize(name, FileTable::META_LEVEL_CREATED);
	  Interpreter* owner = safeCast(MetaModule*, m)->getOwner();  // HACK - probably all modules should have owners
	  m->addParameter(t, owner->makeParameterCopy(name, theory));
	  return true;
	}
    }
  return false;
}

MetaModule*
MetaLevel::downModule(DagNode* metaModule, bool cacheMetaModule, Interpreter* owner)
{
  if (owner == 0)
    owner = &interpreter;  // NASTY HACK - should get default owner from metaModule rather than global variable
  /*
    Ideally we should get a symbol from metaModule, a module from symbol, and an owner from that module,
    so that if an interpreter is not given we use the one containing the metaModule's, top symbol's module.
    Currently we can't do this because only the MetaModule class supports ownership.
  */

  MetaModule* cm = cache.find(metaModule);  // BUG - could be in another interpreter
  /*
    Currently we side step this bug by not caching metaModules belonging to metaIntepreters.
   */
  if (cm != 0)
    return cm;
  Symbol* ms = metaModule->symbol();

  MixfixModule::ModuleType mt;
  if (ms == fmodSymbol)
    mt = MixfixModule::FUNCTIONAL_MODULE;
  else if (ms == fthSymbol)
    mt = MixfixModule::FUNCTIONAL_THEORY;
  else if (ms == modSymbol)
    mt = MixfixModule::SYSTEM_MODULE;
  else if (ms == thSymbol)
    mt = MixfixModule::SYSTEM_THEORY;
  else
    return 0;

  FreeDagNode* f = safeCast(FreeDagNode*, metaModule);
  int id;
  DagNode* metaParameterDeclList;
  if (downHeader(f->getArgument(0), id, metaParameterDeclList))
    {
      MetaModule* m = new MetaModule(id, mt, cacheMetaModule ? &cache : 0, owner);
      if (downParameterDeclList(metaParameterDeclList, m) &&
	  downImports(f->getArgument(1), m))
	{
	  m->importSorts();
	  if (downSorts(f->getArgument(2), m) &&
	      downSubsorts(f->getArgument(3), m))
	    {
	      m->closeSortSet();
	      if (!(m->isBad()))
		{
		  m->importOps();
		  if (downOpDecls(f->getArgument(4), m))
		    {
		      m->closeSignature();
		      m->fixUpImportedOps();
		      if (downFixUps(m) && !(m->isBad()))
			{
			  m->closeFixUps();
			  if (downMembAxs(f->getArgument(5), m) &&
			      downEquations(f->getArgument(6), m) &&
			      (mt == MixfixModule::FUNCTIONAL_MODULE ||
			       mt == MixfixModule::FUNCTIONAL_THEORY ||
			       downRules(f->getArgument(7), m)))
			    {
			      m->localStatementsComplete();
			      m->importStatements();
			      m->closeTheory();
			      m->resetImports();
			      if (cacheMetaModule)  // HACK: this should probably be done elsewhere
				cache.insert(metaModule, m);
			      //
			      //	We may have displace a module from the 
			      //	metamodule cache generating garbage in
			      //	the expression. Also there may be an
			      //	accumulation of garbage anyway from meta-meta
			      //	processing so we should tidy the (expression)
			      //	module cache regularly.
			      //
			      interpreter.destructUnusedModules();
			      return m;
			    }
			}
		    }
		}
	    }
	}
      //
      //	Put the import status flags of any modules that the
      //	metamodule (transitively) depended on in a good state.
      //
      m->resetImports();
      //
      //	Deep self destruction ensures that pointers to the doomed
      //	metamodule are removed from modules it depends on.
      //
      m->deepSelfDestruct();
      //
      //	Pulling down module expressions may have resulted in
      //	the creation of cached modules that no longer have
      //	dependents now that we failed to build the metamodule.
      //	Thus we now need to tidy the module cache.
      //	
      interpreter.destructUnusedModules();
    }
  return 0;
}

bool
MetaLevel::downImports(DagNode* metaImports, MetaModule* m)
{
  Symbol* mi = metaImports->symbol();
  if (mi == importListSymbol)
    {
      for (DagArgumentIterator i(metaImports); i.valid(); i.next())
	{
	  if (!downImport(i.argument(), m))
	    return false;
	}
    }
  else if (mi != nilImportListSymbol)
    return downImport(metaImports, m);
  return true;
}

bool
MetaLevel::downImport(DagNode* metaImport, MetaModule* m)
{
  Symbol* mi = metaImport->symbol();
  ImportModule::ImportMode mode;
  if (mi == protectingSymbol)
    mode = ImportModule::PROTECTING;
  else if (mi == extendingSymbol)
    mode = ImportModule::EXTENDING;
  else if (mi == includingSymbol)
    mode = ImportModule::INCLUDING;
  else
    return false;
  
  FreeDagNode* f = safeCast(FreeDagNode*, metaImport);
  ImportModule* im;
  if (downModuleExpression(f->getArgument(0), m, im) &&
      (im->getNrParameters() == 0 || im->parametersBound()))
    {
      m->addImport(im, mode, LineNumber(FileTable::META_LEVEL_CREATED));
      return true;
    }
  return false;
}

bool
MetaLevel::downSorts(DagNode* metaSorts, MixfixModule* m)
{
  Symbol* ms = metaSorts->symbol();
  if (ms == sortSetSymbol)
    {
      for (DagArgumentIterator i(metaSorts); i.valid(); i.next())
	{
	  if (!downSort(i.argument(), m))
	    return false;
	}
    }
  else if (ms != emptySortSetSymbol)
    return downSort(metaSorts, m);
  return true;
}

bool
MetaLevel::downSort(DagNode* metaSort, MixfixModule* m)
{
  int id;
  if (downQid(metaSort, id))
    {
      Sort* sort = m->findSort(id);
      if (sort == 0)
	{
	  Sort* sort = m->addSort(id);
	  sort->setLineNumber(FileTable::META_LEVEL_CREATED);
	}
      else
	{
	  IssueAdvisory("redeclaration of sort " << QUOTE(sort) <<
			" in meta-module " << QUOTE(m) << '.');
	}
      return true;
    }
  return false;
}

bool
MetaLevel::downNatList(DagNode* metaNatList, Vector<int>& intList)
{
  intList.clear();
  Symbol* mn =  metaNatList->symbol();
  int t;
  if (mn == natListSymbol)
    {
      for (DagArgumentIterator i(metaNatList); i.valid(); i.next())
	{
	  if (!(succSymbol->getSignedInt(i.argument(), t)))
	    return false;
	  intList.append(t);
	}
    }
  else if (succSymbol->getSignedInt(metaNatList, t))
    intList.append(t);
  else
    return false;
  return true;
}

bool
MetaLevel::downQidList(DagNode* metaQidList, Vector<int>& ids)
{
  ids.clear();
  Symbol* mq =  metaQidList->symbol();
  int id;
  if (mq == qidListSymbol)
    {
      for (DagArgumentIterator i(metaQidList); i.valid(); i.next())
	{
	  if (!downQid(i.argument(), id))
	    return false;
	  ids.append(id);
	}
    }
  else if (mq == nilQidListSymbol)
    ;
  else if (downQid(metaQidList, id))
    ids.append(id);
  else
    return false;
  return true;
}

bool
MetaLevel::downTypeList(DagNode* metaTypeList, MixfixModule* m, Vector<Sort*>& typeList)
{
  typeList.clear();
  Symbol* mt = metaTypeList->symbol();
  Sort* t;
  if (mt == qidListSymbol)
    {
      for (DagArgumentIterator i(metaTypeList); i.valid(); i.next())
	{
	  if (!downType(i.argument(), m, t))
	    return false;
	  typeList.append(t);
	}
    }
  else if (mt == nilQidListSymbol)
    ;
  else if (downType(metaTypeList, m, t))
    typeList.append(t);
  else
    return false;
  return true;
}

bool
MetaLevel::downType2(int id, MixfixModule* m, Sort*& type)
{
  switch (Token::auxProperty(id))
    {
    case Token::AUX_SORT:
    case Token::AUX_STRUCTURED_SORT:
      {
	Sort* s = m->findSort(id);
	if (s != 0)
	  {
	    type = s;
	    return true;
	  }
	IssueAdvisory("could not find sort " << QUOTE(Token::name(id)) <<
		      " in meta-module " << QUOTE(m) << '.');
	break;
      }
    case Token::AUX_KIND:
      {
	static Vector<int> sortNames;
	Token::splitKind(id, sortNames);
	int nrSortNames = sortNames.length();
	ConnectedComponent* component = 0;
	for (int i = 0; i < nrSortNames; i++)
	  {
	    int id2 = sortNames[i];
	    Sort* s = m->findSort(id2);
	    if (s == 0)
	      {
		IssueAdvisory("could not find sort " << QUOTE(Token::name(id2)) <<
			      " in meta-module " << QUOTE(m) << '.');
		return false;
	      }
	    if (component == 0)
	      component = s->component();
	    else if (component != s->component())
	      {
		IssueAdvisory("component clash in kind " <<
			      QUOTE(Token::name(id)) <<
			      " in meta-module " << QUOTE(m) << '.');
		return false;
	      }
	  }
	type = component->sort(Sort::ERROR_SORT);
	return true;
      }
    default:
      {
	IssueAdvisory("expected sort or kind, not " << QUOTE(Token::name(id)) <<
		      " in meta-module " << QUOTE(m) << '.');
	break;
      }
    }
  return false;
}

bool
MetaLevel::downType(DagNode* metaType, MixfixModule* m, Sort*& type)
{
  int id;
  return downQid(metaType, id) && downType2(id, m, type);
}

bool
MetaLevel::downComponent(DagNode* metaComponent,
			 MixfixModule* m,
			 ConnectedComponent*& component)
{
  Sort* type;
  if (metaComponent->symbol() == anyTypeSymbol)
    component = 0;
  else if (downType(metaComponent, m, type))
    component = type->component();
  else
    return false;
  return true;
}

bool
MetaLevel::downSimpleSortList(DagNode* metaSortList,
			      MixfixModule* m,
			      Vector<Sort*>& sortList)
{
  sortList.clear();
  Symbol* ms = metaSortList->symbol();
  Sort* s;
  if (ms == qidListSymbol)
    {
      for (DagArgumentIterator i(metaSortList); i.valid(); i.next())
	{
	  if (!downSimpleSort(i.argument(), m, s))
	    return false;
	  sortList.append(s);
	}
    }
  else if (ms == nilQidListSymbol)
    ;
  else if (downSimpleSort(metaSortList, m, s))
    sortList.append(s);
  else
    return false;
  return true;
}

bool
MetaLevel::downSimpleSort(DagNode* metaSort, MixfixModule* m, Sort*& sort)
{
  int id;
  if (downQid(metaSort, id))
    {
      Sort* s = m->findSort(id);
      if (s != 0)
	{
	  sort = s;
	  return true;
	}
      IssueAdvisory("could not find sort " << QUOTE(Token::name(id)) <<
		    " in meta-module " << QUOTE(m) << '.');
    }
  return false;
}

bool
MetaLevel::downSubsorts(DagNode* metaSubsorts, MixfixModule* m)
{
  Symbol* ms = metaSubsorts->symbol();
  if (ms == subsortDeclSetSymbol)
    {
      for (DagArgumentIterator i(metaSubsorts); i.valid(); i.next())
	{
	  if (!downSubsort(i.argument(), m))
	    return false;
	}
    }
  else if (ms != emptySubsortDeclSetSymbol)
    return downSubsort(metaSubsorts, m);
  return true;
}

bool
MetaLevel::downSubsort(DagNode* metaSubsort, MixfixModule* m)
{
  if (metaSubsort->symbol() == subsortSymbol)
    {
      Sort* smaller;
      Sort* bigger;
      FreeDagNode* f = static_cast<FreeDagNode*>(metaSubsort);
      if (downSimpleSort(f->getArgument(0), m, smaller) &&
	  downSimpleSort(f->getArgument(1), m, bigger))
	{
	  bigger->insertSubsort(smaller);
	  return true;
	}
    }
  return false;
}

bool
MetaLevel::downCondition(DagNode* metaCondition,
			 MixfixModule* m,
			 Vector<ConditionFragment*>& condition)
{
  condition.clear();
  Symbol* mc = metaCondition->symbol();
  ConditionFragment* cf;
  if (mc == conjunctionSymbol)
    {
      for (DagArgumentIterator i(metaCondition); i.valid(); i.next())
	{
	  if (!downConditionFragment(i.argument(), m, cf))
	    {
	      FOR_EACH_CONST(j, Vector<ConditionFragment*>, condition)
		delete *j;
	      return false;
	    }
	  condition.append(cf);
	}
    }
  else if (mc == noConditionSymbol)
    ;
  else if (downConditionFragment(metaCondition, m, cf))
    condition.append(cf);
  else
    return false;
  return true;
}

bool
MetaLevel::downConditionFragment(DagNode* metaConditionFragment,
				 MixfixModule* m,
				 ConditionFragment*& fragment)
{
  Symbol* mc = metaConditionFragment->symbol();
  if (mc == equalityConditionSymbol)
    {
      FreeDagNode* f = static_cast<FreeDagNode*>(metaConditionFragment);
      Term* l;
      Term* r;
      if (downTermPair(f->getArgument(0), f->getArgument(1), l, r, m))
	{
	  fragment = new EqualityConditionFragment(l, r);
	  return true;
	}
    }
  else if (mc == sortTestConditionSymbol)
    {
      FreeDagNode* f = static_cast<FreeDagNode*>(metaConditionFragment);
      Term* l;
      Sort* sort;
      if (downTermAndSort(f->getArgument(0), f->getArgument(1), l, sort, m))
	{
	  fragment = new SortTestConditionFragment(l, sort);
	  return true;
	}
    }
  else if (mc == matchConditionSymbol)
    {
      FreeDagNode* f = static_cast<FreeDagNode*>(metaConditionFragment);
      Term* l;
      Term* r;
      if (downTermPair(f->getArgument(0), f->getArgument(1), l, r, m))
	{
	  fragment = new AssignmentConditionFragment(l, r);
	  return true;
	}
    }
  else if (mc == rewriteConditionSymbol)
    {
      FreeDagNode* f = static_cast<FreeDagNode*>(metaConditionFragment);
      Term* l;
      Term* r;
      if (downTermPair(f->getArgument(0), f->getArgument(1), l, r, m))
	{
	  fragment = new RewriteConditionFragment(l, r);
	  return true;
	}
    }
  return false;
}

bool
MetaLevel::downStatementAttrSet(DagNode* metaAttrSet, MixfixModule* m, StatementAttributeInfo& ai)
{
  Symbol* ma = metaAttrSet->symbol();
  if (ma == attrSetSymbol)
    {
      for (DagArgumentIterator i(metaAttrSet); i.valid(); i.next())
	{
	  if (!downStatementAttr(i.argument(), m, ai))
	    return false;
	}
    }
  else if (ma != emptyAttrSetSymbol)
    return downStatementAttr(metaAttrSet, m, ai);
  return true;
}

bool
MetaLevel::downStatementAttr(DagNode* metaAttr, MixfixModule* m, StatementAttributeInfo& ai)
{
  Symbol* ma = metaAttr->symbol();
  if (ma == labelSymbol)
    {
      if (!downQid(safeCast(FreeDagNode*, metaAttr)->getArgument(0), ai.label))
	return false;
    }
  else if (ma == metadataSymbol)
    {
       DagNode* metaStr = safeCast(FreeDagNode*, metaAttr)->getArgument(0);
       if (metaStr->symbol() != stringSymbol)
	 return false;
       string str;
       Token::ropeToString(safeCast(StringDagNode*, metaStr)->getValue(), str);
       ai.metadata = Token::encode(str.c_str());
    }
  else if (ma == owiseSymbol)
    ai.flags.setFlags(OWISE);
  else if (ma == variantAttrSymbol)
    ai.flags.setFlags(VARIANT);
  else if (ma == nonexecSymbol)
    ai.flags.setFlags(NONEXEC);
  else if (ma == printSymbol && !ai.flags.getFlag(PRINT))
    {
      ai.flags.setFlags(PRINT);
      if (!downPrintList(safeCast(FreeDagNode*, metaAttr)->getArgument(0), m, ai))
	return false;
    }
  else
    return false;
  return true;
}

bool
MetaLevel::downPrintList(DagNode* metaPrintList, MixfixModule* m, StatementAttributeInfo& ai)
{
  Symbol* mp =  metaPrintList->symbol();
  if (mp == qidListSymbol)
    {
      for (DagArgumentIterator i(metaPrintList); i.valid(); i.next())
	{
	  if (!downPrintListItem(i.argument(), m, ai))
	    return false;
	}
      return true;
    }
  else if (mp == nilQidListSymbol || downPrintListItem(metaPrintList, m, ai))
    return true;
  return false;
}

bool
MetaLevel::downPrintListItem(DagNode* metaPrintListItem, MixfixModule* m, StatementAttributeInfo& ai)
{
  if (metaPrintListItem->symbol() == qidSymbol)
    {
      int id = safeCast(QuotedIdentifierDagNode*, metaPrintListItem)->getIdIndex();
      if (Token::specialProperty(id) == Token::STRING)
	{
	  ai.printNames.append(id);
	  ai.printSorts.append(0);
	  return true;
	}
      else if (Token::auxProperty(id) == Token::AUX_VARIABLE)
	{
	  int varName;
	  int sortName;
	  Token::split(id, varName, sortName);
	  Sort* sort;
	  if (downType2(sortName, m, sort))
	    {
	      ai.printNames.append(varName);
	      ai.printSorts.append(sort);
	      return true;
	    }
	  else
	    {
	      IssueAdvisory("could not find sort " << QUOTE(Token::name(sortName)) <<
			    " in meta-module \"" << QUOTE(m) << '.');
	    }
	}
    }
  return false;
}

bool
MetaLevel::downMembAxs(DagNode* metaMembAxs, MixfixModule* m)
{
  Symbol* mm = metaMembAxs->symbol();
  if (mm == membAxSetSymbol)
    {
      for (DagArgumentIterator i(metaMembAxs); i.valid(); i.next())
	{
	  if (!downMembAx(i.argument(), m))
	    return false;
	}
    }
  else if (mm != emptyMembAxSetSymbol)
    return downMembAx(metaMembAxs, m);
  return true;
}

bool
MetaLevel::downMembAx(DagNode* metaMembAx, MixfixModule* m)
{
  Symbol* mm = metaMembAx->symbol();
  if (mm == mbSymbol || mm == cmbSymbol)
    {
      FreeDagNode* f = static_cast<FreeDagNode*>(metaMembAx);
      StatementAttributeInfo ai;
      if (downStatementAttrSet(f->getArgument((mm == mbSymbol) ? 2 : 3), m, ai))
	{
	  Term* l;
	  Sort* sort;
	  if (downTermAndSort(f->getArgument(0), f->getArgument(1), l, sort, m))
	    {
	      Vector<ConditionFragment*> condition;
	      if (mm == mbSymbol || downCondition(f->getArgument(2), m, condition))
		{
		  SortConstraint* mb = new SortConstraint(ai.label, l, sort, condition);
		  if (ai.flags.getFlag(NONEXEC))
		    mb->setNonexec();
		  m->insertSortConstraint(mb);
		  if (ai.metadata != NONE)
		    m->insertMetadata(MixfixModule::MEMB_AX, mb, ai.metadata);
		  if (ai.flags.getFlag(PRINT))
		    m->insertPrintAttribute(MixfixModule::MEMB_AX, mb, ai.printNames, ai.printSorts);
		  return true;
		}
	      l->deepSelfDestruct();
	    }
	}
    }
  return false;  
}

bool
MetaLevel::downEquations(DagNode* metaEquations, MixfixModule* m)
{
  Symbol* me = metaEquations->symbol();
  if (me == equationSetSymbol)
    {
      for (DagArgumentIterator i(metaEquations); i.valid(); i.next())
	{
	  if (!downEquation(i.argument(), m))
	    return false;
	}
    }
  else if (me != emptyEquationSetSymbol)
    return downEquation(metaEquations, m);
  return true;
}

bool
MetaLevel::downEquation(DagNode* metaEquation, MixfixModule* m)
{
  Symbol* me = metaEquation->symbol();
  if (me == eqSymbol || me == ceqSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaEquation);
      StatementAttributeInfo ai;
      if (downStatementAttrSet(f->getArgument((me == eqSymbol) ? 2 : 3), m, ai))
	{
	  Term* l;
	  Term* r;
	  if (downTermPair(f->getArgument(0), f->getArgument(1), l, r, m))
	    {
	      Vector<ConditionFragment*> condition;
	      if (me == eqSymbol  || downCondition(f->getArgument(2), m, condition))
		{
		  Equation* eq = new Equation(ai.label, l, r,
					      ai.flags.getFlag(OWISE), condition);
		  if (ai.flags.getFlag(NONEXEC))
		    eq->setNonexec();
		  if (ai.flags.getFlag(VARIANT))
		    {
		      if (condition.empty())
			eq->setVariant();
		      else
			IssueAdvisory("variant attribute not allowed for conditional equation in meta-module " << QUOTE(m) << '.');
		    }
		  m->insertEquation(eq);
		  if (ai.metadata != NONE)
		    m->insertMetadata(MixfixModule::EQUATION, eq, ai.metadata);
		  if (ai.flags.getFlag(PRINT))
		    m->insertPrintAttribute(MixfixModule::EQUATION, eq, ai.printNames, ai.printSorts);
		  return true;
		}
	      l->deepSelfDestruct();
	      r->deepSelfDestruct();
	    }
	}
    }
  return false;
}

bool
MetaLevel::downRules(DagNode* metaRules, MixfixModule* m)
{
  Symbol* mr = metaRules->symbol();
  if (mr == ruleSetSymbol)
    {
      for (DagArgumentIterator i(metaRules); i.valid(); i.next())
	{
	  if (!downRule(i.argument(), m))
	    return false;
	}
    }
  else if (mr != emptyRuleSetSymbol)
    return downRule(metaRules, m);
  return true;
}

bool
MetaLevel::downRule(DagNode* metaRule, MixfixModule* m)
{
  Symbol* mr = metaRule->symbol();
  if (mr == rlSymbol || mr == crlSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaRule);
      StatementAttributeInfo ai;
      if (downStatementAttrSet(f->getArgument((mr == rlSymbol) ? 2 : 3), m, ai))
	{
	  Term* l;
	  Term* r;
	  if (downTermPair(f->getArgument(0), f->getArgument(1), l, r, m))
	    {
	      Vector<ConditionFragment*> condition;
	      if (mr == rlSymbol || downCondition(f->getArgument(2), m, condition))
		{
		  Rule* rl = new Rule(ai.label, l, r, condition);
		  if (ai.flags.getFlag(NONEXEC))
		    rl->setNonexec();
		  m->insertRule(rl);
		  if (ai.metadata != NONE)
		    m->insertMetadata(MixfixModule::RULE, rl, ai.metadata);
		  if (ai.flags.getFlag(PRINT))
		    m->insertPrintAttribute(MixfixModule::RULE, rl, ai.printNames, ai.printSorts);
		  return true;
		}
	      l->deepSelfDestruct();
	      r->deepSelfDestruct();
	    }
	}
    }
  return false;
}

bool
MetaLevel::downTermAndSort(DagNode* metaTerm ,
			   DagNode* metaSort , 
			   Term*& term ,
			   Sort*& sort ,
			   MixfixModule* m)
{
  if (downSimpleSort(metaSort, m, sort))
    {
      term = downTerm(metaTerm, m);
      if (term != 0)
	{
	  if (term->symbol()->rangeComponent() == sort->component())
	    return true;
	  IssueAdvisory("kind clash for term " << QUOTE(term) <<
			" and sort " << QUOTE(sort)  <<" in meta-module " <<
			QUOTE(m) << '.');
	  term->deepSelfDestruct();
	}
    }
  return false;
}

bool
MetaLevel::downUnificationProblem(DagNode* metaUnificationProblem,
				  Vector<Term*>& leftHandSides,
				  Vector<Term*>& rightHandSides,
				  MixfixModule* m,
				  bool makeDisjoint)
{
  leftHandSides.clear();
  rightHandSides.clear();
  Symbol* mu = metaUnificationProblem->symbol();
  Term* lhs;
  Term* rhs;
  if (mu == unificationConjunctionSymbol)
    {
      for (DagArgumentIterator i(metaUnificationProblem); i.valid(); i.next())
	{
	  if (!downUnificandPair(i.argument(), lhs, rhs, m, makeDisjoint))
	    {
	      {
		FOR_EACH_CONST(j, Vector<Term*>, leftHandSides)
		  delete *j;
	      }
	      {
		FOR_EACH_CONST(j, Vector<Term*>, rightHandSides)
		  delete *j;
	      }
	      return false;
	    }
	  leftHandSides.append(lhs);
	  rightHandSides.append(rhs);
	}
    }
  else if (downUnificandPair(metaUnificationProblem, lhs, rhs, m, makeDisjoint))
    {
      leftHandSides.append(lhs);
      rightHandSides.append(rhs);
    }
  else
    return false;
  return true;
}

bool
MetaLevel::downUnificandPair(DagNode* metaUnificandPair,
			     Term*& lhs,
			     Term*& rhs,
			     MixfixModule* m,
			     bool makeDisjoint)
{
  Symbol* mu = metaUnificandPair->symbol();
  if (mu == unificandPairSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaUnificandPair);
      lhs = downTerm(f->getArgument(0), m);
      if (lhs != 0)
	{
	  flagVariables = makeDisjoint;
	  rhs = downTerm(f->getArgument(1), m);
	  flagVariables = false;
	  if (rhs != 0)
	    {
	      if (lhs->symbol()->rangeComponent() ==
		  rhs->symbol()->rangeComponent())
		return true;
	      IssueAdvisory("kind clash for unificand pair" << QUOTE(metaUnificandPair) <<
			    " in meta-module " << QUOTE(m) << '.');
	      rhs->deepSelfDestruct();
	    }
	  lhs->deepSelfDestruct();
	}
    }
  return false;
}

bool
MetaLevel::downTermPair(DagNode* metaTerm1,
			DagNode* metaTerm2, 
			Term*& term1,
			Term*& term2,
			MixfixModule* m,
			bool makeDisjoint)
{
  term1 = downTerm(metaTerm1, m);
  if (term1 != 0)
    {
      flagVariables = makeDisjoint;
      term2 = downTerm(metaTerm2, m);
      flagVariables = false;
      if (term2 != 0)
	{
	  if (term1->symbol()->rangeComponent() ==
	      term2->symbol()->rangeComponent())
	    return true;
	  IssueAdvisory("kind clash for terms " << QUOTE(term1) <<
			" and " << QUOTE(term2)  <<" in meta-module " <<
			QUOTE(m) << '.');
	  term2->deepSelfDestruct();
	}
      term1->deepSelfDestruct();
    }
  return false;
}

Term*
MetaLevel::downTerm(DagNode* metaTerm, MixfixModule* m)
{
  Symbol* mt = metaTerm->symbol();
  int id;
  if (mt == metaTermSymbol)
    {
      FreeDagNode* f = static_cast<FreeDagNode*>(metaTerm);
      Vector<Term*> argList(0, nrPreallocatedArgs);  // pre-allocate memory for speed
      if (downOpName(f->getArgument(0), id) && downTermList(f->getArgument(1), m, argList))
	{
	  int nrArgs = argList.length();
	  if (nrArgs > 0)
	    {
	      static Vector<ConnectedComponent*> domain;
	      domain.resize(nrArgs);
	      for (int i = 0; i < nrArgs; i++)
		domain[i] = argList[i]->symbol()->rangeComponent(); // NEED TO FIX
	      Symbol* s = m->findSymbol(id, domain, 0);
	      if (s != 0)
		return s->makeTerm(argList);
	      if (Token::specialProperty(id) == Token::ITER_SYMBOL)
		{
		  mpz_class number;
		  int opName;
		  if (Token::split(id, opName, number))
		    {
		      Symbol* s = m->findSymbol(opName, domain, 0);
		      if (s != 0 && m->getSymbolType(s).hasFlag(SymbolType::ITER))
			return new S_Term(safeCast(S_Symbol*, s), number, argList[0]);
		    }
		}
	      IssueAdvisory("could not find an operator " << QUOTE(Token::name(id)) <<
			    " with appropriate domain in meta-module " << QUOTE(m) <<
			    " when trying to interprete metaterm " << QUOTE(metaTerm) << '.');
	  // cerr << metaTerm << '\n';
	  // m->showAll(cerr);
	      for (int i = 0; i < nrArgs; i++)
		argList[i]->deepSelfDestruct();
	    }
	}
    }
  else if (downQid(metaTerm, id))
    {
      switch (Token::auxProperty(id))
	{
	case Token::AUX_VARIABLE:
	  {
	    int varName;
	    int sortName;
	    Token::split(id, varName, sortName);
	    Sort* sort;
	    if (downType2(sortName, m, sort))
	      {
		VariableSymbol* symbol = safeCast(VariableSymbol*,
						  m->instantiateVariable(sort));
		if (flagVariables)
		  varName = Token::flaggedCode(varName);
		return new VariableTerm(symbol, varName);
	      }
	    IssueAdvisory("could not find sort " << QUOTE(Token::name(sortName)) <<
			  " in meta-module \"" << QUOTE(m) << '.');
	    break; 
	  }
	case Token::AUX_CONSTANT:
	  {
	    int cName;
	    int sortName;
	    Token::split(id, cName, sortName);
	    Sort* sort;
	    if (downType2(sortName, m, sort))
	      {
		ConnectedComponent* component = sort->component();
		switch (Token::specialProperty(cName))
		  {
		  case Token::QUOTED_IDENTIFIER:
		    {
		      QuotedIdentifierSymbol* s = m->findQuotedIdentifierSymbol(component);
		      if (s != 0)
			return new QuotedIdentifierTerm(s, Token::encode(Token::name(cName) + 1));
		      break;
		    }
		  case Token::STRING:
		    {
		      StringSymbol* s = m->findStringSymbol(component);
		      if (s != 0)
			return new StringTerm(s, Token::codeToRope(cName));
		      break;
		    }
		  case Token::FLOAT:
		    {
		      FloatSymbol* s = m->findFloatSymbol(component);
		      if (s != 0)
			return new FloatTerm(s, Token::codeToDouble(cName));
		      break;
		    }
		  }
		static Vector<ConnectedComponent*> dummy;
		Symbol* s = m->findSymbol(cName, dummy, component);
		if (s != 0)
		  {
		    static Vector<Term*> argList;
		    return s->makeTerm(argList);
		  }
		IssueAdvisory("could not find a constant " << QUOTE(Token::name(cName)) <<
			      " of sort " << QUOTE(sort) <<
			      " in meta-module " << QUOTE(m) << '.');
	      }
	    else
	      {
		IssueAdvisory("could not find sort " << QUOTE(Token::name(sortName)) <<
			      " in meta-module " << QUOTE(m) << '.');
	      }
	    break;
	  }
	default:
	  {
	    IssueAdvisory("did not expect " << QUOTE(Token::name(id)) <<
			  " in meta-module " << QUOTE(m) << '.');
	    break;
	  }
	}
    }
  return 0;
}

bool
MetaLevel::downTermList(DagNode* metaTermList, MixfixModule* m, Vector<Term*>& termList)
{
  termList.clear();
  Symbol* mtl = metaTermList->symbol();
  if (mtl == metaArgSymbol)
    {
      for (DagArgumentIterator i(metaTermList); i.valid(); i.next())
	{
	  Term* t = downTerm(i.argument(), m);
	  if (t == 0)
	    {
	      FOR_EACH_CONST(j, Vector<Term*>, termList)
		(*j)->deepSelfDestruct();
	      return false;
	    }
	  termList.append(t);
	}
    }
  else if (mtl == emptyTermListSymbol)
    return true;
  else
    {
      Term* t = downTerm(metaTermList, m);
      if (t == 0)
	return false;
      termList.append(t);
    }
  return true;
}

bool
MetaLevel::downSubstitution(DagNode* metaSubstitution,
			    MixfixModule* m,
			    Vector<Term*>& variables,
			    Vector<Term*>& values)
{
  variables.clear();
  values.clear();
  Symbol* ms = metaSubstitution->symbol();
  if (ms == substitutionSymbol)
    {
      for (DagArgumentIterator i(metaSubstitution); i.valid(); i.next())
	{
	  if (!downAssignment(i.argument(), m, variables, values))
	    {
	      for (int j = values.length() - 1; j >= 0; j--)
		{
		  variables[j]->deepSelfDestruct();
		  values[j]->deepSelfDestruct();
		}
	      return false;
	    }
	}
    }
  else if (ms != emptySubstitutionSymbol)
    return downAssignment(metaSubstitution, m, variables, values);
  return true;
}

bool
MetaLevel::downAssignment(DagNode* metaAssignment,
			  MixfixModule* m,
			  Vector<Term*>& variables,
			  Vector<Term*>& values)
{
  if (metaAssignment->symbol() == assignmentSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaAssignment);
      Term* variable;
      Term* value;
      if (downTermPair(f->getArgument(0), f->getArgument(1), variable, value, m))
	{
	  if (dynamic_cast<VariableTerm*>(variable))
	    {
	      variables.append(variable);
	      values.append(value);
	      return true;
	    }
	  variable->deepSelfDestruct();
	  value->deepSelfDestruct();
	}
    }
  return false;
}

bool
MetaLevel::downPrintOptionSet(DagNode* metaPrintOptionSet, int& printFlags) const
{
  printFlags = 0;
  Symbol* mp = metaPrintOptionSet->symbol();
  if (mp == printOptionSetSymbol)
    {
      for (DagArgumentIterator i(metaPrintOptionSet); i.valid(); i.next())
	{
	  if (!downPrintOption(i.argument(), printFlags))
	    return false;
	}
    }
  else if (mp != emptyPrintOptionSetSymbol)
    return downPrintOption(metaPrintOptionSet, printFlags);
  return true;
}

bool
MetaLevel::downPrintOption(DagNode* metaPrintOption, int& printFlags) const
{
  Symbol* mp = metaPrintOption->symbol();
  if (mp == mixfixSymbol)
    printFlags |= Interpreter::PRINT_MIXFIX;
  else if (mp == withParensSymbol)
    printFlags |= Interpreter::PRINT_WITH_PARENS;
  else if (mp == flatSymbol)
    printFlags |= Interpreter::PRINT_FLAT;
  else if (mp == formatPrintOptionSymbol)
    printFlags |= Interpreter::PRINT_FORMAT;
  else if (mp == numberSymbol)
    printFlags |= Interpreter::PRINT_NUMBER;
  else if (mp == ratSymbol)
    printFlags |= Interpreter::PRINT_RAT;
  else
    return false;
  return true;
}
