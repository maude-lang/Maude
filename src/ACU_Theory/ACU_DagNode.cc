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
//      Implementation for class ACU_DagNode.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "ACU_Theory.hh"
#include "ACU_Persistent.hh"

//      interface class definitions
#include "term.hh"

//      ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_DagArgumentIterator.hh"
#include "ACU_ExtensionInfo.hh"
#include "ACU_Subproblem.hh"

//	ACU Red-Black class definitions
#include "ACU_TreeDagNode.hh"
#include "ACU_FastIter.hh"

//	our stuff
#include "ACU_Normalize.cc"
#include "ACU_FastMerge.cc"
#include "ACU_MergeSort.cc"
#include "ACU_DagOperations.cc"

Vector<int> ACU_DagNode::runsBuffer(INITIAL_RUNS_BUFFER_SIZE);

ACU_DagNode*
getACU_DagNode(DagNode* dagNode)
{
  ACU_BaseDagNode* d = safeCast(ACU_BaseDagNode*, dagNode);
  if (d->isTree())
    return ACU_TreeDagNode::treeToArgVec(safeCast(ACU_TreeDagNode*, d));
  return safeCast(ACU_DagNode*, d);
}

RawDagArgumentIterator*
ACU_DagNode::arguments()
{
  return new ACU_DagArgumentIterator(&argArray);
}

size_t
ACU_DagNode::getHashValue()
{
  size_t hashValue = symbol()->getHashValue();
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; i++)  // user iterators?
    hashValue = hash(hashValue, argArray[i].dagNode->getHashValue(), argArray[i].multiplicity);
  return hashValue;
}

int
ACU_DagNode::compareArguments(const DagNode* other) const
{
  int len = argArray.length();
  const ACU_BaseDagNode* d = safeCast(const ACU_BaseDagNode*, other);
  if (d->isTree())
    {
      const ACU_TreeDagNode* d2 = safeCast(const ACU_TreeDagNode*, d);
      int r = len - d2->getTree().getSize();
      if (r != 0)
	return r;
      ACU_FastIter j(d2->getTree());
      ArgVec<Pair>::const_iterator i = argArray.begin();
      const ArgVec<Pair>::const_iterator e = argArray.end();
      do
	{
	  r = i->multiplicity - j.getMultiplicity();
	  if (r != 0)
	    return r;
	  r = i->dagNode->compare(j.getDagNode());
	  if (r != 0)
	    return r;
	  j.next();
	  ++i;
	}
      while (i != e);
      Assert(!j.valid(), "iterator problem");
    }
  else
    {
      const ArgVec<ACU_DagNode::Pair>& argArray2 =
	safeCast(const ACU_DagNode*, d)->argArray;
      int r = len - argArray2.length();
      if (r != 0)
	return r;
      ArgVec<ACU_DagNode::Pair>::const_iterator j = argArray2.begin();
      ArgVec<Pair>::const_iterator i = argArray.begin();
      const ArgVec<Pair>::const_iterator e = argArray.end();
      do
	{
	  r = i->multiplicity - j->multiplicity;
	  if (r != 0)
	    return r;
	  r = i->dagNode->compare(j->dagNode);
	  if (r != 0)
	    return r;
	  ++j;
	  ++i;
	}
      while (i != e);
      Assert(j == argArray2.end(), "iterator problem");
    }
  return 0;
}

DagNode*
ACU_DagNode::markArguments()
{
  argArray.evacuate();
  Assert(argArray.length() > 0, "no arguments");
  //
  //	We avoid recursing on the first subterm that shares our symbol.
  //
  Symbol* s = symbol();
  DagNode* r = 0;
  FOR_EACH_CONST(i, ArgVec<Pair>, argArray)
    {
      DagNode* d = i->dagNode;
      if (r == 0 && d->symbol() == s)
	r = d;
      else
	d->mark();
    }
  return r;
}

DagNode*
ACU_DagNode::copyEagerUptoReduced2()
{
  // cerr << "copyEagerUptoReduced2() " << this << endl;
  int nrArgs = argArray.length();
  ACU_Symbol* s = symbol();
  ACU_DagNode* n = new ACU_DagNode(s, nrArgs);
  if (s->getPermuteStrategy() == BinarySymbol::EAGER)
    {
      for (int i = 0; i < nrArgs; i++)
	{
	  n->argArray[i].dagNode = argArray[i].dagNode->copyEagerUptoReduced();
	  n->argArray[i].multiplicity = argArray[i].multiplicity;
	}
    }
  else
    {
      for (int i = 0; i < nrArgs; i++)
	n->argArray[i] = argArray[i];
    }
  return n;
}

