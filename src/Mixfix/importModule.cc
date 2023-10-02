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
//      Implementation for class ImportModule.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "term.hh"
#include "binarySymbol.hh"

//      core class definitions
#include "equation.hh"
#include "rule.hh"
#include "sortConstraint.hh"
#include "conditionFragment.hh"
#include "rewriteStrategy.hh"
#include "strategyDefinition.hh"

//     higher class definitions
#include "equalityConditionFragment.hh"
#include "sortTestConditionFragment.hh"
#include "assignmentConditionFragment.hh"
#include "rewriteConditionFragment.hh"

//	front end class definitions
#include "renaming.hh"
#include "view.hh"
#include "importModule.hh"
#include "moduleCache.hh"
#include "importTranslation.hh"
#include "quotedIdentifierSymbol.hh"
#include "parameter.hh"

//	strategy language class definitions
#include "strategyExpression.hh"

//	our stuff
#include "ooSorts.cc"
#include "pseudoParameters.cc"
#include "renameModule.cc"
#include "parameterization.cc"
#include "instantiateModuleWithFreeParameters.cc"
#include "instantiateModuleWithBoundParameters.cc"
#include "latexImportModule.cc"

ImportModule::ImportModule(int name, ModuleType moduleType)
  : MixfixModule(name, moduleType),
    origin(TEXT)
{
  //
  //	This version is for modules created as subclass VisibleModule, either from
  //	object level syntax or meta-syntax, both of which are considered TEXT.
  //
  importPhase = UNVISITED;
  parameterCopyParameterName = NONE;
  nrSortsFromParameters = 0;
  nrSymbolsFromParameters = 0;
  nrPolymorphsFromParameters = 0;
  protectCount = 0;
  canonicalRenaming = 0;
  baseModule = 0;
}

ImportModule::ImportModule(int name, ModuleType moduleType, Origin origin, Entity::User* parent)
  : MixfixModule(name, moduleType),
    origin(origin)
{
  //
  //	This version is for modules created from ModuleExpressions or for View op->term mappings.
  //	In particular we take an origin so we know where we came from and a parent to
  //	send a regretToInform() message when we are deleted.
  //
  addUser(parent);
  importPhase = UNVISITED;
  parameterCopyParameterName = NONE;
  nrSortsFromParameters = 0;
  nrSymbolsFromParameters = 0;
  nrPolymorphsFromParameters = 0;
  nrStrategiesFromParameters = 0;
  protectCount = 0;
  canonicalRenaming = 0;
  baseModule = 0;
}

ImportModule::~ImportModule()
{
  Assert(canonicalRenaming == 0, "undeleted canonicalRenaming in " << this);
}

const char*
ImportModule::getObjectType() const
{
  return "module";
}

const NamedEntity*
ImportModule::getObjectName() const
{
  return this;
}

const char*
ImportModule::importModeString(ImportMode mode)
{
  switch (mode)
    {
    case INCLUDING:
      return "including";
    case GENERATED_BY:
      return "generated-by";
    case EXTENDING:
      return "extending";
    case PROTECTING:
      return "protecting";
    }
  CantHappen("bad importation mode" << mode);
  return 0;
}

