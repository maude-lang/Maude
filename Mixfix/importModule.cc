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
#include "importModule.hh"
#include "importTranslation.hh"
#include "quotedIdentifierSymbol.hh"

ImportModule::ImportModule(int name, ModuleType moduleType, Parent* parent)
  : MixfixModule(name, moduleType),
    parent(parent)
{
  importPhase = UNVISITED;
  protectCount = 0;
}

void
ImportModule::addImport(ImportModule* importedModule)
{
  importedModules.append(importedModule);
  importedModule->dependentModules.append(this);
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
ImportModule::deepSelfDestruct()
{
  int nrImportedModules = importedModules.length();
  for (int i = 0; i < nrImportedModules; i++)
    {
      ImportModule* import = importedModules[i];
      import->removeDependent(this);
    }
  while (dependentModules.length() > 0)
    dependentModules[0]->deepSelfDestruct();

  if(parent != 0)
    parent->regretToInform(this);
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

void
ImportModule::removeDependent(ImportModule* dependent)
{
  DebugAdvisory("removed " << dependent << " from modules depending on " << this);
  int nrDependentModules = dependentModules.length();
  for (int i = 0; i < nrDependentModules; i++)
    {
      if (dependentModules[i] == dependent)
	{
	  dependentModules[i] = dependentModules[nrDependentModules - 1];
	  dependentModules.contractTo(nrDependentModules - 1);
	  return;
	}
    }
  CantHappen("non-existent dependent" << this);
}

//
//	Routines for sort and subsort importation.
//
void
ImportModule::importSorts()
{
  int nrImportedModules = importedModules.length();
  for (int i = 0; i < nrImportedModules; i++)
    importedModules[i]->donateSorts(this);

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
  const Vector<Sort*>& sorts = getSorts();
  for (int i = nrImportedSorts; i < nrUserSorts; i++)
    {
      Sort* original = sorts[i];
      int id = original->id();
      Sort* sort = importer->findSort(id);
      if (sort == 0)
	{
	  sort = importer->addSort(id);
	  sort->setLineNumber(original->getLineNumber());
	}
      else
	{
	  IssueWarning(*importer << ": sort " << QUOTE(original) <<
		       " has been imported from both " << *original <<
		       " and " << *sort << '.');
	}
    }
  //
  //	Now handle our subsort relations.
  //
  for (int i = 0; i < nrUserSorts; i++)
    {
      int nrImports = getNrImportedSubsorts(i);
      const Sort* s = sorts[i];
      const Vector<Sort*>& subsorts = s->getSubsorts();
      int nrSubsorts = subsorts.length();
      if (nrSubsorts > nrImports)
	{
	  Sort* ts = importer->findSort(s->id());
	  for (int j = nrImports; j < nrSubsorts; j++)
	    ts->insertSubsort(importer->findSort(subsorts[j]->id()));
	}
    }
}

//
//	Routines for operator importation.
//
void
ImportModule::importOps()
{
  int nrImportedModules = importedModules.length();
  for (int i = 0; i < nrImportedModules; i++)
    importedModules[i]->donateOps(this);

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
  responsibleForFixUp.makeEmpty();
  const Vector<Symbol*> symbols = getSymbols();
  ImportTranslation importTranslation(importer);
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
	  name.tokenize(symbol->id(), symbol->getLineNumber());
	  const Vector<OpDeclaration>& opDecls = symbol->getOpDeclarations();
	  int domainAndRangeLength = opDecls[0].getDomainAndRange().length();
	  static Vector<Sort*> domainAndRange;
	  domainAndRange.resize(domainAndRangeLength);
	  static Vector<int> dummy;
	  const Vector<int>& strategy = symbolType.hasFlag(SymbolType::STRAT) ?
	    symbol->getStrategy() : dummy;
	  const NatSet& frozen = symbol->getFrozen();
	  int prec = symbolType.hasFlag(SymbolType::PREC) ? getPrec(symbol) : DEFAULT;
	  static Vector<int> gather;
	  if (symbolType.hasFlag(SymbolType::GATHER))
	    getGather(symbol, gather);
	  else
	    gather.contractTo(0);
	  const Vector<int>& format = getFormat(symbol);
	  bool originator;
	  for (int j = nrImportedDeclarations; j < nrUserDeclarations; j++)
	    {
	      const Vector<Sort*>& oldDomainAndRange = opDecls[j].getDomainAndRange();
	      for (int k = 0; k < domainAndRangeLength; k++)
		domainAndRange[k] = importTranslation.translate(oldDomainAndRange[k]);
	      if (opDecls[j].isConstructor())
		symbolType.setFlags(SymbolType::CTOR);
	      else
		symbolType.clearFlags(SymbolType::CTOR);
	      Symbol* newSymbol = importer->addOpDeclaration(name,
							     domainAndRange,
							     symbolType,
							     strategy,
							     frozen,
							     prec,
							     gather,
							     format,
							     originator);
	      if (j == 0)
		{
		  if (originator)
		    {
		      responsibleForFixUp.insert(i);
		      if (symbolType.getBasicType() == SymbolType::BUBBLE)
			importer->copyBubbleSpec(this, symbol, newSymbol);
		    }
		  else
		    {
		      IssueWarning(*importer << ": operator " << QUOTE(symbol) <<
				   " has been imported from both " << *newSymbol <<
				   " and " << *symbol << " with no common ancestor.");
		    }
		}
	      else
		Assert(!originator, "bad origination of " << symbol);
	    }
	}
    }
  //
  //	Now handle our polymorphic operators.
  //
  firstPolymorphCopy = importer->getNrPolymorphs();
  int nrPolymorphs = getNrPolymorphs();
  for (int i = nrImportedPolymorphs; i < nrPolymorphs; i++)
    importer->copyPolymorph(this, i);
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
  //	The map from imported module's symbols to importing module's symbols
  //	is built dynamically.
  //
  ImportTranslation importTranslation(importer);
  //
  //	Now handle our operators.
  //
  const Vector<Symbol*> symbols = getSymbols();
  for (int i = nrImportedSymbols; i < nrUserSymbols; i++)
    {
      if (responsibleForFixUp.contains(i))
	{
	  Symbol* symbol = symbols[i];
	  SymbolType symbolType = getSymbolType(symbol);
	  if (symbolType.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
	    {
	      //
	      //	Handle identity.
	      //
	      BinarySymbol* s = static_cast<BinarySymbol*>(symbol);
	      Term* id = s->getIdentity();
	      Assert(id != 0, "missing identity");
	      BinarySymbol* s2 = static_cast<BinarySymbol*>(importTranslation.translate(symbol));
	      Assert(s2->getIdentity() == 0, "identity already exists");
	      s2->setIdentity(id->deepCopy(&importTranslation));
	    }
	  if (symbolType.getBasicType() == SymbolType::BUBBLE)
	    {
	      importer->copyFixUpBubbleSpec(importTranslation.translate(symbol),
					    &importTranslation);
	    }
	}
    }
  //
  //	Now handle our polymorphic operators.
  //
  int nrPolymorphs = getNrPolymorphs();
  for (int i = nrImportedPolymorphs; i < nrPolymorphs; i++)
    {
      importer->copyFixUpPolymorph(firstPolymorphCopy + i - nrImportedPolymorphs,
				   this, i, &importTranslation);
    }
  //
  //	Now handle incremental fixups.
  //
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
	  if (getSymbolType(symbol).hasAttachments())
	    importTranslation.translate(symbol)->copyAttachments(symbol, &importTranslation);
	}
    }
}

