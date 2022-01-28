/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2021 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for modules which can import and be imported.
//
#ifndef _importModule_hh_
#define _importModule_hh_
#include "mixfixModule.hh"
#include "entity.hh"
#include "enclosingObject.hh"
#include "fileTable.hh"
#include "rewriteStrategy.hh"

class ImportModule
  : public MixfixModule,
    public Entity,
    public Entity::User,
    public EnclosingObject
{
  NO_COPYING(ImportModule);

public:
  enum ImportMode
  {
    PROTECTING,
    EXTENDING,
    INCLUDING
  };

  enum Origin
  {
    TEXT,
    SUMMATION,
    RENAMING,
    PARAMETER,
    INSTANTIATION
  };

  ImportModule(int name, ModuleType moduleType);
  ImportModule(int name, ModuleType moduleType, Origin origin, Entity::User* parent);
  ~ImportModule();

  static const char* importModeString(ImportMode mode);

  void addImport(ImportModule* importedModule,
		 ImportMode mode,
		 LineNumber lineNumber);
  void addParameter(const Token parameterName,
		    ImportModule* parameterTheory);
  void closeSortSet();
  void closeSignature();
  void deepSelfDestruct();
  void importSorts();
  void importOps();
  void fixUpImportedOps();
  void importStrategies();
  void importStatements();
  void importRuleLabels();
  void resetImports();

  void localStatementsComplete();
  void protect();
  bool unprotect();

  Origin getOrigin() const;
  int getNrParameters() const;
  bool hasFreeParameters() const;
  int getParameterName(int index) const;
  int getNrImports() const;
  ImportModule* getImportedModule(int index) const;
  ImportMode getImportMode(int index) const;
  int getNrImportedSorts() const;
  int getNrUserSorts() const;
  int getNrImportedSubsorts(int sortIndex) const;
  int getNrUserComponents() const;
  int getNrImportedSymbols() const;
  int getNrImportedPolymorphs() const;
  int getNrUserSymbols() const;
  int getNrImportedDeclarations(int symbolIndex) const;
  int getNrUserDeclarations(int symbolIndex) const;
  int getNrOriginalMembershipAxioms() const;
  int getNrOriginalEquations() const;
  int getNrOriginalRules() const;
  int getNrImportedStrategies() const;
  int getNrOriginalStrategyDefinitions() const;
  const set<int>& getLabels() const;
  ImportModule* makeRenamedCopy(int name, Renaming* canonical, ModuleCache* moduleCache);
  ImportModule* makeParameterCopy(int moduleName, int parameterName, ModuleCache* moduleCache);
  ImportModule* makeInstantiation(int moduleName,
				  const Vector<Argument*>& arguments,
				  ModuleCache* moduleCache);
  static ImportModule* makeSummation(int moduleName, const Vector<ImportModule*>& modules, ModuleCache* moduleCache);
  bool moduleDeclared(Sort* sort) const;
  bool moduleDeclared(Symbol* symbol) const;
  bool moduleDeclared(RewriteStrategy* strat) const;
  bool moduleDeclaredPolymorph(int index) const;
  bool parameterDeclared(Sort* sort) const;
  bool parameterDeclared(Symbol* symbol) const;
  bool parameterDeclared(RewriteStrategy* strat) const;
  bool parameterDeclaredPolymorph(int index) const;
  const Renaming* getCanonicalRenaming() const;  // only supported for parameter copies
  //
  //	Needed for EnclosingObject base.
  //
  int findParameterIndex(int name) const;
  ImportModule* getParameterTheory(int index) const;
  const char* getObjectType() const;
  const NamedEntity* getObjectName() const;

  ImportModule* instantiateBoundParameters(const Vector<Argument*>& arguments, ModuleCache* moduleCache);
  
  void handleParameterizedSorts(Renaming* canonical, 
				const ParameterMap& parameterMap,
				const ParameterSet& extraParameterSet) const;

  void addSortMappingsFromTheoryView(Renaming* underConstruction,
				     int parameterName,
				     const View* view) const;
  void addSortMappingsFromModuleView(Renaming* underConstruction, const View* view) const;
  void addSortRenamingsForParameterChange(Renaming* underConstruction, int newParameterName) const;
  void addOpMappingsFromView(Renaming* underConstruction,
			     const View* view,
			     const ImportModule* parameterCopyUser) const;
  void addStratMappingsFromView(Renaming* underConstruction,
				const View* view,
				const ImportModule* parameterCopyUser) const;

#ifndef NO_ASSERT
  void dumpImports(ostream& s) const;
#endif

  // deepCopyStrategyExpression and instantiateExpression could have been
  // defined as virtual methods of StrategyExpression. However, this would
  // require including Mixfix in the imports of the StrategyLanguage, or
  // define an abstract class like SymbolMap.
  static StrategyExpression* deepCopyStrategyExpression(ImportTranslation* importTranslation,
							StrategyExpression* original);
  static StrategyExpression* instantiateExpression(StrategyExpression* expr,
						   const Vector<int>& varsMap,
						   const Vector<Term*>& values,
						   ImportTranslation* translation);

private:
  enum Phase
  {
    UNVISITED,
    SORTS_IMPORTED,
    OPS_IMPORTED,
    STRATS_IMPORTED,
    OPS_FIXED_UP,
    STATEMENTS_IMPORTED,
    DOOMED
  };

  static Sort* localSort(ImportModule* copy, Renaming* renaming, const Sort* sort);
  static Sort* localSort2(ImportModule* copy, Renaming* renaming, const Sort* sort);

  static void deepCopyCondition(ImportTranslation* importTranslation,
				const Vector<ConditionFragment*>& original,
				Vector<ConditionFragment*>& copy);

  static void instantiateCondition(const Vector<ConditionFragment*>& original,
				   Vector<ConditionFragment*>& copy,
				   const Vector<Term*>& subs,
				   ImportTranslation* translation);

  static Term* instantiateTerm(Term* term,
			       const Vector<Term*>& subs,
			       const VariableInfo& varInfo,
			       ImportTranslation* translation);

  static Term* instantiateCall(Term* term,
			       RewriteStrategy* strat,
			       const Vector<Term*>& subs,
			       ImportTranslation* translation);

  static StrategyExpression* instantiateExpression(StrategyExpression* expr,
						   const Vector<Term*>& subs,
						   ImportTranslation* translation);

  void regretToInform(Entity* doomedEntity);
  void donateSorts(ImportModule* importer);
  void donateSorts2(ImportModule* copy, Renaming* renaming = 0);
  void donateOps(ImportModule* importer);
  void donateOps2(ImportModule* copy, Renaming* renaming = 0);
  void fixUpDonatedOps(ImportModule* importer);
  void fixUpDonatedOps2(ImportModule* copy, Renaming* renaming = 0);
  void donateRuleLabels(ImportModule* copy, Renaming* renaming = 0);
  void donateStrategies(ImportModule* importer);
  void donateStrategies2(ImportModule* importer, Renaming* renaming = 0);
  void donateStatements(ImportModule* importer);
  void donateStatements2(ImportModule* importer, ImportTranslation& importTranslation);
  void resetImportPhase();
  void finishCopy(ImportModule* copy, Renaming* canonical);

  void checkForPolymorphOperatorClash();
  
  void copyMetadata(ImportModule* importer,
		    ImportTranslation& importTranslation,
		    ItemType itemType,
		    PreEquation* original,
		    PreEquation* copy);
  
  ConnectedComponent* translateComponent(const ConnectedComponent* component,
					const ImportModule* parameterCopyUser) const;
  void addFromPartOfRenaming(Renaming* underConstruction,
			     Symbol* s,
			     const ImportModule* parameterCopyUser) const;  
  //
  //	Because makeInstantiation() is such complex procedure we split out 5 blocks of code.
  //
  bool handleInstantiationByTheoryView(ImportModule* copy,
				       Renaming* canonical,
				       ParameterMap& parameterMap,
				       ParameterSet& extraParameterSet,
				       const Vector<Argument*>& arguments,
				       ModuleCache* moduleCache) const;
  
  bool handleInstantiationByParameter(ImportModule* copy,
				      Renaming* canonical,
				      ParameterMap& parameterMap,
				      NatSet& positionsInstantiatedParameter,
				      const Vector<Argument*>& arguments,
				      ModuleCache* moduleCache) const;

  bool handleInstantiationByModuleView(ImportModule* copy,
				       Renaming* canonical,
				       ParameterMap& parameterMap,
				       const NatSet& positionsInstantiatedParameter,
				       const Vector<Argument*>& arguments,
				       ModuleCache* moduleCache) const;

  bool handleBoundParameters(ImportModule* copy, View* argumentView, ModuleCache* moduleCache) const;
  
  void handleRegularImports(ImportModule* copy,
			    const Vector<Argument*>& arguments,
			    ModuleCache* moduleCache) const;
  //
  //	Because instantiateBoundParameters() is such complex procedure we split out 4 blocks of code.
  //
  ImportModule* handleSummation(const Vector<Argument*>& arguments, ModuleCache* moduleCache);

  ImportModule* handleRenaming(const Vector<Argument*>& arguments, ModuleCache* moduleCache);
  static Renaming* instantiateRenaming(const Renaming* original,
				       const ParameterMap& parameterMap,
				       const ParameterSet& extraParameterSet);

  ImportModule* handleFinalInstantiation(const Vector<Argument*>& arguments, ModuleCache* moduleCache);
  
  const Origin origin;
  Phase importPhase;
  //
  //	These are the names of our parameters and the associated parameter
  //	theory copies. If we have a parameter X of theory TRIV then
  //	we will have a parameter theory copy X :: TRIV with sort X$Elt.
  //	We need to be careful to get back to TRIV (which will be stored as the
  //	copies baseModule) if we are asked for the actual theory.
  //
  Vector<int> parameterNames;
  Vector<ImportModule*> parameterTheories;
  //
  //	These are the modules we directly (as opposed to transitively) import.
  //
  Vector<ImportModule*> importedModules;
  Vector<ImportMode> importModes;
  //
  //	Because for sorts, symbols, and polymorphs, stuff from parameter
  //	theories is inserted first we can keep track of what came from
  //	theories in order to stop it from being renamed. For modules
  //	without parameters these will be set to 0.
  //
  int nrSortsFromParameters;
  int nrSymbolsFromParameters;
  int nrPolymorphsFromParameters;
  int nrStrategiesFromParameters;
  //
  //	If we are a renaming, parameter copy or instantiation of another
  //	module we need to store this info.
  //
  Renaming* canonicalRenaming;
  ImportModule* baseModule;
  //
  //	If we are are a instantiation, we keep track of any views that we
  //	a user of and if we have bound parameters, we also keep track
  //	track of the parameter arguments our base module was instantiated
  //	with so we can build a new instantiation when our bound 
  //	parameters are instantiated.
  //
  Vector<Argument*> savedArguments;
  //
  //	These data structures are only filled out for theories and record
  //	the indices of any sorts and operators that were declared in an
  //	imported module and are therefore not eligible to be mapped by a
  //	view.
  //
  //	As of 5/2/19 we keep track of sortDeclaredInModule for all imported
  //	sorts, even for modules.
  //
  NatSet sortDeclaredInModule;
  NatSet opDeclaredInModule;
  NatSet stratDeclaredInModule;
  NatSet polymorphDeclaredInModule;
  //
  //	Need to keep track of what parts of MixfixModule actually belong
  //	to us (and need to be donated to our importers) and what parts
  //	we imported or otherwise generated.
  //
  //	For sorts, symbols and op declarations, imports come first,
  //	then locals, then generated.
  //
  //	For subsorts, imported subsort (relations) come first, then locals.
  //	The are no generated subsort relations.
  //
  //	For mbs, eqs, and rls, locals come first, then imports.
  //	The reason for this different order is to avoid imports unless
  //	we are actually going to do some work in the module.
  //
  int nrUserSorts;			// total number of user declared sorts
  int nrImportedSorts;			// how many of these were imported
  Vector<int> nrImportedSubsorts;	// for each sort, how many subsorts were imported
  int nrUserComponents;			// total number of kinds from user declared sorts

  int nrUserSymbols;			// total number of user symbols
  int nrImportedSymbols;		// how many of these were imported
  //
  //	For each symbol:
  //
  Vector<int> nrUserDecls;		// total number of user declarations
  Vector<int> nrImportedDecls;		// total number of imported user declarations

  int nrImportedPolymorphs;		// number of polymorphs that were imported
  int nrImportedStrategies;
  int nrOriginalMembershipAxioms;
  int nrOriginalEquations;
  int nrOriginalRules;
  int nrOriginalStrategyDefinitions;

  set<int> labels;
  //
  //	Because the module may still be in use (by some interrupted computation)
  //	after it has been overwritten by a new module with the same name entered
  //	from the debugger, we keep a count of active users and postpone deletion
  //	of an overwritten module until it has no more users.
  //
  int protectCount;
};

