/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2015 SRI International, Menlo Park, CA 94025, USA.

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

#define FOR_EACH(var, conType, container) \
const conType::iterator var##_end = (container).end(); \
for (conType::iterator var = (container).begin(); var != var##_end; ++var)

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
AU_UnificationSubproblem2::addUnification(DagNode* lhs, DagNode* rhs, bool marked, UnificationContext& solution)
{
  Assert(lhs->symbol() == topSymbol, "bad lhs dag " << lhs);
  Assert(topSymbol->hasIdentity() ||
	 rhs->symbol() == topSymbol ||
	 dynamic_cast<VariableDagNode*>(rhs) != 0, "bad rhs for for non-collapse theory " << rhs);
  Assert(topSymbol->hasIdentity() || !marked, "bad mark for non-collapse theory");
  //
  //	Store unification problem as an abstract equation or assignment.
  //
  if (rhs->symbol() == topSymbol)
    {
      unifications.push_back(Unification());
      Unification& u = unifications.back();
      assocToAbstract(lhs, u.lhs, solution);
      assocToAbstract(rhs, u.rhs, solution);

    }
  else
    {
      assignments.push_back(Assignment());
      Assignment& a = assignments.back();
      a.variable = dagToAbstract(rhs, solution);
      assocToAbstract(lhs, a.value, solution);
    }
}

int
AU_UnificationSubproblem2::dagToAbstract(DagNode* dagNode, UnificationContext& solution)
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
	return i;
    }
  //
  //	Not found so make a new entry.
  //
  subterms.append(dagNode);
  return nrSubterms;
}

void
AU_UnificationSubproblem2::assocToAbstract(DagNode* dagNode, WordSystem::Word& word, UnificationContext& solution)
{
  AU_DagNode* a = safeCast(AU_DagNode*, dagNode);
  int nrArgs = a->argArray.length();
  word.resize(nrArgs);
  for (int i = 0; i < nrArgs; ++i)
    word[i] = dagToAbstract(a->argArray[i], solution);
}

void
AU_UnificationSubproblem2::makeWordSystem(UnificationContext& solution)
{
  int nrVariables = subterms.size();
  int nrEquations = unifications.size();
  wordSystem = new WordSystem(nrVariables, nrEquations);
  //
  //	Insert constraints for variables abstracting element variables, stable aliens and variables
  //   	with stable aliens assigned to them.
  //
  for (int i = 0; i < nrVariables; ++i)
    {
      DagNode* dagNode = subterms[i];
      //cout << "dealing with subterm " << dagNode << endl;
      if (VariableDagNode* varDagNode = dynamic_cast<VariableDagNode*>(dagNode))
	{
	  DagNode* value = solution.value(varDagNode->getIndex());
	  if (value != 0)
	    {
	      Symbol* symbol = value->symbol();
	      if (symbol->isStable() || value->isGround())
		{
		  wordSystem->addConstraint(i, symbol->getIndexWithinModule());
		  continue;
		}
	    }
	  VariableSymbol* variableSymbol = safeCast(VariableSymbol*, varDagNode->symbol());
	  Sort* variableSort = variableSymbol->getSort();
	  int variableSortBound = topSymbol->sortBound(variableSort);
	  if (variableSortBound == 1)
	    wordSystem->addConstraint(i, PigPug::ELEMENT);
	}
      else
	{
	  Symbol* symbol = dagNode->symbol();
	  if (symbol->isStable() || dagNode->isGround())
	    {
	      wordSystem->addConstraint(i, symbol->getIndexWithinModule());
	      //cout << "constrained to " << symbol << endl;
	    }
	}
    }
  //
  //	Insert abstract assignments.
  //
  {
    FOR_EACH_CONST(i, list<Assignment>, assignments)
      wordSystem->addAssignment(i->variable, i->value);
  }
  //
  //	Insert abstract equations.
  //
  {
    int counter = 0;
    FOR_EACH_CONST(i, list<Unification>, unifications)
      {
	wordSystem->addEquation(counter, i->lhs, i->rhs);
	++counter;
      }
  }
}

void
AU_UnificationSubproblem2::unsolve(int index, UnificationContext& solution)
{
  //
  //	We take a solved form X = f(...), turn it into abstract assignment and
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
AU_UnificationSubproblem2::solve(bool findFirst, UnificationContext& solution, PendingUnificationStack& pending)
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
AU_UnificationSubproblem2::buildSolution(UnificationContext& solution, PendingUnificationStack& pending)
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
  //
  //	We then go through each original variable and unify its subterm against the associative
  //	term with fresh variable arguments corresponding to the word system solution.
  //
  int nrVariables = subterms.size();
  for (int i = 0; i < nrVariables; ++i)
    {
      DagNode* subterm = subterms[i];
      //
      //	Convert abstract assignment to Maude dag.
      //
      DagNode* d;
      const WordSystem::Word& value = wordSystem->getAssignment(i);
      int nrArgs = value.size();
      if (nrArgs == 1)
	d = abstractToFreshVariable(value[0], solution);
      else
	{
	  AU_DagNode* a = new AU_DagNode(topSymbol, nrArgs);
	  for (int j = 0; j < nrArgs; ++j)
	    a->argArray[j] = abstractToFreshVariable(value[j], solution);
	  d = a;
	  if (VariableDagNode* varSubterm = dynamic_cast<VariableDagNode*>(subterm))
	    {
	      //
	      //	We need to handle unbound variable subterms that we unify with something in our 
	      //	theory ourself to avoid generating another problem in our theory.
	      //
	      //	It is possible that other unifications generated by earlier iterations of this loop
	      //	merged another variable with our varSubterm so that it is no longer at the
	      //	end of its chain.
	      //
	      VariableDagNode* repVar = varSubterm->lastVariableInChain(solution);
	      if (solution.value(repVar->getIndex()) == 0)
		{
		  solution.unificationBind(repVar, d);
		  continue;
		}
	    }
	}
      //
      //	Now unify with abstracted subterm.
      //
      if (!(subterm->computeSolvedForm(d, solution, pending)))
	return false;
    }
  return true;  
}
