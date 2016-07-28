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
//	Implementation for class UnificationProblem.
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
//	variable class definitions
#include "variableDagNode.hh"
#include "variableTerm.hh"

//	core class definitions
#include "module.hh"
#include "sortBdds.hh"
#include "connectedComponent.hh"
#include "unificationContext.hh"
#include "freshVariableGenerator.hh"

//	higher class definitions
#include "unificationProblem.hh"

UnificationProblem::UnificationProblem(Vector<Term*>& lhs,
				       Vector<Term*>& rhs,
				       FreshVariableGenerator* freshVariableGenerator)
  : freshVariableGenerator(freshVariableGenerator)
{
#ifndef NO_ASSERT
  if (globalAdvisoryFlag)
    {
      cerr << Tty(Tty::RED) << "==== UnificationProblem ====" << Tty(Tty::RESET) << endl;
      for (int i = 0; i < lhs.length(); ++i)
	cerr << "lhs[" << i << "] = " << lhs[i] << endl;
      for (int i = 0; i < rhs.length(); ++i)
	cerr << "rhs[" << i << "] = " << rhs[i] << endl;
      cerr << Tty(Tty::RED) << "=========================================" << Tty(Tty::RESET) << endl;
    }
#endif

  problemOkay = false;  // until we have verified it is ok
  Assert(lhs.size() == rhs.size(), "lhs/rhs size clash");
  leftHandSides.swap(lhs);
  rightHandSides.swap(rhs);
  sortBdds = leftHandSides[0]->symbol()->getModule()->getSortBdds();
  //
  //	Preprocess terms.
  //
  int nrEquations = leftHandSides.size();
  for (int i = 0; i < nrEquations; ++i)
    {
      Term*& lhs = leftHandSides[i];
      lhs = lhs->normalize(true);
      lhs->indexVariables(variableInfo);
      Term*& rhs = rightHandSides[i];
      rhs = rhs->normalize(true);
      rhs->indexVariables(variableInfo);
    }
  //
  //	Check that variables have safe names.
  //
  int nrOriginalVariables = variableInfo.getNrRealVariables();
  for (int i = 0; i < nrOriginalVariables; ++i)
    {
      Term* v = variableInfo.index2Variable(i);
      if (freshVariableGenerator->variableNameConflict(safeCast(VariableTerm*, v)->id()))
	{
	  IssueWarning("unsafe variable name " << QUOTE(v) << " in unification problem.");
	  return;
	}
    }
  //
  //	Create corresponding dags.
  //
  leftHandDags.resize(nrEquations);
  rightHandDags.resize(nrEquations);
  for (int i = 0; i < nrEquations; ++i)
    {
      leftHandDags[i] = leftHandSides[i]->term2Dag();
      if (leftHandDags[i]->computeBaseSortForGroundSubterms() == DagNode::UNIMPLEMENTED)
	return;
      rightHandDags[i] = rightHandSides[i]->term2Dag();
      if (rightHandDags[i]->computeBaseSortForGroundSubterms() == DagNode::UNIMPLEMENTED)
	return;
    }
  problemOkay = true;
  //
  //	Initialize the sorted and unsorted solutions.
  //
  orderSortedUnifiers = 0;
  sortedSolution = new Substitution(nrOriginalVariables);
  unsortedSolution = new UnificationContext(freshVariableGenerator, nrOriginalVariables);
  for (int i = 0; i < nrOriginalVariables; ++i)
    {
      sortedSolution->bind(i, 0);  // so GC doesn't barf
      unsortedSolution->bind(i, 0);  // HACK
    }
  //
  //	Solve the underlying many-sorted unification problem.
  //
#if 0
  cout << "%%%%%UnificationProblem:\n";
  for (int i = 0; i < nrEquations; ++i)
    cout << leftHandDags[i] << " =? " << rightHandDags[i] << endl;
#endif
  for (int i = 0; i < nrEquations; ++i)
    {
      if (!(leftHandDags[i]->computeSolvedForm(rightHandDags[i], *unsortedSolution, pendingStack)))
	{
#if 0
	  cout << "NO SOLVED FORM" << endl;
#endif
	  viable = false;
	  return;
	}
    }
  viable = true;
}

