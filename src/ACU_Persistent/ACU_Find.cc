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
//	Find functions for ACU red-black tree argument lists.
//

bool
ACU_RedBlackNode::find(ACU_RedBlackNode* root,
		       DagNode* dagNode,
		       ACU_Stack& path)
{
  do
    {
      path.push(root);
      int r = dagNode->compare(root->getDagNode());
      if (r == 0)
	return true;
      root = root->getChild(r);
    }
  while (root != 0);
  return false;
}

bool
ACU_RedBlackNode::find(ACU_RedBlackNode* root,
		       Term* term,
		       ACU_Stack& path)
{
  do
    {
      path.push(root);
      int r = term->compare(root->getDagNode());
      if (r == 0)
	return true;
      root = root->getChild(r);
    }
  while (root != 0);
  return false;
}

bool
ACU_RedBlackNode::findFirstPotentialMatch(ACU_RedBlackNode* root,
					  Term* key,
					  const Substitution& partial,
					  ACU_Stack& path)
{
  int explore = 0;
  do
    {
      path.push(root);
      ++explore;
      int r = key->partialCompare(partial, root->getDagNode());
      switch (r)
	{
	case Term::GREATER:
	  {
	    root = root->getRight();
	    break;
	  }
	case Term::EQUAL:
	  {
	    return true;
	  }
	case Term::LESS:
	  {
	    root = root->getLeft();
	    break;
	  }
	case Term::UNKNOWN:
	  {
	    explore = 0;  // so we can retreat to this node
	    root = root->getLeft();
	    break;
	  }
	}
    }
  while (root != 0);
  path.multiPop(explore);  // retreat the last place we zero'd explore
  return !path.empty();
}

bool
ACU_RedBlackNode::findGeqMult(ACU_RedBlackNode* root,
			      int multiplicity,
			      ACU_Stack& path)
{
  if (root->getMaxMult() >= multiplicity)
    {
      for(;;)
	{
	  path.push(root);
	  ACU_RedBlackNode* l = root->getLeft();
	  if (l != 0 && l->getMaxMult() >= multiplicity)
	    root = l;
	  else if (root->getMultiplicity() >= multiplicity)
	    break;
	  else
	    {
	      root = root->getRight();
	      Assert(root != 0 && root->getMaxMult() >= multiplicity,
		     "maxMult invariant failed");
	    }
	}
      return true;
    }
  return false;
}
