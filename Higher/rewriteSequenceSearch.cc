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
//	Implementation for class RewriteSearchState.
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
  : StateTransitionGraph3(initial),
    searchType(searchType),
    goal(goal),
    maxDepth((searchType == ONE_STEP) ? 1 : maxDepth)
{
  matchState = 0;
  explore = -1;
  exploreDepth = -1;
  firstDeeperNodeNr = 0;
  needToTryInitialState = (searchType == ANY_STEPS);
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
      matchState->transferCount(*(getContext()));
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
  if (needToTryInitialState)  // special case - return initial state
    {
      needToTryInitialState = false;
      return 0;
    }

  if (nextArc != NONE)
    goto exploreArcs;

  for(;;)
    {
      //
      //	Get next state to explore.
      //
      ++explore;
      if (explore == getNrStates())
	break;
      if (explore == firstDeeperNodeNr)
	{
	  ++exploreDepth;
	  if (searchType == NORMAL_FORM)
	    {
	      if (maxDepth > 0 && exploreDepth > maxDepth)
		break;
	    }
	  else
	    {
	      if (exploreDepth == maxDepth)
		break;
	    }
	  firstDeeperNodeNr = getNrStates();
	}
      nextArc = 0;
      
    exploreArcs:
      int nrStates = getNrStates();
      int nextStateNr;
      while ((nextStateNr = getNextState(explore, nextArc)) != NONE)
	{
	  ++nextArc;
	  if (searchType == NORMAL_FORM)
	    {
	      if (exploreDepth == maxDepth)
		break;  // no point looking for further arcs
	    }
	  else
	    {
	      if (nextStateNr == nrStates)
		return nextStateNr;
	    }
	}
      if (getContext()->traceAbort())
	return NONE;
      if (searchType == NORMAL_FORM && nextArc == 0)
	{
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
