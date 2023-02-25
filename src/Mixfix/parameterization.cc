/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Miscellaneous functionality to support module system.
//

ImportModule*
ImportModule::makeParameterCopy(int moduleName, int parameterName, ModuleCache* moduleCache)
{
  //
  //	We're a theory T used for a parameter X so we make a parameter copy of
  //	ourselves, X :: T
  //
  DebugEnter("called on " << QUOTE(Token::name(moduleName)) <<
	     " with parameter " << QUOTE(Token::name(parameterName)));
  ImportModule* copy = new ImportModule(moduleName, getModuleType(), PARAMETER, moduleCache);
  LineNumber lineNumber(FileTable::AUTOMATIC);
  //
  //	We construct a parameter renaming which will map sorts and labels from us and
  //	our imported theories.
  //
  Renaming* canonical = new Renaming;
  {
    //
    //	For each sort s declared in us, we add a mapping s -> X$s where X is our parameter name.
    //
    const Vector<Sort*>& sorts = getSorts();
    for (int i = nrImportedSorts; i < nrUserSorts; ++i)
      {
	int sortName = sorts[i]->id();
	canonical->addSortMapping(sortName, Token::makeParameterInstanceName(parameterName, sortName));
      }
  }
  //
  //	Now we look for parameter constants; that is constants that were declared in a theory
  //	with the pconst attribute. We want to do this before considering imports in case
  //	import would add duplicated, which is detected by addSortConstantAndLabelMappings().
  //
  {
    const Vector<Symbol*>& symbols = getSymbols();
    for (int i = nrImportedSymbols; i < nrUserSymbols; ++i)
      {
	Symbol* s = symbols[i];
	SymbolType st = getSymbolType(s);
	if (st.hasFlag(SymbolType::PCONST))
	  {
	    Assert(s->arity() == 0, "pconst non-constant");
	    int symbolName = s->id();
	    canonical->addOpMapping(symbolName);
	    canonical->addType(s->rangeComponent());
	    canonical->addOpTarget(Token::makeParameterInstanceName(parameterName, symbolName));
	  }
      }
  }
  //
  //	Technically this is wrong since labels contains labels from our imports
  //	which if from a theory will get added in a later step and if from a module
  //	shouldn't be added at all.
  //
  //	However addSortConstantAndLabelMappings() will silently ignore dups and renaming
  //	labels from modules is harmess since parameter renamings never get applied to
  //	to modules. There is a certain inelegance and potential inefficiency in carrying
  //	superfluous label mappings around but they will be (very) rare in practice and
  //	doing the right thing would cost a second "local" label set in each ImportModule.
  //
  for (int label : labels)
    canonical->addLabelMapping(label, Token::makeParameterInstanceName(parameterName, label));
  //
  //	We make a parameter copy of imported theories and directly import imported modules.
  //
  for (ImportModule* import : importedModules)
    {
      if (import->isTheory())
	{
	  ImportModule* importCopy = moduleCache->makeParameterCopy(parameterName, import);
	  if (importCopy == 0)
	    {
	      copy->markAsBad();
	      return copy;  // give up
	    }
	  copy->addImport(importCopy, INCLUDING, lineNumber);
	  canonical->addSortConstantAndLabelMappings(importCopy->canonicalRenaming);
	}
      else
	copy->addImport(import, INCLUDING, lineNumber);  // HACK need to fix including
    }
  finishCopy(copy, canonical);
  DebugExit("done making " << QUOTE(Token::name(moduleName)) <<
	    " with parameter " << QUOTE(Token::name(parameterName)));

  return copy;
}

