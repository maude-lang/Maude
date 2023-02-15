/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019-2023 SRI International, Menlo Park, CA 94025, USA.

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
//	This is the code for instantiation a module FOO that has free parameters.
//	Such a module is either a syntactic or meta-syntactic module or possibly
//	with intervening renamings and non-final instantiations.
//
//	The instantiation itself may be non-final, in which case theory-views and
//	parameter-free module-views are allowed or final, in which case, parameters
//	from an enclosing object and arbitrary module-views are allowed.
//

ImportModule*
ImportModule::makeInstantiation(int moduleName,
				const Vector<Argument*>& arguments,
				ModuleCache* moduleCache)
{
  //
  //	Arguments can either be views or parameters from an enclosing object.
  //
  DebugEnter("called on " << QUOTE(Token::name(moduleName)));
  Assert(!isBad(), "trying to instantiate bad module");
  Assert(!hasBoundParameters(), "module " << Token::name(moduleName) << " has bound parameters");
  Assert(static_cast<int>(arguments.size()) == getNrParameters(), "arguments size mismatch");
  //
  //	An instantiation is a renamed copy of an parameterized module with
  //	different imports.
  //
  ModuleType moduleType = getModuleType();
  if (moduleType == FUNCTIONAL_MODULE || moduleType == SYSTEM_MODULE)
    {
      //
      //	Check views to see if we need to promote a functional module
      //	to a system module, or a non-strategy module to a strategy
      //	module.
      //
      for (Argument* a : arguments)
	{
	  View* v = dynamic_cast<View*>(a);
	  if (v != 0)
	    {
	      ModuleType paramType = v->getToModule()->getModuleType();
	      if (moduleType == FUNCTIONAL_MODULE && paramType == SYSTEM_MODULE)
		moduleType = SYSTEM_MODULE;
	      else if (moduleType != STRATEGY_MODULE &&
		       MixfixModule::isStrategic(paramType))
		moduleType = STRATEGY_MODULE;
	    }
	}
    }
  ImportModule* copy = new ImportModule(moduleName, moduleType, INSTANTIATION, moduleCache);
  //
  //	We construct an instantiation renaming which will map the sorts and operations
  //	we got from our parameter theories to those of our view args' targets. Also we
  //	directly import the targets.
  //
  Renaming* canonical = new Renaming;
  //
  //	We make a map from names of our parameters to the names of the instantiating views
  //	or parameters from an enclosing object. Because parameters instantiated by theory-views
  //	don't disappear we keep track of them in extraParameterSet.
  //
  ParameterMap parameterMap;
  ParameterSet extraParameterSet;
  //
  //	Keep a copy of our view arguments.
  //
  copy->savedArguments = arguments;  // deep copy
  //
  //	Three kinds of argument: theory-view, module-view, parameter from an enclosing object.
  //
  //	We do theory-views first.
  //
  if (!handleInstantiationByTheoryView(copy, canonical, parameterMap, extraParameterSet, arguments, moduleCache))
    {
      //
      //	Fell at the first hurdle so clean up an don't finish the copy.
      //
      delete canonical;
      copy->markAsBad();
      return copy;
    }
  //
  //	We do parameters next because we want to get the set of positions that were instantiated
  //	by a parameter.
  //
  NatSet positionsInstantiatedParameter;
  if (!handleInstantiationByParameter(copy, canonical, parameterMap, positionsInstantiatedParameter, arguments, moduleCache))
    {
      //
      //	Likely a parameter theory sort clashing with a funky user sort name - pack up and go home.
      //
      delete canonical;
      copy->markAsBad();
      return copy;
    }
  //
  //	We do module-views last because they generate an import of the target module and we
  //	need the set of positions that were instantiated by a parameter to computed conflicts.
  //
  if (!handleInstantiationByModuleView(copy, canonical, parameterMap, positionsInstantiatedParameter, arguments, moduleCache))
    {
      //
      //	Likely a parameter theory sort clashing with a funky user sort name - pack up and go home.
      //
      delete canonical;
      copy->markAsBad();
      return copy;
    }
  handleParameterizedSorts(canonical, parameterMap, extraParameterSet);
  handleParameterizedConstants(canonical, parameterMap, extraParameterSet);
  handleRegularImports(copy, arguments, moduleCache);
  if (copy->isBad())
    {
      DebugInfo("instantiation of " << this << " produced a bad module " << copy);
      //
      //	Need to delete renaming here since it could be in a bad state and will never
      //	be added to the bad module.
      //
      delete canonical;
    }
  else
    finishCopy(copy, canonical);
  DebugExit("finished making " << QUOTE(Token::name(moduleName)));
  return copy;
}

