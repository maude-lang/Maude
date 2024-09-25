/*

    This file is part of the Maude 3 interpreter.

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
//      Class for condition fragment supporting equality test.
//
#ifndef _equalityConditionFragment_hh_
#define _equalityConditionFragment_hh_
#include "conditionFragment.hh"
#include "rhsBuilder.hh"

class EqualityConditionFragment : public ConditionFragment
{
  NO_COPYING(EqualityConditionFragment);

public:
  EqualityConditionFragment(Term* lhs, Term* rhs);
  ~EqualityConditionFragment();

  void check(VariableInfo& varInfo, NatSet& boundVariables);
  void preprocess();
  void compileBuild(VariableInfo& variableInfo, TermBag& availableTerms);
  void compileMatch(VariableInfo& variableInfo, NatSet& boundUniquely);
  bool solve(bool findFirst,
	     RewritingContext& solution,
	     Stack<ConditionState*>& state);

  Term* getLhs() const;
  Term* getRhs() const;

  void normalize(bool full);
  //
  //	This function exists to enable equality condition fragments to have
  //	a special operational semantics under rewriting modulo SMT.
  //
  void buildInstances(Substitution& substitution, DagNode*& lhs, DagNode*& rhs);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  Term* lhs;
  Term* rhs;
  RhsBuilder builder;
  int lhsIndex;
  int rhsIndex;
};

inline Term*
EqualityConditionFragment::getLhs() const
{
  return lhs;
}

inline Term*
EqualityConditionFragment::getRhs() const
{
  return rhs;
}

inline void
EqualityConditionFragment::normalize(bool full)
{
  lhs = lhs->normalize(full);
  rhs = rhs->normalize(full);
}

#endif
