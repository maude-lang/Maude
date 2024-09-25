/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2021 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class ACU_UnificationSubproblem2.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "AU_Persistent.hh"
#include "AU_Theory.hh"

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

//	AU theory class definitions
#include "AU_Symbol.hh"
#include "AU_DagNode.hh"
#include "AU_UnificationSubproblem2.hh"

AU_UnificationSubproblem2::AU_UnificationSubproblem2(AU_Symbol* topSymbol)
  : topSymbol(topSymbol),
    preSolveSubstitution(0),
    savedSubstitution(0)
{
  wordSystem = 0;
}

AU_UnificationSubproblem2::~AU_UnificationSubproblem2()
{
  delete wordSystem;
}

void
AU_UnificationSubproblem2::markReachableNodes()
{
  //
  //	Protect dags in preSolveSubstitution.
  //
  int nrFragile = preSolveSubstitution.nrFragileBindings();
  for (int i = 0; i < nrFragile; i++)
    {
      DagNode* d = preSolveSubstitution.value(i);
      if (d != 0)
	d->mark();
    }
  //
  //	No need to mark savedSubstitution since its dags are a subset of those in
  //	preSolveSubstitution; we get from preSolveSubstitution to savedSubstitution by
  //	unsolving bindings.
  //
}

void
AU_UnificationSubproblem2::addUnification(DagNode* lhs,
					  DagNode* rhs,
					  bool marked,
					  UnificationContext& solution)
{
  DebugEnter(lhs << " =? " << rhs << "  marked = " << marked);
  Assert(lhs->symbol() == topSymbol, "bad lhs dag " << lhs);
  Assert(topSymbol->hasIdentity() ||
	 rhs->symbol() == topSymbol ||
	 dynamic_cast<VariableDagNode*>(rhs) != 0, "bad rhs for for non-collapse theory " << rhs);
  Assert(topSymbol->hasIdentity() || !marked, "bad mark for non-collapse theory");
  //
  //	Store unification problem as an abstract equation or assignment.
  //
  WordSystem::Word lhsAbstract;
  assocToAbstract(lhs, lhsAbstract, solution);
  if (rhs->symbol() == topSymbol)
    {
      WordSystem::Word rhsAbstract;
      assocToAbstract(rhs, rhsAbstract, solution);
      if (lhsAbstract.empty())
	{
	  if (rhsAbstract.empty())
	    ;  // ignore unification
	  else
	    nullEquations.push_back(rhsAbstract);
	}
      else
	{
	  if (rhsAbstract.empty())
	    nullEquations.push_back(lhsAbstract);
	  else
	    {
	      unifications.push_back(Unification());
	      Unification& u = unifications.back();
	      u.lhs.swap(lhsAbstract);
	      u.rhs.swap(rhsAbstract);
	    }
	}
    }
  else
    {
      Term* identity = topSymbol->getIdentity();
      int rhsIndex = (identity != 0 && identity->equal(rhs)) ? NONE : dagToAbstract(rhs, solution);
      if (lhsAbstract.empty())
	{
	  if (rhsIndex == NONE)
	    ;  // ignore unification of identity with identity
	  else
	    {
	      //
	      //	We don't bother recording rhsIndex as marked because it's
	      //	going to be forced to identity, so the issue of it taking
	      //	more than one lhs subterm is moot.
	      //
	      WordSystem::Word rhsAbstract(1);
	      rhsAbstract[0] = rhsIndex;
	      nullEquations.push_back(rhsAbstract);
	    }
	}
      else
	{
	  if (rhsIndex == NONE)
	    nullEquations.push_back(lhsAbstract);
	  else
	    {
	      assignments.push_back(Assignment());
	      Assignment& a = assignments.back();
	      a.variable = rhsIndex;
	      a.value.swap(lhsAbstract);
	      if (marked)
		markedSubterms.insert(rhsIndex);
	    }
	}
    }
}

