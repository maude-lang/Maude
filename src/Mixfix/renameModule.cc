/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2025 SRI International, Menlo Park, CA 94025, USA.

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

ImportModule*
ImportModule::makeRenamedCopy(int name, Renaming* canonical, ModuleCache* moduleCache)
{
  ImportModule* copy = new ImportModule(name, getModuleType(), RENAMING, moduleCache);
  DebugAdvisory("makeRenamedCopy() made " << copy << " at address " << static_cast<void*>(copy));
  LineNumber lineNumber(FileTable::AUTOMATIC);

  Index nrParameters = parameterNames.size();
  for (Index i = 0; i < nrParameters; ++i)
    {
      Token t;
      t.tokenize(parameterNames[i], FileTable::AUTOMATIC);
      copy->addParameter(t, parameterTheories[i]);
   }
  copy->copyBoundParameters(this);
  //
  //	We only rename regular imports, not parameter theory copies.
  //
  for (ImportModule* i : importedModules)
    {
      if (ImportModule* importCopy = moduleCache->makeRenamedCopy(i, canonical))
	copy->addImport(importCopy, INCLUDING, lineNumber);  // HACK should this be INCLUDING?
      else
	{
	  //
	  //	One of our imports failed its renaming. Abort construction of the
	  //	renamed copy and mark it as bad. Let the module cache handle the
	  //	clean up.
	  //
	  copy->markAsBad();
	  return copy;
	}
    }
  finishCopy(copy, canonical);
  return copy;
}

void
ImportModule::finishCopy(ImportModule* copy, Renaming* canonical)
{
  Assert(!isBad(), "original module bad " << this);
  copy->canonicalRenaming = canonical;
  copy->baseModule = this;
  addUser(copy);
  //
  //	Copy our local contents, renaming it as we go, and flattening
  //	in our imports (but not statements).
  //
  copy->importSorts();
  donateSorts2(copy, canonical);
  copy->closeSortSet();
  
  DebugAdvisory("finishCopy() done with sorts - from " << this << " to " << copy <<
		" copy->isBad() = " << copy->isBad());
  if (!(copy->isBad()))
    {
      copy->importOps();
      donateOps2(copy, canonical);
      DebugAdvisory("finishCopy() done with ops - from " << this << " to " << copy <<
		    " copy->isBad() = " << copy->isBad());
      if (!(copy->isBad()))
	{
	  copy->closeSignature();
	  //
	  // Copy strategies
	  //
	  copy->importStrategies();
	  donateStrategies2(copy, canonical);
	  copy->fixUpImportedOps();
	  fixUpDonatedOps2(copy, canonical);
	  DebugAdvisory("finishCopy() done with fixups - from " << this << " to " << copy <<
			" copy->isBad() = " << copy->isBad());
	  if (!(copy->isBad()))
	    {
	      copy->closeFixUps();
	      //
	      //	Statements are not copied so we have no local statements.
	      //
	      copy->localStatementsComplete();
	    }
	}
    }
  //
  //	Copy rule labels
  //
  copy->importRuleLabels();
  donateRuleLabels(copy, canonical);
  //
  //	Reset phase counter in each imported module and return copy.
  //
  copy->resetImports();
}

Sort*
ImportModule::localSort(ImportModule* copy, Renaming* renaming, const Sort* sort)
{
  return (sort->index() == Sort::KIND) ?
    localSort2(copy, renaming, sort->component()->sort(Sort::FIRST_USER_SORT))->component()->sort(Sort::KIND) :
    localSort2(copy, renaming, sort);
}

Sort*
ImportModule::localSort2(ImportModule* copy, Renaming* renaming, const Sort* sort)
{
  int id = sort->id();
  if (renaming != 0)
    id = renaming->renameSort(id);
  Sort* ts = copy->findSort(id);
  AlwaysAssert(ts != 0, "couldn't find sort " << QUOTE(Token::sortName(id)) <<
	       " renamed from " << QUOTE(sort) << " in module " << QUOTE(copy));
  return ts;
}