bool
ImportModule::handleInstantiationByTheoryView(ImportModule* copy,
					      Renaming* canonical,
					      ParameterMap& parameterMap,
					      ParameterSet& extraParameterSet,
					      const Vector<Argument*>& arguments,
					      ModuleCache* moduleCache) const
{
  //
  //	First we handle parameters instantiated by theory-views. These parameters keep
  //	their name but change their theory to that of the view target.
  //	These parameters give rise to an extra parameter list for parameterized sorts.
  //
  ParameterSet parametersTakingTheoryViews;
  int nrParameters = parameterNames.size();
  for (int i = 0; i < nrParameters; ++i)
    {
      if (View* argumentView = dynamic_cast<View*>(arguments[i]))
	{
	  ImportModule* toModule = argumentView->getToModule();
	  if (toModule->isTheory())
	    {
	      int parameterName = parameterNames[i];
	      Assert(copy->findParameterIndex(parameterName) == NONE, "parameter clash");
	      //
	      //	Check for conflicts between parameters instantiated by
	      //	theory-views.
	      //	It is entirely possible that a parameter has a conflict
	      //	with itself and therefore can never take a theory-view.
	      //
	      parametersTakingTheoryViews.insert(parameterName);
	      int firstClash = findFirstClash(parametersTakingTheoryViews, parameterName);
	      if (firstClash != NONE)
		{
		  if (firstClash == parameterName)
		    {
		      IssueWarning("In nonfinal module instantiation " << QUOTE(copy) << " parameter " <<
				   QUOTE(Token::name(parameterName)) <<
				   " may not take a theory-view because it has a self-conflict in the underlying module " <<
				   QUOTE(this) << ".");
		    }
		  else
		    {
		      IssueWarning("In nonfinal module instantiation " << QUOTE(copy) << " parameters " <<
				   QUOTE(Token::name(firstClash)) << " and " <<
				   QUOTE(Token::name(parameterName)) <<
				   " cannot both be instantiated by theory-views because of a conflict in the underlying module " <<
				   QUOTE(this) << ".");
		    }
		  return false;
		}
	      //
	      //	If we have a parameter X :: T and the argument view maps T to T'
	      //	the instantiation copy gets a parameter X :: T'
	      //
	      Token parameterToken;
	      parameterToken.tokenize(parameterName, FileTable::AUTOMATIC); 
	      ImportModule* targetTheoryParameterCopy = moduleCache->makeParameterCopy(parameterName, toModule);
	      if (targetTheoryParameterCopy == 0)
		return false;
	      copy->addParameter(parameterToken, targetTheoryParameterCopy);
	      argumentView->addUser(copy);
	      //
	      //	If parameter X :: T was instantiated by V from T to T' we need to map
	      //
	      //	Sort{X} |-> Sort{V}{X}
	      //
	      //	If by chance V and X share the same name, we don't add an identity mapping.
	      //
	      int viewName = argumentView->getCleanName();
	      if (parameterName != viewName)
		parameterMap[parameterName] = viewName;
	      extraParameterSet.insert(parameterName);
	      //
	      //	The sort and op mappings from our parameter theory to the theory-view target
	      //	give rise to sort and op mapping in the canonical renaming that maps from
	      //	us into our instantiation and is used to translate statements from us into
	      //	our instantiation as needed.
	      //
	      const ImportModule* parameterCopyOfTheory = parameterTheories[i];
	      parameterCopyOfTheory->addSortMappingsFromTheoryView(canonical, parameterName, argumentView);
	      parameterCopyOfTheory->addOpMappingsFromView(canonical, argumentView, this, targetTheoryParameterCopy);
	      parameterCopyOfTheory->addStratMappingsFromView(canonical, argumentView, this);
	    }
	}
    }
  return true;
}

