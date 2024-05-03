/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class NarrowingUnificationProblem.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "allSat.hh"
#include "bddUser.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "extensionInfo.hh"

//	core class definitions
#include "module.hh"
#include "sortBdds.hh"
#include "connectedComponent.hh"
#include "unificationContext.hh"
#include "freshVariableGenerator.hh"
#include "preEquation.hh"

//	variable class definitions
#include "variableDagNode.hh"
#include "variableTerm.hh"

//	higher class definitions
#include "narrowingVariableInfo.hh"
#include "narrowingUnificationProblem.hh"

NarrowingUnificationProblem::NarrowingUnificationProblem(PreEquation* preEquation,
							 DagNode* target,
							 const NarrowingVariableInfo& variableInfo,
							 FreshVariableGenerator* freshVariableGenerator,
							 int variableFamily)
  : preEquation(preEquation),
    nrPreEquationVariables(preEquation->getNrRealVariables()),
    variableInfo(variableInfo),
    freshVariableGenerator(freshVariableGenerator),
    variableFamily(variableFamily)
{
  DebugAdvisory(Tty(Tty::MAGENTA) << "==== NarrowingUnificationProblem ====" << Tty(Tty::RESET));
  DebugAdvisory("preEquation->getLhsDag() = " << preEquation->getLhsDag());
  DebugAdvisory("target = " << target);
  DebugAdvisory(Tty(Tty::MAGENTA) << "=========================================" << Tty(Tty::RESET));
  //
  //	This is the normal constructor where we are trying to narrow target with a rule or equation
  //	given by preEquation.
  //
  Module* module = preEquation->getModule();
  firstTargetSlot = module->getMinimumSubstitutionSize();  // first slot after variables reserved for preEquation
  substitutionSize = firstTargetSlot + variableInfo.getNrVariables();  // add the number of variables in target
  sortBdds = module->getSortBdds();
  //
  //	Initialize the sorted and unsorted solutions.
  //
  orderSortedUnifiers = 0;
  sortedSolution = new Substitution(substitutionSize);
  unsortedSolution = new UnificationContext(freshVariableGenerator, substitutionSize, variableFamily);
  for (int i = 0; i < substitutionSize; ++i)
    {
      sortedSolution->bind(i, 0);  // so GC doesn't barf
      //
      //	At very least the gap between the preEquation variables and the target variables
      //	must be zero'd out so that PendingUnificationStack cycle detection doesn't look
      //	at garbage.
      //	Also UnificationContext protects its slots from GC so they need to be safe values.
      //
      unsortedSolution->bind(i, 0);
    }
  //
  //	Solve the underlying many-sorted unification problem.
  //
  viable = preEquation->getLhsDag()->computeSolvedForm(target, *unsortedSolution, pendingStack);
}

NarrowingUnificationProblem::NarrowingUnificationProblem(DagNode* lhs,
							 DagNode* rhs,
							 const NarrowingVariableInfo& variableInfo,
							 FreshVariableGenerator* freshVariableGenerator,
							 int variableFamily)
  : preEquation(0),
    nrPreEquationVariables(0),
    variableInfo(variableInfo),
    freshVariableGenerator(freshVariableGenerator),
    variableFamily(variableFamily)
{
  DebugAdvisory(Tty(Tty::GREEN) << "==== NarrowingUnificationProblem ====" << Tty(Tty::RESET));
  DebugAdvisory("lhs = " << lhs);
  DebugAdvisory("rhs = " << rhs);
  DebugAdvisory(Tty(Tty::GREEN) << "=========================================" << Tty(Tty::RESET));
  //
  //	This is a special constructor used for the final lhs =? rhs step in variant unification.
  //	There is no rule or equation here. However we have indexed dagnodes rather than terms, and
  //	need the non-destructive unifier generation and variable family fresh variable naming capabilities
  //	of this class. Thus we can't use class UnificationProblem.
  //
  Module* module = lhs->symbol()->getModule();
  firstTargetSlot = module->getMinimumSubstitutionSize();  // we start allocating slots past those reserved for preEquation
  //cout << "firstTargetSlot: " << firstTargetSlot << endl;
  //cout << "nr unification variables: " << variableInfo.getNrVariables() << endl;
  substitutionSize = firstTargetSlot + variableInfo.getNrVariables();

  sortBdds = module->getSortBdds();
  //
  //	Initialize the sorted and unsorted solutions.
  //
  orderSortedUnifiers = 0;
  sortedSolution = new Substitution(substitutionSize);
  unsortedSolution = new UnificationContext(freshVariableGenerator, substitutionSize, variableFamily);
  for (int i = 0; i < substitutionSize; ++i)
    {
      sortedSolution->bind(i, 0);  // so GC doesn't barf
      unsortedSolution->bind(i, 0);  // HACK
    }
  //
  //	Solve the underlying many-sorted unification problem.
  //
  viable = lhs->computeSolvedForm(rhs, *unsortedSolution, pendingStack);
}

