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

ImportModule*
ImportModule::makeRenamedCopy(int name, Renaming* canonical, ModuleCache* moduleCache)
{
  ImportModule* copy = new ImportModule(name, getModuleType(), RENAMING, moduleCache);
  LineNumber lineNumber(FileTable::AUTOMATIC);

  int nrParameters = parameterNames.size();
  for (int i = 0; i < nrParameters; ++i)
    {
      Token t;
      t.tokenize(parameterNames[i], FileTable::AUTOMATIC);
      copy->addParameter(t, importedModules[i]);
    }
  //Assert(!parametersBound(), "renamed module has bound parameters");

  int nrImports = importedModules.size();
  for (int i = nrParameters; i < nrImports; ++i)
    copy->addImport(moduleCache->makeRenamedCopy(importedModules[i], canonical), INCLUDING, lineNumber);  // HACK should this be INCLUDING?

  finishCopy(copy, canonical);
  return copy;
}

void
ImportModule::finishCopy(ImportModule* copy, Renaming* canonical)
{
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
  
  copy->importOps();
  donateOps2(copy, canonical);
  copy->closeSignature();

  copy->fixUpImportedOps();
  fixUpDonatedOps2(copy, canonical);
  copy->closeFixUps();
  //
  //	Statements are not copied so we have no local statements.
  //
  copy->localStatementsComplete();
  //
  //	Reset phase counter in each imported module and return copy.
  //
  copy->resetImports();
}

Sort*
ImportModule::localSort(ImportModule* copy, Renaming* renaming, const Sort* sort)
{
  return (sort->index() == Sort::KIND) ?
    localSort2(copy, renaming, sort->component()->sort(1))->component()->sort(Sort::KIND) :
    localSort2(copy, renaming, sort);
}

Sort*
ImportModule::localSort2(ImportModule* copy, Renaming* renaming, const Sort* sort)
{
  int id = sort->id();
  if (renaming != 0)
    id = renaming->renameSort(id);
  Sort* ts = copy->findSort(id);
  Assert(ts != 0, "couldn't find sort " << QUOTE(Token::sortName(id)) <<
	 " renamed from " << QUOTE(sort) << " in module " << copy);
  return ts;
}

void
ImportModule::donateSorts2(ImportModule* copy, Renaming* renaming)
{
  //
  //	Donate our sorts, after a possible renaming.
  //
  bool moduleDonatingToTheory = copy->isTheory() && !isTheory();
  const Vector<Sort*>& sorts = getSorts();
  for (int i = nrImportedSorts; i < nrUserSorts; i++)
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
	      //	to sorts from thoeries; so we handle it harshly.
	      //
	      IssueWarning(*copy << ": sort " << QUOTE(original) <<
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
      if (moduleDonatingToTheory)
	copy->sortDeclaredInModule.insert(sort->getIndexWithinModule());
    }
  //
  //	Donate our subsort relations, after a possible renaming.
  //
  for (int i = 0; i < nrUserSorts; i++)
    {
      int nrImports = getNrImportedSubsorts(i);
      const Sort* s = sorts[i];
      const Vector<Sort*>& subsorts = s->getSubsorts();
      int nrSubsorts = subsorts.length();
      if (nrSubsorts > nrImports)
	{
	  Sort* ts = localSort2(copy, renaming, s);
	  for (int j = nrImports; j < nrSubsorts; j++)
	    ts->insertSubsort(localSort2(copy, renaming, subsorts[j]));
	}
    }
}

