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
//	Abstract class for table of operation sort declarations for a symbol.
//
#ifndef _sortTable_hh_
#define _sortTable_hh_
#include <set>
#include "bdd.hh"
#include "opDeclaration.hh"
#include "fullCompiler.hh"
#include "connectedComponent.hh"

class SortTable
{
  NO_COPYING(SortTable);

public:
  enum CtorStatus
  {
    IS_CTOR = 1,
    IS_NON_CTOR = 2,
    IS_COMPLEX = IS_CTOR | IS_NON_CTOR
  };
  
  SortTable(int arity);
  int arity() const;
  void addOpDeclaration(const Vector<Sort*>& domainAndRange,
			bool constructorFlag);
  const Vector<OpDeclaration>& getOpDeclarations() const;
  /* const */ ConnectedComponent* rangeComponent() const;
  const ConnectedComponent* domainComponent(int argNr) const;
  Sort* getSingleNonErrorSort() const;
  int getCtorStatus() const;
  virtual void compileOpDeclarations();
  virtual void fillInSortInfo(Term* subject) = 0;
  virtual void finalizeSortInfo();
  // virtual void computeBaseSort(DagNode* subject) = 0;
  // virtual void computeTrueSort(DagNode* subject, RewritingContext& context) = 0;
  virtual bool canProduceErrorSort() const;
  int traverse(int position, int sortIndex) const;
  bool kindLevelDeclarationsOnly() const;
  const NatSet& getMaximalOpDeclSet(Sort* target);
  //
  //	Unification stuff.
  //
  virtual void computeSortFunctionBdds(const SortBdds& sortBdds, Vector<Bdd>& sortFunctionBdds) const;
  virtual void computeGeneralizedSort(const SortBdds& sortBdds,
				      const Vector<int>& realToBdd,  // first BDD variable for each free real variable
				      DagNode* subject,
				      Vector<Bdd>& generalizedSort) { CantHappen("not implemented"); }

#ifdef COMPILER
  void generateSortDiagram(CompilationContext& context) const;
#endif

protected:
  bool specialSortHandling() const;
  int ctorTraverse(int position, int sortIndex) const;

#ifdef DUMP
  void dumpSortDiagram(ostream& s, int indentLevel);
#endif

private:
  typedef Vector<Bdd> BddVector;
  typedef Vector<BddVector> BddTable;

  struct Node;
  struct SpanningTree;
  
  void buildSortDiagram();
  void buildCtorDiagram();
  void sortErrorAnalysis(bool preregProblem,
			 const set<int>& badTerminals);
  void computeMaximalOpDeclSetTable();
  bool domainSubsumes(int subsumer, int victim) const;
  int findStateNumber(Vector<NatSet>& stateSet, const NatSet& state);
  int findMinSortIndex(const NatSet& state, bool& unique);
  bool partiallySubsumes(int subsumer, int victim, int argNr);
  void minimize(NatSet& alive, int argNr);

  void computeBddVector(const SortBdds& sortBdds,
			int bddVarNr,
			int argNr,
			BddTable& table,
			int nodeNr) const;
  // void panic() const;  // HACK

  bool containsConstructor(const NatSet& state, bool& unique);
  static bool partlyMoreGeneral(const OpDeclaration& subsumer,
				const OpDeclaration& victim,
				int argNr);
  static bool ctorSubsumes(const OpDeclaration& subsumer,
			   const OpDeclaration& victim,
			   int argNr);
  void minimizeWrtCtor(NatSet& alive, int argNr) const;

  const int nrArgs;
  Vector<OpDeclaration> opDeclarations;
  Vector<ConnectedComponent*> componentVector;
  Vector<int> sortDiagram;
  Sort* singleNonErrorSort;  // if we can only generate one non error sort
  Vector<int> ctorDiagram;
  int ctorStatus;  // place holder
  Vector<NatSet> maximalOpDeclSetTable;  // indices of maximal op decls with range <= each sort
};

inline int
SortTable::arity() const
{
  return nrArgs;
}

inline const NatSet&
SortTable::getMaximalOpDeclSet(Sort* target)
{
  if (maximalOpDeclSetTable.isNull())
    computeMaximalOpDeclSetTable();
  return maximalOpDeclSetTable[target->index()];
}

inline bool
SortTable::specialSortHandling() const
{
  return sortDiagram.isNull();
}

inline void 
SortTable::addOpDeclaration(const Vector<Sort*>& domainAndRange, bool constructorFlag)
{
  Assert(domainAndRange.length() - 1 == nrArgs,
	 "bad domain length of " <<
	 domainAndRange.length() - 1 << " instead of " << nrArgs);
  int nrOpDeclarations = opDeclarations.length();
  opDeclarations.resize(nrOpDeclarations + 1);
  opDeclarations[nrOpDeclarations].setInfo(domainAndRange, constructorFlag);
  ctorStatus |= constructorFlag ? IS_CTOR : IS_NON_CTOR;
}

inline const Vector<OpDeclaration>&
SortTable::getOpDeclarations() const
{
  return opDeclarations;
}

inline /* const */ ConnectedComponent*
SortTable::rangeComponent() const
{
  return opDeclarations[0].getDomainAndRange()[nrArgs]->component();
}

inline const ConnectedComponent*
SortTable::domainComponent(int argNr) const
{
  return opDeclarations[0].getDomainAndRange()[argNr]->component();
}

inline Sort*
SortTable::getSingleNonErrorSort() const
{
  return singleNonErrorSort;
}

inline int
SortTable::getCtorStatus() const
{
  return ctorStatus;
}

inline int
SortTable::traverse(int position, int sortIndex) const
{
  /*
  if (position + sortIndex >= sortDiagram.length())  // HACK
    panic();
  */
  return sortDiagram[position + sortIndex];
}

inline int
SortTable::ctorTraverse(int position, int sortIndex) const
{
  return ctorDiagram[position + sortIndex];
}

#endif
