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
//	Implementation for class StrategySequenceSearch.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "pattern.hh"
#include "rewriteSearchState.hh"
#include "strategyLanguage.hh"
#include "strategyExpression.hh"
#include "strategySequenceSearch.hh"

StrategySequenceSearch::StrategySequenceSearch(RewritingContext* initial,
					       SearchType searchType,
					       Pattern* goal,
					       StrategyExpression* strategy,
					       int maxDepth,
					       const set<int>& opaques)
  : graph(initial, strategy, opaques, true, false),
    goal(goal),
    maxDepth((searchType == ONE_STEP) ? 1 : maxDepth)
{
  initial->reduce();
  matchState = nullptr;
  explore = -1;
  exploreDepth = -1;
  firstDeeperNodeNr = 0;
  returnedStateAlready = false;
  returnedStateDag = NONE;
  needToTryInitialState = (searchType == ANY_STEPS);
  normalFormNeeded = (searchType == NORMAL_FORM);
  branchNeeded = (searchType == BRANCH);
  nextArc = NONE;
  stateInfo.append({NONE, PENDING});
}

StrategySequenceSearch::~StrategySequenceSearch()
{
  delete matchState;
  delete goal;
}

bool
StrategySequenceSearch::findNextMatch()
{
  if (matchState != nullptr)
    goto tryMatch;  // non-startup case

  for(;;)
    {
      stateNr = findNextInterestingState();
      if (stateNr == NONE)
	break;
      matchState = new MatchSearchState(graph.getContext()->makeSubcontext(graph.getStateDag(stateNr)),
					goal,
					MatchSearchState::GC_CONTEXT);
    tryMatch:
      bool foundMatch = matchState->findNextMatch();
      matchState->transferCountTo(*(graph.getContext()));
      if (foundMatch)
	return true;
      delete matchState;
    }

  matchState = nullptr;
  return false;
}

int
StrategySequenceSearch::findNextInterestingState()
{
  if (needToTryInitialState)
    {
      //
      //	Special case: return the initial state.
      //
      StateFlag result = exploreState(0);

      // Execution aborted by the user
      if (result == PENDING)
	return NONE;

      needToTryInitialState = false;  // don't do this again

      if (result != FAILED)
	{
	  seenSolutions.insert(0);
	  return 0;
	}
    }

  if (nextArc != NONE)
    goto exploreArcs;

  for(;;)
    {
      //
      //	Get next state to explore.
      //
      ++explore;
      returnedStateAlready = false;  // needed for BRANCH search type
      returnedStateDag = NONE;
      if (explore == graph.getNrStates())
	break;   // all states explored
      //
      //	Are we at the first node of the next level?
      //
      if (explore == firstDeeperNodeNr)
	{
	  ++exploreDepth;
	  if (normalFormNeeded || branchNeeded)
	    {
	      //
	      //	If we're looking for a state that has a certain number of successors we need to
	      //	search one level beyond maxDepth
	      //
	      if (maxDepth != NONE && exploreDepth > maxDepth)
		break;
	    }
	  else
	    {
	      //
	      //	Otherwise we just search to maxDepth (which will never be true if maxDepth == NONE).
	      //
	      if (exploreDepth == maxDepth)
		break;
	    }
	  //
	  //	Next state generated (if there is one) will be the first node of the following level.
	  //
	  firstDeeperNodeNr = getNrStates();
	}
      //
      //	Unexplored state, it may not be valid, we should explore it
      //
      if (stateInfo[explore].flag == PENDING)
	if (exploreState(explore) == PENDING)
	  return NONE;
      //
      //	Skip invalid states
      //
      if (!validState(explore))
	continue;
      nextArc = 0;
      //
      //	Explore the arcs of the current state.
      //
    exploreArcs:
      int nrStates = graph.getNrStates();
      int nextStateNr;
      while ((nextStateNr = graph.getNextState(explore, nextArc)) != NONE)
	{
	  ++nextArc;
	  //
	  //	The state is not explored yet (it may not be a valid state), so we have to explore it now
	  //
	  if (nextStateNr == nrStates)
	    {
	      stateInfo.append({explore, PENDING});
	      nrStates = graph.getNrStates();
	    }
	  if (stateInfo[nextStateNr].flag == PENDING)
	    {
	      if (exploreState(nextStateNr) == PENDING)
		return NONE;
	      // New states may have been added by exploreState and we
	      // should keep nrStates updated to detect new states here
	      nrStates = graph.getNrStates();
	    }
	  //
	  //	Skip invalid states
	  //
	  if (!validState(nextStateNr))
	    continue;
	  //
	  //	Set parent if not yet seen
	  //
	  if (stateInfo[nextStateNr].parent == NONE)
	    stateInfo[nextStateNr].parent = explore;
	  if (normalFormNeeded)
	    {
	      if (graph.isSolutionState(explore))
		break; // the solution will be returned below
	    }
	  else if (branchNeeded)
	    {
	      //
	      //	Branch is having two different terms as successors
	      //
	      if (!returnedStateAlready)
		{
		  int thisDag = graph.getStatePoint(nextStateNr).first;
		  if (returnedStateDag == NONE)
		    returnedStateDag = thisDag;
		  else if (returnedStateDag != thisDag)
		  {
		    returnedStateAlready = true;  // so we don't return the state again if we see another distinct next state
		    if (newSolution(nextStateNr))
			return explore;
		  }
		}
	    }
	  // A new solution has been found
	  else if (newSolution(nextStateNr))
	    return nextStateNr;
	}
      if (graph.getContext()->traceAbort())
	return NONE;
      if (normalFormNeeded && graph.isSolutionState(explore))
	{
	  nextArc = NONE;
	  if (newSolution(explore))
	    return explore;
	}
    }

  return NONE;
}

