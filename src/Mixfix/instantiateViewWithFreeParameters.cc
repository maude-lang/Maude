/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019-2025 SRI International, Menlo Park, CA 94025, USA.

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
//      Code to handle the instantiation of views with free parameters.
//

View*
View::makeInstantiation(int viewName,
			int cleanName,
			const Vector<Argument*>& arguments)
{
  DebugEnter("called on " << this << " to make " << Token::name(viewName));
  Assert(status != BAD, "trying to instantiate bad view");
  Assert(static_cast<int>(arguments.size()) == getNrParameters(), "viewArgs size mismatch " <<
	 getNrParameters() << " expected vs " << arguments.size() << " provided in instantiation " <<
	 Token::name(viewName));
  //
  //	An instantiation of a view (with free parameters) is a copy of the view
  //	with parameters that disappear (module-view argument), change theory
  //	(theory-view argument) or change name and become bound (parameter
  //	from an enclosing view/module).
  //
  View* copy = new View(viewName, cleanName, this, arguments, owner);
  //
  //	We construct an instantiation renaming which will map the sorts and operations
  //	we got from our parameter theories to those of our view args' targets.
  //	The main purpose of this renaming is to map terms from our toTheory
  //	into the toTheory of our instantiation, which is needed to instantiate
  //	our op->term mappings.
  //
  //	It's not possible to use the canonicalRenaming from the toModule of our
  //	instantiation for this, because the toModule could be a sum or a renaming
  //	rather than a direct instantiation.
  //
  Renaming* canonicalRenaming = new Renaming;
  //
  //	We assume that fromTheory cannot be instantiated, because we do not support
  //	parameterized theories. We do not allow parameterized modules to be summed
  //	with theories.
  //
  copy->fromTheory = fromTheory;  // use the same fromTheory
  fromTheory->addUser(copy);  // copy becomes a user of fromTheory
  //
  //	If we have theory-view args we will have free parameters.
  //	If we have parameter args we will have bound parameters.
  //	Can only have one of these possibilities since theory-view args
  //	are only allowed in non-final instantiations and parameter args
  //	are only allowed in final instantiations.
  //
  ParameterMap parameterMap;  // maps parameter names to arguments
  ParameterSet extraParameterSet;  // parameters that escape via a theory-view argument
  if (!handleInstantiationByTheoryView(copy, canonicalRenaming, parameterMap, extraParameterSet, arguments))
    {
      copy->status = BAD;
      delete canonicalRenaming;
      return copy;
    }
  //
  //	After we've checked for theory views on clashing parameters, it is now safe
  //	to make an instantiation of our toModule.
  //
  ImportModule* targetInstance = makeToModule(arguments);
  if (targetInstance == 0)
    {
      copy->status = BAD;
      delete canonicalRenaming;
      return copy;
    }
  copy->setToModule(targetInstance);  // propagates conflicts from targetInstance to copy
  //
  //	Now handle parameters instantiated by parameters from an enclosing object, and
  //	parameters instantiated by module-views that may use parameters from an enclosing object.
  //
  NatSet positionsInstantiatedParameter;
  handleInstantiationByParameter(copy, canonicalRenaming, parameterMap, positionsInstantiatedParameter, arguments);
  handleInstantiationByModuleView(copy, canonicalRenaming, parameterMap, positionsInstantiatedParameter, arguments);
  //
  //	Add renamings for parameterized sorts to canonicalRenaming. These look like
  //	  Foo{X} |-> Foo{V}{X} for theory-view V
  //	  Foo{X} |-> Foo{Y}    for parameter Y from an enclosing object
  //	  Foo{X} |-> Foo{V}    for module-view V
  //	We do this for all the sorts in our toModule
  //
  toModule->handleParameterizedSorts(canonicalRenaming, parameterMap, extraParameterSet);
  //
  //	CHECK: do we need this
  //
  toModule->handleParameterizedConstants(canonicalRenaming, parameterMap, extraParameterSet);
  //
  //	Now we have our canonical renaming which takes use from our toModule to
  //	the instantiation of our toModule, targetInstance.
  //	We now use this canonical renaming to instantiate the to-side of our
  //	sort and operator mappings.
  //
  //	Add a sort mapping to copy for each sort mapping we have.
  //
  handleSortMappings(copy, canonicalRenaming);
  //
  //	Add a specific op->op mappings to copy for operators in our fromTheory
  //	that get mapped by us and/or by our instantiation.
  //
  handleOpMappings(copy, canonicalRenaming);
  //
  //	Add a op->term mapping to copy for each op->term mapping we have.
  //
  handleOpToTermMappings(copy, canonicalRenaming);
  //
  //	Add a general op->op mappings to copy for polymorphic operators in our
  //	fromTheory that get mapped by us and/or by our instantiation.
  //
  handlePolymorphMappings(copy, canonicalRenaming);
  //
  //	Add strat->strat and strat->expr mappings
  //
  handleStratMappings(copy, canonicalRenaming);
  handleStratToExprMappings(copy, canonicalRenaming);
  
  copy->status = GOOD;
  DebugInfo("original view has renaming\n" << static_cast<Renaming*>(this));
  DebugInfo("canonical renaming for instantiation of view " << this << " is\n" << canonicalRenaming);
  DebugInfo("instantiated view has renaming\n" << static_cast<Renaming*>(copy));
  //
  //	The canonicalRenaming is solely used to instantiate the guts of the view;
  //	i.e. the sort->sort mappings, op->op mappings and op->term mappings.
  //   	Once this is done, it is never used again; unlike with modules, there are
  //	no delayed imports of things like equations.
  //
  delete canonicalRenaming;
 
  DebugExit("successfully made view instantiation " << Token::name(copy->id()) <<
	    " which has " << copy->parameters.size() << " parameters");
  return copy;
}

