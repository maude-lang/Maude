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
//      Implementation for class S_Symbol
//
#include <map>

//	utility stuff
#include "macros.hh"
#include "vector.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "S_Theory.hh"

//      core class definitions
#include "hashConsSet.hh"

//	successor theory class definitions
#include "S_Symbol.hh"
#include "S_DagNode.hh"
#include "S_Term.hh"
#include "S_ExtensionInfo.hh"

S_Symbol::S_Symbol(int id, const Vector<int>& strategy, bool memoFlag)
  : Symbol(id, 1, memoFlag)
{
  setStrategy(strategy, 1, memoFlag);
}

void 
S_Symbol::compileOpDeclarations()
{
  Symbol::compileOpDeclarations();  // do default sort processing
  const ConnectedComponent* kind = rangeComponent();
  int nrSorts =  kind->nrSorts();
  sortPathTable.resize(nrSorts);
  for (int i = 0; i < nrSorts; i++)
    computePath(i, sortPathTable[i]);
}

void 
S_Symbol::finalizeSortInfo()
{
  Symbol::finalizeSortInfo();  // do parent classes stuff
  WarningCheck(kindLevelDeclarationsOnly() || getSortConstraints().empty(),
	       "membership axioms are not guaranteed to work correctly for iterated symbol " <<
	       QUOTE(this) << " as it has declarations that are not at the kind level.");
}

void 
S_Symbol::computePath(int sortIndex, SortPath& path)
{
  path.nonCtorBound = NONE;
  bool needToLookForNonCtor = false;
  switch (getCtorStatus())
    {
    case SortTable::IS_NON_CTOR:
      {
	path.nonCtorBound = 0;
	break;
      }
    case SortTable::IS_COMPLEX:
      {
	needToLookForNonCtor = true;
	break;
      }
    }

  map<int, int> seen;
  Vector<int> indices;
  for (;;)
    {
      if (needToLookForNonCtor && !(ctorTraverse(0, sortIndex)))
	{
	  path.nonCtorBound = indices.length();
	  needToLookForNonCtor = false;
	}
      sortIndex = traverse(0, sortIndex);
      if (seen.find(sortIndex) != seen.end())
	{
	  path.leadLength = seen[sortIndex];
	  break;
	}

      seen[sortIndex] = indices.length();
      indices.append(sortIndex);
    }
  path.sortIndices = indices;
}

DagNode*
S_Symbol::ruleRewrite(DagNode* subject, RewritingContext& context)
{
  S_ExtensionInfo extensionInfo(safeCast(S_DagNode*, subject));
  return applyRules(subject, context, &extensionInfo);
}

Term*
S_Symbol::makeTerm(const Vector<Term*>& args)
{
  Assert(args.length() == 1, "bad number of arguments");
  Assert(args[0] != 0, "null argument");
  return new S_Term(this, 1, args[0]);
}

DagNode*
S_Symbol::makeDagNode(const Vector<DagNode*>& args)
{
  Assert(args.length() == 1, "bad number of arguments");
  Assert(args[0] != 0, "null argument");
  return new S_DagNode(this, 1, args[0]);
}

bool
S_Symbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  S_DagNode* s = safeCast(S_DagNode*, subject);
  if (standardStrategy())
    {
      //
      //	Fast eager strategy case.
      //
      s->arg->reduce(context);
      s->normalizeAtTop();  // always needed because shared node may have rewritten
      if (equationFree())
	return false;
      S_ExtensionInfo extensionInfo(s);
      return applyReplace(subject, context, &extensionInfo);
    }
  if (isMemoized())
    {
      //
      //	Memoized case - get the reduced form and enter
      //	it in the memoization table.
      //
      MemoTable::SourceSet from;
      memoStrategy(from, subject, context);
      memoEnter(from, subject);
      return false;
    }
  //
  //	Complex strategy case.
  //
  S_ExtensionInfo extensionInfo(s);
  const Vector<int>& userStrategy = getStrategy();
  int stratLen = userStrategy.length();
  bool seenZero = false;

  for (int i = 0; i < stratLen; i++)
    {
      if(userStrategy[i] == 0)
	{
	  if (!seenZero)
	    {
	      s->arg->computeTrueSort(context);
	      seenZero = true;
	    }
	  s->normalizeAtTop();
	  if ((i + 1 == stratLen) ?
	      applyReplace(subject, context, &extensionInfo) :
	      applyReplaceNoOwise(subject, context, &extensionInfo))
	      return true;
	}
      else
	{
	  if (seenZero)
	    {
	      s->arg->copyReducible();
	      //
	      //	A previous call to applyReplace() may have
	      //	computed a true sort for our subject which will be
	      //	invalidated by the reduce we are about to do.
	      //
	      subject->repudiateSortInfo();
	    }
	  s->arg->reduce(context);
	}
    }
  return false;
}

