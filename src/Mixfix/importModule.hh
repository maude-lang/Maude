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
//      Class for modules which can import and be imported.
//
#ifndef _importModule_hh_
#define _importModule_hh_
#include <set>
#include <map>
#include "mixfixModule.hh"
#include "entity.hh"
#include "fileTable.hh"

class ImportModule
  : public MixfixModule,
    public Entity,
    public Entity::User
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
    INSTANTIATION,
    VIEW_LOCAL
  };

  ImportModule(int name, ModuleType moduleType, Origin origin, Entity::User* parent);
  ~ImportModule();

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
  void importStatements();
  void resetImports();

  void localStatementsComplete();
  void protect();
  bool unprotect();

  Origin getOrigin() const;
  int getNrParameters() const;
  bool parametersBound() const;
  ImportModule* getParameterTheory(int index) const;
  int getParameterName(int index) const;
  int getNrImportedSorts() const;
  int getNrUserSorts() const;
  int getNrImportedSubsorts(int sortIndex) const;
  int getNrImportedSymbols() const;
  int getNrImportedPolymorphs() const;
  int getNrUserSymbols() const;
  int getNrImportedDeclarations(int symbolIndex) const;
  int getNrUserDeclarations(int symbolIndex) const;
  int getNrOriginalMembershipAxioms() const;
  int getNrOriginalEquations() const;
  int getNrOriginalRules() const;
  const set<int>& getLabels() const;
  ImportModule* makeRenamedCopy(int name, Renaming* canonical, ModuleCache* moduleCache);
  ImportModule* makeParameterCopy(int moduleName, int parameterName, ModuleCache* moduleCache);
  ImportModule* makeInstantiation(int moduleName,
				  const Vector<View*>& arguments,
				  const Vector<int>& parameterArgs,
				  ModuleCache* moduleCache);
  int findParameterIndex(int name) const;
  bool moduleDeclared(Sort* sort) const;
  bool moduleDeclared(Symbol* symbol) const;
  bool moduleDeclaredPolymorph(int index) const;
  bool parameterDeclared(Sort* sort) const;
  bool parameterDeclared(Symbol* symbol) const;
  bool parameterDeclaredPolymorph(int index) const;

private:
  enum Phase
  {
    UNVISITED,
    SORTS_IMPORTED,
    OPS_IMPORTED,
    OPS_FIXED_UP,
    STATEMENTS_IMPORTED,
    DOOMED
  };

  typedef map<int,int> ParameterMap;
  typedef set<int> ParameterSet;

  static Sort* localSort(ImportModule* copy, Renaming* renaming, const Sort* sort);
  static Sort* localSort2(ImportModule* copy, Renaming* renaming, const Sort* sort);

  static void deepCopyCondition(ImportTranslation* importTranslation,
				const Vector<ConditionFragment*>& original,
				Vector<ConditionFragment*>& copy);

  static int instantiateSortName(int sortId,
				 const ParameterMap& parameterMap,
				 const ParameterSet& extraParameterSet);

  static Renaming* instantiateRenaming(const Renaming* original,
				       const ParameterMap& parameterMap,
				       const ParameterSet& extraParameterSet);
  
  ImportModule* instantiateBoundParameters(const Vector<View*>& arguments,
					   const Vector<int>& parameterArgs,
					   ModuleCache* moduleCache);
 
  void regretToInform(Entity* doomedEntity);
  void donateSorts(ImportModule* importer);
  void donateSorts2(ImportModule* copy, Renaming* renaming = 0);
  void donateOps(ImportModule* importer);
  void donateOps2(ImportModule* copy, Renaming* renaming = 0);
  void fixUpDonatedOps(ImportModule* importer);
  void fixUpDonatedOps2(ImportModule* copy, Renaming* renaming = 0);
  void donateStatements(ImportModule* importer);
  void donateStatements2(ImportModule* importer, ImportTranslation& importTranslation);
  void resetImportPhase();
  void finishCopy(ImportModule* copy, Renaming* canonical);

  void copyMetadata(ImportModule* importer,
		    ImportTranslation& importTranslation,
		    ItemType itemType,
		    PreEquation* original,
		    PreEquation* copy);
  
  ConnectedComponent* translateComponent(const Renaming* renaming, const ConnectedComponent* component) const;

  void addOpMappingsFromView(Renaming* canonical,
			     const ImportModule* parameterCopyOfTheory,
			     const View* view) const;
  //
  //	Because makeInstantiation() is such complex procedure we split out 5 major
  //	subtasks.
  //
  void handleInstantiationByParameter(ImportModule* copy,
				      Renaming* canonical,
				      ParameterMap& parameterMap,
				      const Vector<View*>& arguments,
				      const Vector<int>& parameterArgs,
				      ModuleCache* moduleCache) const;

  void handleInstantiationByTheoryView(ImportModule* copy,
				       Renaming* canonical,
				       ParameterMap& parameterMap,
				       ParameterSet& extraParameterSet,
				       const Vector<View*>& arguments,
				       ModuleCache* moduleCache) const;

  void handleInstantiationByModuleView(ImportModule* copy,
				       Renaming* canonical,
				       ParameterMap& parameterMap,
				       const Vector<View*>& arguments) const;

  void handleParameterizedSorts(Renaming* canonical, 
				const ParameterMap& parameterMap,
				const ParameterSet& extraParameterSet) const;

  void handleRegularImports(ImportModule* copy,
			    const Vector<View*>& arguments,
			    const Vector<int>& parameterArgs,
			    ModuleCache* moduleCache) const;

  const Origin origin;
  Phase importPhase;
  //
  //	These are the theories and modules we directly import.
  //
  //	0,..., parameterNames.size() - 1			parameters
  //	parameterNames.size(),..., importedModules.size() - 1	regular imports
  //
  Vector<int> parameterNames;
  Vector<ImportModule*> importedModules;
  //
  //	Because for sorts, symbols, and polymorphs, stuff from parameter
  //	theories is inserted first we can keep track of what came from
  //	theories in order to stop it from being renamed. For modules
  //	without parameters these will be set to 0.
  //
  int nrSortsFromParameters;
  int nrSymbolsFromParameters;
  int nrPolymorphsFromParameters;
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
  Vector<View*> viewArgs;
  Vector<int> paramArgs;
  //
  //	These data structures are only filled out for theories and record
  //	the indices of any sorts and operators that were declared in an
  //	imported module and are therefore not eligible to be mapped by a
  //	view.
  //
  NatSet sortDeclaredInModule;
  NatSet opDeclaredInModule;
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
  //	we ace actually going to do some work in the module.
  //
  int nrUserSorts;			// total number of user declared sorts
  int nrImportedSorts;			// how many of these were imported
  Vector<int> nrImportedSubsorts;	// for each sort, how many subsorts were imported

  int nrUserSymbols;			// total number of user symbols
  int nrImportedSymbols;		// how many of these were imported
  //
  //	For each symbol:
  //
  Vector<int> nrUserDecls;		// total number of user declarations
  Vector<int> nrImportedDecls;		// total number of imported user declarations

  int nrImportedPolymorphs;		// number of polymorphs that were imported
  int nrOriginalMembershipAxioms;
  int nrOriginalEquations;
  int nrOriginalRules;

  set<int> labels;
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

inline ImportModule*
ImportModule::getParameterTheory(int index) const
{
  Assert(index < getNrParameters(), "bad parameter index " << index << " in module " << (const MixfixModule*) this);
  return importedModules[index]->baseModule;
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
ImportModule::parameterDeclaredPolymorph(int index) const
{
  return index < nrPolymorphsFromParameters;
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

#endif