void
ImportModule::addImport(ImportModule* importedModule,
			ImportMode mode,
			LineNumber lineNumber)
{
  ModuleType t = importedModule->getModuleType();
  if (isTheory(t) && mode != INCLUDING)
    {
      IssueWarning(lineNumber << ": theories may only be imported using the " <<
		   QUOTE("including") <<
		   " importation mode. Recovering by treating mode as including.");
      mode = INCLUDING;
    }
  if (!canImport(getModuleType(), t))
    {
      //
      //	Allowing modules to import theories would allow inconsistancies
      //	to get into the parameterization mechanism. So we just disallow
      //	illegal imports to avoid having to deal with tricky cases.
      //
      IssueWarning(lineNumber << ": importation of " <<
		   QUOTE(moduleTypeString(t)) << " " <<
		   QUOTE(importedModule) << " by " <<
		   QUOTE(moduleTypeString(getModuleType())) << " " <<
		   QUOTE(this) << " not allowed.  Recovering by ignoring import.");
      return;
    }
  if (!parameterNames.empty())
    {
      const EnclosingObject::ParameterSet& pseudos = importedModule->getPseudoParameters();
      for (int id : parameterNames)
	{
	  if (pseudos.find(id) != pseudos.end())
	    {
	      IssueWarning(lineNumber << ": importation of " <<
			   QUOTE(moduleTypeString(t)) << " " << QUOTE(importedModule) << " by " <<
			   QUOTE(moduleTypeString(getModuleType())) << " " << QUOTE(this) <<
			   " not allowed because the pseudo-parameter " << QUOTE(Token::name(id)) <<
			   " of " << QUOTE(moduleTypeString(t)) << " " << QUOTE(importedModule) <<
			   " would be captured by an actual parameter of " <<
			   QUOTE(moduleTypeString(getModuleType())) << " " << QUOTE(this) <<
			   ".");
	      return;
	    }
	}
    }
  importedModules.append(importedModule);
  importModes.append(mode);
  importedModule->addUser(this);
  //
  //	The imported module may have conflicts between its bound parameters, which we
  //	need to propagate to conflicts between our parameters.
  //
  addInAllConflicts(importedModule);
}

void
ImportModule::addParameter(const Token parameterName,
			   ImportModule* parameterTheory)
{
  if (findParameterIndex(parameterName.code()) != NONE)
    {
      IssueWarning(LineNumber(parameterName.lineNumber()) <<
		   ": there is already a parameter called " << QUOTE(parameterName) <<
		   ". Recovering by ignoring parameter.");
      return;
    }

  ModuleType t = parameterTheory->getModuleType();
  WarningCheck(canHaveAsParameter(getModuleType(), t),
	       LineNumber(parameterName.lineNumber()) << ": parameterization of " <<
	       QUOTE(moduleTypeString(getModuleType())) << " " <<
	       QUOTE(this) << " by parameter " << QUOTE(parameterName) << " of " <<
	       QUOTE(moduleTypeString(t)) << " " <<
	       QUOTE(parameterTheory) << " not allowed."); 
	     
  parameterNames.append(parameterName.code());
  parameterTheories.append(parameterTheory);
  parameterTheory->addUser(this);
}

int
ImportModule::findParameterIndex(int name) const
{
  int nrParameters = parameterNames.size();
  for (int i = 0; i < nrParameters; ++i)
    {
      if (parameterNames[i] == name)
	return i;
    }
  return NONE;
}

ImportModule*
ImportModule::getParameterTheory(int index) const
{
  Assert(index < getNrParameters(), "bad parameter index " << index << " in module " << (const MixfixModule*) this);
  //
  //	The actual theory stored in parameterTheories[index] is a parameter copy; i.e. if we
  //	had parameter X :: TRIV then the parameter copy is a theory called X :: TRIV containing
  //	sort X$Elt. But here we want the underlying theory, so we return the baseModule it was
  //	derived from, in this case TRIV.
  //
  //	We don't put this function in our header file despite its simplicity because
  //	it is virtual in our base class EnclosingObject.
  //
  return parameterTheories[index]->baseModule;
}

void
ImportModule::closeSortSet()
{
  nrUserSorts = getSorts().length();  // exclude error sorts
  MixfixModule::closeSortSet();
  nrUserComponents = (isStrategic() ? getConnectedComponents().length() - 1:
				      getConnectedComponents().length()); 
}

void
ImportModule::closeSignature()
{
  DebugAdvisory("closeSignature() on " << this);

  const Vector<Symbol*>& symbols = getSymbols();
  nrUserSymbols = symbols.length();  // exclude polymorph instances
  nrUserDecls.expandTo(nrUserSymbols);
  for (int i = 0; i < nrUserSymbols; i++)
    nrUserDecls[i] = symbols[i]->getOpDeclarations().length();  // exclude comm completions
  MixfixModule::closeSignature();
  //
  //	We need to do this after all of our imported operators have been
  //	inserted and all of our own operators have been inserted.
  //
  if (getNrParameters() != 0)
    checkForPolymorphOperatorClash();
}