void
S_Symbol::memoStrategy(MemoTable::SourceSet& from,
		       DagNode* subject,
		       RewritingContext& context)
{
  S_DagNode* s = safeCast(S_DagNode*, subject);
  //
  //	Execute user supplied strategy.
  //
  const Vector<int>& userStrategy = getStrategy();
  int stratLen = userStrategy.length();
  bool seenZero = false;
  for (int i = 0; i < stratLen; i++)
    {
      if(userStrategy[i] == 0)
	{
	  if (!seenZero)
	    {
	      s->arg->computeTrueSort(context);
	      seenZero = true;
	    }
	  s->normalizeAtTop();
	  if (memoRewrite(from, subject, context))
	    return;
	  S_ExtensionInfo extensionInfo(s);
	  if ((i + 1 == stratLen) ?
	      applyReplace(subject, context, &extensionInfo) :
	      applyReplaceNoOwise(subject, context, &extensionInfo))
	    {
	      subject->reduce(context);
	      return;
	    }
	}
      else
	{
	  if (seenZero)
	    {
	      s->arg->copyReducible();
	      //
	      //	A previous call to applyReplace() may have
	      //	computed a true sort for our subject which will be
	      //	invalidated by the reduce we are about to do.
	      //
	      subject->repudiateSortInfo();
	    }
	  s->arg->reduce(context);
	}
    }
}

void
S_Symbol::computeBaseSort(DagNode* subject)
{
  Assert(this == subject->symbol(), "bad symbol");
  S_DagNode* s = safeCast(S_DagNode*, subject);
  int argSortIndex = s->getArgument()->getSortIndex();
  Assert(argSortIndex != Sort::SORT_UNKNOWN, "unknown sort");
  subject->setSortIndex(sortPathTable[argSortIndex].computeSortIndex(s->getNumber()));
}

void
S_Symbol::fillInSortInfo(Term* subject)
{
  Assert(this == subject->symbol(), "bad symbol");
  S_Term* s = safeCast(S_Term*, subject);
  Term* arg = s->getArgument();
  arg->symbol()->fillInSortInfo(arg);
  subject->setSortInfo(rangeComponent(), sortPathTable[arg->getSortIndex()].computeSortIndex(s->getNumber()));
}

bool
S_Symbol::isConstructor(DagNode* subject)
{
  Assert(this == subject->symbol(), "bad symbol");
  S_DagNode* s = safeCast(S_DagNode*, subject);
  const SortPath& path = sortPathTable[s->arg->getSortIndex()];
  if (path.nonCtorBound == NONE)
    return true;
  const mpz_class& number = *(s->number);
  return number <= path.nonCtorBound;
}

void
S_Symbol::normalizeAndComputeTrueSort(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  S_DagNode* s = safeCast(S_DagNode*, subject);
  s->arg->computeTrueSort(context);
  s->normalizeAtTop();
  fastComputeTrueSort(subject, context);
}

void
S_Symbol::stackArguments(DagNode* subject,
			 Vector<RedexPosition>& stack,
			 int parentIndex)
{
  DagNode* arg = safeCast(S_DagNode*, subject)->arg;
  if (!(getFrozen().contains(0)) && !(arg->isUnstackable()))
    stack.append(RedexPosition(arg, parentIndex, 0, eagerArgument(0)));
}

