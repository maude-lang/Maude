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
//	Implementation of match compilation for free theory.
//

struct FreeTerm::CP_Sequence
{
  Vector<int> sequence;
  NatSet bound;
  int cardinality;
};

void
FreeTerm::analyseConstraintPropagation(NatSet& boundUniquely) const
{
  //
  //	First gather all symbols lying in or directly under free skeleton
  //
  Vector<FreeOccurrence> freeSymbols;
  Vector<FreeOccurrence> otherSymbols;
  const_cast<FreeTerm*>(this)->scanFreeSkeleton(freeSymbols, otherSymbols); // cast way const
  //
  //	Now extract the non-ground aliens and update BoundUniquely
  //
  Vector<FreeOccurrence> nonGroundAliens;
  int nrOthers = otherSymbols.length();
  for (int i = 0; i < nrOthers; i++)
    {
      const FreeOccurrence& oc = otherSymbols[i];
      Term* t = oc.term();
      VariableTerm* v = dynamic_cast<VariableTerm*>(t);
      if (v != 0)
	boundUniquely.insert(v->getIndex());
      else
	{
	  if (!(t->ground()))
	    nonGroundAliens.append(oc);
	}
    }
  int nrAliens = nonGroundAliens.length();
  if (nrAliens > 0)
    {
      //
      //	Now we have to find a best sequence in which to match the
      //	non-ground alien subterms
      //
      CP_Sequence bestSequence;
      findConstraintPropagationSequence(nonGroundAliens, boundUniquely, bestSequence);
      boundUniquely.insert(bestSequence.bound);
    }
}

FreeRemainder*
FreeTerm::compileRemainder(Equation* equation, const Vector<int>& slotTranslation)
{
  //
  //	Gather all symbols lying in or directly under free skeleton
  //
  Vector<FreeOccurrence> freeSymbols;
  Vector<FreeOccurrence> otherSymbols;
  scanFreeSkeleton(freeSymbols, otherSymbols);
  //
  //	Now classify occurrences of non Free-Theory symbols into 4 types
  //
  Vector<FreeOccurrence> boundVariables;	// guaranteed bound when matched against
  Vector<FreeOccurrence> freeVariables;		// guaranteed unbound when matched against
  Vector<FreeOccurrence> groundAliens;      	// ground alien subterms
  Vector<FreeOccurrence> nonGroundAliens;	// non-ground alien subterms
  NatSet boundUniquely;
  int nrOthers = otherSymbols.length();
  for (int i = 0; i < nrOthers; i++)
    {
      const FreeOccurrence& oc = otherSymbols[i];
      Term* t = oc.term();
      VariableTerm* v = dynamic_cast<VariableTerm*>(t);
      if (v == 0)
	{
	  if (t->ground())
	    groundAliens.append(oc);
	  else
	    nonGroundAliens.append(oc);
	}
      else
	{
	  int index = v->getIndex();
	  if (boundUniquely.contains(index))
	    boundVariables.append(oc);
	  else
	    {
	      boundUniquely.insert(index);
	      freeVariables.append(oc);
	    }
	}
    }
  int nrAliens = nonGroundAliens.length();
  CP_Sequence bestSequence;
  Vector<LhsAutomaton*> subAutomata(nrAliens);
  if (nrAliens > 0)
    {
      //
      //	Now we have to find a best sequence in which to match the
      //	non-ground alien subterms and generate subautomata for them
      //
      findConstraintPropagationSequence(nonGroundAliens, boundUniquely, bestSequence);
      for (int i = 0; i < nrAliens; i++)
	{
	  bool spl;
	  subAutomata[i] = nonGroundAliens[bestSequence.sequence[i]].term()->
	    compileLhs(false, *equation, boundUniquely, spl);
	}
      Assert(boundUniquely == bestSequence.bound, "bound clash");
    }
  return new FreeRemainder(equation, freeSymbols, freeVariables,
			   boundVariables, groundAliens, nonGroundAliens,
			   bestSequence.sequence, subAutomata, slotTranslation);
}