NarrowingUnificationProblem::~NarrowingUnificationProblem()
{
  delete orderSortedUnifiers;
  delete unsortedSolution;
  delete sortedSolution;
}

void
NarrowingUnificationProblem::markReachableNodes()
{
  {
    int nrFragile = sortedSolution->nrFragileBindings();
    for (int i = 0; i < nrFragile; i++)
      {
	DagNode* d = sortedSolution->value(i);
	if (d != 0)
	  d->mark();
      }
  }
}

bool
NarrowingUnificationProblem::findNextUnifier()
{
  if (!viable)
    return false;

  bool first = true;
  //
  //	Do we already have an unsorted solution that we can find another of assigning sorts to.
  //
  if (orderSortedUnifiers != 0)
    {
      if (orderSortedUnifiers->nextAssignment())
	goto good;
      else
	{
	  delete orderSortedUnifiers;
	  orderSortedUnifiers = 0;
	  first = false;
	}
    }
  //
  //	Keep extracting solved forms until we find one which has no cycles and which order-sortable.
  //	If we run out solved forms, we're done.
  //
  for(;;)
    {
      if (!pendingStack.solve(first, *unsortedSolution))
	return false;
      classifyVariables();
      if (findOrderSortedUnifiers())
	{
#ifdef NO_ASSERT
	  (void) orderSortedUnifiers->nextAssignment();
#else
	  bool t = orderSortedUnifiers->nextAssignment();
	  Assert(t, "no first order sorted unifier");
#endif
	  break;
	}
      first = false;
    }

 good:
  if (!freeVariables.empty())
    bindFreeVariables();
  return true;
}

Sort*
NarrowingUnificationProblem::variableIndexToSort(int index)
{
  if (index < nrPreEquationVariables)
    {
      //
      //	Original PreEquation variable.
      //
      return safeCastNonNull<VariableSymbol*>(preEquation->index2Variable(index)->symbol())->getSort();
    }
  if (index < substitutionSize)
    {
      Assert(index >= firstTargetSlot, "variable index in no man's land " << index);
      //
      //	Original dag variable.
      //
      return safeCastNonNull<VariableSymbol*>(variableInfo.index2Variable(index - firstTargetSlot)->symbol())->getSort();
    }
  //
  //	Fresh variable introduced by unification.
  //
  return unsortedSolution->getFreshVariableSort(index);
}

void
NarrowingUnificationProblem::bindFreeVariables()
{
  //
  //	We go through the variables that were free in the unsorted unifier and bind
  //	each to a fresh variable, whose sort is either that calculated by the BDDs
  //	for a constrained variables, or the sort of the original variable for an
  //	unconstrained variable.
  //
  const Vector<Byte>& assignment = orderSortedUnifiers->getCurrentAssignment();
  int bddVar = sortBdds->getFirstAvailableVariable();
  int freshVariableCount = 0;

  for (int fv : freeVariables)
    {
      //
      //	Get original sort of variable and its connected component.
      //
      Sort* sort = variableIndexToSort(fv);
      ConnectedComponent* component = sort->component();

      Sort* newSort;
      if (sortConstrainedVariables.contains(fv))
	{
	  //
	  //	The index of the new sort is binary encoded by the assignments to the
	  //	BDD variables for this free variable.
	  //
	  int nrBddVariables = sortBdds->getNrVariables(component->getIndexWithinModule());
	  int newSortIndex = 0;
	  for (int j = nrBddVariables - 1; j >= 0; --j)
	    {
	      newSortIndex <<= 1;
	      if (assignment[bddVar + j])
		++newSortIndex;
	    }
	  bddVar += nrBddVariables;
	  newSort = component->sort(newSortIndex);
	}
      else
	{
	  //
	  //	We have a free variable that didn't occur in any other variables binding;
	  //	therefore its sort isn't constrained and remains the same.
	  //
	  newSort = sort;
	}
      //
      //	Make a new fresh variable with correct sort and name.
      //
      //	By giving fv as the slot, we ensure that we can reinstantiate users of this variable
      //	for second and subsequent sorted solutions corresponding to our unsorted solution.
      //
      DagNode* newVariable =
	new VariableDagNode(freshVariableGenerator->getBaseVariableSymbol(newSort),
			    freshVariableGenerator->getFreshVariableName(freshVariableCount, variableFamily),
			    fv);
      //
      //	Bind slot to new variable.
      //
      sortedSolution->bind(fv, newVariable);
      ++freshVariableCount;
    }
  //
  //	Now go through the substitution, instantiating anything that is bound to something
  //	other than a free variable on the new free variables.
  //
  for (int i = 0; i < substitutionSize; ++i)
    {
      if (DagNode* d = sortedSolution->value(i))  // might be an unused slot
	{
	  if (!(freeVariables.contains(i)))  // not a free variable
	    {
	      if (DagNode* d2 = d->instantiate(*sortedSolution, true))
		sortedSolution->bind(i, d2);
	    }
	}
    }
}