void
ImportModule::donateOps2(ImportModule* copy, Renaming* renaming)
{
  bool moduleDonatingToTheory = copy->isTheory() && !isTheory();

  Vector<int> gather;
  Vector<Sort*> domainAndRange;
  Vector<int> emptyStrategy;
  //
  //	Handle our regular operators.
  //
  const Vector<Symbol*>& symbols = getSymbols();
  for (int i = 0; i < nrUserSymbols; i++)
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

	  int index = (renaming == 0) ? NONE : renaming->renameOp(symbol);  // index of renaming that applies to symbol
	  if (index == NONE)
	    {
	      name.tokenize(symbol->id(), symbol->getLineNumber());
	      prec = symbolType.hasFlag(SymbolType::PREC) ? getPrec(symbol) : DEFAULT;
	      if (symbolType.hasFlag(SymbolType::GATHER))
		getGather(symbol, gather);
	      else
		gather.clear();
	      format = &(getFormat(symbol));
	    }
	  else
	    {
	      int newName = renaming->getOpTo(index);
	      DebugAdvisory("Old symbol = " << symbol << " newName = " << newName);
	      name.tokenize(newName, symbol->getLineNumber());
	      prec = renaming->getPrec(index);
	      symbolType.assignFlags(SymbolType::PREC, prec != DEFAULT);
	      gather = renaming->getGather(index);  // deep copy
	      symbolType.assignFlags(SymbolType::GATHER, !gather.empty());
	      format = &(renaming->getFormat(index));
	      symbolType.assignFlags(SymbolType::FORMAT, !format->empty());
	    }

	  const Vector<OpDeclaration>& opDecls = symbol->getOpDeclarations();
	  int domainAndRangeLength = opDecls[0].getDomainAndRange().length();
	  domainAndRange.resize(domainAndRangeLength);
	  const Vector<int>& strategy = symbolType.hasFlag(SymbolType::STRAT) ?
	    symbol->getStrategy() : emptyStrategy;
	  const NatSet& frozen = symbol->getFrozen();

	  bool originator;
	  for (int j = nrImportedDeclarations; j < nrUserDeclarations; j++)
	    {
	      const Vector<Sort*>& oldDomainAndRange = opDecls[j].getDomainAndRange();
	      for (int k = 0; k < domainAndRangeLength; k++)
		domainAndRange[k] = localSort(copy, renaming, oldDomainAndRange[k]);
	      symbolType.assignFlags(SymbolType::CTOR, opDecls[j].isConstructor());
	      Symbol* newSymbol = copy->addOpDeclaration(name,  // BUG: name has codeNr = -1
							 domainAndRange,
							 symbolType,
							 strategy,
							 frozen,
							 prec,
							 gather,
							 *format,
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
		      IssueAdvisory(*copy << ": operator " << QUOTE(symbol) <<
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
  int nrPolymorphs = getNrPolymorphs();
  for (int i = nrImportedPolymorphs; i < nrPolymorphs; i++)
    {
      Token name = getPolymorphName(i);
      SymbolType symbolType = getPolymorphType(i);
      int prec = DEFAULT;
      const Vector<int>* format;

      int index = (renaming == 0) ? NONE : renaming->renamePolymorph(name.code());
      if (index == NONE)
	{
	  prec = symbolType.hasFlag(SymbolType::PREC) ? getPolymorphPrec(i) : DEFAULT;
	  if (symbolType.hasFlag(SymbolType::GATHER))
	    getPolymorphGather(i, gather);
	  else
	    gather.clear();
	  format = &(getPolymorphFormat(i));
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
	}

      const Vector<Sort*>& oldDomainAndRange = getPolymorphDomainAndRange(i);
      int domainAndRangeLength = oldDomainAndRange.length();
      domainAndRange.resize(domainAndRangeLength);
      for (int j = 0; j < domainAndRangeLength; j++)
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
  for (int i = 0; i < nrUserSymbols; i++)
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
    
    int nrPolymorphs = getNrPolymorphs();
    for (int i = nrImportedPolymorphs; i < nrPolymorphs; i++)
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
	int domainAndRangeLength = oldDomainAndRange.length();
	domainAndRange.resize(domainAndRangeLength);
	for (int j = 0; j < domainAndRangeLength; j++)
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
  nrOriginalMembershipAxioms = getSortConstraints().length();
  nrOriginalEquations = getEquations().length();
  nrOriginalRules = getRules().length();

  FOR_EACH_CONST(i, Vector<ImportModule*>, importedModules)
    labels.insert((*i)->labels.begin(), (*i)->labels.end());
  if (canonicalRenaming == 0)
    {
      {
	FOR_EACH_CONST(i, Vector<SortConstraint*>, getSortConstraints())
	  {
	    int id = (*i)->getLabel().id();
	    if (id != NONE)
	      labels.insert(id);
	  }
      }
      {
	FOR_EACH_CONST(i, Vector<Equation*>, getEquations())
	  {
	    int id = (*i)->getLabel().id();
	    if (id != NONE)
	      labels.insert(id);
	  }
      }
      {
	FOR_EACH_CONST(i, Vector<Rule*>, getRules())
	  {
	    int id = (*i)->getLabel().id();
	    if (id != NONE)
	      labels.insert(id);
	  }
      }
    }
  else
    {
      FOR_EACH_CONST(i, set<int>, baseModule->labels)
	labels.insert(canonicalRenaming->renameLabel(*i));
    }
}
