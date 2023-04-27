/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class ACU_TreeVariableSubproblem.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "ACU_RedBlack.hh"
#include "ACU_Theory.hh"

//      interface class definitions
#include "argVec.hh"
#include "associativeSymbol.hh"
#include "dagNode.hh"
#include "lhsAutomaton.hh"
#include "subproblem.hh"

//      core class definitions
#include "variableInfo.hh"
#include "substitution.hh"
#include "rewritingContext.hh"
#include "localBinding.hh"

//	ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_TreeDagNode.hh"
#include "ACU_TreeVariableSubproblem.hh"

ACU_TreeVariableSubproblem::ACU_TreeVariableSubproblem(ACU_BaseDagNode* subject,
						       const ACU_Tree& remaining,
						       int stripperVarIndex,
						       Sort* stripperSort,
						       int collectorVarIndex,
						       Sort* collectorSort)
  : subject(subject),
    remaining(remaining),
    stripperVarIndex(stripperVarIndex),
    stripperSort(stripperSort),
    collectorVarIndex(collectorVarIndex),
    collectorSort(collectorSort),
    currentPath(remaining)
{
  previous.reserve(remaining.getSize());
  previousIndex = NONE;
}

ACU_TreeVariableSubproblem::~ACU_TreeVariableSubproblem()
{
  for (LocalBinding* i : previous)
    delete i;
}

bool
ACU_TreeVariableSubproblem::solve(bool findFirst, RewritingContext& solution)
{
  if (findFirst)
    {
      //
      //	We're asked to find the first match with current global solution.
      //	This doens't mean that we haven't found and saved local matches
      //	before, on behalf of a previous global solution.
      //
      if (previousIndex != NONE && previous.empty())
	return false;  // there were no local matches
      previousIndex = 0;
    }
  else
    {
      //
      //	Undo last solution.
      //
      previous[previousIndex]->retract(solution);
      ++previousIndex;
    }
  //
  //	Try the next previously generated match.
  //
  int nrPrevious = previous.size();
  for (; previousIndex < nrPrevious; ++previousIndex)
    {
      if (previous[previousIndex]->assert(solution))
	return true;
    }
  //
  //	Try to generate new matches; this will usually be on behalf of the
  //	first global solution, but it is legal to start with findFirst = true
  //	with a new global solution, even if the we didn't exhaust matches
  //	for the previous global solution.
  //
  for (; currentPath.valid(); currentPath.next())
    {
      //
      //	We general local bindings directly; we potentially need all of
      //	them regardless of whether they are compatible with the current
      //	global solution we were given.
      //
      DagNode* d = currentPath.getDagNode();
      if (leq(d->getSortIndex(), stripperSort))
	{
	  if (DagNode* c = collect(solution))
	    {
	      LocalBinding* b = new LocalBinding(2);
	      b->addBinding(stripperVarIndex, d);
	      b->addBinding(collectorVarIndex, c);
	      previous.push_back(b);
	      if (b->assert(solution))
		{
		  currentPath.next();  // must not try the same argument again
		  return true;
		}
	    }
	}
    }
  return false;
}

DagNode*
ACU_TreeVariableSubproblem::collect(RewritingContext& solution)
{
  //
  //	Take a copy of the remaining arguments tree.
  //
  ACU_Tree t(remaining);
  //
  //	Constructively deleted one copy of argument at current path.
  //	There should still be at least one argument left.
  //
  t.deleteMult2(currentPath, 1);
  Assert(t.getSize() != 0, "no arguments left");

  DagNode* d;
  if (t.getSize() == 1 && t.getMaxMult() == 1)
    {
      //
      //	Only one argument left for collector, so no ACU function symbol on top.
      //
      d = t.getSoleDagNode();
      if (!leq(d->getSortIndex(), collectorSort))
	return nullptr;
    }
  else
    {
      //
      //	General case. Make an ACU term with the arguments in t.
      //
      d = new ACU_TreeDagNode(subject->symbol(), t);
      if (!(d->checkSort(collectorSort, solution)))
	return nullptr;
      if (subject->isReduced() && d->getSortIndex() != Sort::SORT_UNKNOWN)
	d->setReduced();
    }
  return d;
}

void
ACU_TreeVariableSubproblem::markReachableNodes()
{
  remaining.mark();
}

#ifdef DUMP

void
ACU_TreeVariableSubproblem::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << "begin{ACU_TreeVariableSubproblem}\n";
  s << "end{ACU_TreeVariableSubproblem}\n";
}

#endif
