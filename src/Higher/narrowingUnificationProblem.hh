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
//	Hacked up version of UnificationProblem to support narrowing. This should ultimately
//	refactored with UnificationProblem. The reason for having a separate class at the moment is:
//
//	(1) Narrowing unification problems start with a pair of dag nodes rather than a pair of vectors of terms.
//	(2) Narrowing unification gets its variable info from the rule being used
//	(3) Narrowing unification uses a non-standard allocation of slots within substitutions:
//	
//	(rule variables) (rule rhs temporaries) (unused) (narrowing term variables)
//	^                ^                               ^
//	0                #protected for rule             size of substitution for module
//
//	(4) Unifiers should be expressed in terms of "fresh" variables that reuse the variable name and
//	substitution slot space.
//
#ifndef _narrowingUnificationProblem_hh_
#define _narrowingUnificationProblem_hh_
#include "variableInfo.hh"
#include "substitution.hh"
#include "simpleRootContainer.hh"
#include "pendingUnificationStack.hh"

class NarrowingUnificationProblem : private SimpleRootContainer
{
  NO_COPYING(NarrowingUnificationProblem);

public:
  NarrowingUnificationProblem(Rule* rule,
			      DagNode* target,
			      const NarrowingVariableInfo& variableInfo,
			      FreshVariableGenerator* freshVariableGenerator,
			      ExtensionInfo* extensionInfo);

  ~NarrowingUnificationProblem();

  bool findNextUnifier();
  Substitution& getSolution() const;
  int getNrFreeVariables() const;

private:
  void markReachableNodes();
  void findOrderSortedUnifiers();
  bool extractUnifier();
  bool explore(int index);

  Rule* const rule;
  DagNode* const target;
  const NarrowingVariableInfo& variableInfo;
  FreshVariableGenerator* const freshVariableGenerator;
  ExtensionInfo* const extensionInfo;

  int firstTargetSlot;
  int substitutionSize;
  const SortBdds* sortBdds;		// sort computation BDDs for our module

  UnificationContext* unsortedSolution;	// for accumulating solved forms and constructing unsorted unifiers
  //Subproblem* subproblem;		// for stuff unresolved by computeSolvedForm() pass
  PendingUnificationStack pendingStack;
  bool viable;				// true if problem didn't fail computeSolvedForm() pass
  NatSet freeVariables;	     		// indices (slots) of unbound variables in unsorted unifier
  AllSat* orderSortedUnifiers;		// satisfiability problem encoding sorts for order-sorted unifiers
  Substitution* sortedSolution;		// for construction order-sorted unifiers
  //
  //	Data for resolving dependencies in solved form.
  //
  Vector<int> order;			// we build an order in which to instantiate the solved forms
  NatSet done;				// a variable is done when every variable it (indirectly) depends on is in the order
  NatSet pending;			// variables on the current path though the dependency digraph
};

inline Substitution&
NarrowingUnificationProblem::getSolution() const
{
  return *sortedSolution;
}

inline int
NarrowingUnificationProblem::getNrFreeVariables() const
{
  return freeVariables.size();
}

#endif
