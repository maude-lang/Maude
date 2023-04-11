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
//	This is the code for instantiation a module FOO that has bound parameters.
//	Such a module is a final instantiation or a renaming of a module with
//	bound parameters or a summation of modules at least one of which has
//	bound parameters.
//
//	The instatiation of a module with bound parameters is subtle. We don't
//	actually make a transformed copy of the module itself. Rather we go
//	back and see how it was made, and duplicate the making process, but
//	with parameters from the enclosing object replaced by their instantiations.
//	So another way of looking at it is that we are re-evaluating the module
//	expression that is implicitly stored in the structure of existing modules
//	and views.
//

ImportModule*
ImportModule::instantiateBoundParameters(const Vector<Argument*>& arguments, ModuleCache* moduleCache)
{
  //
  //	We are a module with parameters that have bound to those in
  //	a parameterized module that imported us. Now that these
  //	parameters have been been instantiated, we need to make a new instantiated
  //	module that is "like us" except that our bound parameters are
  //	instantiated with the new arguments.
  //
  //	The arguments that instantiate our bound parameters are passed in arguments.
  //	These input vectors have been constructed so that their indices match those in parameterNames.
  //
  Assert(hasBoundParameters(), "parameters not bound");
  Assert(static_cast<int>(arguments.size()) == getNrParameters(), "arguments size bad");

  switch (origin)
    {
    case SUMMATION:
      return handleSummation(arguments, moduleCache);
    case RENAMING:
      return handleRenaming(arguments, moduleCache);
    case INSTANTIATION:
      return handleFinalInstantiation(arguments, moduleCache);
    default:
      CantHappen("bad origin " << origin);
    }
  return 0;
}

ImportModule*
ImportModule::handleSummation(const Vector<Argument*>& arguments, ModuleCache* moduleCache)
{
  Assert(savedArguments.empty(), "unexpected saved arguments in summation");
  //
  //	We are instantiating a summation expression:
  //	  modExp1 + ... + modExpn
  //	where some or all of the modExpi have bound parameters.
  //
  //	We go through our imports and make an instantiation for each module
  //	expression that has bound parameters. We then form a sum
  //	of the instantiations with any unparamterized modules form the
  //	orginal summation.
  //
  Vector<ImportModule*> instantiatedImports;
  for (ImportModule* import : importedModules)
    {
      int nrParameters = import->parameterNames.size();
      if (nrParameters != 0)
	{
	  //
	  //	The module expression has parameters so form a new argument list
	  //	to match the order of arguments required by the module expression.
	  //
	  Vector<Argument*> newArgs(nrParameters);
	  for (Index j = 0; j < nrParameters; ++j)
	    {
	      int parameterName = import->parameterNames[j];
	      int indexInUs = findParameterIndex(parameterName);
	      Assert(indexInUs != NONE, "couldn't find bound parameter " << Token::name(parameterName) <<
		     " for instantiation of import " << import << " in sum " << this);
	      newArgs[j] = arguments[indexInUs];
	    }
	  //
	  //	We make a mutually recursive call to make the instantiation of
	  //	the module expression.
	  //
	  if (ImportModule* instance = import->instantiateBoundParameters(newArgs, moduleCache))
	    instantiatedImports.append(instance);
	  else
	    return 0;
	}
      else
	instantiatedImports.append(import);
    }
  return moduleCache->makeSummation(instantiatedImports);
}