void
ImportModule::checkForPolymorphOperatorClash()
{
  //
  //	We're a parameterized module. If we have two polymorphic operators
  //	with the same name, some weird situations can occur when we are
  //	instantiated and they have to be renamed in different directions.
  //	We avoid a lot of complexity by simply disallowing this highly
  //	pathological case.
  //
  set<int> seen;
  int nrPolymorphs = getNrPolymorphs();
  for (int i = 0; i < nrPolymorphs; ++i)
    {
      int id = getPolymorphName(i).code();
      if (seen.find(id) != seen.end())
	{
	  IssueWarning(*this << ": parameterized module " << QUOTE(this) <<
		       " has multiple polymorphic operators with name " << QUOTE(Token::name(id)) << ".");
	  markAsBad();  // we will never be instantiated
	}
      else
	seen.insert(id);
    }
}

void
ImportModule::regretToInform(Entity* /* doomedEntity */)
{
  //
  //	Something that we depend on is about to disappear - so we must self destruct.
  //
  deepSelfDestruct();
}

void
ImportModule::deepSelfDestruct()
{
  //
  //	First remove ourself from the list of users of each of our imports, parameters,
  // 	view arguments and base module. This is so we will not receive a regretToInform()
  //	message after we delete ourself.
  //
  for (ImportModule* m : parameterTheories)
    m->removeUser(this);
  for (ImportModule* m : importedModules)
    m->removeUser(this);
  for (Argument* arg : savedArguments)
    {
      if (View* v = dynamic_cast<View*>(arg))
	v->removeUser(this);
    }
  if (baseModule != 0)
    baseModule->removeUser(this);
  //
  //	Now we inform all our users of our impending demise.
  //
  informUsers();
  //
  //	Delete our canonical renaming.
  //
  delete canonicalRenaming;
#ifndef NO_ASSERT
  canonicalRenaming = 0;
#endif
  //
  //	And then delete ourself or mark ourself for deletion.
  //
  DebugAdvisory("module " << this << " invalidated");
  if (protectCount > 0)
    importPhase = DOOMED;
  else
    delete this;
}

bool
ImportModule::unprotect()
{
  Assert(protectCount > 0, "trying to decrement protectCount of " << protectCount);
  --protectCount;
  if (protectCount == 0 && importPhase == DOOMED)
    {
      DebugAdvisory("module " << this << " deleted after delay");
      delete this;
      return true;
    }
  return false;
}

//
//	Routines for sort and subsort importation.
//
void
ImportModule::importSorts()
{
  //
  //	We first go through our parameter theories and ask them
  //	to donate their sorts.
  for (ImportModule* i : parameterTheories)
    i->donateSorts(this);
  //
  //	We just imported the sorts from our last parameter theory
  //	so record this so we know what sorts came from parameters.
  //
  nrSortsFromParameters = getSorts().size();
  //
  //	We now go through our regular imports and ask them to
  //	donate their sorts.
  //
  for (ImportModule* i : importedModules)
    i->donateSorts(this);
  //
  //	Keep track of which sort and subsort declarations were imported
  //	from parameter theories or regular imports.
  //
  const Vector<Sort*> sorts = getSorts();
  nrImportedSorts = sorts.length();
  nrImportedSubsorts.expandTo(nrImportedSorts);
  for (int i = 0; i < nrImportedSorts; i++)
    nrImportedSubsorts[i] = sorts[i]->getSubsorts().length();
}

void
ImportModule::donateSorts(ImportModule* importer)
{
  Assert(!isBad(), "original module bad " << this);

  if (importPhase == SORTS_IMPORTED)
    return;
  Assert(importPhase == UNVISITED, "bad importPhase = " << importPhase);
  importPhase = SORTS_IMPORTED;
  //
  //	First recurse to handle our own imports (but not our parameterTheories
  //	- importer should have identical parameterTheories because our parameter
  //	must be bound.
  //
  for (ImportModule* i : importedModules)
    i->donateSorts(importer);
  //
  //	Now handle our sorts.
  //
  donateSorts2(importer);
}

