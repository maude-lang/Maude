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
//      Implementation for class ACU_Tree.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "ACU_RedBlack.hh"

//	interface class definitions
#include "binarySymbol.hh"
#include "dagNode.hh"
#include "term.hh"

//	ACU red-black class definitions
#include "ACU_RedBlackNode.hh"
#include "ACU_Stack.hh"
#include "ACU_Tree.hh"

ACU_Tree::ACU_Tree(const ArgVec<ACU_Pair>& source)
{
  size = source.length();
  root = makeTree(source, 0, size, false);
  
#ifdef CHECK_TREE
  checkIntegrity();
#endif
}

local_inline bool
ACU_Tree::pow2min1(int i)
{
  //
  //	Return true iff i is 2^n - 1 for some n >= 0.
  //
  return ((i + 1) & i) == 0;
}

ACU_RedBlackNode* 
ACU_Tree::makeTree(const ArgVec<ACU_Pair>& args,
		   int first,
		   int size,
		   bool makeRed)
{
  if (size == 0)
    return 0;
  int leftSize = size / 2;
  int rightSize = size - 1 - leftSize;
  ACU_RedBlackNode* leftTree = makeTree(args, first, leftSize,
					leftSize > rightSize && pow2min1(leftSize));
  ACU_RedBlackNode* rightTree = makeTree(args, first + leftSize + 1, rightSize, false);
  ACU_RedBlackNode* t = new ACU_RedBlackNode(args[first + leftSize].dagNode,
					     args[first + leftSize].multiplicity,
					     leftTree,
					     rightTree);
  if (makeRed)
    t->makeRed();
  return t;
}

/*
void
ACU_Tree::copyToArgVec(ArgVec<ACU_Pair>& dest) const
{
  ArgVec<ACU_DagNode::Pair>::iterator j = dest->argArray.begin();
  for (ACU_FastIter i(t); i.valid(); i.next(), ++j)
    {
      j->dagNode = i.getDagNode();
      j->multiplicity = i.getMultiplicity();
    }
}
*/

int
ACU_Tree::computeBaseSort2(BinarySymbol* symbol, ACU_RedBlackNode* root)
{
  //
  //	Postorder traversal of node is red-black tree without a sort.
  //
  ACU_Stack s;
  ACU_RedBlackNode* n;

  for(;;)
    {
      n = root->getLeft();
      if (n == 0 || n->getSortIndex() != Sort::SORT_UNKNOWN)
	{
	tryRight:
	  n = root->getRight();
	  if (n == 0 || n->getSortIndex() != Sort::SORT_UNKNOWN)
	    break;
	}
      s.push(root);
      root = n;
    }

  int f = root->getDagNode()->getSortIndex();
  f = symbol->computeMultSortIndex(f, f, root->getMultiplicity() - 1);
  n = root->getLeft();
  if (n != 0)
    f = symbol->computeSortIndex(f, n->getSortIndex());
  n = root->getRight();
  if (n != 0)
    f = symbol->computeSortIndex(f, n->getSortIndex());
  root->setSortIndex(f);

  while (!(s.empty()))
    {
      ACU_RedBlackNode* oldRoot = root;
      root = s.pop();
      n = root->getLeft();
      if (n == oldRoot)
	goto tryRight;
      if (n != 0)
	f = symbol->computeSortIndex(f, n->getSortIndex());
      f = symbol->computeMultSortIndex(f,
				       root->getDagNode()->getSortIndex(),
				       root->getMultiplicity());
      root->setSortIndex(f);
    }

  return f;
}

void
ACU_Tree::mark()
{
  //
  //	Preorder traversal of unmarked nodes in red-black tree.
  //
  ACU_Stack i;
  ACU_RedBlackNode* n = root;
  for(;;)
    {
      while (n != 0 && !(n->isMarked()))
	{
	  n->setMarked();
	  n->getDagNode()->mark();
	  i.push(n);
	  n = n->getLeft();
	}
      if (i.empty())
	break;
      n = i.pop()->getRight();
    }
}

bool
ACU_Tree::makeCanonical(ACU_Tree& canonizedVersion, HashConsSet* hcs)
{
  ACU_RedBlackNode* c = root->canonicalRebuild(hcs);
  if (c != root)
    {
      canonizedVersion.size = size;
      canonizedVersion.root = c;
      return true;
    }
  return false;
}

#ifdef CHECK_TREE

void
ACU_Tree::checkIntegrity(DagNode* dagNode, int multiplicity) const
{
  //
  //	should also check size
  //
  if (root != 0 && !(root->checkRedBlackProperty()))
    {
      if (dagNode != 0)
	{
	  cerr << "dagNode = " << dagNode <<
	    "\t multiplcity = " << multiplicity << endl;
	}
      root->dump(cerr, 0);
      abort();
    }
}

#endif