void
ACU_DagNode::clearCopyPointers2()
{
  if (symbol()->getPermuteStrategy() == BinarySymbol::EAGER)
    {
      int nrArgs = argArray.length();
      for (int i = 0; i < nrArgs; i++)
	argArray[i].dagNode->clearCopyPointers();
    }
}

void
ACU_DagNode::overwriteWithClone(DagNode* old)
{
  int nrArgs = argArray.length();
  ACU_DagNode* d = new(old) ACU_DagNode(symbol(), nrArgs);
  d->copySetRewritingFlags(this);
  d->setTheoryByte(getTheoryByte());
  d->setSortIndex(getSortIndex());
  fastCopy(argArray.begin(), argArray.end(), d->argArray.begin());
}

DagNode*
ACU_DagNode::makeClone()
{
  int nrArgs = argArray.length();
  ACU_DagNode* d = new ACU_DagNode(symbol(), nrArgs);
  d->copySetRewritingFlags(this);
  d->setTheoryByte(getTheoryByte());
  d->setSortIndex(getSortIndex());
  fastCopy(argArray.begin(), argArray.end(), d->argArray.begin());
  return d;
}

DagNode*
ACU_DagNode::copyWithReplacement(int argIndex, DagNode* replacement)
{
  int nrArgs = argArray.length();
  ACU_Symbol* s = symbol();
  ACU_DagNode* n = new ACU_DagNode(s, nrArgs);
  ArgVec<ACU_DagNode::Pair>& args2 = n->argArray;
  int p = 0;

  for (int i = 0; i < nrArgs; i++)
    {
      if (i != argIndex)
	args2[p++] = argArray[i];
      else
	{
	  int m = argArray[i].multiplicity - 1;
	  if (m > 0)
	    {
	      args2[p].dagNode = argArray[i].dagNode;
	      args2[p].multiplicity = m;
	      ++p;
	    }
	}
    }
  Assert(p >= 1, "no arguments left");
  args2.contractTo(p);
  args2.expandBy(1);
  args2[p].dagNode = replacement;
  args2[p].multiplicity = 1;
  return n;
}

DagNode*
ACU_DagNode::copyWithReplacement(Vector<RedexPosition>& redexStack,
				 int first,
				 int last)
{
  //
  //	We need to build a copy of our dag node, replacing those
  //	arguments that were stacked with those on the stack between
  //	first and last. Unfortunately, unstackable flags are not
  //	a reliable guide to which arguments were stacked since they
  //	may have become true since the arguments were stacked.
  //
  int nrArgs = argArray.length();
  //
  //	The first task is to calculate the number of physical arguments
  //	in the new dag node.
  //
  int nrNewArgs = 0;
  {
    int j = first;
    int argNr = 0;  // notional argument number
    int nextReplacementIndex = redexStack[j].argIndex();
    for (int i = 0; i < nrArgs; i++)
      {
	int m = argArray[i].multiplicity;
	//
	//	We figure out if this block of m identical arguments was
	//	stacked by checking if our notional arg number matches
	//	the next stacked arg number.
	//
	if (argNr == nextReplacementIndex)
	  {
	    nrNewArgs += m;  // stacked so each will need a slot in new arg array
	    j += m;
	    if (j <= last)
	      nextReplacementIndex = redexStack[j].argIndex();
	    else
	      {
		//
		//	No more stacked arguments; each remaining physical
		//	argument will use one slot in new arg array.
		//
		nrNewArgs += nrArgs - (i + 1);
		break;
	      }
	  }
	else
	  ++nrNewArgs;  // not stacked so they will use a single slot in new arg array
	argNr += m;
      }
  }
  //
  //	Now make the new node.
  //
  ACU_DagNode* n = new ACU_DagNode(symbol(), nrNewArgs);
  {
    ArgVec<ACU_DagNode::Pair>& args = n->argArray;
    int j = first;
    int k = 0;  // new physical arg number
    int argNr = 0;  // notional argument number
    int nextReplacementIndex = redexStack[j].argIndex();
    for (int i = 0; i < nrArgs; i++)
      {
	int m = argArray[i].multiplicity;
	if (argNr == nextReplacementIndex)
	  {
	    for (int n = 0; n < m ; n++)
	      {
		args[k].dagNode = redexStack[j].node();
		args[k].multiplicity = 1;
		++j;
		++k;
	      }
	    nextReplacementIndex = (j <= last) ?
	      redexStack[j].argIndex() : NONE;
	  }
	else
	  {
	    args[k].dagNode = argArray[i].dagNode;
	    args[k].multiplicity = m;
	    ++k;
	  }
	argNr += m;
      }
  }
  return n;
}

