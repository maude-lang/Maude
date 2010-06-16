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
ImportModule::makeParameterCopy(int moduleName, int parameterName, ModuleCache* moduleCache)
{
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
	canonical->addSortMapping(sortName, Token::parameterRename(parameterName, sortName));
      }
  }
  {
    //
    //	Technically this is wrong since labels contains labels from our imports
    //	which if from a theory will get added in a later step and if from a module
    //	shouldn't be added at all.
    //
    //	However addSortAndLabelMappings() will silently ignore dups and renaming
    //	labels from modules is harmess since parameter renamings never get applied to
    //	to modules. There is a certain inelegance and potential inefficiency in carrying
    //	superfluous label mappings around but they will be (very) rare in practice and
    //	doing the right thing would cost a second "local" label set in each ImportModule.
    //
    FOR_EACH_CONST(i, set<int>, labels)
      canonical->addLabelMapping(*i, Token::parameterRename(parameterName, *i));
  }
  {
    //
    //	We make a parameter copy of imported theories and directly import imported modules.
    //
    FOR_EACH_CONST(i, Vector<ImportModule*>, importedModules)
      {
	ImportModule* import = *i;
	if (import->isTheory())
	  {
	    ImportModule* importCopy = moduleCache->makeParameterCopy(parameterName, *i);
	    copy->addImport(importCopy, INCLUDING, lineNumber);
	    canonical->addSortAndLabelMappings(importCopy->canonicalRenaming);
	  }
	else
	  copy->addImport(*i, INCLUDING, lineNumber);  // HACK need to fix including
      }
  }
  finishCopy(copy, canonical);
  return copy;
}

void
ImportModule::handleInstantiationByParameter(ImportModule* copy,
					     Renaming* canonical,
					     ParameterMap& parameterMap,
					     const Vector<View*>& arguments,
					     const Vector<int>& parameterArgs,
					     ModuleCache* moduleCache) const
{
  //
  //	First we handle parameters instantiated by parameters from an enclosing module.
  //	These parameters keep their theory but change their name to that of the
  //	parameter from the enclosing module. They also become bound. Multiple parameters
  //	may be instantiated to the same parameter from the enclosing module, in which case
  //	they collapse to a single bound parameter.
  //
  int nrParameters = parameterNames.size();
  for (int i = 0; i < nrParameters; ++i)
    {
     if (arguments[i] == 0)
	{
	  //
	  //	Parameter instantiated by a parameter from an enclosing module.
	  //
	  int parameterName = parameterArgs[i];  // name can change
	  if (copy->findParameterIndex(parameterName) == NONE)
	    {
	      //
	      //	We don't already have a (must be bound since we've only added
	      //	bound parameters so far) parameter with name we need so add one.
	      //
	      Token t;
	      t.tokenize(parameterName, FileTable::AUTOMATIC);
	      ImportModule* toModule = getParameterTheory(i);  // parameter theory stays the same
	      ImportModule* parameterTheory = moduleCache->makeParameterCopy(parameterName, toModule);
	      copy->addParameter(t, parameterTheory);
	    }
	  int oldParameterName = parameterNames[i];
	  if (oldParameterName != parameterName)
	    {
	      parameterMap[oldParameterName] = parameterName;
	      //
	      //	For each Foo -> X$Foo we mapped in the parameter copy of the parameter theory,
	      //	we need to have a mapping X$Foo -> Y$Foo where Y is the new parameter name.
	      //
	      const Renaming* parameterRenaming = importedModules[i]->canonicalRenaming;
	      int nrSortMappings = parameterRenaming->getNrSortMappings();
	      for (int j = 0; j < nrSortMappings; ++j)
		{
		  int fromName = parameterRenaming->getSortTo(j);  // X$Foo
		  int baseName = parameterRenaming->getSortFrom(j);  // Foo
		  int toName = Token::parameterRename(parameterName, baseName);  // Y$Foo
		  canonical->addSortMapping(fromName, toName);
		}
	    }
	}
    }
}

