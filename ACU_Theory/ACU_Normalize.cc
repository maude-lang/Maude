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
//	AC/ACU normalization.
//

void
ACU_DagNode::copyAndBinaryInsert(const ACU_DagNode* source,
				 DagNode* dagNode,
				 int multiplicity)
{
  //
  //	Copy source's argArray into our argArray, inserting dagNode
  //	in the correct place.
  //
  int pos;
  int nrSourceArgs = source->argArray.length();
  if (source->binarySearch(dagNode, pos))
    {
      //DebugAdvisory("copyAndBinaryInsert() " << pos << " out of " << nrSourceArgs);
      argArray.resizeWithoutPreservation(nrSourceArgs);
      fastCopy(source->argArray.begin(), source->argArray.end(), argArray.begin());
      argArray[pos].multiplicity += multiplicity;
    }
  else
    {
      //DebugAdvisory("copyAndBinaryInsert() " << pos << " out of " << nrSourceArgs);
      argArray.resizeWithoutPreservation(nrSourceArgs + 1);
      const ArgVec<Pair>::const_iterator i = source->argArray.begin();
      const ArgVec<Pair>::const_iterator p = i + pos;
      const ArgVec<Pair>::iterator j = fastCopy(i, p, argArray.begin());
      j->dagNode = dagNode;
      j->multiplicity = multiplicity;
      fastCopy(p, source->argArray.end(), j + 1);
    }
}

void
ACU_DagNode::collapse(DagNode* arg)
{
  DagNode* remaining = (symbol()->getPermuteStrategy() == BinarySymbol::EAGER) ?
    arg : arg->copyReducible();
  remaining->overwriteWithClone(this);
}

void
ACU_DagNode::insertAlien(ACU_BaseDagNode* normalForm,
			 int nMult,
			 DagNode* alien,
			 int aMult)
{
  if (normalForm->isTree())
    {
      ACU_Tree t = safeCast(ACU_TreeDagNode*, normalForm)->getTree();
      if (nMult == 1)
	{
	  t.insertMult(alien, aMult);
	  (void) new (this) ACU_TreeDagNode(symbol(), t);
	}
      else
	{
	  //
	  //	Rare case - do slow thing.
	  //
	  flattenSortAndUniquize(t.getSize() - 1);
	}
    }
  else
    {
      ACU_DagNode* d = safeCast(ACU_DagNode*, normalForm);
      if (nMult == 1)
	{
#if 0
	  copyAndBinaryInsert(d, alien, aMult);
	  if (symbol()->useTree() && argArray.length() >= CONVERT_THRESHOLD)
	    {
	      //
	      //	This is the place ACU_TreeDagNodes are
	      //	created where none previously exist.
	      //
	      ACU_Tree t(d->argArray);
	      (void) new (this) ACU_TreeDagNode(symbol(), t);
	    }
#else
	  if (symbol()->useTree() && d->argArray.length() >= CONVERT_THRESHOLD)
	    {
	      ACU_Tree t(d->argArray);
	      t.insertMult(alien, aMult);
	      (void) new (this) ACU_TreeDagNode(symbol(), t);
	    }
	  else
	    copyAndBinaryInsert(d, alien, aMult);
#endif
	}
      else
	{
	  //
	  //	Rare case - do slow thing.
	  //
	  flattenSortAndUniquize(d->argArray.length() - 1);
	}
    }
}      