UnificationProblem::~UnificationProblem()
{
  //cerr << this << " ~UnificationProblem()" << endl;
  delete freshVariableGenerator;
  if (problemOkay)
    {
      delete orderSortedUnifiers;
      delete unsortedSolution;
      delete sortedSolution;
    }
  //
  //	Only now can we safely destruct these as they are needed by VariableInfo.
  //
  int nrEquations = leftHandSides.size();
  for (int i = 0; i < nrEquations; ++i)
    {
      leftHandSides[i]->deepSelfDestruct();
      rightHandSides[i]->deepSelfDestruct();
    }
}

void
UnificationProblem::markReachableNodes()
{
  //cerr << this << " markReachableNodes()" << endl;
  {
    int nrEquations = leftHandSides.size();
    for (int i = 0; i < nrEquations; ++i)
      {
	leftHandDags[i]->mark();
	rightHandDags[i]->mark();
      }
  }
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
UnificationProblem::findNextUnifier()
{
  if (!viable)
    return false;

  bool first = (orderSortedUnifiers == 0);
  //
  //	Do we already have an unsorted solution that we can find another of assigning sorts to.
  //
  if (!first)
    {
      if (orderSortedUnifiers->nextAssignment())
	goto good;
      delete orderSortedUnifiers;
      orderSortedUnifiers = 0;
    }
  //
  //	Keep extracting solved forms until we find one which has no cycles and which order-sortable.
  //	If we run out solved forms, we're done.
  //
  do
    {
      if (!pendingStack.solve(first, *unsortedSolution))
	return false;
      findOrderSortedUnifiers();
      first = false;
    }
  while (orderSortedUnifiers == 0);
  orderSortedUnifiers->nextAssignment();  // can't fail
 good:
  bindFreeVariables();
  return true;
}

void
UnificationProblem::bindFreeVariables()
{
  //
  //	We go through the variables that were free in the unsorted unifier.
  //
  const Vector<Byte>& assignment = orderSortedUnifiers->getCurrentAssignment();
  int bddVar = sortBdds->getFirstAvailableVariable();

  FOR_EACH_CONST(i, Vector<int>, freeVariables)
    {
      DagNode* variable = sortedSolution->value(*i);
      DebugAdvisory("findNextUnifier(): finding sort of free variable " << variable);

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
      DebugAdvisory("index = " << index);
      //
      //	Replace each variable symbol in a free variable with the
      //	variable symbol corresponding to its newly calculated sort.
      //
      Assert(index < component->nrSorts(), "bad sort index " << index << " computed for free variable" << variable);
      variable->replaceSymbol(freshVariableGenerator->getBaseVariableSymbol(component->sort(index)));
    }
}

void
UnificationProblem::findOrderSortedUnifiers()
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
  freeVariables.clear();
  int nrActualVariables = sortedSolution->nrFragileBindings();
  Vector<int> realToBdd(nrActualVariables);
  int nextBddVariable = sortBdds->getFirstAvailableVariable();
  int nrOriginalVariables = variableInfo.getNrRealVariables();
  for (int i = 0; i < nrActualVariables; ++i)
    {
      //cout << "variable with index " << i << endl;
      if (sortedSolution->value(i) == 0)
	{
	  DebugAdvisory("allocated BDD variables starting at " << nextBddVariable << " for variable with slot " << i);
	  freeVariables.append(i);
	  realToBdd[i] = nextBddVariable;
	  Sort* sort = (i < nrOriginalVariables) ?
	    safeCast(VariableSymbol*, variableInfo.index2Variable(i)->symbol())->getSort() :
	    unsortedSolution->getFreshVariableSort(i);
	  nextBddVariable += sortBdds->getNrVariables(sort->component()->getIndexWithinModule());
	  //cout << "allocated bdds" << endl;
	}
      else
	DebugAdvisory("variable with index " << i << " bound to " << sortedSolution->value(i));
    }
  //cout << "allocated " << nextBddVariable << " BDD variables" << endl;
  //
  //	Make sure BDD package has enough variables allocated.
  //
  DebugAdvisory("setting " << nextBddVariable << " BDD variables");
  BddUser::setNrVariables(nextBddVariable);
  //
  //	Constrain free fresh variables to have indices in valid range.
  //
  Bdd unifier = bddtrue;
  for (int i = nrOriginalVariables; i < nrActualVariables; ++i)
    {
      if (sortedSolution->value(i) == 0)
	{
	  Sort* sort = unsortedSolution->getFreshVariableSort(i);
	  Bdd leqRelation = sortBdds->getRemappedLeqRelation(sort, realToBdd[i]);
	  unifier = bdd_and(unifier, leqRelation);
	  DebugAdvisory("Adding constraint for free, non-original variable: " << leqRelation <<
			" unifier becomes " << unifier);
	}
    }
  //
  //	Now compute a BDD which tells us if a given assignment of sorts to free
  //	variables yields an order-sorted unifier.
  //
  //  Bdd unifier = bddtrue;
  for (int i = 0; i < nrOriginalVariables; ++i)
    {
      DebugAdvisory("Considering variable " << variableInfo.index2Variable(i));
      Sort* sort = safeCast(VariableSymbol*, variableInfo.index2Variable(i)->symbol())->getSort();
      Bdd leqRelation;
      DagNode* d = sortedSolution->value(i);
      if (d != 0)
	{
	  //
	  //	Bound variable: term must have sort <= variables sort.
	  //
	  Vector<Bdd> genSort;
	  //d->computeGeneralizedSort(*sortBdds, realToBdd, genSort);
	  d->computeGeneralizedSort2(*sortBdds, realToBdd, genSort);
	  leqRelation = sortBdds->applyLeqRelation(sort, genSort);
	  DebugAdvisory("bound to " << d << " induces leqRelation " << leqRelation);
	}
      else
	{
	  //
	  //	Free variable: sort assigned to free variable must be <= variables original sort.
	  //
	  leqRelation = sortBdds->getRemappedLeqRelation(sort, realToBdd[i]);
	  DebugAdvisory("free variable induces leqRelation " << leqRelation);
	}
      unifier = bdd_and(unifier, leqRelation);
      DebugAdvisory("findOrderSortedUnifiers(): unifier = " << unifier);
      if (unifier == bddfalse)
	return;
    }
  //
  //	Now compute a BDD that tells us if a unifier has maximal sort assignments to free
  //	variables.
  //
  //	maximal(X1,...,  Xn) = unifier(X1,..., Xn) &
  //	  for each i in 1 ... n
  //	    not(exists Y .[gt(Y,Xi) & unifier(X1,..., Y,...., Xn)])
  //
  //	We actually evaluate this subterm as
  //	  forall Y .[not(gt(Y,Xi) & unifier(X1,..., Y,...., Xn))]
  //	pushing the not inside the quantifier.
  //
  Bdd maximal = unifier;
  int nrFreeVariables = freeVariables.size();
  int secondBase = sortBdds->getFirstAvailableVariable();
  for (int i = 0; i < nrFreeVariables; ++i)
    {
      int fv = freeVariables[i];
      Sort* sort = (fv < nrOriginalVariables) ?
	safeCast(VariableSymbol*, variableInfo.index2Variable(fv)->symbol())->getSort() :
	unsortedSolution->getFreshVariableSort(fv);
      int nrBddVariables = sortBdds->getNrVariables(sort->component()->getIndexWithinModule());
      //
      //	Replace the bdd variables allocated to the real variable in unifier with 0...nrBddVariables-1.
      //	Replace the bdd variables firstAvail,..., firstAvail+nrBddVariables-1 in gtOp with the
      //	variables allocated to real variable.
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
  DebugAdvisory("findOrderSortedUnifiers(): maximal = " << maximal);
  orderSortedUnifiers = new AllSat(maximal, secondBase, nextBddVariable - 1);

  if (nrFreeVariables > 0)
    {
      int freshVariableCount = 0;
      //
      //	Finally we bind each free variable to a fresh variable of the appropriate sort and
      //	instantiate all bindings to replace all occurences of the (formerly) free variables.
      //
      for (int i = 0; i < nrActualVariables; ++i)
	{
	  if (sortedSolution->value(i) == 0)
	    {
	      Sort* sort = (i < nrOriginalVariables) ?
		safeCast(VariableSymbol*, variableInfo.index2Variable(i)->symbol())->getSort() :
		unsortedSolution->getFreshVariableSort(i);
	      sortedSolution->bind(i, new VariableDagNode(freshVariableGenerator->getBaseVariableSymbol(sort),
					  freshVariableGenerator->getFreshVariableName(freshVariableCount),
					  i));
	      ++freshVariableCount;
	    }
	}
      for (int i = 0; i < nrOriginalVariables; ++i)
	{
	  if (DagNode* d = sortedSolution->value(i)->instantiate(*sortedSolution))
	    sortedSolution->bind(i, d);
	}
    }
}