int
AU_UnificationSubproblem2::dagToAbstract(DagNode* dagNode, UnificationContext& solution)
{
  //
  //	First we replace a variable with its current representative. Really we should also
  //	do this for variables within aliens as well.
  //
  //	It is very tempting to replace variables by what they are bound to, but
  //	this fails. If we have a binding X |-> f(...) then the f-theory has already
  //	been solved. Thowing f(...) back into the mix means we will need to solve
  //	it again, and if there is a cycle between two or more theories that are
  //	doing eager replacement we won't terminate. Such cycles need to be handled
  //	by the cycle breaking code after the solved form has been computed.
  //
  //	Of course new f-theory terms could be exposed by delayering, resulting in
  //	the f-theory being solved again, but because terms have finite depth this
  //	can only happen a finite number of times.
  //
  if (VariableDagNode* varDagNode = dynamic_cast<VariableDagNode*>(dagNode))
    {
      varDagNode = varDagNode->lastVariableInChain(solution);
      //
      //	Normally we don't care about variables bound into our theory since they
      //	will be unsolved as part of the A/AU unification procedure to ensure
      //	termination. The exception is variables bound to our identity.
      //
      if (Term* identity = topSymbol->getIdentity())
	{
	  if (DagNode* subject = solution.value(varDagNode->getIndex()))
	    {
	      if (identity->equal(subject))
		return NONE;  // identity elements are just eliminated
	    }
	}
      //
      //	Otherwise we work with the representative variable.
      //
      dagNode = varDagNode;
    }
  //
  //	Now look for dag in list of nominally abstracted dags.
  //
  int nrSubterms = subterms.size();
  for (int i = 0; i < nrSubterms; ++i)
    {
      if (dagNode->equal(subterms[i]))
	return i;
    }
  //
  //	Not found so make a new entry.
  //
  subterms.append(dagNode);
  return nrSubterms;
}

void
AU_UnificationSubproblem2::assocToAbstract(DagNode* dagNode,
					   WordSystem::Word& word,
					   UnificationContext& solution)
{
  AU_DagNode* a = safeCast(AU_DagNode*, dagNode);
  int nrArgs = a->argArray.length();
  for (int i = 0; i < nrArgs; ++i)
    {
      int index = dagToAbstract(a->argArray[i], solution);
      if (index != NONE)
	word.append(index);
    }
}

