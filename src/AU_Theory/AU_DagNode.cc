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
//      Implementation for class AU_DagNode.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "AU_Theory.hh"
 
//      interface class definitions
#include "term.hh"

//      core class definitions
#include "substitution.hh"
#include "pendingUnificationStack.hh"
#include "unificationContext.hh"

//	variable class definitions
#include "variableSymbol.hh"
#include "variableDagNode.hh"

//      AU theory class definitions
#include "AU_Symbol.hh"
#include "AU_DagNode.hh"
#include "AU_DequeDagNode.hh"
#include "AU_DagArgumentIterator.hh"
#include "AU_ExtensionInfo.hh"
#include "AU_Subproblem.hh"

//	our stuff
#include "AU_Normalize.cc"
#include "AU_DagOperations.cc"

AU_DagNode*
getAU_DagNode(DagNode* d)
{
  if (safeCast(AU_BaseDagNode*, d)->isDeque())
    return AU_DequeDagNode::dequeToArgVec(safeCast(AU_DequeDagNode*, d));
  return safeCast(AU_DagNode*, d);
}

RawDagArgumentIterator*
AU_DagNode::arguments()
{
  return new AU_DagArgumentIterator(argArray);
}

size_t
AU_DagNode::getHashValue()
{
  size_t hashValue = symbol()->getHashValue();
  FOR_EACH_CONST(i, ArgVec<DagNode*>, argArray)
    hashValue = hash(hashValue, (*i)->getHashValue());
  return hashValue;
}

int
AU_DagNode::compareArguments(const DagNode* other) const
{
  if (safeCast(const AU_BaseDagNode*, other)->isDeque())
    return - safeCast(const AU_DequeDagNode*, other)->compare(this);

  const ArgVec<DagNode*>& argArray2 = safeCast(const AU_DagNode*, other)->argArray;
  int r = argArray.length() - argArray2.length();
  if (r != 0)
    return r;
 
  ArgVec<DagNode*>::const_iterator j = argArray2.begin();
  FOR_EACH_CONST(i, ArgVec<DagNode*>, argArray)
    {
      int r = (*i)->compare(*j);
      if (r != 0)
	return r;
      ++j;
    }
  Assert(j == argArray2.end(), "iterator problem");

  return 0;
}

DagNode*
AU_DagNode::markArguments()
{
  Assert(argArray.length() > 0, "no arguments");
  argArray.evacuate();
  //
  //	We avoid recursing on the first subterm that shares our symbol.
  //
  Symbol* s = symbol();
  DagNode* r = 0;
  FOR_EACH_CONST(i, ArgVec<DagNode*>, argArray)
    {
      DagNode* d = *i;
      if (r == 0 && d->symbol() == s)
	r = d;
      else
	d->mark();
    }
  return r;
}

DagNode*
AU_DagNode::copyEagerUptoReduced2()
{
  int nrArgs = argArray.length();
  AU_Symbol* s = symbol();
  AU_DagNode* n = new AU_DagNode(s, nrArgs);
  if (s->getPermuteStrategy() == BinarySymbol::EAGER)
    {
      for (int i = 0; i < nrArgs; i++)
	n->argArray[i] = argArray[i]->copyEagerUptoReduced();
    }
  else
    copy(argArray.begin(), argArray.end(), n->argArray.begin());
  return n;
}

DagNode*
AU_DagNode::copyAll2()
{
  int nrArgs = argArray.length();
  AU_Symbol* s = symbol();
  AU_DagNode* n = new AU_DagNode(s, nrArgs);
  for (int i = 0; i < nrArgs; i++)
    n->argArray[i] = argArray[i]->copyAll();
  return n;
}

void
AU_DagNode::clearCopyPointers2()
{
  FOR_EACH_CONST(i, ArgVec<DagNode*>, argArray)
    (*i)->clearCopyPointers();
}

void
AU_DagNode::overwriteWithClone(DagNode* old)
{
  AU_DagNode* d = new(old) AU_DagNode(symbol(), argArray.length());
  d->copySetRewritingFlags(this);
  d->setNormalizationStatus(getNormalizationStatus());
  d->setSortIndex(getSortIndex());
  copy(argArray.begin(), argArray.end(), d->argArray.begin());
}

DagNode*
AU_DagNode::makeClone()
{
  int nrArgs = argArray.length();
  AU_DagNode* d = new AU_DagNode(symbol(), nrArgs);
  d->copySetRewritingFlags(this);
  d->setNormalizationStatus(getNormalizationStatus());
  d->setSortIndex(getSortIndex());
  copy(argArray.begin(), argArray.end(), d->argArray.begin());
  return d;
}

DagNode*
AU_DagNode::copyWithReplacement(int argIndex, DagNode* replacement)
{
  int nrArgs = argArray.length();
  AU_DagNode* n = new AU_DagNode(symbol(), nrArgs);
  ArgVec<DagNode*>& args2 = n->argArray;
  for (int i = 0; i < nrArgs; i++)
    args2[i] = (i == argIndex) ? replacement : argArray[i];
  return n;
}

