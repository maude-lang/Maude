/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Class to solve a variant unification problem between the lhs of a rule and
//	a subdag of a dag being narrowed. This is essentially a wrapper around
//	VariantSearch that translates variant unifiers that are expressed by
//	vector of dags and a NarrowingVariableInfo object into a substitution
//	for a potentially larger class of variables described by the given
//	NarrowingVariableInfo object, and which may include variables that
//	only appear the larger dag.
//
#ifndef _variantUnificationProblem_hh_
#define _variantUnificationProblem_hh_
#include "simpleRootContainer.hh"
#include "variantSearch.hh"

class VariantUnificationProblem : private SimpleRootContainer
{
  NO_COPYING(VariantUnificationProblem);

public:
  //
  //	This extends VariantSearch::Flags and one day maybe merged into
  //	such if FilteredVariantUnifierSearch is merged into VariantSearch.
  //
  enum Flags
    {
     FILTER_VARIANT_UNIFIERS = 0x1000,
    };
  //
  //	context is just used to create a new context to work in.
  //
  //	blockerDags are indirectly protected from GC by internal VariantSearch object.
  //
  //	The unificands are the lhs of the preEquation and the target. Note that the sets
  //	of variables in these two unificands must be disjoint. The variables in the lhs
  //	of the preEquations will have slots starting from 0 while the variables in target
  //	must be given indices >= getMinimumSubstitutionSize()
  //	target should not be rewritten in place while VariantUnificationProblem object exists.
  //
  //	Solutions have bindings for every variable in the preEquation (even those not in the lhs)
  //	and every variable mentioned in variableInfo (even those not in target). In those
  //	cases binding is just a fresh variable not mentioned anywhere else.
  //
  //	Fresh variables used to express a unifier belong to a family other than
  //	disallowedVariableFamily, but different solutions may be expressed in variables
  //	from different families.	
  //
  VariantUnificationProblem(RewritingContext* context,
			    const Vector<DagNode*>& blockerDags,
			    PreEquation* preEquation,
			    DagNode* target,
			    const NarrowingVariableInfo& variableInfo,
			    FreshVariableGenerator* freshVariableGenerator,
			    int disallowedVariableFamily,
			    int variantFlags);
  ~VariantUnificationProblem();

  bool isIncomplete() const;
  //
  //	These are specific to unifiers and can be overriden by a derived class.
  //
  bool findNextUnifier();
  Substitution& getSolution() const;
  int getNrFreeVariables() const;
  int getVariableFamily() const;

private:
  void markReachableNodes();

  RewritingContext* const context;
  PreEquation* preEquation;
  const NarrowingVariableInfo& variableInfo;
  int firstTargetSlot;
  int substitutionSize;
  int nrPreEquationVariables;
  RewritingContext* newContext;
  VariantSearch* variantSearch;
  FreshVariableGenerator* freshVariableGenerator;
  //
  //	Information about the current unifier.
  //
  Substitution* solution;
  int nrFreeVariables;
  int variableFamily;
};

inline Substitution&
VariantUnificationProblem::getSolution() const
{
  return *solution;
}

inline int
VariantUnificationProblem::getNrFreeVariables() const
{
  //
  //	Return number of free variables used to express a unifier; includes
  //	any fresh variables created for preEquation variables and variableInfo variables
  //	that didn't occur in the actual unfication problem.
  //
  return nrFreeVariables;
}

inline int
VariantUnificationProblem::getVariableFamily() const
{
  //
  //	Returns index of variable family used to express current solutions.
  //
  return variableFamily;
}
 
inline bool
VariantUnificationProblem::isIncomplete() const
{
  //
  //	Returns true if any incompleteness has been encountered so far.
  //
  return variantSearch->isIncomplete();
}

#endif