//
//	Routines for operator importation.
//
void
ImportModule::importOps()
{
  //
  //	We first go through our parameter theories and ask them
  //	to donate their operators.
  for (ImportModule* i : parameterTheories)
    i->donateOps(this);
  //
  //	We just imported the operators from our last parameter theory
  //	so record record the number of symbols and polymorphs we
  //	got from parameters.
  //
  nrSymbolsFromParameters = getSymbols().size();
  nrPolymorphsFromParameters = getNrPolymorphs();
  //
  //	We now go through our regular imports and ask them to
  //	donate their operators.
  //
  for (ImportModule* i : importedModules)
    i->donateOps(this);
  //
  //	Keep track of which operators and operator declarations were
  //	imported from parameter theories or regular imports.
  //
  const Vector<Symbol*>& symbols = getSymbols();
  nrImportedSymbols = symbols.length();
  nrImportedDecls.expandTo(nrImportedSymbols);
  for (int i = 0; i < nrImportedSymbols; i++)
    nrImportedDecls[i] = symbols[i]->getOpDeclarations().length();
  nrImportedPolymorphs = getNrPolymorphs();
}

void
ImportModule::donateOps(ImportModule* importer)
{
  Assert(!isBad(), "original module bad " << this);

  if (importPhase == OPS_IMPORTED)
    return;
  Assert(importPhase == SORTS_IMPORTED, "bad importPhase = " << importPhase);
  importPhase = OPS_IMPORTED;
  //
  //	First recurse to handle our own imports (but not our parameterTheories
  //	- importer should have identical parameterTheories because our parameter
  //	must be bound.
  //
  for (ImportModule* i : importedModules)
    i->donateOps(importer);
  //
  //	Now handle our operators.
  //
  donateOps2(importer);
}

//
//	Routines for fixing up imported operations.
//
void
ImportModule::fixUpImportedOps()
{
  //
  //	We first go through our parameter theories and ask them
  //	to fix up the operators they donated.
  for (ImportModule* i : parameterTheories)
    i->fixUpDonatedOps(this);
  //
  //	We now go through our regular imports and ask them to
  //	fix up the operators they donated.
  //
  for (ImportModule* i : importedModules)
    i->fixUpDonatedOps(this);
}

void
ImportModule::fixUpDonatedOps(ImportModule* importer)
{
  if (importPhase == OPS_FIXED_UP)
    return;
  Assert(importPhase == STRATS_IMPORTED, "bad importPhase = " << importPhase);
  importPhase = OPS_FIXED_UP;
  //
  //	First recurse to handle our own imports (but not our parameterTheories
  //	- importer should have identical parameterTheories because our parameter
  //	must be bound.
  //
  for (ImportModule* i : importedModules)
    i->fixUpDonatedOps(importer);
  //
  //	Handle our operators.
  //
  fixUpDonatedOps2(importer);
}

//
//	Routines for statement importation.
//

void
ImportModule::importStrategies()
{
  //
  //	We first fo through our parameter theories and ask them
  //	to donate their strategies.
  for (ImportModule* m : parameterTheories)
    m->donateStrategies(this);
   //
  //	We just imported the operators from our last parameter theory,
  //	so we record the number of strategies we got from parameters.
  //
  nrStrategiesFromParameters = getStrategies().size();
  //
  //	We now go through our regular imports and ask them to
  //	donate their strategies.
  //
  for (ImportModule* m : importedModules)
    m->donateStrategies(this);
  //
  //	Keep track of which strategies were imported from parameter
  //	theories or regular imports.
  //
  nrImportedStrategies = getStrategies().length();
}

void
ImportModule::importStatements()
{
  DebugEnter("for module " << this);
  //
  //	We first go through our parameter theories and ask them
  //	to donate their statements.
  //
  for (ImportModule* i : parameterTheories)
    i->donateStatements(this);
  //
  //	We now go through our regular imports and ask them to
  //	donate their statements.
  //
  for (ImportModule* i : importedModules)
    i->donateStatements(this);
}

