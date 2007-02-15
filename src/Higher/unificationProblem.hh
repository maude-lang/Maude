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
//	Class for searching for order-sorted unifiers.
//
#ifndef _unificationProblem_hh_
#define _unificationProblem_hh_
#include "variableInfo.hh"
#include "substitution.hh"
#include "simpleRootContainer.hh"

class UnificationProblem : public VariableInfo, private SimpleRootContainer
{
  NO_COPYING(UnificationProblem);

public:
  class FreshVariableGenerator
  {
  public:
    virtual ~FreshVariableGenerator() {}
    virtual int getFreshVariableName() = 0;
    virtual Symbol* getBaseVariableSymbol(Sort* sort) = 0;
    virtual void reset() = 0;
  };

  UnificationProblem(Term* lhs, Term* rhs, FreshVariableGenerator* freshVariableGenerator);
  ~UnificationProblem();

  bool findNextUnifier();
  const Substitution& getSolution() const;

private:
  void markReachableNodes();
  void findOrderSortedUnifiers();

  Term* lhs;
  Term* rhs;
  FreshVariableGenerator* freshVariableGenerator;

  const SortBdds* sortBdds;
  DagNode* lhsDag;
  DagNode* rhsDag;

  RewritingContext* unsortedSolution;
  Subproblem* subproblem;
  bool viable;
  Vector<int> freeVariables;
  AllSat* orderSortedUnifiers;
  Substitution* sortedSolution;
};

inline const Substitution&
UnificationProblem::getSolution() const
{
  return *sortedSolution;
}

#endif
