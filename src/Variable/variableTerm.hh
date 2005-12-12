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
//	Class for variable terms.
//
#ifndef _variableTerm_hh_
#define _variableTerm_hh_
#include "term.hh"
#include "namedEntity.hh"
#include "variableSymbol.hh"

class VariableTerm : public Term, public NamedEntity
{
  NO_COPYING(VariableTerm);

public:
  VariableTerm(VariableSymbol* symbol, int name);

  RawArgumentIterator* arguments();
  void deepSelfDestruct();
  Term* deepCopy2(SymbolMap* translator) const;
  Term* normalize(bool full, bool& changed);
  int compareArguments(const Term* other) const;
  int compareArguments(const DagNode* other) const;

  void findEagerVariables(bool atTop, NatSet& eagerVariables) const;
  void analyseConstraintPropagation(NatSet& boundUniquely) const;
  LhsAutomaton* compileLhs2(bool matchAtTop,
			    const VariableInfo& variableInfo,
			    NatSet& boundUniquely,
			    bool& subproblemLikely);
  void markEagerArguments(int nrVariables,
			  const NatSet& eagerVariables,
			  Vector<int>& problemVariables);
  DagNode* dagify2();

  void findAvailableTerms(TermBag& availableTerms, bool eagerContext, bool atTop);
  int compileRhs2(RhsBuilder& rhsBuilder,
		  VariableInfo& variableInfo,
		  TermBag& availableTerms,
		  bool eagerContext);
  //
  //	Optional stuff that is easy to define for variable terms.
  //
  bool subsumes(const Term* other, bool sameVariableSet) const;
  int partialCompareUnstable(const Substitution& partialSubstitution,
			     DagNode* other) const;
  //
  //	Needed because we actually do the instantiation of variables.
  //
  Term* instantiate2(const Vector<Term*>& varBindings, SymbolMap* translator);
  //
  //	Functions particular to variable terms.
  //
  Sort* getSort() const;
  int getIndex() const;
  void setIndex(int indx);

private:
  int index;
};

inline Sort*
VariableTerm::getSort() const
{
  return safeCast(VariableSymbol*, symbol())->getSort();
}

inline int
VariableTerm::getIndex() const
{
  return index;
}

inline void
VariableTerm::setIndex(int indx)
{
  index = indx;
}

#endif