void
ImportModule::donateSorts2(ImportModule* copy, Renaming* renaming)
{
  Assert(!isBad(), "original module bad " << this);
  //
  //	Donate our sorts, after a possible renaming.
  //
  bool moduleDonatingToTheory = copy->isTheory() && !isTheory();
  const Vector<Sort*>& sorts = getSorts();
  for (Index i = nrImportedSorts; i < nrUserSorts; ++i)
    {
      Sort* original = sorts[i];
      int id = original->id();
      if (renaming != 0)
	id = renaming->renameSort(id);
      Sort* sort = copy->findSort(id);
      if (sort == 0)
	{
	  sort = copy->addSort(id);
	  sort->setLineNumber(original->getLineNumber());
	}
      else
	{
	  if (copy->isTheory() &&
	      (moduleDonatingToTheory != copy->sortDeclaredInModule.contains(sort->getIndexWithinModule())))
	    {
	      //
	      //	A theory is getting the same sort from a module and a theory.
	      //	This is a nasty situation that can cause various inconsistancies
	      //	down the road since sorts from modules are handled differently
	      //	to sorts from theories; so we handle it harshly.
	      //
	      IssueWarning(*copy << ": sort " << QUOTE(sort) <<
			    " has been imported from both " << *original <<
			   " and " << *sort <<
			   ". Since it is imported from both a module and a theory, this renders theory " <<
			   QUOTE(copy) << " unusable.");
	      copy->markAsBad();
	    }
	  else
	    {
	      IssueAdvisory(*copy << ": sort " << QUOTE(original) <<
			    " has been imported from both " << *original <<
			    " and " << *sort << '.');
	    }
	}
      //
      //	As of 5/2/19 if we are a module, we tell the copy that the sort was declared
      //	in a module, even if the copy is also a module, and not just in the
      //	moduleDonatingToTheory case.
      //
      if (!isTheory())
	copy->sortDeclaredInModule.insert(sort->getIndexWithinModule());
    }
  //
  //	Donate our subsort relations, after a possible renaming.
  //
  for (Index i = 0; i < nrUserSorts; ++i)
    {
      Index nrImports = getNrImportedSubsorts(i);
      const Sort* s = sorts[i];
      const Vector<Sort*>& subsorts = s->getSubsorts();
      Index nrSubsorts = subsorts.size();
      if (nrSubsorts > nrImports)
	{
	  Sort* ts = localSort2(copy, renaming, s);
	  for (Index j = nrImports; j < nrSubsorts; ++j)
	    ts->insertSubsort(localSort2(copy, renaming, subsorts[j]));
	}
    }
}

