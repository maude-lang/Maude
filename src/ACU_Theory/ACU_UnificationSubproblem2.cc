/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2008 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class ACU_Subproblem.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"
#include "intSystem.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "ACU_Persistent.hh"
#include "ACU_Theory.hh"

//      interface class definitions
#include "argVec.hh"
#include "associativeSymbol.hh"
#include "dagNode.hh"
#include "subproblem.hh"

//      core class definitions
#include "variableInfo.hh"
#include "unificationContext.hh"
#include "localBinding.hh"

//	variable class definitions
#include "variableSymbol.hh"
#include "variableDagNode.hh"

//	ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_UnificationSubproblem2.hh"

ACU_UnificationSubproblem2::ACU_UnificationSubproblem2(ACU_Symbol* topSymbol)
  : topSymbol(topSymbol),
    savedSubstitution(0),
    preSolveSubstitution(0)
{
#ifndef NO_ASSERT
  DebugAdvisory("Created ACU_UnificationSubproblem2() base " << ((void*) this) <<
		" for topSymbol " << topSymbol);
#endif
  maximalSelections = 0;
}

ACU_UnificationSubproblem2::~ACU_UnificationSubproblem2()
{
  delete maximalSelections;
}

void
ACU_UnificationSubproblem2::markReachableNodes()
{
  //  cout << this << endl;
  int nrFragile = savedSubstitution.nrFragileBindings();
  //  cout << "nrFragile = " << nrFragile << endl;
  for (int i = 0; i < nrFragile; i++)
    {
      //      cout << i << endl;
      DagNode* d = savedSubstitution.value(i);
      if (d != 0)
	{
	  //	  cout << d << endl;
	  d->mark();
	}
    }
  //
  //	No need to mark preSolveSubstitution since it's dags are a subset of those in
  //	savedSubstitution.
  //
}

void
ACU_UnificationSubproblem2::addUnification(DagNode* lhs, DagNode* rhs, bool marked, UnificationContext& solution)
{
  Assert(lhs->symbol() == topSymbol, "bad lhs dag " << lhs);
  Assert(topSymbol->hasIdentity() ||
	 rhs->symbol() == topSymbol ||
	 dynamic_cast<VariableDagNode*>(rhs) != 0, "bad rhs for for non-collapse theory " << rhs);
  Assert(topSymbol->hasIdentity() || !marked, "bad mark for non-collapse theory");
  {
    int nrSubterms = subterms.size();
    for (int i = 0; i < nrSubterms; ++i)
      multiplicities[i] = 0;
  }
  if (rhs->symbol() != topSymbol)
    {
      //
      //	Must be of the form f(...) =? X
      //	or a theory clash of the form f(...) =? g(...)
      //
      Term* identity = topSymbol->getIdentity();
      if (identity != 0 && identity->equal(rhs))
	;  // rhs disappears in our theory
      else
	{
	  int subtermIndex = setMultiplicity(rhs, -1, solution);
	  if (marked)
	    markedSubterms.insert(subtermIndex);  // cannot be assigned multiple things
	}
    }
  else
    {
      ArgVec<ACU_DagNode::Pair> rhsArgs = safeCast(ACU_DagNode*, rhs)->argArray;
      FOR_EACH_CONST(i, ArgVec<ACU_DagNode::Pair>, rhsArgs)
	setMultiplicity(i->dagNode, - i->multiplicity, solution);
    }

  {
    ArgVec<ACU_DagNode::Pair> lhsArgs = safeCast(ACU_DagNode*, lhs)->argArray;
    FOR_EACH_CONST(i, ArgVec<ACU_DagNode::Pair>, lhsArgs)
      setMultiplicity(i->dagNode, i->multiplicity, solution);
  }
  //
  //	Check to see if everything cancelled.
  //
  FOR_EACH_CONST(i, Vector<int>, multiplicities)
    {
      if (*i != 0)
	{
	  unifications.push_back(multiplicities);
	  return;
	}
    }
  //
  //	No non-zero entries so we can ignore unification problem.
  //
}

