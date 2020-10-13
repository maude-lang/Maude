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
//      Implementation for class CUI_Symbol.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "CUI_Theory.hh"
 
//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"
#include "subproblem.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "variableInfo.hh"
#include "sortBdds.hh"
#include "hashConsSet.hh"

//	CUI theory class definitions
#include "CUI_Symbol.hh"
#include "CUI_DagNode.hh"
#include "CUI_Term.hh"
#include "CUI_UnificationSubproblem.hh"
#include "CUI_UnificationSubproblem2.hh"

CUI_Symbol::CUI_Symbol(int id,
		       const Vector<int>& strategy,
		       bool memoFlag,
		       Axioms axioms,
		       Term* identity)
  : BinarySymbol(id, memoFlag, identity),
    axioms(axioms)
{
  if (axioms & COMM)
    setPermuteStrategy(strategy);
  else
    setStrategy(strategy, 2, memoFlag);
}

void 
CUI_Symbol::compileOpDeclarations()
{
  if (axioms & COMM)
    commutativeSortCompletion();
  Symbol::compileOpDeclarations();  // do default sort processing
  if (axioms & IDEM)
    idempotentSortCheck();
}

void 
CUI_Symbol::postOpDeclarationPass()
{
  processIdentity();
  if (axioms & LEFT_ID)
    leftIdentitySortCheck();
  //
  //	If we are commutative, a problem that shows up with rightIdentitySortCheck()
  //	will already have shown up with leftIdentitySortCheck() and the latter is faster.
  //
  if (!(axioms & COMM))
    {
      if (axioms & RIGHT_ID)
	rightIdentitySortCheck();
    }
}

Term*
CUI_Symbol::makeTerm(const Vector<Term*>& args)
{
  return new CUI_Term(this, args);
}

DagNode*
CUI_Symbol::makeDagNode(const Vector<DagNode*>& args)
{
  CUI_DagNode* c = new CUI_DagNode(this);
  c->argArray[0] = args[0];
  c->argArray[1] = args[1];
  return c;
}

bool
CUI_Symbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  CUI_DagNode* s = static_cast<CUI_DagNode*>(subject);
  DagNode** args = s->argArray;
  if (standardStrategy())
    {
      args[0]->reduce(context);
      args[1]->reduce(context);
      if (s->normalizeAtTop())
	return false;
      return !(equationFree()) && applyReplace(s, context);
    }
  else
    {
      if (isMemoized())
	{
	  MemoTable::SourceSet from;
	  bool result = memoStrategy(from, subject, context);
	  memoEnter(from, subject);
	  //
	  //	We may need to return true in the case we collapse to a unreduced subterm.
	  //
	  return result;
	}
      //
      //	Execute user supplied strategy.
      //
      const Vector<int>& userStrategy = getStrategy();
      int stratLen = userStrategy.length();
      bool seenZero = false;
      for (int i = 0; i < stratLen; i++)
        {
          int a = userStrategy[i];
          if(a == 0)
            {
              if (!seenZero)
                {
		  args[0]->computeTrueSort(context);
		  args[1]->computeTrueSort(context);
		  seenZero = true;
		}
	      //
	      //	If we collapse to one of our subterms which has not been
	      //	reduced we pretend that we did a rewrite so that the
	      //	reduction process continues.
	      //
	      if (s->normalizeAtTop())
		return !(s->isReduced());
              if ((i + 1 == stratLen) ? applyReplace(s, context) :
		  applyReplaceNoOwise(s, context))
                return true;
            }
          else
            {
              --a;  // real arguments start at 0 not 1
              if (seenZero)
                {
                  args[a] = args[a]->copyReducible();
                  //
                  //    A previous call to applyReplace() may have
                  //    computed a true sort for our subject which will be
                  //    invalidated by the reduce we are about to do.
                  //
                  s->repudiateSortInfo();
                }
              args[a]->reduce(context);
            }
        }
      return false;
    }
}