inline void
ImportModule::protect()
{
  ++protectCount;
}

inline ImportModule::Origin
ImportModule::getOrigin() const
{
  return origin;
}

inline int
ImportModule::getNrImports() const
{
  return importedModules.size();
}

inline ImportModule*
ImportModule::getImportedModule(int index) const
{
  return importedModules[index];
}

inline ImportModule::ImportMode
ImportModule::getImportMode(int index) const
{
  return importModes[index];
}

inline int
ImportModule::getNrImportedSorts() const
{
  //
  //	Sorts with index < this value were imported.
  //
  return nrImportedSorts;
}

inline int
ImportModule::getNrParameters() const
{
  return parameterNames.size();
}

inline int
ImportModule::getParameterName(int index) const
{
  return parameterNames[index];
}

inline int
ImportModule::getNrUserSorts() const
{
  //
  //	Sorts with index >= this value were generated (e.g. for kinds).
  //
  return nrUserSorts;
}

inline int
ImportModule::getNrImportedSubsorts(int sortIndex) const
{
  //
  //	Subsorts with index < this value were imported.
  //
  return (sortIndex < nrImportedSorts) ? nrImportedSubsorts[sortIndex] : 0;
}

inline int
ImportModule::getNrUserComponents() const
{
  //
  //	Components with index >= this value were generated (for non-user sorts).
  //
  return nrUserComponents;
}