LhsAutomaton*
FreeTerm::compileLhs2(bool /* matchAtTop */,
		      const VariableInfo& variableInfo,
		      NatSet& boundUniquely,
		      bool& subproblemLikely)
{
  //
  //	First gather all symbols lying in or directly under free skeleton
  //
  Vector<FreeOccurrence> freeSymbols;
  Vector<FreeOccurrence> otherSymbols;
  scanFreeSkeleton(freeSymbols, otherSymbols);
  //
  //	Now classify occurrences of non Free-Theory symbols into 4 types
  //
  Vector<FreeOccurrence> boundVariables;	// guaranteed bound when matched against
  Vector<FreeOccurrence> uncertainVariables;	// status when matched against uncertain
  Vector<FreeOccurrence> groundAliens;      	// ground alien subterms
  Vector<FreeOccurrence> nonGroundAliens;	// non-ground alien subterms
  int nrOthers = otherSymbols.length();
  for (int i = 0; i < nrOthers; i++)
    {
      const FreeOccurrence& oc = otherSymbols[i];
      Term* t = oc.term();
      VariableTerm* v = dynamic_cast<VariableTerm*>(t);
      if (v == 0)
	{
	  if (t->ground())
	    groundAliens.append(oc);
	  else
	    nonGroundAliens.append(oc);
	}
      else
	{
	  int index = v->getIndex();
	  if (boundUniquely.contains(index))
	    boundVariables.append(oc);
	  else
	    {
	      boundUniquely.insert(index);
	      uncertainVariables.append(oc);
	    }
	}
    }
  int nrAliens = nonGroundAliens.length();
  CP_Sequence bestSequence;
  Vector<LhsAutomaton*> subAutomata(nrAliens);
  subproblemLikely = false;
  if (nrAliens > 0)
    {
      //
      //	Now we have to find a best sequence in which to match the
      //	non-ground alien subterms and generate subautomata for them
      //
      findConstraintPropagationSequence(nonGroundAliens, boundUniquely, bestSequence);
      for (int i = 0; i < nrAliens; i++)
	{
	  bool spl;
	  subAutomata[i] = nonGroundAliens[bestSequence.sequence[i]].term()->
	    compileLhs(false, variableInfo, boundUniquely, spl);
	  subproblemLikely = subproblemLikely || spl;
	}
      Assert(boundUniquely == bestSequence.bound, "bound clash");
    }
  return new FreeLhsAutomaton(freeSymbols, uncertainVariables,
			      boundVariables, groundAliens, nonGroundAliens,
			      bestSequence.sequence, subAutomata);
}

void
FreeTerm::findConstraintPropagationSequence(const Vector<FreeOccurrence>& aliens,
					    const NatSet& boundUniquely,
					    CP_Sequence& bestSequence)
{
  int nrAliens = aliens.length();
  Vector<int> currentSequence(nrAliens);
  for (int i = 0; i < nrAliens; i++)
    currentSequence[i] = i;
  bestSequence.cardinality = -1;
  int step = 0;
  insertGroundOutAliens(aliens, currentSequence, boundUniquely, step);
  findConstraintPropagationSequence(aliens,
				    currentSequence,
				    boundUniquely,
                                    step,
				    bestSequence);
  Assert(bestSequence.cardinality >= 0, "didn't find a sequence");
}

void
FreeTerm::insertGroundOutAliens(const Vector<FreeOccurrence>& aliens,
				Vector<int>& sequence,
				const NatSet& boundUniquely,
				int& step)
{
  //
  //	Add any alien that will "ground out match" to sequence.
  //	By matching these early we maximize the chance of early failure.
  //
  int nrAliens = aliens.length();
  for (int i = step; i < nrAliens; i++)
   {
      Term* t = aliens[sequence[i]].term();
      if (t->willGroundOutMatch(boundUniquely))
	{
	  swap(sequence[step], sequence[i]);
	  ++step;
	}
    }
}

void
FreeTerm::findConstraintPropagationSequence(const Vector<FreeOccurrence>& aliens,
					    const Vector<int>& currentSequence,
					    const NatSet& boundUniquely,
					    int step,
					    CP_Sequence& bestSequence)
{
  int nrAliens = aliens.length();
  if (step < nrAliens)
    {
      //
      //        Try to grow search tree
      //
      bool growth = false;
      for (int i = step; i < nrAliens; i++)
        {
	  Term* t = aliens[currentSequence[i]].term();
	  NatSet newBound(boundUniquely);
	  t->analyseConstraintPropagation(newBound);
	  if (newBound != boundUniquely)
	    {
	      Vector<int> newSequence(currentSequence);
	      swap(newSequence[step], newSequence[i]);
	      int newStep = step + 1;
	      insertGroundOutAliens(aliens, newSequence, boundUniquely, newStep);
	      findConstraintPropagationSequence(aliens,
						newSequence,
						newBound,
						newStep,
						bestSequence);
	      growth = true;
	    }
	}
      if (growth)
        return;
    }
  //
  //    Leaf of search tree.
  //
  int n = boundUniquely.cardinality();
  if (n > bestSequence.cardinality)
    {
      bestSequence.sequence = currentSequence;  // deep copy
      bestSequence.bound = boundUniquely;  // deep copy
      bestSequence.cardinality = n;
    }
}

void
FreeTerm::scanFreeSkeleton(Vector<FreeOccurrence>& freeSymbols,
			   Vector<FreeOccurrence>& otherSymbols,
			   int parent,
			   int argIndex)
{
  int ourPosition = freeSymbols.length();
  FreeOccurrence oc(parent, argIndex, this);
  freeSymbols.append(oc);
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; i++)
    {
      Term* t = argArray[i];
      FreeTerm* f = dynamic_cast<FreeTerm*>(t);
      if (f != 0)
	f->scanFreeSkeleton(freeSymbols, otherSymbols, ourPosition, i);
      else
	{
	  FreeOccurrence oc(ourPosition, i, t);
	  otherSymbols.append(oc);
	}
    }
}
