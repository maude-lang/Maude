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
//      Implementation for class VariableDagNode
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      core class definitions
#include "substitution.hh"
#include "variableInfo.hh"
#include "narrowingVariableInfo.hh"
#include "unificationContext.hh"

//	variable class definitions
#include "variableSymbol.hh"
#include "variableDagNode.hh"

RawDagArgumentIterator*
VariableDagNode::arguments()
{
  return 0;
}

size_t
VariableDagNode::getHashValue()
{
  return hash(symbol()->getHashValue(), id());
}

int
VariableDagNode::compareArguments(const DagNode* other) const
{
  return id() - safeCastNonNull<const VariableDagNode*>(other)->id();
}

DagNode*
VariableDagNode::markArguments()
{
  return 0;
}

DagNode*
VariableDagNode::copyEagerUptoReduced2()
{
  return new VariableDagNode(symbol(), id(), index);
}

DagNode*
VariableDagNode::copyAll2()
{
  return new VariableDagNode(symbol(), id(), index);
}

void
VariableDagNode::clearCopyPointers2()
{
}

void
VariableDagNode::overwriteWithClone(DagNode* old)
{
  
  VariableDagNode* d = new(old) VariableDagNode(symbol(), id(), index);
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
}

DagNode*
VariableDagNode::makeClone()
{
  VariableDagNode* d = new VariableDagNode(symbol(), id(), index);
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
  return d;
}

DagNode*
VariableDagNode::copyWithReplacement(int /* argIndex */, DagNode* /* replacement */)
{
  CantHappen("should never be called");
  return 0;
}

DagNode*
VariableDagNode::copyWithReplacement(Vector<RedexPosition>& /* redexStack  */,
				     int /* first */,
				     int /* last */)
{
  CantHappen("should never be called");
  return 0;
}

//
//	Unification code.
//

DagNode::ReturnResult
VariableDagNode::computeBaseSortForGroundSubterms(bool /* warnAboutUnimplemented */)
{
  return NONGROUND;
}

bool
VariableDagNode::computeSolvedForm2(DagNode* rhs,
				    UnificationContext& solution,
				    PendingUnificationStack& pending)
{
  DebugEnter((DagNode*) this << " vs " << rhs);
  //
  //	In this version we only handle variable vs variable unfication and
  //	punt on everything else.
  //
  if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(rhs))
    {
      VariableDagNode* lv = lastVariableInChain(solution);
      VariableDagNode* rv = v->lastVariableInChain(solution);
      if (lv->equal(rv))
	return true;
      //
      //	We are preferentially going to bind lv |-> rv
      //	In order to maximally constrain the seach we want rv to
      //	have the lowest sort if they are comparable and unequal.
      //	We do this by ensuring rv has the largest sort index.
      //
      if (lv->symbol()->getSort()->index() > rv->symbol()->getSort()->index())
	swap(lv, rv);
      //
      //	Need to replace one variable by the other throughout the problem. We do this
      //	virtually and must check for implicit occurs check problems.
      //
      //	Might need to check we never map an original variable to a fresh variable.
      //
      DagNode* lt = solution.value(lv->index);
      if (lt == 0)
	return safeVirtualReplacement(lv, rv, solution, pending);

      DagNode* rt = solution.value(rv->index);
      if (rt == 0)
	return safeVirtualReplacement(rv, lv, solution, pending);
      //
      //	Both variables are bound.
      //
      return safeVirtualReplacement(lv, rv, solution, pending) &&
	lt->computeSolvedForm(rt, solution, pending);
    }
  //
  //	Calling computeSolvedForm() would just kick the problem back to us if
  //	rhs is ground, since this is a variable, and this would cause an infinite recursion.
  //
  return rhs->computeSolvedForm2(this, solution, pending);
}

bool
VariableDagNode::safeVirtualReplacement(VariableDagNode* oldVar,
					VariableDagNode* newVar,
					UnificationContext& solution,
					PendingUnificationStack& pending)
{
  //
  //	We want to replace all occurrences of oldVar by newVar. We assume oldVar is the last
  //	variable in its chain and is unbound (or has a binding which can be ignored because the
  //	caller is dealing with it) and newVar is the last variable in its chain.
  //	We do this by binding oldVar to newVar and since whenever we access a variable,
  //	we look for the last variable in the chain, accessing oldVar will give us newVar.
  //
  //	There is however a problem. If newVar is bound and its binding contains oldVar
  //	(or a variable equivalent to it) we generate an implicit occur check issue. We
  //    could un-solve and re-solve its binding, but if there is no occur check issue,
  //	re-solving could give us a similar variable replacement problem, also resulting in
  //	non-termination.
  //
  //	So we check the binding of newVar to see if it contains oldVar, and only then
  //	do we unsolve it.
  //
  solution.unificationBind(oldVar, newVar);
  DagNode* newBinding = solution.value(newVar->index);
  if (newBinding == 0 || newBinding->isGround())
    return true;
  
  NatSet occurs;
  newBinding->insertVariables(occurs);
  for (int index : occurs)
    {
      if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(solution.value(index)))
	{
	  if (v->lastVariableInChain(solution)->equal(newVar))
	    {
	      //
	      //	We have an occur check issue. We unsolve newVar |-> newBinding and re-solve it.
	      //
	      solution.bind(newVar->index, 0);
	      return newBinding->computeSolvedForm2(newVar, solution, pending);
	    }
	}
    }
  //
  //	No implicit occurs check issue found. Leave the binding of newVar as it was.
  //
  return true;
}

void
VariableDagNode::insertVariables2(NatSet& occurs)
{
  occurs.insert(index);
}

DagNode*
VariableDagNode::instantiate2(const Substitution& substitution, bool /* maintainInvariants */)
{
  return substitution.value(index);
}

VariableDagNode*
VariableDagNode::lastVariableInChain(Substitution& solution)
{
  //
  //	If a variable has been bound to anther variable, it is notionally
  //	replaced by that variable thoughout the problem, and in particular
  //	we need chase the replacement chain and find out what variable is
  //	notionally in its place.
  //
  VariableDagNode* v = this;
  for (;;)
    {
      DagNode* d = solution.value(v->index);
      if (d == 0)
	break;
      VariableDagNode* n = dynamic_cast<VariableDagNode*>(d);
      if (n == 0)
	break;
      Assert(v != n, "variable " << (DagNode*) v <<
	     " is bound to itself in a chain starting at " << (DagNode*) this);
      v = n;
    }
  return v;
}

//
//	Narrowing code.
//

bool
VariableDagNode::indexVariables2(NarrowingVariableInfo& indices, int baseIndex)
{
  index = baseIndex + indices.variable2Index(this);
  return false;
}

DagNode*
VariableDagNode::instantiateWithCopies2(const Substitution& /* substitution */,
					const Vector<DagNode*>& eagerCopies)
{
  //
  //	We must be in an eager position so use the eager copy.
  //
  return eagerCopies[index];
}

//
//	Variant match code.
//

void
VariableDagNode::indexVariables(VariableInfo& indicies)
{
  index = indicies.variable2Index(this);
}