int
ACU_UnificationSubproblem2::setMultiplicity(DagNode* dagNode, int multiplicity, UnificationContext& solution)
{
  //
  //	First we replace a variable with its current representative. Really we should also
  //	do this for variables within aliens as well.
  //
  if (VariableDagNode* varDagNode = dynamic_cast<VariableDagNode*>(dagNode))
     dagNode = varDagNode->lastVariableInChain(solution);
  //
  //	Now look for dag in list of nominally abstracted dags.
  //
  int nrSubterms = subterms.size();
  for (int i = 0; i < nrSubterms; ++i)
    {
      if (dagNode->equal(subterms[i]))
	{
	  multiplicities[i] += multiplicity;
	  return i;
	}
    }
  //
  //	Not found so make a new entry.
  //
  subterms.append(dagNode);
  multiplicities.append(multiplicity);
  return nrSubterms;
}

void
ACU_UnificationSubproblem2::unsolve(int index, UnificationContext& solution)
{
  //
  //	We take a solved form X = f(...), turn it into a Diophantine equation and
  //	remove it from the current solution.
  //
  DagNode* variable = solution.getVariableDagNode(index);
  DagNode* value = solution.value(index);
  solution.bind(index, 0);

  Assert(variable != 0, "couldn't get variable for index " << index << " bound to " << value);
  DebugAdvisory("### unsolving " << variable << " <- " << value);

  {
    int nrSubterms = subterms.size();
    for (int i = 0; i < nrSubterms; ++i)
      multiplicities[i] = 0;
  }
  ArgVec<ACU_DagNode::Pair> args = safeCast(ACU_DagNode*, value)->argArray;
  FOR_EACH_CONST(i, ArgVec<ACU_DagNode::Pair>, args)
    setMultiplicity(i->dagNode, i->multiplicity, solution);
  setMultiplicity(variable, -1, solution);
  unifications.push_back(multiplicities);
}

bool
ACU_UnificationSubproblem2::solve(bool findFirst, UnificationContext& solution, PendingUnificationStack& pending)
{
  if (subterms.empty())
    return findFirst;  // degenerate case where all unification problems cancelled
  if (findFirst)
    {
      //
      //	Save the current substitution
      //
      preSolveSubstitution.clone(solution);
      //
      //	Unsolve any solved forms that are in our theory. This seeming wasteful step
      //	has to be done in order to avoid nontermination.
      //
      int nrFragile = solution.nrFragileBindings();
      for (int i = 0; i < nrFragile; ++i)
	{
	  DagNode* value = solution.value(i);
	  if (value != 0 && value->symbol() == topSymbol)
	    unsolve(i, solution);
	}

      if (!buildAndSolveDiophantineSystem(solution))
	{
	  //
	  //	Restore current solution and fail.
	  //
	  solution.clone(preSolveSubstitution);
	  return false;
	}
      if (topSymbol->hasIdentity())
	{
	  bdd legal = computeLegalSelections();
	  DebugAdvisory("legal = " << legal <<
			" node count = " << bdd_nodecount(legal) <<
			" path count = " << bdd_pathcount(legal));
	  int nrBasisElements = basis.size();
	  bdd maximal = legal;
	  if (topSymbol->hasUnequalLeftIdentityCollapse())
	    {
	      //
	      //	Can't ignore non-maximal solutions because
	      //	(1) Maximal solution may not have a sorting while a non-maximal solution may have.
	      //	(2) The sorting of a maximal solution may exclude a non-maximal solution it was covering.
	      //
	    }
	  else
	    {
	      //
	      //	Assume that each variable introduced for a Diophantine basis element will
	      //	be able to disappear by taking the basis element. Thus we only want maximal
	      //	vectors from legal.
	      //
	      for (int i = 0; i < nrBasisElements; ++i)
		{
		  //
		  //	bigger = not(ith var) /\ (legal restricted to ith var true)
		  //
		  //	i.e. bigger represents those vectors where the ith variable is false and we get
		  //	a legal solution by making it true. Clearly the original vector was not maximal,
		  //	even if it was a legal solution.
		  //
		  //	We need to eliminate these vectors if we want maximal solutions, so we compute the
		  //	complement of this set of vectors and conjunct them in.
		  //	
		  bdd notBigger = bdd_or(bdd_ithvar(i), bdd_not(bdd_restrict(legal, bdd_ithvar(i))));
		  maximal = bdd_and(maximal, notBigger);
		}
	    }
	  DebugAdvisory("maximal = " << maximal <<
			" node count = " << bdd_nodecount(maximal) <<
			" path count = " << bdd_pathcount(maximal));
	  maximalSelections = new AllSat(maximal, 0, nrBasisElements - 1);
	}
      //
      //	Save state of the pending stack with the substitution so that
      //	we can restore both in order to undo each of our solutions.
      //
      savedSubstitution.clone(solution);
      savedPendingState = pending.checkPoint();
    }
  else
    {
      //
      //	Restore pending stack and current substitution, implicitly deallocating
      //	any fresh variables we introduced.
      //
      pending.restore(savedPendingState);
      solution.clone(savedSubstitution);
    }
  while (topSymbol->hasIdentity() ? nextSelectionWithIdentity(findFirst) : nextSelection(findFirst))
    {
      findFirst = false;

      if (buildSolution(solution, pending))
	return true;
      //
      //	Restore pending stack and current substitution, implicitly deallocating
      //	any fresh variables we introduced.
      //
      pending.restore(savedPendingState);
      solution.clone(savedSubstitution);
    }
  solution.clone(preSolveSubstitution);
  return false;
}

