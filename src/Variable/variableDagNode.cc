/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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
  return id() - safeCast(const VariableDagNode*, other)->id();
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

void
VariableDagNode::stackArguments(Vector<RedexPosition>& /* stack */,
				int /* parentIndex */,
				bool /* respectFrozen */)
{
}

//
//	Unification code.
//

DagNode::ReturnResult
VariableDagNode::computeBaseSortForGroundSubterms()
{
  return NONGROUND;
}

bool
VariableDagNode::computeSolvedForm2(DagNode* rhs, UnificationContext& solution, PendingUnificationStack& pending)
{
  VariableDagNode* lv = lastVariableInChain(solution);
  if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(rhs))
    {
      //
      //	Variable against variable case.
      //
      VariableDagNode* rv = v->lastVariableInChain(solution);
      if (rv->index == lv->index)  // unifying a variable with itself
	return true;
      DagNode* lt = solution.value(lv->index);
      if (lt == 0)  // left variable unbound
	{
	  solution.unificationBind(lv, rv);
	  return true;
	}
      DagNode* rt = solution.value(rv->index);
      if (rt == 0)  // right variable unbound
	{
	  solution.unificationBind(rv, lv);
	  return true;
	}
      //
      //	Hard case - two variable chains with solved forms.
      //	In order to ensure termination we keep the smaller
      //	solved form.
      //
      //	Suppose lt >= rt. Notionally we add lv =? rv to the mix.
      //	Then we have:
      //	  lv =? rv, lv =? lt, rv =? rt
      //	By coalesce of lv and rv, we replace lv by rv, recording lv |-> rv and we get:
      //	  lv |-> rv, rv =? lt, rv =? rt
      //	and by merge on rv we get:
      //	  lv |-> rv, rv =? rt, lt =? rt
      //	which we implement by making the lv |-> rv binding followed
      //	by a recursive call to resolve lt =? rt. The lt >= rt case
      //	is almost symmetric.
      //	
      if (lt->nonVariableSize() >= rt->nonVariableSize())
	solution.unificationBind(lv, rv);
      else
	solution.unificationBind(rv, lv);
      return lt->computeSolvedForm(rt, solution, pending);
    }
  //
  //	Variable against non-variable case
  //
  {
    //
    //	Quick check for sort failure on ground term.
    //
    int sortIndex = rhs->getSortIndex();
    if (sortIndex != Sort::SORT_UNKNOWN && !::leq(sortIndex, safeCast(VariableSymbol*, symbol())->getSort()))
      return false;
  }
  DagNode* lt = solution.value(lv->index);
  if (lt == 0)
    {
      solution.unificationBind(lv, rhs);
      return true;
    }
  //
  //	Hard case - our variable already has a solved form. We do a merge step.
  //
  if (lt->nonVariableSize() > rhs->nonVariableSize())
    solution.unificationBind(lv, rhs);
  return lt->computeSolvedForm(rhs, solution, pending);
}

mpz_class
VariableDagNode::nonVariableSize()
{
  return 0;
}

void
VariableDagNode::insertVariables2(NatSet& occurs)
{
  occurs.insert(index);
}

DagNode*
VariableDagNode::instantiate2(const Substitution& substitution)
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
      v = n;
    }
  return v;
}

bool
VariableDagNode::indexVariables2(NarrowingVariableInfo& indices, int baseIndex)
{
  index = baseIndex + indices.variable2Index(this);
  return false;
}