ImportModule*
View::makeToModule(const Vector<Argument*>& arguments) const
{
  //
  //	We make an instantiation of our toModule. We require that
  //	toModule have parameters that are bound to our own.
  //
  //	Because modules are a lot more substantial than views we will use
  //	the module machinery to do the heavy lifting.
  //
  Assert(!(toModule->hasFreeParameters()), "free parameters in toModule");
  int nrToModuleParameters = toModule->getNrParameters();
  Assert(nrToModuleParameters == getNrParameters(), "toModule " << toModule << " has " <<
	 nrToModuleParameters << " parameters whereas enclosing view " << this << " has " <<
	 getNrParameters() << " parameters");
  //
  //	We go through the bound parameters of our toModule construct a permutation of
  //	our argument list.
  //
  Vector<Argument*> toModuleArgs(nrToModuleParameters);
  for (int i = 0; i < nrToModuleParameters; ++i)
    {
      int parameterName = toModule->getParameterName(i);
      int indexInUs = findParameterIndex(parameterName);
      Assert(indexInUs != NONE, "couldn't find bound parameter " << Token::name(parameterName) <<
	     " for instantiation " << toModule << " of view " << this);
      toModuleArgs[i] = arguments[indexInUs];
    }
  //
  //	We now make an instance of toModule by instantiating its bound parameters.
  //
  //	We're not sure whether instantiating a legal module with bound parameters
  //	can ever fail; and this could change in the future.
  //	For robustness we assert that it can't so we notice it during
  //	debugging and then handle in anyway.
  //
  ImportModule* targetInstance = toModule->instantiateBoundParameters(toModuleArgs, owner);
  Assert(targetInstance != 0, "failed to instantiate toModule for " << this);
  return targetInstance;
}