ImportModule*
ImportModule::handleRenaming(const Vector<Argument*>& arguments, ModuleCache* moduleCache)
{
  DebugEnter("original: " << this);
  Assert(savedArguments.empty(), "unexpected saved arguments in renaming");
  //
  //        We are a renaming of a module (baseModule) with bound parameters.
  //        The bound parameters will be identical to our own so
  //        we first make a new instantiation of our base module and
  //        then rename it.
  //
  //
  ImportModule* newBase = baseModule->instantiateBoundParameters(arguments, moduleCache);
  if (newBase == 0)
    {
      DebugExit("FAILED to instantiate baseModule " << baseModule);
      return 0;
    }
  //
  //	We now need to instantiate the renaming. This requires
  //	instantiating all parameterized sorts occurring in sort mappings
  //	and specific operator mappings within the renaming.
  //
  //	To instantiate a parameter sort we need a mapping from parameter names
  //	to arguments (view names and parameter names), and we also need to
  //	keep a set of parameter names that survive because they were
  //	instatiated by a theory-view.
  //
  //	We now go through our parameters and make parameterMap that takes
  //	our parameter names to arguments that are passed in. When
  //	an argument is a theory-view, we add the corresponding parameter name to
  //	extraParameterSet.
  //
  ParameterMap parameterMap;
  ParameterSet extraParameterSet;
  int nrParameters = parameterNames.size();
  for (int i = 0; i < nrParameters; ++i)
    {
      int name = parameterNames[i];
      Argument* argument = arguments[i];
      if (View* argumentView = dynamic_cast<View*>(argument))
	{
	  //
	  //	Our parameter is instantiated with view.
	  //	It could be a module-view with parameters bound by the
	  //	new enclosing object that requested this instantiation.
	  //
	  parameterMap[name] = argumentView->getCleanName();
	  ImportModule* toModule = argumentView->getToModule();
	  if (toModule->isTheory())
	    {
	      //
	      //	It's a theory view so parameter name survives.
	      //
	      extraParameterSet.insert(name);
	    }
	}
      else
	{
	  //
	  //	Our parameter is instantiated with with a parameter from
	  //	the new enclosing object.
	  //
	  parameterMap[name] = safeCast(Parameter*, argument)->id();
	}
    }
  //
  //	We now instantiate the renaming.
  //
  Renaming* newRenaming = instantiateRenaming(canonicalRenaming, parameterMap, extraParameterSet);
  //
  //	Now we make a renamed copy of our instantiated base. This will make a private
  //	canonical copy of newRenaming. It's quite possible that that the canonical
  //	renaming will be the empty renaming in which case instance = newBase.
  //
  ImportModule* instance = moduleCache->makeRenamedCopy(newBase, newRenaming);  // can return 0
  //
  //	This instantiated renaming is of no further use.
  //
  delete newRenaming;
  if (instance != 0)
    {
      DebugExit("instance: " << instance);
      return instance;
    }
  DebugExit("FAILED to rename " << newBase); 
  return 0;
}