DagNode*
AU_DagNode::copyWithReplacement(Vector<RedexPosition>& redexStack,
				int first,
				int last)
{
  int nrArgs = argArray.length();
  AU_DagNode* n = new AU_DagNode(symbol(), nrArgs);
  ArgVec<DagNode*>& args = n->argArray;
  int nextReplacementIndex = redexStack[first].argIndex();
  for (int i = 0; i < nrArgs; i++)
    {
      if (i == nextReplacementIndex)
	{
	  args[i] = redexStack[first].node();
	  ++first;
	  nextReplacementIndex = (first <= last) ?
	    redexStack[first].argIndex() : NONE;
	}
      else
	args[i] = argArray[i];
    }
  return n;
}

void
AU_DagNode::partialReplace(DagNode* replacement, ExtensionInfo* extensionInfo)
{
  AU_ExtensionInfo* e = safeCast(AU_ExtensionInfo*, extensionInfo);
  int first = e->firstMatched();
  int last = e->lastMatched();
  argArray[first++] = replacement;
  int nrArgs = argArray.length();
  for (last++; last < nrArgs; last++)
    argArray[first++] = argArray[last];
  argArray.contractTo(first);
  repudiateSortInfo();  // probably not set but be safe
  if (isProducedByAssignment())  // we _were_ in theory normal form
    {
      if (replacement->symbol() == symbol() ||  // replacement is in our theory -> we are no longer in theory normal form
	  !(replacement->isReduced()))  // replacement is not reduced -> we are no longer in theory normal form
	setNormalizationStatus(FRESH);
    } 
}

DagNode*
AU_DagNode::partialConstruct(DagNode* replacement, ExtensionInfo* extensionInfo)
{
  AU_ExtensionInfo* e = safeCast(AU_ExtensionInfo*, extensionInfo);
  int first = e->firstMatched();
  int last = e->lastMatched();
  int nrArgs = argArray.length();
  AU_DagNode* n = new AU_DagNode(symbol(), nrArgs + first - last);
  ArgVec<DagNode*>& args2 = n->argArray;
  for (int i = 0; i < first; i++)
    args2[i] = argArray[i]; 
  args2[first++] = replacement;
  for (last++; last < nrArgs; last++)
    args2[first++] = argArray[last]; 
  return n;
}

ExtensionInfo*
AU_DagNode::makeExtensionInfo()
{
  return new AU_ExtensionInfo(this);
}

bool
AU_DagNode::matchVariableWithExtension(int index,
				      const Sort* sort,
				      Substitution& /* solution */,
				      Subproblem*& returnedSubproblem,
				      ExtensionInfo* extensionInfo)
{
  //
  //    This code could be much more sophisticated: in particular we could look for
  //    the variable having too smaller sort and return false; the subject having
  //    total subterm multiplicity of 2 and return unique solution.
  //
  AU_ExtensionInfo* e = safeCast(AU_ExtensionInfo*, extensionInfo);
  AU_Subproblem* subproblem = new AU_Subproblem(this, 0, argArray.length() - 1, 1, e);
  int min = symbol()->oneSidedId() ? 1 : 2;
  subproblem->addTopVariable(0, index, min, UNBOUNDED, const_cast<Sort*>(sort));  // HACK
  subproblem->complete();
  returnedSubproblem = subproblem;
  extensionInfo->setValidAfterMatch(false);
  return true;
}

//
//	Unification code.
//

DagNode::ReturnResult
AU_DagNode::computeBaseSortForGroundSubterms(bool warnAboutUnimplemented)
{
  AU_Symbol* s = symbol();
  //
  //	If we have an identity we bail to backstop version since AU/AUl/AUr is not
  //	currently supported for unification.
  //
  
  if (s->oneSidedId())
    return DagNode::computeBaseSortForGroundSubterms(warnAboutUnimplemented);
  
  
  ReturnResult result = GROUND;
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; ++i)
    {
      ReturnResult r = argArray[i]->computeBaseSortForGroundSubterms(warnAboutUnimplemented);
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
AU_DagNode::computeSolvedForm2(DagNode* rhs,
			       UnificationContext& solution,
			       PendingUnificationStack& pending)
{
  DebugEnter(this << " =? " << rhs);
  //
  //	If we have a one-sided identity we bail.
  //
  
  if (symbol()->oneSidedId())
    return DagNode::computeSolvedForm2(rhs, solution, pending);
  
  if (symbol() == rhs->symbol())
    {
      //
      //	AU unification problems with the same top symbol need to be collected and solved
      //	simultaneously for termination reasons.
      //
      pending.push(symbol(), this, rhs);
      return true;
    }
  if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(rhs))
    {
      VariableDagNode* r = v->lastVariableInChain(solution);
      if (DagNode* value = solution.value(r->getIndex()))
	return computeSolvedForm2(value, solution, pending);
      //
      //	We now treat unification problems f(...) =? X where X's representative
      //	variable is unbound as full AU unification problems now that the variable
      //	theory no longer resolves such problems and we require
      //	purification.
      //
      pending.push(symbol(), this, rhs);
      return true;
    }
  return pending.resolveTheoryClash(this, rhs);
}