StrategySequenceSearch::StateFlag
StrategySequenceSearch::exploreState(int stateNr)
{
  int nextStateNr = 0;
  int index = 0;
  StateFlag currentFlag = FAILED;

  // Interruption aborted by the user
  if (graph.getContext()->traceAbort())
    {
      stateInfo[stateNr].flag = PENDING;
      return PENDING;
    }

  // The state flag is set to in-loop for the depth-first searh to find loops
  stateInfo[stateNr].flag = IN_LOOP;

  // Explore the successors of the current state in depth until a solution is found
  while (currentFlag < IN_SOLUTION && (nextStateNr = graph.getNextState(stateNr, index++)) != NONE)
    {
      // The successor is a new state
      if (nextStateNr == stateInfo.size())
	stateInfo.append({NONE, PENDING});

      // Check whether the current state is a solution of the strategy
      if (graph.isSolutionState(stateNr))
	  currentFlag = IN_SOLUTION;

      // The next state need to be explored
      else
	{
	  if (stateInfo[nextStateNr].flag == PENDING)
	    exploreState(nextStateNr);
	  switch (stateInfo[nextStateNr].flag)
	    {
	      // A failed successor does not change the current flag
	      case FAILED:
		break;
	      // The successor is in the path to a solution,
	      // so we are in the path to a solution
	      case IN_SOLUTION:
		currentFlag = IN_SOLUTION;
		break;
	      // The successor is in the path to a loop or in a loop,
	      // so we are in the same situation
	      case IN_LOOP:
		  currentFlag = IN_LOOP;
		break;
	      // The exploration has been aborted by the user, so
	      // we interrupt the execution too
	      case PENDING:
		currentFlag = PENDING;
		break;
	    }
	}
    }

  // Check whether the current state is a solution of the strategy
  if (currentFlag != PENDING && graph.isSolutionState(stateNr))
   currentFlag = IN_SOLUTION;

  stateInfo[stateNr].flag = currentFlag;
  return currentFlag;
}

inline bool
StrategySequenceSearch::newSolution(int stateNr)
{
  return seenSolutions.insert(graph.getStatePoint(stateNr).first).second;
}

const StrategyTransitionGraph::Transition&
StrategySequenceSearch::getStateTransition(int stateNr) const
{
  const StrategyTransitionGraph::ArcMap& fwdArcs = getStateFwdArcs(stateInfo[stateNr].parent);
  return *fwdArcs.at(stateNr).begin();
}
