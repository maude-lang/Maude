/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2021 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class AU_Symbol.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "AU_Persistent.hh"
#include "AU_Theory.hh"

//      core class definitions
#include "hashConsSet.hh"
#include "sortBdds.hh"

//	AU persistent class definitions
#include "AU_DequeIter.hh"

//	AU theory class definitions
#include "AU_Symbol.hh"
#include "AU_DagNode.hh"
#include "AU_DequeDagNode.hh"
#include "AU_Term.hh"
#include "AU_ExtensionInfo.hh"
#include "AU_UnificationSubproblem2.hh"

AU_Symbol::AU_Symbol(int id,
		     const Vector<int>& strategy,
		     bool memoFlag,
		     bool leftId,
		     bool rightId,
		     Term* identity)
  : AssociativeSymbol(id, strategy, memoFlag, identity),
    leftIdFlag(leftId),
    rightIdFlag(rightId),
    oneSidedIdFlag(leftId ^ rightId)
{
  //
  //	We only use the deque representaton for arguments if we
  //	don't need to deal with the complexities of:
  //	(1)	a one sided identity
  //	(2)	a lazy or semi-eager strategy
  //	(3)	memoization
  //	(4)	rewriting at the top
  //
  //	(1) is rare and (2) and (3) don't make sense without (4).
  //	(4) can't be checked until the compileEquations() pass.
  //	(4) might not be so rare but there is no fast deque based
  //	matching algorithm that works with extension.
  //
  useDequeFlag = !oneSidedIdFlag && standardStrategy();
}

void
AU_Symbol::postOpDeclarationPass()
{
  processIdentity();
  if (leftIdFlag)
    leftIdentitySortCheck();
  if (rightIdFlag)
    rightIdentitySortCheck();
}

void
AU_Symbol::compileEquations()
{
  AssociativeSymbol::compileEquations();
  if (!equationFree())
    useDequeFlag = false;
}

DagNode*
AU_Symbol::ruleRewrite(DagNode* subject, RewritingContext& context)
{
  if (ruleFree())
    return 0;
  AU_ExtensionInfo extensionInfo(getAU_DagNode(subject));
  return applyRules(subject, context, &extensionInfo);
}

Term*
AU_Symbol::makeTerm(const Vector<Term*>& args)
{
  return new AU_Term(this, args);
}

DagNode* 
AU_Symbol::makeDagNode(const Vector<DagNode*>& args)
{
  int nrArgs = args.length();
  AU_DagNode* a = new AU_DagNode(this, nrArgs);
  copy(args.begin(), args.end(), a->argArray.begin());
  return a;
}

bool
AU_Symbol::rewriteAtTop(AU_DagNode* subject, RewritingContext& context)
{
  //
  //	We have a separate function for this to keep AU_ExtensionInfo
  //	off of the eqRewrite() stack frame since recursion through
  //	eqRewrite() can get very deep.
  //	
  AU_ExtensionInfo extensionInfo(subject);
  return applyReplace(subject, context, &extensionInfo);
}

bool
AU_Symbol::rewriteAtTopNoOwise(AU_DagNode* subject, RewritingContext& context)
{
  //
  //	Same idea as above.
  //	
  AU_ExtensionInfo extensionInfo(subject);
  return applyReplaceNoOwise(subject, context, &extensionInfo);
}

bool
AU_Symbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  if (standardStrategy())
    {
      if (safeCast(AU_BaseDagNode*, subject)->isDeque())
	{
	  Assert(equationFree(), "deque with equations");
	  return false;
	}
      else
	{
	  AU_DagNode* s = safeCast(AU_DagNode*, subject);
	  if (s->isFresh())
	    {
	      //
	      //	Not safe to use iterator because reduce() can
	      //	call garbage collector which can relocate argArray.
	      //
	      int nrArgs = s->argArray.length();
	      for (int i = 0; i < nrArgs; i++)
		s->argArray[i]->reduce(context);
	      //
	      //	We always need to renormalize at the top because
	      //	shared subterms may have rewritten.
	      //
	      if (s->normalizeAtTop() <= AU_DagNode::DEQUED)
		return false;  // COLLAPSED or DEQUED
	    }
	  //
	  //	Even we were created by an assignment we could
	  //	be equation-free and not reduced because our true
	  //	sort was not known because of a membership axiom.
	  //
	  if (equationFree())
	    return false;

#ifndef NO_ASSERT
	  //
	  //	Look for Riesco 1/18/10 bug.
	  //
	  for (int i = 0; i < s->argArray.length(); i++)
	    {
	      DagNode* d = s->argArray[i];
	      Assert(d->getSortIndex() != Sort::SORT_UNKNOWN,
		     "AU_Symbol::eqRewrite(): unknown sort for AU argument " << d <<
		     " at index " << i << " in subject " << subject <<
		     " s->getNormalizationStatus() = " << s->getNormalizationStatus());
	    }
#endif

	  return rewriteAtTop(s, context);
	}
    }
  return complexStrategy(safeCast(AU_DagNode*, subject), context);
}

