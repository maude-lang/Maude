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
//	Implementation for class NarrowingSequenceSearch.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	variable class definitions
#include "variableDagNode.hh"

//	core class definitions
#include "rewritingContext.hh"

//	higher class definitions
#include "pattern.hh"
//#include "narrowingSearchState.hh"
#include "narrowingSequenceSearch.hh"
#include "freshVariableGenerator.hh"
#include "narrowingVariableInfo.hh"


NarrowingSequenceSearch::NarrowingSequenceSearch(RewritingContext* initial,
						 SearchType searchType,
						 Pattern* goal,
						 int maxDepth,
						 int narrowingFlags,
						 FreshVariableGenerator* freshVariableGenerator)
  : initial(initial),
    goal(goal),
    maxDepth((searchType == ONE_STEP) ? 1 : maxDepth),
    narrowingFlags(narrowingFlags),
    freshVariableGenerator(freshVariableGenerator)
{
  //
  //	First we replace all the variables in our initial term so they can't clash with variables in the rules.
  //
  NarrowingVariableInfo variableInfo;
  initial->root()->indexVariables(variableInfo, 0);
  int nrVariables = variableInfo.getNrVariables();
  Substitution s(nrVariables);
  for (int i = 0; i < nrVariables; ++i)
    {
      Sort* sort = safeCast(VariableSymbol*, variableInfo.index2Variable(i)->symbol())->getSort();
      VariableDagNode* v = new VariableDagNode(freshVariableGenerator->getBaseVariableSymbol(sort),
					       freshVariableGenerator->getFreshVariableName(i, false),
					       i);
      s.bind(i, v);
    }
  DagNode* newDag = initial->root()->instantiate(s);  // not safe if we haven't determined ground terms in context->root()
  if (newDag == 0)
    newDag = initial->root();

  RewritingContext* redContext = initial->makeSubcontext(newDag);
  redContext->reduce();

  seenSet.insert(redContext->root());
  matchState = 0;
  //
  //	initialState becomes responsible for deleting redContext
  //
  NarrowingSearchState* initialState = new NarrowingSearchState(redContext, freshVariableGenerator, true, UNDEFINED, narrowingFlags);
  stateStack.append(initialState);

  needToTryInitialState = (searchType == ANY_STEPS);
  //reachingInitialStateOK = (searchType == AT_LEAST_ONE_STEP || searchType == ONE_STEP);
  normalFormNeeded = (searchType == NORMAL_FORM);
  topOfStackFresh = true;
  variableTotalForPreviouslyReturnedStates = 0;
  variableTotalForAllReturnedStates = 0;
  incompleteFlag = false;
}

NarrowingSequenceSearch::~NarrowingSequenceSearch()
{
  delete matchState;
  delete goal;
  delete freshVariableGenerator;
  FOR_EACH_CONST(i, Vector<NarrowingSearchState*>, stateStack)
    delete *i;
  delete initial;
}

bool
NarrowingSequenceSearch::findNextMatch()
{
  if (matchState != 0)
    goto tryMatch;  // non-startup case

  for(;;)
    {
      if (!(normalFormNeeded ? findNextNormalForm() : findNextInterestingState()))
	break;
      if (goal == 0)
	{
	  //
	  //	No pattern case: we do some extra accounting needed by metalevel.
	  //
	  variableTotalForPreviouslyReturnedStates = variableTotalForAllReturnedStates;
	  variableTotalForAllReturnedStates += stateStack[stateStack.size() - 1]->getNrOfVariablesInSubject();
	  /*
	  cout << variableTotalForPreviouslyReturnedStates << '\n' <<
	    variableTotalForAllReturnedStates << '\n' <<
	    stateStack[stateStack.size() - 1]->getNrOfVariablesInSubject() << '\n';
	  */
	  return true;  // no pattern case
	}
      matchState = new MatchSearchState(initial->makeSubcontext(getStateDag()),  // CHECK: could we just use existing context?
					goal,
					MatchSearchState::GC_CONTEXT);
    tryMatch:
      bool foundMatch = matchState->findNextMatch();
      //
      //	We don't transfer the rewrite count from matchState since there is
      //	no condition, and there should be no membership axioms so count must
      //	be zero.
      //
      if (foundMatch)
	return true;
      delete matchState;
      matchState = 0;
    }

  return false;
}

