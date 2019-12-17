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
//	Three fast merge functions:
//	  ArgVec and ArgVec
//	  ArgVec and red-black
//	  red-black and red-black
//

void
ACU_DagNode::fastMerge(const ACU_DagNode* source0,
		       const ACU_DagNode* source1)
{
  //
  //	Merge the argArrays from source0 and source1 into our argArray.
  //
  argArray.resizeWithoutPreservation(source0->argArray.length() +
				     source1->argArray.length());
  
  ArgVec<Pair>::const_iterator s0 = source0->argArray.begin();
  const ArgVec<Pair>::const_iterator e0 = source0->argArray.end();
  ArgVec<Pair>::const_iterator s1 = source1->argArray.begin();
  const ArgVec<Pair>::const_iterator e1 = source1->argArray.end();
  ArgVec<Pair>::iterator d = argArray.begin();
  for(;;)
    {
      int r = s0->dagNode->compare(s1->dagNode);
      if (r < 0)
	{
	  *d = *s0;
	  ++d;
	  ++s0;
	  if (s0 == e0)
	    goto source0Exhausted;
	}
      else if (r > 0)
	{
	  *d = *s1;
	  ++d;
	  ++s1;
	  goto checkSource1;
	}
      else
	{
	  d->dagNode = s0->dagNode;
	  d->multiplicity = s0->multiplicity + s1->multiplicity;
	  ++d;
	  ++s0;
	  ++s1;
	  if (s0 == e0)
	    {
	    source0Exhausted:
	      d = fastCopy(s1, e1, d);
	      break;
	    }
	checkSource1:
	  if (s1 == e1)
	    {
	      d = fastCopy(s0, e0, d);
	      break;
	    }
	}
    }
  argArray.contractTo(d - argArray.begin());
}

void
ACU_DagNode::fastMerge(const ACU_DagNode* source0,
		       const ACU_TreeDagNode* source1)
{
  //
  //	Merge the argArray from source0 with red-black tree from
  //	source1 into our argArray.
  //
  const ACU_Tree& tree1 = source1->getTree();
  argArray.resizeWithoutPreservation(source0->argArray.length() +
				     tree1.getSize());
  
  ArgVec<Pair>::const_iterator s0 = source0->argArray.begin();
  const ArgVec<Pair>::const_iterator e0 = source0->argArray.end();
  ACU_FastIter s1(tree1);
  ArgVec<Pair>::iterator d = argArray.begin();
  for(;;)
    {
      DagNode* d1 = s1.getDagNode();
      int r = s0->dagNode->compare(d1);
      if (r < 0)
	{
	  *d = *s0;
	  ++d;
	  ++s0;
	  if (s0 == e0)
	    goto source0Exhausted;
	}
      else if (r > 0)
	{
	  d->dagNode = d1;
	  d->multiplicity = s1.getMultiplicity();
	  ++d;
	  s1.next();
	  goto checkSource1;
	}
      else
	{
	  d->dagNode = d1;
	  d->multiplicity = s0->multiplicity + s1.getMultiplicity();
	  ++d;
	  ++s0;
	  s1.next();
	  if (s0 == e0)
	    {
	    source0Exhausted:
	      d = fastCopy(s1, d);
	      break;
	    }
	checkSource1:
	  if (!s1.valid())
	    {
	      d = fastCopy(s0, e0, d);
	      break;
	    }
	}
    }
  argArray.contractTo(d - argArray.begin());
}

void
ACU_DagNode::fastMerge(const ACU_TreeDagNode* source0,
		       const ACU_TreeDagNode* source1)
{
  //
  //	Merge the red-black tree from source0 with red-black tree
  //	from source1 into our argArray.
  //
  const ACU_Tree& tree0 = source0->getTree();
  const ACU_Tree& tree1 = source1->getTree();
  argArray.resizeWithoutPreservation(tree0.getSize() + tree1.getSize());

  ACU_FastIter s0(tree0);
  ACU_FastIter s1(tree1);
  ArgVec<Pair>::iterator d = argArray.begin();
  for(;;)
    {
      DagNode* d0 = s0.getDagNode();
      DagNode* d1 = s1.getDagNode();
      int r = d0->compare(d1);
      if (r < 0)
	{
	  d->dagNode = d0;
	  d->multiplicity = s0.getMultiplicity();
	  ++d;
	  s0.next();
	  if (!s0.valid())
	    goto source0Exhausted;
	}
      else if (r > 0)
	{
	  d->dagNode = d1;
	  d->multiplicity = s1.getMultiplicity();
	  ++d;
	  s1.next();
	  goto checkSource1;
	}
      else
	{
	  d->dagNode = d0;
	  d->multiplicity = s0.getMultiplicity() + s1.getMultiplicity();
	  ++d;
	  s0.next();
	  s1.next();
	  if (!s0.valid())
	    {
	    source0Exhausted:
	      d = fastCopy(s1, d);
	      break;
	    }
	checkSource1:
	  if (!s1.valid())
	    {
	      d = fastCopy(s0, d);
	      break;
	    }
	}
    }
  argArray.contractTo(d - argArray.begin());
}