bool
AU_Symbol::complexStrategy(AU_DagNode* subject, RewritingContext& context)
{
  if (isMemoized())
    {
      MemoTable::SourceSet from;
      bool result = memoStrategy(from, subject, context);
      memoEnter(from, subject);
      //
      //	We may need to return true in the case we collapse to
      //	a unreduced subterm.
      //
      return result;
    }
  if (subject->isFresh())
    {
      int nrArgs = subject->argArray.length();
      for (int i = 0; i < nrArgs; i++)
	subject->argArray[i]->computeTrueSort(context);
      //
      //	If we collapse to one of our subterms which has not been reduced
      //	we pretend that we did a rewrite so that the reduction process
      //	continues.
      //
      if (subject->normalizeAtTop() == AU_DagNode::COLLAPSED)
	return !(subject->isReduced());
    }

  if (getPermuteStrategy() == LAZY)
    {
      if (rewriteAtTop(subject, context))
	return true;
      return false;
    }
  //
  //	Semi-eager case.
  //
  if (rewriteAtTopNoOwise(subject, context))
    return true;
  copyAndReduceSubterms(subject, context);
  if (subject->normalizeAtTop() == AU_DagNode::COLLAPSED)
    return false;
  subject->repudiateSortInfo();  // rewriteAtTopNoOwise() might have left sort behind
  return rewriteAtTop(subject, context);
}

bool
AU_Symbol::memoStrategy(MemoTable::SourceSet& from,
			DagNode* subject,
			RewritingContext& context)
{
  AU_DagNode* s = safeCast(AU_DagNode*, subject);
  ArgVec<DagNode*>& args = s->argArray;
  PermuteStrategy strat = getPermuteStrategy();
  if (strat == EAGER)
    {
      if (s->isFresh())
	{
	  int nrArgs = args.length();
	  for (int i = 0; i < nrArgs; i++)
	    args[i]->reduce(context);
	  //
	  //	We always need to renormalize at the top because
	  //	shared subterms may have rewritten.
	  //
	  if (s->normalizeAtTop() == AU_DagNode::COLLAPSED)
	    return false;
	}
    }
  else
    {
      if (s->isFresh())
	{
	  int nrArgs = args.length();
	  for (int i = 0; i < nrArgs; i++)
	    args[i]->computeTrueSort(context);
	  //
	  //	If we collapse to one of our subterms which has not been reduced
	  //	we pretend that we did a rewrite so that the reduction process
	  //	continues.
	  //
	  if (s->normalizeAtTop() == AU_DagNode::COLLAPSED)
	    return !(s->isReduced());  // the only place we might return true
	}

      if (memoRewrite(from, subject, context))
	return false;

      if (getPermuteStrategy() == LAZY)
	{
	  if (rewriteAtTop(s, context))
	    subject->reduce(context);
	  return false;
	}
      //
      //	Semi-eager case.
      //
      if (rewriteAtTopNoOwise(s, context))
	{
	  subject->reduce(context);
	  return false;
	}
      copyAndReduceSubterms(s, context);
      if (s->normalizeAtTop() == AU_DagNode::COLLAPSED)
	return false;
      s->repudiateSortInfo();  // rewriteAtTopNoOwise() might have left sort behind
    }
  if (!memoRewrite(from, subject, context) && rewriteAtTop(s, context))
    subject->reduce(context);
  return false;
}

void
AU_Symbol::copyAndReduceSubterms(AU_DagNode* subject, RewritingContext& context)
{
  ArgVec<DagNode*>& args = subject->argArray;
  int nrArgs = args.length();
  for (int i = 0; i < nrArgs; i++)
    args[i] = args[i]->copyAndReduce(context);
}

