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
//	Class for Buchi Automata.
//
#ifndef _buchiAutomaton2_hh_
#define _buchiAutomaton2_hh_
#include <map>
#include "bddUser.hh"
#include "bdd.hh"
#include "indexedSet.hh"

class BuchiAutomaton2 : private BddUser
{
public:
  typedef pair<int, Bdd> Transition;
  typedef map<int, Bdd> TransitionMap;

  BuchiAutomaton2(LogicFormula* formula, int top);

  int getNrStates() const;
  const NatSet& getInitialStates() const;
  bool isAccepting(int stateNr) const;
  const TransitionMap& getTransitions(int stateNr) const;

  void dump(ostream& s);

private:
  typedef IndexedSet<TransitionMap> TransitionMapTable;

  static void remapNatSet(NatSet& newSet,
			  const NatSet& oldSet,
			  const Vector<int>& natMap);

  bool hasNonAcceptingTarget(const TransitionMap& tm);
  void transformTransitionMap(TransitionMap& transformed,
			      const TransitionMap& original);
  void collapseStates();
  void generate(const GenBuchiAutomaton& g,
		int oldStateNr,
		int instanceNr);
  void insertTransition(TransitionMap& m, const Transition& t);

  NatSet initialStates;
  NatSet acceptingStates;
  Vector<int> states;
  TransitionMapTable transitionMaps;
};

inline int
BuchiAutomaton2::getNrStates() const
{
  return states.length();
}

inline const NatSet&
BuchiAutomaton2::getInitialStates() const
{
  return initialStates;
}

inline bool
BuchiAutomaton2::isAccepting(int stateNr) const
{
  return acceptingStates.contains(stateNr);
}

inline const BuchiAutomaton2::TransitionMap&
BuchiAutomaton2::getTransitions(int stateNr) const
{
  return transitionMaps.ithElement(states[stateNr]);
}

#endif
