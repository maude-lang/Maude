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
//      Implementation for class CUI_UnificationSubproblem2.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

#include "CUI_Theory.hh"

//      interface class definitions
#include "dagNode.hh"

//      core class definitions
#include "variableInfo.hh"
#include "unificationContext.hh"
#include "localBinding.hh"

//	variable class definitions
#include "variableSymbol.hh"
#include "variableDagNode.hh"

//	CUI theory class definitions
#include "CUI_Symbol.hh"
#include "CUI_DagNode.hh"
#include "CUI_UnificationSubproblem2.hh"

CUI_UnificationSubproblem2::CUI_UnificationSubproblem2()
{
  DebugAdvisory("Created CUI_UnificationSubproblem2() base " << ((void*) this));
}

CUI_UnificationSubproblem2::~CUI_UnificationSubproblem2()
{
}

void
CUI_UnificationSubproblem2::markReachableNodes()
{
  for (const Problem& p : problems)
    {
      int nrFragile = p.savedSubstitution.nrFragileBindings();
      for (int j = 0; j < nrFragile; j++)
	{
	  if (DagNode* d = p.savedSubstitution.value(j))
	    d->mark();
	}
    }
}

bool
CUI_UnificationSubproblem2::leftCollapse(DagNode* leftArg,
					 CUI_Symbol* topSymbol,
					 UnificationContext& solution)
{
  if (topSymbol->leftId())
    {
      if (VariableDagNode* var = dynamic_cast<VariableDagNode*>(leftArg))
	{
	  VariableDagNode* rep = var->lastVariableInChain(solution);
	  DagNode* binding = solution.value(rep->getIndex());
	  if (binding != 0 && binding->equal(topSymbol->getIdentityDag()))
	    return true;
	}
            //
      //	We expect that dags will be normalized before unifying so
      //	we should never see identity underneath us.
      //
      Assert(!(leftArg->equal(topSymbol->getIdentityDag())),"left arg is identity!");

    }
  return false;
}

bool
CUI_UnificationSubproblem2::rightCollapse(DagNode* rightArg,
					  CUI_Symbol* topSymbol,
					  UnificationContext& solution)
{
  if (topSymbol->rightId())
    {
      if (VariableDagNode* var = dynamic_cast<VariableDagNode*>(rightArg))
	{
	  VariableDagNode* rep = var->lastVariableInChain(solution);
	  DagNode* binding = solution.value(rep->getIndex());
	  if (binding != 0 && binding->equal(topSymbol->getIdentityDag()))
	    return true;
	}
      //
      //	We expect that dags will be normalized before unifying so
      //	we should never see identity underneath us.
      //
      Assert(!(rightArg->equal(topSymbol->getIdentityDag())),"right arg is identity!");
    }
  return false;
}

bool
CUI_UnificationSubproblem2::equivalent(DagNode* first,
				       DagNode* second,
				       UnificationContext& solution)
{
  if (VariableDagNode* var = dynamic_cast<VariableDagNode*>(first))
    {
      VariableDagNode* rep = var->lastVariableInChain(solution);
      DagNode* binding = solution.value(rep->getIndex());
      first = binding ? binding : rep;
    }
  if (VariableDagNode* var = dynamic_cast<VariableDagNode*>(second))
    {
      VariableDagNode* rep = var->lastVariableInChain(solution);
      DagNode* binding = solution.value(rep->getIndex());
      second = binding ? binding : rep;
    }
  return first->equal(second);
}
				 