inline int
ImportModule::getNrImportedSymbols() const
{
  //
  //	Symbols with index < this value were imported.
  //
  return nrImportedSymbols;
}

inline int
ImportModule::getNrImportedPolymorphs() const
{
  //
  //	Symbols with index < this value were imported.
  //
  return nrImportedPolymorphs;
}

inline int
ImportModule::getNrUserSymbols() const
{
  //
  //	Symbols with index >= this value were generated (e.g. for polymorph instantiation).
  //
  return nrUserSymbols;
}

inline int
ImportModule::getNrImportedDeclarations(int symbolIndex) const
{
  //
  //	Declarations with index < this value were imported.
  //
  return (symbolIndex >= nrImportedSymbols) ? 0 : nrImportedDecls[symbolIndex];
}

inline int
ImportModule::getNrUserDeclarations(int symbolIndex) const
{
  //
  //	Declarations with index >= this value were generated (e.g. for comm completion).
  //
  return nrUserDecls[symbolIndex];
}

inline int
ImportModule::getNrOriginalMembershipAxioms() const
{
  //
  //	Membership axioms with index >= this value were imported.
  //
  return nrOriginalMembershipAxioms;
}

inline int
ImportModule::getNrOriginalEquations() const
{
  //
  //	 Equations with index >= this value were imported.
  //
  return nrOriginalEquations;
}
 
