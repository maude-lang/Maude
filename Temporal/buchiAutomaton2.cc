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
//	Implementation for class BuchiAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"

//	temporal class definitions
#include "genBuchiAutomaton.hh"
#include "buchiAutomaton2.hh"

BuchiAutomaton2::BuchiAutomaton2(LogicFormula* formula, int top)
{
  GenBuchiAutomaton g(formula, top);
  g.simplify();

  int nrOldStates = g.getNrStates();
  if (nrOldStates == 0)
    return;
  initialStates = g.getInitialStates();
  int nrFairnessSets = g.getNrFairnessSets();
  int nrStates = nrOldStates * (nrFairnessSets + 1);
  states.expandTo(nrStates);
  for (int i = 0; i < nrStates; i++)
    states[i] = NONE;
  const NatSet::const_iterator e = initialStates.end();
  for (NatSet::const_iterator i = initialStates.begin(); i != e; ++i)
    generate(g, *i, 0);
#ifdef TDEBUG
  dump(cout);
#endif
  if (transitionMaps.size() < states.length())
    {
      int nrStates;
      do
	{
	  nrStates = states.length();
	  collapseStates();
#ifdef TDEBUG
	  dump(cout);
#endif
	}
      while (states.length() < nrStates);
    }
}

void
BuchiAutomaton2::generate(const GenBuchiAutomaton& g,
			  int oldStateNr,
			  int instanceNr)
{
  int nrFairnessSets = g.getNrFairnessSets();
  int nrOldStates = g.getNrStates();
  int stateNr = oldStateNr + instanceNr * nrOldStates;
  states[stateNr] = UNBOUNDED;  // to prevent looping

  TransitionMap m;
  const GenBuchiAutomaton::FairTransitionSet& ts = g.getTransitions(oldStateNr);
  GenBuchiAutomaton::FairTransitionSet::const_iterator e = ts.end();
  for (GenBuchiAutomaton::FairTransitionSet::const_iterator i = ts.begin(); i != e; ++i)
    {
      Transition t;
      const NatSet& fairness = g.getFairnessCombination(i->first.second);
      int newInstanceNr = (instanceNr == nrFairnessSets) ? 0 : instanceNr;
      while (fairness.contains(newInstanceNr))
	++newInstanceNr;
      int oldTarget = i->first.first;
      int newTarget = oldTarget + newInstanceNr * nrOldStates;
      t.first = newTarget;
      t.second = i->second;
      insertTransition(m, t);
      if (states[newTarget] == NONE)
	generate(g, oldTarget, newInstanceNr);
    }
  states[stateNr] = transitionMaps.insert(m);
  if (instanceNr == nrFairnessSets)
    acceptingStates.insert(stateNr);
}

void
BuchiAutomaton2::insertTransition(TransitionMap& m, const Transition& t)

{
  pair<TransitionMap::iterator, bool> i = m.insert(t);
  if (!(i.second))
    i.first->second = bdd_or(i.first->second, t.second);
}

void
BuchiAutomaton2::collapseStates()
{
  //
  //	Identify transition sets that are used by both accepting
  //	and non-accepting states.
  //
  NatSet usedByAccepting;
  NatSet usedByNonAccepting;
  int nrStates = states.length();
  for (int i = 0; i < nrStates; i++)
    {
      int tsIndex = states[i];
      if (tsIndex != UNDEFINED)
	{
	  if (acceptingStates.contains(i))
	    usedByAccepting.insert(tsIndex);
	  else
	    usedByNonAccepting.insert(tsIndex);
	}
    }
  usedByAccepting.intersect(usedByNonAccepting);
  //
  //	Build a map on transition set indices that maps indices of
  //	shared transition sets that where some of the transitions lead
  //	to non-accepting states to fresh indices.
  //
  int nrTransitionSets = transitionMaps.size();
  Vector<int> acceptingMap(nrTransitionSets);
  int freshIndices = nrTransitionSets;
  for (int i = 0; i < nrTransitionSets; i++)
    {
      if (usedByAccepting.contains(i) && hasNonAcceptingTarget(transitionMaps.ithElement(i)))
	acceptingMap[i] = freshIndices++;
      else
	acceptingMap[i] = UNDEFINED;
    }
  //
  //	Accepting states that used these shared transition sets
  //	now use these fresh indices.
  //
  for (int i = 0; i < nrStates; i++)
    {
      if (acceptingStates.contains(i))
	{
	  int replacement = acceptingMap[states[i]];
	  if (replacement != UNDEFINED)
	    states[i] = replacement;
	}
    }
  //
  //	Now rebuild the Buchi Automata, collapsing states with the
  //	same transition sets.
  //
  NatSet newInitialStates;
  NatSet newAcceptingStates;
  Vector<int> newStates(freshIndices);
  TransitionMapTable newTransitionMaps;
  remapNatSet(newInitialStates, initialStates, states);
  remapNatSet(newAcceptingStates, acceptingStates, states);
  for (int i = 0; i < nrTransitionSets; i++)
    {
      TransitionMap transformed;
      transformTransitionMap(transformed, transitionMaps.ithElement(i));
      newStates[i] = newTransitionMaps.insert(transformed);
      int acceptingCopy = acceptingMap[i];
      if (acceptingCopy != UNDEFINED)
	newStates[acceptingCopy] = newStates[i];
    }

  //
  //	Swap new and old automata.
  //
  initialStates.swap(newInitialStates);
  acceptingStates.swap(newAcceptingStates);
  states.swap(newStates);
  transitionMaps.swap(newTransitionMaps);
}


void
BuchiAutomaton2::transformTransitionMap(TransitionMap& transformed,
					const TransitionMap& original)
{
  TransitionMap::const_iterator e = original.end();
  for (TransitionMap::const_iterator i = original.begin(); i != e; ++i)
    {
      Transition t(states[i->first], i->second);
      insertTransition(transformed, t);
    }
}

bool
BuchiAutomaton2::hasNonAcceptingTarget(const TransitionMap& tm)
{
  TransitionMap::const_iterator e = tm.end();
  for (TransitionMap::const_iterator i = tm.begin(); i != e; ++i)
    {
      if (!(acceptingStates.contains(i->first)))
	return  true;
    }
  return false;
}

void
BuchiAutomaton2::remapNatSet(NatSet& newSet,
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
BuchiAutomaton2::dump(ostream& s)
{
  s << "begin{BuchiAutomaton2}\n";
  int nrStates = states.length();
  for (int i = 0; i < nrStates; i++)
    {
      s << "state " << i;
      if (acceptingStates.contains(i))
	s << "\taccepting";
      s << '\n';
      if (states[i] != NONE)
	{
	  const TransitionMap& t = transitionMaps.ithElement(states[i]);
	  TransitionMap::const_iterator e = t.end();
	  for(TransitionMap::const_iterator j = t.begin(); j != e; ++j)
	    {
	      s << j->first << '\t';
	      BddUser::dump(s, j->second);
	      s << '\n';
	    }
	}
      s << '\n';
    }
  s << "initial states: " << initialStates << '\n';
  s << "end{BuchiAutomaton2}\n";
}
