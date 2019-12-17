/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2009 SRI International, Menlo Park, CA 94025, USA.

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
  //	First gather all symbols lying in or directly under free skeleton.
  //
  Vector<FreeOccurrence> freeSymbols;
  Vector<FreeOccurrence> otherSymbols;
  const_cast<FreeTerm*>(this)->scanFreeSkeleton(freeSymbols, otherSymbols); // cast way const
  //
  //	Now extract the non-ground aliens and update BoundUniquely with variables
  //	that lie directly under the free skeleton and thus will receive an unique binding.
  //
  Vector<FreeOccurrence> nonGroundAliens;
  FOR_EACH_CONST(i, Vector<FreeOccurrence>, otherSymbols)
    {
      Term* t = i->term();
      if (VariableTerm* v = dynamic_cast<VariableTerm*>(t))
	boundUniquely.insert(v->getIndex());
      else
	{
	  if (!(t->ground()))
	    nonGroundAliens.append(*i);
	}
    }
  if (!(nonGroundAliens.empty()))
    {
      DebugAdvisory("FreeTerm::analyseConstraintPropagation() : looking at " << this <<
		    " and saw " << nonGroundAliens.length() << " nonground aliens");
      //
      //	Now we have to find a best sequence in which to match the non-ground alien
      //        subterms. Sequences that pin down unique values for variables allow those
      //	values to be propagated.
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
  FOR_EACH_CONST(i, Vector<FreeOccurrence>, otherSymbols)
    {
      Term* t = i->term();
      if (VariableTerm* v = dynamic_cast<VariableTerm*>(t))
	{
	  int index = v->getIndex();
	  if (boundUniquely.contains(index))
	    boundVariables.append(*i);
	  else
	    {
	      boundUniquely.insert(index);
	      freeVariables.append(*i);
	    }
	}
      else
	{
	  if (t->ground())
	    groundAliens.append(*i);
	  else
	    nonGroundAliens.append(*i);
	}
    }
  CP_Sequence bestSequence;
  int nrAliens = nonGroundAliens.length();
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
  FOR_EACH_CONST(i, Vector<FreeOccurrence>, otherSymbols)
    {
      Term* t = i->term();
      if (VariableTerm* v = dynamic_cast<VariableTerm*>(t))
	{
	  int index = v->getIndex();
	  if (boundUniquely.contains(index))
	    boundVariables.append(*i);
	  else
	    {
	      boundUniquely.insert(index);
	      uncertainVariables.append(*i);
	    }
	}
      else
	{
	  if (t->ground())
	    groundAliens.append(*i);
	  else
	    nonGroundAliens.append(*i);
	}
    }
  CP_Sequence bestSequence;
  int nrAliens = nonGroundAliens.length();
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
  findConstraintPropagationSequence(aliens,
				    currentSequence,
				    boundUniquely,
                                    0,
				    bestSequence);
  Assert(bestSequence.cardinality >= 0, "didn't find a sequence");
}

bool
FreeTerm::remainingAliensContain(const Vector<FreeOccurrence>& aliens,
				 Vector<int>& currentSequence,
				 int step,
				 int us,
				 const NatSet& interestingVariables)
{
  if (interestingVariables.empty())
    return false;
  int nrAliens = aliens.size();
  for (int i = step; i < nrAliens; i++)
    {
      if (i != us &&
	  !(interestingVariables.disjoint(aliens[currentSequence[i]].term()->occursBelow())))
	return true;
    }
  return false;
}

void
FreeTerm::findConstraintPropagationSequence(const Vector<FreeOccurrence>& aliens,
					    Vector<int>& currentSequence,
					    const NatSet& boundUniquely,
					    int step,
					    CP_Sequence& bestSequence)
{
  //
  //	Add any alien that will "ground out match" to current sequence.
  //	By matching these early we maximize the chance of early match failure,
  //	and avoid wasted work at match time.
  //
  int nrAliens = aliens.size();
  for (int i = step; i < nrAliens; i++)
   {
      if (aliens[currentSequence[i]].term()->willGroundOutMatch(boundUniquely))
	{
	  swap(currentSequence[step], currentSequence[i]);
	  ++step;
	}
    }
  if (step < nrAliens)
    {
      Vector<NatSet> newBounds(nrAliens);
      //
      //	Now we search over possible ordering of remaining NGAs.
      //
      DebugAdvisory("FreeTerm::findConstraintPropagationSequence(): phase 1 step = " << step);
      for (int i = step; i < nrAliens; i++)
	{
	  newBounds[i] = boundUniquely;
	  Term * t = aliens[currentSequence[i]].term();
	  t->analyseConstraintPropagation(newBounds[i]);
	  //
	  //	We now check if t has the potential could benefit from delayed matching.
	  //
	  NatSet unbound(t->occursBelow());
	  unbound.subtract(newBounds[i]);
	  if (!remainingAliensContain(aliens, currentSequence, step, i, unbound))
	    {
	      //
	      //	No, so commit to matching it here.
	      //
	      DebugAdvisory("FreeTerm::findConstraintPropagationSequence(): step = " <<
			    step << " committed to " << t);
	      swap(currentSequence[step], currentSequence[i]);
	      findConstraintPropagationSequence(aliens, currentSequence, newBounds[i], step + 1, bestSequence);
	      return;
	    }
	}
      //
      //	We didn't find a NGA that we could commit to matching without possibly missing a better sequence.
      //	Now go over the NGAs again. This time we need to consider expanding multiple branches in the
      //	search tree.
      //
      DebugAdvisory("FreeTerm::findConstraintPropagationSequence(): phase 2 step = " << step);
      bool expandedAtLeastOneBranch = false;
      for (int i = step; i < nrAliens; i++)
	{
	  //
	  //	We expand this branch if it binds something that could help another NGA.
	  //
	  NatSet newlyBoundUniquely(newBounds[i]);
	  newlyBoundUniquely.subtract(boundUniquely);
	  if (remainingAliensContain(aliens, currentSequence, step, i, newlyBoundUniquely))	    
	    {
	      //
	      //	Explore this path.
	      //
	      DebugAdvisory("FreeTerm::findConstraintPropagationSequence(): step = " <<
			    step << " exploring " << aliens[currentSequence[i]].term());
	      swap(currentSequence[step], currentSequence[i]);
	      findConstraintPropagationSequence(aliens, currentSequence, newBounds[i], step + 1, bestSequence);
	      swap(currentSequence[step], currentSequence[i]);
	      expandedAtLeastOneBranch = true;
	    }
	}
      if (expandedAtLeastOneBranch)
	return;
      //
      //	If we get here, none of the remaining NGAs can bind a variable that could affect
      //	the ability of other NGAs to bind variables, so there is no point persuing further
      //	exploration. But we still need to union any other variable they may bind and score
      //	the result, by making a recursive call to our leaf case.
      //
      DebugAdvisory("FreeTerm::findConstraintPropagationSequence(): phase 3 step = " << step);
      NatSet newBoundUnion;
      for (int i = step; i < nrAliens; i++)
	newBoundUnion.insert(newBounds[i]);
      findConstraintPropagationSequence(aliens, currentSequence, newBoundUnion, nrAliens, bestSequence);
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
      if (FreeTerm* f = dynamic_cast<FreeTerm*>(t))
	f->scanFreeSkeleton(freeSymbols, otherSymbols, ourPosition, i);
      else
	{
	  FreeOccurrence oc2(ourPosition, i, t);
	  otherSymbols.append(oc2);
	}
    }
}