void
ACU_UnificationSubproblem2::classify(int subtermIndex,
				     UnificationContext& solution,
				     bool& canTakeIdentity,
				     int& upperBound,
				     Symbol*& stableSymbol)
{
  //
  //	For a subterm that we have nominally abstraction by a Diophantine variable we determine:
  //	(1) Is is possible that the subject can be unified against identity.
  //	(2) Can we determine an upperbound on how many other subjects could be unified against it.
  //	(3) Can we determine what it will only unify against something with a specific stable top symbol.
  //
  Term* identity = topSymbol->getIdentity();
  //
  //	Default determinations:
  //
  canTakeIdentity = (identity != 0);  // can unify with identity if it exists
  upperBound = markedSubterms.contains(subtermIndex) ? 1 : UNBOUNDED;  // marked subterms are bounded by 1
  stableSymbol = 0;  // no stable symbol
  //
  //	Now we try to tighten these restrictions.
  //
  DagNode* subject = subterms[subtermIndex];
  if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(subject))
    {
      //
      //	If subterm is a variable we may be able to constrain it based on its sort.
      //
      VariableSymbol* variableSymbol = safeCast(VariableSymbol*, v->symbol());
      Sort* variableSort = variableSymbol->getSort();
      int variableSortBound = topSymbol->sortBound(variableSort);
      if (variableSortBound < upperBound)
	upperBound = variableSortBound;
      canTakeIdentity = canTakeIdentity && topSymbol->takeIdentity(variableSort);
      subject = solution.value(v->getIndex());
      if (subject == 0)
	return;  // unbound variable
      Assert(subject->symbol() != topSymbol, "variable " << (DagNode*) v <<
	     " still bound into our theory " << subject << " even after unsolving phase");
    }
  //
  //	Now we have a nonvariable, or a variable bound to a nonvariable.
  //	We look to see if the top symbol is stable.
  //
  Symbol* symbol = subject->symbol();
  DebugAdvisory("ACU_UnificationSubproblem2::classify() symbol = " << symbol <<
	        " symbol->isStable() = " << symbol->isStable());
  if (symbol->isStable())
    {
      //
      //	Anything that unifies with subject must have symbol on top.
      //
      upperBound = 1;  // stable symbol can unify with at most one thing
      canTakeIdentity = canTakeIdentity && (symbol == identity->symbol());
      stableSymbol = symbol;
    }
}

