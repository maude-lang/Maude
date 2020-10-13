/*

    This file is part of the Maude 3 interpreter.

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
//      Implementation for class ACU_Symbol.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "ACU_Persistent.hh"
#include "ACU_Theory.hh"

//      core class definitions
#include "sortBdds.hh"
#include "hashConsSet.hh"

//	ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_TreeDagNode.hh"
#include "ACU_Term.hh"
#include "ACU_ExtensionInfo.hh"
#include "ACU_UnificationSubproblem2.hh"

ACU_Symbol::ACU_Symbol(int id,
		       const Vector<int>& strategy,
		       bool memoFlag,
		       Term* identity,
		       bool useTree)
  : AssociativeSymbol(id, strategy, memoFlag, identity)
{
  useTreeFlag = useTree;
}

void 
ACU_Symbol::compileOpDeclarations()
{
  commutativeSortCompletion();
  Symbol::compileOpDeclarations();  // do default sort processing
}

void
ACU_Symbol::postOpDeclarationPass()
{
  processIdentity();
  if(getIdentity() != 0)
    {
      //
      //	No need to call rightIdentitySortCheck() - since we are commutative
      //	if there is a problem it will show up with leftIdentitySortCheck() and
      //	the latter is somewhat faster.
      //
      leftIdentitySortCheck();
    }
}

DagNode*
ACU_Symbol::ruleRewrite(DagNode* subject, RewritingContext& context)
{
  if (ruleFree())
    return 0;
  ACU_ExtensionInfo extensionInfo(safeCast(ACU_BaseDagNode*, subject));
  return applyRules(subject, context, &extensionInfo);
}

bool
ACU_Symbol::rewriteAtTop(DagNode* subject, RewritingContext& context)
{
  //
  //	We have a separate function for this to keep ACU_ExtensionInfo
  //	off of the eqRewrite() stack frame since recursion through
  //	eqRewrite() can get very deep.
  //	
  ACU_ExtensionInfo extensionInfo(safeCast(ACU_BaseDagNode*, subject));
  return applyReplace(subject, context, &extensionInfo);
}

bool
ACU_Symbol::rewriteAtTopNoOwise(DagNode* subject, RewritingContext& context)
{
  //
  //	Same idea as above.
  //	
  ACU_ExtensionInfo extensionInfo(safeCast(ACU_BaseDagNode*, subject));
  return applyReplaceNoOwise(subject, context, &extensionInfo);
}

bool
ACU_Symbol::normalize(DagNode* subject, RewritingContext& context)
{
  if (safeCast(ACU_BaseDagNode*, subject)->isFresh())
    {
      ACU_DagNode* s = safeCast(ACU_DagNode*, subject);
      int nrArgs = s->argArray.length();
      for (int i = 0; i < nrArgs; i++)
	s->argArray[i].dagNode->computeTrueSort(context);
      return s->normalizeAtTop();
    }
  return false;
}

bool
ACU_Symbol::copyReduceSubtermsAndNormalize(DagNode* subject, RewritingContext& context)
{
  ACU_DagNode* s = getACU_DagNode(subject);
  int nrArgs = s->argArray.length();
  for (int i = 0; i < nrArgs; i++)
    s->argArray[i].dagNode = s->argArray[i].dagNode->copyAndReduce(context);
  return s->normalizeAtTop();
}

Term*
ACU_Symbol::makeTerm(const Vector<Term*>& args)
{
  return new ACU_Term(this, args);
}

DagNode*
ACU_Symbol::makeDagNode(const Vector<DagNode*>& args)
{
  int nrArgs = args.length();
  ACU_DagNode* a = new ACU_DagNode(this, nrArgs);
  ArgVec<ACU_DagNode::Pair>& args2 = a->argArray;
  for (int i = 0; i < nrArgs; i++)
    {
      ACU_DagNode::Pair& p = args2[i];
      p.dagNode = args[i];
      p.multiplicity = 1;
    }
  return a;
}

DagNode*
ACU_Symbol::makeDagNode(const Vector<DagNode*>& args,
			const Vector<int>& multiplicities)
{
  int nrArgs = args.length();
  Assert(multiplicities.length() == nrArgs, "length mismatch");
  ACU_DagNode* a = new ACU_DagNode(this, nrArgs);
  ArgVec<ACU_DagNode::Pair>& args2 = a->argArray;
  for (int i = 0; i < nrArgs; i++)
    {
      ACU_DagNode::Pair& p = args2[i];
      p.dagNode = args[i];
      p.multiplicity = multiplicities[i];
    }
  return a;
}

bool
ACU_Symbol::reduceArgumentsAndNormalize(DagNode* subject, RewritingContext& context)
{
  if (safeCast(ACU_BaseDagNode*, subject)->isFresh())
    {
      ACU_DagNode* s = safeCast(ACU_DagNode*, subject);
      int nrArgs = s->argArray.length();
      for (int i = 0; i < nrArgs; i++)
	s->argArray[i].dagNode->reduce(context);
      //
      //	We always need to renormalize at the top because
      //	shared subterms may have rewritten.
      //
      return s->normalizeAtTop();
    }
  return false;
}

bool
ACU_Symbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  if (standardStrategy())
    {
      if (safeCast(ACU_BaseDagNode*, subject)->isFresh())
	{
	  ACU_DagNode* s = safeCast(ACU_DagNode*, subject);
	  int nrArgs = s->argArray.length();
	  for (int i = 0; i < nrArgs; i++)
	    s->argArray[i].dagNode->reduce(context);
	  //
	  //	We always need to renormalize at the top because
	  //	shared subterms may have rewritten.
	  //
	  if (s->normalizeAtTop())
	    return false;
	}
      if (equationFree())
	return false;
      return rewriteAtTop(subject, context);
    }
  return complexStrategy(subject, context);
}

bool
ACU_Symbol::complexStrategy(DagNode* subject, RewritingContext& context)
{
  if (isMemoized())
    {
      MemoTable::SourceSet from;
      bool result = memoStrategy(from, subject, context);
      memoEnter(from, subject);
      //
      //	We may need to return true in the case we collapse
      //	to a unreduced subterm.
      //
      return result;
    }
  //
  //	If we collapse to one of our subterms which has not been reduced
  //	we pretend that we did a rewrite so that the reduction process
  //	continues.
  //
  if (normalize(subject, context))
    return !(subject->isReduced());
  
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
  if (copyReduceSubtermsAndNormalize(subject, context))
    return false;
  subject->repudiateSortInfo();  // rewriteAtTopNoOwise() might have left sort behind
  return rewriteAtTop(subject, context);
}

bool
ACU_Symbol::memoStrategy(MemoTable::SourceSet& from,
			 DagNode* subject,
			 RewritingContext& context)
{
  if (getPermuteStrategy() == EAGER)
    {
      if (safeCast(ACU_BaseDagNode*, subject)->isFresh())
	{
	  ACU_DagNode* s = safeCast(ACU_DagNode*, subject);
	  int nrArgs = s->argArray.length();
	  for (int i = 0; i < nrArgs; i++)
	    s->argArray[i].dagNode->reduce(context);
	  //
	  //	We always need to renormalize at the top because
	  //	shared subterms may have rewritten.
	  //
	  if (s->normalizeAtTop())
	    return false;
	}
    }
  else
    {
      //
      //	If we collapse to one of our subterms which has not been reduced
      //	we pretend that we did a rewrite so that the reduction process
      //	continues.
      //
      if (normalize(subject, context))
	return !(subject->isReduced());

      if (memoRewrite(from, subject, context))
	return false;

      if (getPermuteStrategy() == LAZY)
	{
	  if (rewriteAtTop(subject, context))
	    subject->reduce(context);
	  return false;
	}
      //
      //	Semi-eager case.
      //
      if (rewriteAtTopNoOwise(subject, context))
	{
	  subject->reduce(context);
	  return false;
	}
      if (copyReduceSubtermsAndNormalize(subject, context))
	return false;
      subject->repudiateSortInfo();  // rewriteAtTopNoOwise() might have left sort behind
    }
  if (!memoRewrite(from, subject, context) && rewriteAtTop(subject, context))
    subject->reduce(context);
  return false;
}

void
ACU_Symbol::computeBaseSort(DagNode* subject)
{
  Assert(this == subject->symbol(), "bad symbol");
  subject->setSortIndex(safeCast(ACU_BaseDagNode*, subject)->isTree() ?
			(safeCast(ACU_TreeDagNode*, subject)->
			 getTree().computeBaseSort(this)) :
			safeCast(ACU_DagNode*, subject)->argVecComputeBaseSort());
}

void
ACU_Symbol::normalizeAndComputeTrueSort(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  if (normalize(subject, context))
    return;
  fastComputeTrueSort(subject, context);
}

void
ACU_Symbol::stackArguments(DagNode* subject,
			   Vector<RedexPosition>& stack,
			   int parentIndex,
			   bool respectFrozen,
			   bool eagerContext)
{
  if (respectFrozen && !(getFrozen().empty()))  // under AC, any frozen argument affects all
    return;
  bool eager = eagerContext & (getPermuteStrategy() == EAGER);
  int argNr = 0;
  if (safeCast(ACU_BaseDagNode*, subject)->isTree())
    {
      const ACU_Tree& tree = safeCast(ACU_TreeDagNode*, subject)->getTree();
      for (ACU_FastIter i(tree); i.valid(); i.next())
	{
	  DagNode* d = i.getDagNode();
	  int m = i.getMultiplicity();
	  if (d->isUnstackable())
	    argNr += m;
	  else
	    {
	      for (int j = m; j > 0; j--)
		{
		  stack.append(RedexPosition(d, parentIndex, argNr, eager));
		  ++argNr;
		}
	    }
	}
    }
  else
    {
      ArgVec<ACU_Pair>& args = safeCast(ACU_DagNode*, subject)->argArray;
      FOR_EACH_CONST(i, ArgVec<ACU_Pair>, args)
	{
	  DagNode* d = i->dagNode;
	  int m = i->multiplicity;
	  if (d->isUnstackable())
	    argNr += m;
	  else
	    {
	      for (int j = m; j > 0; j--)
		{
		  stack.append(RedexPosition(d, parentIndex, argNr, eager));
		  ++argNr;
		}
	    }
	}
    }
}

void
ACU_Symbol::stackPhysicalArguments(DagNode* subject,
				   Vector<RedexPosition>& stack,
				   int parentIndex,
				   bool respectFrozen,
				   bool eagerContext)
{
  if (respectFrozen && !(getFrozen().empty()))  // under AC, any frozen argument affects all
    return;
  bool eager = eagerContext & (getPermuteStrategy() == EAGER);
  if (safeCast(ACU_BaseDagNode*, subject)->isTree())
    {
      int argNr = 0;
      const ACU_Tree& tree = safeCast(ACU_TreeDagNode*, subject)->getTree();
      for (ACU_FastIter i(tree); i.valid(); i.next())
	{
	  DagNode* d = i.getDagNode();
	  if (!(d->isUnstackable()))
	    stack.append(RedexPosition(d, parentIndex, argNr, eager));
	  ++argNr;
	}
    }
  else
    {
      ArgVec<ACU_Pair>& argArray = safeCast(ACU_DagNode*, subject)->argArray;
      int nrArgs = argArray.length();
      for (int i = 0; i < nrArgs; i++)
	{
	  DagNode* d = argArray[i].dagNode;
	  if (!(d->isUnstackable()))
	    stack.append(RedexPosition(d, parentIndex, i, eager));
	}
    }
}

Term*
ACU_Symbol::termify(DagNode* dagNode)
{
  Vector<Term*> arguments;
  Vector<int> multiplicities;

  if (safeCast(const ACU_BaseDagNode*, dagNode)->isTree())
    {
      const ACU_Tree& tree = safeCast(const ACU_TreeDagNode*, dagNode)->getTree();
      for (ACU_FastIter i(tree); i.valid(); i.next())
	{
	  DagNode* a = i.getDagNode();
	  arguments.append(a->symbol()->termify(a));
	  multiplicities.append(i.getMultiplicity());
	}
    }
  else
    {
      const ArgVec<ACU_DagNode::Pair>& argArray = safeCast(const ACU_DagNode*, dagNode)->argArray;
      FOR_EACH_CONST(i, ArgVec<ACU_DagNode::Pair>, argArray)
	{
	  DagNode* a = i->dagNode;
	  arguments.append(a->symbol()->termify(a));
	  multiplicities.append(i->multiplicity);
	}
    }
  return new ACU_Term(this, arguments, multiplicities);
}

//
//	Unification code.
//

void
ACU_Symbol::computeGeneralizedSort(const SortBdds& sortBdds,
				   const Vector<int>& realToBdd,
				   DagNode* subject,
				   Vector<Bdd>& generalizedSort)
{
  Assert(safeCast(ACU_BaseDagNode*, subject)->isTree() == false,
	 "Tree case not implemented: " << subject <<
	 " " <<  static_cast<void*>(dynamic_cast<ACU_DagNode*>(subject)) <<
	 " " <<  static_cast<void*>(dynamic_cast<ACU_TreeDagNode*>(subject)));

  const Vector<Bdd>& sortFunction = sortBdds.getSortFunction(this);
  int nrBdds = sortFunction.size();

  ArgVec<ACU_Pair>& args = safeCast(ACU_DagNode*, subject)->argArray;
  bool firstArg = true;
  bddPair* argMap = bdd_newpair();
  FOR_EACH_CONST(i, ArgVec<ACU_Pair>, args)
    {
      Vector<Bdd> argGenSort;
      i->dagNode->computeGeneralizedSort(sortBdds, realToBdd, argGenSort);
      Assert((int) argGenSort.size() == nrBdds, "nrBdds clash");
      int multiplicity = i->multiplicity;

      if (firstArg)
	{
	  firstArg = false;
	  generalizedSort = argGenSort;  // deep copy
	  --multiplicity;
	}

      for(; multiplicity != 0; --multiplicity)
	{
	  //
	  //	Do a sort function application step.
	  //
	  for (int j = 0; j < nrBdds; ++j)
	    {
	      bdd_setbddpair(argMap, j, generalizedSort[j]);
	      bdd_setbddpair(argMap,  nrBdds + j, argGenSort[j]);
	    }
	  for (int j = 0; j < nrBdds; ++j)
	    generalizedSort[j] = bdd_veccompose(sortFunction[j], argMap);
	}
    }
  bdd_freepair(argMap);
}

// experimental code
void
ACU_Symbol::computeGeneralizedSort2(const SortBdds& sortBdds,
				    const Vector<int>& realToBdd,
				    DagNode* subject,
				    Vector<Bdd>& outputBdds)
{
  Assert(safeCast(ACU_BaseDagNode*, subject)->isTree() == false,
	 "Tree case not implemented: " << subject <<
	 " " <<  static_cast<void*>(dynamic_cast<ACU_DagNode*>(subject)) <<
	 " " <<  static_cast<void*>(dynamic_cast<ACU_TreeDagNode*>(subject)));

  ArgVec<ACU_Pair>& args = safeCast(ACU_DagNode*, subject)->argArray;
  int lastArg = args.length() - 1;
  Vector<Bdd> inputBdds;
  Vector<Bdd> middleBdds;
  for (int i = 0;; ++i)   
    {
      //
      //	Generalized sort ith argument.
      //
      args[i].dagNode->computeGeneralizedSort2(sortBdds, realToBdd, inputBdds);
      int multiplicity = args[i].multiplicity;
      
      if (i == 0)
	{
	  --multiplicity;
	  if (multiplicity == 0)
	    continue;  // must be a next argument
	  //
	  //	Special case - first argument has multiplicity > 1 so we
	  //	need to clone its sort BDDs.
	  //
	  int nrBdds = inputBdds.size();
	  //
	  //	Need to be careful since we are reading and writing the same Vector
	  //	and a reference can be come stale if the Vector reallocates.
	  //	So we force the reallocation, if needed, up front.
	  //
	  inputBdds.resize(2 * nrBdds);
	  for (int j = 0; j < nrBdds; ++j)
	    inputBdds[nrBdds + j] = inputBdds[j];
	}

      for (--multiplicity; multiplicity != 0; --multiplicity)
	{
	  middleBdds.clear();
	  sortBdds.operatorCompose(this, inputBdds, middleBdds);
	  //
          //    Copy middleBdds over first half of inputBdds.
          //
          Vector<Bdd>::iterator input = inputBdds.begin();
          FOR_EACH_CONST(j, Vector<Bdd>, middleBdds)
            {
              *input = *j;
              ++input;
            }
	}

      if (i == lastArg)
	{
	  //
	  //	Final application of our sort function with result
	  //	directly appended to outputBdds.
	  //
	  sortBdds.operatorCompose(this, inputBdds, outputBdds);
	  break;
	}
      else
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
ACU_Symbol::canResolveTheoryClash()
{
  //
  //	We don't put this in parent class because return true has
  //	the obligation to handle clashing in our unification subproblems.
  //
  return getIdentity() != 0;
}

UnificationSubproblem*
ACU_Symbol::makeUnificationSubproblem()
{
  if (getIdentity() != 0)
    {
      //
      //	Because ACU_UnificationSubproblem2 may introduce the
      //	identity element we make sure it had its sort computed
      //	and its ground flag set.
      //
      DagNode* id = getIdentityDag();
      if (!(id->isGround()))
	id->computeBaseSortForGroundSubterms(false);
    }
  return new ACU_UnificationSubproblem2(this);
}

int 
ACU_Symbol::unificationPriority() const
{
  return 10 + 10 * (getIdentity() != 0);
}

//
//	Hash cons code.
//

DagNode*
ACU_Symbol::makeCanonical(DagNode* original, HashConsSet* hcs)
{
  if (safeCast(ACU_BaseDagNode*, original)->isTree())
    return safeCast(ACU_TreeDagNode*, original)->makeCanonical(hcs);

  const ACU_DagNode* d = safeCast(const ACU_DagNode*, original);
  int nrArgs = d->argArray.size();
  for (int i = 0; i < nrArgs; i++)
    {
      DagNode* b = d->argArray[i].dagNode;
      DagNode* c = hcs->getCanonical(hcs->insert(b));
      if (c != b)
        {
	  //
	  //	Detected a non-canonical argument so need to make a new node.
	  //
	  ACU_DagNode* n = new ACU_DagNode(this, nrArgs, ACU_BaseDagNode::ASSIGNMENT);
	  n->copySetRewritingFlags(original);
	  n->setSortIndex(original->getSortIndex());
	  for (int j = 0; j < i; ++j)
	    n->argArray[j] = d->argArray[j];
	  n->argArray[i].dagNode = c;
	  n->argArray[i].multiplicity = d->argArray[i].multiplicity;
	  for (++i; i < nrArgs; i++)
	    {
	      n->argArray[i].dagNode = hcs->getCanonical(hcs->insert(d->argArray[i].dagNode));
	      n->argArray[i].multiplicity = d->argArray[i].multiplicity;
	    }
	  return n;
        }
    }
  return original;  // can use the original dag node as the canonical version
}

DagNode*
ACU_Symbol::makeCanonicalCopy(DagNode* original, HashConsSet* hcs)
{
  //
  //	We have a unreduced node - copy forced.
  //
  if (safeCast(ACU_BaseDagNode*, original)->isTree())
    {
      //
      //	Never use tree form as canonical for unreduced dag.
      //
      const ACU_TreeDagNode* d = safeCast(const ACU_TreeDagNode*, original);
      ACU_DagNode* n = new ACU_DagNode(this, d->tree.getSize(), ACU_BaseDagNode::ASSIGNMENT);
      n->copySetRewritingFlags(original);
      n->setSortIndex(original->getSortIndex());
      ArgVec<ACU_DagNode::Pair>::iterator j = n->argArray.begin();
      for (ACU_FastIter i(d->tree); i.valid(); i.next(), ++j)
	{
	  j->dagNode = hcs->getCanonical(hcs->insert(i.getDagNode()));
	  j->multiplicity = i.getMultiplicity();
	}
      return n;
    }

  const ACU_DagNode* d = safeCast(const ACU_DagNode*, original);
  int nrArgs = d->argArray.size();
  ACU_DagNode* n = new ACU_DagNode(this, nrArgs, ACU_BaseDagNode::ASSIGNMENT);
  n->copySetRewritingFlags(original);
  n->setSortIndex(original->getSortIndex());
  for (int i = 0; i < nrArgs; i++)
    {
      n->argArray[i].dagNode = hcs->getCanonical(hcs->insert(d->argArray[i].dagNode));
      n->argArray[i].multiplicity = d->argArray[i].multiplicity;
    }
  return n;
}
