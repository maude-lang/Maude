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
//	Class for Very Weak Alternating Automata.
//
//	We implement the LTL to VWAA translation and VWAA optimization
//	algorithms from:
//	  Paul Gastin and Denis Oddoux.
//	  "Fast LTL to Buchi Automaton Translation",
//	  CAV 2001, LNCS 2102, pp 53-65, 2001.
//
#ifndef _veryWeakAlternationAutomaton_hh_
#define _veryWeakAlternationAutomaton_hh_
#include "bddUser.hh"
#include "transitionSet.hh"
#include "natSet.hh"

class VeryWeakAlternatingAutomaton : private BddUser
{
public:
  VeryWeakAlternatingAutomaton(LogicFormula* formula, int top);

  int getNrStates() const;
  int getNrFinalStates() const;
  const TransitionSet& getInitialStates();
  const TransitionSet& getTransitionSet(int stateIndex) const;
  void computeFairnessSet(const Transition& transition,
			  NatSet& fairnessSet);
  void dump(ostream& s) const;

private:
  void dnf(int subformulaIndex, TransitionSet& result);
  void computeTransitionSet(int subformulaIndex);
  void reachabilityOpt();
  void findReachable(const TransitionSet& ts);
  bool checkFairness(const Transition& transition, int finalStateIndex) const;

  TransitionSet initialStates;
  Vector<TransitionSet> states;
  Vector<int> finalStates;
  //
  //	Temporary data used during construction.
  //
  LogicFormula* const formula;
  int nrNewStates;
  Vector<int> renaming;
};

inline int
VeryWeakAlternatingAutomaton::getNrStates() const
{
  return states.length();
}

inline int
VeryWeakAlternatingAutomaton::getNrFinalStates() const
{
  return finalStates.length();
}

inline const TransitionSet&
VeryWeakAlternatingAutomaton::getInitialStates()
{
  return initialStates;
}

inline const TransitionSet&
VeryWeakAlternatingAutomaton::getTransitionSet(int stateIndex) const
{
  return states[stateIndex];
}

#endif