Renaming*
ImportModule::instantiateRenaming(const Renaming* original,
				  const ParameterMap& parameterMap,
				  const ParameterSet& extraParameterSet)
{
  DebugEnter("Original renaming: " << original);
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
    for (int i = 0; i < nrOpMappings; ++i)
      {
	//
	//	We instantiate operator names as if they were sort names, because a parameterized
	//	constant name is a multi-token name that is never legal for a non-constant operator.
	//
	int fromName = instantiateSortName(original->getOpFrom(i), parameterMap, extraParameterSet);
	int toName = instantiateSortName(original->getOpTo(i), parameterMap, extraParameterSet);
	instance->addOpMappingPartialCopy(original, i, fromName, toName);
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
  {
    Vector<Token> token(1);
    int nrStratMappings = original->getNrStratMappings();
    for (int i = 0; i < nrStratMappings; i++)
      {
	instance->addStratMappingPartialCopy(original, i);
	int nrTypes = original->getNrStratTypes(i);
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
  DebugExit("Instantiated renaming: " << instance);
  return instance;
}

ImportModule*
ImportModule::handleFinalInstantiation(const Vector<Argument*>& arguments, ModuleCache* moduleCache)
{
  DebugEnter("original: " << this);
  Assert(!savedArguments.empty(), "no saved arguments in final instantiation");
  //
  //	We are an instantiation of a parameterized module with free parameters so
  //	we build a new set of arguments to instantiate our baseModule on.
  //
  //	We're going to go through the parameters of our base module. It could have more
  //	parameters than us because the instantiation that created us may have mapped
  //	multiple parameters in our base module to the same parameter from our enclosing
  //	object, or may have eliminated parameters with module-views.
  //
  int nrFreeParameters = baseModule->getNrParameters();
  Assert(nrFreeParameters == static_cast<int>(savedArguments.size()), "nrFreeParameters clash");
  Vector<Argument*> newArgs(nrFreeParameters);
  Vector<Argument*> extraArgs;

  for (int i = 0; i < nrFreeParameters; ++i)
    {
      //
      //	See what the free parameter was originally instantiated by.
      //
      Argument* a = savedArguments[i];
      if (Parameter* p = dynamic_cast<Parameter*>(a))
	{
	  //
	  //	The free parameter was originally instantiated by a parameter
	  //	from an enclosing object, making it bound in us. So we now look
	  //	up its name in our parameters to find the index of its
	  //	new instantiation in the argument list we were passed.
	  //
	  int boundParameterName = p->id();
	  DebugInfo("argument " << i << " of base module is parameter " << Token::name(boundParameterName));
	  int index = findParameterIndex(boundParameterName);
	  Assert(index != NONE,  "didn't find bound parameter");
	  Argument* newArgument = arguments[index];  // remember arguments was constructed to match our bound arguments vector
	  newArgs[i] = newArgument;
	  //
	  //	Need to check for passing a theory-view.
	  //
	  if (View* passedView = dynamic_cast<View*>(newArgument))
	    {
	      if (passedView->getToModule()->isTheory())
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
		  extraArgs.append(p);
		}
	    }
	}
      else
	{
	  View* v = safeCast(View*, a);
	  DebugInfo("argument " << i << " of base module is view " << v);
	  //
	  //	The free parameter was originally instantiated by a module-view
	  //	so this instantiation does not change except we may have to instantiate
	  //	bound parameters in the module-view.
	  //	Note that we cannot have a theory-view here since the instantiation
	  //	that made us must have been a final instantiation in order to have
	  //	parameters from an enclosing object.
	  //
	  Assert(!v->getToModule()->isTheory(), "unexpected theory-view in what should be a final instantiation" << v);
	  int nrViewParameters = v->getNrParameters();
	  if (nrViewParameters > 0)
	    {
	      //
	      //	The module-view has parameters which must be bound.
	      //	We go through these bound parameters, looking at their names.
	      //	We look them up in our parameters to get an index which tells
	      //	us which argument they will be instantiated with.
	      //	We construct argument lists and make an instantiation of the
	      //	module-view which we will use to instantiate our base module on.
	      //	
	      //
	      DebugInfo("view has " <<  nrViewParameters << " bound parameters");
	      Vector<Argument*> argumentsForView(nrViewParameters);
	      for (int j = 0; j < nrViewParameters; ++j)
		{
		  int boundParameterName = v->getParameterName(j);
		  int indexInUs = findParameterIndex(boundParameterName);
		  Assert(indexInUs != NONE, "couldn't find bound parameter " << Token::name(boundParameterName));
		  argumentsForView[j] = arguments[indexInUs];
		}
	      v = v->instantiateBoundParameters(argumentsForView);
	      //
	      //	We're not sure whether instantiating a legal view with bound
	      //	parameters can ever fail; and this could change in the future.
	      //	For robustness we assert that it can't so we notice it during
	      //	debugging and then handle in anyway.
	      //
	      Assert(v != 0, "failed to instantiate a view argument for an instantiation of " << this);
	      if (v == 0)
		return 0;
	    }
	  newArgs[i] = v;
	}
    }
  //
  //	We now instantiate our base module on these new argument lists.
  //
  if (ImportModule* instance = moduleCache->makeModuleInstantiation(baseModule, newArgs))
    {
      if (!extraArgs.empty())
	{
	  //
	  //	Because some of our parameters got instantiated by theory-views, they are still
	  //	free the instantiation we just made of our base module. We to do a final
	  //	intantiation using the extraArgs vector we accumulated to make them bound.
	  //
	  instance = moduleCache->makeModuleInstantiation(instance, extraArgs);
	}
      DebugExit("instance: " << instance);
      return instance;
    }
  DebugExit("FAILED to instantiate " << baseModule);
  return 0;
}