void
ImportModule::addSortMappingsFromTheoryView(Renaming* underConstruction,
					    int parameterName,  // because we don't store the parameter name in the copy
					    const View* view) const
{
  Assert(origin == PARAMETER, "called on " << this << " which isn't a parameter copy of a theory");
  Assert(view->getFromTheory() == baseModule, "theory clash: view " << view << " maps from " <<
	 view->getFromTheory() << " but " << this << " has base theory " << baseModule);
  //
  //	We're a parameter copy X :: T of a theory T.
  //	We have to deal with a theory-view V that maps T to T'.
  //	For each sort renaming
  //	  Foo -> X$Foo
  //	in our canonical renaming, if V maps Foo |-> Bar, we want to add a renaming
  //	  X$Foo -> X$Bar
  //	or
  //	  X$Foo -> Bar
  //	depending on whether Bar is theory declared, to underConstruction.
  //
  ImportModule* viewToTheory = view->getToModule();
  int nrSortMappings = canonicalRenaming->getNrSortMappings();
  for (int i = 0; i < nrSortMappings; ++i)
    {
      //
      //	The parameter copy mapped Foo |-> X$Foo .
      //
      int oldFromName = canonicalRenaming->getSortFrom(i);  // Foo
      int oldToName = canonicalRenaming->getSortTo(i);  // X$Foo
      //
      //	The theory-view argument maps Foo |-> Bar .
      //
      int newBaseName = view->renameSort(oldFromName);  // Bar
      //
      //	Find out what sort we land on in the target theory of the view.
      //	If it was declared in a module that the target theory imported
      //	then is cannot be changed by instantiation and does not get the
      //	X$ prefix.
      //
      Sort* toTheorySort = viewToTheory->findSort(newBaseName);
      int newToName = (viewToTheory->moduleDeclared(toTheorySort)) ?
	newBaseName :  // declared in a module so stays as Bar
	Token::makeParameterInstanceName(parameterName, newBaseName);  // X$Bar

      DebugInfo("adding sort mapping during theory view instantiation: " <<
		Token::name(oldFromName) << " to " << Token::name(oldToName) <<
		" becomes " <<
		Token::name(oldToName) << " to " << Token::name(newToName));

      if (oldToName != newToName)  // don't add identity renamings
	underConstruction->addSortMapping(oldToName, newToName);
    }
}

void
ImportModule::addSortMappingsFromModuleView(Renaming* underConstruction, const View* view) const
{
  Assert(origin == PARAMETER, "called on " << this << " which isn't a parameter copy of a theory");
  Assert(view->getFromTheory() == baseModule, "theory clash: view " << view << " maps from " <<
	 view->getFromTheory() << " but " << this << " has base theory " << baseModule);
  //
  //	We're a parameter copy X :: T of a theory T.
  //	We have to deal with a module-view V that maps T to M.
  //	For each sort renaming
  //	  Foo -> X$Foo
  //	in our canonical renaming, if V maps Foo |-> Bar, we want to add a renaming
  //	  X$Foo -> Bar
  //	to underConstruction.
  //
  int nrSortMappings = canonicalRenaming->getNrSortMappings();
  for (int i = 0; i < nrSortMappings; ++i)
    {
      //
      //	The parameter copy mapped Foo |-> X$Foo .
      //
      int oldFromName = canonicalRenaming->getSortFrom(i);  // Foo
      int oldToName = canonicalRenaming->getSortTo(i);  // X$Foo
      //
      //	The module-view argument maps Foo |-> Bar .
      //
      int newToName = view->renameSort(oldFromName);  // Bar
      //
      //	We add a mapping from parameter theory to instantiation by module view
      //	  X$Foo |-> Bar
      //
      underConstruction->addSortMapping(oldToName, newToName);
    }
}

void
ImportModule::addSortRenamingsForParameterChange(Renaming* underConstruction, int newParameterName) const
{
  Assert(origin == PARAMETER, "called on " << this << " which isn't a parameter copy of a theory");
  //
  //	We're a parameter copy X :: T of a theory T.
  //	We have to deal with a change of parameter to Y.
  //	For each each sort renaming
  //	  Foo -> X$Foo
  //	in our canonical renaming, we add a sort renaming
  //	  X$Foo -> Y$Foo
  //	to underConstruction.
  //
  int nrSortMappings = canonicalRenaming->getNrSortMappings();
  for (int i = 0; i < nrSortMappings; ++i)
    {
      int oldFromName = canonicalRenaming->getSortFrom(i);  // Foo
      int oldToName = canonicalRenaming->getSortTo(i);  // X$Foo
      int newToName = Token::makeParameterInstanceName(newParameterName, oldFromName);  // Y$Foo
      underConstruction->addSortMapping(oldToName, newToName);
    }
}

