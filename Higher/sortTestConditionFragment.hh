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
//      Class for condition fragment supporting sort test.
//
#ifndef _sortTestConditionFragment_hh_
#define _sortTestConditionFragment_hh_
#include "conditionFragment.hh"
#include "rhsBuilder.hh"

class SortTestConditionFragment : public ConditionFragment
{
  NO_COPYING(SortTestConditionFragment);

public:
  SortTestConditionFragment(Term* lhs, Sort* sort);
  ~SortTestConditionFragment();

  void check(VariableInfo& varInfo, NatSet& boundVariables);
  void preprocess();
  void compileBuild(VariableInfo& variableInfo, TermBag& availableTerms);
  void compileMatch(VariableInfo& variableInfo, NatSet& boundUniquely);
  bool solve(bool findFirst,
	     RewritingContext& solution,
	     stack<ConditionState*>& state);

  Term* getLhs() const;
  Sort* getSort() const;

private:
  Term* lhs;
  Sort* sort;
  RhsBuilder builder;
  int lhsIndex;
};

inline Term*
SortTestConditionFragment::getLhs() const
{
  return lhs;
}

inline Sort*
SortTestConditionFragment::getSort() const
{
  return sort;
}

#endif
