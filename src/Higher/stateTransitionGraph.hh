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
//	Class for building a state transition graph on-the-fly, with hash consing.
//
#ifndef _stateTransitionGraph_hh_
#define _stateTransitionGraph_hh_
#include <set>
#include <map>
#include "hashConsSet.hh"
#include "rewritingContext.hh"

class StateTransitionGraph
{
  NO_COPYING(StateTransitionGraph);

public:
  typedef map<int, set<Rule*> > ArcMap;

  StateTransitionGraph(RewritingContext* initial);
  ~StateTransitionGraph();

  int getNrStates() const;
  int getNextState(int stateNr, int index);
  DagNode* getStateDag(int stateNr) const;
  const ArcMap& getStateFwdArcs(int stateNr) const;
  //
  //	Stuff needed for search.
  //
  RewritingContext* getContext();
  void transferCountTo(RewritingContext& recipient);
  int getStateParent(int stateNr) const;

private:
  struct State
  {
    State(int hashConsIndex, int parent);

    const int hashConsIndex;
    const int parent;
    Vector<int> nextStates;
    RewriteSearchState* rewriteState;
    bool fullyExplored;
    ArcMap fwdArcs;
  };

  void insertNewState(int parent);

  RewritingContext* initial;
  Vector<State*> seen;
  Vector<int> hashCons2seen;  // partial map of hashCons indices to state indices
  HashConsSet hashConsSet;
};

inline
StateTransitionGraph::State::State(int hashConsIndex, int parent)
  : hashConsIndex(hashConsIndex),
    parent(parent)
{
  rewriteState = 0;
  fullyExplored = false;
}

inline int
StateTransitionGraph::getNrStates() const
{
  return seen.length();
}

inline DagNode*
StateTransitionGraph::getStateDag(int stateNr) const
{
  return hashConsSet.getCanonical(seen[stateNr]->hashConsIndex);
}

inline const StateTransitionGraph::ArcMap&
StateTransitionGraph::getStateFwdArcs(int stateNr) const
{
  return seen[stateNr]->fwdArcs;
}

inline RewritingContext*
StateTransitionGraph::getContext()
{
  return initial;
}

inline void
StateTransitionGraph::transferCountTo(RewritingContext& recipient)
{
  recipient.transferCountFrom(*initial);
}

inline int
StateTransitionGraph::getStateParent(int stateNr) const
{
  return seen[stateNr]->parent;
}

#endif
