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
//      Implementation for class CUI_DagNode.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "CUI_Theory.hh"
 
//      interface class definitions
#include "binarySymbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "rawDagArgumentIterator.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"
#include "subproblem.hh"
#include "extensionInfo.hh"
 
//      core class definitions
#include "substitution.hh"
#include "pendingUnificationStack.hh"

//	variable class definitions
#include "variableDagNode.hh"

//      CUI theory class definitions
#include "CUI_Symbol.hh"
#include "CUI_DagNode.hh"
#include "CUI_DagArgumentIterator.hh"

RawDagArgumentIterator*
CUI_DagNode::arguments()
{
  return new CUI_DagArgumentIterator(argArray);
}

size_t
CUI_DagNode::getHashValue()
{
  if (isHashValid())
    return hashCache;
  size_t hashValue = hash(hash(symbol()->getHashValue(), argArray[0]->getHashValue()),
			  argArray[1]->getHashValue());
  hashCache = hashValue;
  setHashValid();
  return hashValue;
}

int
CUI_DagNode::compareArguments(const DagNode* other) const
{
  const CUI_DagNode* d = static_cast<const CUI_DagNode*>(other);
  int r = argArray[0]->compare(d->argArray[0]);
  if (r != 0)
    return r;
  return argArray[1]->compare(d->argArray[1]);
}

DagNode*
CUI_DagNode::markArguments()
{
  argArray[0]->mark();
  return argArray[1];
}

DagNode*
CUI_DagNode::copyEagerUptoReduced2()
{
  CUI_Symbol* s = symbol();
  CUI_DagNode* n = new CUI_DagNode(s);
  n->argArray[0] = s->eagerArgument(0) ? argArray[0]->copyEagerUptoReduced() : argArray[0];
  n->argArray[1] = s->eagerArgument(1) ? argArray[1]->copyEagerUptoReduced() : argArray[1];
  return n;
}

void
CUI_DagNode::clearCopyPointers2()
{
  argArray[0]->clearCopyPointers();
  argArray[1]->clearCopyPointers();
}

void
CUI_DagNode::overwriteWithClone(DagNode* old)
{
  CUI_DagNode* d = new(old) CUI_DagNode(symbol());
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
  d->argArray[0] = argArray[0];
  d->argArray[1] = argArray[1];
}

DagNode*
CUI_DagNode::makeClone()
{
  CUI_DagNode* d = new CUI_DagNode(symbol());
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
  d->argArray[0] = argArray[0];
  d->argArray[1] = argArray[1];
  return d;
}

DagNode*
CUI_DagNode::copyWithReplacement(int argIndex, DagNode* replacement)
{
  CUI_DagNode* d = new CUI_DagNode(symbol());
  if (argIndex == 0)
    {
      d->argArray[0] = replacement;
      d->argArray[1] = argArray[1];
    }
  else
    {
      Assert(argIndex == 1, "bad argument index");
      d->argArray[0] = argArray[0];
      d->argArray[1] = replacement;
    }
  return d;
}

DagNode*
CUI_DagNode::copyWithReplacement(Vector<RedexPosition>& redexStack,
				 int first,
				 int last)
{
  if (first == last)
    return copyWithReplacement(redexStack[first].argIndex(), redexStack[first].node());

  Assert(first + 1 == last, "nrArgs clash");
  CUI_DagNode* d = new CUI_DagNode(symbol());
  d->argArray[0] = redexStack[first].node();
  d->argArray[1] = redexStack[last].node();
  return d;
}

void
CUI_DagNode::stackArguments(Vector<RedexPosition>& stack,
			    int parentIndex,
			    bool respectFrozen)
{
  const NatSet& frozen = symbol()->getFrozen();
  DagNode* d = argArray[0];
  if (!(respectFrozen && frozen.contains(0)) && !(d->isUnstackable()))
    stack.append(RedexPosition(d, parentIndex, 0));
  DagNode* d2 = argArray[1];
  if (!(respectFrozen && frozen.contains(1)) && !(d->isUnstackable()) &&
      !(symbol()->comm() && d->equal(d2)))  // don't stack equal args in the comm case
    stack.append(RedexPosition(d2, parentIndex, 1));
}

void
CUI_DagNode::collapseTo(int argNr)
{
  DagNode* remaining = (symbol()->eagerArgument(argNr)) ?
    argArray[argNr] : argArray[argNr]->copyReducible();
  remaining->overwriteWithClone(this);
}

bool
CUI_DagNode::normalizeAtTop()
{
  CUI_Symbol* s = symbol();
  Term* identity = s->getIdentity();
  if (identity != 0)
    {
      if (s->leftId() && identity->equal(argArray[0]))
	{
	  collapseTo(1);
	  return true;
	}
      if (s->rightId() && identity->equal(argArray[1]))
	{
	  collapseTo(0);
	  return true;
	}
    }
  if (s->comm() || s->idem())
    {
      int r = argArray[0]->compare(argArray[1]);
      if (s->idem() && r == 0)
	{
	  collapseTo(0);
	  return true;
	}
      if (s->comm() && r > 0)
	{
	  DagNode* t = argArray[0];
	  argArray[0] = argArray[1];
	  argArray[1] = t;
	}
    }
  return false;
}

//
//	Unification code.
//

