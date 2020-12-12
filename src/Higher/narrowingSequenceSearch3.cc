/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class NarrowingSequenceSearch3.
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
#include "variableSymbol.hh"
#include "variableDagNode.hh"

//	core class definitions
#include "rewritingContext.hh"

//	higher class definitions
#include "freshVariableGenerator.hh"
#include "narrowingVariableInfo.hh"
#include "narrowingSearchState3.hh"
#include "variantSearch.hh"
#include "narrowingSequenceSearch3.hh"

NarrowingSequenceSearch3::NarrowingSequenceSearch3(RewritingContext* initial,
						   SearchType searchType,
						   DagNode* goal,
						   int maxDepth,
						   FreshVariableGenerator* freshVariableGenerator,
						   int variantFlags)
  : initial(initial),
    goal(goal),
    maxDepth((searchType == ONE_STEP) ? 1 : maxDepth),
    needToTryInitialState(searchType == ANY_STEPS),
    normalFormNeeded(searchType == NORMAL_FORM),
    freshVariableGenerator(freshVariableGenerator),
    variantFlags(variantFlags),
    stateCollection(variantFlags & FOLD, variantFlags & KEEP_HISTORY)
{
  incompleteFlag = false;
  unificationProblem = 0;
  stateBeingExpanded = 0;
  stateBeingExpandedIndex = NONE;
  stateBeingExpandedDepth = 0;
  expansionSuccessful = false;
  nextInterestingState = NONE;
  counter = 0;
  nrStatesExpanded = 0;
  //
  //	Index variables occurring in the initial term and create a fresh
  //	#variable for each original variable and an accumulated substitution
  //	that maps fresh original variables to fresh variables.
  //
  DagNode* dagToNarrow = initial->root();
  dagToNarrow->indexVariables(initialVariableInfo, 0);
  int nrInitialVariables = initialVariableInfo.getNrVariables();
  Substitution* accumulatedSubstitution = new Substitution(nrInitialVariables);
  for (int i = 0; i < nrInitialVariables; ++i)
    {
      Symbol* baseSymbol = initialVariableInfo.index2Variable(i)->symbol();
      int name = freshVariableGenerator->getFreshVariableName(i, 0);
      VariableDagNode* v = new VariableDagNode(baseSymbol, name, i);
      accumulatedSubstitution->bind(i, v);
    }
  //
  //	HACK: this is so we can use instantiate which expects ground terms
  //	to have sorts. FIXME
  //
  //goal->computeTrueSort(*initial); 
  //
  //	We also want to index goal variables so we can apply the accumulated
  //	substitution, but we do not want to carry around the extra variables
  //	since they cannot play a role in narrowing, as they don't occur in
  //	the initial term.
  //
  goal->indexVariables(initialVariableInfo, 0);
  //
  //	Reduction could lose variables. But initial variables will be protected
  //	from garbage collection by initial RewritingContext, and goal only
  //	variables will be protected by goal.
  //
  if (DagNode* renamedDagToNarrow = dagToNarrow->instantiate(*accumulatedSubstitution, false))
    dagToNarrow = renamedDagToNarrow;
  RewritingContext* reduceContext = initial->makeSubcontext(dagToNarrow);
  reduceContext->reduce();
  initial->transferCountFrom(*reduceContext);
  //
  //	Create initial state in state collection.
  //
  (void) stateCollection.insertState(0, reduceContext->root(), -1);
  stateCollection.addAccumulatedSubstitution(0, 0, accumulatedSubstitution);
  delete reduceContext;
  //
  //	Need to make internal symbol for variant unification with goal.
  //
  Symbol* goalSymbol = goal->symbol();
  Module* module = goalSymbol->getModule();
  ConnectedComponent* range = goal->symbol()->rangeComponent();

  Vector<ConnectedComponent*> domain(2);
  domain[0] = range;
  domain[1] = range;
  unificationPairSymbol = module->createInternalTupleSymbol(domain, range);
}

NarrowingSequenceSearch3::~NarrowingSequenceSearch3()
{
  delete unificationProblem;
  delete stateBeingExpanded;
  delete freshVariableGenerator;
  delete initial;
}

