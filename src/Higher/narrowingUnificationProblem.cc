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
#include "rule.hh"

//	variable class definitions
#include "variableDagNode.hh"
#include "variableTerm.hh"

//	higher class definitions
#include "narrowingVariableInfo.hh"
#include "narrowingUnificationProblem.hh"

NarrowingUnificationProblem::NarrowingUnificationProblem(Rule* rule,
							 DagNode* target,
							 const NarrowingVariableInfo& variableInfo,
							 FreshVariableGenerator* freshVariableGenerator)
  : rule(rule),
    target(target),
    variableInfo(variableInfo),
    freshVariableGenerator(freshVariableGenerator)
{
  Module* module = rule->getModule();
  firstTargetSlot = module->getMinimumSubstitutionSize();
  substitutionSize = firstTargetSlot + variableInfo.getNrVariables();
  sortBdds = module->getSortBdds();
  //
  //	Initialize the sorted and unsorted solutions.
  //
  orderSortedUnifiers = 0;
  sortedSolution = new Substitution(substitutionSize);
  unsortedSolution = new UnificationContext(freshVariableGenerator, substitutionSize);
  for (int i = 0; i < substitutionSize; ++i)
    {
      sortedSolution->bind(i, 0);  // so GC doesn't barf
      unsortedSolution->bind(i, 0);  // HACK
    }
  //
  //	Solve the underlying many-sorted unification problem.
  //
  viable = rule->getLhsDag()->computeSolvedForm(target, *unsortedSolution, pendingStack);
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
  if (orderSortedUnifiers == 0)
    {
      //
      //	First solution.
      //
      if (!(pendingStack.solve(true, *unsortedSolution)))
	  return false;
      if (!extractUnifier())
	goto nextUnsorted;
      findOrderSortedUnifiers();
      if (orderSortedUnifiers == 0)
	goto nextUnsorted;
#ifdef NO_ASSERT
      (void) orderSortedUnifiers->nextAssignment();
#else
      bool t = orderSortedUnifiers->nextAssignment();
      Assert(t, "no first order sorted unifier");
#endif
    }
  else
    {
      //
      //	Try to find another way of assigning sorts to current solution.
      //
      if (!(orderSortedUnifiers->nextAssignment()))
	{
	  delete orderSortedUnifiers;
	  orderSortedUnifiers = 0;
	nextUnsorted:
	  if (!(pendingStack.solve(false, *unsortedSolution)))
	    return false;
	  if (!extractUnifier())
	    goto nextUnsorted;
	  //freshVariableGenerator->reset();
	  findOrderSortedUnifiers();
	  if (orderSortedUnifiers == 0)
	    goto nextUnsorted;
#ifdef NO_ASSERT
	  (void) orderSortedUnifiers->nextAssignment();
#else
	  bool t = orderSortedUnifiers->nextAssignment();
	  Assert(t, "no first order sorted unifier");
#endif
	}
    }

  const Vector<Byte>& assignment = orderSortedUnifiers->getCurrentAssignment();
  int bddVar = sortBdds->getFirstAvailableVariable();
  FOR_EACH_CONST(i, NatSet, freeVariables)
    {
      DagNode* variable = sortedSolution->value(*i);
      ConnectedComponent* component = variable->symbol()->rangeComponent();
      //
      //	Compute the index of the new sort for this variable from the
      //	value of the BDD variables in the current satisfying assignment.
      //
      int nrBddVariables = sortBdds->getNrVariables(component->getIndexWithinModule());
      int index = 0;
      for (int j = nrBddVariables - 1; j >= 0; --j)
	{
	  index <<= 1;
	  if (assignment[bddVar + j])
	    ++index;
	}
      bddVar += nrBddVariables;
      //
      //	Replace each variable symbol in a free variable with the
      //	variable symbol corresponding to its newly calculated sort.
      //
      variable->replaceSymbol(freshVariableGenerator->getBaseVariableSymbol(component->sort(index)));
    }
  return true;
}