bool
View::handleInstantiationByTheoryView(View* copy,
				      Renaming* canonicalRenaming,
				      ParameterMap& parameterMap,
				      ParameterSet& extraParameterSet,
				      const Vector<Argument*>& arguments) const
{
  DebugEnter("instantiating\n" << this);
  //
  //	We return true if all parameters taking theory-view arguments were
  //	successfully instantiated and false if there was a problem.
  //
  ParameterSet parametersTakingTheoryViews;
  int nrParameters = getNrParameters();
  for (int i = 0; i < nrParameters; ++i)
    {
      if (View* argumentView = dynamic_cast<View*>(arguments[i]))
	{
	  ImportModule* viewToTheory = argumentView->getToModule();
	  if (viewToTheory->isTheory())
	    {
	      //
	      //	Check to see if we can instantiate this parameter to a theory-view
	      //	without trigger a conflict with some other parameter already
	      //	instantiated to a theory-view. Note that a parameter can have
	      //	a conflict with itself in which case it can never take a theory-view.
	      //
	      int parameterName = parameters[i].name;
	      parametersTakingTheoryViews.insert(parameterName);
	      int firstClash = findFirstClash(parametersTakingTheoryViews, parameterName);
	      if (firstClash != NONE)
		{
		  if (firstClash == parameterName)
		    {
		      IssueWarning("In nonfinal view instantiation " << QUOTE(copy) << " parameter " <<
				   QUOTE(Token::name(parameterName)) <<
				   " may not take a theory-view because it has a self-conflict in the underlying view " <<
				   QUOTE(this) << ".");
		    }
		  else
		    {
		      IssueWarning("In nonfinal view instantiation " << QUOTE(copy) << " parameters " <<
				   QUOTE(Token::name(firstClash)) << " and " <<
				   QUOTE(Token::name(parameterName)) <<
				   " cannot both be instantiated by theory-views because of a conflict in the underlying view " <<
				   QUOTE(this) << ".");
		    }
		  return false;
		}    
	      //
	      //	A theory-view changes the theory of a parameter
	      //	while maintaining the parameter name.
	      //
	      copy->addParameter(parameterName, viewToTheory);  // copy becomes a user of the theory viewToModule
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
	      //	Since we store parameter theories rather than parameter theory copies (unlike ImportModule)
	      //	we need to make the copy - it will already be in the module cache since our toModule imports it.
	      //
	      Assert(argumentView->getFromTheory() == parameters[i].theory, "theory mismatch " <<
		     argumentView->getFromTheory() << " vs " << parameters[i].theory << " for argument " << i);
	      const ImportModule* parameterCopyOfParameterTheory = owner->makeParameterCopy(parameterName, parameters[i].theory);
	      //
	      //	Get the parameter copy of the theory-view's target. The target module for copy which is instantiated
	      //	by the theory-view and which we haven't at this point will also need this and  will get if from the cache.
	      //
	      ImportModule* parameterCopyOfArgumentViewToTheory = owner->makeParameterCopy(parameterName, viewToTheory);
	      //
	      //	We rely on the parameter copies to do the heavy lifting of
	      //	adding sort and operator renamings to our canonicalRenaming.
	      //
	      parameterCopyOfParameterTheory->addSortMappingsFromTheoryView(canonicalRenaming,
									    parameterName,
									    argumentView);
	      //
	      //	4/26/25 Previously we passed parameterCopyOfParameterTheory as the parameterCopyUser
	      //	that is used to look up sorts and compute types for the renaming, but this
	      //	introduced a subtle bug - toModule may have additional sorts in a given kind
	      //	which means that the types in canonical renaming may be missing those sorts
	      //	and if a missing sort is the Sort::FIRST_USER_SORT we can fail to match
	      //	specific op->op mappings and op->terms mappings. By passing toModule we avoid
	      //	this problem.
	      //
	      parameterCopyOfParameterTheory->addOpMappingsFromView(canonicalRenaming,
								    argumentView,
								    toModule,
								    parameterCopyOfArgumentViewToTheory);
	      parameterCopyOfParameterTheory->addStratMappingsFromView(canonicalRenaming,
								       argumentView,
								       toModule);
	    }
	}
    }
  DebugExit("instantiating\n" << this);
  return true;
}

