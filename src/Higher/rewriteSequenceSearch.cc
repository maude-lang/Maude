/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class RewriteSequenceSearch.
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
#include "rewriteSequenceSearch.hh"

RewriteSequenceSearch::RewriteSequenceSearch(RewritingContext* initial,
					     SearchType searchType,
					     Pattern* goal,
					     int maxDepth)
  : StateTransitionGraph(initial),
    goal(goal),
    maxDepth((searchType == ONE_STEP) ? 1 : maxDepth)
{
  matchState = 0;
  explore = -1;
  exploreDepth = -1;
  firstDeeperNodeNr = 0;
  returnedStateAlready = false;
  needToTryInitialState = (searchType == ANY_STEPS);
  reachingInitialStateOK = (searchType == AT_LEAST_ONE_STEP || searchType == ONE_STEP);
  normalFormNeeded = (searchType == NORMAL_FORM);
  branchNeeded = (searchType == BRANCH);
  nextArc = NONE;
}

RewriteSequenceSearch::~RewriteSequenceSearch()
{
  delete matchState;
  delete goal;
}

bool
RewriteSequenceSearch::findNextMatch()
{
  if (matchState != 0)
    goto tryMatch;  // non-startup case

  for(;;)
    {
	stateNr = findNextInterestingState();
	if (stateNr == NONE)
	  break;
	matchState = new MatchSearchState(getContext()->makeSubcontext(getStateDag(stateNr)),
					  goal,
					  MatchSearchState::GC_CONTEXT);
    tryMatch:
      bool foundMatch = matchState->findNextMatch();
      matchState->transferCountTo(*(getContext()));
      if (foundMatch)
	return true;
      delete matchState;
    }

  matchState = 0;
  return false;
}

int
RewriteSequenceSearch::findNextInterestingState()
{
  if (needToTryInitialState)
    {
      //
      //	Special case: return the initial state.
      //
      needToTryInitialState = false;  // don't do this again
      return 0;
    }

  if (nextArc != NONE)
    goto exploreArcs;

  for(;;)
    {
      //
      //	Get index of next state to explore.
      //
      ++explore;
      returnedStateAlready = false;  // needed for BRANCH search type
      if (explore == getNrStates())
	break;  // all states explored
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
      nextArc = 0;
      //
      //	Explore the arcs of the current state.
      //
    exploreArcs:
      int nrStates = getNrStates();
      int nextStateNr;
      while ((nextStateNr = getNextState(explore, nextArc)) != NONE)
	{
	  ++nextArc;
	  if (normalFormNeeded)
	    {
	      if (exploreDepth == maxDepth)
		break;  // no point looking for further arcs from this state
	    }
	  else if (branchNeeded)
	    {
	      if (!returnedStateAlready && nextArc >= 2 && nextStateNr != getNextState(explore, 0))
		{
		  returnedStateAlready = true;  // so we don't return the state again if we see another distinct next state
		  return explore;
		}
	    }
	  else
	    {
	      if (nextStateNr == nrStates)
		return nextStateNr;  // we reached a new state so return it
	      //
	      //	We reached a state that we already saw.
	      //
	      if (nextStateNr == 0 && reachingInitialStateOK)
		{
		  //
		  //	We have arrived back at our initial state, but because
		  //	we didn't try matching the initial state, we do it now.
		  //
		  reachingInitialStateOK = false;  // don't do this again
		  return 0;
		}
	    }
	}
      if (getContext()->traceAbort())
	return NONE;
      if (normalFormNeeded && nextArc == 0)
	{
	  //
	  //	No next states so we can return the state we just explored as a normal form.
	  //
	  nextArc = NONE;
	  return explore;
	}
    }
  return NONE;
}

Rule*
RewriteSequenceSearch::getStateRule(int stateNr) const
{
  const ArcMap& fwdArcs = getStateFwdArcs(getStateParent(stateNr));
  return *(fwdArcs.find(stateNr)->second.begin());
}