void
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
  int nrRuleVariables = rule->getNrProtectedVariables();
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
    FOR_EACH_CONST(i, NatSet, freeVariables)
      {
	int fv = *i;
	realToBdd[fv] = nextBddVariable;
	//
	//	There are 3 possibilities for a free variable:
	//	(1) it came from the rule;
	//	(2) it came from the dag being narrowed; or
	//	(3) it was a fresh variable introduced by unsorted unification.
	//
	Sort* sort;
	if (fv < nrRuleVariables)
	  sort = safeCast(VariableSymbol*, rule->index2Variable(fv)->symbol())->getSort();
	else if (fv < substitutionSize)
	  sort = safeCast(VariableSymbol*, variableInfo.index2Variable(fv - firstTargetSlot)->symbol())->getSort();
	else
	  sort = unsortedSolution->getFreshVariableSort(fv);
	nextBddVariable += sortBdds->getNrVariables(sort->component()->getIndexWithinModule());
      }
  }
  //
  //	Make sure BDD package has enough variables allocated.
  //
  BddUser::setNrVariables(nextBddVariable);
  //
  //	Constrains free fresh variables to have indices in valid range.
  //
  Bdd unifier = bddtrue;
  {
    FOR_EACH_CONST(i, NatSet, freeVariables)
      {
	int fv = *i;
	if (fv >= substitutionSize)
	  {
	    Sort* sort = unsortedSolution->getFreshVariableSort(fv);
	    int nrBddVariables = sortBdds->getNrVariables(sort->component()->getIndexWithinModule());
	    int firstVar = realToBdd[fv];
	    
	    bddPair* bitMap = bdd_newpair();
	    for (int j = 0; j < nrBddVariables; ++j)
	      bdd_setbddpair(bitMap, j, bdd_ithvar(firstVar + j));
	    
	    Bdd leqRelation = sortBdds->getLeqRelation(sort->getIndexWithinModule());
	    leqRelation = bdd_veccompose(leqRelation, bitMap);
	    unifier = bdd_and(unifier, leqRelation);
	    DebugAdvisory("NarrowingUnificationProblem::findOrderSortedUnifiers() : Adding constraint for free, non-original variable: "
			  << leqRelation << " unifier becomes " << unifier);
	    bdd_freepair(bitMap);
	  }
      }
  }
  //
  //	Now compute a BDD which tells us if a given assignment of sorts to free
  //	variables yields an order-sorted unifier.
  //
  for (int i = 0; i < substitutionSize; ++i)
    {
      //
      //	We don't even look at fresh variables. Each slot corresponds to:
      //	(1) an original variable that was bound;
      //	(2) an original variable that is free; or
      //	(3) an unused slot lying between the rule variables and the dag variables.
      //
      DagNode* d = sortedSolution->value(i);
      if (d != 0 || freeVariables.contains(i))
	{
	  bddPair* bitMap = bdd_newpair();
	  Sort* sort = (i < nrRuleVariables) ?
	    safeCast(VariableSymbol*, rule->index2Variable(i)->symbol())->getSort() :
	    safeCast(VariableSymbol*, variableInfo.index2Variable(i - firstTargetSlot)->symbol())->getSort();
	  Bdd leqRelation = sortBdds->getLeqRelation(sort->getIndexWithinModule());
	  if (d != 0)
	    {
	      //
	      //	Bound variable: term must have sort <= variables sort.
	      //
	      Vector<Bdd> genSort;
	      d->computeGeneralizedSort(*sortBdds, realToBdd, genSort);	  
	      int nrBdds =  genSort.size();
	      for (int j = 0; j < nrBdds; ++j)
		bdd_setbddpair(bitMap, j, genSort[j]);
	      leqRelation = bdd_veccompose(leqRelation, bitMap);
	    }
	  else
	    {
	      //
	      //	Free variable: sort assigned to free variable must be <= variables original sort.
	      //
	      int firstVar = realToBdd[i];
	      int nrBdds= sortBdds->getNrVariables(sort->component()->getIndexWithinModule());
	      for (int j = 0; j < nrBdds; ++j)
		bdd_setpair(bitMap, j, firstVar + j);
	      leqRelation = bdd_replace(leqRelation, bitMap);
	    }
	  bdd_freepair(bitMap);
	  unifier = bdd_and(unifier, leqRelation);
	  if (unifier == bddfalse)
	    return;
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
  FOR_EACH_CONST(i, NatSet, freeVariables)
    {
      int fv = *i;
      Sort* sort;
      if (fv < nrRuleVariables)
	sort = safeCast(VariableSymbol*, rule->index2Variable(fv)->symbol())->getSort();
      else if (fv < substitutionSize)
	sort = safeCast(VariableSymbol*, variableInfo.index2Variable(fv - firstTargetSlot)->symbol())->getSort();
      else
	sort = unsortedSolution->getFreshVariableSort(fv);

     int nrBddVariables = sortBdds->getNrVariables(sort->component()->getIndexWithinModule());
      //
      //	Replace the bdd variables allocated to the real variable in unifier with 0...nrBddVariables-1.
      //	Replace the bdd variables firstAvail,..., firstAvail+nrBddVariables-1 in gtOp with the
      //	variables allocated to real variable.
      //
      bddPair* realTofirstArg = bdd_newpair();
      bddPair* secondArgToReal = bdd_newpair();
      int firstVar = realToBdd[fv];
      for (int j = 0; j < nrBddVariables; ++j)
	{
	  int bddVariable = firstVar + j;
	  bdd_setpair(realTofirstArg, bddVariable, j);
	  bdd_setpair(secondArgToReal, secondBase + j, bddVariable);
	}
      Bdd gtRelation = sortBdds->getGtRelation(sort->component()->getIndexWithinModule());
      Bdd modifiedGtRelation = bdd_replace(gtRelation, secondArgToReal);
      bdd_freepair(secondArgToReal);
      Bdd modifiedUnifier = bdd_replace(unifier, realTofirstArg);
      bdd_freepair(realTofirstArg);
      maximal = bdd_and(maximal, bdd_appall(modifiedGtRelation, modifiedUnifier, bddop_nand,
					    sortBdds->makeVariableBdd(0, nrBddVariables)));
      Assert(maximal != bddfalse, "maximal false even though unifier isn't");
    }
  orderSortedUnifiers = new AllSat(maximal, secondBase, nextBddVariable - 1);
  if (!freeVariables.empty())
    {
      int freshVariableCount = 0;
      //
      //	Finally we bind each free variable to a fresh variable of the appropriate sort and
      //	instantiate all bindings to replace all occurences of the (formerly) free variables.
      //
      FOR_EACH_CONST(i, NatSet, freeVariables)
	{
	  int fv = *i;
	  Sort* sort;
	  if (fv < nrRuleVariables)
	    sort = safeCast(VariableSymbol*, rule->index2Variable(fv)->symbol())->getSort();
	  else if (fv < substitutionSize)
	    sort = safeCast(VariableSymbol*, variableInfo.index2Variable(fv - firstTargetSlot)->symbol())->getSort();
	  else
	    sort = unsortedSolution->getFreshVariableSort(fv);

	  sortedSolution->bind(fv, new VariableDagNode(freshVariableGenerator->getBaseVariableSymbol(sort),
						      freshVariableGenerator->getFreshVariableName(freshVariableCount),
						      fv));
	  ++freshVariableCount;
	}
      for (int i = 0; i < substitutionSize; ++i)
	{
	  if (DagNode* d = sortedSolution->value(i))  // might be an unused slot
	    {
	      if (DagNode* d2 = d->instantiate(*sortedSolution))
		sortedSolution->bind(i, d2);
	    }
	}
    }
}

bool
NarrowingUnificationProblem::extractUnifier()
{
  //
  //	We try to extract a unifier from the solved form by first ordering
  //	bound variables by their dependencies and if that can be done, by
  //	instantiating their bindings in that order.
  //
  int nrRuleVariables = rule->getNrProtectedVariables();
  order.clear();
  done.clear();
  pending.clear();
  freeVariables.clear();
  for (int i = 0; i < substitutionSize; ++i)
    {
      if (i < nrRuleVariables || i >= firstTargetSlot)
	{
	  if (!explore(i))
	    return false;
	}
    }
  FOR_EACH_CONST(i, Vector<int>, order)
    {
      //cout << "processing " << *i << endl;
      if (DagNode* d = unsortedSolution->value(*i)->instantiate(*unsortedSolution))
	unsortedSolution->bind(*i, d);
    }
  return true;
}

bool
NarrowingUnificationProblem::explore(int index)
{
  //
  //	Depth-first exploration of the dependencies of the binding for the
  //	variable with a given index.
  //
  if (done.contains(index))
    return true;
  DagNode* d = unsortedSolution->value(index);
  if (d == 0)
    {
      freeVariables.insert(index);
      done.insert(index);
      return true;
    }
  NatSet occurs;
  d->insertVariables(occurs);
  if (!occurs.disjoint(pending))
    return false;  // dependency cycle
  occurs.subtract(done);
  if (!occurs.empty())
    {
      pending.insert(index);
      FOR_EACH_CONST(i, NatSet, occurs)
	{
	  if (!explore(*i))
	    return false;
	}
      pending.subtract(index);
    }
  order.append(index);
  done.insert(index);
  return true; 
}
