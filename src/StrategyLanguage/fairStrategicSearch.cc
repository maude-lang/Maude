/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2006 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class FairStrategicSearch.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"

//      core class definitions
#include "rewritingContext.hh"

//	strategy language class definitions
#include "fairStrategicSearch.hh"
#include "decompositionProcess.hh"

FairStrategicSearch::FairStrategicSearch(RewritingContext* initial, StrategyExpression* strategy)
  : StrategicSearch(initial, strategy)
{
  nextToRun = new DecompositionProcess(insert(initial->root()),
				       push(EMPTY_STACK, strategy),
				       getDummyExecution(),
				       0);
}

DagNode*
FairStrategicSearch::findNextSolution()
{
  solutionIndex = NONE;
  while (!exhausted)
    {
      //
      //	Run can delete any process except nextToRun, which must
      //	request deletion by returning DIE.
      //
      Survival s = nextToRun->run(*this);
      //
      //	t will always be a valid process, but might be equal to
      //	nextToRun in the degenerate case.
      //
      StrategicProcess* t = nextToRun->getNextProcess();
      Assert(s == SURVIVE || t != nextToRun || exhausted, "ran out of processes without exhausted being set");
      //
      //	Now safe to delete nextToRun if it requested deletion.
      //
      if (s == DIE)
	delete nextToRun;
      //
      //	Now safe to abort.
      //
      if (RewritingContext::getTraceStatus() && initial->traceAbort())
	break;
      //
      //	t may not be valid anymore, but in this case we should be
      //	exhausted.
      //
      nextToRun = t;
      //
      //	If the run triggered a solution we're done.
      //
      if (solutionIndex != NONE)
	return getCanonical(solutionIndex);
    }
  return 0;
}