bool
ImportModule::handleInstantiationByParameter(ImportModule* copy,
					     Renaming* canonical,
					     ParameterMap& parameterMap,
					     NatSet& positionsInstantiatedParameter,
					     const Vector<Argument*>& arguments,
					     ModuleCache* moduleCache) const
{
  //
  //	Second we handle parameters instantiated by parameters from an enclosing module.
  //	These parameters keep their theory but change their name to that of the
  //	parameter from the enclosing module. They also become bound. Multiple parameters
  //	may be instantiated to the same parameter from the enclosing module, in which case
  //	they collapse to a single bound parameter.
  //	We compute the set of positions instantiated by parameter and propagate conflicts
  //	arising between those positions.
  //
  int nrParameters = parameterNames.size();
  for (int i = 0; i < nrParameters; ++i)
    {
      if (Parameter* p = dynamic_cast<Parameter*>(arguments[i]))
	{
	  //
	  //	Parameter instantiated by a parameter from an enclosing module.
	  //
	  int parameterName = p->id();  // name can change
	  if (copy->findParameterIndex(parameterName) == NONE)
	    {
	      //
	      //	We don't already have a parameter with name we need to add one.
	      //
	      Token parameterToken;
	      parameterToken.tokenize(parameterName, FileTable::AUTOMATIC);
	      //
	      //	The base parameter theory doesn't change, but we need a new parameter
	      //	theory copy to match the a change of parameter name.
	      //
	      ImportModule* baseParameterTheory = getParameterTheory(i);
	      if (ImportModule* parameterTheory = moduleCache->makeParameterCopy(parameterName, baseParameterTheory))
		{
		  DebugAdvisory("For parameter " << Token::name(parameterNames[i]) << " bound to parameter " <<
				parameterToken << " from an enclosing object we have baseParameterTheory " <<
				baseParameterTheory << " and make a new parameter theory copy " << parameterTheory);
		  copy->addParameter(parameterToken, parameterTheory);
		  copy->addBoundParameter(parameterName);
		}
	      else
		return false;
	    }
	  int oldParameterName = parameterNames[i];
	  if (oldParameterName != parameterName)
	    {
	      //
	      //	If the parameter changed names, say from X to Y, then we need to map
	      //	both parameterized sorts and parameter copies of the theory sorts.
	      //
	      //	Sort{X} |-> Sort{Y}
	      //
	      parameterMap[oldParameterName] = parameterName;
	      //
	      //	For each sort Foo -> X$Foo we mapped in the parameter copy of the parameter theory,
	      //	X$Foo |-> Y$Foo
	      //
	      parameterTheories[i]->addSortRenamingsForParameterChange(canonical, parameterName);
	      //
	      //	For each pconst constant c : -> [S] |-> X$c we mapped in the parameter
	      //	copy of the theory, we add a mapping c : -> [T] |-> Y$c
	      //	where T is whatever S mapped to in the parameter copy.
	      //
	      parameterTheories[i]->addConstantRenamingsForParameterChange(canonical, parameterName, this);
	    }
	  //
	  //	Need to propagate parameter conflicts from ourselves into copy.
	  //
	  FOR_EACH_CONST(j, NatSet, positionsInstantiatedParameter)
	    {
	      if (hasConflict(oldParameterName, parameterNames[*j]))
		{
		  //
		  //	We have parameters, say X and Y, that have a conflict.
		  //	They have been instantiated by parameters p and p2 in
		  //	the copy, so p and p2 must be given a conflict in the copy.
		  //
		  Parameter* p2 = safeCast(Parameter*, arguments[*j]);
		  copy->addConflict(parameterName, p2->id());
		}
	    }
	  positionsInstantiatedParameter.insert(i);
	}
    }
  return true;
}