bool
NarrowingSequenceSearch3::findNextUnifier()
{
  for (;;)
    {
      if (unificationProblem != 0)
	{
	  bool moreUnifiers = unificationProblem->findNextUnifier();
	  initial->transferCountFrom(*(unificationProblem->getContext()));
	  if (unificationProblem->isIncomplete())
	    incompleteFlag = true;
	  if (moreUnifiers)
	    {
	      currentUnifier = &(unificationProblem->getCurrentUnifier(nrFreeVariablesInUnifier,
								       variableFamilyInUnifier));
	      return true;
	    }	  
	  delete unificationProblem;
	  unificationProblem = 0;
	}

      nextInterestingState = findNextInterestingState();
      if (nextInterestingState == NONE)
	break;
      DagNode* stateDag;
      int variableFamily;
      Substitution* accumulatedSubstitution;
      stateCollection.getState(nextInterestingState, stateDag, variableFamily, accumulatedSubstitution);
      //
      //	Because the goal dag might include some of the variables from the initial state
      //	we need to instantiate it.
      //
      DagNode* instantiatedGoal;
      int nrInitialVariables = accumulatedSubstitution->nrFragileBindings();
      int totalNrVariables = initialVariableInfo.getNrVariables();
      if (totalNrVariables > nrInitialVariables)
	{
	  //
	  //	Need to make a bigger substitution with extra variables zero'd out.
	  //
	  Substitution bigger(totalNrVariables);
	  for (int i = 0; i < nrInitialVariables; ++i)
	    bigger.bind(i, accumulatedSubstitution->value(i));
	  for (int i = nrInitialVariables; i < totalNrVariables; ++i)
	    bigger.bind(i, 0);
	  instantiatedGoal = goal.getNode()->instantiate(bigger, false);
	}
      else
	instantiatedGoal = goal.getNode()->instantiate(*accumulatedSubstitution, false);
      if (instantiatedGoal == 0)
	instantiatedGoal = goal.getNode();  // no change under instantiation
      //
      //	Need to variant unify the narrrowed dag in this state with the goal dag.
      //
      Vector<DagNode*> args(2);
      args[0] = instantiatedGoal;
      args[1] = stateDag;
      DagNode* pairDag = unificationPairSymbol->makeDagNode(args);
      RewritingContext* pairContext = initial->makeSubcontext(pairDag);

      const Vector<DagNode*> dummy;
      unificationProblem = new VariantSearch(pairContext,  // will be deleted by VariantSearch
					     dummy,
					     freshVariableGenerator,
					     VariantSearch::UNIFICATION_MODE,
					     variableFamily);
    }
  return false;
}

