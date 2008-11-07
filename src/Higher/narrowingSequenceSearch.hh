/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2007 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for depthfirst searching for sequences of narrowing steps.
//
#ifndef _narrowingSequenceSearch_hh_
#define _narrowingSequenceSearch_hh_
#include "sequenceSearch.hh"
#include "matchSearchState.hh"
#include "protectedDagNodeSet.hh"
#include "narrowingSearchState.hh"

class NarrowingSequenceSearch : public SequenceSearch
{
  NO_COPYING(NarrowingSequenceSearch);

public:
  //
  //	We take responsibility for deleting initial, goal and freshVariableGenerator.
  //	A null goal can be passed to accept all states whose reachability is consistent with searchType and maxDepth.
  //	In the null goal case a count is available of the number of variables in all the previously returned states.
  //	This number is needed by the metalevel.
  //
  NarrowingSequenceSearch(RewritingContext* initial,
			  SearchType searchType,
			  Pattern* goal,
			  int maxDepth,
			  int narrowingFlags,
			  FreshVariableGenerator* freshVariableGenerator);
  ~NarrowingSequenceSearch();

  bool findNextMatch();
  const Pattern* getGoal() const;
  FreshVariableGenerator* getFreshVariableGenerator() const;
  NarrowingSearchState* getState() const;
  const Substitution* getSubstitution() const;
  RewritingContext* getContext() const;
  DagNode* getStateDag() const;
  int getVariableTotalForPreviouslyReturnedStates() const;

private:
  bool findNextInterestingState();
  bool findNextNormalForm();

  RewritingContext* initial;
  Pattern* const goal;
  const int maxDepth;
  const int narrowingFlags;
  FreshVariableGenerator* freshVariableGenerator;

  bool needToTryInitialState;
  //bool reachingInitialStateOK;
  bool normalFormNeeded;
  bool topOfStackFresh;

  Vector<NarrowingSearchState*> stateStack;
  MatchSearchState* matchState;

  ProtectedDagNodeSet seenSet;

  int variableTotalForPreviouslyReturnedStates;
  int variableTotalForAllReturnedStates;
};

inline const Pattern*
NarrowingSequenceSearch::getGoal() const
{
  return goal;
}

inline FreshVariableGenerator* 
NarrowingSequenceSearch::getFreshVariableGenerator() const
{
  return freshVariableGenerator;
}

inline const Substitution*
NarrowingSequenceSearch::getSubstitution() const
{
  return matchState->getContext();
}

inline NarrowingSearchState*
NarrowingSequenceSearch::getState() const
{
  return stateStack[stateStack.size() - 1];
}

inline RewritingContext*
NarrowingSequenceSearch::getContext() const
{
  Assert(stateStack.size() > 0, "once stack is empty initial has been deleted");
  return initial;
}

inline DagNode*
NarrowingSequenceSearch::getStateDag() const
{
  return stateStack[stateStack.size() - 1]->getContext()->root();
}

inline int
NarrowingSequenceSearch::getVariableTotalForPreviouslyReturnedStates() const
{
  return variableTotalForPreviouslyReturnedStates;
}

#endif
