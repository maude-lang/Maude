/*

    This file is part of the Maude 3 strategy-aware model checker.

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
//	Class for searching for sequences of rewrites within a DAG following a strategy.
//
#ifndef _strategySequenceSearch_hh_
#define _strategySequenceSearch_hh_
#include "sequenceSearch.hh"
#include "strategyTransitionGraph.hh"
#include "matchSearchState.hh"

class StrategySequenceSearch : public SequenceSearch
{
  NO_COPYING(StrategySequenceSearch);

public:
  StrategySequenceSearch(RewritingContext* initial,
			 SearchType searchType,
			 Pattern* goal,
			 StrategyExpression* strategy,
			 int maxDepth = -1,
			 const set<int> &opaqueIds = {});
  ~StrategySequenceSearch();

  bool findNextMatch();
  const Pattern* getGoal() const;
  const StrategyTransitionGraph::Transition& getStateTransition(int stateNr) const;
  int getStateNr() const;
  DagNode* getStateDag(int stateNr) const;
  int getStateParent(int stateNr) const;
  int getRealStateNr(int stateNr) const;
  bool validState(int stateNr) const;
  const StrategyTransitionGraph::ArcMap& getStateFwdArcs(int stateNr) const;
  StrategyExpression* getStrategyContinuation(int stateNr) const;
  const Substitution* getSubstitution() const;

  // Methods that should have been inherited from StrategyTransitionGraph
  RewritingContext* getContext();
  int getNrStates() const;
  void transferCountTo(RewritingContext& recipient);

private:
  int findNextInterestingState();

  enum StateFlag {
    FAILED,
    IN_LOOP,
    IN_SOLUTION,
    PENDING
  };

  struct MoreStateInfo {
    int parent;
    int depth;
    StateFlag flag;
  };

  StateFlag exploreState(int stateNr);
  bool newSolution(int stateNr);

  // This class cannot derive both from SequenceSearch and
  // StrategyTransitionGraph because of the diamond problem with
  // CacheableState. Alternatively, we could have used virtual
  // inheritance or rethought the inheritance relations of
  // StrategyTransitionGraph and StrategicSearch.
  StrategyTransitionGraph graph;

  Pattern* const goal;
  Vector<MoreStateInfo> stateInfo;
  set<int> seenSolutions;
  const int maxDepth;
  int explore;
  int exploreDepth;
  int nextArc;
  bool needToTryInitialState;
  bool normalFormNeeded;
  MatchSearchState* matchState;
  int stateNr;
};

inline const Pattern*
StrategySequenceSearch::getGoal() const
{
  return goal;
}

inline const Substitution*
StrategySequenceSearch::getSubstitution() const
{
  return matchState->getContext();
}

inline int
StrategySequenceSearch::getStateNr() const
{
  return stateNr;
}

inline StrategyExpression*
StrategySequenceSearch::getStrategyContinuation(int stateNr) const
{
  return graph.getStrategyContinuation(stateNr);
}

inline RewritingContext*
StrategySequenceSearch::getContext()
{
  return graph.getContext();
}

inline int
StrategySequenceSearch::getNrStates() const
{
  return graph.getNrStates();
}

inline DagNode*
StrategySequenceSearch::getStateDag(int stateNr) const
{
  return graph.getStateDag(stateNr);
}

inline int
StrategySequenceSearch::getStateParent(int stateNr) const
{
  return stateInfo[stateNr].parent;
}

inline const StrategyTransitionGraph::ArcMap&
StrategySequenceSearch::getStateFwdArcs(int stateNr) const
{
  return graph.getStateFwdArcs(stateNr);
}

inline int
StrategySequenceSearch::getRealStateNr(int stateNr) const
{
  return graph.getRealStateNr(stateNr);
}

inline bool
StrategySequenceSearch::validState(int stateNr) const {
  return (stateInfo[stateNr].flag == IN_SOLUTION || stateInfo[stateNr].flag == IN_LOOP)
	  && getRealStateNr(stateNr) == stateNr;
}

inline void
StrategySequenceSearch::transferCountTo(RewritingContext& recipient) {
  graph.transferCount(recipient);
}

#endif