void
ImportModule::handleInstantiationByTheoryView(ImportModule* copy,
					      Renaming* canonical,
					      ParameterMap& parameterMap,
					      ParameterSet& extraParameterSet,
					      const Vector<View*>& arguments,
					      ModuleCache* moduleCache) const
{
  //
  //	Second we handle parameters instantiated by theory-views. These parameters keep
  //	their name but change their theory to that of the view target.
  //	These parameters give rise to an extra parameter list for parameterized sorts.
  //
  int nrParameters = parameterNames.size();
  for (int i = 0; i < nrParameters; ++i)
    {
      View* argumentView = arguments[i];
      if (argumentView != 0)
	{
	  ImportModule* toModule = argumentView->getToModule();
	  if (toModule->isTheory())
	    {
	      int parameterName = parameterNames[i];
	      Assert(copy->findParameterIndex(parameterName) == NONE, "parameter clash");

	      Token t;
	      t.tokenize(parameterName, FileTable::AUTOMATIC); 
	      ImportModule* parameterTheory = moduleCache->makeParameterCopy(parameterName, toModule);
	      copy->addParameter(t, parameterTheory);
	      argumentView->addUser(copy);

	      int viewName = argumentView->id();
	      if (parameterName != viewName)
		parameterMap[parameterName] = viewName;
	      extraParameterSet.insert(parameterName);
	      //
	      //	For each Foo -> X$Foo we mapped in the parameter copy of the parameter theory,
	      //	we need to have a mapping X$Foo -> X$Bar where the view maps Foo -> Bar.
	      //
	      const ImportModule* parameterCopyOfTheory = importedModules[i];
	      const Renaming* parameterRenaming = parameterCopyOfTheory->canonicalRenaming;
	      int nrSortMappings = parameterRenaming->getNrSortMappings();
	      for (int j = 0; j < nrSortMappings; ++j)
		{
		  int oldFromName = parameterRenaming->getSortFrom(j);  // Foo
		  int fromName = parameterRenaming->getSortTo(j);  // X$Foo
		  int baseName = argumentView->renameSort(oldFromName);  // Bar
		  //
		  //	Find out what sort we land on in the target theory of the view.
		  //
		  Sort* toModuleSort = toModule->findSort(baseName);
		  int toName = (toModule->moduleDeclared(toModuleSort)) ?
		    baseName :  // sorts declared in a module will not get a X$ prefix and stay as Bar
		    Token::parameterRename(parameterName, baseName);  // X$Bar

		  DebugAdvisory("adding sort mapping during theory view instantiation: " <<
				Token::name(oldFromName) << " to " << Token::name(fromName) <<
				" becomes " <<
				Token::name(fromName) << " to " << Token::name(toName));

		  if (fromName != toName)
		    canonical->addSortMapping(fromName, toName);
		}
	      addOpMappingsFromView(canonical, parameterCopyOfTheory, argumentView);
	    }
	}
    }
}

void
ImportModule::handleInstantiationByModuleView(ImportModule* copy,
					      Renaming* canonical,
					      ParameterMap& parameterMap,
					      const Vector<View*>& arguments) const
{
  //
  //	Third we handle parameters instantiated by views to modules.
  //
  LineNumber lineNumber(FileTable::AUTOMATIC);
  int nrParameters = parameterNames.size();
  for (int i = 0; i < nrParameters; ++i)
    {
      View* argumentView = arguments[i];
      if (argumentView != 0)
	{
	  ImportModule* toModule = argumentView->getToModule();
	  if (!(toModule->isTheory()))
	    {
	      copy->addImport(argumentView->getToModule(), PROTECTING, lineNumber);
	      argumentView->addUser(copy);
	      //
	      //	For each Foo -> X$Foo we mapped in the parameter copy of the parameter theory,
	      //	we need to have a mapping X$Foo -> Bar where Bar is the target of Foo in our view.
	      //
	      const ImportModule* parameterCopyOfTheory = importedModules[i];
	      const Renaming* parameterRenaming = parameterCopyOfTheory->canonicalRenaming;
	      int nrSortMappings = parameterRenaming->getNrSortMappings();
	      for (int j = 0; j < nrSortMappings; ++j)
		{
		  int fromName = parameterRenaming->getSortTo(j);  // X$Foo
		  int toName = argumentView->renameSort(parameterRenaming->getSortFrom(j));  // Bar
		  canonical->addSortMapping(fromName, toName);
		}
	      addOpMappingsFromView(canonical, parameterCopyOfTheory, argumentView);
	      int oldParameterName = parameterNames[i];
	      int newParameterName = argumentView->id();
	      if (oldParameterName != newParameterName)
		parameterMap[oldParameterName] = newParameterName;
	    }
	}
    }
}

