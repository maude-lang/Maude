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
//      Implementation for class FreeDagNode.
//
#include <typeinfo>

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "freeTheory.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "rawDagArgumentIterator.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"
#include "subproblem.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "subproblemAccumulator.hh"
#include "pendingUnificationStack.hh"
#include "unificationContext.hh"

//	variable class definitions
#include "variableDagNode.hh"

//	free theory class definitions
#include "freeSymbol.hh"
#include "freeDagNode.hh"
#include "freeDagArgumentIterator.hh"

FreeDagNode::~FreeDagNode()
{
  //
  //	Accessing our symbol is forbidden since it may already have been deleted.
  //
  delete [] external;
}

RawDagArgumentIterator*
FreeDagNode::arguments()
{
  int nrArgs = symbol()->arity();
  if (nrArgs == 0)
    return 0;
  return new FreeDagArgumentIterator(argArray(), nrArgs);
}

size_t
FreeDagNode::getHashValue()
{
  if (isHashValid())
    return hashCache[nrWords - 1].size;
  size_t hashValue = symbol()->getHashValue();
  int nrArgs = symbol()->arity();
  DagNode** p = argArray();
  for (int i = 0; i < nrArgs; i++)
    hashValue = hash(hashValue, (*p++)->getHashValue());
  if (nrArgs != nrWords)
    {
      hashCache[nrWords - 1].size = hashValue;
      setHashValid();
    }
  return hashValue;
}

int
FreeDagNode::compareArguments(const DagNode* other) const
{
  Assert(symbol() == other->symbol(), "symbols differ");
  Symbol* s = symbol();
  int nrArgs = s->arity();
  if (nrArgs != 0)
    {
      const FreeDagNode* pd = this;  // last use of this pointer
      const FreeDagNode* qd = static_cast<const FreeDagNode*>(other);
      for (;;)
	{
	  DagNode** p = pd->argArray();
	  DagNode** q = qd->argArray();
	  
	  for (int i = nrArgs - 1; i > 0; --i, ++p, ++q)
	    {
	      int r = (*p)->compare(*q);
	      if (r != 0)
		return r;
	    }
	  //
	  //	Rightmost argument.
	  //
	  DagNode* pd2 = *p;
	  DagNode* qd2 = *q;
	  if (pd2 == qd2)
	    return 0;  // same node
	  Symbol* ps = pd2->symbol();
	  Symbol* qs = qd2->symbol();
	  if (ps != qs)
	    return ps->compare(qs);  // different symbols
	  if (ps != s)
	    return pd2->compareArguments(qd2);  // same symbol, not ours
	  //
	  //	Next iteration will compare argument lists using tail recursion elimination.
	  //
	  pd = static_cast<const FreeDagNode*>(pd2);
	  qd = static_cast<const FreeDagNode*>(qd2);
	}
    }
  return 0;
}

DagNode*
FreeDagNode::markArguments()
{
  int nrArgs = symbol()->arity();
  if (nrArgs != 0)
    {
      DagNode** p = argArray();
      while (--nrArgs > 0)
	(*p++)->mark();
      return *p;
    }
  return 0;
}

DagNode*
FreeDagNode::copyEagerUptoReduced2()
{
  FreeSymbol* s = symbol();
  FreeDagNode* n = new FreeDagNode(s);
  int nrArgs = s->arity();
  if (nrArgs != 0)
    {
      DagNode** p = argArray();
      DagNode** q = n->argArray();
      if (s->standardStrategy())  // everything is eager
	{
	  for (int i = nrArgs; i > 0; i--, p++, q++)
	    *q = (*p)->copyEagerUptoReduced();
	}
      else
	{
	  for (int i = 0; i < nrArgs; i++, p++, q++)
	    *q = s->eagerArgument(i) ? (*p)->copyEagerUptoReduced() : *p;
	}
    }
  return n;
}

DagNode*
FreeDagNode::copyAll2()
{
  FreeSymbol* s = symbol();
  FreeDagNode* n = new FreeDagNode(s);
  int nrArgs = s->arity();
  if (nrArgs != 0)
    {
      DagNode** p = argArray();
      DagNode** q = n->argArray();
      for (int i = nrArgs; i > 0; i--, p++, q++)
	*q = (*p)->copyAll();
    }
  return n;
}

void
FreeDagNode::clearCopyPointers2()
{
  int nrArgs = symbol()->arity();
  DagNode** p = argArray();
  for (int i = nrArgs; i > 0; i--, p++)
    (*p)->clearCopyPointers();
}

