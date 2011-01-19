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
//	Class for searching for sequences of rewrites within a DAG.
//
#ifndef _rewriteSequenceSearch_hh_
#define _rewriteSequenceSearch_hh_
#include "sequenceSearch.hh"
#include "stateTransitionGraph.hh"
#include "matchSearchState.hh"

class RewriteSequenceSearch : public SequenceSearch, public StateTransitionGraph
{
  NO_COPYING(RewriteSequenceSearch);

public:
  RewriteSequenceSearch(RewritingContext* initial,
			SearchType searchType,
			Pattern* goal,
			int maxDepth = -1);
  ~RewriteSequenceSearch();

  bool findNextMatch();
  const Pattern* getGoal() const;
  Rule* getStateRule(int stateNr) const;
  int getStateNr() const;
  const Substitution* getSubstitution() const;
  
private:
  int findNextInterestingState();

  Pattern* const goal;
  const int maxDepth;
  int explore;
  int exploreDepth;
  int firstDeeperNodeNr;
  int nextArc;
  bool needToTryInitialState;
  bool reachingInitialStateOK;
  bool normalFormNeeded;
  MatchSearchState* matchState;
  int stateNr;
};

inline const Pattern*
RewriteSequenceSearch::getGoal() const
{
  return goal;
}

inline const Substitution*
RewriteSequenceSearch::getSubstitution() const
{
  return matchState->getContext();
}

inline int
RewriteSequenceSearch::getStateNr() const
{
  return stateNr;
}

#endif
