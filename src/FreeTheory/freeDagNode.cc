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
//      Implementation for class FreeDagNode.
//
#include <typeinfo>

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
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

//	variable class definitions
#include "variableDagNode.hh"

//	free theory class definitions
#include "freeNet.hh"
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
  int nrArgs = symbol()->arity();
  if (nrArgs != 0)
    {
      DagNode** p = argArray();
      DagNode** q =
	const_cast<FreeDagNode*>(static_cast<const FreeDagNode*>(other))->argArray();
      for (;;)
        {
          int r = (*p)->compare(*q);
          if (r != 0)
            return r;
	  if (--nrArgs == 0)
	    break;
          ++p;
          ++q;
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
  FreeSymbol* s = static_cast<FreeSymbol*>(symbol());
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
  FreeSymbol* s = static_cast<FreeSymbol*>(symbol());
  FreeDagNode* d = new FreeDagNode(s);
  int nrArgs = s->arity();
  Assert(argIndex >= 0 && argIndex < nrArgs,
	 "bad argIndex");
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

  FreeSymbol* s = static_cast<FreeSymbol*>(symbol());
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

void
FreeDagNode::stackArguments(Vector<RedexPosition>& stack,
			    int parentIndex,
			    bool respectFrozen)
{
  int nrArgs = symbol()->arity();
  if (nrArgs != 0)
    {
      const NatSet& frozen = symbol()->getFrozen();
      DagNode** args = argArray();
      for (int i = 0; i < nrArgs; i++)
	{
	  DagNode* d = args[i];
	  if (!(respectFrozen && frozen.contains(i)) && !(d->isUnstackable()))
	    stack.append(RedexPosition(d, parentIndex, i));
	}
    }
}

bool
FreeDagNode::unify(DagNode* rhs,
		   Substitution& solution,
		   Subproblem*& returnedSubproblem,
		   ExtensionInfo* extensionInfo)
{
  if (symbol() == rhs->symbol())
    {
      int nrArgs = symbol()->arity();
      if (nrArgs != 0)
	{
	  SubproblemAccumulator subproblems;
	  DagNode** args = argArray();
	  DagNode** rhsArgs = safeCast(FreeDagNode*, rhs)->argArray();
	  for (int i = 0; i < nrArgs; ++i)
	    {
	      if (!(args[i]->unify(rhsArgs[i], solution, returnedSubproblem, 0)))
		return false;
	      subproblems.add(returnedSubproblem);
	    }
	  returnedSubproblem = subproblems.extractSubproblem();
	}
      else
	returnedSubproblem = 0;
      return true;
    }
  if (dynamic_cast<VariableDagNode*>(rhs))
    return rhs->unify(this, solution, returnedSubproblem, 0);
  return false;
}

bool
FreeDagNode::computeBaseSortForGroundSubterms()
{
  bool ground = true;
  Symbol* s = symbol();
  int nrArgs = s->arity();
  DagNode** args = argArray();
  for (int i = 0; i < nrArgs; ++i)
    {
      if (!(args[i]->computeBaseSortForGroundSubterms()))
	ground = false;
    }
  if (ground)
    s->computeBaseSort(this);
  return ground;
}

DagNode*
FreeDagNode::instantiate2(Substitution& substitution)
{
  //  cout << "FreeDagNode::instantiate2 enter " << this << endl;
  Symbol* s = symbol();
  int nrArgs = s->arity();
  Assert(nrArgs > 0, "we shouldn't be called on constants");
  DagNode** args = argArray();
  for (int i = 0; i < nrArgs; ++i)
    {
      if (DagNode* n = args[i]->instantiate(substitution))
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
	      if (a->getSortIndex() == Sort::SORT_UNKNOWN)
		ground = false;
	      args2[j] = a;
	    }
	  //
	  //	Handle current argument.
	  //
	  args2[i] = n;
	  if (n->getSortIndex() == Sort::SORT_UNKNOWN)
	    ground = false;
	  //
	  //	Handle remaining arguments.
	  //
	  for (++i; i < nrArgs; ++i)
	    {
	      DagNode* a = args[i];
	      if (DagNode* n = a->instantiate(substitution))
		a = n;
	      if (a->getSortIndex() == Sort::SORT_UNKNOWN)
		ground = false;
	      args2[i] = a;
	    }
	  //
	  //	Now if all the arguments of the new dagnode are ground
	  //	we compute its base sort.
	  //
	  if (ground)
	    s->computeBaseSort(d);
	  // cout << "FreeDagNode::instantiate2 exit " << d << endl;
	  return d;	
	}
    }
  //  cout << "FreeDagNode::instantiate2 exit null" << endl;
  return 0;  // unchanged
}

bool
FreeDagNode::occurs2(int index)
{
  int nrArgs = symbol()->arity();
  Assert(nrArgs > 0, "we shouldn't be called on constants");
  DagNode** p = argArray();
  for (int i = nrArgs; i > 0; i--, p++)
    {
      if ((*p)->occurs(index))
	return true;
    }
  return false;
}
