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
// 	Class for symbols from permutative theories
//
#ifndef _associativeSymbol_hh_
#define _associativeSymbol_hh_
#include "binarySymbol.hh"

class AssociativeSymbol : public BinarySymbol
{
public:
  enum Structure
  {
    UNSTRUCTURED,	// no guarantees
    LIMIT_SORT,		// s_1 <= s & s_2 <= s ===> s_f(s_1, s_2) <= s
    PURE_SORT		// replaces ===> with <===>,
    			// taking sort constraints in to account
  };

  AssociativeSymbol(int id,
		    const Vector<int>& strategy,
		    bool memoFlag,
		    Term* identity = 0);

  const Sort* uniformSort() const;
  int sortBound(const Sort* sort) const;
  Structure sortStructure(const Sort* sort) const;
  bool mightCollapseToOurSymbol(const Term* subterm) const;
  void finalizeSortInfo();  // virtual in base class SortTable
  void fillInSortInfo(Term* subject);  // virtual in base class SortTable
  bool isConstructor(DagNode* subject);  // virtual in base class Symbol
  void setFrozen(const NatSet& frozen);  // virtual in base class Strategy

protected:
  void processIdentity();

private:
  struct Inv;

  void associativeSortCheck();
  void associativeSortBoundsAnalysis();
  void associativeSortStructureAnalysis();
  bool checkUniformity(const Sort* uniformSort, int nrSorts);
  void insertGreaterOrEqualSorts(const Sort* sort, NatSet& set);

  const Sort* uniSort;
  Vector<int> sortBounds;
  Vector<Structure> sortStructures;
};

inline const Sort*
AssociativeSymbol::uniformSort() const
{
  return uniSort;
}

inline int
AssociativeSymbol::sortBound(const Sort* sort) const
{
  return sortBounds[sort->index()];
}

inline AssociativeSymbol::Structure
AssociativeSymbol::sortStructure(const Sort* sort) const
{
  return sortStructures[sort->index()];
}

ostream& operator<<(ostream& s, AssociativeSymbol::Structure structure);

#endif