void
ImportModule::donateStatements(ImportModule* importer)
{
  DebugEnter("from module " << this);
  if (importPhase == STATEMENTS_IMPORTED)
    return;
  //
  //	Normally statements are imported on demand after resetting, but in metalevel they can
  //	be imported immediately after the operator fixup phase.
  //
  Assert(importPhase == UNVISITED || importPhase == OPS_FIXED_UP, "bad importPhase = " << importPhase);
  importPhase = STATEMENTS_IMPORTED;
  //
  //	First recurse to handle our own imports (but not our parameterTheories
  //	- importer should have identical parameterTheories because our parameter
  //	must be bound.
  //
  for (ImportModule* i : importedModules)
    i->donateStatements(importer);
  //
  //	The map from imported module's symbols to importing module's symbols
  //	is built dynamically.
  //
  ImportTranslation importTranslation(importer);    
  donateStatements2(importer, importTranslation);
}

void ImportModule::importRuleLabels()
{
  //
  //	We assume parent modules have already imported labels
  //	from their own imported modules and have included theirs
  //
  for (ImportModule* m : parameterTheories)
      ruleLabels.insert(m->ruleLabels.begin(), m->ruleLabels.end());

  for (ImportModule* m : importedModules)
      ruleLabels.insert(m->ruleLabels.begin(), m->ruleLabels.end());
}

void
ImportModule::deepCopyCondition(ImportTranslation* importTranslation,
				const Vector<ConditionFragment*>& original,
				Vector<ConditionFragment*>& copy)
{
  int nrFragments = original.length();
  copy.expandTo(nrFragments);
  for (int i = 0; i < nrFragments; i++)
    {
      ConditionFragment* c = original[i];
      ConditionFragment* n;
      if (EqualityConditionFragment* e = dynamic_cast<EqualityConditionFragment*>(c))
	{
	  n = new EqualityConditionFragment(e->getLhs()->deepCopy(importTranslation),
					    e->getRhs()->deepCopy(importTranslation));
	}
      else if (SortTestConditionFragment* s = dynamic_cast<SortTestConditionFragment*>(c))
	{
	  n = new SortTestConditionFragment(s->getLhs()->deepCopy(importTranslation),
					    importTranslation->translate(s->getSort()));
	}
      else if (AssignmentConditionFragment* a = dynamic_cast<AssignmentConditionFragment*>(c))
	{
	  n = new AssignmentConditionFragment(a->getLhs()->deepCopy(importTranslation),
					      a->getRhs()->deepCopy(importTranslation));
	}
      else if (RewriteConditionFragment* r = dynamic_cast<RewriteConditionFragment*>(c))
	{
	  n = new RewriteConditionFragment(r->getLhs()->deepCopy(importTranslation),
					   r->getRhs()->deepCopy(importTranslation));
	}
      else
	{
	  CantHappen("bad condition fragment");
	  n = 0;  // avoid compiler warning
	}
      copy[i] = n;
    }
}

void
ImportModule::donateRuleLabels(ImportModule* importer, Renaming* renaming)
{
  for (int label : ruleLabels)
    importer->ruleLabels.insert(renaming ? renaming->renameLabel(label) : label);
}

void
ImportModule::donateStrategies(ImportModule* importer)
{
  if (importPhase == STRATS_IMPORTED)
    return;
  Assert(importPhase == OPS_IMPORTED, "bad importPhase for strategies = " << importPhase);
  importPhase = STRATS_IMPORTED;

  //
  //	First recurse to handle our own imports (but not our parameterTheories
  //	- importer should have identical parameterTheories because our parameter
  //	must be bound.
  //
  for (ImportModule* m : importedModules)
    m->donateStrategies(importer);

  donateStrategies2(importer);
}

