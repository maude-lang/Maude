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
//	Class for Generalized Buchi Automata.
//
//	We implement the VWAA to GBA translation and GBA optimization
//	algorithms from:
//	  Paul Gastin and Denis Oddoux,
//	  "Fast LTL to Buchi Automaton Translation",
//	  CAV 2001, LNCS 2102, pp 53-65, 2001.
//
//	We also do SCC based optimization using ideas from:
//	  Fabio Somenzi and Roderick Bloem,
//	  "Efficient Buchi Automata from LTL Formulae",
//	  CAV 2000, LNCS 1633, p247-263, 2000.
//	However our GBAs have fairness conditions on the transitions
//	rather than on the states.
//
#ifndef _genBuchiAutomaton_hh_
#define _genBuchiAutomaton_hh_
#include <stack>
#include <list>
#include "bddUser.hh"
#include "natSet.hh"
#include "bdd.hh"
#include "indexedSet.hh"

class GenBuchiAutomaton : private BddUser
{
public:
  typedef pair<int, int> Key;  // (state index, fairness set index)
  typedef pair<Key, Bdd> FairTransition;
  typedef map<Key, Bdd> FairTransitionSet;

  GenBuchiAutomaton(LogicFormula* formula, int top);

  void simplify();
  bool satSolve(list<Bdd>& leadIn, list<Bdd>& cycle);
  int getNrStates() const;
  int getNrFairnessSets() const;
  const NatSet& getInitialStates() const;
  const NatSet& getFairnessCombination(int fairnessIndex) const;
  const FairTransitionSet& getTransitions(int stateNr) const;
  void dump(ostream& s) const;

private:
  typedef IndexedSet<NatSet> NatSetTable;
  typedef IndexedSet<FairTransitionSet> FairTransitionSetTable;

  void generateState(int index);
  int getStateIndex(const NatSet& stateSet);
  void insertFairTransition(FairTransitionSet& fts, const FairTransition& ft);
  void insertFairTransition2(FairTransitionSet& fts, const FairTransition& ft);
  void transformFairTransitionSet(FairTransitionSet& transformed,
				  const FairTransitionSet& original);
  void maximallyCollapseStates();
  void collapseStates();

  void sccOptimizations();
  void sccAnalysis();
  int strongConnected(int v);
  void handleComponent(int component);

  void eliminateFairness(FairTransitionSet& transformed,
			 const FairTransitionSet& original,
			 const Vector<int>& stateMap);
  void transformFairTransitionSet2(NatSetTable& oldFairnessConditions,
				   FairTransitionSet& transformed,
				   const FairTransitionSet& original,
				   const Vector<int>& stateMap,
				   const Vector<int>& fairMap,
				   int component);
  void remapNatSet(NatSet& newSet,
		   const NatSet& oldSet,
		   const Vector<int>& natMap);

  void bfsToFairComponent(int& fairState, list<Bdd>& path) const;
  void bfsToMoreFairness(NatSet& fairness, int& start, list<Bdd>& path) const;
  void bfsToTarget(int start, int target, list<Bdd>& path) const;

  NatSet initialStates;
  Vector<int> states;
  FairTransitionSetTable fairTransitionSets;
  int nrFairnessSets;
  NatSetTable fairnessConditions;

  //
  //	Temporary data used during construction.
  //
  VeryWeakAlternatingAutomaton* vwaa;
  NatSetTable vwaaStateSets;
  //
  //	Structures used for SCC Optimizations.
  //
  struct StateInfo
  {
    int traversalNumber;	// DFS traversal number
    int component;		// number of the component we're in
  };

  enum ComponentStatus
  {
    DEAD,	// unfair and can't reach a fair SCC
    UNFAIR,	// unfair and can reach a fair SCC
    FAIR	// has internal transitions covering all fairness sets
  };

  struct ComponentInfo
  {
    ComponentStatus status;
    NatSet redundant;		// redundant fairness sets for a fair component
  };

  int traversalCount;
  int componentCount;
  stack<int> stateStack;
  Vector<StateInfo> stateInfo;
  Vector<ComponentInfo> componentInfo;
  NatSet allFair;
  NatSet essential;
  //
  //	Stucture used for sat solving.
  //
  struct Step;
};

inline int
GenBuchiAutomaton::getNrStates() const
{
  return states.length();
}

inline int
GenBuchiAutomaton::getNrFairnessSets() const
{
  return nrFairnessSets;
}

inline const NatSet&
GenBuchiAutomaton::getInitialStates() const
{
  return initialStates;
}

inline const NatSet&
GenBuchiAutomaton::getFairnessCombination(int fairnessIndex) const
{
  return fairnessConditions.ithElement(fairnessIndex);
}

inline const GenBuchiAutomaton::FairTransitionSet&
GenBuchiAutomaton::getTransitions(int stateNr) const
{
  return fairTransitionSets.ithElement(states[stateNr]);
}

#endif