void
FreeDagNode::overwriteWithClone(DagNode* old)
{
  Symbol* s = symbol();
  FreeDagNode* d = new(old) FreeDagNode(s);
  int nrArgs = s->arity();
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
  DagNode** p = argArray();
  DagNode** q = d->argArray();
  for (int i = nrArgs; i > 0; i--, p++, q++)
    *q = *p;
}

DagNode*
FreeDagNode::makeClone()
{
  Symbol* s = symbol();
  FreeDagNode* d = new FreeDagNode(s);
  int nrArgs = s->arity();
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
  DagNode** p = argArray();
  DagNode** q = d->argArray();
  for (int i = nrArgs; i > 0; i--, p++, q++)
    *q = *p;
  return d;
}

DagNode*
FreeDagNode::copyWithReplacement(int argIndex, DagNode* replacement)
{
  FreeSymbol* s = symbol();
  FreeDagNode* d = new FreeDagNode(s);
  int nrArgs = s->arity();
  Assert(argIndex >= 0 && argIndex < nrArgs, "bad argIndex");
  DagNode** p = argArray();
  DagNode** q = d->argArray();
  for (int i = 0; i < nrArgs; i++, p++, q++)
    *q = (i == argIndex) ? replacement : *p;
  return d;
}

DagNode*
FreeDagNode::copyWithReplacement(Vector<RedexPosition>& redexStack,
				 int first,
				 int last)
{
  Assert(first >= 0 && first <= last && last < redexStack.length(),
	 "bad replacement range");

  FreeSymbol* s = symbol();
  int nrArgs = s->arity();
  Assert(redexStack[first].argIndex() >= 0 &&
	 redexStack[first].argIndex() < nrArgs &&
	 redexStack[last].argIndex() >= 0 &&
	 redexStack[last].argIndex() < nrArgs,
	 "bad replacement arg index");

  FreeDagNode* d = new FreeDagNode(s);
  DagNode** p = argArray();
  DagNode** q = d->argArray();
  int nextReplacementIndex = redexStack[first].argIndex();
  for (int i = 0; i < nrArgs; i++, p++, q++)
    {
      if (i == nextReplacementIndex)
	{
	  *q = redexStack[first].node();
	  ++first;
	  nextReplacementIndex = (first <= last) ?
	    redexStack[first].argIndex() : NONE;
	}
      else
	*q = *p;
    }
  return d;
}

//
//	Unification code.
//

DagNode::ReturnResult
FreeDagNode::computeBaseSortForGroundSubterms(bool warnAboutUnimplemented)
{
  ReturnResult result = GROUND;
  Symbol* s = symbol();
  int nrArgs = s->arity();
  DagNode** args = argArray();
  for (int i = 0; i < nrArgs; ++i)
    {
      ReturnResult r = args[i]->computeBaseSortForGroundSubterms(warnAboutUnimplemented);
      if (r > result)
	result = r;  // NONGROUND dominates GROUND, UNIMPLEMENTED dominates NONGROUND
    }
  if (result == GROUND)
    {
      s->computeBaseSort(this);
      setGround();
    }
  return result;
}

bool
FreeDagNode::computeSolvedForm2(DagNode* rhs,
				UnificationContext& solution,
				PendingUnificationStack& pending)
{
  DebugEnter(this << " vs " << rhs);
  Symbol* s = symbol();
  if (s == rhs->symbol())
    {
      int nrArgs = s->arity();
      Assert(nrArgs > 0, "we shouldn't be called on constants " << this);
      DagNode** args = argArray();
      DagNode** rhsArgs = safeCast(FreeDagNode*, rhs)->argArray();
      for (int i = 0; i < nrArgs; ++i)
	{
	  if (!(args[i]->computeSolvedForm(rhsArgs[i], solution, pending)))
	    return false;
	}
      return true;
    }
  if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(rhs))
    {
      //
      //	Get representative variable.
      //
      VariableDagNode* r = v->lastVariableInChain(solution);
      if (DagNode* value = solution.value(r->getIndex()))
	return computeSolvedForm(value, solution, pending);
      FreeDagNode* purified;
      switch (purifyAndOccurCheck(r, solution, pending, purified))
	{
	case OCCURS_CHECK_FAIL:
	  return false;
	case PURE_AS_IS:
	  purified = this;
	  break;
	case PURIFIED:
	  break;
	}
      solution.unificationBind(r, purified);
      DebugInfo("bound " << (DagNode*) r << " to " << purified);
      return true;
    }
  return pending.resolveTheoryClash(this, rhs);
}