void
AU_Symbol::computeBaseSort(DagNode* subject)
{
  Assert(this == subject->symbol(), "bad symbol");
  if (safeCast(AU_BaseDagNode*, subject)->isDeque())
    {
      subject->setSortIndex(safeCast(AU_DequeDagNode*, subject)->
			    getDeque().computeBaseSort(this));
      return;
    }
  ArgVec<DagNode*>& args = safeCast(AU_DagNode*, subject)->argArray;
  if (const Sort* uniSort = uniformSort())
    {
      //
      //	If symbol has a uniform sort structure do a fast sort computation.
      //
      if (!(uniSort->component()->errorFree()))
	{
	  //
	  //	Check we're not in the error sort.
	  //
	  int lastIndex = Sort::SORT_UNKNOWN;
	  for (DagNode* d : args)
	    {
	      int index = d->getSortIndex();
	      if (index != lastIndex)
		{
		  if (!(leq(index, uniSort)))
		    {
		      subject->setSortIndex(Sort::ERROR_SORT);
		      return;
		    }
		  lastIndex = index;
		}
	    }
	}
      subject->setSortIndex(uniSort->index());
      return;
    }
  //
  //	Standard sort calculation.
  //
  int sortIndex = Sort::SORT_UNKNOWN;
  for (DagNode* d : args)
    {
      int t = d->getSortIndex();
      Assert(t != Sort::SORT_UNKNOWN, "bad sort index");
      sortIndex = (sortIndex == Sort::SORT_UNKNOWN) ? t :
	traverse(traverse(0, sortIndex), t);
    }
  subject->setSortIndex(sortIndex);
}

void
AU_Symbol::normalizeAndComputeTrueSort(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  if (safeCast(AU_BaseDagNode*, subject)->isFresh())
    {
      //
      //	Make sure each subterm has its true sort.
      //
      AU_DagNode* s = safeCast(AU_DagNode*, subject);
      ArgVec<DagNode*>& args = s->argArray;
      int nrArgs = args.length();
      for (int i = 0; i < nrArgs; i++)
	args[i]->computeTrueSort(context);
      //
      //    Put subject in normal form (could collapse to a subterm).
      //
      if (s->normalizeAtTop() == AU_DagNode::COLLAPSED)
	return;
    }
  //
  //	Finally compute subjects true sort.
  //
  fastComputeTrueSort(subject, context);
}

int
AU_Symbol::calculateNrSubjectsMatched(DagNode* d,
				      bool leftEnd,  // match starts at left extreme
				      bool rightEnd,  // match end at right extreme
				      bool& nasty)
{
  nasty = false;
  Term* identity = getIdentity();
  if (d->symbol() == this)
    {
      if (safeCast(AU_BaseDagNode*, d)->isDeque())
	return safeCast(AU_DequeDagNode*, d)->nrArgs();

      ArgVec<DagNode*>& args = safeCast(AU_DagNode*, d)->argArray;
      int nrArgs = args.length();
      if (oneSidedIdFlag)
	{
	  if (rightIdFlag)
	    {
	      if (identity->equal(args[0]))
		{
		  if (leftEnd)
		    nasty = true;
		  else
		    --nrArgs;  // identity may not match anything
		}
	    }
	  else
	    {
	      if (identity->equal(args[nrArgs - 1]))
		{
		  if (rightEnd)
		    nasty = true;
		  else
		    --nrArgs;  // identity may not match anything
		}
	    }
	}
      return nrArgs;
    }
  if (identity != 0 && identity->equal(d))
    {
      if (!(oneSidedIdFlag && (rightIdFlag ? leftEnd : rightEnd)))
	return 0;
      nasty = true;
    }
  return 1;
}

void
AU_Symbol::stackArguments(DagNode* subject,
			  Vector<RedexPosition>& stack,
			  int parentIndex,
			  bool respectFrozen,
			  bool eagerContext)
{
  if (respectFrozen && !(getFrozen().empty()))  // under A, any frozen argument affects all
    return;
  bool eager = eagerContext && (getPermuteStrategy() == EAGER);
  if (safeCast(AU_BaseDagNode*, subject)->isDeque())
    {
      //
      //	Deque case.
      //
      //Assert(getPermuteStrategy() == EAGER, "non eager strategy with deque");
      int j = 0;
      for (AU_DequeIter i(safeCast(AU_DequeDagNode*, subject)->getDeque());
	   i.valid(); i.next(), ++j)
	{
	  DagNode* d = i.getDagNode();
	  if (!(d->isUnstackable()))
	    stack.append(RedexPosition(d, parentIndex, j, eager));
	}
    }
  else
    {
      //
      //	ArgVec case.
      //
      ArgVec<DagNode*>& args = safeCast(AU_DagNode*, subject)->argArray;
      int nrArgs = args.length();
      for (int i = 0; i < nrArgs; i++)
	{
	  DagNode* d = args[i];
	  if (!(d->isUnstackable()))
	    stack.append(RedexPosition(d, parentIndex, i, eager));
	}
    }
}