bool
S_Symbol::mightCollapseToOurSymbol(const Term* subterm) const
{
  const PointerSet& cs = subterm->collapseSymbols();
  int nrSymbols = cs.cardinality();
  for (int i = 0; i < nrSymbols; i++)
    {
      Symbol* s = static_cast<Symbol*>(cs.index2Pointer(i));
      if (static_cast<const Symbol*>(s) == this)
        return true;
      VariableSymbol* vs = dynamic_cast<VariableSymbol*>(s);
      if (vs != 0)  // might want to check that vs has big enough sort
        return true;
    }
  return false;
}

//
//	Unification code.
//

void
S_Symbol::computeSortFunctionBdds(const SortBdds& /* sortBdds */, Vector<Bdd>& /* sortFunctionBdds */) const
{
  //
  //	We don't make use of a precomputed sort function since we need to handle stacks of
  //	symbols efficiently - therefore we don't waste time and space computing one.
  //
}

void
S_Symbol::computeGeneralizedSort(const SortBdds& sortBdds,
				 const Vector<int>& realToBdd,
				 DagNode* subject,
				 Vector<Bdd>& generalizedSort)
{
  Assert(this == subject->symbol(), "bad symbol");
  //
  //	First we compute the generalized sort of our argument.
  //
  S_DagNode* s = safeCast(S_DagNode*, subject);
  DagNode* arg = s->getArgument();
  const mpz_class& number = s->getNumber();
  Vector<Bdd> argGenSort;
  arg->computeGeneralizedSort(sortBdds, realToBdd, argGenSort);
  //
  //	Prepare all false generalized output sort vector.
  //
  Assert(generalizedSort.empty(), "non-empty generalizedSort");
  int nrBits = argGenSort.size();
  generalizedSort.resize(nrBits);
  //
  //	The negation of each input BDD will be used at least once
  //	(otherwise the bit would always be 1 and hence unneeded) and
  //	thus we calculate them in advance.
  //
  Vector<Bdd> negArgGenSort(nrBits);
  for (int i = 0; i < nrBits; ++i)
    negArgGenSort[i] = bdd_not(argGenSort[i]);
  //
  //	Then for each possible value of this sort we compute
  //	the index of the sort produced by our iterated functon symbol.
  //
  int nrSorts = sortPathTable.size();
  for (int i = 0; i < nrSorts; ++i)
    {
      //
      //	equal will hold the BDD that returns true when our argument
      //	has sort index i.
      //
      Bdd equal = bddtrue;
      int inIndex = i;
      for (int j = 0; j < nrBits; ++j, inIndex >>= 1)
	equal = bdd_and(equal, (inIndex & 1) ? argGenSort[j] : negArgGenSort[j]);
      //
      //	We compute the output sort index and OR the equal BDD into each
      //	output BDD whose corrresponding bit is 1 in the output sort index.
      //
      int outIndex = sortPathTable[i].computeSortIndex(number);
      for (int j = 0; j < nrBits; ++j, outIndex >>= 1)
	{
	  if (outIndex & 1)
	    generalizedSort[j] = bdd_or(generalizedSort[j], equal);
	}
    }
}

bool
S_Symbol::isStable() const
{
  return true;
}

//
//	Hash cons code.
//

DagNode*
S_Symbol::makeCanonical(DagNode* original, HashConsSet* hcs)
{
  S_DagNode* s = safeCast(S_DagNode*, original);
  DagNode* d = s->getArgument();
  DagNode* c = hcs->getCanonical(hcs->insert(d));
  if (c == d)
    return original;  // can use the original dag node as the canonical version
  //
  //	Need to make new node.
  //
  S_DagNode* n = new S_DagNode(this, s->getNumber(), c);
  n->copySetRewritingFlags(original);
  n->setSortIndex(original->getSortIndex());
  return n;
}

DagNode*
S_Symbol::makeCanonicalCopy(DagNode* original, HashConsSet* hcs)
{
  //
  //	We have a unreduced node - copy forced.
  //
  S_DagNode* s = safeCast(S_DagNode*, original);
  DagNode* c = hcs->getCanonical(hcs->insert(s->getArgument()));
  S_DagNode* n = new S_DagNode(this, s->getNumber(), c);
  n->copySetRewritingFlags(original);
  n->setSortIndex(original->getSortIndex());
  return n;
}
