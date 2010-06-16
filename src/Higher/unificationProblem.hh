/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2010 SRI International, Menlo Park, CA 94025, USA.

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
#include "cacheableState.hh"
#include "simpleRootContainer.hh"
#include "variableInfo.hh"
#include "substitution.hh"
#include "pendingUnificationStack.hh"

class UnificationProblem : public CacheableState, private SimpleRootContainer
{
  NO_COPYING(UnificationProblem);

public:
  UnificationProblem(Vector<Term*>& lhs,
		     Vector<Term*>& rhs,
		     FreshVariableGenerator* freshVariableGenerator);
  ~UnificationProblem();

  bool problemOK() const;
  bool findNextUnifier();
  const Substitution& getSolution() const;
  int getNrFreeVariables() const;
  const VariableInfo& getVariableInfo() const;

private:
  void markReachableNodes();
  void findOrderSortedUnifiers();
  bool extractUnifier();
  bool explore(int index);

  Vector<Term*> leftHandSides;
  Vector<Term*> rightHandSides;
  FreshVariableGenerator* freshVariableGenerator;

  VariableInfo variableInfo;

  const SortBdds* sortBdds;		// sort computation BDDs for our module
  Vector<DagNode*> leftHandDags;
  Vector<DagNode*> rightHandDags;

  UnificationContext* unsortedSolution;	// for accumulating solved forms and constructing unsorted unifiers
  PendingUnificationStack pendingStack;
  bool problemOkay;			// true if problem didn't violate ctor invariants
  bool viable;				// true if problem didn't fail computeSolvedForm() pass
  Vector<int> freeVariables;	     	// indices (slots) of unbound variables in unsorted unifier
  AllSat* orderSortedUnifiers;		// satisfiability problem encoding sorts for order-sorted unifiers
  Substitution* sortedSolution;		// for construction order-sorted unifiers
  //
  //	Data for resolving dependencies in solved form.
  //
  Vector<int> order;			// we build an order in which to instantiate the solved forms
  NatSet done;				// a variable is done when every variable it (indirectly) depends on is in the order
  NatSet pending;			// variables on the current path though the dependency digraph
};

inline bool
UnificationProblem::problemOK() const
{
  return problemOkay;
}

inline const Substitution&
UnificationProblem::getSolution() const
{
  return *sortedSolution;
}

inline const VariableInfo&
UnificationProblem::getVariableInfo() const
{
  return variableInfo;
}

inline int
UnificationProblem::getNrFreeVariables() const
{
  return freeVariables.size();
}

#endif
