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
#include "ACU_TreeDagNode.hh"
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
  for (const Alternative& i : previous)
    {
      delete i.difference;
      delete i.subproblem;
    }
}

bool
ACU_LazySubproblem::solve(bool findFirst, RewritingContext& solution)
{
  if (findFirst)
    {
      //
      //	We're asked to find the first match with current global solution.
      //	This doens't mean that we haven't found and saved local matches
      //	before, on behalf of a previous global solution.
      //
      if (previousIndex == NONE)
	{
	  //
	  //	This really is the first time, so initialize path to subject to strip.
	  //
	  previousIndex = 0;
	  if (!remaining.findFirstPotentialMatch(stripperTerm,
						 matchTime,
						 currentPath))
	  return false;
	}
      else
	{
	  //
	  //	This isn't the first time, so look at saved local solutions.
	  //
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
  //	Try to generate new matches; this will usually be on behalf of the
  //	first global solution, but it is legal to start with findFirst = true
  //	with a new global solution, even if the we didn't exhaust matches
  //	for the previous global solution.
  //
  for (; currentPath.valid(); currentPath.next())
    {
      //
      //	In order for saved local matches to be valid wrt any global
      //	solution we may be given, we can only use bindings that were
      //	available at match time.
      //
      local.copy(matchTime);
      DagNode* d = currentPath.getDagNode();
      if (stripperTerm->partialCompare(local, d) == Term::LESS)
	{
	  //
	  //	All instantiations of stripperTerm consistent with local are
	  //	less than d, and therefore will be less than nodes further to the right.
	  //	Thus we have gone pass the last potential match and can give up.
	  //
	  break;
	}
      Subproblem* sp;
      if (stripperAutomaton->match(d, local, sp) &&
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
  //
  //	It's possible that matching the stripper could have bound the collector,
  //	since we are in the "FULL" case and not insisting on linearity.
  //
  if (DagNode* c = local.value(collectorVarIndex))
    {
      DebugAdvisory("ACU_LazySubproblem::bindCollector() Collector variable bound to " << d);
      return c->equal(d);
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
