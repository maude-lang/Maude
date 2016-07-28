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
//	(5) Narrowing unification does not corrupt previously generated unifiers when making the next unifier, at
//	the cost of more copying if an unsorted unifier corresponds to multiple order-sorted unifiers.
//	(6) Narrowing unification supports the idea of "odd" and "even" variables to avoid clashing variable
//	names on successive steps of narrowing.
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
  //
  //	The usual case is that we are unifying the lhs of a rule or variant equation against a DagNode* target.
  //
  NarrowingUnificationProblem(PreEquation* preEquation,
			      DagNode* target,
			      const NarrowingVariableInfo& variableInfo,
			      FreshVariableGenerator* freshVariableGenerator,
			      bool odd = false);
  //
  //	However for variant unification, we also need to unify a pair of DagNode* arguments.
  //
  NarrowingUnificationProblem(DagNode* lhs,
			      DagNode* rhs,
			      const NarrowingVariableInfo& variableInfo,
			      FreshVariableGenerator* freshVariableGenerator,
			      bool odd);

  ~NarrowingUnificationProblem();

  bool findNextUnifier();
  Substitution& getSolution() const;
  int getNrFreeVariables() const;
  bool isIncomplete() const;

private:
  void markReachableNodes();
  Sort* variableIndexToSort(int index);
  void bindFreeVariables();
  bool findOrderSortedUnifiers();
  void classifyVariables();

  PreEquation* const preEquation;
  const int nrPreEquationVariables;
  const NarrowingVariableInfo& variableInfo;
  FreshVariableGenerator* const freshVariableGenerator;
  const bool odd;

  int firstTargetSlot;			// start of slots for variables in target
  int substitutionSize;			// initial substitution size (before any fresh variables are added)
  const SortBdds* sortBdds;		// sort computation BDDs for our module

  UnificationContext* unsortedSolution;	// for accumulating solved forms and constructing unsorted unifiers
  PendingUnificationStack pendingStack;
  bool viable;				// true if problem didn't fail computeSolvedForm() pass
  NatSet freeVariables;	     		// indices (slots) of unbound variables in unsorted unifier
  NatSet sortConstrainedVariables;	// subset of the above whose sorts are constrained by their appearence in bindings of other variables
  AllSat* orderSortedUnifiers;		// satisfiability problem encoding sorts for order-sorted unifiers
  Substitution* sortedSolution;		// for construction order-sorted unifiers
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

inline bool
NarrowingUnificationProblem::isIncomplete() const
{
  //cout << "NarrowingUnificationProblem::isIncomplete() returned " << pendingStack.isIncomplete() << endl;
  return pendingStack.isIncomplete();
}

#endif