bool
ImportModule::handleInstantiationByModuleView(ImportModule* copy,
					      Renaming* canonical,
					      ParameterMap& parameterMap,
					      const NatSet& positionsInstantiatedParameter,
					      const Vector<Argument*>& arguments,
					      ModuleCache* moduleCache) const
{
  //
  //	Finally we handle parameters instantiated by views to modules.
  //
  LineNumber lineNumber(FileTable::AUTOMATIC);
  Index nrParameters = getNrParameters();
  for (Index i = 0; i < nrParameters; ++i)
    {
      //
      //	Do we have a view (as opposed to a bound parameter)?
      //
      if (View* argumentView = dynamic_cast<View*>(arguments[i]))
	{
	  //
	  //	Is it a module view?
	  //
	  ImportModule* toModule = argumentView->getToModule();
	  if (!(toModule->isTheory()))
	    {
	      if (!handleBoundParameters(copy, argumentView, moduleCache))
		return false;
	      //
	      //	The copy imports the view to-module, and the copy is registered as a
	      //	user of the view.
	      //
	      copy->addImport(argumentView->getToModule(), PROTECTING, lineNumber);
	      argumentView->addUser(copy);
	      //
	      //	For each sort and pconst constant Foo -> X$Foo we mapped in the parameter copy
	      //	of the parameter theory, we need to have a mapping X$Foo -> Bar where Bar is the
	      //	target of Foo in our view.
	      //
	      const ImportModule* parameterCopyOfTheory = parameterTheories[i];
	      parameterCopyOfTheory->addSortMappingsFromModuleView(canonical, argumentView);
	      parameterCopyOfTheory->addOpMappingsFromView(canonical, argumentView, this);
	      parameterCopyOfTheory->addStratMappingsFromView(canonical, argumentView, this);
	      //
	      //	We need to map a parameterized sort Foo{X} to F{V}.
	      //
	      int oldParameterName = parameterNames[i];
	      int newParameterName = argumentView->getCleanName();
	      if (oldParameterName != newParameterName)
		parameterMap[oldParameterName] = newParameterName;
	      //
	      //	Need to add conflicts between bound parameters in the argument view
	      //	to the copy.
	      //
	      copy->addInAllConflicts(argumentView);
	      //
	      //	Need to add any conflicts between the bound parameters in our argument
	      //	and bare paramters in the same instantiation.
	      //	This is one of two places where new conflicts are generated ab initio.
	      //	If the instantiation looks like modExp{..., viewExpr,..., X,...}
	      //	then X will have a conflict with all bound parameters in viewExpr, since
	      //	it has the possibility of leaving them stranded in a nonfinal instantiation
	      //	should both recieve theory-views in a nonfinal instantiation.
	      //
	      for (int j : positionsInstantiatedParameter)
		{
		  int bareParameterName = safeCast(Parameter*, arguments[j])->id();
		  copy->addConflictsWithBoundParameters(argumentView, bareParameterName);
		}
	    }
	}
    }
  return true;
}

bool
ImportModule::handleBoundParameters(ImportModule* copy, View* argumentView, ModuleCache* moduleCache) const
{
  //
  //	We go through the bound parameters in our module-view argument
  //	and add any that we don't already have.
  //
  int nrParameters = argumentView->getNrParameters();
  DebugInfo("view = " << argumentView << " with " << nrParameters << " parameters");
  for (int i = 0; i < nrParameters; ++i)
    {
      int parameterName = argumentView->getParameterName(i);
      DebugInfo("  looking at parameter " << Token::name(parameterName));
      //
      //	Because we need this parameter for our instantiation, because
      //	it is needed to instantiate a module-view, it must be added as
      //	a bound parameter for our instantiation.
      //
      if (copy->findParameterIndex(parameterName) == NONE)
	{
	  DebugNew("  Didn't find parameter " << Token::name(parameterName) << " in " << copy);
	  //
	  //	Our instantiation doesn't already have a parameter
	  //	with this name so we must add one.
	  //
	  Token t;
	  t.tokenize(parameterName, FileTable::AUTOMATIC);
	  //
	  //	The base parameter theory doesn't change, but we need a new parameter
	  //	theory copy to match the a change of parameter name.
	  //	Note that the parameter theory copy need not already exist and we can
	  //	run into an error when we tray to make it.
	  //
	  ImportModule* baseParameterTheory = argumentView->getParameterTheory(i);
	  if (ImportModule* parameterTheory = moduleCache->makeParameterCopy(parameterName, baseParameterTheory))
	    {
	      DebugInfo(" - For parameter " << t <<
			" of theory " << baseParameterTheory <<
			" from enclosing object needed to instantiate argument view " <<
			argumentView << " we make a new parameter copy " << parameterTheory);
	      copy->addParameter(t, parameterTheory);
	      copy->addBoundParameter(parameterName);
	    }
	  else
	    return false;
	}
    }
  return true;
}

