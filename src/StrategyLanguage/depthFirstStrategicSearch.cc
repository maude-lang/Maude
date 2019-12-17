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
//      Implementation for class DepthFirstStrategicSearch.
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
#include "depthFirstStrategicSearch.hh"
#include "decompositionProcess.hh"

DepthFirstStrategicSearch::DepthFirstStrategicSearch(RewritingContext* initial,
						     StrategyExpression* strategy)
  : StrategicSearch(initial, strategy)
{
  DecompositionProcess* firstToRun =
    new DecompositionProcess(insert(initial->root()),
			     push(EMPTY_STACK, strategy),
			     getDummyExecution(),
			     0);
  //
  // stackTop is a ghost node to access the top of the process stack.
  // It must never be run. The stack is empty is stackTop->getNextProcess()
  // is stackTop.
  //
  stackTop = new DecompositionProcess(0, 0, this, firstToRun);
}

DepthFirstStrategicSearch::~DepthFirstStrategicSearch()
{
  delete stackTop;
}

DagNode*
DepthFirstStrategicSearch::findNextSolution()
{
  //
  // The implementation of this function is the same as findNextSolution
  // in StrategicSearch except for the election of the next process to be run.
  //

  solutionIndex = NONE;
  while (stackTop->getNextProcess() != stackTop)
    {
      StrategicProcess* current = stackTop->getNextProcess();
      Survival s = current->run(*this);
      //
      //	The newly created processes are always placed at the left of the
      //	current process.
      //
      if (s == DIE)
	delete current;
      if (RewritingContext::getTraceStatus() && initial->traceAbort())
	break;
      if (solutionIndex != NONE)
	return getCanonical(solutionIndex);
    }
  exhausted = true;
  return 0;
}