ConnectedComponent*
ImportModule::translateComponent(const Renaming* renaming, const ConnectedComponent* component) const
{
  Sort* sort = findSort(renaming->renameSort(component->sort(Sort::FIRST_USER_SORT)->id()));
  Assert(sort != 0, "no sort translation for " << component->sort(Sort::FIRST_USER_SORT));
  return sort->component();
}

void
ImportModule::addOpMappingsFromView(Renaming* canonical,
				    const ImportModule* parameterCopyOfTheory,
				    const View* view) const
{
  const ImportModule* originalTheory = parameterCopyOfTheory->baseModule;
  Assert(view->getFromTheory() == originalTheory, "theory clash");
  {
    //
    //	For each operator
    //	  f : Foo1 .... Foon -> Bar
    //	in the theory, we need to check if it matches an op mapping in view,
    //	to say, g. If so we need to add an op mapping
    //	  f : [Foo1'] ... [Foo1'] -> [Bar'] to g
    //	to canonical, where for each sort s, s' is the translation of s
    //	used in making the parameter copy of the theory.
    //
    const Renaming* parameterRenaming = parameterCopyOfTheory->canonicalRenaming;
    const Vector<Symbol*>& symbols = originalTheory->getSymbols();
    
    int nrUserSymbols = originalTheory->getNrUserSymbols();
    for (int i = 0; i < nrUserSymbols; ++i)
      {
	Symbol* s = symbols[i];
	Assert(originalTheory->getSymbolType(s).getBasicType() != SymbolType::VARIABLE,
	       "didn't expect variable");
	Assert(originalTheory->getSymbolType(s).getBasicType() != SymbolType::SORT_TEST,
	       "didn't expect sort test");
	if (originalTheory->moduleDeclared(s))
	  continue;  // don't map operators with a module declaration
	Term* toTerm = view->getOpMapTerm(s);
	if (toTerm != 0)
	  {
	    canonical->addOpMapping(s->id());
	    int nrArgs = s->arity();
	    for (int j = 0; j < nrArgs; ++j)
	      canonical->addType(translateComponent(parameterRenaming, s->domainComponent(j)));
	    canonical->addType(translateComponent(parameterRenaming, s->rangeComponent()));
	    canonical->addOpTargetTerm(toTerm);
	  }
	int index = view->renameOp(s);
	if (index != NONE)
	  {
	    canonical->addOpMapping(s->id());
	    int nrArgs = s->arity();
	    for (int j = 0; j < nrArgs; ++j)
	      canonical->addType(translateComponent(parameterRenaming, s->domainComponent(j)));
	    canonical->addType(translateComponent(parameterRenaming, s->rangeComponent()));
	    canonical->addOpTarget(view->getOpTo(index));
	  }
      }
  }
  {
    //
    //	For each polymorph f, check if it matches a generic op mapping f to g in view.
    //	If so we need to add the the generic op mapping to canonical.
    //
    int nrPolymorphs = originalTheory->getNrPolymorphs();
    for (int i = 0; i < nrPolymorphs; ++i)
      {
	if (originalTheory->moduleDeclaredPolymorph(i))
	  continue;  // don't map polymorphs declared in modules
	int id = originalTheory->getPolymorphName(i).code();
	int index = view->renamePolymorph(id);
	if (index != NONE)
	  {
	    canonical->addOpMapping(id);
	    canonical->addOpTarget(view->getOpTo(index));
	  }
      }
  }
}