void
View::handleInstantiationByParameter(View* copy,
				     Renaming* canonicalRenaming,
				     ParameterMap& parameterMap,
				     NatSet& positionsInstantiatedParameter,
				     const Vector<Argument*>& arguments) const
{
  DebugEnter("instantiating\n" << this);
  int nrParameters = getNrParameters();
  for (int i = 0; i < nrParameters; ++i)
    {
      if (Parameter* p = dynamic_cast<Parameter*>(arguments[i]))
	{
	  //
	  //	We map the old parameter name to the new parameter name.
	  //
	  int newParameterName = p->id();
	  int oldParameterName = parameters[i].name;
	  if (copy->findParameterIndex(newParameterName) == NONE)
	    {
	      //
	      //	Our instantiation doesn't already have a parameter
	      //	with this name so we must add one.
	      //
	      //	The base parameter theory doesn't change and views deal with
	      //	original theories, not parameter copies so we can reuse the
	      //	parameter theory from our base view.
	      //
	      ImportModule* baseParameterTheory = parameters[i].theory;
	      DebugInfo("binding parameter " << Token::name(oldParameterName) <<
			" to parameter " << Token::name(newParameterName) << " of theory " <<
			baseParameterTheory << " from an enclosing object");
	      copy->addParameter(newParameterName, baseParameterTheory);  // copy becomes a user of baseParameterTheory
	      copy->addBoundParameter(newParameterName);
	    }
	  if (oldParameterName != newParameterName)
	    {
	      //
	      //	If the parameter changed names, say from X to Y, then we need to map
	      //	both parameterized sorts and parameter copies of the theory sorts.
	      //
	      //	Sort{X} |-> Sort{Y}
	      //
	      parameterMap[oldParameterName] = newParameterName;
	      //
	      //	Since we store parameter theories rather than parameter theory copies (unlike ImportModule)
	      //	we need to make the copy - it will already be in the module cache since our toModule imports it.
	      //
	      ImportModule* parameterCopyOfTheory = owner->makeParameterCopy(oldParameterName, parameters[i].theory);
	      //
	      //	For each Foo -> X$Foo we mapped in the parameter copy of the parameter theory,
	      //	X$Foo |-> Y$Foo
	      //
	      parameterCopyOfTheory->addSortRenamingsForParameterChange(canonicalRenaming, newParameterName);
	      //
	      //	For each c : -> [S] |-> X$c we mapped in the parameter copy of the parameter theory, we
	      //	add a mapping X$c : -> [T] |-> Y$c to the canonical renaming where T is whatever
	      //	S mapped to in the parameter copy.
	      //
	      //	We require that toModule has each of parameters (bound by us), so we pass it as
	      //	the parameterCopyUser used to look up [T] once we map S |-> T by the
	      //	parameterCopyOfTheory canonical renaming.
	      //
	      parameterCopyOfTheory->addConstantRenamingsForParameterChange(canonicalRenaming,
									    newParameterName,
									    toModule);
	    }
	  positionsInstantiatedParameter.insert(i);
	  //
	  //	In principle we should propagate conflicts between positions instantiated by bare variables
	  //	into copy. In practice this is not necessary.
	  //	If a parameter is involved in a conflict, the conflict must have come from our toModule
	  //	and the parameter must be bound in our toModule.
	  //	Now copy will get a toModule that is an instantiated version of our toModule, and
	  //	module instantiation takes care of propagating conflicts on bare parameters. Thus
	  //	copy will eventually get any such conflicts from its toModule.
	  //
	}
    }
  DebugExit("instantiating\n" << this);
}

