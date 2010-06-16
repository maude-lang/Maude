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
//	Implementation for class ACU_TreeDagNode.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "ACU_Persistent.hh"
#include "ACU_Theory.hh"

//	ACU Persistent class definitions
#include "ACU_FastIter.hh"

//      ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_ExtensionInfo.hh"
#include "ACU_TreeDagArgumentIterator.hh"
#include "ACU_TreeDagNode.hh"

RawDagArgumentIterator*
ACU_TreeDagNode::arguments()
{
  return new ACU_TreeDagArgumentIterator(tree);
}

size_t
ACU_TreeDagNode::getHashValue()
{
  if (isHashValid())
    return hashCache;
  size_t hashValue = symbol()->getHashValue();
  for (ACU_FastIter i(tree); i.valid(); i.next())
    hashValue = hash(hashValue, i.getDagNode()->getHashValue(), i.getMultiplicity());
  hashCache = hashValue;
  setHashValid();
  return hashValue;
}

int 
ACU_TreeDagNode::compareArguments(const DagNode* other) const
{
  const ACU_BaseDagNode* d = safeCast(const ACU_BaseDagNode*, other);
  if (d->isTree())
    {
      const ACU_TreeDagNode* d2 = safeCast(const ACU_TreeDagNode*, d);
      int r = tree.getSize() - d2->tree.getSize();
      if (r != 0)
	return r;
      ACU_FastIter i(tree);
      ACU_FastIter j(d2->tree);
      do
	{
	  r = i.getMultiplicity() - j.getMultiplicity();
	  if (r != 0)
	    return r;
	  r = i.getDagNode()->compare(j.getDagNode());
	  if (r != 0)
	    return r;
	  j.next();
	  i.next();
	}
      while (i.valid());
      return 0;
    }
  return - other->compareArguments(this);
}

void
ACU_TreeDagNode::overwriteWithClone(DagNode* old)
{
  ACU_TreeDagNode* d = new(old) ACU_TreeDagNode(symbol(), tree);
  d->copySetRewritingFlags(this);
  d->setTheoryByte(getTheoryByte());
  d->setSortIndex(getSortIndex());
}

DagNode* 
ACU_TreeDagNode::makeClone()
{
  ACU_TreeDagNode* d = new ACU_TreeDagNode(symbol(), tree);
  d->copySetRewritingFlags(this);
  d->setTheoryByte(getTheoryByte());
  d->setSortIndex(getSortIndex());
  return d;
}

DagNode*
ACU_TreeDagNode::copyWithReplacement(int argIndex, DagNode* replacement)
{
  return treeToArgVec(this)->copyWithReplacement(argIndex, replacement);  // HACK
}

DagNode*
ACU_TreeDagNode::copyWithReplacement(Vector<RedexPosition>& redexStack,
				     int first,
				     int last)
{
  return treeToArgVec(this)->copyWithReplacement(redexStack, first, last);
}

void
ACU_TreeDagNode::stackArguments(Vector<RedexPosition>& stack,
				int parentIndex,
				bool respectFrozen)
{
  if (respectFrozen && !(symbol()->getFrozen().empty()))
    return;
  int j = 0;
  for (ACU_FastIter i(tree); i.valid(); i.next(), ++j)
    {
      DagNode* d = i.getDagNode();
      if (!(d->isUnstackable()))
	stack.append(RedexPosition(d, parentIndex, j));
    }
}

bool
ACU_TreeDagNode::matchVariableWithExtension(int index,
				const Sort* sort,
				Substitution& solution,
				Subproblem*& returnedSubproblem,
				ExtensionInfo* extensionInfo)
{
  return treeToArgVec(this)->matchVariableWithExtension(index,
							sort,
							solution,
							returnedSubproblem,
							extensionInfo);
}

void
ACU_TreeDagNode::partialReplace(DagNode* replacement, ExtensionInfo* extensionInfo)
{
  ArgVec<ACU_DagNode::Pair>& args = (new(this) ACU_DagNode(symbol(), 2))->argArray;
  args[0].dagNode = safeCast(ACU_ExtensionInfo*, extensionInfo)->getUnmatched();
  args[0].multiplicity = 1;
  args[1].dagNode = replacement;
  args[1].multiplicity = 1;
}

DagNode*
ACU_TreeDagNode::partialConstruct(DagNode* replacement, ExtensionInfo* extensionInfo)
{
  ACU_DagNode* n = new ACU_DagNode(symbol(), 2);
  ArgVec<ACU_DagNode::Pair>& args = n->argArray;
  args[0].dagNode = safeCast(ACU_ExtensionInfo*, extensionInfo)->getUnmatched();
  args[0].multiplicity = 1;
  args[1].dagNode = replacement;
  args[1].multiplicity = 1;
  return n;
}

DagNode*
ACU_TreeDagNode::markArguments()
{
  tree.mark();
  return 0;
}

DagNode*
ACU_TreeDagNode::copyEagerUptoReduced2()
{
  ACU_Symbol* s = symbol();
  return (s->getPermuteStrategy() == BinarySymbol::EAGER) ?
    treeToArgVec(this)->copyEagerUptoReduced2() :
    new ACU_TreeDagNode(s, tree);
}

void
ACU_TreeDagNode::clearCopyPointers2()
{
  Assert(symbol()->getPermuteStrategy() != BinarySymbol::EAGER,
	 "should not be copying ACU_TreeDagNode with eager symbol");
}

ACU_DagNode*
ACU_TreeDagNode::treeToArgVec(ACU_TreeDagNode* original)
{
  // cerr << "in: " << original << endl;

  ACU_Symbol* s = original->symbol();
  ACU_Tree t = original->tree;  // deep copy
  int sortIndex = original->getSortIndex();
  bool redFlag = original->isReduced();
  //
  //	Now we overwrite original.
  //
  ACU_DagNode* d = new(original) ACU_DagNode(s, t.getSize(), ASSIGNMENT);
  ArgVec<ACU_DagNode::Pair>::iterator j = d->argArray.begin();
  
  for (ACU_FastIter i(t); i.valid(); i.next(), ++j)
    {
      j->dagNode = i.getDagNode();
      j->multiplicity = i.getMultiplicity();
    }
  
  Assert(j == d->argArray.end(), "iterators inconsistant");
  
  //cerr << "out: " << d << endl;

  d->setSortIndex(sortIndex);
  if (redFlag)
    d->setReduced();

  return d;
}

//
//	Narrowing code.
//

bool
ACU_TreeDagNode::indexVariables2(NarrowingVariableInfo& indices, int baseIndex)
{
  //
  //	Just revert to argvec ACU representation and use its code.
  //
  return treeToArgVec(this)->indexVariables2(indices, baseIndex);
}

//
//	Hash cons code.
//

ACU_TreeDagNode*
ACU_TreeDagNode::makeCanonical(HashConsSet* hcs)
{
  ACU_Tree canonical;
  if (tree.makeCanonical(canonical, hcs))
    {
      ACU_TreeDagNode* d = new ACU_TreeDagNode(symbol(), canonical);
      d->copySetRewritingFlags(this);
      d->setTheoryByte(getTheoryByte());
      d->setSortIndex(getSortIndex());
      return d;
    }
  return this;
}
