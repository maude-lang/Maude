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
//	Code for strongly connected component optimizations.
//

void
GenBuchiAutomaton::sccOptimizations()
{
  sccAnalysis();
  //
  //	We now renumber states and fairness sets to take account of
  //	states in dead components and fairness sets which are not essential.
  //
  int nrStates = states.length();
  Vector<int> stateMap(nrStates);
  int nrLive = 0;
  for (int i = 0; i < nrStates; i++)
    stateMap[i] = (componentInfo[stateInfo[i].component].status == DEAD) ? NONE : nrLive++;

  Vector<int> fairMap(nrFairnessSets);
  int nrFair = 0;
  for (int i = 0; i < nrFairnessSets; i++)
    fairMap[i] = essential.contains(i) ? nrFair++ : NONE;
  //
  //	Then we rebuild the GBA, removing dead states, non-essential fairness
  //	sets together fairness info from every transition that is not
  //	internal to a fair component. For transitions within a fair component
  //	each arc will belong to the (surviving) redundant fairness sets for
  //	that component in addition to anything they belonged to previously.
  //
  NatSet newInitialStates;
  Vector<int> newStates(nrLive);
  FairTransitionSetTable newFairTransitionSets;

  NatSetTable oldFairnessConditions;
  oldFairnessConditions.swap(fairnessConditions);

  for (int i = 0; i < nrStates; i++)
    {
      if (stateMap[i] != NONE)
	{
	  FairTransitionSetTable::FastPair transformed;
	  int component = stateInfo[i].component;
	  const FairTransitionSet& original = fairTransitionSets.ithElement(states[i]);
	  if (componentInfo[component].status == UNFAIR)
	    eliminateFairness(transformed.first, original, stateMap);
	  else
	    {
	      transformFairTransitionSet2(oldFairnessConditions,
					  transformed.first,
					  original,
					  stateMap,
					  fairMap,
					  component);
	    }
	  newStates[stateMap[i]] = newFairTransitionSets.insert(transformed);
	}
    }
  remapNatSet(newInitialStates, initialStates, stateMap);

  initialStates.swap(newInitialStates);
  fairTransitionSets.swap(newFairTransitionSets);
  states.swap(newStates);
  nrFairnessSets = nrFair;
}

void
GenBuchiAutomaton::eliminateFairness(FairTransitionSet& transformed,
				     const FairTransitionSet& original,
				     const Vector<int>& stateMap)
{
  FairTransitionSet::const_iterator e = original.end();
  for (FairTransitionSet::const_iterator i = original.begin(); i != e; ++i)
    {
      int to = i->first.first;
      if (stateMap[to] != NONE)
	{
	  FairTransition f;
	  f.first.first = stateMap[to];
	  NatSet emptySet;
	  f.first.second = fairnessConditions.insert(emptySet);
	  f.second = i->second;
	  insertFairTransition2(transformed, f);
	}
    }
}

void
GenBuchiAutomaton::transformFairTransitionSet2(NatSetTable& oldFairnessConditions,
					       FairTransitionSet& transformed,
					       const FairTransitionSet& original,
					       const Vector<int>& stateMap,
					       const Vector<int>& fairMap,
					       int component)
{
  FairTransitionSet::const_iterator e = original.end();
  for (FairTransitionSet::const_iterator i = original.begin(); i != e; ++i)
    {
      int to = i->first.first;
      if (stateMap[to] != NONE)
	{
	  FairTransition f;
	  f.first.first = stateMap[to];
	  if (stateInfo[to].component == component)
	    {
	      //
	      //	Internal transition in fair component.
	      //
	      NatSet fairness(oldFairnessConditions.ithElement(i->first.second));
#ifdef TDEBUG
	      cout << "start " << fairness;
#endif
	      fairness.insert(componentInfo[component].redundant);
#ifdef TDEBUG
	      cout << " after insert " << fairness;
#endif
	      NatSet newFairness;
	      remapNatSet(newFairness, fairness, fairMap);
#ifdef TDEBUG
	      cout << " after remap " << newFairness << '\n';
#endif
	      f.first.second = fairnessConditions.insert(newFairness);
	    }
	  else
	    {
	      NatSet emptySet;
	      f.first.second = fairnessConditions.insert(emptySet);
	    }
	  f.second = i->second;
	  insertFairTransition2(transformed, f);
	}
    }
}