void
ImportModule::addConstantRenamingsForParameterChange(Renaming* underConstruction,
						     int newParameterName,
						     const ImportModule* parameterCopyUser) const
{
  Assert(origin == PARAMETER, "called on " << this << " which isn't a parameter copy of a theory");
  //
  //	We're a parameter copy X :: T of a theory T.
  //	We have to deal with a change of parameter to Y due to our
  //	parameter X being instantiated by a parameter Y from an enclosing entity with X != Y
  //
  //	We only expect to see op mappings in our cannonical renaming if they are mapping pconst constants.
  //	We treat them like sort mappings.
  //
  int nrOpMappings = canonicalRenaming->getNrOpMappings();
  for (int i = 0; i < nrOpMappings; ++i)
    {
      //
      //	We have a pconst constant that generated a op mapping
      //	  c : -> [S] to X$c
      //	In the new parameter copy, X is going to be replaced by Y.
      //	We need to add an op mapping to take us from the parametrized module we're importing
      //	which as a constant X$c to the importer with parameter Y which must have a constant Y$c
      //	generated from the same base theory. This looks like
      //	  X$c: -> [T] to Y$c
      //	where T is the sort S from the base theory was was mapping to in the parameter copy;
      //	T could be S if S came from a module or X$S if X cam from a theory.
      //
      int oldFromName = canonicalRenaming->getOpFrom(i);  // c
      int oldToName = canonicalRenaming->getOpTo(i);      // X$c
      int newToName = Token::makeParameterInstanceName(newParameterName, oldFromName);  // Y$c
      //
      //	Next we need to figure out T so we can make a specific op mapping.
      //
      Assert(canonicalRenaming->getNrTypes(i) == 1, "unexpected mapping of a non-constant oldFromName = " << Token::name(oldFromName) <<
	     " oldToName = " << Token::name(oldToName));
      const set<int>& typeSorts = canonicalRenaming->getTypeSorts(i, 0);
      Assert(!typeSorts.empty(), "no type sorts");
      //
      //	Pick any sort and follow its mapping to find the kind in the parameter theory.
      //	This is a sort in the base theory.
      //
      int firstSortName = *(typeSorts.begin());
      DebugInfo("We saw a mapping " << Token::name(oldFromName) << " : " << Token::name(firstSortName) << " to " <<
		Token::name(oldToName));
      //
      //	See what that sort mapped to in the parameter copy (us).
      //
      int newSortName = canonicalRenaming->renameSort(firstSortName);
      DebugInfo("newSortName = " << Token::name(newSortName));
      //
      //	Now we find the sort with that name in the parameterCopyUser.
      //
      Sort* newSort = parameterCopyUser->findSort(newSortName);
      Assert(newSort, "missing sort " << Token::name(newSortName) << " in " << parameterCopyUser);
      //
      //	Add the mapping
      //	  X$c : -> [Y] to c'
      //
      underConstruction->addOpMapping(oldToName);
      underConstruction->addType(newSort->component());
      underConstruction->addOpTarget(newToName);
    }
}

ConnectedComponent*
ImportModule::translateComponent(const ConnectedComponent* component,
				 const ImportModule* parameterCopyUser) const
{
  Assert(origin == PARAMETER, "called on " << this << " which isn't a parameter copy of a theory");
  //
  //	We're a parameter copy X :: T of a theory T.
  //	We translate a connected component appearing in T to its equivalent in parameterCopyUser.
  //
  Sort* firstUserSort = component->sort(Sort::FIRST_USER_SORT);
  int firstUserSortName = firstUserSort->id();
  int nameInUs = canonicalRenaming->renameSort(firstUserSortName);
  Sort* sortInUser = parameterCopyUser->findSort(nameInUs);
  Assert(sortInUser != 0, "no sort translation for " << component->sort(Sort::FIRST_USER_SORT));
  return sortInUser->component();
}

void
ImportModule::addFromPartOfRenaming(Renaming* underConstruction,
				    Symbol* s,
				    const ImportModule* parameterCopyUser) const
{
  //
  //	Add the from-part of a mapping from s to underConstruction, mapping
  //	the arity and coarity into us.
  //
  int fromName;
  if (baseModule->getSymbolType(s).hasFlag(SymbolType::PCONST))
    {
      //
      //	We need to map from X$c rather than c
      //
      int renamingIndex = canonicalRenaming->renameOp(s);
      Assert(renamingIndex != NONE, "couldn't find renaming form theory to parameter copy for pconst " << s);
      fromName = canonicalRenaming->getOpTo(renamingIndex);
    }
  else
    fromName = s->id();

  DebugInfo("from name is " << Token::name(fromName));
  underConstruction->addOpMapping(fromName);
  int nrArgs = s->arity();
  for (int i = 0; i < nrArgs; ++i)
    underConstruction->addType(translateComponent(s->domainComponent(i), parameterCopyUser));
  underConstruction->addType(translateComponent(s->rangeComponent(), parameterCopyUser));
}

