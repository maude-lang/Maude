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
//      Implementation for class ACU_RedBlackNode.
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
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "hashConsSet.hh"

//	ACU red-black class definitions
#include "ACU_RedBlackNode.hh"
#include "ACU_Stack.hh"

//	our parts
#include "ACU_Insert.cc"
#include "ACU_Delete.cc"
#include "ACU_Rebalance.cc"
#include "ACU_Copy.cc"
#include "ACU_Find.cc"

ACU_RedBlackNode*
ACU_RedBlackNode::blackNode(ACU_RedBlackNode* key,
			    ACU_RedBlackNode* left,
			    ACU_RedBlackNode* right)
{
  return new ACU_RedBlackNode(key->getDagNode(),
			      key->getMultiplicity(),
			      left,
			      right);
}

//
//	Experimental hash consing code
//
ACU_RedBlackNode*
ACU_RedBlackNode::canonicalRebuild(HashConsSet* hcs)
{
  //
  //	Horribly inefficient - recode it after we determine it is worthwhile.
  //
  bool needRebuild = false;
  ACU_RedBlackNode* left = getLeft();
  if (left)
    {
      ACU_RedBlackNode* canonicalLeft = left->canonicalRebuild(hcs);
      if (left != canonicalLeft)
	{
	  left = canonicalLeft;
	  needRebuild = true;
	}
    }
  ACU_RedBlackNode* right = getRight();
  if (right)
    {
      ACU_RedBlackNode* canonicalRight = right->canonicalRebuild(hcs);
      if (right != canonicalRight)
	{
	  right = canonicalRight;
	  needRebuild = true;
	}
    }
  DagNode* d = getDagNode();
  DagNode* c = hcs->getCanonical(hcs->insert(d));
  if (needRebuild || c != d)
    {
      ACU_RedBlackNode* n = new ACU_RedBlackNode(c, getMultiplicity(), left, right, getMaxMult());
      n->makeRedIfRed(this);
      n->setSortIndex(getSortIndex());
      return n;
    }
  return this;
}

#ifdef CHECK_TREE

bool
ACU_RedBlackNode::checkRedBlackProperty()
{
  int height;
  if (!checkRedBlackProperty(height) || isRed())
    {
      DebugAlways("Red-Black Violation\n");
      dump(cerr, 0);
      return false;
    }
  return true;
}

bool
ACU_RedBlackNode::checkRedBlackProperty(int& height)
{
  bool ok = true;
  height = 0;
  if (ACU_RedBlackNode* l = getLeft())
    {
      if (l->isRed() && isRed())
	{
	  DebugAlways("left child of red not is red");
	  ok = false;
	}
      ok = l->checkRedBlackProperty(height);
      if (getDagNode()->compare(l->getDagNode()) <= 0)
	{
	  DebugAlways("order violation " << getDagNode() <<
		      " <= " << l->getDagNode());
	  ok = false;
	}
    }
  int rHeight = 0;
  if (ACU_RedBlackNode* r = getRight())
    {
      if (r->isRed() && isRed())
	{
	  DebugAlways("right child of red not is red");
	  ok = false;
	}
      ok = r->checkRedBlackProperty(rHeight) && ok;
      if (getDagNode()->compare(r->getDagNode()) >= 0)
	{
	  DebugAlways("order violation " << getDagNode() <<
		      " >= " << r->getDagNode());
	  ok = false;
	}
    }
  if (height != rHeight)
    {
      DebugAlways("black height violation " << height << " != " << rHeight);
      ok = false;
    }
  if (!isRed())
    ++height;
  return ok;
}

#endif

void
ACU_RedBlackNode::dump(ostream& s, int indent)
{
  s << Indent(indent) <<
    "address = " << this <<
    "\tdagNode = " << getDagNode() <<
    "\tmultiplicity = " << getMultiplicity() <<
    "\tcolor = " << (isRed() ? "red" : "black") << endl;

  s << Indent(indent) << "left = ";
  if (ACU_RedBlackNode* l = getLeft())
    {
      s << '\n';
      l->dump(s, indent + 1);
    }
  else
    s << "null\n";

  s << Indent(indent) << "right = ";
  if (ACU_RedBlackNode* r = getRight())
    {
      s << '\n';
      r->dump(s, indent + 1);
    }
  else
    s << "null\n";
}
