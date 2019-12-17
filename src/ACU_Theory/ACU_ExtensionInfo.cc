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
//      Implementation for class ACU_ExtensionInfo.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "ACU_Persistent.hh"
#include "ACU_Theory.hh"

//      ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_TreeDagNode.hh"
#include "ACU_ExtensionInfo.hh"


DagNode*
ACU_ExtensionInfo::buildMatchedPortion() const
{
  if (matchedWhole())
    return subject;

  ACU_DagNode* s = getACU_DagNode(subject);
  ACU_DagNode* n =
    new ACU_DagNode(s->symbol(), s->argArray.length(), ACU_DagNode::ASSIGNMENT);
  ArgVec<ACU_Pair>::iterator dest = n->argArray.begin();

  DagNode* d = unmatched.getNode();
  if (d == 0)
    {
      //
      //	Unmatched stuff is given by a multiplicity vector.
      //
      Vector<int>::const_iterator i = unmatchedMultiplicity.begin();
      FOR_EACH_CONST(source, ArgVec<ACU_Pair>, s->argArray)
	{
	  int m = source->multiplicity - *i;
	  Assert(m >= 0, "bad unmatchedMultiplicity");
	  if (m > 0)
	    {
	      dest->set(source->dagNode, m);
	      ++dest;
	    }
	  ++i;
	}
    }
  else if (d->symbol() == s->symbol())
    {
      //
      //	Unmatched stuff is given by a dag node in our theory.
      //
      ACU_DagNode* u = getACU_DagNode(d);
      ArgVec<ACU_Pair>::const_iterator i = u->argArray.begin();
      FOR_EACH_CONST(source, ArgVec<ACU_Pair>, s->argArray)
	{
	  int m = source->multiplicity;
	  if (i->dagNode == source->dagNode)
	    {
	      m -= i->multiplicity;
	      Assert(m >= 0, "bad unmatched");
	      ++i;
	      if (m == 0)
		continue;
	    }
	  dest->set(source->dagNode, m);
	  ++dest;
	}
    }
  else
    {
      //
      //	Unmatched stuff is given an alien dag node.
      //
      FOR_EACH_CONST(source, ArgVec<ACU_Pair>, s->argArray)
	{
	  int m = source->multiplicity;
	  if (d == source->dagNode)
	    {
	      --m;
	      if (m == 0)
		continue;
	    }
	  dest->set(source->dagNode, m);
	  ++dest;
	}
    }
  int size = dest - n->argArray.begin();
  Assert(size > 1 || n->argArray[0].multiplicity > 1,
	 "must match more than one thing");
  n->argArray.contractTo(size);
  return n;
}

DagNode*
ACU_ExtensionInfo::buildUnmatchedPortion() const
{
  if (DagNode* d = unmatched.getNode())
    return d;
  Assert(!(subject->isTree()), "tree form!");
  
  int size = 0;
  Vector<int>::const_iterator last;  // gcc gives uninitialized warning
  {
    FOR_EACH_CONST(i, Vector<int>, unmatchedMultiplicity)
      {
	if (*i > 0)
	  {
	    ++size;
	    last = i;
	  }
      }
  }
  Assert(size >= 1, "no unmatched portion");
  
  ACU_DagNode* s = safeCast(ACU_DagNode*, subject);
  if (size == 1 && *last == 1)
    return s->argArray[last - unmatchedMultiplicity.begin()].dagNode;
  
  ACU_DagNode* n = new ACU_DagNode(s->symbol(), size, ACU_DagNode::ASSIGNMENT);
  ArgVec<ACU_Pair>::const_iterator source = s->argArray.begin();
  ArgVec<ACU_Pair>::iterator dest = n->argArray.begin();
  {
    FOR_EACH_CONST(i, Vector<int>, unmatchedMultiplicity)
      {
	int t = *i;
	if (t > 0)
	  {
	    dest->set(source->dagNode, t);
	    ++dest;
	  }
	++source;
      }
  }
  return n;
}

ExtensionInfo*
ACU_ExtensionInfo::makeClone() const
{
  ACU_ExtensionInfo* e = new ACU_ExtensionInfo(subject);
  e->setValidAfterMatch(validAfterMatch());
  bool whole = matchedWhole();
  e->setMatchedWhole(whole);
  if (!whole)
    {
      if (DagNode* d = unmatched.getNode())
	e->unmatched.setNode(d);
      else
	e->unmatchedMultiplicity = unmatchedMultiplicity;  // deep copy
    }
  e->upperBound = upperBound;
  return e;
}

void
ACU_ExtensionInfo::copy(ExtensionInfo* extensionInfo)
{
  ACU_ExtensionInfo* e = safeCast(ACU_ExtensionInfo*, extensionInfo);
  setValidAfterMatch(e->validAfterMatch());
  bool whole = e->matchedWhole();
  setMatchedWhole(whole);
  subject = e->subject;
  if (!whole)
    {
      if (DagNode* d = e->unmatched.getNode())
	unmatched.setNode(d);
      else
	unmatchedMultiplicity = e->unmatchedMultiplicity;  // deep copy
    }
  upperBound = e->upperBound;
}