void
View::handleInstantiationByModuleView(View* copy,
				      Renaming* canonicalRenaming,
				      ParameterMap& parameterMap,
				      const NatSet& positionsInstantiatedParameter,
				      const Vector<Argument*>& arguments) const
{
  DebugEnter("instantiating\n" << this);
  int nrParameters = getNrParameters();
  for (int i = 0; i < nrParameters; ++i)
    {
      Argument* argument = arguments[i];
      if (View* argumentView = dynamic_cast<View*>(argument))
	{
	  ImportModule* viewToModule = argumentView->getToModule();
	  if (!(viewToModule->isTheory()))
	    {
	      DebugInfo("module view argument is " << argumentView << " which has target " << viewToModule);
	      //
	      //	We map the parmeter name to the argument view name.
	      //
	      int viewName = argumentView->getCleanName();
	      int parameterName = parameters[i].name;
	      if (parameterName != viewName)
		parameterMap[parameterName] = viewName;
	      //
	      //	Since we store parameter theories rather than parameter theory copies (unlike ImportModule)
	      //	we need to make the copy - it will already be in the module cache since our toModule imports it.
	      //
	      ImportModule* parameterCopyOfTheory = owner->makeParameterCopy(parameterName, parameters[i].theory);
	      //
	      //	For each Foo -> X$Foo we mapped in the parameter copy of the parameter theory,
	      //	we need to have a mapping X$Foo -> Bar where Bar is the target of Foo in our view.
	      //
	      //	4/26/25 Previously we passed parameterCopyOfTheory as the parameterCopyUser
	      //	that is used to look up sorts and compute types for the renaming, but this
	      //	introduced a subtle bug - toModule may have additional sorts in a given kind
	      //	which means that the types in canonical renaming may be missing those sorts
	      //	and if a missing sort is the Sort::FIRST_USER_SORT we can fail to match
	      //	specific op->op mappings and op->terms mappings. By passing toModule we avoid
	      //	this problem.
	      //
	      parameterCopyOfTheory->addSortMappingsFromModuleView(canonicalRenaming, argumentView);
	      parameterCopyOfTheory->addOpMappingsFromView(canonicalRenaming, argumentView, toModule);
	      parameterCopyOfTheory->addStratMappingsFromView(canonicalRenaming, argumentView, toModule);
	      //
	      //	Need to check if the argument view itself has bound parameters.
	      //
	      int nrBoundParameters = argumentView->getNrParameters();
	      if (nrBoundParameters != 0)
		{
		  DebugInfo("instantiation by module-view " << argumentView << " with bound parameters");
		  for (int j = 0; j < nrBoundParameters; ++j)
		    {
		      int parameterName = argumentView->getParameterName(j);
		      DebugInfo("--- looking at parameter " << Token::name(parameterName));
		      //
		      //	We need this parameter for our instantiation, because
		      //	it is need to instantiate a module-view. Thus it must
		      //	be added as a bound parameter for our instantiation.
		      //
		      if (copy->findParameterIndex(parameterName) == NONE)
			{
			  //
			  //	Our instantiation doesn't already have a parameter
			  //	with this name so we must add one.
			  //
			  //	The base parameter theory doesn't change and views deal with
			  //	original theories, not parameter copies so we can reuse the
			  //	parameter theory from our argument view.
			  //
			  ImportModule* baseParameterTheory = argumentView->getParameterTheory(j);
			  DebugInfo("--- for parameter " << Token::name(parameterName) <<
				    " of theory " << baseParameterTheory <<
				    " from enclosing object needed to instantiate argument view " <<
				    argumentView << " we make add it as a bound parameter");
			  copy->addParameter(parameterName, baseParameterTheory);  // copy becomes a user of baseParameterTheory
			  copy->addBoundParameter(parameterName);
			}
		    }
		}
	      //
	      //	Normally any conflicts in a module-view argument would also end up
	      //	propagated into the instantiation of our toModule and get into copy that way.
	      //	There is an exception though: currently we don't insist that every
	      //	parameter in a view be a bound parameter in the view's toModule.
	      //	In this unlikely case we need to add-in any conflicts from the
	      //	module-view to copy directly. So we do the add-in anyway.
	      //
	      //	5/9/18 We no longer allow toModules not to take all our parameters so we don't
	      //	do copy->addInAllConflicts(argumentView).
	      //
	      //	Need to add any conflicts between the bound parameters in our argument
	      //	and bare paramters in the same instantiation.
	      //	This is one of two places where new conflicts are generated ab initio.
	      //	If the instantiation looks like viewExp'{..., viewExpr,..., X,...}
	      //	then X will have a conflict with all bound parameters in viewExpr, since
	      //	it has the possibility of leaving them stranded in a nonfinal instantiation
	      //	should both recieve theory-views in a nonfinal instantiation.
	      //
	      for (int j : positionsInstantiatedParameter)
		{
		  int bareParameterName = safeCastNonNull<Parameter*>(arguments[j])->id();
		  copy->addConflictsWithBoundParameters(argumentView, bareParameterName);
		}
	    }
	}
    }
  DebugExit("instantiating\n" << this);
}

void
View::handleSortMappings(View* copy, const Renaming* canonicalRenaming) const
{
  //
  //	For each sort in fromTheory, we first push it through our own
  //	sort mappings and then the canonical renaming. This catches pathological
  //	cases where there is a sort X$Elt in fromTheory that is implicitly mapped
  //	to X$Elt in a parameterTheory and then needs to be mapped explicitly
  //	after instantiation. Not even clear we should allow such mischief but
  //	for the moment we support it.
  //
  int nrUserSorts = fromTheory->getNrUserSorts();
  const Vector<Sort*>& sortVec =  fromTheory->getSorts();
  for (int i = 0; i < nrUserSorts; ++i)
    {
      Sort* s = sortVec[i];
      if (!(fromTheory->moduleDeclared(s)))
	{
	  //
	  //	Only theory declared sorts get mapped.
	  //
	  int sortName = s->id();
	  int ourMapping = renameSort(sortName);
	  int instantiationMapping = canonicalRenaming->renameSort(ourMapping);
	  if (sortName != instantiationMapping)
	  copy->addSortMapping(sortName, instantiationMapping);
	}
    }
}