void
CUI_UnificationSubproblem2::addUnification(DagNode* lhs,
					   DagNode* rhs,
					   bool marked,
					   UnificationContext& solution)
{
  DebugEnter(lhs << " vs " << rhs);
  
#ifdef CUI_DEBUG
  cout << "add " << lhs << " =? " << rhs << endl;
  cout << "context:\n";
  solution.dump(cout);
#endif

  CUI_DagNode* l = safeCast(CUI_DagNode*, lhs);
  CUI_Symbol* topSymbol = l->symbol();

  NatSet legalAlternatives;
  if (rhs->symbol() == topSymbol)
    {
      //
      //	f(u, v) =? f(s, t) cases.
      //
      CUI_DagNode* r = safeCast(CUI_DagNode*, rhs);
      //
      //	Check for degenerate cases.
      //
      if (leftCollapse(l->getArgument(0), topSymbol, solution))
	{
	  addUnification(rhs, l->getArgument(1), false, solution);
	  return;
	}
      if (rightCollapse(l->getArgument(1), topSymbol, solution))
	{
	  addUnification(rhs, l->getArgument(0), false, solution);
	  return;
	}
      if (leftCollapse(r->getArgument(0), topSymbol, solution))
	{
	  addUnification(lhs, r->getArgument(1), false, solution);
	  return;
	}
      if (rightCollapse(r->getArgument(1), topSymbol, solution))
	{
	  addUnification(lhs, r->getArgument(0), false, solution);
	  return;
	}
      //
      //	Main case.
      //
      legalAlternatives.insert(FORWARDS);
      //
      //	Will swappping the arguments make a difference?
      //
      bool lhsArgEquiv = equivalent(l->getArgument(0), l->getArgument(1), solution);
      bool rhsArgEquiv = equivalent(r->getArgument(0), r->getArgument(1), solution);
      if (topSymbol->comm() && !lhsArgEquiv && !rhsArgEquiv)
	legalAlternatives.insert(REVERSE);
      //
      //	Left id collapse on each side.
      //
      if (topSymbol->leftId())
	{
	  legalAlternatives.insert(LHS_ARG0_TAKES_ID);
	  legalAlternatives.insert(RHS_ARG0_TAKES_ID);
	  if (topSymbol->rightId())
	    {
	      //
	      //	Right id collapse on each side, if it is different from
	      //	left id collapse.
	      //
	      if (!lhsArgEquiv)
		legalAlternatives.insert(LHS_ARG1_TAKES_ID);
	      if (!rhsArgEquiv)
		legalAlternatives.insert(RHS_ARG1_TAKES_ID);
	    }
	}
      else if (topSymbol->rightId())
	{
	  //
	  //	Left id collapse on each side.
	  //
	  legalAlternatives.insert(LHS_ARG1_TAKES_ID);
	  legalAlternatives.insert(RHS_ARG1_TAKES_ID);
	}
    }
  else
    {
      //
      //	f(u, v) =? X and f(u, v) =? g(...) cases.
      //
      if (equivalentToGroundDag(rhs, topSymbol->getIdentityDag(), solution))
	{
	  //
	  //	Actually both arguments have to take identity regardless
	  //	of whether identity is left, right or two sided. This
	  //	is just a convenient way of getting this effect.
	  //
	  legalAlternatives.insert(LHS_ARG0_TAKES_ID);  // only option
	}
      else if (leftCollapse(l->getArgument(0), topSymbol, solution))
	legalAlternatives.insert(LHS_ARG0_TAKES_ID);  // only option
      else if (rightCollapse(l->getArgument(1), topSymbol, solution))
	legalAlternatives.insert(LHS_ARG1_TAKES_ID);  // only option
      else 
	{
	  if (topSymbol->leftId())
	    legalAlternatives.insert(LHS_ARG0_TAKES_ID);
	  //
	  //	We only do both collapses if the arguments are distinct
	  //	in order to reduce redundant solutions.
	  //
	  if (topSymbol->rightId() &&
	      (!(topSymbol->leftId()) ||
	       !(equivalent(l->getArgument(0), l->getArgument(1), solution))))
	    legalAlternatives.insert(LHS_ARG1_TAKES_ID);
	  if (!marked)
	    {
	      //
	      //	If we're marked we're obliged to collapse to a subterm.
	      //	If we're not marked and rhs is a variable we need to
	      //	consider X |-> purification of f(u, v) if X is free OR
	      //	X =? f(u, v) if X is bound.
	      //
	      if (dynamic_cast<VariableDagNode*>(rhs) != 0)
		legalAlternatives.insert(RHS_VARIABLE_TAKES_ALL);
	    }
	}
    }
  problems.append(Problem(l, rhs, legalAlternatives));
}

bool
CUI_UnificationSubproblem2::Problem::findAlternative(bool first,
						     UnificationContext& solution,
						     PendingUnificationStack& pending)
{
  if (first)
    alternative = FORWARDS;
  else
    {
      solution.restoreFromClone(savedSubstitution);
      pending.restore(savedPendingState);
      ++alternative;
    }

  while (alternative != NO_MORE_ALTERNATIVES)
    {
      if (legalAlternatives.contains(alternative))
	{
	  savedSubstitution.clone(solution);
	  savedPendingState = pending.checkPoint();
	  if (tryAlternative(solution, pending))
	    return true;
	  solution.restoreFromClone(savedSubstitution);
	  pending.restore(savedPendingState);
	}
      ++alternative;
    }
  return false;
}