bool
ACU_DagNode::normalizeAtTop()
{
  //
  //	We return true if we have an identity and collapsed out of theory or
  //	to something that was already in normal form.
  //
  ACU_Symbol* s = symbol();
  Term* identity = s->getIdentity();
  int nrArgs = argArray.length();

  if (nrArgs == 2)
    {
      //
      //	This is the usual case so we heavily optimize it,
      //	considering many subcases.
      //
      DagNode* d0 = argArray[0].dagNode;
      DagNode* d1 = argArray[1].dagNode;
      if (d0->symbol() != s)
	{
	  if (d1->symbol() != s)
	    {
	      //
	      //	(1) Two alien subterms.
	      //
	      int r = d0->compare(d1);
	      if (r < 0)
		{
		  //
		  //	In order - the most common sub-subcase.
		  //
		  if (identity != 0)
		    {
		      if (identity->equal(d0))
			goto collapseToArgument1;
		      if (identity->equal(d1))
			goto collapseToArgument0;
		    }
		}
	      else if (r > 0)
		{
		  //
		  //	Reverse order.
		  //
		  if (identity != 0)
		    {
		      if (identity->equal(d0))
			{
			collapseToArgument1:
			  int m = argArray[1].multiplicity;
			  if (m == 1)
			    {
			      collapse(d1);
			      return true;
			    }
			  argArray[0].dagNode = d1;
			  argArray[0].multiplicity = m;
			  argArray.contractTo(1);
			  return false;
			}
		      else if (identity->equal(d1))
			{
			collapseToArgument0:
			  int m = argArray[0].multiplicity;
			  if (m == 1)
			    {
			      collapse(d0);
			      return true;
			    }
			  argArray.contractTo(1);
			  return false;
			}
		    }
		  int m = argArray[1].multiplicity;
		  argArray[1].multiplicity = argArray[0].multiplicity;
		  argArray[1].dagNode = d0;
		  argArray[0].dagNode = d1;
		  argArray[0].multiplicity = m;
		}
	      else
		{
		  //
		  //	Combine equal arguments.
		  //
		  if (identity != 0 && identity->equal(d0))
		    {
		      collapse(d0);
		      return true;
		    }
		  argArray[0].multiplicity += argArray[1].multiplicity;
		  argArray.contractTo(1);
		}
	      //
	      //	End of two alien subterms subcase.
	      //
	    }
	  else
	    {
	      //
	      //	(2) d0 alien, d1 in theory normal form.
	      //
	      if (identity != 0 && identity->equal(d0))
		{
		  //
		  //	Collapse to d1.
		  //
		  d1->overwriteWithClone(this);
		  return true;
		}
	      insertAlien(safeCast(ACU_BaseDagNode*, d1),
			  argArray[1].multiplicity,
			  d0,
			  argArray[0].multiplicity);
	      //
	      //	End of d0 alien, d1 in theory normal form subcase.
	      //
	    }
	}
      else
	{
	  if (d1->symbol() != s)
	    {
	      //
	      //	(3) d0 in theory normal form, d1 alien.
	      //
	      if (identity != 0 && identity->equal(d1))
		{
		  //
		  //	Collapse to d0.
		  //
		  d0->overwriteWithClone(this);
		  return true;
		}
	      insertAlien(safeCast(ACU_BaseDagNode*, d0),
			  argArray[0].multiplicity,
			  d1,
			  argArray[1].multiplicity);
	      //
	      //	End of d0 in theory normal form, d1 alien subcase.
	      //
	    }
	  else
	    {
	      //
	      //	(4) Two theory normal form subterms.
	      //
	      ACU_BaseDagNode* b0 = safeCast(ACU_BaseDagNode*, d0);
	      ACU_BaseDagNode* b1 = safeCast(ACU_BaseDagNode*, d1);
	      if (argArray[0].multiplicity == 1 && argArray[1].multiplicity == 1)
		{
		  if (b0->isTree())
		    {
		      if (b1->isTree())
			{
			  fastMerge(safeCast(ACU_TreeDagNode*, d0),
				    safeCast(ACU_TreeDagNode*, d1));
			  //
			  //	Convert back to tree representation if
			  //	size of merged argument lists exceeds
			  //	a threshold.
			  //
			  if (symbol()->useTree() && argArray.length() >= MERGE_THRESHOLD)
			    {
			      ACU_Tree t(argArray);
			      (void) new (this) ACU_TreeDagNode(symbol(), t);
			    }
			}
		      else
			{
			  fastMerge(safeCast(ACU_DagNode*, d1),
				    safeCast(ACU_TreeDagNode*, d0));
			}
		    }
		  else
		    {
		      if (b1->isTree())
			{
			  fastMerge(safeCast(ACU_DagNode*, d0),
				    safeCast(ACU_TreeDagNode*, d1));
			}
		      else
			{
			  fastMerge(safeCast(ACU_DagNode*, d0),
				    safeCast(ACU_DagNode*, d1));
			}
		    }
		}
	      else
		{
		  //
		  //	Rare sub-subcase - do slow thing.
		  //
		  flattenSortAndUniquize(b0->getSize() + b1->getSize() - 2);
		}
	      //
	      //	End of two theory normal form subterms subcase.
	      //
	    }
	}
      return false;
      //
      //	End of two arguments case.
      //
    }
  //
  //	General case.
  //
  int expansion = 0;
  bool needToFlatten = false;
  const ArgVec<Pair>::const_iterator e = argArray.end();
  for (ArgVec<Pair>::const_iterator i = argArray.begin(); i != e; ++i)
    {
      DagNode* d = i->dagNode;
      if (d->symbol() == s)
	{
	  expansion += safeCast(ACU_BaseDagNode*, d)->getSize() - 1;
	  needToFlatten = true;  // expansion due to flattening could be zero 
	}
    }
  if (needToFlatten)
    flattenSortAndUniquize(expansion);
  else
    sortAndUniquize();
  return identity != 0 && eliminateArgument(identity);
}
