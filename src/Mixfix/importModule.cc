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

//	our stuff
#include "renameModule.cc"
#include "parameterization.cc"

ImportModule::ImportModule(int name, ModuleType moduleType, Origin origin, Entity::User* parent)
  : MixfixModule(name, moduleType),
    origin(origin)
{
  if (parent != 0)
    addUser(parent);  // HACK
  importPhase = UNVISITED;
  nrSortsFromParameters = 0;
  nrSymbolsFromParameters = 0;
  nrPolymorphsFromParameters = 0;
  protectCount = 0;
  canonicalRenaming = 0;
  baseModule = 0;
}

ImportModule::~ImportModule()
{
  Assert(canonicalRenaming == 0, "undeleted canonicalRenaming in " << this);
}

bool
ImportModule::parametersBound() const
{
  //
  //	Should only be called on modules that actually have parameters.
  //
  Assert(!parameterNames.empty(), "no parameters!");
  if (origin == TEXT)
    return false;
  if (origin == RENAMING)
    return baseModule->parametersBound();
  Assert(origin == INSTANTIATION, "bad origin " << origin);
  Assert(!viewArgs.empty(), "viewArgs empty");
  return !paramArgs.empty();  // we init this vector iff our parameters are bound
}

void
ImportModule::addImport(ImportModule* importedModule,
			ImportMode mode,
			LineNumber lineNumber)
{
  ModuleType t = importedModule->getModuleType();
  if (isTheory(t) && mode != INCLUDING)
    {
      //
      //	Since we don't use or even store the mode we don't need
      //	to do anything special to recover.
      //
      IssueWarning(lineNumber << ": theories may only be imported using the " <<
		   QUOTE("including") <<
		   " importation mode. Recovering by treating mode as including.");
    }
  if (!canImport(getModuleType(), t))
    {
      //
      //	Allowing modules to import theories would allow inconsistancies
      //	to get into the parameterization mechanism. So we just disallow
      //	illegal imports to avoid having to deal with tricky cases.
      //
      IssueWarning(lineNumber << ": importation of " <<
		   QUOTE(moduleTypeString(t)) << " by " <<
		   QUOTE(moduleTypeString(getModuleType())) <<
		   " not allowed.  Recovering by ignoring import.");
      return;
    }
  importedModules.append(importedModule);
  importedModule->addUser(this);
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
	       QUOTE(moduleTypeString(getModuleType())) << " by " <<
	       QUOTE(moduleTypeString(t)) << " not allowed."); 
	     
  parameterNames.append(parameterName.code());
  importedModules.append(parameterTheory);
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

void
ImportModule::closeSortSet()
{
  nrUserSorts = getSorts().length();  // exclude error sorts
  MixfixModule::closeSortSet();
}

