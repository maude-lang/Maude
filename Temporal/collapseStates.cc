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
//	Code for collapsing states which have same fair transition set.
//

void
GenBuchiAutomaton::maximallyCollapseStates()
{
  while (fairTransitionSets.size() < states.length())
    {
      collapseStates();
#ifdef TDEBUG
      dump(cout);
#endif
    }
}

void
GenBuchiAutomaton::collapseStates()
{
  int nrNewStates = fairTransitionSets.size();
  //
  //	We build collapsed automaton in these data structures.
  //	Fairness combinations does not change.
  //
  NatSet newInitialStates;
  Vector<int> newStates(nrNewStates);
  FairTransitionSetTable newFairTransitionSets;
  //
  //	Use states as a surjection to collapse equivalent states.
  //
  remapNatSet(newInitialStates, initialStates, states);
  for (int i = 0; i < nrNewStates; i++)
    {
      FairTransitionSetTable::FastPair transformed;
      transformFairTransitionSet(transformed.first, fairTransitionSets.ithElement(i));
      newStates[i] = newFairTransitionSets.insert(transformed);
    }
  //
  //	Swap new and old automata.
  //
  initialStates.swap(newInitialStates);
  states.swap(newStates);
  fairTransitionSets.swap(newFairTransitionSets);
}

void
GenBuchiAutomaton::transformFairTransitionSet(FairTransitionSet& transformed,
					      const FairTransitionSet& original)
{
  FairTransitionSet::const_iterator e = original.end();
  for (FairTransitionSet::const_iterator i = original.begin(); i != e; ++i)
    {
      FairTransition f;
      f.first.first = states[i->first.first];
      Assert(f.first.first != UNDEFINED, "state not generated");
      f.first.second = i->first.second;
      f.second = i->second;
      insertFairTransition2(transformed, f);
    }
}

void
GenBuchiAutomaton::insertFairTransition2(FairTransitionSet& fts, const FairTransition& ft)
{
  Bdd formula = ft.second;
  Assert(formula != bdd_false(), "tried to insert false transition");
  const FairTransitionSet::iterator e = fts.end();
  FairTransitionSet::iterator equal = e;
  for (FairTransitionSet::iterator i = fts.begin(); i != e;)
    {
      FairTransitionSet::iterator t = i++;
      if (t->first == ft.first)
	equal = t;
      else if (t->first.first == ft.first.first)
	{
	  const NatSet& existingFairness = fairnessConditions.ithElement(t->first.second);
	  const NatSet& newFairness = fairnessConditions.ithElement(ft.first.second);
	  if (newFairness.contains(existingFairness))
	    {
	      t->second = bdd_and(t->second, bdd_not(formula));
	      if (t->second == bdd_false())
		fts.erase(t);  // existing pair completely subsumed
	    }
	  else if (existingFairness.contains(newFairness))
	    {
	      formula = bdd_and(formula, bdd_not(t->second));
	      if (formula == bdd_false())
		return;  // new transition completely subsumed
	    }
	}
    }
  if (equal == e)
    {
      pair<FairTransitionSet::iterator, bool> p = fts.insert(ft);
      Assert(p.second, "failed to insert");
      p.first->second = formula;
    }
  else
    equal->second = bdd_or(equal->second, formula);
}