void
ImportModule::handleParameterizedSorts(Renaming* canonical,
				       const ParameterMap& parameterMap,
				       const ParameterSet& extraParameterSet) const
{
  //
  //	Check for parameterized sorts declared inside us.
  //	We also need to map sorts delared in parameterized modules that
  //	we import otherwise our use of those sorts will fail.
  //	For the moment we consider all user sorts for mapping.
  //
  const Vector<Sort*>& sorts = getSorts();
  int nrSorts = getNrUserSorts();
  for (int i = 0; i < nrSorts; ++i)
    {
      int sortId = sorts[i]->id();
      if (Token::auxProperty(sortId) == Token::AUX_STRUCTURED_SORT)
	{
	  bool mapped = false;
	  int header;
	  Vector<int> parameters;
	  Vector<int> extraParameters;
	  Token::splitParameterList(sortId, header, parameters);
	  int nrSortParameters = parameters.size();
	  for (int j = 0; j < nrSortParameters; ++j)
	    {
	      int original = parameters[j];
	      ParameterMap::const_iterator m = parameterMap.find(original);
	      if (m != parameterMap.end())
		{
		  parameters[j] = m->second;
		  mapped = true;
		}
	      if (extraParameterSet.find(original) != extraParameterSet.end())
		extraParameters.append(original);
	    }
	  int newSortId = mapped ? Token::joinParameterList(header, parameters) : sortId;
	  if (!extraParameters.empty())
	    newSortId = Token::joinParameterList(newSortId, extraParameters);
	  if (newSortId != sortId)
	    canonical->addSortMapping(sortId, newSortId);
	}
    }
}

void
ImportModule::handleRegularImports(ImportModule* copy,
				   const Vector<View*>& arguments,
				   const Vector<int>& parameterArgs,
				   ModuleCache* moduleCache) const
{
  //
  //	Now handle our regular imports.
  //
  LineNumber lineNumber(FileTable::AUTOMATIC);
  int nrImports = importedModules.size();
  for (int i = parameterNames.size(); i < nrImports; ++i)
    {
      ImportModule* import = importedModules[i];
      int nrImportParameters = import->parameterNames.size();
      if (nrImportParameters == 0)
	copy->addImport(import, INCLUDING, lineNumber);  // HACK need to fix including
      else
	{
	  //
	  //	Imported module is itself parameterized; it parameters must all be bound and must be a subset of
	  //	our own so we can build an instantiation for them.
	  //
	  Assert(import->parametersBound(), "free parameter in imported module " << import);
	  Vector<View*> views(nrImportParameters);
	  Vector<int> names(nrImportParameters);
	  for (int j = 0; j < nrImportParameters; ++j)
	    {
	      int parameterName = import->parameterNames[j];
	      int indexInUs = findParameterIndex(parameterName);
	      Assert(indexInUs != NONE, "couldn't find bound parameter " << Token::name(parameterName) <<
		     " for import " << import << " in " << this);
	      views[j] = arguments[indexInUs];
	      names[j] = parameterArgs[indexInUs];
	    }
	  ImportModule* instance = import->instantiateBoundParameters(views, names, moduleCache);
	  Assert(instance->getNrParameters() == 0 || instance->parametersBound(),
		 "free parameter in instance " << instance << " of imported module " << import);
	  copy->addImport(instance, INCLUDING, lineNumber);  // HACK need to fix including
	}
    }
}

ImportModule*
ImportModule::makeInstantiation(int moduleName,
				const Vector<View*>& arguments,
				const Vector<int>& parameterArgs,
				ModuleCache* moduleCache)
{
  Assert(!parametersBound(), "parameters bound");
  Assert(arguments.size() == getNrParameters(), "arguments size mismatch");
  Assert(parameterArgs.size() == getNrParameters(), "parameterArgs size mismatch");
  //
  //	An instantiation is a renamed copy of an parameterized module with
  //	different imports.
  //
  ModuleType moduleType = getModuleType();
  if (moduleType == FUNCTIONAL_MODULE)
    {
      //
      //	Check views to see if we need to promote a functional module
      //	to a system module.
      //
      FOR_EACH_CONST(i, Vector<View*>, arguments)
	{
	  View* v = *i;
	  if (v != 0 && v->getToModule()->getModuleType() == SYSTEM_MODULE)
	    {
	      moduleType = SYSTEM_MODULE;
	      break;
	    }
	}
    }
  ImportModule* copy = new ImportModule(moduleName, moduleType, INSTANTIATION, moduleCache);
  //
  //	We construct an instantiation renaming which will map sorts and operations
  //	we got from our parameters to those of our view args' targets. Also we
  //	directly import the targets.
  //
  Renaming* canonical = new Renaming;
  ParameterMap parameterMap;  // map for parameters in sorts
  ParameterSet extraParameterSet;  // set for generating extra parameter list for parameterized sorts
  
  handleInstantiationByParameter(copy, canonical, parameterMap, arguments, parameterArgs, moduleCache);
  //
  //	All bound parameters have now been added. We keep a copy of our
  //	view args and if we have bound parameters we also keep track of
  //	our parameter args.
  //
  copy->viewArgs = arguments;  // deep copy
  if (copy->parameterNames.size() > 0)
    copy->paramArgs = parameterArgs;  // deep copy
  handleInstantiationByTheoryView(copy, canonical, parameterMap, extraParameterSet, arguments, moduleCache);
  handleInstantiationByModuleView(copy, canonical, parameterMap, arguments);
  handleParameterizedSorts(canonical, parameterMap, extraParameterSet);
  handleRegularImports(copy, arguments, parameterArgs, moduleCache);
  finishCopy(copy, canonical);
  return copy;
}