void
ImportModule::addOpMappingsFromView(Renaming* underConstruction,
				    const View* view,
				    const ImportModule* parameterCopyUser,
				    ImportModule* targetTheoryParameterCopy) const
{
  Assert(origin == PARAMETER, "called on " << this << " which isn't a parameter copy of a theory");
  Assert(view->getFromTheory() == baseModule, "theory clash: view " << view << " maps from " <<
	 view->getFromTheory() << " but " << this << " has base theory " << baseModule);
  //
  //	The renaming underConstruction will be used to translate operators from
  //	parameterCopyUser to an instantiation of it by arguments where the argument
  //	instantiating the parameter copy is view.
  //
  //	We're a parameter copy X :: T of a theory T.
  //	We have to deal with a view V that maps T to T'.
  //
  {
    //
    //	For each operator
    //	  f : Foo1 .... Foon -> Bar
    //	in the T that is not module-declared, we need to check if it matches an
    //	op mapping in view, to say, g. If so we need to add an op mapping
    //	  f : [Foo1'] ... [Foo1'] -> [Bar'] to g
    //	to underConstruction, where for each sort s in T, s' is its
    //	translation in X :: T, i.e. in us.
    //
    //	Since we now allow pconst, we also need to deal with the case that f is a pconst constant
    //	that was mapped by from from c in T to X$c in X :: T by canonicalRenaming and may need to be
    //	mapped back to c or new name given by the mapping of c by view.
    //
    const Vector<Symbol*>& symbols = baseModule->getSymbols();
    int nrUserSymbols = baseModule->getNrUserSymbols();
    for (int i = 0; i < nrUserSymbols; ++i)
      {
	Symbol* s = symbols[i];
	Assert(baseModule->getSymbolType(s).getBasicType() != SymbolType::VARIABLE, "didn't expect variable");
	Assert(baseModule->getSymbolType(s).getBasicType() != SymbolType::SORT_TEST, "didn't expect sort test");
	if (baseModule->moduleDeclared(s))
	  continue;  // don't map operators with a module declaration
	//
	//	Check for op->term mapping in the view.
	//
	Term* fromTerm;
	Term* toTerm;
	if (view->getOpToTermMapping(s, fromTerm, toTerm))
	  {
	    //
	    //	We matched an op->term mapping.
	    //	underConstruction "borrows" toTerm and fromTerm.
	    //
	    addFromPartOfRenaming(underConstruction, s, parameterCopyUser);
	    if (targetTheoryParameterCopy)
	      {
		//
		//	Out view maps to a theory so the instantiation we're going to build
		//	will import a parameter copy of the target theory and thus the
		//	names of the sorts and (pconst) symbols referred to in our toTerm
		//	may have different names in the instantiations signature.
		//	We need a suitably mapped copy of toTerm, but we need to retain
		//	the indices in the variables that are used for instantiating toTerm.
		//
		ImportTranslation toTheoryToParameterCopy(targetTheoryParameterCopy,
							  targetTheoryParameterCopy->canonicalRenaming,
							  true);
		toTerm = toTerm->deepCopy(&toTheoryToParameterCopy);  // FIXME: need to garbage collect toTerm
		underConstruction->addOpTargetTerm(fromTerm, toTerm, true);
	      }
	    else
	      {
		//
		//	Our view maps to a module so the instantiation we're going to build
		//	will import that module, and the sorts and symbols referred to in our toTerm
		//	can just be looked up in the instantiation's signature. So underConstruction
		//	can just "borrow" toTerm
		//
		underConstruction->addOpTargetTerm(fromTerm, toTerm, false);
	      }
	    //
	    //	Assume this is the only possible renaming of s
	    //
	    continue;
	  }
	//
	//	Note that op->term mappings _are not_ stored in a view's base Renaming.
	//	They only appear in canonicalRenamings used for instantiation.
	//
	//	See how the parameter copy and the view rename this operator.
	//
	int baseName = s->id();
	int copyRenamingIndex = canonicalRenaming->renameOp(s);
	//
	//	Only pconst constants should ever be mapped by canonicalRenaming
	//
	Assert(copyRenamingIndex == NONE ||
	       baseModule->getSymbolType(s).hasFlag(SymbolType::PCONST), "copy renaming of " << s << " to " <<
	       Token::name(canonicalRenaming->getOpTo(copyRenamingIndex)) << "for non-pconst");
	int fromName = (copyRenamingIndex == NONE) ? baseName : canonicalRenaming->getOpTo(copyRenamingIndex);
	int viewRenamingIndex = view->renameOp(s);
	int toName = (viewRenamingIndex == NONE) ? baseName : view->getOpTo(viewRenamingIndex);
	if (targetTheoryParameterCopy)
	  {
	    //
	    //	view is a theory-view and the parameter copy of its target theory might further
	    //	map toName to X$toName
	    //
	    if (s->arity() == 0)
	      {
		//
		//	First we need to find the range component translated into the target view.
		//
		ImportModule* viewToTheory = view->getToModule();
		Assert(viewToTheory == targetTheoryParameterCopy->baseModule,
		       "target theory mismatch " << viewToTheory << " vs " <<  targetTheoryParameterCopy->baseModule);

		Sort* fromTheoryRangeSort = s->getRangeSort();  // any will do
		int fromTheoryRangeSortName = fromTheoryRangeSort->id();
		int toTheoryRangeSortName = view->renameSort(fromTheoryRangeSortName);
		Sort* toTheoryRangeSort = viewToTheory->findSort(toTheoryRangeSortName);
		Assert(toTheoryRangeSort != 0, "couldn't find sort " << Token::name(toTheoryRangeSortName) << " in " <<
		       viewToTheory << " to map " << fromTheoryRangeSort << " to under " << view);
		ConnectedComponent* toTheoryRangeComponent = toTheoryRangeSort->component();
		//
		//	Now we find the constant symbol in viewToTheory.
		//
		static Vector<ConnectedComponent*> dummy;
		Symbol* symbolInToTheory = viewToTheory->findSymbol(toName, dummy, toTheoryRangeComponent);
		Assert(symbolInToTheory != 0, "couldn't find " << Token::name(toName) << " with range " <<
		       toTheoryRangeSort << " in " << viewToTheory << " to map " << s << " to.");
		//
		//	Now we can ask targetTheoryParameterCopy what it mapped our constant to.
		//
		int index = targetTheoryParameterCopy->canonicalRenaming->renameOp(symbolInToTheory);
		if (index != NONE)
		  toName = targetTheoryParameterCopy->canonicalRenaming->getOpTo(index);
		DebugInfo("constant " << s << " was mapped to " << symbolInToTheory << " by view " <<
			  view << " and then to " << Token::name(toName) << " by parameter copy");
	      }
	  }
	if (toName != fromName)
	  {
	    //
	    //	Need to add specific renaming.
	    //
	    DebugInfo("adding a mapping for " << s << " from " << Token::name(fromName) <<
		      " to " << Token::name(toName));
	    underConstruction->addOpMapping(fromName);
	    //
	    //	Deal with the domain and range specificity.
	    //
	    int nrArgs = s->arity();
	    for (int i = 0; i < nrArgs; ++i)
	      underConstruction->addType(translateComponent(s->domainComponent(i), parameterCopyUser));
	    underConstruction->addType(translateComponent(s->rangeComponent(), parameterCopyUser));

	    underConstruction->addOpTarget(toName);
	  }
      }
  }
  {
    //
    //	For each polymorph f, check if it matches a generic op mapping f to g in view.
    //	If so we need to add the the generic op mapping to underConstruction.
    //
    int nrPolymorphs = baseModule->getNrPolymorphs();
    for (int i = 0; i < nrPolymorphs; ++i)
      {
	if (baseModule->moduleDeclaredPolymorph(i))
	  continue;  // don't map polymorphs declared in modules
	int id = baseModule->getPolymorphName(i).code();
	int index = view->renamePolymorph(id);
	if (index != NONE)
	  {
	    underConstruction->addOpMapping(id);
	    underConstruction->addOpTarget(view->getOpTo(index));
	  }
      }
  }
}