bool
NarrowingSequenceSearch::findNextNormalForm()
{
  int currentIndex = stateStack.size() - 1;
  NarrowingSearchState* currentState;

  if (!topOfStackFresh)
    {
    backtrack:
      //
      //	Backtrack.
      //
      incompleteFlag |= stateStack[currentIndex]->isIncomplete();
      delete stateStack[currentIndex];
      stateStack.resize(currentIndex);
      --currentIndex;
      if (currentIndex < 0)
	return false;
      topOfStackFresh = false;
    }
  //
  //	Forward.
  //
  currentState = stateStack[currentIndex];
  while (currentState->findNextNarrowing())
    {
      if (currentIndex == maxDepth)
	goto backtrack;
      
      DagNode* replacement;  // will be set by getNarrowedDag()
      DagNode* narrowedDag = currentState->getNarrowedDag(replacement);

      if (RewritingContext::getTraceStatus())
	{
	  RewritingContext* context = currentState->getContext();
	  context->traceNarrowingStep(currentState->getRule(),
				      currentState->getDagNode(),
				      replacement,
				      &(currentState->getVariableInfo()),
				      &(currentState->getSubstitution()),
				      narrowedDag);
	  if (context->traceAbort())
	    return false;
	}
      initial->incrementNarrowingCount();

      RewritingContext* newContext = initial->makeSubcontext(narrowedDag);
      newContext->reduce();
      if (newContext->traceAbort())
	{
	  delete newContext;
	  return false;
	}
      initial->addInCount(*newContext);
      
      if (seenSet.dagNode2Index(newContext->root()) != NONE)
	{
	  delete newContext;
	  topOfStackFresh = false;
	  continue;
	}
      seenSet.insert(newContext->root());
      currentState = new NarrowingSearchState(newContext, freshVariableGenerator, !(currentState->isOdd()), UNDEFINED, narrowingFlags);
      stateStack.append(currentState);
      ++currentIndex;
      topOfStackFresh = true;
    }
  if (!topOfStackFresh)
    goto backtrack;

  topOfStackFresh = false;  // top of stack will be stale next time we're called
  return true;
}

bool
NarrowingSequenceSearch::findNextInterestingState()
{
  if (needToTryInitialState)
    {
      //
      //	Special case: return the initial state.
      //
      needToTryInitialState = false;  // don't do this again
      return true;
    }

  int currentIndex = stateStack.size() - 1;
  while (currentIndex >= 0)
    {
      NarrowingSearchState* currentState = stateStack[currentIndex];
      if ((maxDepth == NONE || currentIndex < maxDepth) && currentState->findNextNarrowing())
	{
	  DagNode* replacement;  // will be set by getNarrowedDag()
	  DagNode* narrowedDag = currentState->getNarrowedDag(replacement);

	  if (RewritingContext::getTraceStatus())
	    {
	      RewritingContext* context = currentState->getContext();
	      context->traceNarrowingStep(currentState->getRule(),
					  currentState->getDagNode(),
					  replacement,
					  &(currentState->getVariableInfo()),
					  &(currentState->getSubstitution()),
					  narrowedDag);
	      if (context->traceAbort())
		return false;
	    }
	  initial->incrementNarrowingCount();

	  RewritingContext* newContext = initial->makeSubcontext(narrowedDag);
	  newContext->reduce();
	  ////cout << "which reduced to " << newContext->root() << endl;
	  if (seenSet.dagNode2Index(newContext->root()) != NONE)
	    {
	      DebugAdvisory(Tty(Tty::RED) << "DUP state " << Tty(Tty::RESET) << newContext->root());
	      delete newContext;
	      continue;
	    }
	  seenSet.insert(newContext->root());
	  stateStack.append(new NarrowingSearchState(newContext, freshVariableGenerator, !(currentState->isOdd()), UNDEFINED, narrowingFlags));
	  return true;
	}
      //
      //	Backtrack.
      //
      incompleteFlag |= stateStack[currentIndex]->isIncomplete();
      delete stateStack[currentIndex];
      stateStack.resize(currentIndex);
       --currentIndex;
    }
  return false;
}