void
ImportModule::copyMetadata(ImportModule* importer,
			   ImportTranslation& importTranslation,
			   ItemType itemType,
			   PreEquation* original,
			   PreEquation* copy)
{
  importer->insertMetadata(itemType, copy, getMetadata(itemType, original));
  const PrintAttribute* p = getPrintAttribute(itemType, original);
  if (p != 0)
    {
      int nrItems = p->getNrItems();
      Vector<int> names(nrItems);
      Vector<Sort*> sorts(nrItems);
      for (int i = 0; i < nrItems; ++i)
	{
	  int code = p->getTokenCode(i);
	  if (code == NONE)
	    {
	      int index = p->getVariableIndex(i);
	      VariableTerm* v = safeCast(VariableTerm*, original->index2Variable(index));
	      names[i] = v->id();
	      Sort* sort = v->getSort();
	      sorts[i] = importTranslation.translate(sort);
	    }
	  else
	    {
	      names[i] = code;
	      sorts[i] = 0;
	    }
	}
      importer->insertPrintAttribute(itemType, copy, names, sorts);
    }
}

void
ImportModule::donateStrategies2(ImportModule* importer, Renaming* renaming)
{
  DebugAdvisory("donateStrategies2(), from " << this << " to " << importer);
  Assert(!isBad(), "original module bad " << this);

  bool moduleDonatingToTheory = importer->isTheory() && !isTheory();

  const Vector<RewriteStrategy*>& strategies = getStrategies();
  int nrStrategies = strategies.size();
  for (int i = nrImportedStrategies; i < nrStrategies; i++)
    {
      const Vector<Sort*>& domain = strategies[i]->getDomain();
      int nrArgs = domain.length();

      // Then we translate the strategy name and its domain and subject sort
      int renamingIndex = renaming ? renaming->renameStrat(strategies[i]) : NONE;
      int label = renamingIndex != NONE ? renaming->getStratTo(renamingIndex) : strategies[i]->id();
      Sort* subjectSort = localSort(importer, renaming, strategies[i]->getSubjectSort());

      Vector<Sort*> translatedDomain(nrArgs);
      for (int j = 0; j < nrArgs; j++)
	translatedDomain[j] = localSort(importer, renaming, domain[j]);

      Token name;
      name.tokenize(label, strategies[i]->getLineNumber());
      int index = importer->addStrategy(name,
					translatedDomain,
					subjectSort,
					getMetadata(STRAT_DECL, strategies[i]),
					true);

      if (moduleDonatingToTheory)
	importer->stratDeclaredInModule.insert(index);
    }
}