bool
CUI_Symbol::memoStrategy(MemoTable::SourceSet& from,
			 DagNode* subject,
			 RewritingContext& context)
{
  CUI_DagNode* s = static_cast<CUI_DagNode*>(subject);
  DagNode** args = s->argArray;
  //
  //	Execute user supplied strategy.
  //
  const Vector<int>& userStrategy = getStrategy();
  int stratLen = userStrategy.length();
  bool seenZero = false;
  for (int i = 0; i < stratLen; i++)
    {
      int a = userStrategy[i];
      if(a == 0)
	{
	  if (!seenZero)
	    {
	      args[0]->computeTrueSort(context);
	      args[1]->computeTrueSort(context);
	      seenZero = true;
	    }
	  //
	  //	If we collapse to one of our subterms which has not been
	  //	reduced we pretend that we did a rewrite so that the
	  //	reduction process continues.
	  //
	  if (s->normalizeAtTop())
	    return !(s->isReduced());
	  if (memoRewrite(from, subject, context))
	    return false;
	  if ((i + 1 == stratLen) ? applyReplace(s, context) :
	      applyReplaceNoOwise(s, context))
	    {
	      subject->reduce(context);
	      return false;
	    }
	}
      else
	{
	  --a;  // real arguments start at 0 not 1
	  if (seenZero)
	    {
	      args[a] = args[a]->copyReducible();
	      //
	      //    A previous call to applyReplace() may have
	      //    computed a true sort for our subject which will be
	      //    invalidated by the reduce we are about to do.
	      //
	      s->repudiateSortInfo();
	    }
	  args[a]->reduce(context);
	}
    }
  return false;
}

void
CUI_Symbol::computeBaseSort(DagNode* subject)
{
  Assert(this == subject->symbol(), "bad symbol");
  DagNode** args = static_cast<CUI_DagNode*>(subject)->argArray;
  int i0 = args[0]->getSortIndex();
  int i1 = args[1]->getSortIndex();
  Assert(i0 >= 0 && i1 >= 0, "bad sort index");
  subject->setSortIndex(traverse(traverse(0, i0), i1));
}

void
CUI_Symbol::normalizeAndComputeTrueSort(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  CUI_DagNode* s = static_cast<CUI_DagNode*>(subject);
  DagNode** args = s->argArray;
  //
  //	Make sure each subterm has a sort.
  //
  args[0]->computeTrueSort(context);
  args[1]->computeTrueSort(context);
  //
  //	Put subject in normal form (could collapse to a subterm).
  //
  if (s->normalizeAtTop())
    return;
  //
  //	Finally compute subjects true sort.
  //
  fastComputeTrueSort(subject, context);
}

void
CUI_Symbol::stackArguments(DagNode* subject,
			   Vector<RedexPosition>& stack,
			   int parentIndex,
			   bool respectFrozen,
			   bool eagerContext)
{
  DagNode** args = safeCast(CUI_DagNode*, subject)->argArray;
  const NatSet& frozen = getFrozen();

  DagNode* d = args[0];
  if (!(respectFrozen && frozen.contains(0)) && !(d->isUnstackable()))
    stack.append(RedexPosition(args[0], parentIndex, 0, eagerContext && eagerArgument(0)));

  d = args[1];
  if (!(respectFrozen && frozen.contains(1)) && !(d->isUnstackable()))
    stack.append(RedexPosition(args[1], parentIndex, 1, eagerContext && eagerArgument(1)));
}

void
CUI_Symbol::setFrozen(const NatSet& frozen)
{
  if (axioms & COMM)
    setPermuteFrozen(frozen);
  else
    BinarySymbol::setFrozen(frozen);
}

Term*
CUI_Symbol::termify(DagNode* dagNode)
{
  Vector<Term*> arguments(2);
  DagNode** argArray = safeCast(CUI_DagNode*, dagNode)->argArray;
  DagNode* d0 = argArray[0];
  DagNode* d1 = argArray[1];
  arguments[0] = d0->symbol()->termify(d0);
  arguments[1] = d1->symbol()->termify(d1);
  return new CUI_Term(this, arguments);
}

//
//	Unification code.
//