ImportModule*
ImportModule::instantiateBoundParameters(const Vector<View*>& arguments,
					 const Vector<int>& parameterArgs,
					 ModuleCache* moduleCache)
{
  DebugAdvisory("instantiating bound parameters of " << this);
  //
  //	We are an instantiated (not renamed) module with parameters that have
  //	been bound by a parameterized module that imported us. Now that these
  //	parameters have been been instantiated, we need to make a new instantiated
  //	module that is "like us" except that our bound parameters are
  //	instantiated with the new arguments.
  //
  Assert(parametersBound(), "parameters not bound");
  Assert(arguments.size() == getNrParameters(), "arguments size bad");
  Assert(parameterArgs.size() == getNrParameters(), "parameterArgs size bad");
  Assert(baseModule != 0, "no base module");
  Assert(viewArgs.size() == paramArgs.size(), "original args are inconsistant");

  if (viewArgs.empty())
    {
      Assert(origin == RENAMING, "bad origin");
      //
      //        We are a renaming of a module with bound parameters.
      //        The bound parameters will be identical to our own so
      //        we first make a new instantiation of our base module and
      //        then rename it.
      //
      ImportModule* newBase = baseModule->
        instantiateBoundParameters(arguments, parameterArgs, moduleCache);

      ParameterMap parameterMap;
      ParameterSet extraParameterSet;
      int nrParameters = parameterNames.size();
      for (int i = 0; i < nrParameters; ++i)
	{
	  int name = parameterNames[i];
	  View* argumentView = arguments[i];
	  if (argumentView != 0)
	    {
	      parameterMap[name] = argumentView->id();
	      ImportModule* toModule = argumentView->getToModule();
	      if (toModule->isTheory())
		extraParameterSet.insert(name);
	    }
	  else
	    parameterMap[name] = parameterArgs[i];
	}

      Renaming* newRenaming =
	instantiateRenaming(canonicalRenaming, parameterMap, extraParameterSet);
      ImportModule* instance = moduleCache->makeRenamedCopy(newBase, newRenaming);
      delete newRenaming;
      return instance;
    }
  Assert(origin == INSTANTIATION, "bad origin");
  //
  //	We are an instantiation of a parameterized module so
  //	we build a new set of arguments to instantiate our baseModule on.
  //
  int nrFreeParameters = baseModule->getNrParameters();
  Assert(nrFreeParameters == viewArgs.size(), "nrFreeParameters clash");
  Vector<View*> newViewArgs(nrFreeParameters);
  Vector<int> newParamArgs(nrFreeParameters);
  Vector<View*> extraViewArgs;
  Vector<int> extraParamArgs;

  for (int i = 0; i < nrFreeParameters; ++i)
    {
      //
      //	See what the free parameter was originally instantiated by.
      //
      View* v = viewArgs[i];
      if (v == 0)
	{
	  //
	  //	The free parameter was originally instantiated by a parameter
	  //	from an enclosing module, making it bound in us. So we now look
	  //	up its name in our parameters to find the index of its
	  //	new instantiation in the argument list we were passed.
	  //
	  int boundParameterName = paramArgs[i];
	  int index = findParameterIndex(boundParameterName);
	  Assert(index != NONE,  "didn't find bound parameter");
	  View* v2 = arguments[index];
	  newViewArgs[i] = v2;
	  newParamArgs[i] = parameterArgs[index];
	  if (v2 != 0 && v2->getToModule()->isTheory())
	    {
	      //
	      //	Instantiating a bound parameter by a theory-view allows it
	      //	to escape. This must be recorded so that it can be rebound
	      //	in a subsequent instantiation.
	      //
	      DebugAdvisory("bound parameter " << Token::name(boundParameterName) <<
			    " escaped to become free parameter " <<
			    Token::name(baseModule->getParameterName(i)) <<
			    " that will need rebinding"); 
	      extraViewArgs.append(0);
	      extraParamArgs.append(boundParameterName);
	    }
	}
      else
	{
	  //
	  //	The free parameter was originally instantiated by a view
	  //	so this instantiation does not change.
	  //
	  Assert(!v->getToModule()->isTheory(), "unexpected theory-view " << v);
	  newViewArgs[i] = v;
	  newParamArgs[i] = 0;
	}
    }
  ImportModule* instance = moduleCache->makeInstatiation(baseModule, newViewArgs, newParamArgs);
  if (!extraParamArgs.empty())
    instance = moduleCache->makeInstatiation(instance, extraViewArgs, extraParamArgs);
  return instance;
}

