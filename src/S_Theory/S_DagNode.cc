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
//      Implementation for class S_DagNode.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "S_Theory.hh"
 
//      interface class definitions
#include "term.hh"

//	variable class definitions
#include "variableDagNode.hh"

//      S theory class definitions
#include "S_Symbol.hh"
#include "S_DagNode.hh"
#include "S_DagArgumentIterator.hh"
#include "S_ExtensionInfo.hh"
#include "S_Subproblem.hh"

S_DagNode::S_DagNode(S_Symbol* symbol, const mpz_class& number, DagNode* arg)
  : DagNode(symbol),
    number(new mpz_class(number)),
    arg(arg)
{
  setCallDtor();  // need our dtor called when garbage collected to destruct number
}

S_DagNode::~S_DagNode()
{
  delete number;
}

RawDagArgumentIterator*
S_DagNode::arguments()
{
  return new S_DagArgumentIterator(arg);
}

size_t
S_DagNode::getHashValue()
{
  return hash(symbol()->getHashValue(),
	      arg->getHashValue(),
	      mpz_tdiv_ui(number->get_mpz_t(), INT_MAX));
}

int
S_DagNode::compareArguments(const DagNode* other) const
{
  const S_DagNode* d = safeCast(const S_DagNode*, other);
  int r = cmp(*number, *(d->number));
  return (r == 0) ? arg->compare(d->arg) : r;
}

DagNode*
S_DagNode::markArguments()
{
  return arg;
}

DagNode*
S_DagNode::copyEagerUptoReduced2()
{
  S_Symbol* s = symbol();
  DagNode* argCopy = (s->standardStrategy()) ? arg->copyEagerUptoReduced() : arg;
  return new S_DagNode(s, *number, argCopy);
}

void
S_DagNode::clearCopyPointers2()
{
  arg->clearCopyPointers();
}

void
S_DagNode::overwriteWithClone(DagNode* old)
{
  S_DagNode* d = new(old) S_DagNode(symbol(), *number, arg);
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
}

DagNode*
S_DagNode::makeClone()
{
  S_DagNode* d = new S_DagNode(symbol(), *number, arg);
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
  return d;
}

void
S_DagNode::normalizeAtTop()
{
  if (arg->symbol() == symbol())
    {
      S_DagNode* a = safeCast(S_DagNode*, arg);
      *number += *(a->number);
      arg = a->arg;
    }
}

DagNode*
S_DagNode::copyWithReplacement(int argIndex, DagNode* replacement)
{
  Assert(argIndex == 0, "bad arg index");
  return new S_DagNode(symbol(), *number, replacement);
}

DagNode*
S_DagNode::copyWithReplacement(Vector<RedexPosition>& redexStack,
			       int first,
			       int last)
{
  Assert(first == last, "nrArgs clash");
  return new S_DagNode(symbol(), *number, redexStack[first].node());
}

void
S_DagNode::stackArguments(Vector<RedexPosition>& stack,
			  int parentIndex,
			  bool respectFrozen)
{
  if (!(respectFrozen && symbol()->getFrozen().contains(0)) &&
      !(arg->isUnstackable()))
    stack.append(RedexPosition(arg, parentIndex, 0));
}

void
S_DagNode::partialReplace(DagNode* replacement, ExtensionInfo* extensionInfo)
{
  *number = safeCast(S_ExtensionInfo*, extensionInfo)->getUnmatched();
  arg = replacement;
  repudiateSortInfo();
}

DagNode*
S_DagNode::partialConstruct(DagNode* replacement, ExtensionInfo* extensionInfo)
{
  return new S_DagNode(symbol(),
		       safeCast(S_ExtensionInfo*, extensionInfo)->getUnmatched(),
		       replacement);
}

ExtensionInfo*
S_DagNode::makeExtensionInfo()
{
  return new S_ExtensionInfo(this);
}

bool
S_DagNode::matchVariableWithExtension(int index,
				      const Sort* sort,
				      Substitution& /* solution */,
				      Subproblem*& returnedSubproblem,
				      ExtensionInfo* extensionInfo)
{
  returnedSubproblem =
    new S_Subproblem(this,
		     *number,
		     index,
		     sort,
		     safeCast(S_ExtensionInfo*, extensionInfo));
  return true;
}

bool
S_DagNode::unify(DagNode* rhs,
		 Substitution& solution,
		 Subproblem*& returnedSubproblem,
		 ExtensionInfo* extensionInfo)
{
  //cout << this << " vs " << rhs << endl;
  S_Symbol* s = symbol();
  if (s == rhs->symbol())
    {
      S_DagNode* rhs2 = safeCast(S_DagNode*, rhs);
      mpz_class diff = *(rhs2->number) - *number;
      if (diff == 0)
	return arg->unify(rhs2->arg, solution, returnedSubproblem, 0);
      if (diff > 0)
	{
	  if (dynamic_cast<VariableDagNode*>(arg))
	    {
	      DagNode* d = new S_DagNode(s, diff, rhs2->arg);
	      if (rhs2->arg->getSortIndex() != Sort::SORT_UNKNOWN)
		s->computeBaseSort(d);
	      return arg->unify(d, solution, returnedSubproblem, 0);
	    }
	}
      else
	{
	  if (dynamic_cast<VariableDagNode*>(rhs2->arg))
	    {
	      DagNode* d = new S_DagNode(s, -diff, arg);
	      if (arg->getSortIndex() != Sort::SORT_UNKNOWN)
		s->computeBaseSort(d);
	      return rhs2->arg->unify(d, solution, returnedSubproblem, 0);
	    }
	}
      return 0;
    }
  if (dynamic_cast<VariableDagNode*>(rhs))
    return rhs->unify(this, solution, returnedSubproblem, 0);
  return false;
}

bool
S_DagNode::computeBaseSortForGroundSubterms()
{
  if (arg->computeBaseSortForGroundSubterms())
    {
      symbol()->computeBaseSort(this);
      return true;
    }
  return false;
}

DagNode*
S_DagNode::instantiate2(Substitution& substitution)
{
  if (DagNode* n = arg->instantiate(substitution))
    {
      mpz_class num = *number;
      S_Symbol* s = symbol();
      if (s == n->symbol())
	{
	  S_DagNode* t = safeCast(S_DagNode*, n);
	  num += *(t->number);
	  n = t->arg;
	}
      DagNode* d =  new S_DagNode(s, num, n);
      if (n->getSortIndex() != Sort::SORT_UNKNOWN)
	s->computeBaseSort(d);
      return d;
    }
  return 0;
}

bool
S_DagNode::occurs2(int index)
{
  return arg->occurs(index);
}