void
ImportModule::closeSignature()
{
  const Vector<Symbol*>& symbols = getSymbols();
  nrUserSymbols = symbols.length();  // exclude polymorph instances
  nrUserDecls.expandTo(nrUserSymbols);
  for (int i = 0; i < nrUserSymbols; i++)
    nrUserDecls[i] = symbols[i]->getOpDeclarations().length();  // exclude comm completions
  MixfixModule::closeSignature();
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
  {
    FOR_EACH_CONST(i, Vector<ImportModule*>, importedModules)
      (*i)->removeUser(this);
  }
  {
    FOR_EACH_CONST(i, Vector<View*>, viewArgs)
      {
	if (*i != 0)
	  (*i)->removeUser(this);
      }
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
  int nrParameters = getNrParameters();
  int nrImportedModules = importedModules.length();
  for (int i = 0; i < nrImportedModules; i++)
    {
      if (i == nrParameters)
	nrSortsFromParameters = getSorts().size();
      importedModules[i]->donateSorts(this);
    }
  const Vector<Sort*> sorts = getSorts();
  nrImportedSorts = sorts.length();
  nrImportedSubsorts.expandTo(nrImportedSorts);
  for (int i = 0; i < nrImportedSorts; i++)
    nrImportedSubsorts[i] = sorts[i]->getSubsorts().length();
}

void
ImportModule::donateSorts(ImportModule* importer)
{
  if (importPhase == SORTS_IMPORTED)
    return;
  importPhase = SORTS_IMPORTED;
  //
  //	First handle our own imports.
  //
  int nrImportedModules = importedModules.length();
  for (int i = 0; i < nrImportedModules; i++)
    importedModules[i]->donateSorts(importer);
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
  int nrParameters = getNrParameters();
  int nrImportedModules = importedModules.length();
  for (int i = 0; i < nrImportedModules; i++)
    {
      if (i == nrParameters)
	{
	  nrSymbolsFromParameters = getSymbols().size();
	  nrPolymorphsFromParameters = getNrPolymorphs();
	}
      importedModules[i]->donateOps(this);
    }
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
  if (importPhase == OPS_IMPORTED)
    return;
  importPhase = OPS_IMPORTED;
  //
  //	First handle our own imports.
  //
  int nrImportedModules = importedModules.length();
  for (int i = 0; i < nrImportedModules; i++)
    importedModules[i]->donateOps(importer);
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
  int nrImportedModules = importedModules.length();
  for (int i = 0; i < nrImportedModules; i++)
    importedModules[i]->fixUpDonatedOps(this);
}

void
ImportModule::fixUpDonatedOps(ImportModule* importer)
{
  if (importPhase == OPS_FIXED_UP)
    return;
  importPhase = OPS_FIXED_UP;
  //
  //	First handle our own imports.
  //
  int nrImportedModules = importedModules.length();
  for (int i = 0; i < nrImportedModules; i++)
    importedModules[i]->fixUpDonatedOps(importer);
  //
  //	Handle our operators.
  //
  fixUpDonatedOps2(importer);
}

//
//	Routines for statement importation.
//

void
ImportModule::importStatements()
{
  int nrImportedModules = importedModules.length();
  for (int i = 0; i < nrImportedModules; i++)
    importedModules[i]->donateStatements(this);
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
      else if(AssignmentConditionFragment* a = dynamic_cast<AssignmentConditionFragment*>(c))
	{
	  n = new AssignmentConditionFragment(a->getLhs()->deepCopy(importTranslation),
					      a->getRhs()->deepCopy(importTranslation));
	}
      else if(RewriteConditionFragment* r = dynamic_cast<RewriteConditionFragment*>(c))
	{
	  n = new RewriteConditionFragment(r->getLhs()->deepCopy(importTranslation),
					   r->getRhs()->deepCopy(importTranslation));
	}
      else
	CantHappen("bad condition fragment");
      copy[i] = n;
    }
}

void
ImportModule::donateStatements(ImportModule* importer)
{
  if (importPhase == STATEMENTS_IMPORTED)
    return;
  importPhase = STATEMENTS_IMPORTED;
  //
  //	First handle our own imports.
  //
  int nrImportedModules = importedModules.length();
  for (int i = 0; i < nrImportedModules; i++)
    importedModules[i]->donateStatements(importer);
  //
  //	The map from imported module's symbols to importing module's symbols
  //	is built dynamically.
  //
  ImportTranslation importTranslation(importer);    
  donateStatements2(importer, importTranslation);
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
	  importer->insertMetadata(MEMB_AX, copy, getMetadata(MEMB_AX, ma));
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
	  copy->setLineNumber(e->getLineNumber());
	  importer->insertEquation(copy);
	  importer->insertMetadata(EQUATION, copy, getMetadata(EQUATION, e));
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
	  copy->setLineNumber(r->getLineNumber());
	  importer->insertRule(copy);
	  importer->insertMetadata(RULE, copy, getMetadata(RULE, r));
	}
    }
}

void
ImportModule::resetImports()
{
  int nrImportedModules = importedModules.length();
  for (int i = 0; i < nrImportedModules; i++)
    importedModules[i]->resetImportPhase();
}

void
ImportModule::resetImportPhase()
{
  if (importPhase == UNVISITED)
    return;
  importPhase = UNVISITED;
  resetImports();
}