DagNode::ReturnResult
CUI_DagNode::computeBaseSortForGroundSubterms()
{
  CUI_Symbol* s = symbol();
  if (s->leftId() || s->rightId() || s->idem())
    {
      //
      //	We only support unification for commutativity at the moment
      //	so let the backstop version handle it.
      //
      return DagNode::computeBaseSortForGroundSubterms();
    }
  ReturnResult r0 = argArray[0]->computeBaseSortForGroundSubterms();
  if (r0 == UNIMPLEMENTED)
    return UNIMPLEMENTED;
  ReturnResult r1 = argArray[1]->computeBaseSortForGroundSubterms();
  if (r1 == UNIMPLEMENTED)
    return UNIMPLEMENTED;
  if (r0 == GROUND && r1 == GROUND)
    {
      s->computeBaseSort(this);
      return GROUND;
    }
  return NONGROUND;
}

bool
CUI_DagNode::computeSolvedForm2(DagNode* rhs, UnificationContext& solution, PendingUnificationStack& pending)
{
  if (symbol() == rhs->symbol())
    {
      //
      //	Both dagnodes are assumed to have their arguments sorted in ascending order. Equality
      //	between any two of the four arguments eliminates the need for branching.
      //
      DagNode** rhsArgs = safeCast(CUI_DagNode*, rhs)->argArray;
      DagNode* l0 = argArray[0];
      DagNode* l1 = argArray[1];
      DagNode* r0 = rhsArgs[0];
      DagNode* r1 = rhsArgs[1];
      //
      //	We know l0 <= l1 and r0 <= r1 because of normal forms. We will decide if at least one of
      //	the 6 possible equalities holds in at most 4 comparisons.
      //
      int r = l0->compare(r0);
      if (r == 0)
	return l1->computeSolvedForm(r1, solution, pending);
      if (r < 0)
	{
	  r = l1->compare(r0);
	  if (r == 0)
	    return l0->computeSolvedForm(r1, solution, pending);
	  if (r < 0)
	    {
	      //
	      //	We have l0 <= l1 < r0 <= r1 so we just check the two inequalities.
	      //
	      if (l0->equal(l1) || r0->equal(r1))
		goto dupArg;
	    }
	  else
	    {
	      r = l1->compare(r1);
	      if (r == 0)
		return l0->computeSolvedForm(r0, solution, pending);
	      if (r < 0)
		{
		  //
		  //	We have l0 < r0 < l1 < r1. No equalities possible.
		  //
		}
	      else
		{
		  //
		  //	We have l0 < r0 < l1 and r1 < l1. So r0 <= r1 is our only possible equality.
		  //
		  if (r0->equal(r1))
		    goto dupArg;
		}
	    }
	  
	}
      else
	{
	  r = l0->compare(r1);
	  if (r == 0)
	    return l1->computeSolvedForm(r0, solution, pending);
	  if (r < 0)
	    {
	      r = l1->compare(r1);
	      if (r == 0)
		return l0->computeSolvedForm(r0, solution, pending);
	      if (r < 0)
		{
		  //
		  //	We have r0 < l0 < r1 and l1 < r1. So l0 <= l1 is our only possible equality.
		  //
		  if (l0->equal(l1))
		    goto dupArg;
		}
	      else
		{
		  //
		  //	We have r0 < l0 < r1 < l1. No equalities possible.
		  //
		}
	    }
	  else
	    {
	      //
	      //	We have r0 <= r1 < l0 <= l1 so we just check the two inequalities.
	      //
	      if (l0->equal(l1) || r0->equal(r1))
		goto dupArg;

	    }
	}
      //
      //	We got here by falling out of one of the branches because no equalities were found. Therefore
      //	we have two possible ways of unifying that could give a mgu and need to postpone the decision.
      //
      pending.push(symbol(), this, rhs);
      return true;
    dupArg:
      //
      //	We got here because one side of the problem had duplicate arguments.
      //
      return l0->computeSolvedForm(r0, solution, pending) && l1->computeSolvedForm(r1, solution, pending);
    }
  if (dynamic_cast<VariableDagNode*>(rhs))
    return rhs->computeSolvedForm(this, solution, pending);
  return false;
}

mpz_class
CUI_DagNode::nonVariableSize()
{
  return 1 + argArray[0]->nonVariableSize() + argArray[1]->nonVariableSize();
}

void
CUI_DagNode::insertVariables2(NatSet& occurs)
{
  argArray[0]->insertVariables(occurs);
  argArray[1]->insertVariables(occurs);
}

DagNode*
CUI_DagNode::instantiate2(const Substitution& substitution)
{
  bool changed = false;
  DagNode* a0 = argArray[0];
  if (DagNode* n = a0->instantiate(substitution))
    {
      a0 = n;
      changed = true;
    }
  DagNode* a1 = argArray[1];
  if (DagNode* n = a1->instantiate(substitution))
    {
      a1 = n;
      changed = true;
    }
  if (changed)
    {
      CUI_Symbol* s = symbol();
      CUI_DagNode* d = new CUI_DagNode(s);
      d->argArray[0] = a0;
      d->argArray[1] = a1;
      if(!(d->normalizeAtTop()) && a0->isGround() && a1->isGround())
	{
	  s->computeBaseSort(d);
	  d->setGround();
	}
      return d;
    }
  return 0;
}

//
//	Narrowing code.
//

bool
CUI_DagNode::indexVariables2(NarrowingVariableInfo& indices, int baseIndex)
{
  return argArray[0]->indexVariables(indices, baseIndex) &  // always make both calls
    argArray[1]->indexVariables(indices, baseIndex);
}

DagNode*
CUI_DagNode::instantiateWithReplacement(const Substitution& substitution, int argIndex, DagNode* replacement)
{
  CUI_DagNode* d = new CUI_DagNode(symbol());
  int other = 1 - argIndex;
  d->argArray[argIndex] = replacement;
  DagNode* n = argArray[other];
  if (DagNode* c = n->instantiate(substitution))  // changed under substitutition
    n = c;
  d->argArray[other] = n;
  return d;
}