inline int
ImportModule::getNrOriginalRules() const
{
  //
  //	 Rules with index >= this value were imported.
  //
  return nrOriginalRules;
}

inline int
ImportModule::getNrImportedStrategies() const
{
  //
  //	Strategies with index < this value were imported.
  //
  return nrImportedStrategies;
}

inline int
ImportModule::getNrOriginalStrategyDefinitions() const
{
  //
  //	 Strategy definitions with index >= this value were imported.
  //
  return nrOriginalStrategyDefinitions;
}

inline const set<int>&
ImportModule::getLabels() const
{
  return labels;
}

inline bool
ImportModule::moduleDeclared(Sort* sort) const
{
  Assert(sort->getModule() == this, "wrong module");
  return sortDeclaredInModule.contains(sort->getIndexWithinModule());
}

inline bool
ImportModule::moduleDeclared(Symbol* symbol) const
{
  Assert(symbol->getModule() == this, "wrong module");
  return opDeclaredInModule.contains(symbol->getIndexWithinModule());
}

inline bool
ImportModule::moduleDeclared(RewriteStrategy* strat) const
{
  Assert(strat->getModule() == this, "wrong module");
  return stratDeclaredInModule.contains(strat->getIndexWithinModule());
}

inline bool
ImportModule::moduleDeclaredPolymorph(int index) const
{
  return polymorphDeclaredInModule.contains(index);
}

inline bool
ImportModule::parameterDeclared(Sort* sort) const
{
  Assert(sort->getModule() == this, "wrong module");
  return sort->getIndexWithinModule() < nrSortsFromParameters;
}

inline bool
ImportModule::parameterDeclared(Symbol* symbol) const
{
  Assert(symbol->getModule() == this, "wrong module");
  return symbol->getIndexWithinModule() < nrSymbolsFromParameters;
}

inline bool
ImportModule::parameterDeclared(RewriteStrategy* strat) const
{
  Assert(strat->getModule() == this, "wrong module");
  return strat->getIndexWithinModule() < nrStrategiesFromParameters;
}

inline bool
ImportModule::parameterDeclaredPolymorph(int index) const
{
  return index < nrPolymorphsFromParameters;
}

inline const Renaming*
ImportModule::getCanonicalRenaming() const
{
  Assert(origin == PARAMETER, "called on origin = " << origin);
  return canonicalRenaming;
}

#ifndef NO_ASSERT
inline ostream&
operator<<(ostream& s, const ImportModule* m)
{
  //
  //	Needed to avoid ambiguity.
  //
  s << static_cast<const MixfixModule*>(m);
  return s;
}
#endif

inline bool
ImportModule::hasFreeParameters() const
{
  //
  //	If we have parameters and they are not bound they must be free.
  //
  return getNrParameters() > 0 && !(hasBoundParameters());
}

#endif