void
ImportModule::handleParameterizedSorts(Renaming* canonical,
				       const ParameterMap& parameterMap,
				       const ParameterSet& extraParameterSet) const
{
  //
  //	Check for parameterized sorts declared inside us.
  //	We also need to map sorts declared in parameterized modules that
  //	we import otherwise our use of those sorts will fail.
  //	For the moment we consider all user sorts for mapping.
  //
  const Vector<Sort*>& sorts = getSorts();
  int nrSorts = getNrUserSorts();
  //
  //	We consider all user sorts that did not come from a parameter theory.
  //	Sorts with index < nrSortsFromParameter came from parameter theories.
  //	Sorts with index >= getNrUserSorts() were system generated.
  //
  for (int i = nrSortsFromParameters; i < nrSorts; ++i)
    {
      int sortId = sorts[i]->id();
      int newSortId = instantiateSortName(sortId, parameterMap, extraParameterSet);
      if (newSortId != sortId)
	{
	  DebugAdvisory("handleParameterizedSorts() mapping sort " << Token::name(sortId) <<
			" to " << Token::name(newSortId));
	  canonical->addSortMapping(sortId, newSortId);
	}
    }
}

void
ImportModule::handleParameterizedConstants(Renaming* canonical,
					   const ParameterMap& parameterMap,
					   const ParameterSet& extraParameterSet) const
{
  //
  //	Check for parameterized constants declared inside us.
  //	We also need to map constants declared in parameterized modules that
  //	we import otherwise our use of those constants will fail.
  //	For the moment we consider all user constants for mapping.
  //
  const Vector<Symbol*>& symbols = getSymbols();
  int nrUserSymbols = getNrUserSymbols();
  for (int i = 0; i < nrUserSymbols; ++i)
    {
      Symbol* s = symbols[i];
      if (s->arity() == 0)
	{
	  //
	  //	We cheat and pretend the constant is a sort.
	  //
	  int constantId = s->id();
	  int newConstantId = instantiateSortName(constantId, parameterMap, extraParameterSet);
	  if (newConstantId != constantId)
	    {
	      //
	      //	Need to add an specific op mapping.
	      //
	      canonical->addOpMapping(constantId);
	      canonical->addType(s->rangeComponent());
	      canonical->addOpTarget(newConstantId);
	    }
	}
    }
}

void
ImportModule::handleRegularImports(ImportModule* copy,
				   const Vector<Argument*>& arguments,
				   ModuleCache* moduleCache) const
{
  //
  //	Now handle our regular imports.
  //
  LineNumber lineNumber(FileTable::AUTOMATIC);
  FOR_EACH_CONST(i, Vector<ImportModule*>, importedModules)
    {
      ImportModule* import = *i;
      DebugInfo("Instantiating " << this << " and now need to instantiate its import " <<
		import << " which has " << import->parameterNames.size() << " parameters");
      int nrImportParameters = import->parameterNames.size();
      if (nrImportParameters == 0)
	copy->addImport(import, INCLUDING, lineNumber);  // HACK need to fix including
      else
	{
	  //
	  //	Imported module is itself parameterized; it parameters must all be bound and must be a subset of
	  //	our own so we can build an instantiation for them.
	  //
	  Assert(!(import->hasFreeParameters()), "free parameter in imported module " << import);
	  Vector<Argument*> newArgs(nrImportParameters);
	  for (int j = 0; j < nrImportParameters; ++j)
	    {
	      int parameterName = import->parameterNames[j];
	      int indexInUs = findParameterIndex(parameterName);
	      Assert(indexInUs != NONE, "couldn't find bound parameter " << Token::name(parameterName) <<
		     " for import " << import << " in " << this);
	      newArgs[j] = arguments[indexInUs];
	    }
	  ImportModule* instance = import->instantiateBoundParameters(newArgs, moduleCache);
	  if (instance == 0)
	    {
	      DebugInfo("handleRegularImports() - instantiateBoundParameters() returned null");
	      copy->markAsBad();
	      return;
	    }
	  Assert(!(instance->hasFreeParameters()),
	  	 "free parameter in instance " << instance << " of imported module " << import);
	  copy->addImport(instance, INCLUDING, lineNumber);  // HACK need to fix including
	}
    }
}