void
ACU_DagNode::stackArguments(Vector<RedexPosition>& stack,
			    int parentIndex,
			    bool respectFrozen)
{
  if (respectFrozen && !(symbol()->getFrozen().empty()))
    return;
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; i++)
    {
      DagNode* d = argArray[i].dagNode;
      if (!(d->isUnstackable()))
	stack.append(RedexPosition(d, parentIndex, i));
    }
}

void
ACU_DagNode::partialReplace(DagNode* replacement, ExtensionInfo* extensionInfo)
{
  ACU_ExtensionInfo* e = safeCast(ACU_ExtensionInfo*, extensionInfo);
  DagNode* unmatched = e->buildUnmatchedPortion();
  argArray.resizeWithoutPreservation(2);
  argArray[0].set(unmatched, 1);
  argArray[1].set(replacement, 1);
  setNormalizationStatus(FRESH);
  repudiateSortInfo();
}

DagNode*
ACU_DagNode::partialConstruct(DagNode* replacement, ExtensionInfo* extensionInfo)
{
  ACU_DagNode* n = new ACU_DagNode(symbol(), 2);
  ACU_ExtensionInfo* e = safeCast(ACU_ExtensionInfo*, extensionInfo);
  n->argArray[0].set(e->buildUnmatchedPortion(), 1);
  n->argArray[1].set(replacement, 1);
  return n;
}

bool
ACU_DagNode::matchVariableWithExtension(int index,
					const Sort* sort,
					Substitution& /* solution */,
					Subproblem*& returnedSubproblem,
					ExtensionInfo* extensionInfo)
{
  int bound = symbol()->sortBound(sort);
  if (bound == 1)
    return false;
  //
  //	This code could be much more sophisticated: in particular we look for
  //	the subject having total subterm multiplicity of 2 and return unique solution.
  //
  //	There is also the possibility that our variable could be bound; in this case
  //	we could avoid generating and solving the subproblem. But this case is so
  //	rare because the extension means we matching at the top, the binding would
  //	have had to be made by some external agency (e.g. a meta level). 
  //	The extra code needed to do the optimal thing with extension is not
  //	worth the performance gain.
  //
  //	May use runsBuffer
  //
  ACU_ExtensionInfo* e = safeCast(ACU_ExtensionInfo*, extensionInfo);
  int totalSubjectMultiplicity = 0;
  int nrArgs = argArray.length();
  Vector<int> currentMultiplicity(nrArgs);
  for (int i = 0; i < nrArgs; i++)
    {
      int m = argArray[i].multiplicity;
      currentMultiplicity[i] = m;
      totalSubjectMultiplicity += m;
    }
  e->setUpperBound(totalSubjectMultiplicity - 2);

  ACU_Subproblem* subproblem = new ACU_Subproblem(this, e);
  subproblem->addSubjects(currentMultiplicity);
  subproblem->addTopVariable(index, 1, 2, bound, sort);
  returnedSubproblem = subproblem;
  extensionInfo->setValidAfterMatch(false);
  return true;
}

int
ACU_DagNode::argVecComputeBaseSort() const
{
  ACU_Symbol* s = symbol();
  if (const Sort* uniSort = s->uniformSort())
    {
      //
      //	If symbol has a uniform sort structure do a fast sort computation.
      //
      if (!(uniSort->component()->errorFree()))
	{
	  int lastIndex = Sort::SORT_UNKNOWN;
	  const ArgVec<Pair>::const_iterator e = argArray.end();
	  for (ArgVec<Pair>::const_iterator i = argArray.begin(); i != e; ++i)
	    {
	      int index = i->dagNode->getSortIndex();
	      Assert(index >= Sort::ERROR_SORT, "bad sort index");
	      if (index != lastIndex)
		{
		  if (!(::leq(index, uniSort)))
		    return Sort::ERROR_SORT;
		  lastIndex = index;
		}
	    }
	}
      return uniSort->index();
    }
  //
  //	Standard sort calculation.
  //
  int sortIndex;
  ArgVec<Pair>::const_iterator i = argArray.begin();
  const ArgVec<Pair>::const_iterator e = argArray.end();
  {
    int index = i->dagNode->getSortIndex();
    Assert(index >= Sort::ERROR_SORT, "bad sort index");
    sortIndex = s->computeMultSortIndex(index, index, i->multiplicity - 1);
  }
  for (++i; i != e; ++i)
    {
      int index = i->dagNode->getSortIndex();
      Assert(index >= Sort::ERROR_SORT, "bad sort index");
      sortIndex = s->computeMultSortIndex(sortIndex, index, i->multiplicity);
    }
  return sortIndex;
}