void
View::handleOpMappings(View* copy, const Renaming* canonicalRenaming) const
{
  //
  //	For each operator in fromTheory we first push it through our own
  //	op->op mappings (our op->term mappings are handled separately)
  //	and then through the canonical renaming. This catches the
  //	pathological case where there is an operator in fromTheory
  //	that is implicitly mapped to an operator in a parameter theory
  //	when its domain and/or range sorts are mapped (implicitly or
  //	explicitly) to those from a parameter theory.
  //
  DebugNew("canonicalRenaming = " << canonicalRenaming);
  int nrUserSymbols = fromTheory->getNrUserSymbols();
  const Vector<Symbol*>& symbolVec = fromTheory->getSymbols();
  for (int i = 0; i < nrUserSymbols; ++i)
    {
      Symbol* s = symbolVec[i];
      if (!(fromTheory->moduleDeclared(s)))
	{
	  //
	  //	Only theory declared operators get mapped.
	  //
	  Term* dummy1;
	  Term* dummy2;
	  if (getOpToTermMapping(s, dummy1, dummy2))
	    {
	      //
	      //	This operator is mapped via an op->term mapping that
	      //	we deal with elsewhere.
	      //
	      continue;
	    }

	  int oldId = s->id();
	  int id = oldId;
	  int index = renameOp(s);
	  if (index != NONE)
	    {
	      id = getOpTo(index);
	      DebugNew("first step " << s << " to " << Token::name(id));
	    }

	  int nrArgs = s->arity();
	  Vector<ConnectedComponent*> domainComponents(nrArgs);
	  for (int j = 0; j < nrArgs; ++j)
	    domainComponents[j] = mapComponent(s->domainComponent(j));
	  ConnectedComponent* rangeComponent = mapComponent(s->rangeComponent());
	  Symbol* t = toModule->findSymbol(id, domainComponents, rangeComponent);
	      
	  Assert(t != 0, "can't find operator " << Token::name(id) << " in module " << toModule <<
		 " that is translated from " << s << " in theory " << fromTheory <<
		 " by view " << this);

	  int index2 = canonicalRenaming->renameOp(t);
	  if (index2 != NONE)
	    {
	      Term* toTerm = canonicalRenaming->getOpTargetTerm(index2);
	      if (toTerm != 0)
		{
		  handleAwkwardCase(copy, s, canonicalRenaming->getFromTerm(index2), toTerm);
		  continue;
		}
	      id = canonicalRenaming->getOpTo(index2);
	      DebugNew("second step " << t << " to " << Token::name(id));
	    }
	  if (oldId != id)
	    {
	      //
	      //	Need to add an op->op mapping. We always make it specific.
	      //
	      copy->addOpMapping(oldId);
	      for (int i = 0; i < nrArgs; ++i)
		copy->addType(s->domainComponent(i));
	      copy->addType(s->rangeComponent());
	      copy->addOpTarget(id);
	      DebugNew("adding " << s << " to " << Token::name(id) << " for view instantiation " << this);
	    }
	}
    }
}

void
View::handleAwkwardCase(View* copy, Symbol* symbol, Term* fromTerm, Term* toTerm) const
{
  DebugEnter("view = " << copy << "  symbol = " << symbol << "  fromTerm = " << fromTerm <<
	     "  toTerm = " << toTerm);
  //
  //	We have symbol from our fromTheory which is explicitly or implicity
  //	mapped to a symbol' from one of our parameter theories. Now this parameter
  //	has been instantiated by a view that maps symbol' to a term toTerm.
  //	In order to handle this we need to synthesize an op->term mapping that
  //	takes symbol to toTerm and add it to copy.
  //	We do this an a very inefficient manner since this is a rare edge case.
  //
  int nrArgs = symbol->arity();
  //
  //	First we synthesize a fromTerm for symbol.
  //
  Vector<Term*> arguments(nrArgs);
  int j = 0;
  for (ArgumentIterator i(*fromTerm); i.valid(); i.next(), ++j)
    {
      VariableTerm* vt = safeCast(VariableTerm*, i.argument());
      int variableName = vt->id();
      const ConnectedComponent* component = symbol->domainComponent(j);
      Sort* sort = component->sort(Sort::KIND);
      VariableSymbol* variableSymbol = safeCast(VariableSymbol*, fromTheory->instantiateVariable(sort));
      arguments[j] = new VariableTerm(variableSymbol, variableName);
    }
  Term* newFromTerm = symbol->makeTerm(arguments);
  //
  //	We next make a copy of toTerm; this will not have variable indices, and
  //	will have each variable lifted to the kind level.
  //
  ImportTranslation toTermTranslation(copy->toModule, nullptr, false, true);
  Term* toTermCopy = toTerm->deepCopy(&toTermTranslation);
  //
  //	Finally we insert it into the view instantiation.
  //
  copy->insertOpToTermMapping(newFromTerm, toTermCopy);
  DebugExit("inserted " << newFromTerm << " to term " << toTermCopy << " into " <<
	   copy << " during the instantiation of " << this);
}

