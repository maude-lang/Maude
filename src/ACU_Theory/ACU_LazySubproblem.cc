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
//      Implementation for class ACU_LazySubproblem.
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
//#include "ACU_DagNode.hh"
#include "ACU_TreeDagNode.hh"
//#include "ACU_ExtensionInfo.hh"
#include "ACU_LazySubproblem.hh"

ACU_LazySubproblem::ACU_LazySubproblem(ACU_BaseDagNode* subject,
				       const ACU_Tree& remaining,
				       const Substitution& substitution,
				       LhsAutomaton* const stripperAutomaton,
				       Term* const stripperTerm,
				       int collectorVarIndex,
				       Sort* collectorSort)
  : subject(subject),
    remaining(remaining),
    matchTime(substitution.nrFragileBindings()),
    stripperAutomaton(stripperAutomaton),
    stripperTerm(stripperTerm),
    collectorVarIndex(collectorVarIndex),
    collectorSort(collectorSort),
    local(substitution.nrFragileBindings())
{
  matchTime.copy(substitution);
  previousIndex = NONE;
}

ACU_LazySubproblem::~ACU_LazySubproblem()
{
  FOR_EACH_CONST(i, Vector<Alternative>, previous)
    {
      delete i->difference;
      delete i->subproblem;
    }
}

bool
ACU_LazySubproblem::solve(bool findFirst, RewritingContext& solution)
{
  if (findFirst)
    {
      if (previousIndex == NONE)
	{
	  previousIndex = 0;
	  if (!remaining.findFirstPotentialMatch(stripperTerm,
						 matchTime,
						 currentPath))
	  return false;
	}
      else
	{
	  if (previous.empty())
	    return false;
	  previousIndex = 0;
	}
    }
  else
    {
      //
      //	Try to solve last used match in a new way.
      //
      Alternative& a = previous[previousIndex];
      if (a.subproblem != 0 && a.subproblem->solve(false, solution))
	return true;
      a.difference->retract(solution);
      ++previousIndex;
    }
  //
  //	Try the next previously generated match.
  //
  int nrPrevious = previous.length();
  for (; previousIndex < nrPrevious; ++previousIndex)
    {
      Alternative& a = previous[previousIndex];
      if (a.difference->assert(solution))
	{
	  if (a.subproblem == 0 || a.subproblem->solve(true, solution))
	    return true;
	  a.difference->retract(solution);
	}
    }
  //
  //	Try to generate new matches.
  //
  for (; currentPath.valid(); currentPath.next())
    {
      Subproblem* sp;
      local.copy(matchTime);
      if (stripperAutomaton->match(currentPath.getDagNode(), local, sp) &&
	  bindCollector(solution))
	{
	  previousIndex = previous.length();
	  previous.resize(previousIndex + 1);
	  Alternative& a = previous[previousIndex];
	  a.subproblem = sp;
	  a.difference = local - matchTime;
	  if (a.difference->assert(solution))
	    {
	      if (sp == 0 || sp->solve(true, solution))
		{
		  currentPath.next();  // must not try the same argument again
		  return true;
		}
	      a.difference->retract(solution);
	    }
	}
    }
  return false;
}

bool
ACU_LazySubproblem::bindCollector(RewritingContext& solution)
{
  ACU_Tree t(remaining);
  t.deleteMult2(currentPath, 1);
  Assert(t.getSize() != 0, "no arguments left");

  DagNode* d;
  if (t.getSize() == 1 && t.getMaxMult() == 1)
    {
      //
      //	Only one argument left for collector.
      //
      d = t.getSoleDagNode();
      if (!leq(d->getSortIndex(), collectorSort))
	return false;
    }
  else
    {
      //
      //	General case.
      //
      d = new ACU_TreeDagNode(subject->symbol(), t);
      if (!(d->checkSort(collectorSort, solution)))
	return false;
      if (subject->isReduced() && d->getSortIndex() != Sort::SORT_UNKNOWN)
	d->setReduced();
    }
  local.bind(collectorVarIndex, d);
  return true;
}

void
ACU_LazySubproblem::markReachableNodes()
{
  remaining.mark();
}

#ifdef DUMP

void
ACU_LazySubproblem::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << "begin{ACU_LazySubproblem}\n";
  s << "end{ACU_LazySubproblem}\n";
}

#endif
