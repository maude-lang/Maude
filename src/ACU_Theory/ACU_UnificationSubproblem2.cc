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
}


ACU_UnificationSubproblem2::~ACU_UnificationSubproblem2()
{
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
ACU_UnificationSubproblem2::addUnification(DagNode* lhs, DagNode* rhs)
{
  Assert(lhs->symbol() == topSymbol, "bad lhs dag " << lhs);
  Assert(rhs->symbol() == topSymbol, "bad rhs dag " << rhs);

  ArgVec<ACU_DagNode::Pair> lhsArgs = safeCast(ACU_DagNode*, lhs)->argArray;
  ArgVec<ACU_DagNode::Pair> rhsArgs = safeCast(ACU_DagNode*, rhs)->argArray;
  int nrLhsArgs = lhsArgs.length();
  int nrRhsArgs = rhsArgs.length();
  int i = 0;
  int j = 0;
  multiplicities.clear();
  for (;;)
    {
      int r;
      if (i == nrLhsArgs)
	{
	  if (j == nrRhsArgs)
	    break;
	  r = 1;
	}
      else
	r = (j == nrRhsArgs) ? -1 : lhsArgs[i].dagNode->compare(rhsArgs[j].dagNode);
      
      if (r < 0)
	{
	  setMultiplicity(lhsArgs[i].dagNode, lhsArgs[i].multiplicity);
	  ++i;
	}
      else if (r > 0)
	{
	  setMultiplicity(rhsArgs[j].dagNode, - rhsArgs[j].multiplicity);
	  ++j;
	}
      else
	{
	  int diff = lhsArgs[i].multiplicity - rhsArgs[j].multiplicity;
	  if (diff != 0)
	    setMultiplicity(lhsArgs[i].dagNode, diff);
	  ++i;
	  ++j;
	}      
    }
  if (!multiplicities.empty())
    unifications.push_back(multiplicities);
}

void
ACU_UnificationSubproblem2::setMultiplicity(DagNode* dagNode, int multiplicity)
{
  int nrSubterms = subterms.size();
  int i = 0;
  for (;; ++i)
    {
      if (i == nrSubterms)
	{
	  subterms.append(dagNode);
	  break;
	}
      if (dagNode->equal(subterms[i]))
	break;
    }
  int nrMultiplicities = multiplicities.size();
  if (i >= nrMultiplicities)
    {
      multiplicities.resize(i + 1);
      for (int j = nrMultiplicities; j < i; ++j)
	multiplicities[j] = 0;
    }
  multiplicities[i] = multiplicity;
}

bool
ACU_UnificationSubproblem2::unsolve(int index, UnificationContext& solution)
{
  //
  //	We take a solved form X = f(...), turn it into a Diophantine equation and
  //	remove it from the current solution.
  //
  DagNode* variable = solution.getVariableDagNode(index);
  DagNode* value = solution.value(index);
  Assert(variable != 0, "couldn't get variable for index " << index << " bound to " << value);
  DebugAdvisory("### unsolving " << variable << " <- " << value);

  multiplicities.clear();
  ArgVec<ACU_DagNode::Pair> args = safeCast(ACU_DagNode*, value)->argArray;
  FOR_EACH_CONST(i, ArgVec<ACU_DagNode::Pair>, args)
    {
      DagNode* d = i->dagNode;
      if (variable->equal(d))
	return false;  // occur check fail
      setMultiplicity(d, i->multiplicity);
    }
  setMultiplicity(variable, -1);
  unifications.push_back(multiplicities);
  solution.bind(index, 0);
  return true;
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
	    {
	      if (!unsolve(i, solution))
		{
		  solution.clone(preSolveSubstitution);
		  return false;
		}
	    }
	}

      if (!buildAndSolveDiophantineSystem(solution))
	{
	  solution.clone(preSolveSubstitution);
	  return false;
	}
      //
      //	Save state of the pending stack so that it with the substitution
      //	we can restore  in order to undo each of our solutions.
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
  while (nextSelection(findFirst))
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

bool
ACU_UnificationSubproblem2::buildAndSolveDiophantineSystem(UnificationContext& solution)
{
#ifndef NO_ASSERT
  DebugAdvisory("building DiophantineSystem for ACU_UnificationSubproblem2 " << ((void*) this));
  /*
  for (int i = 0; i < subterms.length(); ++i)
  cerr << subterms[i] << '\t';
  cerr << endl;
  */
#endif
  //
  //	Each distinct alien subdag from a unification problem that didn't get cancelled
  //	is represented by a Diophantine variable.
  //
  int nrDioVars = subterms.size();
  Assert(nrDioVars > 0, "shouldn't be called in degenerate case");
  if (nrDioVars == 1)
    return false;  // trivial failure
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
  for (int i = 0; i < nrDioVars; ++i)
    {
      int bound = 1;  // for aliens and variables bound to aliens
      DagNode* d = subterms[i];
      if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(d))
	{
	  DagNode* b = solution.value(v->lastVariableInChain(solution)->getIndex());
	  if (b == 0 || b->symbol() == topSymbol)  // unbound or bound to a dag with our top symbol
	    bound = topSymbol->sortBound(static_cast<VariableSymbol*>(v->symbol())->getSort());
	}
      upperBounds[i] = bound;
      upperBnds[i] = bound;
    }
  system.setUpperBounds(upperBnds);
  //
  //	Extract the basis.
  //
  Vector<int> dioSol;
  while (system.findNextMinimalSolution(dioSol))
    {
#ifndef NO_ASSERT
      DebugAdvisory("added basis element for ACU_UnificationSubproblem2 " << ((void*) this));
      /*
      for (int i = 0; i < dioSol.length(); ++i)
	cerr << dioSol[i] << '\t';
      cerr << endl;
      */
#endif
      basis.push_front(Entry());
      Entry& e = basis.front();
      e.remainder = accumulator;  // deep copy
      e.element.resize(nrDioVars);
      for (int i = 0; i < nrDioVars; ++i)
	{
	  if ((e.element[i] = dioSol[i]) != 0)
	    accumulator.insert(i);  // subterm i is covered
	}
    }
  //
  //	Check that all the subterms are covered by at least one basis element and initialize
  //	totals vector and uncovered set.
  //
  totals.resize(nrDioVars);
  for (int i = 0; i < nrDioVars; ++i)
    {
      if (!(accumulator.contains(i)))
	return false;
      totals[i] = 0;
      uncovered.insert(i);
    }
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

bool
ACU_UnificationSubproblem2::buildSolution(UnificationContext& solution, PendingUnificationStack& pending)
{
  //
  //	First we allocate a fresh variable for each basis element selected.
  //
  ConnectedComponent* component = topSymbol->rangeComponent();
  int selectionSize = selection.size();
  Vector<DagNode*> freshVariables(selectionSize);
  for (int i = 0; i < selectionSize; ++i)
    freshVariables[i] = solution.makeFreshVariable(component);
  //
  //	Now for each abstracted subterm we unify it against its solution in
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
      Assert(nrElements > 0, "empty solution");
      DagNode* d;
      if (nrElements == 1 && selection[lastElement]->element[i] == 1)
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
      if (!(subterms[i]->computeSolvedForm(d, solution, pending)))
	return false;
    }
  return true;
}