void
ImportModule::localStatementsComplete()  // NASTY HACK
{
  nrOriginalMembershipAxioms = getSortConstraints().length();
  nrOriginalEquations = getEquations().length();
  nrOriginalRules = getRules().length();
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
  //
  //	Handle our membership axioms.
  //
  const Vector<SortConstraint*>& membershipAxioms = getSortConstraints();
  for (int i = 0; i < nrOriginalMembershipAxioms; i++)
    {
      SortConstraint* ma = membershipAxioms[i];
      if (!(ma->isBad()))
	{
	  Term* lhs = ma->getLhs()->deepCopy(&importTranslation);
	  Sort* sort = importTranslation.translate(ma->getSort());
	  Vector<ConditionFragment*> condition;
	  deepCopyCondition(&importTranslation, ma->getCondition(), condition);
	  SortConstraint* copy = new SortConstraint(ma->getLabel().id(), lhs, sort, condition);
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
	  Term* lhs = e->getLhs()->deepCopy(&importTranslation);
	  Term* rhs = e->getRhs()->deepCopy(&importTranslation);
	  Vector<ConditionFragment*> condition;
	  deepCopyCondition(&importTranslation, e->getCondition(), condition);
	  Equation* copy = new Equation(e->getLabel().id(), lhs, rhs, e->isOwise(), condition);
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
	  Term* lhs = r->getLhs()->deepCopy(&importTranslation);
	  Term* rhs = r->getRhs()->deepCopy(&importTranslation);
	  Vector<ConditionFragment*> condition;
	  deepCopyCondition(&importTranslation, r->getCondition(), condition);
	  Rule* copy = new Rule(r->getLabel().id(), lhs, rhs, condition);
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