bool
NarrowingUnificationProblem::findOrderSortedUnifiers()
{
  //
  //	We use clone() rather than copy() because addNewVariable() may have
  //	increased the number of variables above the number of original variables.
  //
  sortedSolution->clone(*unsortedSolution);
  //
  //	For each free variable allocate a block of BDD variables based on the
  //	size of the connected component of its sort.
  //
  //	We may have a bigger substitution than we started with due to fresh variables
  //	being created to express unifiers in theories such as AC.
  //	
  int newSubstitutionSize = sortedSolution->nrFragileBindings();
  //
  //	We keep track of the first BDD variable used for each free variable.
  //
  Vector<int> realToBdd(newSubstitutionSize);
  int nextBddVariable = sortBdds->getFirstAvailableVariable();
  {
    //
    //	Each variable that was noted as being sort constrained when the unsorted
    //	unifier was extract now gets allocated a vector of BDD variables to encode its
    //	sort.
    //
    for (int scv : sortConstrainedVariables)
      {
	realToBdd[scv] = nextBddVariable;
	Sort* sort = variableIndexToSort(scv);
	nextBddVariable += sortBdds->getNrVariables(sort->component()->getIndexWithinModule());
      }
  }
  DebugAdvisory("allocated " << nextBddVariable << " BDD variables" <<
		" sc vars " << sortConstrainedVariables.size() << " freeVariables " << freeVariables.size());
  //
  //	Make sure BDD package has enough variables allocated.
  //
  BddUser::setNrVariables(nextBddVariable);
  //
  //	Constrain fresh sort constrained variables to have sort indices in valid range.
  //
  Bdd unifier = bddtrue;
  {
    for (int fv : sortConstrainedVariables)
      {
	if (fv >= substitutionSize)
	  {
	    Sort* sort = unsortedSolution->getFreshVariableSort(fv);
	    Bdd leqRelation = sortBdds->getRemappedLeqRelation(sort, realToBdd[fv]);
	    unifier = bdd_and(unifier, leqRelation);
	    DebugAdvisory("NarrowingUnificationProblem::findOrderSortedUnifiers() : Adding constraint for free, non-original variable; nodes in new bdd = " <<
			  bdd_nodecount(unifier) << " unifier = " << unifier);

	    DebugAdvisory("NarrowingUnificationProblem::findOrderSortedUnifiers() : Adding constraint for free, non-original variable: "
			  << leqRelation << " unifier becomes " << unifier);
	  }
      }
  }
  //
  //	Now compute a BDD which tells us if a given assignment of sorts to free
  //	variables yields an order-sorted unifier.
  //
  for (int i = 0; i < substitutionSize; ++i)  // consider original variables only
    {
      //
      //	We don't even look at fresh variables - they are not subject to original
      //	constraints - they get implicitly constrained by where they show up if
      //	they are free. Therefore each slot we examine corresponds to:
      //	(1) an original variable that was bound;
      //	(2) an original variable that is free but sort constrained by its occurrence
      //	    in bindings to other variables;
      //	(3) an original variable that is free and doesn't occur in bindings to other
      //	    variables; or
      //	(4) an unused slot lying between the preEquation variables and the dag
      //	    variables (no man's land).
      //
      //	Cases (3) and (4) play no role in the BDD based sort computations.
      //
      DagNode* d = sortedSolution->value(i);
      if (d != 0 || sortConstrainedVariables.contains(i))
	{
	  Sort* sort = variableIndexToSort(i);
	  Bdd leqRelation;
	  if (d != 0)
	    {
	      //
	      //	(1) Bound variable: term must have sort <= variables sort.
	      //
	      Vector<Bdd> genSort;
	      d->computeGeneralizedSort2(*sortBdds, realToBdd, genSort);	  

	      leqRelation = sortBdds->applyLeqRelation(sort, genSort);
	      DebugAdvisory("bound to " << d << " induces leqRelation " << leqRelation);
	    }
	  else
	    {
	      //
	      //	(2) Free, sort constrained variable: sort assigned to free variable
	      //	must be <= variables original sort.
	      //
	      leqRelation = sortBdds->getRemappedLeqRelation(sort, realToBdd[i]);
	      DebugAdvisory("free variable induces leqRelation " << leqRelation);
	    }
	  unifier = bdd_and(unifier, leqRelation);
	  DebugAdvisory("unifier = " << unifier);
	  if (unifier == bddfalse)
	    return false;
	}
    }
  //
  //	Now compute a BDD that tells us if a unifier has maximal sort assignments to free
  //	variables.
  //
  //	maximal(X1,...,  Xn) = unifier(X1,..., Xn) &
  //	  for each i in 1 ... n
  //	    not(exists Yi .[gt(Yi,Xi) & unifier(X1,..., Y1,...., Xn)])
  //
  Bdd maximal = unifier;
  int secondBase = sortBdds->getFirstAvailableVariable();
  for (int fv : sortConstrainedVariables)
    {
      Sort* sort = variableIndexToSort(fv);
      int nrBddVariables = sortBdds->getNrVariables(sort->component()->getIndexWithinModule());
      //
      //	Replace the bdd variables allocated to the real variable in unifier with
      //	0...nrBddVariables-1.
      //	Replace the bdd variables firstAvail,..., firstAvail+nrBddVariables-1 in
      //	gtRelation with the variables allocated to real variable.
      //
      bddPair* realTofirstArg = bdd_newpair();
      bddPair* secondArgToReal = BddUser::getCachedPairing();
      int firstVar = realToBdd[fv];
      for (int j = 0; j < nrBddVariables; ++j)
	{
	  int bddVariable = firstVar + j;
	  bdd_setpair(realTofirstArg, bddVariable, j);
	  bdd_setpair(secondArgToReal, j, j);
	  bdd_setpair(secondArgToReal, secondBase + j, bddVariable);
	}
      Bdd gtRelation = sortBdds->getGtRelation(sort->component()->getIndexWithinModule());
      Bdd modifiedGtRelation = bdd_replace(gtRelation, secondArgToReal);
      Bdd modifiedUnifier = bdd_replace(unifier, realTofirstArg);
      bdd_freepair(realTofirstArg);

      maximal = bdd_and(maximal, bdd_appall(modifiedGtRelation, modifiedUnifier, bddop_nand,
					    sortBdds->makeVariableBdd(0, nrBddVariables)));
      Assert(maximal != bddfalse, "maximal false even though unifier isn't");
    }
  orderSortedUnifiers = new AllSat(maximal, secondBase, nextBddVariable - 1);
  return true;
}