void
ImportModule::donateStatements2(ImportModule* importer, ImportTranslation& importTranslation)
{
  if (canonicalRenaming != 0)
    {
      importTranslation.push(canonicalRenaming, this);
      baseModule->donateStatements2(importer, importTranslation);
      return;
    }
  //
  //	Handle our membership axioms.
  //
  const Vector<SortConstraint*>& membershipAxioms = getSortConstraints();
  for (int i = 0; i < nrOriginalMembershipAxioms; i++)
    {
      SortConstraint* ma = membershipAxioms[i];
      if (!(ma->isBad()))
	{
	  int label = importTranslation.translateLabel(ma->getLabel().id());
	  Term* lhs = ma->getLhs()->deepCopy(&importTranslation);
	  Sort* sort = importTranslation.translate(ma->getSort());
	  Vector<ConditionFragment*> condition;
	  deepCopyCondition(&importTranslation, ma->getCondition(), condition);
	  SortConstraint* copy = new SortConstraint(label, lhs, sort, condition);
	  if (ma->isNonexec())
	    copy->setNonexec();
	  copy->setLineNumber(ma->getLineNumber());
	  importer->insertSortConstraint(copy);
	  importer->checkSortConstraint(copy);
	  copyMetadata(importer, importTranslation, MEMB_AX, ma, copy);
	  //importer->insertMetadata(MEMB_AX, copy, getMetadata(MEMB_AX, ma));
	}
    }
  //
  //	Now handle our equations.
  //
  const Vector<Equation*>& equations = getEquations();
  for (int i = 0; i < nrOriginalEquations; i++)
    {
      Equation* e = equations[i];
      if (!(e->isBad()))
	{
	  int label = importTranslation.translateLabel(e->getLabel().id());
	  Term* lhs = e->getLhs()->deepCopy(&importTranslation);
	  Term* rhs = e->getRhs()->deepCopy(&importTranslation);
	  Vector<ConditionFragment*> condition;
	  deepCopyCondition(&importTranslation, e->getCondition(), condition);
	  Equation* copy = new Equation(label, lhs, rhs, e->isOwise(), condition);
	  if (e->isNonexec())
	    copy->setNonexec();
	  if (e->isVariant())
	    copy->setVariant();
	  copy->setLineNumber(e->getLineNumber());
	  importer->insertEquation(copy);
	  importer->checkEquation(copy);
	  copyMetadata(importer, importTranslation, EQUATION, e, copy);
	  //importer->insertMetadata(EQUATION, copy, getMetadata(EQUATION, e));
	}
    }
  //
  //	Now handle our rules.
  //
  const Vector<Rule*>& rules = getRules();
  for (int i = 0; i < nrOriginalRules; i++)
    {
      Rule* r = rules[i];
      if (!(r->isBad()))
	{
	  int label = importTranslation.translateLabel(r->getLabel().id());
	  Term* lhs = r->getLhs()->deepCopy(&importTranslation);
	  Term* rhs = r->getRhs()->deepCopy(&importTranslation);
	  Vector<ConditionFragment*> condition;
	  deepCopyCondition(&importTranslation, r->getCondition(), condition);
	  Rule* copy = new Rule(label, lhs, rhs, condition);
	  if (r->isNonexec())
	    copy->setNonexec();
	  if (r->isNarrowing())
	    copy->setNarrowing();
	  copy->setLineNumber(r->getLineNumber());
	  importer->insertRule(copy);
	  importer->checkRule(copy);
	  copyMetadata(importer, importTranslation, RULE, r, copy);
	  //importer->insertMetadata(RULE, copy, getMetadata(RULE, r));
	}
    }
  //
  //	Now handle our strategy definitions.
  //
  const Vector<StrategyDefinition*>& strategyDefs = getStrategyDefinitions();
  for (int i = 0; i < nrOriginalStrategyDefinitions; i++)
    {
      StrategyDefinition* sdef = strategyDefs[i];
      if (!(sdef->isBad()))
	{
	  int label = importTranslation.translateLabel(sdef->getLabel().id());
	  RewriteStrategy* strategy = importTranslation.translate(sdef->getStrategy());
	  Term* lhs = strategy->copyAuxiliaryTerm(sdef->getLhs(), &importTranslation);
	  StrategyExpression* rhs = deepCopyStrategyExpression(&importTranslation, sdef->getRhs());
	  Vector<ConditionFragment*> condition;
	  deepCopyCondition(&importTranslation, sdef->getCondition(), condition);
	  StrategyDefinition* copy = new StrategyDefinition(label, strategy, lhs, rhs, condition);
	  if (sdef->isNonexec())
	    copy->setNonexec();
	  copy->setLineNumber(sdef->getLineNumber());
	  importer->insertStrategyDefinition(copy);
	  copyMetadata(importer, importTranslation, STRAT_DEF, sdef, copy);
	}
    }
}

void
ImportModule::resetImports()
{
  for (ImportModule* m : parameterTheories)
    m->resetImportPhase();
  for (ImportModule* m : importedModules)
    m->resetImportPhase();
}

void
ImportModule::resetImportPhase()
{
  if (importPhase == UNVISITED)
    return;
  importPhase = UNVISITED;
  //
  //	No need to reset parameter theories because we won't
  //	have visited them. We just reset our importedModules.
  //
  for (ImportModule* m : importedModules)
    m->resetImportPhase();
}

#ifndef NO_ASSERT
void
ImportModule::dumpImports(ostream& s) const
{
  for (ImportModule* m : parameterTheories)
    s << "    " << m << '\n';
  for (ImportModule* m : importedModules)
    s << "    " << m << '\n';
}
#endif
