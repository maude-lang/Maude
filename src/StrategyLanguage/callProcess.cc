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
//      Implementation for the process controlling the matchrew main pattern search.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "strategyLanguage.hh"

//	interface class definitions
#include "term.hh"
#include "extensionInfo.hh"

//	core class definitions
#include "rewriteStrategy.hh"
#include "strategyDefinition.hh"

//	higher class definitions
#include "searchState.hh"

//	strategy language class definitions
#include "strategicSearch.hh"
#include "callProcess.hh"
#include "callTask.hh"


CallProcess::CallProcess(RewriteStrategy* namedStrategy,
			 RewritingContext* callContext,
			 int subjectDagIndex,
			 StrategyStackManager::StackId pending,
			 StrategicExecution* taskSibling,
			 StrategicProcess* insertionPoint)
  : StrategicProcess(taskSibling, insertionPoint),
    strategy(namedStrategy),
    callContext(callContext),
    search(callContext, SearchState::GC_SUBSTITUTION | SearchState::GC_CONTEXT, 0, 0),
    subjectDagIndex(subjectDagIndex),
    pending(pending),
    defIndex(0),
    first(true)
{
  Assert(callContext->root() != 0, "empty call term");
  Assert(callContext->root()->getSort() != 0, "call term sort missing");
  Assert(namedStrategy->getDefinitions().length() > 0, "strategy must have a definition here");

  // We are matching on top, so only one position is considered
  search.findNextPosition();
}

StrategicExecution::Survival
CallProcess::run(StrategicSearch& searchObject)
{
  // Index of the last definition for the strategy
  const int lastDef = strategy->getDefinitions().length() - 1;

  //
  // Using defIndex, we recover the strategy definition where we left
  // the exploration before. It may be the first trial for it or we may
  // have found a match before.
  //
  StrategyDefinition* sdef = strategy->getDefinitions()[defIndex];

  bool matched = first ? search.findFirstSolution(sdef, sdef->getLhsAutomaton())
		       : search.findNextSolution();

  // We try to match other definitions if the previous do not match
  while (!matched && defIndex < lastDef)
    {
      search.transferCountTo(*searchObject.getContext());

      sdef = strategy->getDefinitions()[++defIndex];
      matched = search.findFirstSolution(sdef, sdef->getLhsAutomaton());
    }

  search.transferCountTo(*searchObject.getContext());

  if (matched)
    {
      if (RewritingContext::getTraceStatus())
	{
	  //
	  //	The problem described in ApplicationProcess::doRewrite also happens
	  //	here, so we need to build a rewriting context just for tracing.
	  //
	  RewritingContext* baseContext = searchObject.getContext();
	  RewritingContext* tracingContext = baseContext->makeSubcontext(baseContext->root());
	  tracingContext->clone(*search.getContext());
	  tracingContext->traceStrategyCall(sdef,
					    search.getContext()->root(),
					    searchObject.getCanonical(subjectDagIndex),
					    search.getContext());
	  delete tracingContext;
	}

      first = false;

      // We need to open a new variable context for its execution
      // and recover the original context when its execution finishes,
      // so we create a CallTask.
      const Vector<int>& contextSpec = sdef->getContextSpec();

      VariableBindingsManager::ContextId cid = contextSpec.empty()
		? VariableBindingsManager::EMPTY_CONTEXT
		: searchObject.openContext(*callContext,
			contextSpec
		);

      (void) new CallTask(searchObject,
			  subjectDagIndex,
			  sdef->getRhs(),
			  pending,
			  cid,
			  this,
			  this);

      // Other solutions may be available
      return StrategicExecution::SURVIVE;
    }

  // Any definition has matched the given term (it might not be an error)
  // if (first)
  //  IssueAdvisory("strategy execution stucked. No strategy definition matches the call.");

  finished(this);
  return StrategicExecution::DIE;
}