Term*
AU_Symbol::termify(DagNode* dagNode)
{
  Vector<Term*> arguments;

  if (safeCast(const AU_BaseDagNode*, dagNode)->isDeque())
    {
      const AU_Deque& deque = safeCast(const AU_DequeDagNode*, dagNode)->getDeque();
      for (AU_DequeIter i(deque); i.valid(); i.next())
	{
	  DagNode* a = i.getDagNode();
	  arguments.append(a->symbol()->termify(a));
	}
    }
  else
    {
      const ArgVec<DagNode*>& argArray = safeCast(const AU_DagNode*, dagNode)->argArray;
      for (DagNode* a : argArray)
	arguments.append(a->symbol()->termify(a));
    }
  return new AU_Term(this, arguments);
}

bool
AU_Symbol::determineGround(DagNode* dagNode)
{
  if (safeCastNonNull<const AU_BaseDagNode*>(dagNode)->isDeque())
    {
      const AU_Deque& deque = safeCastNonNull<const AU_DequeDagNode*>(dagNode)->getDeque();
      for (AU_DequeIter i(deque); i.valid(); i.next())
	{
	  if (!(i.getDagNode()->determineGround()))
	    return false;
	}
    }
  else
    {
      const ArgVec<DagNode*>& argArray = safeCastNonNull<const AU_DagNode*>(dagNode)->argArray;
      for (DagNode* a : argArray)
	{
	  if (!(a->determineGround()))
	    return false;
	}
    }
  dagNode->setGround();
  return true;
}

//
//	Unification code.
//

void
AU_Symbol::computeGeneralizedSort(const SortBdds& sortBdds,
				   const Vector<int>& realToBdd,
				   DagNode* subject,
				   Vector<Bdd>& generalizedSort)
{
  Assert(safeCast(AU_BaseDagNode*, subject)->isDeque() == false,
	 "Deque case not implemented: " << subject <<
	 " " <<  static_cast<void*>(dynamic_cast<AU_DagNode*>(subject)) <<
	 " " <<  static_cast<void*>(dynamic_cast<AU_DequeDagNode*>(subject)));

  const Vector<Bdd>& sortFunction = sortBdds.getSortFunction(this);
  int nrBdds = sortFunction.size();  // number of bits/bdds to represent kind

  ArgVec<DagNode*>& args = safeCast(AU_DagNode*, subject)->argArray;
  bool firstArg = true;
  bddPair* argMap = bdd_newpair();
  for (DagNode* d : args)
    {
      //
      //	Get generalized sort of argument.
      //
      Vector<Bdd> argGenSort;
      d->computeGeneralizedSort(sortBdds, realToBdd, argGenSort);
      Assert((int) argGenSort.size() == nrBdds, "nrBdds clash");

      if (firstArg)
	{
	  firstArg = false;
	  generalizedSort = argGenSort;  // deep copy
	}
      else
	{
	  //
	  //	Map input variables to BDDs for current generalized sort, and generalized sort of argument.
	  //
	  for (int j = 0; j < nrBdds; ++j)
	    {
	      bdd_setbddpair(argMap, j, generalizedSort[j]);
	      bdd_setbddpair(argMap,  nrBdds + j, argGenSort[j]);
	    }
	  //
	  //	Use the sort function to compute a new generalized sort.
	  //
	  for (int j = 0; j < nrBdds; ++j)
	    generalizedSort[j] = bdd_veccompose(sortFunction[j], argMap);
	}
    }
  bdd_freepair(argMap);
}

// experimental code
void
AU_Symbol::computeGeneralizedSort2(const SortBdds& sortBdds,
				   const Vector<int>& realToBdd,
				   DagNode* subject,
				   Vector<Bdd>& outputBdds)
{
  Assert(safeCast(AU_BaseDagNode*, subject)->isDeque() == false,
	 "Deque case not implemented: " << subject <<
	 " " <<  static_cast<void*>(dynamic_cast<AU_DagNode*>(subject)) <<
	 " " <<  static_cast<void*>(dynamic_cast<AU_DequeDagNode*>(subject)));

  Vector<Bdd> inputBdds;  // assemble input to our sort function
  Vector<Bdd> middleBdds;  // put our output here if we're not finished

  ArgVec<DagNode*>& args = safeCast(AU_DagNode*, subject)->argArray;
  int lastArg = args.length() - 1;
  for (int i = 0;; i++)
    {
      //
      //	Generalized sort of ith argument.
      //
      args[i]->computeGeneralizedSort2(sortBdds, realToBdd, inputBdds);
      if (i == lastArg)
	{
	  //
	  //	Final application of our sort function with result
	  //	directly appended to outputBdds.
	  //
	  sortBdds.operatorCompose(this, inputBdds, outputBdds);
	  break;
	}
      else if (i > 0)
	{
	  //
	  //	Middle case - write result to middleBdds.
	  //
	  middleBdds.clear();
	  sortBdds.operatorCompose(this, inputBdds, middleBdds);
	  //
	  //	middleBdds become first part of inputBdds.
	  //
	  inputBdds.swap(middleBdds);
	}
    }
}