void
AU_UnificationSubproblem2::makeWordSystem(UnificationContext& solution)
{
  int nrVariables = subterms.size();
  int nrEquations = unifications.size();
  //
  //	If we have an identity and the sort structure is such that
  //	it won't prevent fresh variables from taking identity or fail
  //	solutions that include extra variables that could take identity
  //	we can attempt to reduce the number of redundant unifiers found.
  //
  bool identityOptimizations = topSymbol->hasIdentity() &&
    !(topSymbol->hasUnequalLeftIdentityCollapse()) &&
    !(topSymbol->hasUnequalRightIdentityCollapse());
  wordSystem = new WordSystem(nrVariables, nrEquations, identityOptimizations);
  //
  //	Insert constraints for:
  //	* variables with a sort derived upper-bound
  //	* variables abstracting stable aliens
  //	* variables bound to stable aliens
  //
  //	The satisfaction of these constraints avoids some trivial
  //	failures do to order-sorted considerations or theory
  //	clashes that would otherwise be discovered later.
  //
  Term* identity = topSymbol->getIdentity();
  for (int i = 0; i < nrVariables; ++i)
    {
      DagNode* dagNode = subterms[i];
      //cout << "dealing with subterm " << dagNode << endl;
      if (VariableDagNode* varDagNode = dynamic_cast<VariableDagNode*>(dagNode))
	{
	  DagNode* value = solution.value(varDagNode->getIndex());
	  if (value != 0)
	    {
	      //
	      //	Variable is bound; see if it bound to a ground alien.
	      //
	      Symbol* symbol = value->symbol();
	      if (value->isGround())
		{
		  // FIXME: in one-side identity case, we could have the identity
		  //	itself as a ground alien; need to decide how we pass
		  //	uncanceled identity elements into WordSystem
		  wordSystem->setTheoryConstraint(i, symbol->getIndexWithinModule());
		  continue;
		}
	      //
	      //	Otherwise see if it bound to a stable alien.
	      //
	      if (symbol->isStable())
		{
		  wordSystem->setTheoryConstraint(i, symbol->getIndexWithinModule());
		  if (identity != 0 && symbol == identity->symbol())
		    {
		      //
		      //	We are bound to a stable alien that has the same
		      //	top symbol as our identity. We need to check
		      //	the sort of the variable.
		      //
		      VariableSymbol* variableSymbol = safeCast(VariableSymbol*, varDagNode->symbol());
		      Sort* variableSort = variableSymbol->getSort();
		      if (topSymbol->takeIdentity(variableSort))
			wordSystem->setTakeEmpty(i);
		    }
		  continue;
		}
	      // fall into unbound variable case
	    }
	  //
	  //	See if the variable has an upper-bound on the number
	  //	of subterms under our symbol that it can take, derived
	  //	from its sort. The classic case is a variable of an
	  //	element sort that cannot take our symbol.
	  //
	  VariableSymbol* variableSymbol = safeCast(VariableSymbol*, varDagNode->symbol());
	  Sort* variableSort = variableSymbol->getSort();
	  if (markedSubterms.contains(i))
	    wordSystem->setUpperBound(i, 1);  // must collapse to avoid nontermination
	  else
	    {
	      int variableSortBound = topSymbol->sortBound(variableSort);
	      if (variableSortBound != UNBOUNDED)
		wordSystem->setUpperBound(i, variableSortBound);
	    }
	  if (identity != 0 && topSymbol->takeIdentity(variableSort))
	    wordSystem->setTakeEmpty(i);
	}
      else
	{
	  Symbol* symbol = dagNode->symbol();
	  if (dagNode->isGround())
	    {
	      // FIXME: in one-side identity case, we could have the identity
	      //	itself as a ground alien; need to decide how we pass
	      //	uncanceled identity elements into WordSystem
	      wordSystem->setTheoryConstraint(i, symbol->getIndexWithinModule());
	      continue;
	    }
	  if (symbol->isStable())
	    {
	      wordSystem->setTheoryConstraint(i, symbol->getIndexWithinModule());
	      if (identity != 0 && symbol == identity->symbol())
		{
		  //
		  //	We are have a stable alien that has the same
		  //	top symbol as our identity. We must assume it could unify
		  //	with our identity.
		  //
		  wordSystem->setTakeEmpty(i);
		}
	      continue;
	    }
	  //
	  //	Unstable alien; if we have an identity we must assume it could
	  //	unify with the identity.
	  //
	  if (markedSubterms.contains(i))
	    wordSystem->setUpperBound(i, 1);  // must collapse to avoid nontermination
	  if (identity != 0)
	    wordSystem->setTakeEmpty(i);
	}
    }
  //
  //	Insert null equations
  //
  {
    for (const Word& w : nullEquations)
      {
	wordSystem->addNullEquation(w);
	DebugInfo("added null equation for " << w);
      }
  }
  //
  //	Insert abstract assignments.
  //
  {
    for (Assignment& a : assignments)
      {
	wordSystem->addAssignment(a.variable, a.value);
	DebugInfo("added assignment x" << a.variable << " |-> " << a.value);
      }
  }
  //
  //	Insert abstract equations.
  //
  {
    int counter = 0;
    for (Unification& u : unifications)
      {
	wordSystem->addEquation(counter, u.lhs, u.rhs);
	++counter;
	DebugInfo("added equation " << u.lhs << " =? " << u.rhs);
      }
  }
}

void
AU_UnificationSubproblem2::unsolve(int index, UnificationContext& solution)
{
  //
  //	We take a solved form X |-> f(...), turn it into abstract assignment and
  //	remove it from the current solution.
  //
  DagNode* variable = solution.getVariableDagNode(index);
  DagNode* value = solution.value(index);
  solution.bind(index, 0);

  Assert(variable != 0, "couldn't get variable for index " << index << " bound to " << value);
  DebugAdvisory("### unsolving " << variable << " <- " << value);

  assignments.push_back(Assignment());
  Assignment& a = assignments.back();
  a.variable = dagToAbstract(variable, solution);
  assocToAbstract(value, a.value, solution);
}

bool
AU_UnificationSubproblem2::solve(bool findFirst,
				 UnificationContext& solution,
				 PendingUnificationStack& pending)
{
  if (findFirst)
    {
      //
      //	Save the current substitution
      //
      preSolveSubstitution.clone(solution);
      //
      //	Unsolve any solved forms that are in our theory. This seemingly wasteful step
      //	has to be done in order to avoid nontermination.
      //
      //	The idea is that solved forms X = f(...) in our theory were created by us at some
      //	earlier invocation and represent decisions made about the solution on the current
      //	path. They must therefore be considered simultaneously with current unification
      //	subproblems otherwise we might generate an additional binding for X which is
      //	then resolved by creating yet another f-theory subproblem.
      //
      int nrFragile = solution.nrFragileBindings();
      for (int i = 0; i < nrFragile; ++i)
	{
	  DagNode* value = solution.value(i);
	  if (value != 0 && value->symbol() == topSymbol)
	    unsolve(i, solution);
	}
      //
      //	Make an abstract system of word equations.
      //
      makeWordSystem(solution);
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
      solution.restoreFromClone(savedSubstitution);
    }

  for (;;)
    {
      int result = wordSystem->findNextSolution();
      if (result & WordSystem::INCOMPLETE)
	pending.flagAsIncomplete(topSymbol);
      if (!(result & WordSystem::SUCCESS))
	break;

      if (buildSolution(solution, pending))
	return true;
      //
      //	Restore pending stack and current substitution, implicitly deallocating
      //	any fresh variables we introduced.
      //
      pending.restore(savedPendingState);
      solution.restoreFromClone(savedSubstitution);
    }
  //
  //	No more solutions; restore any solved forms that we unsolved.
  //
  solution.restoreFromClone(preSolveSubstitution);
  return false;
}