void
View::handleOpToTermMappings(View* copy, Renaming* canonicalRenaming) const
{
  if (!(opTermMap.empty()))
    {
      //
      //	For each operator mapping
      //	  op f(X, Y, Z) to term t(...)
      //	in us, we need to make an instantiated version in copy.
      //
      //	Since our fromTheory doesn't change, we just need to make an identity copy for the lhs.
      //
      ImportTranslation fromTermTranslation(fromTheory);
      //
      //	The rhs needs be pushed through the canonicaRenaming to deal with
      //	any operators from parameter theories that got instantiated by
      //	argumentsViews and any sort tests against sorts from parameter theories
      //	that have been instantiated.
      //
      ImportTranslation toTermTranslation(copy->toModule, canonicalRenaming);
      for (const auto& i : opTermMap)
	{
	  Term* fromTerm = i.second.first->deepCopy(&fromTermTranslation);
	  Term* toTerm = i.second.second->deepCopy(&toTermTranslation);
	  //
	  //	This is a slow way to get toTerm indexed and fromTerm and toTerm inserted
	  //	into the map - but we expect this case to be very rare so it's not worth
	  //	the code to do something smarter.
	  //
	  copy->insertOpToTermMapping(fromTerm, toTerm);
	  DebugNew("instantiating " << this << " to " << copy << " inserted op->term mapping " <<
		   fromTerm << " to " << toTerm);
	}
    }
}

void
View::handlePolymorphMappings(View* copy, const Renaming* canonicalRenaming) const
{
  int nrPolymorphs = fromTheory->getNrPolymorphs();
  for (int i = 0; i < nrPolymorphs; ++i)
    {
      if (!(fromTheory->moduleDeclaredPolymorph(i)))
	{
	  //
	  //	Only theory declared polymorphs get mapped.
	  //
	  int name = fromTheory->getPolymorphName(i).code();
	  //
	  //	See if mapped by us.
	  //
	  int index = renamePolymorph(name);
	  int newName = (index == NONE) ? name : getOpTo(index);
	  //
	  //	See if mapping in canonicalRenaming.
	  //
	  int index2 = canonicalRenaming->renamePolymorph(newName);
	  if (index2 != NONE)
	    newName = canonicalRenaming->getOpTo(index2);
	  //
	  //	If all this resulted in a name change, add a generic
	  //	operator mapping to our instantiation.
	  //
	  if (name != newName)
	    {
	      copy->addOpMapping(name);
	      copy->addOpTarget(newName);
	      DebugNew("instantiating " << this << " to " << copy << " inserted polymorph mapping " <<
		       Token::name(name) << " to " << Token::name(newName));
	    }
	}
    }
}

void
View::handleStratMappings(View* copy, const Renaming* canonicalRenaming) const
{
  //
  //	strat->strat mapping are handled similarly to op->op
  //	(strat->expr mappings are handled separately too)
  //
  DebugNew("canonicalRenaming = " << canonicalRenaming);
  const Vector<RewriteStrategy*>& strategies = fromTheory->getStrategies();
  for (RewriteStrategy* s : strategies)
    {
      if (!(fromTheory->moduleDeclared(s)))
	{
	  //
	  //	Only theory declared strategies get mapped.
	  //
	  CallStrategy* dummy1;
	  StrategyExpression* dummy2;
	  const Vector<int>* dummy3;
	  if (getStratToExprMapping(s, dummy1, dummy2, dummy3))
	    {
	      //
	      //	This strategy is mapped via an strat->expr mapping that
	      //	we deal with elsewhere.
	      //
	      continue;
	    }

	  int oldId = s->id();
	  int id = oldId;
	  int index = renameStrat(s);
	  if (index != NONE)
	    {
	      id = getStratTo(index);
	      DebugNew("first step " << s << " to " << Token::name(id));
	    }

	  int nrArgs = s->arity();
	  const Vector<Sort*>& domainSorts = s->getDomain();
	  Vector<ConnectedComponent*> domainComponents(nrArgs);
	  for (int j = 0; j < nrArgs; ++j)
	    domainComponents[j] = mapComponent(domainSorts[j]->component());
	  RewriteStrategy* t = toModule->findStrategy(id, domainComponents);

	  Assert(t != 0, "can't find strategy " << Token::name(id) << " in module " << toModule <<
		 " that is translated from " << s << " in theory " << fromTheory <<
		 " by view " << this);

	  int index2 = canonicalRenaming->renameStrat(t);
	  if (index2 != NONE)
	    {
	      StrategyExpression* toExpr = canonicalRenaming->getStratTargetExpr(index2);
	      if (toExpr != 0)
		{
		  handleStratAwkwardCase(copy, s, canonicalRenaming->getStratFromCall(index2),
					 toExpr, canonicalRenaming->getStratVarIndices(index2));
		  continue;
		}
	      id = canonicalRenaming->getStratTo(index2);
	      DebugNew("second step " << t << " to " << Token::name(id));
	    }
	  if (oldId != id)
	    {
	      //
	      //	Need to add an op->op mapping. We always make it specific.
	      //
	      copy->addStratMapping(oldId);
	      for (int i = 0; i < nrArgs; ++i)
		copy->addType(domainSorts[i]->component());
	      copy->addStratTarget(id);
	      DebugNew("adding " << s << " to " << Token::name(id) << " for view instantiation " << this);
	    }
	}
    }
}