bool
ACU_UnificationSubproblem2::buildAndSolveDiophantineSystem(UnificationContext& solution)
{
#ifndef NO_ASSERT
  DebugAdvisory("building DiophantineSystem for ACU_UnificationSubproblem2 " << ((void*) this));
  if (globalAdvisoryFlag)
    {
      for (int i = 0; i < subterms.length(); ++i)
	cerr << subterms[i] << '\t';
      cerr << endl;
    }
#endif
  //
  //	Each distinct alien subdag from a unification problem that didn't get cancelled
  //	is represented by a Diophantine variable.
  //
  int nrDioVars = subterms.size();
  Assert(nrDioVars > 0, "shouldn't be called in degenerate case");
  //if (nrDioVars == 1)
  //  return false;  // trivial failure
  //
  //	Create the Diophantine system.
  //
  IntSystem system(nrDioVars);
  FOR_EACH_CONST(i, list<Vector<int> >, unifications)
    system.insertEqn(*i);
  //
  //	Compute an upperbound on the assignment to each Diophantine variable.
  //
  upperBounds.resize(nrDioVars);  // for basis selection use
  IntSystem::IntVec upperBnds(nrDioVars);  // for Diophantine system use
  Vector<Symbol*> stableSymbols(nrDioVars);   // if we know we can only unify against a stable alien
  for (int i = 0; i < nrDioVars; ++i)
    {
      bool canTakeIdentity;
      int upperBound;
      classify(i, solution, canTakeIdentity, upperBound, stableSymbols[i]);
      DebugAdvisory("ACU_UnificationSubproblem2::buildAndSolveDiophantineSystem() i = " << i <<
		    " subterms[i] = " << subterms[i] <<
		    " canTakeIdentity = " << canTakeIdentity <<
		    " upperBound = " << upperBound);
		    
      if (!canTakeIdentity)
	needToCover.insert(i);  // can't take identity so mark as uncovered and be sure to cover
      upperBounds[i] = upperBound;
      upperBnds[i] = upperBound;
    }
  system.setUpperBounds(upperBnds);
  //
  //	Extract the basis.
  //
  Vector<int> dioSol;
  for (int index = 0; system.findNextMinimalSolution(dioSol);)
    {
#ifndef NO_ASSERT
      DebugAdvisory("added basis element for ACU_UnificationSubproblem2 " << ((void*) this));
      if (globalAdvisoryFlag)
	{
	  for (int i = 0; i < dioSol.length(); ++i)
	    cerr << dioSol[i] << '\t';
	  cerr << endl;
	}
#endif
      Symbol* existingStableSymbol = 0;
      for (int i = 0; i < nrDioVars; ++i)
	{
	  int t = dioSol[i];
	  if (t != 0)
	    {
	      Symbol* stableSymbol = stableSymbols[i];
	      if (stableSymbol != 0)
		{
		  //
		  //	We have a basis element that is going to force a term with a stable symbol to
		  //	unify with something.
		  //
		  if (existingStableSymbol == 0)
		    existingStableSymbol = stableSymbol;
		  else if (existingStableSymbol != stableSymbol)
		    {
		      //
		      //	Forced unification is guaranteed to fail.
		      //
		      DebugAdvisory("killing basis element, " << existingStableSymbol << " vs " << stableSymbol);
		      goto killElement;
		    }
		}
	    }
	}
      {
	basis.push_front(Entry());
	Entry& e = basis.front();
	e.remainder = accumulator;  // deep copy
	e.element.resize(nrDioVars);
	for (int i = 0; i < nrDioVars; ++i)
	  {
	    if ((e.element[i] = dioSol[i]) != 0)
	      accumulator.insert(i);  // subterm i is covered
	  }
	e.index = index;
	++index;
      }
    killElement:
      ;
    }
  //
  //	Check that each term that needs to be covered is covered by at least one basis element.
  //
  if (!accumulator.contains(needToCover))
    return false;
  //
  //	Initialize totals vector and uncovered set.
  //
  totals.resize(nrDioVars);
  for (int i = 0; i < nrDioVars; ++i)
    totals[i] = 0;
  uncovered = needToCover;
  return true;
}

