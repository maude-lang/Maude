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
//	Implementation for class RewriteConditionState.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"

//	interface class definitions
//#include "symbol.hh"
//#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "subproblem.hh"

//	core class definitions
#include "rewritingContext.hh"

//	higher class definitions
#include "rewriteConditionState.hh"

RewriteConditionState::RewriteConditionState(RewritingContext& original,
					     DagNode* lhsInstance,
					     LhsAutomaton* matcher)
  : stateGraph(original.makeSubcontext(lhsInstance, RewritingContext::CONDITION_EVAL)),
    matcher(matcher),
    saved(original.nrFragileBindings())
{
  saved.copy(original);
  subproblem = 0;
  explore = -1;
  arcNr = -1;
}

RewriteConditionState::~RewriteConditionState()
{
  delete subproblem;
}

bool
RewriteConditionState::solve(bool findFirst, RewritingContext& solution)
{
  //
  //	See if there is another solution to last match.
  //
  if (!findFirst)
    {
      if (subproblem != 0)
	{
	  if (subproblem->solve(false, solution))
	    return true;
	  delete subproblem;
	  subproblem = 0;
	}
      solution.copy(saved);  // restore substitution to pre-match condition
    }
  //
  //	Look for other reachable states to match.
  //
  for(;;)
    {
      int stateNr = findNextState();
      stateGraph.transferCountTo(solution);
      if (stateNr == NONE)
	break;
      if (matcher->match(stateGraph.getStateDag(stateNr), solution, subproblem))
	{
	  if (subproblem == 0 || subproblem->solve(true, solution))
	    return true;
	  delete subproblem;
	  subproblem = 0;
	}
      solution.copy(saved);  // restore substitution to pre-match condition
    }
  return false;
}

int
RewriteConditionState::findNextState()
{
  if (explore == -1)  // special case - return initial state
    {
      explore = 0;
      return 0;
    }

  int nrStates = stateGraph.getNrStates();
  for(; explore != nrStates; explore++)
    {
      for (;;)
	{
	  ++arcNr;
	  int stateNr = stateGraph.getNextState(explore, arcNr);
	  if (stateNr == NONE)
	    {
	      if (stateGraph.getContext()->traceAbort())
		return NONE;
	      break;
	    }
	  if (stateNr == nrStates)
	    return stateNr;
	}
      arcNr = -1;
    }
  return NONE;
}
