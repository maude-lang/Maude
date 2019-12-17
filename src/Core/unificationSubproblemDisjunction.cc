/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2008 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class CUI_UnificationSubproblem.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "dagNode.hh"

//      core class definitions
#include "unificationSubproblemDisjunction.hh"

void
UnificationSubproblemDisjunction::addUnification(DagNode* lhs, DagNode* rhs, bool /* marked */, UnificationContext& /* solution */)
{
  int nrProblems = problems.size();
  problems.expandBy(1);
  problems[nrProblems].lhs = lhs;
  problems[nrProblems].rhs = rhs;
}

bool
UnificationSubproblemDisjunction::solve(bool findFirst, UnificationContext& /* solution */, PendingUnificationStack& pending)
{
  //
  //	We only push problems on the pending stack and restore the pending stack.
  //	We don't touch the substitution so we don't need to save or restore it.
  //
  int nrProblems = problems.size();
  int i;
  if (findFirst)
    i = 0;
  else
    {
      for (i = nrProblems - 1;; --i)
	{
	  TheoryClash& p = problems[i];
	  if (p.lhsControlling)
	    {
	      //
	      //	Restore the state to what it was before we pushed the
	      //	problem with the lhs controlling.
	      //
	      pending.restore(p.savedPendingState);
	      p.lhsControlling = false;
	      pending.push(p.rhs->symbol(), p.rhs, p.lhs, true);  // swap lhs and rhs
	      ++i;
	      break;
	    }
	  if (i == 0)
	    {
	      //
	      //	All combinations exhausted; restore intial state.
	      //
	      pending.restore(p.savedPendingState);
	      return false;
	    }
	}
    }
  //
  //	Give lhs control of any remaining problems.
  //
  for (; i < nrProblems; ++i)
    {
      TheoryClash& p = problems[i];
      p.savedPendingState = pending.checkPoint();
      p.lhsControlling = true;
      pending.push(p.lhs->symbol(), p.lhs, p.rhs, true);
    }
  return true;
}
