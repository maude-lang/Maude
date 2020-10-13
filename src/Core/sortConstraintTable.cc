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
//      Implementation for abstract class RuleTable.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "subproblem.hh"
#include "extensionInfo.hh"

//      core class definitions
#include "substitution.hh"
#include "rewritingContext.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"
#include "variableInfo.hh"
#include "preEquation.hh"
#include "sortConstraint.hh"
#include "sortConstraintTable.hh"

SortConstraintTable::SortConstraintTable()
{
  tableComplete = false;
}

local_inline bool
SortConstraintTable::sortConstraintLt(SortConstraint* const& sc1,
				      SortConstraint* const& sc2)
{
  //
  //	Sort constraints are sorted in the order: largest index (smallest sort) first
  //
  return sc2->getSort()->index() < sc1->getSort()->index();  // reverse order
  // IDEA: might want to weaken comparison and do a stable_sort()
  // BUT: stable_sort() requires a strict weak ordering - much stronger that
  // the partial ordering we have on sorts.
}

void
SortConstraintTable::orderSortConstraints()
{
  //
  //	sortConstraints may contain sort constraints with variable lhs which have
  //	too low a sort to ever match our symbol. However the sort of our symbol
  //	is itself affected by sort constraints. So we "comb" out usable sort
  //	constraints in successive passes; this is inefficient but we expect the number
  //	of sort constraints to be very small so it's not worth doing anything smarter.
  //
  tableComplete = true;  // not really complete until we've finished, but pretend it is
  int nrSortConstraints = sortConstraints.length();
  if (nrSortConstraints == 0)
    return;
  Vector<SortConstraint*> all;
  all.swap(sortConstraints);
  bool addedSortConstraint;
  do
    {
      addedSortConstraint = false;
      for (int i = 0; i < nrSortConstraints; i++)
	{
	  SortConstraint* sc = all[i];
	  //
	  //	Because we set tableComplete = true; acceptSortConstraint() may
	  //	inspect the table of sortConstraints accepted so far and make
	  //	a finer distinction than it could in offerSortConstraint().
	  //
	  if (sc != 0 && acceptSortConstraint(sc))
	    {
	      sortConstraints.append(sc);
	      all[i] = 0;
	      addedSortConstraint = true;
	    }
	}
    }
  while (addedSortConstraint);
  sort(sortConstraints.begin(), sortConstraints.end(), sortConstraintLt);
}

void
SortConstraintTable::compileSortConstraints()
{
  int nrConstraints = sortConstraints.length();
  for (int i = 0; i < nrConstraints; i++)
    sortConstraints[i]->compile(true);
}

void
SortConstraintTable::constrainToSmallerSort2(DagNode* subject, RewritingContext& context)
{
  if (context.isLimited())
    {
      //
      //	Limited rewriting contexts don't support sort constraint application and
      //	are only used for functionality that doesn't support sort constraints.
      //
      DebugInfo("ignoring sort constraints for " << subject << " because context is limited");
      return;
    }
  int currentSortIndex = subject->getSortIndex();
  Subproblem* sp;
  //
  //	We try sort constraints, smallest sort first until one applies or
  //	all remaining sort constraints have sort >= than our current sort.
  //	Whenever we succeed in applying a sort constraint we start again
  //	with the new sort, because earlier sort constraints (via collapse
  //	or variable lhs patterns) may be able to test this new sort.
  //
  int nrConstraints = sortConstraints.length();
retry:
  for (int i = 0; i < nrConstraints; i++)
    {
      SortConstraint* sc = sortConstraints[i];
      Sort* s = sc->getSort();
      if (leq(currentSortIndex, s))
	break;
      if (leq(s, currentSortIndex))  // not equal because of previous test
	{
	  int nrVariables = sc->getNrProtectedVariables();
	  context.clear(nrVariables);
	  if (sc->getLhsAutomaton()->match(subject, context, sp, 0))
	    {
	      if (sp == 0 || sp->solve(true, context))
		{
		  if (!(sc->hasCondition()) || sc->checkCondition(subject, context, sp))
		    {
		      delete sp;
		      if (RewritingContext::getTraceStatus())
			{
			  context.tracePreScApplication(subject, sc);
			  if (context.traceAbort())
			    {
			      context.finished();
			      return;
			    }
			}
		      context.incrementMbCount();
		      context.finished();
		      MemoryCell::okToCollectGarbage();
		      currentSortIndex = s->index();
		      subject->setSortIndex(currentSortIndex);
		      goto retry;
		    }
		}
	      delete sp;
	    }
	  context.finished();
	  MemoryCell::okToCollectGarbage();
	}
    }
}
