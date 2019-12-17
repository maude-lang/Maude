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
//	Implementation for class AssignmentConditionState.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
//#include "symbol.hh"
//#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "subproblem.hh"

//	core class definitions
#include "rewritingContext.hh"

//	higher class definitions
#include "assignmentConditionState.hh"

AssignmentConditionState::AssignmentConditionState(RewritingContext& original,
						   LhsAutomaton* matcher,
						   DagNode* rhsInstance)
  : saved(original.nrFragileBindings())
{
  saved.copy(original);
  rhsContext = original.makeSubcontext(rhsInstance, RewritingContext::CONDITION_EVAL);
  rhsContext->reduce();
  original.addInCount(*rhsContext);
  subproblem = 0;
  succeeded = matcher->match(rhsContext->root(), original, subproblem);
}

AssignmentConditionState::~AssignmentConditionState()
{
  delete subproblem;
  delete rhsContext;
}

bool
AssignmentConditionState::solve(bool findFirst, RewritingContext& solution)
{
  if (succeeded)
    {
      if (subproblem == 0)
	{
	  if (findFirst)
	    return true;
	}
      else
	{
	  if (subproblem->solve(findFirst, solution))
	    return true;
	}
    }
  solution.copy(saved);  // restore substitution to pre-match condition
  return false;
}