void
CUI_Symbol::computeGeneralizedSort(const SortBdds& sortBdds,
				   const Vector<int>& realToBdd,
				   DagNode* subject,
				   Vector<Bdd>& generalizedSort)
{
  DagNode** args = safeCast(CUI_DagNode*, subject)->argArray;
  int varCounter = 0;
  bddPair* argMap = bdd_newpair();
  for (int i = 0; i < 2; i++)
    {
      Vector<Bdd> argGenSort;
      args[i]->computeGeneralizedSort(sortBdds, realToBdd, argGenSort);
      int nrBdds = argGenSort.size();
      for (int j = 0; j < nrBdds; ++j, ++varCounter)
	bdd_setbddpair(argMap, varCounter, argGenSort[j]);
    }
  const Vector<Bdd>& sortFunction = sortBdds.getSortFunction(this);
  int nrBdds = sortFunction.size();
  generalizedSort.resize(nrBdds);
  for (int i = 0; i < nrBdds; ++i)
    generalizedSort[i] = bdd_veccompose(sortFunction[i], argMap);
  bdd_freepair(argMap);
}


// experimental code for faster sort computations
void
CUI_Symbol::computeGeneralizedSort2(const SortBdds& sortBdds,
				    const Vector<int>& realToBdd,
				    DagNode* subject,
				    Vector<Bdd>& outputBdds)
{
  DagNode** args = safeCast(CUI_DagNode*, subject)->argArray;
  Vector<Bdd> inputBdds;
  args[0]->computeGeneralizedSort2(sortBdds, realToBdd, inputBdds);
  args[1]->computeGeneralizedSort2(sortBdds, realToBdd, inputBdds);
  sortBdds.operatorCompose(this, inputBdds, outputBdds);
}

UnificationSubproblem*
CUI_Symbol::makeUnificationSubproblem()
{
  if (leftId() || rightId())
    {
      //
      //	Because CUI_UnificationSubproblem2 may introduce the
      //	identity element we make sure it had its sort computed
      //	and its ground flag set.
      //
      DagNode* id = getIdentityDag();
      if (!(id->isGround()))
	id->computeBaseSortForGroundSubterms(false);
      return new CUI_UnificationSubproblem2();
    }
  return new CUI_UnificationSubproblem();
}

int
CUI_Symbol::unificationPriority() const
{
  if (idem())
    return Symbol::unificationPriority();   // unimplemented
  //
  //	Make a rough guess about how branchy we are.
  //
  return comm() + 2 * (leftId() + rightId());
}

bool
CUI_Symbol::canResolveTheoryClash()
{
  return leftId() || rightId();
}

//
//	Hash cons code.
//

DagNode*
CUI_Symbol::makeCanonical(DagNode* original, HashConsSet* hcs)
{
  DagNode** p = safeCast(CUI_DagNode*, original)->argArray;
  DagNode* d = p[0];
  DagNode* c = hcs->getCanonical(hcs->insert(d));
  DagNode* d1 = p[1];
  DagNode* c1 = hcs->getCanonical(hcs->insert(d1));
  if (c == d && c1 == d1)
    return original;  // can use the original dag node as the canonical version
  //
  //	Need to make new node.
  //
  CUI_DagNode* n = new CUI_DagNode(this);
  n->copySetRewritingFlags(original);
  n->setSortIndex(original->getSortIndex());
  n->argArray[0] = c;
  n->argArray[1] = c1;
  return n;
}

DagNode*
CUI_Symbol::makeCanonicalCopy(DagNode* original, HashConsSet* hcs)
{
  //
  //	We have a unreduced node - copy forced.
  //
  CUI_DagNode* n = new CUI_DagNode(this);
  n->copySetRewritingFlags(original);
  n->setSortIndex(original->getSortIndex());
  DagNode** p = safeCast(CUI_DagNode*, original)->argArray;
  n->argArray[0] = hcs->getCanonical(hcs->insert(p[0]));
  n->argArray[1] = hcs->getCanonical(hcs->insert(p[1]));
  return n;
}
