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

//	ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_TreeDagNode.hh"
#include "ACU_Term.hh"
#include "ACU_ExtensionInfo.hh"

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
      leftIdentitySortCheck();
      rightIdentitySortCheck();
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
			   int parentIndex)
{
  if (!(getFrozen().empty()))
    return;
  bool eager = (getPermuteStrategy() == EAGER);
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