int
ImportModule::instantiateSortName(int sortId,
				  const ParameterMap& parameterMap,
				  const ParameterSet& extraParameterSet)
{
  if (Token::auxProperty(sortId) == Token::AUX_STRUCTURED_SORT)
    {
      bool mapped = false;
      int header;
      Vector<int> parameters;
      Vector<int> extraParameters;
      Token::splitParameterList(sortId, header, parameters);
      int nrSortParameters = parameters.size();
      for (int i = 0; i < nrSortParameters; ++i)
	{
	  int original = parameters[i];
	  ParameterMap::const_iterator m = parameterMap.find(original);
	  if (m != parameterMap.end())
	    {
	      parameters[i] = m->second;
	      mapped = true;
	    }
	  if (extraParameterSet.find(original) != extraParameterSet.end())
	    extraParameters.append(original);
	}
      if (mapped)
	sortId = Token::joinParameterList(header, parameters);
      if (!extraParameters.empty())
	sortId = Token::joinParameterList(sortId, extraParameters);
    }
  return sortId;
}

Renaming*
ImportModule::instantiateRenaming(const Renaming* original,
				  const ParameterMap& parameterMap,
				  const ParameterSet& extraParameterSet)
{
  DebugAdvisory("Original renaming: " << original);
  Renaming* instance = new Renaming;
  {
    int nrSortMappings = original->getNrSortMappings();
    for (int i = 0; i < nrSortMappings; i++)
      {
	int from = instantiateSortName(original->getSortFrom(i), parameterMap, extraParameterSet);
	int to = instantiateSortName(original->getSortTo(i), parameterMap, extraParameterSet);
	instance->addSortMapping(from, to);
      }
  }
  {
    int nrLabelMappings = original->getNrLabelMappings();
    for (int i = 0; i < nrLabelMappings; i++)
      instance->addLabelMapping(original->getLabelFrom(i), original->getLabelTo(i));
  }
  {
    Vector<Token> token(1);
    int nrOpMappings = original->getNrOpMappings();
    for (int i = 0; i < nrOpMappings; i++)
      {
	instance->addOpMappingPartialCopy(original, i);
	int nrTypes = original->getNrTypes(i);
	for (int j = 0; j < nrTypes; ++j)
	  {
	    //
	    //	Only map one sort from each type.
	    //
	    const set<int>& typeSorts = original->getTypeSorts(i, j);
	    Assert(!typeSorts.empty(), "no sorts");
	    int sortName = instantiateSortName(*(typeSorts.begin()), parameterMap, extraParameterSet);
	    token[0].tokenize(sortName, FileTable::SYSTEM_CREATED);
	    instance->addType(true, token);
	  }
      }
  }
  DebugAdvisory("Instantiated renaming: " << instance);
  return instance;
}