DagNode*
AU_UnificationSubproblem2::abstractToFreshVariable(int variableIndex, UnificationContext& solution)
{
  DagNode*& d = freshVariables[variableIndex];
  if (d == 0)
    d = solution.makeFreshVariable(topSymbol->rangeComponent());
  return d;
}

bool
AU_UnificationSubproblem2::buildSolution(UnificationContext& solution,
					 PendingUnificationStack& pending)
{
  //
  //	Not all fresh variables created by the word system need appear in the final solution; some
  //	may have been eliminated by being assigned to. So we created Maude fresh variables
  //	for them on demand.
  //
  int nrFreshVariables = wordSystem->getNrVariables();
  freshVariables.resize(nrFreshVariables);
  for (int i = 0; i < nrFreshVariables; ++i)
    freshVariables[i] = 0;
  NatSet reusedVariables;
  int nrVariables = subterms.size();
#define REUSE_VARIABLES
#ifdef REUSE_VARIABLES
  for (int i = 0; i < nrVariables; ++i)
    {
      //
      //	Look for abstracted variables whose abstract variable is assigned
      //	exactly one thing. The abstracted variable can be used in place
      //	of the fresh variable we would otherwise create for the abstract
      //	variable.
      //
      const WordSystem::Word& value = wordSystem->getAssignment(i);
      if (value.size() == 1)
	{
	  if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(subterms[i]))
	    {
	      int abstractVariable = value[0];
	      if (freshVariables[abstractVariable] == 0)
		{
		  freshVariables[abstractVariable] = v;
		  reusedVariables.insert(i);
		  DebugInfo("using original variable index " << i << ", " << subterms[i] <<
			    " instead of a fresh variable for abstract variable " <<
			    abstractVariable);
		}
	    }
	}
    }
#endif
  //
  //	We then go through each original variable and unify its subterm against the associative
  //	term with fresh variable arguments corresponding to the word system solution.
  //
  for (int i = 0; i < nrVariables; ++i)
    {
      if (reusedVariables.contains(i))
	continue;  // no point unifying a variable with itself
      bool inTheory = true;
       //
      //	Convert abstract assignment to Maude dag.
      //
      DagNode* d;
      const WordSystem::Word& value = wordSystem->getAssignment(i);
      int nrArgs = value.size();
      if (nrArgs == 0)
	d = topSymbol->getIdentityDag();
      else if (nrArgs == 1)
	{
	  d = abstractToFreshVariable(value[0], solution);
	  inTheory = false;  // we are assigning an alien term
	}
      else
	{
	  AU_DagNode* a = new AU_DagNode(topSymbol, nrArgs);
	  for (int j = 0; j < nrArgs; ++j)
	    a->argArray[j] = abstractToFreshVariable(value[j], solution);
	  d = a;
	}
      
      DagNode* subterm = subterms[i];
      if (VariableDagNode* varSubterm = dynamic_cast<VariableDagNode*>(subterm))
	{
	  //
	  //	We need to handle unbound variable subterms that we unify with something in our 
	  //	theory ourself to avoid generating another problem in our theory.
	  //
	  //	It is possible that other unifications generated by earlier iterations of this
	  //	loop merged another variable with our varSubterm so that it is no longer at the
	  //	end of its chain.
	  //
	  VariableDagNode* repVar = varSubterm->lastVariableInChain(solution);
	  if (solution.value(repVar->getIndex()) == 0 && inTheory)
	    {
	      solution.unificationBind(repVar, d);
	      continue;
	    }
	}
      //
      //	Now unify with abstracted subterm.
      //
      DebugInfo("unifying " << subterm << " =? " << d);
      if (!(subterm->computeSolvedForm(d, solution, pending)))
	return false;
    }
  return true;  
}
