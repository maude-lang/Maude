/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2006 SRI International, Menlo Park, CA 94025, USA.

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
//	Class to storage bdds associate with each sort index and each component.
//	These are currently only used for order-sorted unification and can take up
//	a fair amount of computation and storage. Thus they are computed on demand
//	and cached in this structure.
//
#ifndef _sortBdds_hh_
#define _sortBdds_hh_
#include "bdd.hh"
#include "bddUser.hh"

class SortBdds : private BddUser
{
public:
  SortBdds(Module* module);

  int getFirstAvailableVariable() const;
  int getNrVariables(int componentIndex) const;
  Bdd getGtRelation(int componentIndex) const;
  Bdd getLeqRelation(int sortIndex) const;
  const Vector<Bdd>& getSortFunction(Symbol* symbol) const;

  void makeIndexVector(int nrBdds, int index, Vector<Bdd>& vec) const;
  Bdd makeIndexBdd(int firstVariable, int nrVariables, int index) const;
  void makeVariableVector(int firstVariable, int nrVariables,  Vector<Bdd>& vec) const;
  Bdd makeVariableBdd(int firstVariable, int nrVariables) const;

private:
  struct ComponentInfo
  {
    int nrVariables;	// number of bits needed to represent a sort in this component
    Bdd gtRelation;	// bdd for relation s1 valid /\ s2 valid /\ s1 > s2
  };

  int calculateNrBits(int nrIndicies);

  int maxNrVariables;	// the max number of variables needed for any component
  //
  //	For each component we keep the number of variables needed to encode a sort
  //	in this component and the bdd for relation s1 valid /\ s2 valid /\ s1 > s2.
  //
  Vector<ComponentInfo> componentInfo;
  //
  //	For each sort s we keep the bdd for the relation x <= s.
  //
  Vector<Bdd> leqRelations;
  //
  //	For each symbol, we keep a vector of BDDs to hold its sort function - which
  //	may or may not be filled out.
  //
  mutable Vector<Vector<Bdd> > sortFunctions;
};

inline int
SortBdds::getFirstAvailableVariable() const
{
  return maxNrVariables;
}

inline int
SortBdds::getNrVariables(int componentIndex) const
{
  return componentInfo[componentIndex].nrVariables;
}

inline Bdd
SortBdds::getGtRelation(int componentIndex) const
{
  return componentInfo[componentIndex].gtRelation;
}

inline Bdd
SortBdds::getLeqRelation(int sortIndex) const
{
  return leqRelations[sortIndex];
}

#endif