bool
ACU_UnificationSubproblem2::nextSelection(bool findFirst)
{
  int nrSubterms = subterms.size();
  if (findFirst)
    { 
      current = basis.begin();
    forward:
      //
      //	We keep adding basis elements to the selection as long as they don't violate
      //	an upper bound. When we do hit a violation, if we find that a covering is
      //	not possible without this element we backtrack.
      //
      for (; current != basis.end(); ++current)
	{
	  if (includable(current))
	    {
	      for (int i = 0; i < nrSubterms; ++i)
		{
		  if (int v = current->element[i])
		    {
		      totals[i] += v;
		      uncovered.subtract(i);
		    }
		}
	      selection.append(current);
	    }
	  else
	    {
	      if (!(current->remainder.contains(uncovered)))
		goto backtrack;
	    }
	}
      Assert(uncovered.empty(), "failed to cover");
      return true;
    }
  else
    {
    backtrack:
      //
      //	We backtrack by removing basis elements from the current selection. Each time
      //	we remove an element, if we can still get a covering with later elements we
      //	start forward again.
      //
      Assert(selection.size() > 0, "backtracking over empty selection");
      for (int i = selection.size() - 1; i >= 0; --i)
	{
	  current = selection[i];
	  for (int j = 0; j < nrSubterms; ++j)
	    {
	      if ((totals[j] -= current->element[j]) == 0)
		uncovered.insert(j);
	    }
	  if (current->remainder.contains(uncovered))
	    {
	      ++current;
	      selection.resize(i);
	      goto forward;
	    }
	}
    }
  return false;
}

bool
ACU_UnificationSubproblem2::nextSelectionWithIdentity(bool /* findFirst */)
{
  if (maximalSelections->nextAssignment())
    {
      const Vector<Byte>& assignment = maximalSelections->getCurrentAssignment();
      selection.clear();
      FOR_EACH_CONST(i, Basis, basis)
	{
	  if (assignment[i->index])
	    selection.append(i);
	}
      return true;
    }
  return false;
}

bool
ACU_UnificationSubproblem2::includable(Basis::const_iterator potential)
{
  int nrSubterms = subterms.size();
  for (int i = 0; i < nrSubterms; ++i)
    {
      if (totals[i] + potential->element[i] > upperBounds[i])
	return false;
    }
  return true;
}

bdd
ACU_UnificationSubproblem2::computeLegalSelections()
{
  int nrBasisElements = basis.size();
  BddUser::setNrVariables(nrBasisElements);
  bdd conjunction = bddtrue;
  Vector<bdd> bounds;

  int nrSubterms = subterms.size();
  for (int i = 0; i < nrSubterms; ++i)
    {
      int upperBound = upperBounds[i];
      if (upperBound != UNBOUNDED)
	{
	  //
	  //	Compute a BDD that ensures the selection of basis elements
	  //	keeps the assignment to the subterm at or below upper bound.
	  //
	  bounds.resize(upperBound + 1);
	  for (int j = 0; j <= upperBound; ++j)
	    bounds[j] = bddtrue;
	  FOR_EACH_CONST(k, Basis, basis)
	    {
	      int value = k->element[i];
	      if (value != 0)
		{
		  for (int j = upperBound; j >= 0; --j)
		    {
		      if (j - value >= 0)
			bounds[j] = bdd_ite(bdd_ithvar(k->index), bounds[j - value], bounds[j]);
		      else
			bounds[j] = bdd_ite(bdd_ithvar(k->index), bddfalse, bounds[j]);
		    }
		}
	    }
	  DebugAdvisory("upperbound " << i <<
			" bound = " << bounds[upperBound] <<
			" node count = " << bdd_nodecount(bounds[upperBound]) <<
			" path count = " << bdd_pathcount(bounds[upperBound]));
	  conjunction = bdd_and(conjunction, bounds[upperBound]);
	}
      if (needToCover.contains(i))
	{
	  //
	  //	Compute a BDD that ensures the selection of basis elements
	  //	assigns at least one thing to the subterm.
	  //
	  bdd disjunction = bddfalse;
	  FOR_EACH_CONST(k, Basis, basis)
	    {
	      if (k->element[i] != 0)
		disjunction = bdd_or(disjunction, bdd_ithvar(k->index));
	    }
	  DebugAdvisory("need to cover " << i <<
			" disjunction = " << disjunction <<
			" node count = " << bdd_nodecount(disjunction) <<
			" path count = " << bdd_pathcount(disjunction));
	  conjunction = bdd_and(conjunction, disjunction);
	}
    }
  return conjunction;
}