void
AU_DagNode::insertVariables2(NatSet& occurs)
{
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; i++)
    argArray[i]->insertVariables(occurs);
}

DagNode*
AU_DagNode::instantiate2(const Substitution& substitution, bool maintainInvariants)
{
  AU_Symbol* s = symbol();
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; ++i)
    {
      if (DagNode* n = argArray[i]->instantiate(substitution, maintainInvariants))
	{
	  //
	  //	Argument changed under instantiation - need to make a new
	  //	dagnode.
	  //
	  bool ground = true;
	  AU_DagNode* d = new AU_DagNode(s, nrArgs);
	  //
	  //	Copy the arguments we already looked at.
	  //
	  for (int j = 0; j < i; ++j)
	    {
	      if (!(argArray[j]->isGround()))
		ground = false;
	      d->argArray[j] = argArray[j];	
	    }
	  //
	  //	Handle current argument.
	  //
	  d->argArray[i] = n;
	  if (!(n->isGround()))
	    ground = false;
	  //
	  //	Handle remaining arguments.
	  //
	  for (++i; i < nrArgs; ++i)
	    {
	      DagNode* a = argArray[i];
	      if (DagNode* n = a->instantiate(substitution, maintainInvariants))
		a = n;
	      if (!(a->isGround()))
		ground = false;
	      d->argArray[i] = a;
	    }
	  if (maintainInvariants)
	    {
	      //
	      //	Normalize the new dagnode. We pass the dumb flag as true to prevent deque
	      //	formation. If it doesn't collapse and all its arguments are ground we
	      //	compute its base sort, and set ground flag.
	      //
	      if (d->normalizeAtTop(true) != COLLAPSED && ground)
		{
		  s->computeBaseSort(d);
		  d->setGround();
		}
	      Assert(d->isDeque() == false, "Oops we got a deque! " << d);
	    }
	  else
	    {
	      if (ground)
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
AU_DagNode::indexVariables2(NarrowingVariableInfo& indices, int baseIndex)
{
  int nrArgs = argArray.length();
  bool ground = true;
  for (int i = 0; i < nrArgs; i++)
    {   
      if (!(argArray[i]->indexVariables(indices, baseIndex)))
	ground = false;
    }
  return ground;
}

DagNode*
AU_DagNode::instantiateWithReplacement(const Substitution& substitution,
				       const Vector<DagNode*>* eagerCopies,
				       int argIndex,
				       DagNode* newDag)
{
  int nrArgs = argArray.length();
  AU_DagNode* n = new AU_DagNode(symbol(), nrArgs);
  ArgVec<DagNode*>& args2 = n->argArray;
  bool eager = (eagerCopies != 0) && symbol()->getPermuteStrategy() == BinarySymbol::EAGER;
  for (int i = 0; i < nrArgs; i++)
    {
      DagNode* d;
      if (i == argIndex)
	d = newDag;
      else
	{
	  d = argArray[i];
	  SAFE_INSTANTIATE(d, eager, substitution, *eagerCopies);
	}
      args2[i] = d;
    }
  return n;
}

DagNode*
AU_DagNode::instantiateWithCopies2(const Substitution& substitution, const Vector<DagNode*>& eagerCopies)
{
  AU_Symbol* s = symbol();
  bool eager = s->getPermuteStrategy() == BinarySymbol::EAGER;
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; ++i)
    {
      DagNode* a = argArray[i];
      DagNode* n = eager ?
	a->instantiateWithCopies(substitution, eagerCopies) :
	a->instantiate(substitution, false);
      if (n != 0)
	{
	  //
	  //	Argument changed under instantiation - need to make a new
	  //	dagnode.
	  //
	  //bool ground = true;
	  AU_DagNode* d = new AU_DagNode(s, nrArgs);
	  //
	  //	Copy the arguments we already looked at.
	  //
	  for (int j = 0; j < i; ++j)
	    {
	      //if (!(argArray[j]->isGround()))
	      //	ground = false;
	      d->argArray[j] = argArray[j];	
	    }
	  //
	  //	Handle current argument.
	  //
	  d->argArray[i] = n;
	  //if (!(n->isGround()))
	  //  ground = false;
	  //
	  //	Handle remaining arguments.
	  //
	  for (++i; i < nrArgs; ++i)
	    {
	      DagNode* a = argArray[i];
	      SAFE_INSTANTIATE(a, eager, substitution, eagerCopies);
	      //if (!(a->isGround()))
	      //	ground = false;
	      d->argArray[i] = a;
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
	  //
	  //	Normalize the new dagnode. We pass the dumb flag as true to prevent deque
	  //	formation. If it doesn't collapse and all its arguments are ground we
	  //	compute its base sort, and set ground flag.
	  //
	  if (d->normalizeAtTop(true) != COLLAPSED && ground)
	    {
	      s->computeBaseSort(d);
	      d->setGround();
	    }
	  Assert(d->isDeque() == false, "Oops we got a deque! " << d);
#endif	
	  return d;
	}
    }
  return 0;  // unchanged
}
