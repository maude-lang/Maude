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
  void buildSortDiagram();
  void buildCtorDiagram();
  int findStateNumber(Vector<NatSet>& stateSet, const NatSet& state);
  int findMinSortIndex(const NatSet& state);
  bool partiallySubsumes(int subsumer, int victim, int argNr);
  void minimize(NatSet& alive, int argNr);

  // void panic() const;  // HACK

  int containsConstructor(const NatSet& state);
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
};

inline int
SortTable::arity() const
{
  return nrArgs;
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