FreeDagNode::PurificationStatus
FreeDagNode::purifyAndOccurCheck(DagNode* repVar,
				 UnificationContext& solution,
				 PendingUnificationStack& pending,
				 FreeDagNode*& purified)
{
  DebugEnter("this = " << this << "  representative variable = " << repVar);
  //
  //	For the moment we allow ground terms to be impure since they can't take part in cycles.
  //
  if (isGround())
    return PURE_AS_IS;

  FreeSymbol* s = symbol();
  int nrArgs = s->arity();
  DagNode** args = argArray();
  for (int i = 0; i < nrArgs; ++i)
    {
      DagNode* arg  = args[i];

      if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(arg))
	{
	  //
	  //	Variable - do an occur check.
	  //
	  if (v->lastVariableInChain(solution)->equal(repVar))
	    return OCCURS_CHECK_FAIL;
	  continue;
	}

      if (FreeDagNode* f = dynamic_cast<FreeDagNode*>(arg))
	{
	  //
	  //	Free subterm - recursive call.
	  //
	  switch (f->purifyAndOccurCheck(repVar, solution, pending, purified))
	    {
	    case OCCURS_CHECK_FAIL:
	      return OCCURS_CHECK_FAIL;
	    case PURE_AS_IS:
	      continue;
	    case PURIFIED:
	      {
		arg = purified;
		break;
	      }
	    }
	}
      else
	{
	  //
	  //	Alien subterm - purify it by variable abstraction.
	  //
	  VariableDagNode* abstractionVariable = solution.makeFreshVariable(s->domainComponent(i));
	  //
	  //	We can't do solution.unificationBind(abstractionVariable, arg) here because
	  //	arg may not be pure and this can lead to nontermination.
	  //
	  arg->computeSolvedForm(abstractionVariable, solution, pending);
	  arg = abstractionVariable;
	}
      {
	//
	//	If we get here, either we purified an alien or purification happen below us.
	//	Either way we need to rebuild.
	//
	FreeDagNode* d = new FreeDagNode(s);
	DagNode** args2 = d->argArray();
	//
	//	Copy the arguments we already looked at.
	//
	for (int j = 0; j < i; ++j)
	  args2[j] = args[j];
	//
	//	Handle current argument.
	//
	args2[i] = arg;
	//
	//	Handle remaining arguments.
	//
	for (++i; i < nrArgs; ++i)
	  {
	    DagNode* arg  = args[i];
	    
	    if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(arg))
	      {
		//
		//	Variable - do an occur check.
		//
		if (v->lastVariableInChain(solution)->equal(repVar))
		  return OCCURS_CHECK_FAIL;
	      }
	    else if (FreeDagNode* f = dynamic_cast<FreeDagNode*>(arg))
	      {
		//
		//	Free subterm - recursive call.
		//
		switch (f->purifyAndOccurCheck(repVar, solution, pending, purified))
		  {
		  case OCCURS_CHECK_FAIL:
		    return OCCURS_CHECK_FAIL;
		  case PURE_AS_IS:
		    break;
		  case PURIFIED:
		    {
		      arg = purified;
		      break;
		    }
		  }
	      }
	    else
	      {
		//
		//	Alien subterm - purify it by variable abstraction.
		//
		VariableDagNode* abstractionVariable = solution.makeFreshVariable(s->domainComponent(i));
		solution.unificationBind(abstractionVariable, arg);
		arg = abstractionVariable;
	      }
	    args2[i] = arg;
	  }
	purified = d;
	return PURIFIED;
      }
    }
  return PURE_AS_IS;
}

void
FreeDagNode::insertVariables2(NatSet& occurs)
{
  int i = symbol()->arity();
  if (i > 0)
    {
      for (DagNode** p = argArray(); i > 0; i--, p++)
	(*p)->insertVariables(occurs);
    }
}

DagNode*
FreeDagNode::instantiate2(const Substitution& substitution, bool maintainInvariants)
{
  //  cout << "FreeDagNode::instantiate2 enter " << this << endl;
  Symbol* s = symbol();
  int nrArgs = s->arity();

  Assert(nrArgs > 0, "we shouldn't be called on constants: " << this);
  DagNode** args = argArray();
  for (int i = 0; i < nrArgs; ++i)
    {
      if (DagNode* n = args[i]->instantiate(substitution, maintainInvariants))
	{
	  //
	  //	Argument changed under instantiation - need to make a new
	  //	dagnode.
	  //
	  bool ground = true;
	  FreeDagNode* d = new FreeDagNode(s);
	  DagNode** args2 = d->argArray();
	  //
	  //	Copy the arguments we already looked at.
	  //
	  for (int j = 0; j < i; ++j)
	    {
	      DagNode* a = args[j];
	      if (!(a->isGround()))
		ground = false;
	      args2[j] = a;
	    }
	  //
	  //	Handle current argument.
	  //
	  args2[i] = n;
	  if (!(n->isGround()))
	    ground = false;
	  //
	  //	Handle remaining arguments.
	  //
	  for (++i; i < nrArgs; ++i)
	    {
	      DagNode* a = args[i];
	      if (DagNode* n = a->instantiate(substitution, maintainInvariants))
		a = n;
	      if (!(a->isGround()))
		ground = false;
	      args2[i] = a;
	    }
	  //
	  //	Now we are maintaining invariants and if all the
	  //	arguments of the new dagnode are ground
	  //	we compute its base sort.
	  //
	  if (maintainInvariants && ground)
	    {
	      s->computeBaseSort(d);
	      d->setGround();
	    }
	  return d;	
	}
    }
  return 0;  // unchanged
}