void
View::handleStratAwkwardCase(View* copy,
			     RewriteStrategy* s,
			     CallStrategy* fromCall,
			     StrategyExpression* toExpr,
			     const Vector<int>& varIndices) const
{
  //
  //	This is the same case as handleAwkwardCase, but for strategies.
  //
  int nrArgs = s->arity();
  //
  //	First we synthesize a fromTerm for symbol.
  //
  Vector<Term*> arguments(nrArgs);
  const Vector<Sort*>& domain = s->getDomain();
  int j = 0;
  for (ArgumentIterator i(*fromCall->getTerm()); i.valid(); i.next(), ++j)
    {
      VariableTerm* vt = safeCast(VariableTerm*, i.argument());
      int variableName = vt->id();
      const ConnectedComponent* component = domain[j]->component();
      Sort* sort = component->sort(Sort::KIND);
      VariableSymbol* variableSymbol = safeCast(VariableSymbol*, fromTheory->instantiateVariable(sort));
      arguments[j] = new VariableTerm(variableSymbol, variableName);
    }
  CallStrategy* newFromCall = new CallStrategy(s, s->makeAuxiliaryTerm(arguments));
  //
  //	We next make a copy of toExpr; this will not have variable indices, and
  //	will have each variable lifted to the kind level.
  //
  ImportTranslation toExprTranslation(copy->toModule, nullptr, false, true);
  StrategyExpression* toExprCopy = ImportModule::deepCopyStrategyExpression(&toExprTranslation, toExpr);
  //
  //	Finally we insert it into the view instantiation.
  //
  copy->insertStratToExprMapping(newFromCall, toExprCopy, copy->toModule);
  copy->addStratMappingVarIndices(varIndices);
  DebugNew("inserted " << newFromCall << " to expr " << toExprCopy << " into " <<
	   copy << " during the instantiation of " << this);
}

void
View::handleStratToExprMappings(View* copy, Renaming* canonicalRenaming) const
{
  if (!(stratExprMap.empty()))
    {
      //
      //	For each strategy mapping
      //	  strat f(X, Y, Z) to expr e(...)
      //	in us, we need to make an instantiated version in copy.
      //
      //	Since our fromTheory doesn't change, we just need to make an identity copy for the lhs.
      //
      ImportTranslation fromCallTranslation(fromTheory);
      //
      //	The rhs needs be pushed through the canonicaRenaming to deal with
      //	any operators from parameter theories that got instantiated by
      //	argumentsViews and any sort tests against sorts from parameter theories
      //	that have been instantiated.
      //
      ImportTranslation toExprTranslation(copy->toModule, canonicalRenaming);
      for (const auto& i : stratExprMap)
	{
	  CallStrategy* fromCall = static_cast<CallStrategy*>(ImportModule::deepCopyStrategyExpression(&fromCallTranslation, i.second.call));
	  StrategyExpression* toExpr = ImportModule::deepCopyStrategyExpression(&toExprTranslation, i.second.value);

	  copy->insertStratToExprMapping(fromCall, toExpr, copy->toModule);
	  DebugNew("instantiating " << this << " to " << copy << " inserted strat->expr mapping " <<
		   fromCall << " to " << toExpr);
	}
    }
}