bool
CUI_UnificationSubproblem2::Problem::tryAlternative(UnificationContext& solution,
						    PendingUnificationStack& pending)
{
  switch (alternative)
    {
    case FORWARDS:
      {
	CUI_DagNode* r = safeCast(CUI_DagNode*, rhs);
#ifdef CUI_DEBUG
	cout << "trying forwards: " << lhs->getArgument(0) << " =? " << r->getArgument(0) <<
	  " /\\ " << lhs->getArgument(1) << " =? " << r->getArgument(1) << endl;
#endif
	return lhs->getArgument(0)->computeSolvedForm(r->getArgument(0), solution, pending) &&
	  lhs->getArgument(1)->computeSolvedForm(r->getArgument(1), solution, pending);
      }
    case REVERSE:
      {
	CUI_DagNode* r = safeCast(CUI_DagNode*, rhs);
#ifdef CUI_DEBUG
	cout << "trying reverse: " << lhs->getArgument(0) << " =? " << r->getArgument(1) <<
	  " /\\ " << lhs->getArgument(1) << " =? " << r->getArgument(0) << endl;
#endif
	return lhs->getArgument(0)->computeSolvedForm(r->getArgument(1), solution, pending) &&
	  lhs->getArgument(1)->computeSolvedForm(r->getArgument(0), solution, pending);
      }
    case LHS_ARG0_TAKES_ID:
      {
	DagNode* id = lhs->symbol()->getIdentityDag();
#ifdef CUI_DEBUG
	cout << "trying lhs arg0 takes id: " << lhs->getArgument(0) << " =? " << id <<
	  " /\\ " << lhs->getArgument(1) << " =? " << rhs << endl;
#endif
	return lhs->getArgument(0)->computeSolvedForm(id, solution, pending) &&
	  lhs->getArgument(1)->computeSolvedForm(rhs, solution, pending);
      }
    case LHS_ARG1_TAKES_ID:
      {
	DagNode* id = lhs->symbol()->getIdentityDag();
#ifdef CUI_DEBUG
	cout << "trying lhs arg1 takes id: " << lhs->getArgument(1) << " =? " << id <<
	  " /\\ " << lhs->getArgument(0) << " =? " << rhs << endl;
#endif
	return lhs->getArgument(1)->computeSolvedForm(id, solution, pending) &&
	  lhs->getArgument(0)->computeSolvedForm(rhs, solution, pending);
      }
    case RHS_ARG0_TAKES_ID:
      {
	CUI_DagNode* r = safeCast(CUI_DagNode*, rhs);
	DagNode* id = lhs->symbol()->getIdentityDag();
#ifdef CUI_DEBUG
	cout << "trying rhs arg0 takes id: " << r->getArgument(0) << " =? " << id <<
	  " /\\ " << r->getArgument(1) << " =? " << rhs << endl;
#endif
	return r->getArgument(0)->computeSolvedForm(id, solution, pending) &&
	  r->getArgument(1)->computeSolvedForm(lhs, solution, pending);
      }
    case RHS_ARG1_TAKES_ID:
      {
	CUI_DagNode* r = safeCast(CUI_DagNode*, rhs);
	DagNode* id = lhs->symbol()->getIdentityDag();
#ifdef CUI_DEBUG
	cout << "trying rhs arg1 takes id: " << r->getArgument(1) << " =? " << id <<
	  " /\\ " << r->getArgument(0) << " =? " << rhs << endl;
#endif
	return r->getArgument(1)->computeSolvedForm(id, solution, pending) &&
	  r->getArgument(0)->computeSolvedForm(lhs, solution, pending);
      }
    case RHS_VARIABLE_TAKES_ALL:
      {
	VariableDagNode* r = safeCast(VariableDagNode*, rhs);
	VariableDagNode* r2 = r->lastVariableInChain(solution);
	DagNode* d = solution.value(r2->getIndex());
#ifdef CUI_DEBUG
	cout << "trying rhs take all: " << "rhs = " << (DagNode*) r <<
	  " = " << (DagNode*) r2 << " |-> " << d << " lhs = " << lhs << endl;
#endif
	if (d == 0)
	  {
	    //
	    //	Because we need to achieve a consistent simultaneous solution to
	    //	all equations in our theory including those that are bindings
	    //	into our theory that we didn't bother to unsolve, we need to
	    //	avoid making any binding that gives rise to an occur-check
	    //	failure local to our theory. We cannot cheat like we do in
	    //	pure commutativity and rely on compound cycle detection because
	    //	we have a collapse axiom.
	    //	Even if an occur-check failure is resolvable by collapse we
	    //	fail here because the collapse will have been tried on some
	    //	other branch.
	    //
	    if (lhs->indirectOccursCheck(r2, solution))
	      return false;
	    //
	    //	Need to bind this to purified ourselves to avoid generating the same problem again.
	    //
	    CUI_DagNode* purified = lhs->makePurifiedVersion(solution, pending);
#ifdef CUI_DEBUG
	    cout << "purified " << lhs << " to " << purified << endl;
#endif
	    solution.unificationBind(r2, purified);
	    return true;
	  }
#ifdef CUI_DEBUG
	cout << "solving " << d << " =? " << lhs << endl;
#endif
	return d->computeSolvedForm(lhs, solution, pending);	  
      }
    }
  CantHappen("nonexistent alternative");
  return false;
}

bool
CUI_UnificationSubproblem2::solve(bool findFirst,
				  UnificationContext& solution,
				  PendingUnificationStack& pending)
{
  int nrProblems = problems.size();
  DebugInfo("have " << nrProblems << " problems");
  int i;

  if (findFirst)
    {
      i = 0;
    forward:
      for (; i < nrProblems; ++i)
	{
	  Problem& p = problems[i];
	  if (!(p.findAlternative(true, solution, pending)))
	    {
	      --i;
	      goto backtrack;
	    }
	}
      return true;
    }
  else
    {
      i = nrProblems - 1;
    backtrack:
      for (; i >= 0; --i)
	{
	  Problem& p = problems[i];
	  if (p.findAlternative(false, solution, pending))
	    {
	      ++i;
	      goto forward;
	    }
	}
    }
  return false;
}