bool
ACU_UnificationSubproblem2::buildSolution(UnificationContext& solution, PendingUnificationStack& pending)
{
#ifndef NO_ASSERT
  DebugAdvisory("buildSolution() using basis elements:");
  if (globalAdvisoryFlag)
    {
      for (int j = 0; j < selection.size(); ++j)
	{
	  for (int i = 0; i < subterms.size(); ++i)
	    cerr << selection[j]->element[i] << '\t';
	  cerr << endl;
	}
    }
#endif
  //
  //	First we allocate a fresh variable for each basis element selected.
  //
  ConnectedComponent* component = topSymbol->rangeComponent();
  int selectionSize = selection.size();
  Vector<DagNode*> freshVariables(selectionSize);
  for (int i = 0; i < selectionSize; ++i)
    freshVariables[i] = solution.makeFreshVariable(component);
  //
  //	Now for each abstracted subterm we compute a solution in
  //	the purified problem.
  //
  int nrSubterms = subterms.size();
  for (int i = 0; i < nrSubterms; ++i)
    {
      int nrElements = 0;
      int lastElement = NONE;
      for (int j = 0; j < selectionSize; ++j)
	{
	  if (selection[j]->element[i] > 0)
	    {
	      ++nrElements;
	      lastElement = j;
	    }
	}
      Assert(nrElements <= upperBounds[i], "have at least " << nrElements <<
	     " assign variables vs upper bound of " << upperBounds[i] <<
	     " for subterm " << subterms[i]);
      DagNode* d;
      if (nrElements == 0)
	{
	  d = topSymbol->getIdentityDag();
	  //
	  //	If this is the first time we use the identity element it is possible
	  //	that it will not have its sort computed or ground flag set.
	  //
	  if (!(d->isGround()))
	    d->computeBaseSortForGroundSubterms();
	}
      else if (nrElements == 1 && selection[lastElement]->element[i] == 1)
	d = freshVariables[lastElement];
      else
	{
	  ACU_DagNode* a = new ACU_DagNode(topSymbol, nrElements);
	  int pos = 0;
	  for (int j = 0; j < selectionSize; ++j)
	    {
	      int m = selection[j]->element[i];
	      if (m > 0)
		{
		  a->argArray[pos].dagNode = freshVariables[j];
		  a->argArray[pos].multiplicity = m;
		  ++pos;
		}
	    }
	  //
	  //	There is no guarantee that the fresh variables that we generate are
	  //	is the same order as needed for AC normal form - so we call this
	  //	private member function to fix this without too much overhead.
	  //
	  a->sortAndUniquize();	  
	  Assert(a->isTree() == false, "Oops we got a tree! " << a);
	  d = a;
	}

      DagNode* subterm = subterms[i];
      if (VariableDagNode* varSubterm = dynamic_cast<VariableDagNode*>(subterm))
	{
	  //
	  //	We need to handle unbound variable subterms ourself to avoid
	  //	generating another problem in our theory.
	  //
	  VariableDagNode* repVar = varSubterm->lastVariableInChain(solution);
	  if (solution.value(repVar->getIndex()) == 0)
	    {
	      solution.unificationBind(repVar, d);
	      continue;
	    }
	}
      if (!(subterms[i]->computeSolvedForm(d, solution, pending)))
	return false;
    }
  return true;
}
