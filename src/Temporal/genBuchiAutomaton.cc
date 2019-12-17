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
//	Implementation for class GenBuchiAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"

//	temporal class definitions
#include "veryWeakAlternatingAutomaton.hh"
#include "rawTransitionSet.hh"
#include "genBuchiAutomaton.hh"

#include "collapseStates.cc"
#include "sccAnalysis.cc"
#include "sccOptimizations.cc"
#include "satSolve.cc"

GenBuchiAutomaton::GenBuchiAutomaton(LogicFormula* formula, int top)
{
  vwaa = new VeryWeakAlternatingAutomaton(formula, top);
  nrFairnessSets = vwaa->getNrFinalStates();
  for (int i = nrFairnessSets - 1; i >= 0; i--)
    allFair.insert(i);
  const TransitionSet& vwaaInitialStates = vwaa->getInitialStates();
  const TransitionSet::TransitionMap& tm = vwaaInitialStates.getMap();
  TransitionSet::TransitionMap::const_iterator e = tm.end();
  for (TransitionSet::TransitionMap::const_iterator i = tm.begin(); i != e; ++i)
    {
      int index = getStateIndex(i->first);
      initialStates.insert(index);
      generateState(index);
    }
  vwaaStateSets.clear();
  delete vwaa;
}

void
GenBuchiAutomaton::simplify()
{
#ifdef TDEBUG
    dump(cout);
#endif
  maximallyCollapseStates();
  sccOptimizations();
#ifdef TDEBUG
    dump(cout);
#endif
  maximallyCollapseStates();
}

void
GenBuchiAutomaton::generateState(int index)
{
  //
  //	Construct the fair transition set for a GBA state if it has not
  //	been done already.
  //
  if (states[index] != UNDEFINED)
    return;
  FairTransitionSetTable::FastPair fts;
  const NatSet& components = vwaaStateSets.ithElement(index);  // set of VWAA state indices
  if (components.empty())
    {
      //
      //	If we hit the empty state, there are no future obligations
      //	to be discharged and all future infinite sequences are acceptable.
      //	We model this with a "true" self transition which has all fairness
      //	conditions.
      //
      FairTransition f;
      f.first.first = index;
      f.first.second = fairnessConditions.insert(allFair);
      f.second = bdd_true();
      insertFairTransition(fts.first, f);
      states[index] = fairTransitionSets.insert(fts);
      return;
    }
  //
  //	We first compute the raw (unsimplified) product of the
  //	transition sets for each component state in the VWAA.
  //
  NatSet::const_iterator i = components.begin();
  RawTransitionSet pi(vwaa->getTransitionSet(*i));
  {
    const NatSet::const_iterator e = components.end();
    for (++i; i != e; ++i)
      {
	RawTransitionSet c(vwaa->getTransitionSet(*i));
	RawTransitionSet t(pi, c);
	pi.swap(t);
      }
  }
  //
  //	Each transition in the raw product must have a fairness set
  //	computed for it. Any simplification must explicitly take fairness
  //	into account so we don't end up discarding more fair transitions
  //	in favor of less fair transitions.
  //
  RawTransitionSet::const_iterator e = pi.end();
  for (RawTransitionSet::const_iterator i = pi.begin(); i != e; ++i)
    {
      NatSet fairness;
      vwaa->computeFairnessSet(*i, fairness);
      FairTransition f;
      f.first.first = getStateIndex(i->first);
      f.first.second = fairnessConditions.insert(fairness);
      f.second = i->second;
      insertFairTransition(fts.first, f);
    }
  states[index] = fairTransitionSets.insert(fts);
  //
  //	Now generate states reachable from new state.
  //
  FairTransitionSet::const_iterator e2 = fts.first.end();
  for (FairTransitionSet::const_iterator i = fts.first.begin();  i != e2; ++i)
    generateState(i->first.first);
}

int
GenBuchiAutomaton::getStateIndex(const NatSet& stateSet)
{
  //
  //	Convert a set of VWAA state indices into a GBA state index,
  //	creating a new GBA state index if we haven't seen this set before.
  //
  int index = vwaaStateSets.insert(stateSet);
  if (index >= states.length())
    {
      Assert(index == states.length(), "states synch error");
      states.expandTo(index + 1);
      states[index] = UNDEFINED;
    }
  return index;
}

void
GenBuchiAutomaton::insertFairTransition(FairTransitionSet& fts, const FairTransition& ft)
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
      else
	{
	  const NatSet& existingStates = vwaaStateSets.ithElement(t->first.first);
	  const NatSet& existingFairness = fairnessConditions.ithElement(t->first.second);
	  const NatSet& newStates = vwaaStateSets.ithElement(ft.first.first);
	  const NatSet& newFairness = fairnessConditions.ithElement(ft.first.second);
	  if (existingStates.contains(newStates) && newFairness.contains(existingFairness))
	    {
	      t->second = bdd_and(t->second, bdd_not(formula));
	      if (t->second == bdd_false())
		fts.erase(t);  // existing pair completely subsumed
	    }
	  else if (newStates.contains(existingStates) && existingFairness.contains(newFairness))
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

void
GenBuchiAutomaton::remapNatSet(NatSet& newSet,
			       const NatSet& oldSet,
			       const Vector<int>& natMap)
{
  for (int i = natMap.length() - 1; i >= 0; i--)
    {
      int t = natMap[i];
      if (t != NONE && oldSet.contains(i))
	newSet.insert(t);
    }
}

void
GenBuchiAutomaton::dump(ostream& s) const
{
  s << "begin{GenBuchiAutomaton}\n";
  int nrStates = states.length();
  for (int i = 0; i < nrStates; i++)
    {
      s << "state " << i << "\t(" << states[i] << ")\n";
      if (states[i] != UNDEFINED)
	{
	  const FairTransitionSet& fts = fairTransitionSets.ithElement(states[i]);
	  FairTransitionSet::const_iterator e = fts.end();
	  for (FairTransitionSet::const_iterator j = fts.begin();  j != e; ++j)
	    {
	      s << j->first.first << '\t' <<
		fairnessConditions.ithElement(j->first.second) << '\t';
	      BddUser::dump(s, j->second);
	      s << '\n';
	    }
	}
      s << '\n';
    }
  s << "initial states: " << initialStates << '\n';
  s << "\nend{GenBuchiAutomaton}\n";
}
