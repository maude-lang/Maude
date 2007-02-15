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

DagNode*
MetaLevel::upModule(bool flat, PreModule* pm, PointerMap& qidMap)
{
  static Vector<DagNode*> args;
  args.clear();

  args.append(upHeader(pm, qidMap));
  ImportModule* m = pm->getFlatModule();
  args.append(flat ? nilImportListSymbol->makeDagNode() : upImports(pm, qidMap));
  args.append(upSorts(flat, m, qidMap));
  args.append(upSubsortDecls(flat, m, qidMap));
  args.append(upOpDecls(flat, m, qidMap));
  args.append(upMbs(flat, m, qidMap));
  args.append(upEqs(flat, m, qidMap));
  MixfixModule::ModuleType mt = m->getModuleType();
  if (mt == MixfixModule::FUNCTIONAL_MODULE)
    return fmodSymbol->makeDagNode(args);
  else if (mt == MixfixModule::FUNCTIONAL_THEORY)
    return fthSymbol->makeDagNode(args);
  args.append(upRls(flat, m, qidMap));
  return ((mt == MixfixModule::SYSTEM_MODULE) ? modSymbol : thSymbol)->makeDagNode(args);
}

DagNode*
MetaLevel::upHeader(PreModule* pm, PointerMap& qidMap)
{
  DagNode* name = upQid(pm->id(), qidMap);
  if (pm->getNrParameters() == 0)
    return name;
  Vector<DagNode*> args(2);
  args[0] = name;
  args[1] = upParameterDecls(pm, qidMap);
  return headerSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upParameterDecls(PreModule* pm, PointerMap& qidMap)
{
  int nrParameters = pm->getNrParameters();
  if (nrParameters == 1)
    return upParameterDecl(pm, 0, qidMap);
  Vector<DagNode*> args(nrParameters);
  for (int i = 0; i < nrParameters; ++i)
    args[i] = upParameterDecl(pm, i, qidMap);
  return parameterDeclListSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upParameterDecl(PreModule* pm, int index, PointerMap& qidMap)
{
  Vector<DagNode*> args(2);
  args[0] = upQid(pm->getParameterName(index), qidMap);
  args[1] = upModuleExpression(pm->getParameter(index), qidMap);
  return parameterDeclSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upImports(PreModule* pm, PointerMap& qidMap)
{
  static Vector<DagNode*> args;
  args.clear();
  static Vector<DagNode*> args2(1);
  //
  //	Handle automatic imports.
  //
  {
    const ModuleDatabase::ImportMap& autoImports = pm->getAutoImports();
    FOR_EACH_CONST(i, ModuleDatabase::ImportMap, autoImports)
      {
	args2[0] = upQid(i->first, qidMap);
	Symbol* s = includingSymbol;
	if (i->second == ImportModule::PROTECTING)
	  s = protectingSymbol;
	else if (i->second == ImportModule::EXTENDING)
	  s = extendingSymbol;
	args.append(s->makeDagNode(args2));
      }
  }
  //
  //	Handle explicit imports.
  //
  {
    int nrImports = pm->getNrImports();
    for (int i = 0; i < nrImports; i++)
      {
	args2[0] = upModuleExpression(pm->getImport(i), qidMap);
	int mode = pm->getImportMode(i);
	Symbol* s = includingSymbol;
	if ((mode == Token::encode("pr")) || (mode == Token::encode("protecting")))
	  s = protectingSymbol;
	else if ((mode == Token::encode("ex")) || (mode == Token::encode("extending")))
	  s = extendingSymbol;
	args.append(s->makeDagNode(args2));
      }
  }
  return upGroup(args, nilImportListSymbol, importListSymbol);
}

DagNode*
MetaLevel::upModuleExpression(const ModuleExpression* e, PointerMap& qidMap)
{
  switch (e->getType())
    {
    case ModuleExpression::MODULE:
      {
	return upQid(e->getModuleName().code(), qidMap);
      }
    case ModuleExpression::SUMMATION:
      {
	const list<ModuleExpression*>& modules = e->getModules();
	Vector<DagNode*> args(modules.size());
	Vector<DagNode*>::iterator j = args.begin();
	FOR_EACH_CONST(i, list<ModuleExpression*>, modules)
	  {
	    *j = upModuleExpression(*i, qidMap);
	    ++j;
	  }
	return sumSymbol->makeDagNode(args);
      }
    case ModuleExpression::RENAMING:
      {
	Vector<DagNode*> args(2);
	args[0] = upModuleExpression(e->getModule(), qidMap);
	args[1] = upRenaming(e->getRenaming(), qidMap);
	return renamingSymbol->makeDagNode(args);
      }
    case ModuleExpression::INSTANTIATION:
      {
	Vector<DagNode*> args(2);
	args[0] = upModuleExpression(e->getModule(), qidMap);
	args[1] = upArguments(e->getArguments(), qidMap);
	return instantiationSymbol->makeDagNode(args);
      }
    default:
      CantHappen("bad module expression");
    }
  return 0;
}

DagNode*
MetaLevel::upArguments(const Vector<Token>& arguments, PointerMap& qidMap)
{
  int nrArguments = arguments.size();
  Assert(nrArguments >= 1, "no arguments");
  if (nrArguments == 1)
    return upQid(arguments[0].code(), qidMap);
  Vector<DagNode*> args(nrArguments);
  for (int i = 0; i < nrArguments; ++i)
    args[i] = upQid(arguments[i].code(), qidMap);
  return metaArgSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upRenaming(const Renaming* r, PointerMap& qidMap)
{
  Vector<DagNode*> args;
  Vector<DagNode*> args2(2);
  Vector<DagNode*> args3;
  {
    int nrSortMapings = r->getNrSortMappings();
    for (int i = 0; i < nrSortMapings; i++)
      {
	args2[0] = upQid(r->getSortFrom(i), qidMap);
	args2[1] = upQid(r->getSortTo(i), qidMap);
	args.append(sortRenamingSymbol->makeDagNode(args2));
      }
  }
  {
    int nrLabelMapings = r->getNrLabelMappings();
    for (int i = 0; i < nrLabelMapings; i++)
      {
	args2[0] = upQid(r->getLabelFrom(i), qidMap);
	args2[1] = upQid(r->getLabelTo(i), qidMap);
	args.append(labelRenamingSymbol->makeDagNode(args2));
      }
  }
  {
    int nrOpMappings = r->getNrOpMappings();
    for (int i = 0; i < nrOpMappings; i++)
      {
	int nrTypes = r->getNrTypes(i);
	if (nrTypes == 0)
	  {
	    args2.resize(3);
	    args2[0] = upQid(r->getOpFrom(i), qidMap);
	    args2[1] = upQid(r->getOpTo(i), qidMap);
	    args2[2] = upRenamingAttributeSet(r, i, qidMap);
	    args.append(opRenamingSymbol->makeDagNode(args2));
	  }
	else
	  {
	    args2.resize(5);
	    args2[0] = upQid(r->getOpFrom(i), qidMap);
	    --nrTypes;
	    if (nrTypes == 0)
	      args2[1] = nilQidListSymbol->makeDagNode();
	    else
	      {
		args3.resize(nrTypes);
		for (int j = 0; j < nrTypes; j++)
		  args3[j] = upTypeSorts(r->getTypeSorts(i, j), qidMap);
		args2[1] = (nrTypes == 1) ? args3[0] : qidListSymbol->makeDagNode(args3);
	      }
	    args2[2] = upTypeSorts(r->getTypeSorts(i, nrTypes), qidMap);
	    args2[3] = upQid(r->getOpTo(i), qidMap);
	    args2[4] = upRenamingAttributeSet(r, i, qidMap);
	    args.append(opRenamingSymbol2->makeDagNode(args2));
	  }
      }
  }
  return (args.size() > 1) ? renamingSetSymbol->makeDagNode(args) : args[0];
}

DagNode*
MetaLevel::upTypeSorts(const set<int>& sorts, PointerMap& qidMap)
{
  Assert(!sorts.empty(), "empty sort set");
  int id;
  if (sorts.size() == 1)
    id = *(sorts.begin());
  else
    {
      string fullName;
      const char* sep = "`[";
      FOR_EACH_CONST(i, set<int>, sorts)
	{
	  fullName += sep;
	  sep = "`,";
	  fullName += Token::name(*i);
	}
      fullName += "`]";
      id = Token::encode(fullName.c_str());
    }
  return upQid(id, qidMap);
}

DagNode*
MetaLevel::upRenamingAttributeSet(const Renaming* r, int index, PointerMap& qidMap)
{
  Vector<DagNode*> args;
  {
    int prec = r->getPrec(index);
    if (prec >= MixfixModule::MIN_PREC)
      {
	Vector<DagNode*> args2(1);
	args2[0] = succSymbol->makeNatDag(prec);
	args.append(precSymbol->makeDagNode(args2));
      }
  }
  {
    const Vector<int>& gather = r->getGather(index);
    if (!gather.empty())
      args.append(upGather(gather, qidMap));
  }
  {
    const Vector<int>& format = r->getFormat(index);
    if (!format.empty())
      {
	Vector<DagNode*> args2(1);
	args2[0] = upQidList(format, qidMap);
	args.append(formatSymbol->makeDagNode(args2));
      }
  }
  return upGroup(args, emptyAttrSetSymbol, attrSetSymbol);
}

DagNode*
MetaLevel::upSorts(bool flat, ImportModule* m, PointerMap& qidMap)
{
  int begin = flat ? 0 : m->getNrImportedSorts();
  return upSortSet(m->getSorts(), begin, m->getNrUserSorts() - begin, qidMap);
}

DagNode*
MetaLevel::upSubsortDecls(bool flat, ImportModule* m, PointerMap& qidMap)
{
  static Vector<DagNode*> args;
  args.clear();

  const Vector<Sort*>& sorts = m->getSorts();
  int nrUserSorts =  m->getNrUserSorts();
  for (int i = 0; i < nrUserSorts; i++)
    {
      int begin = flat ? 0 : m->getNrImportedSubsorts(i);
      const Sort* sort = sorts[i];
      const Vector<Sort*>& subsorts = sort->getSubsorts();
      int end = subsorts.length();
      if (begin < end)
	{
	  static Vector<DagNode*> args2(2);
	  args2[1] = upQid(sort->id(), qidMap);
	  for (int j = begin; j < end; j++)
	    {
	      args2[0] = upQid(subsorts[j]->id(), qidMap);
	      args.append(subsortSymbol->makeDagNode(args2));
	    }
	}
    }
  return upGroup(args, emptySubsortDeclSetSymbol, subsortDeclSetSymbol);
}

DagNode*
MetaLevel::upOpDecls(bool flat, ImportModule* m, PointerMap& qidMap)
{
  static Vector<DagNode*> args;
  args.clear();
  //
  //	Handle polymorphic operators.
  //
  {
    int nrPolymorphs = m->getNrPolymorphs();
    int begin = flat ? 0 : m->getNrImportedPolymorphs();
    for (int i = begin; i < nrPolymorphs; i++)
      args.append(upPolymorphDecl(m, i, qidMap));
  }
  //
  //	Handle non-polymorphic operators.
  //
  {
    int nrSymbols = m->getNrUserSymbols();
    for (int i = 0; i < nrSymbols; i++)
      {
	int nrOpDecls = m->getNrUserDeclarations(i);
	for (int j = flat ? 0 : m->getNrImportedDeclarations(i); j < nrOpDecls; j++)
	  args.append(upOpDecl(m, i, j, qidMap));
      }
  }
  return upGroup(args, emptyOpDeclSetSymbol, opDeclSetSymbol);
}

DagNode*
MetaLevel::upPolymorphDecl(ImportModule* m, int index, PointerMap& qidMap)
{
  static Vector<DagNode*> args(4);
  static Vector<DagNode*> polyArgs;
  polyArgs.clear();
  DagNode* universal = upQid(Token::encode("Universal"), qidMap);
  
  args[0] = upQid(m->getPolymorphName(index).code(), qidMap);
  const Vector<Sort*>& domainAndRange = m->getPolymorphDomainAndRange(index);
  int nrArgs = domainAndRange.length() - 1;
  {
    if (nrArgs == 0)
      args[1] = nilQidListSymbol->makeDagNode();
    else
      {
	static Vector<DagNode*> args2;
	args2.resize(nrArgs);
	for (int i = 0; i < nrArgs; i++)
	  {
	    Sort* s = domainAndRange[i];
	    if (s == 0)
	      {
		args2[i] = universal;
		polyArgs.append(succSymbol->makeNatDag(i + 1));
	      }
	    else
	      args2[i] = upType(s, qidMap);
	  }
	args[1] = (nrArgs == 1) ? args2[0] : qidListSymbol->makeDagNode(args2);
      }
  }
  Sort* s = domainAndRange[nrArgs];
  if (s == 0)
    {
      args[2] = universal;
      polyArgs.append(succSymbol->makeNatDag(0));
    }
  else
    args[2] = upType(s, qidMap);
  {
    static Vector<DagNode*> attrArgs;
    attrArgs.resize(1);
    Assert(polyArgs.length() > 0, "empty poly");
    if (polyArgs.length() > 1)
      {
	polyArgs[0] = natListSymbol->makeDagNode(polyArgs);
	polyArgs.resize(1);
      }
    attrArgs[0] = polySymbol->makeDagNode(polyArgs);

    const Vector<int>& strategy = m->getPolymorphStrategy(index);
    if (strategy.length() > 0)
      attrArgs.append(upStrat(strategy));

    SymbolType st = m->getPolymorphType(index);
    if (st.hasFlag(SymbolType::CTOR))
     attrArgs.append(ctorSymbol->makeDagNode());
    if (st.hasFlag(SymbolType::FROZEN))
      attrArgs.append(upFrozen(m->getPolymorphFrozen(index)));
    if (st.hasFlag(SymbolType::FORMAT))
      {
	polyArgs[0] = upQidList(m->getPolymorphFormat(index), qidMap);
	attrArgs.append(formatSymbol->makeDagNode(polyArgs));
      }
    if (st.hasFlag(SymbolType::PREC))
      {
	polyArgs[0] = succSymbol->makeNatDag(m->getPolymorphPrec(index));
	attrArgs.append(precSymbol->makeDagNode(polyArgs));
      }
    if (st.hasFlag(SymbolType::GATHER))
      {
	Vector<int> gather;
	m->getPolymorphGather(index, gather);
	attrArgs.append(upGather(gather, qidMap));
      }
    if (st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
      attrArgs.append(upIdentity(m, st, m->getPolymorphIdentity(index), qidMap));
    int metadata = m->getPolymorphMetadata(index);
    if (metadata != NONE)
      {
	Vector<DagNode*> args2(1);
	args2[0] = new StringDagNode(stringSymbol, Token::codeToRope(metadata));
	attrArgs.append(metadataSymbol->makeDagNode(args2));
      }
    if (st.hasSpecial())
      attrArgs.append(upPolymorphSpecial(index, m, qidMap));
    args[3] = upAttributeSet(st, attrArgs);
  }
  return opDeclSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upPolymorphSpecial(int index, MixfixModule* m, PointerMap& qidMap)
{
  static Vector<DagNode*> hookArgs;
  hookArgs.clear();
  int purpose;
  {
    static Vector<int> items;
    for (int i = 0; m->getPolymorphDataAttachment(index, i, purpose, items); i++)	
      hookArgs.append(upIdHook(purpose, items, qidMap));
  }
  {
    Symbol* op;
    for (int i = 0; m->getPolymorphSymbolAttachment(index, i, purpose, op); i++)
      hookArgs.append(upOpHook(purpose, op, qidMap));
  }
  {
    Term* term;
    for (int i = 0; m->getPolymorphTermAttachment(index, i, purpose, term); i++)
      hookArgs.append(upTermHook(purpose, term, m, qidMap));
  }
  Assert (hookArgs.length() > 0, "no hooks");

  static Vector<DagNode*> arg(1);
  arg[0] = (hookArgs.length() == 1) ? hookArgs[0] : hookListSymbol->makeDagNode(hookArgs);
  return specialSymbol->makeDagNode(arg);
}

DagNode*
MetaLevel::upIdHook(int purpose, const Vector<int>& items, PointerMap& qidMap)
{
  static Vector<DagNode*> args(2);
  args[0] = upQid(purpose, qidMap);
  args[1] = upQidList(items, qidMap);
  return idHookSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upOpHook(int purpose, Symbol* op, PointerMap& qidMap)
{
  static Vector<DagNode*> args(4);
  args[0] = upQid(purpose, qidMap);
  args[1] = upQid(op->id(), qidMap);

  const Vector<Sort*>& domainAndRange =
    op->getOpDeclarations()[0].getDomainAndRange();
  int nrSorts = domainAndRange.length() - 1;
  static Vector<int> ids;
  ids.resize(nrSorts);
  for (int i = 0; i < nrSorts; i++)
    ids[i] = MixfixModule::hookSort(domainAndRange[i])->id();
  args[2] = upQidList(ids, qidMap);
  args[3] = upQid(MixfixModule::hookSort(domainAndRange[nrSorts])->id(), qidMap);
  return opHookSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upTermHook(int purpose, Term* term, MixfixModule* m, PointerMap& qidMap)
{
  static Vector<DagNode*> args(2);
  args[0] = upQid(purpose, qidMap);
  args[1] = upTerm(term, m, qidMap);
  return termHookSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upOpDecl(ImportModule* m, int symbolNr, int declNr, PointerMap& qidMap)
{
  static Vector<DagNode*> args(4);

  Symbol* symbol = m->getSymbols()[symbolNr];
  args[0] = upQid(symbol->id(), qidMap);

  const OpDeclaration& d = symbol->getOpDeclarations()[declNr];
  const Vector<Sort*>& domainAndRange = d.getDomainAndRange();
  int nrArgs = domainAndRange.length() - 1;
  {
    if (nrArgs == 0)
      args[1] = args[1] = nilQidListSymbol->makeDagNode();
    else
      {
	static Vector<DagNode*> args2;
	args2.resize(nrArgs);
	for (int i = 0; i < nrArgs; i++)
	  args2[i] = upType(domainAndRange[i], qidMap);
	args[1] = (nrArgs == 1) ? args2[0] : qidListSymbol->makeDagNode(args2);
      }
  }
  args[2] = upType(domainAndRange[nrArgs], qidMap);
  {
    static Vector<DagNode*> attrArgs;
    attrArgs.clear();
    if (d.isConstructor())
      attrArgs.append(ctorSymbol->makeDagNode());

    const Vector<int>& strategy = symbol->getStrategy();
    if (strategy.length() > 0)
      attrArgs.append(upStrat(strategy));

    SymbolType st = m->getSymbolType(symbol);
    if (st.hasFlag(SymbolType::FROZEN))
      attrArgs.append(upFrozen(symbol->getFrozen()));

    static Vector<DagNode*> args3(1);
    if (st.hasFlag(SymbolType::FORMAT))
      {
	args3[0] = upQidList(m->getFormat(symbol), qidMap);
	attrArgs.append(formatSymbol->makeDagNode(args3));
      }
    if (st.hasFlag(SymbolType::PREC))
      {
	args3[0] = succSymbol->makeNatDag(m->getPrec(symbol));
	attrArgs.append(precSymbol->makeDagNode(args3));
      }
    if (st.hasFlag(SymbolType::GATHER))
      {
	Vector<int> gather;
	m->getGather(symbol, gather);
	attrArgs.append(upGather(gather, qidMap));
      }
    if (st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
      attrArgs.append(upIdentity(m, st, safeCast(BinarySymbol*, symbol)->getIdentity(), qidMap));

    int metadata = m->getMetadata(symbol, declNr);
    if (metadata != NONE)
      {
	Vector<DagNode*> args2(1);
	args2[0] = new StringDagNode(stringSymbol, Token::codeToRope(metadata));
	attrArgs.append(metadataSymbol->makeDagNode(args2));
      }
    if (st.hasSpecial())
      attrArgs.append(upSpecial(symbol, d, m, qidMap));
    args[3] = upAttributeSet(st, attrArgs);
  }
  return opDeclSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upSpecial(Symbol* symbol,
		     const OpDeclaration& decl,
		     MixfixModule* m,
		     PointerMap& qidMap)
{
  static Vector<DagNode*> hookArgs;
  hookArgs.clear();
  static Vector<const char*> purposes;
  purposes.clear();
  {
    static Vector<Vector<const char*> > data;
    data.clear();
    m->getDataAttachments(symbol, decl.getDomainAndRange(), purposes, data);
    int nrHooks = purposes.length();
    for (int i = 0; i < nrHooks; i++)
      {
	const Vector<const char*>& items = data[i];
	int nrItems = items.length();
	static Vector<int> ids;
	ids.resize(nrItems);
	for (int j = 0; j < nrItems; j++)
	  ids[j] = Token::encode(items[j]);
	hookArgs.append(upIdHook(Token::encode(purposes[i]), ids, qidMap));
      }
  }
  purposes.clear();
  {
    static Vector<Symbol*> ops;
    ops.clear();
    m->getSymbolAttachments(symbol, purposes, ops);
    int nrHooks = purposes.length();
    for (int i = 0; i < nrHooks; i++)
      hookArgs.append(upOpHook(Token::encode(purposes[i]), ops[i], qidMap));
  }
  purposes.clear();
  {
    static Vector<Term*> terms;
    terms.clear();
    m->getTermAttachments(symbol, purposes, terms);
    int nrHooks = purposes.length();
    for (int i = 0; i < nrHooks; i++)
       hookArgs.append(upTermHook(Token::encode(purposes[i]), terms[i], m, qidMap));
  }
  Assert (hookArgs.length() > 0, "no hooks");

  static Vector<DagNode*> arg(1);
  arg[0] = (hookArgs.length() == 1) ? hookArgs[0] : hookListSymbol->makeDagNode(hookArgs);
  return specialSymbol->makeDagNode(arg);
}

DagNode*
MetaLevel::upGather(const Vector<int>& gather, PointerMap& qidMap)
{
  int gatherLength = gather.length();
  static Vector<DagNode*> args;
  args.resize(gatherLength);
  for (int i = 0; i < gatherLength; i++)
    {
      const char* s;
      switch (gather[i])
	{
	case MixfixModule::GATHER_e:
	  s = "e";
	  break;
	case MixfixModule::GATHER_E:
	  s = "E";
	  break;
	case MixfixModule::GATHER_AMP:
	  s = "&";
	  break;
	}
      args[i] = (upQid(Token::encode(s), qidMap));
    }
  if (gatherLength > 1)
    {
      args[0] = qidListSymbol->makeDagNode(args);
      args.resize(1);
    }
  return gatherSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upIdentity(MixfixModule* m, SymbolType st, Term* identity, PointerMap& qidMap)
{
  Symbol* s;
  if (!(st.hasFlag(SymbolType::LEFT_ID)))
    s = rightIdSymbol;
  else if (!(st.hasFlag(SymbolType::RIGHT_ID)))
    s = leftIdSymbol;
  else
    s = idSymbol;
  static Vector<DagNode*> args(1);
  args[0] = upTerm(identity, m, qidMap);
  return s->makeDagNode(args);
}

DagNode*
MetaLevel::upStrat(const Vector<int>& strategy)
{
  static Vector<DagNode*> args;
  args.clear();
  FOR_EACH_CONST(i, Vector<int>, strategy)
    args.append(succSymbol->makeNatDag(*i));
  Assert(args.length() > 0, "empty strat");
  if (args.length() > 1)
    {
      args[0] = natListSymbol->makeDagNode(args);
      args.resize(1);
    }
  return stratSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upFrozen(const NatSet& frozen)
{
  static Vector<DagNode*> args;
  args.clear();
  FOR_EACH_CONST(i, NatSet, frozen)
    args.append(succSymbol->makeNatDag(*i + 1));
  Assert(args.length() > 0, "empty frozen");
  if (args.length() > 1)
    {
      args[0] = natListSymbol->makeDagNode(args);
      args.resize(1);
    }
  return frozenSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upAttributeSet(SymbolType st, Vector<DagNode*>& args)
{
  //
  //	Easy attributes.
  //
  if (st.hasFlag(SymbolType::ASSOC))
    args.append(assocSymbol->makeDagNode());
  if (st.hasFlag(SymbolType::COMM))
    args.append(commSymbol->makeDagNode());
  if (st.hasFlag(SymbolType::IDEM))
    args.append(idemSymbol->makeDagNode());
  if (st.hasFlag(SymbolType::ITER))
    args.append(iterSymbol->makeDagNode());
  if (st.hasFlag(SymbolType::CONFIG))
    args.append(configSymbol->makeDagNode());
  if (st.hasFlag(SymbolType::OBJECT))
    args.append(objectSymbol->makeDagNode());
  if (st.hasFlag(SymbolType::MESSAGE))
    args.append(msgSymbol->makeDagNode());
  if (st.hasFlag(SymbolType::MEMO))
    args.append(memoSymbol->makeDagNode());
  return upGroup(args, emptyAttrSetSymbol, attrSetSymbol);
}

DagNode*
MetaLevel::upMbs(bool flat, ImportModule* m, PointerMap& qidMap)
{
  const Vector<SortConstraint*>& mbs = m->getSortConstraints();
  int nrMbs = flat ? mbs.length() : m->getNrOriginalMembershipAxioms();

  static Vector<DagNode*> args;
  args.clear();
  for (int i = 0; i < nrMbs; i++)
    {
      const SortConstraint* mb = mbs[i];
      if (!(mb->isBad()))
	args.append(upMb(mb, m, qidMap));
    }
  return upGroup(args, emptyMembAxSetSymbol, membAxSetSymbol);
}

DagNode*
MetaLevel::upMb(const SortConstraint* mb, MixfixModule* m, PointerMap& qidMap)
{
  static Vector<DagNode*> args(4);

  args[0] = upTerm(mb->getLhs(), m, qidMap);
  args[1] = upType(mb->getSort(), qidMap);
  const Vector<ConditionFragment*>& condition = mb->getCondition();
  if (condition.empty())
    {
      args.resize(3);
      args[2] = upStatementAttributes(m, MixfixModule::MEMB_AX, mb, qidMap);
      return mbSymbol->makeDagNode(args);
    }
  args.resize(4);
  args[2] = upCondition(condition, m, qidMap);
  args[3] = upStatementAttributes(m, MixfixModule::MEMB_AX, mb, qidMap);
  return cmbSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upEqs(bool flat, ImportModule* m, PointerMap& qidMap)
{
  const Vector<Equation*>& equations = m->getEquations();
  int nrEquations = flat ? equations.length() : m->getNrOriginalEquations();

  static Vector<DagNode*> args;
  args.clear();
  for (int i = 0; i < nrEquations; i++)
    {
      const Equation* eq = equations[i];
      if (!(eq->isBad()))
	args.append(upEq(eq, m, qidMap));
    }
  return upGroup(args, emptyEquationSetSymbol, equationSetSymbol);
}

DagNode*
MetaLevel::upEq(const Equation* equation, MixfixModule* m, PointerMap& qidMap)
{
  static Vector<DagNode*> args(4);

  args[0] = upTerm(equation->getLhs(), m, qidMap);
  args[1] = upTerm(equation->getRhs(), m, qidMap);
  const Vector<ConditionFragment*>& condition = equation->getCondition();
  if (condition.empty())
    {
      args.resize(3);
      args[2] = upStatementAttributes(m, MixfixModule::EQUATION, equation, qidMap);
      return eqSymbol->makeDagNode(args);
    }
  args.resize(4);
  args[2] = upCondition(condition, m, qidMap);
  args[3] = upStatementAttributes(m, MixfixModule::EQUATION, equation, qidMap);
  return ceqSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upRls(bool flat, ImportModule* m, PointerMap& qidMap)
{
  const Vector<Rule*>& rules = m->getRules();
  int nrRules = flat ? rules.length() : m->getNrOriginalRules();

  static Vector<DagNode*> args;
  args.clear();
  for (int i = 0; i < nrRules; i++)
    {
      const Rule* rl = rules[i];
      if (!(rl->isBad()))
	args.append(upRl(rl, m, qidMap));
    }
  return upGroup(args, emptyRuleSetSymbol, ruleSetSymbol);
}

DagNode*
MetaLevel::upRl(const Rule* rule, MixfixModule* m, PointerMap& qidMap)
{
  static Vector<DagNode*> args(4);

  args[0] = upTerm(rule->getLhs(), m, qidMap);
  args[1] = upTerm(rule->getRhs(), m, qidMap);
  const Vector<ConditionFragment*>& condition = rule->getCondition();
  if (condition.empty())
    {
      args.resize(3);
      args[2] = upStatementAttributes(m, MixfixModule::RULE, rule, qidMap);
      return rlSymbol->makeDagNode(args);
    }
  args.resize(4);
  args[2] = upCondition(condition, m, qidMap);
  args[3] = upStatementAttributes(m, MixfixModule::RULE, rule, qidMap);
  return crlSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upStatementAttributes(MixfixModule* m,
				 MixfixModule::ItemType type,
				 const PreEquation *pe,
				 PointerMap& qidMap)
{
  Vector<DagNode*> args;

  int id = pe->getLabel().id();
  if (id != NONE)
    {
      Vector<DagNode*> args2(1);
      args2[0] = upQid(id, qidMap);
      args.append(labelSymbol->makeDagNode(args2));
    }
  int metadata = m->getMetadata(type, pe);
  if (metadata != NONE)
    {
      Vector<DagNode*> args2(1);
      args2[0] = new StringDagNode(stringSymbol, Token::codeToRope(metadata));
      args.append(metadataSymbol->makeDagNode(args2));
    }
  if (pe->isNonexec())
    args.append(nonexecSymbol->makeDagNode());
  const Equation* eq = dynamic_cast<const Equation*>(pe);
  if (eq != 0 && eq->isOwise())
    args.append(owiseSymbol->makeDagNode());
  return upGroup(args, emptyAttrSetSymbol, attrSetSymbol);
}

DagNode*
MetaLevel::upLabel(const Label& label, PointerMap& qidMap)
{
  static const char emptyString[1] = {'\0'};

  int id = label.id();
  if (id == NONE)
    id = Token::encode(emptyString);
  return upQid(id, qidMap);
}

DagNode*
MetaLevel::upCondition(const Vector<ConditionFragment*>& condition,
		       MixfixModule* m,
		       PointerMap& qidMap)
{
  static Vector<DagNode*> args;
 
  int nrFragments = condition.length();
  if (nrFragments == 1)
    return upConditionFragment(condition[0], m, qidMap);

  args.resize(nrFragments);
  for (int i = 0; i < nrFragments; i++)
    args[i] = upConditionFragment(condition[i], m, qidMap);
  return conjunctionSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upConditionFragment(const ConditionFragment* fragment,
			       MixfixModule* m,
			       PointerMap& qidMap)
{
  static Vector<DagNode*> args(2);
  Symbol* s;

  if (const EqualityConditionFragment* e =
      dynamic_cast<const EqualityConditionFragment*>(fragment))
    {
      args[0] = upTerm(e->getLhs(), m, qidMap);
      args[1] = upTerm(e->getRhs(), m, qidMap);
      s = equalityConditionSymbol;
    }
  else if (const SortTestConditionFragment* t =
	   dynamic_cast<const SortTestConditionFragment*>(fragment))
    {
      args[0] = upTerm(t->getLhs(), m, qidMap);
      args[1] = upType(t->getSort(), qidMap);
      s = sortTestConditionSymbol;
    }
  else if(const AssignmentConditionFragment* a =
	  dynamic_cast<const AssignmentConditionFragment*>(fragment))
    {
      args[0] = upTerm(a->getLhs(), m, qidMap);
      args[1] = upTerm(a->getRhs(), m, qidMap);
      s = matchConditionSymbol;
    }
  else if(const RewriteConditionFragment* r =
	  dynamic_cast<const RewriteConditionFragment*>(fragment))
    {
      args[0] = upTerm(r->getLhs(), m, qidMap);
      args[1] = upTerm(r->getRhs(), m, qidMap);
      s = rewriteConditionSymbol;
    }
  else
    CantHappen("bad condition fragment");
  return s->makeDagNode(args);
}