void
NarrowingUnificationProblem::classifyVariables()
{
  //
  //	A narrowing unification problem may contain original variables
  //	that are neither bound nor show up in a binding to an original variable.
  //	This is because while they occur in the target term, they don't occur below
  //	the position being narrowed.
  //	Thus we need to be careful about what unbound variables we treat as free
  //	variables and which variables get sorts represented by BDD variables.
  //
  //	We are interested in two sets of variables:
  //	  Those which must be bound to a fresh variable under our final numbering scheme.
  //	  Those for which a sort must be calculated based on the their occurances in bindings.
  //	The former is a superset of the latter.
  //
  //	Look at original variables. We compute two sets:
  //	freeVariables is intially the set of unbound original variables.
  //	sortConstrainedVariables is the set of variables appearing in a binding
  //	to an original variable.
  //
  freeVariables.clear();
  sortConstrainedVariables.clear();
  for (int i = 0; i < substitutionSize; ++i)
    {
      if (i < nrPreEquationVariables || i >= firstTargetSlot)
	{
	  DagNode* value = unsortedSolution->value(i);
	  if (value == 0)
	    freeVariables.insert(i);
	  else
	    value->insertVariables(sortConstrainedVariables);
	}
    }
  //
  //	Sort constrained variables are also treated as free variables
  //	in that they will be bound to a fresh variable in the final unifier.
  //
  freeVariables.insert(sortConstrainedVariables);
}