void
ImportModule::donateOps2(ImportModule* copy, Renaming* renaming)
{
  DebugOld("donateOps2(), from " << this << " to " << copy);
  Assert(!isBad(), "original module bad " << this);

  bool moduleDonatingToTheory = copy->isTheory() && !isTheory();

  Vector<int> gather;
  Vector<Sort*> domainAndRange;
  Vector<int> emptyStrategy;
  //
  //	Handle our regular operators.
  //
  const Vector<Symbol*>& symbols = getSymbols();
  for (Index i = 0; i < nrUserSymbols; ++i)
    {
      int nrImportedDeclarations = getNrImportedDeclarations(i);
      int nrUserDeclarations = nrUserDecls[i];
      if (nrUserDeclarations > nrImportedDeclarations)
	{
	  //
	  //	Need to donate some declarations for this symbol.
	  //
	  Symbol* symbol = symbols[i];
	  SymbolType symbolType = getSymbolType(symbol);
	  Assert(!(symbolType.isCreatedOnTheFly()),
		 "unexpected variable/sort test/polymorph instance " << symbol);

	  Token name;
	  int prec = DEFAULT;
	  const Vector<int>* format;
	  int latexMacro;
	  int rpo;

	  int index = (renaming == 0) ? NONE : renaming->renameOp(symbol);  // index of renaming that applies to symbol
	  DebugOld("index = " << index);
	  if (index == NONE)
	    {
	      name.tokenize(symbol->id(), symbol->getLineNumber());
	      prec = symbolType.hasFlag(SymbolType::PREC) ? getPrec(symbol) : DEFAULT;
	      if (symbolType.hasFlag(SymbolType::GATHER))
		getGather(symbol, gather);
	      else
		gather.clear();
	      format = &(getFormat(symbol));
	      latexMacro = getLatexMacro(symbol);
	      rpo = getRpo(symbol);
	    }
	  else
	    {
	      int newName = renaming->getOpTo(index);
	      DebugAdvisory("Old symbol = " << symbol << " newName = " << Token::name(newName));
	      name.tokenize(newName, symbol->getLineNumber());
	      prec = renaming->getPrec(index);
	      symbolType.assignFlags(SymbolType::PREC, prec != DEFAULT);
	      gather = renaming->getGather(index);  // deep copy
	      symbolType.assignFlags(SymbolType::GATHER, !gather.empty());
	      format = &(renaming->getFormat(index));
	      symbolType.assignFlags(SymbolType::FORMAT, !format->empty());
	      latexMacro = renaming->getLatexMacro(index);
	      symbolType.assignFlags(SymbolType::LATEX, latexMacro != NONE);
	      rpo = renaming->getRpo(index);
	      symbolType.assignFlags(SymbolType::RPO, rpo != NONE);
	    }

	  const Vector<OpDeclaration>& opDecls = symbol->getOpDeclarations();
	  int domainAndRangeSize = opDecls[0].getDomainAndRange().size();
	  domainAndRange.resize(domainAndRangeSize);
	  const Vector<int>& strategy = symbolType.hasFlag(SymbolType::STRAT) ?
	    symbol->getStrategy() : emptyStrategy;
	  const NatSet& frozen = symbol->getFrozen();

	  bool originator;
	  for (Index j = nrImportedDeclarations; j < nrUserDeclarations; ++j)
	    {
	      const Vector<Sort*>& oldDomainAndRange = opDecls[j].getDomainAndRange();
	      for (Index k = 0; k < domainAndRangeSize; ++k)
		domainAndRange[k] = localSort(copy, renaming, oldDomainAndRange[k]);
	      symbolType.assignFlags(SymbolType::CTOR, opDecls[j].isConstructor());
	      Assert(name.code() != NONE, "bad code");
	      Assert(name.lineNumber() != NONE, "bad line number");
	      Symbol* newSymbol = copy->addOpDeclaration(name,
							 domainAndRange,
							 symbolType,
							 strategy,
							 frozen,
							 prec,
							 gather,
							 *format,
							 latexMacro,
							 rpo,
							 getMetadata(symbol, j),
							 originator);
	      if (j == 0)
		{
		  if (originator)
		    {
		      if (symbolType.getBasicType() == SymbolType::BUBBLE)
		        copy->copyBubbleSpec(symbol, newSymbol);
		    }
		  else
		    {
		      IssueAdvisory(*copy << ": operator " << QUOTE(newSymbol) <<
				   " has been imported from both " << *newSymbol <<
				   " and " << *symbol << " with no common ancestor.");
		    }
		}
	      else  // we must have already imported some declarations
	        Assert(!originator, "bad origination of " << symbol);
	      if (moduleDonatingToTheory)
		copy->opDeclaredInModule.insert(newSymbol->getIndexWithinModule());
	    }
	}
    }
  //
  //	Now handle our polymorphic operators.
  //
  Index nrPolymorphs = getNrPolymorphs();
  for (Index i = nrImportedPolymorphs; i < nrPolymorphs; ++i)
    {
      Token name = getPolymorphName(i);
      SymbolType symbolType = getPolymorphType(i);
      int prec = DEFAULT;
      const Vector<int>* format;
      int latexMacro;
      int rpo;

      int index = (renaming == 0) ? NONE : renaming->renamePolymorph(name.code());
      if (index == NONE)
	{
	  prec = symbolType.hasFlag(SymbolType::PREC) ? getPolymorphPrec(i) : DEFAULT;
	  if (symbolType.hasFlag(SymbolType::GATHER))
	    getPolymorphGather(i, gather);
	  else
	    gather.clear();
	  format = &(getPolymorphFormat(i));
	  latexMacro = getPolymorphLatexMacro(i);
	  rpo = getPolymorphRpo(i);
	}
      else
	{
	  name.tokenize(renaming->getOpTo(index), name.lineNumber());
	  prec = renaming->getPrec(index);
	  symbolType.assignFlags(SymbolType::PREC, prec != DEFAULT);
	  gather = renaming->getGather(index);  // deep copy
	  symbolType.assignFlags(SymbolType::GATHER, !gather.empty());
	  format = &(renaming->getFormat(index));
	  symbolType.assignFlags(SymbolType::FORMAT, !format->empty());
	  latexMacro = renaming->getLatexMacro(index);
	  symbolType.assignFlags(SymbolType::LATEX, latexMacro != NONE);
	  rpo = renaming->getRpo(index);
	  symbolType.assignFlags(SymbolType::RPO, rpo != NONE);
	}

      const Vector<Sort*>& oldDomainAndRange = getPolymorphDomainAndRange(i);
      Index domainAndRangeLength = oldDomainAndRange.size();
      domainAndRange.resize(domainAndRangeLength);
      for (Index j = 0; j < domainAndRangeLength; ++j)
	{
	  Sort* s = oldDomainAndRange[j];
	  domainAndRange[j] = (s == 0) ? 0 : localSort(copy, renaming, s);
	}

      const Vector<int>& strategy = symbolType.hasFlag(SymbolType::STRAT) ?
	getPolymorphStrategy(i) : emptyStrategy;

      int copyIndex = copy->addPolymorph(name,
					 domainAndRange,
					 symbolType,
					 strategy,
					 getPolymorphFrozen(i),
					 prec,
					 gather,
					 *format,
					 latexMacro,
					 rpo,
					 getPolymorphMetadata(i));
      if (moduleDonatingToTheory)
	copy->polymorphDeclaredInModule.insert(copyIndex);
    }
}