void
ImportModule::addStratMappingsFromView(Renaming* canonical,
				       const View* view,
				       const ImportModule* parameterCopyUser) const
{
  Assert(origin == PARAMETER, "called on " << this << " which isn't a parameter copy of a theory");
  Assert(view->getFromTheory() == baseModule, "theory clash: view " << view << " maps from " <<
	 view->getFromTheory() << " but " << this << " has base theory " << baseModule);
  //
  //	We're a parameter copy X :: T of a theory T.
  //	We have to deal with a view V that maps T to T'.
  //
  {
    //
    //	For each strategy
    //	  s : Foo1 .... Foon @ Bar
    //	in the theory, we need to check if it matches an op mapping in view,
    //	to say, g. If so we need to add an op mapping
    //	  s : [Foo1'] ... [Foo1'] @ [Bar'] to r
    //	to canonical, where for each sort s, s' is the translation of s
    //	used in making the parameter copy of the theory.
    //
    const Vector<RewriteStrategy*>& strategies = baseModule->getStrategies();
    for (RewriteStrategy* s : strategies)
      {
	if (baseModule->moduleDeclared(s))
	  continue;  // don't map strategies declared in a module
	int nrArgs = s->arity();
	const Vector<Sort*>& domain = s->getDomain();
	CallStrategy* fromCall;
	StrategyExpression* toExpr;
	const Vector<int>* varIndices;
	if (view->getStratToExprMapping(s, fromCall, toExpr, varIndices))
	  {
	    canonical->addStratMapping(s->id());
	    for (int j = 0; j < nrArgs; ++j)
	      canonical->addType(translateComponent(domain[j]->component(), parameterCopyUser));
	    canonical->addStratTargetExpr(fromCall, toExpr);
	    canonical->addStratMappingVarIndices(*varIndices);
	  }
	int index = view->renameStrat(s);
	if (index != NONE)
	  {
	    canonical->addStratMapping(s->id());
	    for (int j = 0; j < nrArgs; ++j)
	      canonical->addType(translateComponent(domain[j]->component(), parameterCopyUser));
	    canonical->addStratTarget(view->getStratTo(index));
	  }
      }
  }
}