//
//	Narrowing code.
//

bool
FreeDagNode::indexVariables2(NarrowingVariableInfo& indices, int baseIndex)
{
  int nrArgs = symbol()->arity();
  DagNode** args = argArray();
  bool ground = true;
  for (int i = 0; i < nrArgs; ++i)
    {
     if (!(args[i]->indexVariables(indices, baseIndex)))
	ground = false;
    }
  return ground;
}

DagNode*
FreeDagNode::instantiateWithReplacement(const Substitution& substitution,
					const Vector<DagNode*>* eagerCopies,
					int argIndex,
					DagNode* newDag)
{
  FreeSymbol* s = symbol();
  FreeDagNode* d = new FreeDagNode(s);
  int nrArgs = s->arity();
  Assert(argIndex >= 0 && argIndex < nrArgs, "bad argIndex");
  DagNode** p = argArray();
  DagNode** q = d->argArray();
  for (int i = 0; i < nrArgs; i++)
    {
      DagNode* n;
      if (i == argIndex)
	n = newDag;
      else
	{
	  n = p[i];
	  DagNode* c = (eagerCopies != 0) && s->eagerArgument(i) ?
	    n->instantiateWithCopies(substitution, *eagerCopies) :  // eager case - use copies of bindings
	    n->instantiate(substitution, false);  // lazy case - ok to use original unifier bindings
	  if (c != 0)  // changed under substitutition
	    n = c;
	}
      q[i] = n;
    }
  return d;
}

DagNode*
FreeDagNode::instantiateWithCopies2(const Substitution& substitution, const Vector<DagNode*>& eagerCopies)
{
  Symbol* s = symbol();
  int nrArgs = s->arity();

  Assert(nrArgs > 0, "we shouldn't be called on constants");
  DagNode** args = argArray();
  for (int i = 0; i < nrArgs; ++i)
    {
      DagNode* a = args[i];
      DagNode* n = s->eagerArgument(i) ?
	a->instantiateWithCopies(substitution, eagerCopies) :
	a->instantiate(substitution, false);  // lazy case - ok to use original unifier bindings since we won't evaluate them

      if (n != 0)
	{
	  //
	  //	Argument changed under instantiation - need to make a new
	  //	dagnode.
	  //
	  //bool ground = true;
	  FreeDagNode* d = new FreeDagNode(s);
	  DagNode** args2 = d->argArray();
	  //
	  //	Copy the arguments we already looked at.
	  //
	  for (int j = 0; j < i; ++j)
	    {
	      DagNode* a = args[j];
	      //if (!(a->isGround()))
	      //	ground = false;
	      args2[j] = a;
	    }
	  //
	  //	Handle current argument.
	  //
	  args2[i] = n;
	  //if (!(n->isGround()))
	  //  ground = false;
	  //
	  //	Handle remaining arguments.
	  //
	  for (++i; i < nrArgs; ++i)
	    {
	      DagNode* a = args[i];
	      DagNode* n = s->eagerArgument(i) ?
		a->instantiateWithCopies(substitution, eagerCopies) :
		a->instantiate(substitution, false);  // lazy case - ok to use original unifier bindings since we won't evaluate them
	      if (n != 0)
		a = n;
	      //if (!(a->isGround()))
	      //	ground = false;
	      args2[i] = a;
	    }
	  //
	  //	Currently the only user of this function is PositionState::rebuildAndInstantiateDag()
	  //	via instantiateWithCopies(), SAFE_INSTANTIATE() and instantiateWithReplacement(),
	  //	and this is only used for various kinds of narrowing steps. These are followed
	  //	by reduction so we don't need to worry about:
	  //	  normal forms
	  //	  sort computations
	  //	  ground flags
	  //
	  //	If this changes in the future the following will be needed:
	  //
#if 0
	  if (ground)
	    {
	      s->computeBaseSort(d);
	      d->setGround();
	    }
#endif
	  return d;	
	}
    }
  return 0;  // unchanged
}