bool
AU_Symbol::canResolveTheoryClash()
{
  //
  //	We don't put this in parent class because returning true has
  //	the obligation to handle clashing in our unification subproblems.
  //
  return getIdentity() != 0;
}

UnificationSubproblem*
AU_Symbol::makeUnificationSubproblem()
{
  if (getIdentity() != 0)
    {
      //
      //	Because AU_UnificationSubproblem2 may introduce the
      //	identity element we make sure it had its sort computed
      //	and its ground flag set.
      //
      DagNode* id = getIdentityDag();
      if (!(id->isGround()))
	id->computeBaseSortForGroundSubterms(false);
    }
  return new AU_UnificationSubproblem2(this);
}

//
//	Hash cons code.
//

DagNode*
AU_Symbol::makeCanonical(DagNode* original, HashConsSet* hcs)
{
  if (safeCast(AU_BaseDagNode*, original)->isDeque())
    {
      //
      //	Never use deque form as canonical.
      //
      const AU_DequeDagNode* d = safeCast(const AU_DequeDagNode*, original);
      const AU_Deque& deque = d->getDeque();
      AU_DagNode* n = new AU_DagNode(this, deque.length());
       n->copySetRewritingFlags(original);
      n->setSortIndex(original->getSortIndex());
      ArgVec<DagNode*>::iterator j = n->argArray.begin();
      for (AU_DequeIter i(deque); i.valid(); i.next(), ++j)
	*j = hcs->getCanonical(hcs->insert(i.getDagNode()));
      n->setProducedByAssignment();  // deque form must be theory normal
      return n;
    }
  const AU_DagNode* d = safeCast(const AU_DagNode*, original);
  int nrArgs = d->argArray.size();
  for (int i = 0; i < nrArgs; i++)
    {
      DagNode* b = d->argArray[i];
      DagNode* c = hcs->getCanonical(hcs->insert(b));
      if (c != b)
        {
	  //
	  //	Detected a non-canonical argument so need to make a new node.
	  //
	  AU_DagNode* n = new AU_DagNode(this, nrArgs);
	  n->copySetRewritingFlags(original);
	  n->setSortIndex(original->getSortIndex());
	  for (int j = 0; j < i; ++j)
	    n->argArray[j] = d->argArray[j];
	  n->argArray[i] = c;
	  for (++i; i < nrArgs; i++)
	    n->argArray[i] = hcs->getCanonical(hcs->insert(d->argArray[i]));
	  n->setProducedByAssignment();  // only theory normal dags will be hash cons'd
	  return n;
        }
    }
  return original;  // can use the original dag node as the canonical version
}

DagNode*
AU_Symbol::makeCanonicalCopy(DagNode* original, HashConsSet* hcs)
{
  //
  //	We have a unreduced node - copy forced.
  //
  if (safeCast(AU_BaseDagNode*, original)->isDeque())
    {
      //
      //	Never use deque form as canonical for unreduced.
      //
      const AU_DequeDagNode* d = safeCast(const AU_DequeDagNode*, original);
      const AU_Deque& deque = d->getDeque();
      AU_DagNode* n = new AU_DagNode(this, deque.length());
      n->copySetRewritingFlags(original);
      n->setSortIndex(original->getSortIndex());
      ArgVec<DagNode*>::iterator j = n->argArray.begin();
      for (AU_DequeIter i(deque); i.valid(); i.next(), ++j)
	*j = hcs->getCanonical(hcs->insert(i.getDagNode()));
      n->setProducedByAssignment();  // deque form must be theory normal
      return n;
    }

  const AU_DagNode* d = safeCast(const AU_DagNode*, original);
  int nrArgs = d->argArray.size();
  AU_DagNode* n = new AU_DagNode(this, nrArgs);
  n->copySetRewritingFlags(original);
  n->setSortIndex(original->getSortIndex());
  for (int i = 0; i < nrArgs; i++)
    n->argArray[i] = hcs->getCanonical(hcs->insert(d->argArray[i]));
  n->setProducedByAssignment();  // only theory normal dags will be hash cons'd
  return n;
}
