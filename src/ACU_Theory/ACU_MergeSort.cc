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
//	Specialized mergesort algorithm for ACU argument lists.
//

void
ACU_DagNode::mergeSortAndUniquize()
{
  int nrArgs = argArray.length();
  int nrRuns = runsBuffer.length();
  ArgVec<Pair> mergeBuffer(nrArgs);

  while (nrRuns > 1)
    {
      int d = 0;
      int rd = 0;
      int i = 0;
      for (; i < nrRuns - 1; i += 2)
	{
	  //
	  //	Merge runs i and i+1
	  //
	  int p1 = runsBuffer[i];
	  int e1 = runsBuffer[i + 1];
	  int p2 = runsBuffer[i + 1];
	  int e2 = i + 2 < nrRuns ? runsBuffer[i + 2] : nrArgs;
	  runsBuffer[rd++] = d;  // save start of merged run we are creating
	  for(;;)
	    {
	      int r = argArray[p1].dagNode->compare(argArray[p2].dagNode);
	      if (r < 0)
		{
		  mergeBuffer[d++] = argArray[p1++];
		  if (p1 == e1)
		    {
		      p1 = p2;
		      e1 = e2;
		      break;
		    }
		}
	      else if (r > 0)
		{
		  mergeBuffer[d++] = argArray[p2++];
		  if (p2 == e2)
		    break;
		}
	      else
		{
		  mergeBuffer[d].dagNode = argArray[p1].dagNode;
		  mergeBuffer[d++].multiplicity =
		    argArray[p1++].multiplicity + argArray[p2++].multiplicity;
		  if (p1 == e1)
		    {
		      p1 = p2;
		      e1 = e2;
		      break;
		    }
		  if (p2 == e2)
		    break;
		}
	    }
	  while (p1 < e1)
	    mergeBuffer[d++] = argArray[p1++];
	}
      if (i < nrRuns)
	{
	  Assert(rd < i, "floor(nrRuns/2) >= nrRuns - 1");
	  runsBuffer[rd++] = d;  // Assert guarantees we don't overwrite runsBuffer[i]
	  //
	  //	Copy odd run into merge buffer
	  //
	  for (int p = runsBuffer[i]; p < nrArgs; p++)
	    mergeBuffer[d++] = argArray[p];
	}
      nrArgs = d;
      nrRuns = rd;
      argArray.swap(mergeBuffer);
    }
  argArray.contractTo(nrArgs);  // merging may have shrunk number of arguments
}

void
ACU_DagNode::sortAndUniquize()
{
  //
  //	ACU argument lists tend to have long runs of in order
  //	arguments so we discover these runs first befor running
  //	the mergesort algorithm.
  //
  int nrArgs = argArray.length();
  for (int i = 1; i < nrArgs; i++)
    {
      int r = argArray[i - 1].dagNode->compare(argArray[i].dagNode);
      if (r >= 0)
	{
	  runsBuffer.contractTo(1);
	  int lastRun = 0;
	  runsBuffer[0] = lastRun;
	  int d = i - 1;
	  for(;;)
	    {
	      if (r == 0)
		argArray[d].multiplicity += argArray[i].multiplicity;
	      else if (r < 0)
		argArray[++d] = argArray[i];
	      else
		{
		  if (lastRun == d)  // don't make a run of length 1
		    {
		      Pair t(argArray[d]);
		      argArray[d] = argArray[i];
		      argArray[++d] = t;
		    }
		  else
		    {
		      argArray[++d] = argArray[i];
		      lastRun = d;
		      runsBuffer.append(lastRun);
		    }
		}
	      if (++i == nrArgs)
		break;
	      r = argArray[d].dagNode->compare(argArray[i].dagNode);
	    }
	  argArray.contractTo(d + 1);
	  if (runsBuffer.length() > 1)
	    mergeSortAndUniquize();
	  break;
	}
    }
}

void
ACU_DagNode::flattenSortAndUniquize(int expansion)
{
  Symbol* s = symbol();
  int nrArgs = argArray.length();
  ArgVec<Pair> newArray(nrArgs + expansion);
  runsBuffer.contractTo(1);
  int lastRun = 0;
  runsBuffer[0] = lastRun;
  int d = -1;  // HACK: should use iterators for everything

  for (int i = 0; i < nrArgs; i++)
    {
      DagNode* n = argArray[i].dagNode;
      if (n->symbol() == s)
	{
	  //
	  //	Need to flatten in subterm.
	  //
	  int m = argArray[i].multiplicity;
	  if (d >= 0)
	    {
	      lastRun = d + 1;
	      runsBuffer.append(lastRun);
	    }
	  ArgVec<Pair>::iterator dest = newArray.begin() + (d + 1);

	  if (safeCast(ACU_BaseDagNode*, n)->isTree())
	    {
	      const ACU_Tree& tree = safeCast(ACU_TreeDagNode*, n)->getTree();
	      ACU_FastIter i(tree);
	      do       
		{
		  dest->dagNode = i.getDagNode();
		  dest->multiplicity = m * i.getMultiplicity();
		  ++dest;
		  i.next();
		}
	      while (i.valid());
	      d += tree.getSize();
	    }
	  else
	    {
	      const ArgVec<Pair>& argArray2 = safeCast(ACU_DagNode*, n)->argArray;
	      ArgVec<Pair>::const_iterator i = argArray2.begin();
	      const ArgVec<Pair>::const_iterator e = argArray2.end();
	      do       
		{
		  dest->dagNode = i->dagNode;
		  dest->multiplicity = m * i->multiplicity;
		  ++dest;
		  ++i;
		}
	      while (i != e);
	      d += argArray2.length();
	    }
	}
      else
	{
	  if (d >= 0)
	    {
	      int r = newArray[d].dagNode->compare(n);
	      if (r == 0)
		{
		  newArray[d].multiplicity += argArray[i].multiplicity;
		  continue;
		}
	      else if (r > 0)
		{
		  if (lastRun == d)  // don't make a run of length 1
		    {
		      newArray[d + 1] = newArray[d];
		      newArray[d] = argArray[i];
		      ++d;
		      continue;
		    }
		  else
		    {
		      lastRun = d + 1;
		      runsBuffer.append(lastRun);
		    }
		}
	    }
	  newArray[++d] = argArray[i];
	}
    }
  /*
  cout << " flattenSortAndUniquize() runs buffer: ";
  for (int i = 0; i < runsBuffer.length(); i++)
    cout << runsBuffer[i] << ' ';
  cout << " nrArgs = " << newArray.length() << '\n';
  */
  newArray.contractTo(d + 1);
  argArray.swap(newArray);
  if (runsBuffer.length() > 1)
    mergeSortAndUniquize();
}
