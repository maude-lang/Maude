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
//	Code to manipulate AU argument lists taking account of
//	all the complexities introduced by one sided identities.
//

bool
AU_DagNode::eliminateForward(DagNode* target, int& pos, int limit) const
{
  int last = argArray.length() - 1;
  //
  //	pos may point to 1 beyond limit to indicate that target must match nothing.
  //
  Assert(pos >= 0 && limit <= last && pos <= limit + 1,
	 "bad pos or limit");
  AU_Symbol* s = symbol();
  bool leftId = s->leftId();
  bool rightId = s->rightId();
  Term* identity = s->getIdentity();

  if (target->symbol() == s)
    {
      const ArgVec<DagNode*> args2 = getAU_DagNode(target)->argArray;
      int start = 0;
      int finish = args2.length() - 1;
      if (rightId)
	{
	  if (!leftId && pos > 0 && identity->equal(args2[start]))
	    ++start;  // skip over leading identity in target
	}
      else
	{
	  if (leftId && pos + finish <= last && identity->equal(args2[finish]))
	    --finish;  // skip over trailing identity in target
	}
      if (pos + (finish - start) > limit)
	return false;

      for (int i = start; i <= finish; i++)
	{
	  if (!(args2[i]->equal(argArray[pos])))
	    return false;
	  ++pos;
	}
      return true;
    }
  else
    {
      if (((pos > 0 && rightId) || (pos <= last && leftId)) && identity->equal(target))
	return true;
      if (pos <= limit && target->equal(argArray[pos]))
	{
	  ++pos;
	  return true;
	}
      return false;
    }
}

bool
AU_DagNode::eliminateBackward(DagNode* target, int& pos, int limit) const
{
  int last = argArray.length() - 1;
  //
  //	pos may point to 1 before limit to indicate that target must match nothing.
  //
  Assert(pos <= last && limit >= 0 && pos >= limit - 1,
	 "bad pos or limit");
  AU_Symbol* s = symbol();
  bool leftId = s->leftId();
  bool rightId = s->rightId();
  Term* identity = s->getIdentity();

  if (target->symbol() == s)
    {
      const ArgVec<DagNode*> args2 = getAU_DagNode(target)->argArray;
      int start = 0;
      int finish = args2.length() - 1;
      if (rightId)
	{
	  if (!leftId && pos - finish >= 0 && identity->equal(args2[start]))
	    ++start;  // skip over leading identity in target
	}
      else
	{
	  if (leftId && pos < last && identity->equal(args2[finish]))
	    --finish;  // skip over trailing identity in target
	}
      if (pos - (finish - start) < limit)
	return false;

      for (int i = finish; i >= start; i--)
	{
	  if (!(args2[i]->equal(argArray[pos])))
	    return false;
	  --pos;
	}
      return true;
    }
  else
    {
      if (((pos >= 0 && rightId) || (pos < last && leftId)) && identity->equal(target))
	return true;
      if (pos >= limit && target->equal(argArray[pos]))
	{
	  --pos;
	  return true;
	}
      return false;
    }
}

DagNode*
AU_DagNode::makeFragment(int start, int nrSubterms, bool extraId) const
{
  Assert(nrSubterms > 0, "no subterms");
  if (extraId)
    ++nrSubterms;
  if (nrSubterms == 1)
    return argArray[start];

  AU_Symbol* s = symbol();
  AU_DagNode* d = new AU_DagNode(s, nrSubterms);
  int i = 0;
  if (extraId)
    d->argArray[s->leftId() ? --nrSubterms : i++] = s->getIdentityDag();

  for (; i < nrSubterms; i++, start++)
    d->argArray[i] = argArray[start];
  return d;
}