void
ImportModule::fixUpDonatedOps2(ImportModule* copy, Renaming* renaming)
{
  //
  //	The map from imported module's symbols to importing module's symbols
  //	is built dynamically.
  //
  ImportTranslation importTranslation(copy, renaming);
  //
  //	Handle our regular operators.
  //
  const Vector<Symbol*>& symbols = getSymbols();
  for (Index i = 0; i < nrUserSymbols; ++i)
    {
      int nrImportedDeclarations = getNrImportedDeclarations(i);
      if (nrUserDecls[i] > nrImportedDeclarations)
	{
	  //
	  //	We donated some declarations for this symbol so do
	  //	an incremental fixup if needed.
	  //
	  Symbol* symbol = symbols[i];
	  SymbolType st = getSymbolType(symbol);
	  if (st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
	    {
	      BinarySymbol* ns = safeCast(BinarySymbol*, importTranslation.translate(symbol));
	      if (ns->getIdentity() == 0)
		{
		  Term* id = safeCast(BinarySymbol*, symbol)->getIdentity();
		  Assert(id != 0, "missing identity");
		  ns->setIdentity(id->deepCopy(&importTranslation));
		}
	    }

	  if (st.hasAttachments())
	    importTranslation.translate(symbol)->copyAttachments(symbol, &importTranslation);
	  else if (st.getBasicType() == SymbolType::BUBBLE)
	    copy->copyFixUpBubbleSpec(symbol, &importTranslation);
	}
    }
  //
  //	Now handle our polymorphic operators.
  //
  {
    Vector<Sort*> domainAndRange;
    
    Index nrPolymorphs = getNrPolymorphs();
    for (Index i = nrImportedPolymorphs; i < nrPolymorphs; ++i)
      {
	//
	//	Get polymorph name.
	//
	int id = getPolymorphName(i).code();
	if (renaming != 0)
	  {
	    int index = renaming->renamePolymorph(id);
	    if (index != NONE)
	      id = renaming->getOpTo(index);
	  }
	//
	//	Get polymorph domain and range.
	//
	const Vector<Sort*>& oldDomainAndRange = getPolymorphDomainAndRange(i);
	Index domainAndRangeSize = oldDomainAndRange.size();
	domainAndRange.resize(domainAndRangeSize);
	for (Index j = 0; j < domainAndRangeSize; ++j)
	  {
	    Sort* s = oldDomainAndRange[j];
	    domainAndRange[j] = (s == 0) ? 0 : localSort(copy, renaming, s);
	  }

	int donatedCopyIndex = copy->findPolymorphIndex(id, domainAndRange);
	Assert(donatedCopyIndex != NONE, "missing polymorph");
	copy->copyFixUpPolymorph(donatedCopyIndex, this, i, &importTranslation);
      }
  }
}

void
ImportModule::localStatementsComplete()
{
  //
  //	Record how many local statement we have so we can
  //	distiguish them from later imports.
  //
  nrOriginalMembershipAxioms = getSortConstraints().size();
  nrOriginalEquations = getEquations().size();
  nrOriginalRules = getRules().size();
  nrOriginalStrategyDefinitions = getStrategyDefinitions().size();
  //
  //	Collect labels from parameters and imports.
  //	We include parameters because we current import statements from
  //	parameter theories Because labels is a set
  //	we don't need to worry about duplicates from diamond imports.
  //
  for (const ImportModule* i : parameterTheories)
    labels.insert(i->labels.begin(), i->labels.end());
  for (const ImportModule* i : importedModules)
    labels.insert(i->labels.begin(), i->labels.end());

  if (canonicalRenaming == 0)
    {
      //
      //	We're an original module so collect labels from our statements.
      //
      for (const SortConstraint* sc : getSortConstraints())
	{
	  int id = sc->getLabel().id();
	  if (id != NONE)
	    labels.insert(id);
	}
      for (const Equation* eq : getEquations())
	{
	  int id = eq->getLabel().id();
	  if (id != NONE)
	    labels.insert(id);
	}
      for (const Rule* rl : getRules())
	{
	  int id = rl->getLabel().id();
	  if (id != NONE)
	    labels.insert(id);
	}
    }
  else
    {
      //
      //	We're a renaming of baseModule, so rename the labels from baseModule.
      //
      for (int i :  baseModule->labels)
	labels.insert(canonicalRenaming->renameLabel(i));
    }
}
