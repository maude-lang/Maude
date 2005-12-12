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
//	Class for terms in the sucessor theory.
//
#ifndef _S_Term_hh_
#define _S_Term_hh_
#include <gmpxx.h>
#include "term.hh"

class S_Term : public Term
{
  NO_COPYING(S_Term);

public:
  S_Term(S_Symbol* symbol, const mpz_class& number, Term* arg);
  //
  //    Functions required by theory interface.
  //
  RawArgumentIterator* arguments();
  void deepSelfDestruct();
  Term* deepCopy2(SymbolMap* map) const;
  Term* normalize(bool full, bool& changed);
  int compareArguments(const Term* other) const;
  int compareArguments(const DagNode* other) const;
  void findEagerVariables(bool atTop, NatSet& eagerVariables) const;
  void analyseConstraintPropagation(NatSet& boundUniquely) const;
  void insertAbstractionVariables(VariableInfo& variableInfo);
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
  //	Needed because we have hidden data.
  //
  Term* instantiate2(const Vector<Term*>& varBindings, SymbolMap* translator);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif
  //
  //    Functions specific to S_Term.
  //
  S_Symbol* symbol() const;
  const mpz_class& getNumber() const;
  Term* getArgument() const;

private:
  S_Term(const S_Term& original, S_Symbol* symbol, SymbolMap* translator);

  mpz_class number;
  Term* arg;
  int abstractionVariableIndex;
};

inline S_Symbol*
S_Term::symbol() const
{
  return safeCast(S_Symbol*, Term::symbol());
}

inline const mpz_class&
S_Term::getNumber() const
{
  return number;
}

inline Term*
S_Term::getArgument() const
{
  return arg;
}

#endif
