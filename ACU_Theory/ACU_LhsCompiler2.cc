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
//	Implementation of match compilation for AC and ACU theories.
//	Part 2: aliens only case.
//

void
ACU_Term::compileAliensOnlyCase(ACU_LhsAutomaton* automaton,
				const Vector<Pair>& nonGroundAliens,
				const VariableInfo& variableInfo,
				NatSet& boundUniquely,
				bool& subproblemLikely)
{
  subproblemLikely = false;
  CP_Sequence bestSequence;
  findConstraintPropagationSequence(nonGroundAliens, boundUniquely, bestSequence);
  int i = 0;
  for (; i < bestSequence.nrIndependent; i++)
    {
      const Pair& p = nonGroundAliens[bestSequence.sequence[i]];
      bool spl;
      LhsAutomaton* subAutomaton =
	p.term->compileLhs(false, variableInfo, boundUniquely, spl);
      subproblemLikely = subproblemLikely || spl;
      automaton->addNonGroundAlien(p.term, subAutomaton, p.multiplicity);
    }
  Assert(boundUniquely == bestSequence.bound, "bound clash");

  addIndependentAliens(nonGroundAliens, bestSequence);
  int nrNonGroundAliens = nonGroundAliens.length();
  if (bestSequence.nrIndependent < nrNonGroundAliens)
    {
      Assert(nrNonGroundAliens - bestSequence.nrIndependent >= 2,
	     "lone non-independent alien is impossible");
      subproblemLikely = true;
      weakConstraintPropagation(nonGroundAliens, boundUniquely,
				bestSequence.nrIndependent, bestSequence.sequence);
    }
  for (; i < nrNonGroundAliens; i++)
    {
      const Pair& p = nonGroundAliens[bestSequence.sequence[i]];
      NatSet local(boundUniquely);
      bool spl;
      LhsAutomaton* subAutomaton =
	p.term->compileLhs(false, variableInfo, local, spl);
      subproblemLikely = subproblemLikely || spl;
      automaton->addNonGroundAlien(p.term, subAutomaton, p.multiplicity);
    }
  automaton->complete(ACU_LhsAutomaton::ALIENS_ONLY,
		      bestSequence.nrIndependent);
}

void
ACU_Term::findConstraintPropagationSequence(const Vector<Pair>& aliens,
					    const NatSet& boundUniquely,
					    CP_Sequence& bestSequence)
{
  int nrAliens = aliens.length();
  Vector<int> currentSequence(nrAliens);
  for (int i = 0; i < nrAliens; i++)
    currentSequence[i] = i;
  bestSequence.cardinality = -1;
  findConstraintPropagationSequence(aliens, currentSequence, boundUniquely,
				    0, bestSequence);
  Assert(bestSequence.cardinality >= 0, "didn't find a sequence");
}

void
ACU_Term::findConstraintPropagationSequence(const Vector<Pair>& aliens,
					    const Vector<int>& currentSequence,
					    const NatSet& boundUniquely,
					    int step,
					    CP_Sequence& bestSequence)
{
  int nrAliens = aliens.length();
  if (step < nrAliens)
	    {
      //
      //        Try to grow search tree.
      //
      bool growth = false;
      for (int i = step; i < nrAliens; i++)
        {
	  Term* t = aliens[currentSequence[i]].term;
	  //
	  //	Check t for match independence from remaining terms.
	  //
	  for (int j = step; j < nrAliens; j++)
	    {
	      if (j != i && !(t->matchIndependent(aliens[currentSequence[j]].term)))
		goto nextAlien;
	    }
	  //
	  //	Now see if term will force the binding of additional variables.
	  //
	  {
	    NatSet newBound(boundUniquely);
	    t->analyseConstraintPropagation(newBound);
	    if (newBound != boundUniquely)
	      {
		//
		//	Build new sequence by adding t, together with any alien
		//	that will "ground out match".
		//
		Vector<int> newSequence(currentSequence);
		swap(newSequence[step], newSequence[i]);
		int newStep = step + 1;
		for (int j = newStep; j < nrAliens; j++)
		  {
		    Term* a = aliens[newSequence[j]].term;
		    if (a->willGroundOutMatch(newBound))
		      {
			swap(newSequence[newStep], newSequence[j]);
			++newStep;
		      }
		  }
		findConstraintPropagationSequence(aliens, newSequence, newBound,
						  newStep, bestSequence);
		growth = true;
	      }
	  }
	nextAlien:
	  ;
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
      bestSequence.nrIndependent = step;
      bestSequence.bound = boundUniquely;  // deep copy
      bestSequence.cardinality = n;
    }
}

void
ACU_Term::addIndependentAliens(const Vector<Pair>& aliens,
			       CP_Sequence& bestSequence)
{
  int nrAliens = aliens.length();
  //
  //	An alien can be "forced" if it is match independent.
  //
  for (int i = bestSequence.nrIndependent; i < nrAliens; i++)
    {
      Term* t = aliens[bestSequence.sequence[i]].term;
      for (int j = bestSequence.nrIndependent; j < nrAliens; j++)
	{
	  if (j != i && !(t->matchIndependent(aliens[bestSequence.sequence[j]].term)))
	    goto nextAlien;
	}
      swap(bestSequence.sequence[bestSequence.nrIndependent],
	   bestSequence.sequence[i]);
      ++(bestSequence.nrIndependent);
    nextAlien:
      ;
    }
}