ImportModule*
ImportModule::makeSummation(int moduleName, const Vector<ImportModule*>& modules, ModuleCache* moduleCache)
{
  //
  //	First we determine what kind of module will result.
  //
  Vector<ImportModule*>::const_iterator i = modules.begin();
  const Vector<ImportModule*>::const_iterator e = modules.end();
  MixfixModule::ModuleType moduleType = (*i)->getModuleType();
  for (++i; i != e; ++i)
    moduleType = MixfixModule::join(moduleType, (*i)->getModuleType());
  //
  //	Make the module.
  //
  ImportModule* sum = new ImportModule(moduleName, moduleType, ImportModule::SUMMATION, moduleCache);
  //
  //	Combine bound parameters.
  //
  //	Handle imports and their parameters.
  //
  LineNumber lineNumber(FileTable::AUTOMATIC);
  for (ImportModule* import : modules)
    {
      Assert(!(import->hasFreeParameters()), "free parameters in summand " << import);

      int nrParameters = import->getNrParameters();
      for (int j = 0; j < nrParameters; ++j)
	{
	  int parameterName = import->parameterNames[j];
	  if (sum->findParameterIndex(parameterName) == NONE)
	    {
	      Token t;
	      t.tokenize(parameterName, FileTable::AUTOMATIC);
	      sum->addParameter(t, import->parameterTheories[j]);
	      sum->addBoundParameter(parameterName);
	    }
	}

      sum->addImport(import, ImportModule::INCLUDING, lineNumber);
    }
  //
  //	Process module.
  //
  sum->importSorts();
  sum->closeSortSet();
  if (!(sum->isBad()))
    {
      sum->importOps();
      if (!(sum->isBad()))
	{
	  sum->closeSignature();
	  sum->importStrategies();
	  sum->importRuleLabels();
	  if (!(sum->isBad()))
	    {
	      sum->fixUpImportedOps();
	      if (!(sum->isBad()))
		{
		  sum->closeFixUps();
		  //
		  //	We have no local statements.
		  //
		  sum->localStatementsComplete();
		}
	    }
	}
    }
  //
  //	Reset phase counter in each imported module.
  //
  sum->resetImports();
  return sum;
}