int
NarrowingSequenceSearch3::findNextInterestingState()
{
  if (needToTryInitialState)
    {
      //
      //	Special case: return the initial state; only happens for =>*
      //
      needToTryInitialState = false;  // don't do this again
      return 0;
    }

  if (stateBeingExpanded != 0)
    {
    tryToExpand:
      //
      //	Try to get a descendant of the state being expanded.
      //
      for (;;)
	{
	  bool success = stateBeingExpanded->findNextNarrowing();
	  initial->transferCountFrom(*(stateBeingExpanded->getContext()));
	  if (stateBeingExpanded->isIncomplete())
	    incompleteFlag = true;
	  if (!success)
	    {
	      delete stateBeingExpanded;
	      stateBeingExpanded = 0;
	      if (normalFormNeeded && !expansionSuccessful)
		{
		  //
		  //	stateBeingExpanded was a normalForm.
		  //
		  return stateBeingExpandedIndex;
		}
	      break;
	    }
	  if (normalFormNeeded && stateBeingExpandedDepth == maxDepth)
	    {
	      //
	      //	The new state will be at maxDepth + 1 so need not be considered. It's
	      //	enough to know that the parent state has a successor and thus isn't
	      //	in normal form.
	      //
	      delete stateBeingExpanded;
	      stateBeingExpanded = 0;
	      break;
	    }
	  expansionSuccessful = true;  // stateBeingExpanded can't be a normal form
	  //
	  //	Get the details fo the new state we reached.
	  //
	  DagNode* replacement;
	  DagNode* replacementContext;
	  DagNode* newState = stateBeingExpanded->getNarrowedDag(replacement, replacementContext);
	  //
	  //	Do tracing if we care about it.
	  //
	  if (RewritingContext::getTraceStatus())
	    {
	      RewritingContext* context = stateBeingExpanded->getContext();
	      context->traceNarrowingStep(stateBeingExpanded->getRule(),
					  stateBeingExpanded->getReplacedDag(),
					  replacement,
					  &(stateBeingExpanded->getVariableInfo()),
					  &(stateBeingExpanded->getUnifier()),
					  newState);
	      if (context->traceAbort())
		return NONE;
	    }
	  initial->incrementNarrowingCount();
	  //
	  //	Reduce new state dag.
	  //
	  RewritingContext* reduceContext = initial->makeSubcontext(newState);
	  replacementContextProtector.setNode(replacementContext);  // protect replacementContext from GC during reduce()
	  reduceContext->reduce();  // can call GC
	  replacementContextProtector.setNode(0);  // withdraw protection
	  initial->transferCountFrom(*reduceContext);
	  //
	  //	Does new state survive folding?
	  //
	  int newStateIndex = ++counter;
	  bool survived = stateCollection.insertState(newStateIndex,
						      reduceContext->root(),
						      stateBeingExpandedIndex);
	  delete reduceContext;
	  if (survived)
	    {
	      stateCollection.addAccumulatedSubstitution(newStateIndex,
							 stateBeingExpanded->getVariableFamily(),
							 stateBeingExpanded->makeAccumulatedSubstitution());
	      if (stateCollection.keepingHistory())
		{
		  //
		  //	We rely on addHistory() to make copies of unifier
		  //	and variableInfo since originals will disappear in
		  //	due course.
		  //
		  stateCollection.addHistory(newStateIndex,
					     stateBeingExpanded->getRule(),
					     replacementContext,
					     replacement,
					     stateBeingExpanded->getUnifier(),
					     stateBeingExpanded->getVariableInfo());
		}
	      if (!normalFormNeeded)
		return newStateIndex;
	    }
	}
    }
  //
  //	Need to find a new state to expand.
  //
  DagNode* nextState;
  Substitution* nextStateAccumulatedSubstitution;
  int nextStateVariableFamily;
  stateBeingExpandedIndex = stateCollection.getNextSurvivingState(nextState,
								  nextStateAccumulatedSubstitution,
								  nextStateVariableFamily,
								  stateBeingExpandedDepth);
  //cout << "next surviving state is " << stateBeingExpandedIndex  << endl;
  if (stateBeingExpandedIndex != NONE)
    {
      //cout << nextStateDepth << " : " <<  maxDepth << endl;
      //
      //	If we are looking for normalForms, we only put states of depths <= maxDepth
      //	in the stateCollection, and we need to expand states of maxDepth to check for
      //	successors.
      //
      //	If we are not looking for normalForms, we only spend states from
      //	stateCollection that have depth < maxDepth.
      //
      //	Note that in both cases we need to do equality checks rather than less-than
      //	checks because we might have maxDepth = NONE to indicate no bound.
      //
      if (normalFormNeeded || stateBeingExpandedDepth != maxDepth)
	{
	  //cout << "expanding state is " << stateBeingExpandedIndex << " : " << nextState << endl;

	  RewritingContext* narrowingContext = initial->makeSubcontext(nextState);
	  stateBeingExpanded = new NarrowingSearchState3(narrowingContext,
							 nextStateAccumulatedSubstitution,
							 freshVariableGenerator,
							 nextStateVariableFamily,
							 NarrowingSearchState3::ALLOW_NONEXEC |
							 PositionState::RESPECT_FROZEN,
							 0,
							 UNBOUNDED,
							 variantFlags);
	  //
	  //	Initialize flag to false, since have yet to see an expansion.
	  //	We use this to detect normal form.
	  //
	  expansionSuccessful = false;
	  ++nrStatesExpanded;
	  goto tryToExpand;
	}
    }
  //
  //	We're done expanding, so we've seen all the states we'll ever see.
  //	States were numbered 0 (initial), ..., counter (last added to stateCollection).
  //
  Verbose("Total number of states seen = " << counter + 1);
  Verbose("Of which " << nrStatesExpanded << " were considered for further narrowing.");
  return NONE;
}
