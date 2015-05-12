/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2012 SRI International, Menlo Park, CA 94025, USA.

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
//#include "subproblemAccumulator.hh"
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
  // maybe we only need to mark one of these?

  FOR_EACH_CONST(i, Vector<Problem>, problems)
    {
      int nrFragile = i->savedSubstitution.nrFragileBindings();
      for (int j = 0; j < nrFragile; j++)
	{
	  if (DagNode* d = i->savedSubstitution.value(j))
	    d->mark();
	}
    }
}

void
CUI_UnificationSubproblem2::addUnification(DagNode* lhs, DagNode* rhs, bool marked, UnificationContext& /* solution */)
{
  
  //cout << "CUI_UnificationSubproblem2::addUnification() " << lhs << " vs " << rhs << endl;

  CUI_DagNode* l = safeCast(CUI_DagNode*, lhs);
  CUI_Symbol* topSymbol = l->symbol();

  NatSet legalAlternatives;
  if (rhs->symbol() == topSymbol)
    {
      //
      //	f(u, v) =? f(s, t) case.
      //
      legalAlternatives.insert(FORWARDS);
      if (topSymbol->comm())
	legalAlternatives.insert(REVERSE);
      if (topSymbol->leftId())
	{
	  legalAlternatives.insert(LHS_ARG0_TAKES_ID);
	  legalAlternatives.insert(RHS_ARG0_TAKES_ID);
	}
      if (topSymbol->rightId())
	{
	  legalAlternatives.insert(LHS_ARG1_TAKES_ID);
	  legalAlternatives.insert(RHS_ARG1_TAKES_ID);
	}
    }
  else
    {
      //
      //	f(u, v) =? X and f(u, v) =? g(...) cases.
      //
      if (topSymbol->leftId())
	legalAlternatives.insert(LHS_ARG0_TAKES_ID);
      if (topSymbol->rightId())
	legalAlternatives.insert(LHS_ARG1_TAKES_ID);
     
      if (dynamic_cast<VariableDagNode*>(rhs) != 0 && !marked)  // will it always be true that marked == false?
	legalAlternatives.insert(RHS_VARIABLE_TAKES_ALL);
    }

  problems.append(Problem(l, rhs, legalAlternatives));
}

bool
CUI_UnificationSubproblem2::Problem::findAlternative(bool first, UnificationContext& solution, PendingUnificationStack& pending)
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
CUI_UnificationSubproblem2::Problem::tryAlternative(UnificationContext& solution, PendingUnificationStack& pending)
{
  switch (alternative)
    {
    case FORWARDS:
      {
	CUI_DagNode* r = safeCast(CUI_DagNode*, rhs);
	return lhs->getArgument(0)->computeSolvedForm(r->getArgument(0), solution, pending) &&
	  lhs->getArgument(1)->computeSolvedForm(r->getArgument(1), solution, pending);
      }
    case REVERSE:
      {
	CUI_DagNode* r = safeCast(CUI_DagNode*, rhs);
	return lhs->getArgument(0)->computeSolvedForm(r->getArgument(1), solution, pending) &&
	  lhs->getArgument(1)->computeSolvedForm(r->getArgument(0), solution, pending);
      }
    case  LHS_ARG0_TAKES_ID:
      {
	DagNode* id = lhs->symbol()->getIdentityDag();
	//
	//	If this is the first time we use the identity element it is possible
	//	that it will not have its sort computed or ground flag set.
	//
	if (!(id->isGround()))
	  id->computeBaseSortForGroundSubterms();
	return lhs->getArgument(0)->computeSolvedForm(id, solution, pending) &&
	  lhs->getArgument(1)->computeSolvedForm(rhs, solution, pending);
      }
    case  LHS_ARG1_TAKES_ID:
      {
	DagNode* id = lhs->symbol()->getIdentityDag();
	//
	//	If this is the first time we use the identity element it is possible
	//	that it will not have its sort computed or ground flag set.
	//
	if (!(id->isGround()))
	  id->computeBaseSortForGroundSubterms();
	return lhs->getArgument(1)->computeSolvedForm(id, solution, pending) &&
	  lhs->getArgument(0)->computeSolvedForm(rhs, solution, pending);
      }
    case  RHS_ARG0_TAKES_ID:
      {
	CUI_DagNode* r = safeCast(CUI_DagNode*, rhs);
	DagNode* id = lhs->symbol()->getIdentityDag();
	//
	//	If this is the first time we use the identity element it is possible
	//	that it will not have its sort computed or ground flag set.
	//
	if (!(id->isGround()))
	  id->computeBaseSortForGroundSubterms();
	return r->getArgument(0)->computeSolvedForm(id, solution, pending) &&
	  r->getArgument(1)->computeSolvedForm(lhs, solution, pending);
      }
    case  RHS_ARG1_TAKES_ID:
      {
	CUI_DagNode* r = safeCast(CUI_DagNode*, rhs);
	DagNode* id = lhs->symbol()->getIdentityDag();
	//
	//	If this is the first time we use the identity element it is possible
	//	that it will not have its sort computed or ground flag set.
	//
	if (!(id->isGround()))
	  id->computeBaseSortForGroundSubterms();
	return r->getArgument(1)->computeSolvedForm(id, solution, pending) &&
	  r->getArgument(0)->computeSolvedForm(lhs, solution, pending);
      }
    case RHS_VARIABLE_TAKES_ALL:
      {
	VariableDagNode* r = safeCast(VariableDagNode*, rhs);
	VariableDagNode* r2 = r->lastVariableInChain(solution);
	DagNode* d = solution.value(r2->getIndex());
	if (d == 0)
	  {
	    //
	    //	These occur checks are strictly necessary but they avoid generating
	    //	duplicate solutions via the cycle resolution code.
	    //
	    if (VariableDagNode* a = dynamic_cast<VariableDagNode*>(lhs->getArgument(0)))
	      {
		if (a->lastVariableInChain(solution)->equal(r))
		  return false;  // occurs check fail
	      }
	    if (VariableDagNode* a = dynamic_cast<VariableDagNode*>(lhs->getArgument(1)))
	      {
		if (a->lastVariableInChain(solution)->equal(r))
		  return false;  // occurs check fail
	      }
	    //
	    //	Need to bind this to purified ourselves to avoid generating the same problem again.
	    //
	    CUI_DagNode* purified = lhs->makePurifiedVersion(solution, pending);
	    solution.unificationBind(r, purified);
	    return true;
	  }
	//return false;
	return d->computeSolvedForm(lhs, solution, pending);	  
      }
    }
  return false;  // can't happen
}

bool
CUI_UnificationSubproblem2::solve(bool findFirst, UnificationContext& solution, PendingUnificationStack& pending)
{
  int nrProblems = problems.size();
  // cout << "CUI_UnificationSubproblem2::solve() have " << nrProblems << " problems" << endl;

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
