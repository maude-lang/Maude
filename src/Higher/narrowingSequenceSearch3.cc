/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
#include "filteredVariantUnifierSearch.hh"
#include "narrowingSequenceSearch3.hh"

bool
NarrowingSequenceSearch3::handleInitialState(DagNode* dagToNarrow, NarrowingVariableInfo& varInfo)
{
  //
  //	Index the variables in the state dag.
  //
  dagToNarrow->indexVariables(varInfo, 0);
  //
  //	Build a renaming from these variable to fresh variables.
  //
  int nrVariables = varInfo.getNrVariables();
  Substitution* renamingSubstitution = new Substitution(nrVariables);
  for (int i = 0; i < nrVariables; ++i)
    {
      VariableDagNode* v = varInfo.index2Variable(i);
      if (freshVariableGenerator->variableNameConflict(v->id(), NONE))
	{
	  IssueWarning("unsafe variable name " << QUOTE((DagNode*) v) << " in initial state " <<
		       QUOTE(dagToNarrow) << ".");
	  return false;
	}
      int name = freshVariableGenerator->getFreshVariableName(i, 0);  // 0 indicates a #variable
      VariableDagNode* nv = new VariableDagNode(v->symbol(), name, i);
      renamingSubstitution->bind(i, nv);
    }
  //
  //	Protect the variable dagnodes we just made from the garbage collector in case they
  //	disappear from the reduced term.
  //
  protectedSubstitution = renamingSubstitution;
  //
  //	Rename all the variables in the state
  //
  if (DagNode* renamedDagToNarrow = dagToNarrow->instantiate(*renamingSubstitution, false))
    dagToNarrow = renamedDagToNarrow;
  //
  //	Reduce the state with equations. 
  //
  RewritingContext* reduceContext = initial->makeSubcontext(dagToNarrow);
  reduceContext->reduce();
  initial->transferCountFrom(*reduceContext);
  protectedSubstitution = nullptr;
  //
  //	Add the renamed and reduced state to the state collection, along with the
  //	renaming as its accumulated substitution.
  //
  int newStateIndex = ++counter;
  bool survived = stateCollection.insertState(newStateIndex, reduceContext->root(), -1);
  delete reduceContext;
  if  (survived)
    stateCollection.addAccumulatedSubstitution(newStateIndex, 0, renamingSubstitution);
  else
    delete renamingSubstitution;
  //
  //	If we cared about the renaming substitution, it is now protected by stateCollection.
  //
  protectedSubstitution = nullptr;
  return true;
}

NarrowingSequenceSearch3::NarrowingSequenceSearch3(RewritingContext* initial,
						   const Vector<DagNode*>& startStates,
						   SearchType searchType,
						   DagNode* goal,
						   int maxDepth,
						   FreshVariableGenerator* freshVariableGenerator,
						   int variantFlags)
  : initial(initial),
    goal(goal),
    nrInitialStatesToTry((searchType == ANY_STEPS) ? (startStates.size() ? startStates.size() : 1) : 0),
    maxDepth((searchType == ONE_STEP) ? 1 : maxDepth),
    normalFormNeeded(searchType == NORMAL_FORM),
    termDisjunction(startStates.size() > 1),
    freshVariableGenerator(freshVariableGenerator),
    variantFlags(variantFlags),
    stateCollection(variantFlags & FOLD, variantFlags & (KEEP_HISTORY | KEEP_PATHS))
{
  incompleteFlag = false;
  problemOkay = false;
  unificationProblem = nullptr;
  stateBeingExpanded = nullptr;
  stateBeingExpandedIndex = NONE;
  stateBeingExpandedDepth = 0;
  expansionSuccessful = false;
  nextInterestingState = NONE;
  counter = -1;
  nrStatesExpanded = 0;
  protectedSubstitution = nullptr;
  //
  //	Index variables occurring in the startStates.
  //
  map<DagNode*, Index, DagNode::LessThan> seenVariables;
  if (termDisjunction)
    {
      Index nrInitialStates = startStates.size();
      initialStates.resize(nrInitialStates);
      //
      //	First protect all startStates from garbage collection putting them in initialStates.
      //
      for (Index i = 0; i < nrInitialStates; ++i)
	initialStates[i].state = startStates[i];
      //
      //	Then do the processing on each initial state which may call reduce() and the garbage collector.
      //
      for (Index i = 0; i < nrInitialStates; ++i)
	{
	  InitialState& is = initialStates[i];
	  if (!handleInitialState(is.state, is.varInfo))
	    return;
	  //
	  //	Check for illegal variable sharing.
	  //
	  int nrVariables = is.varInfo.getNrVariables();
	  for (int j = 0; j < nrVariables; ++j)
	    {
	      DagNode* v = is.varInfo.index2Variable(j);
	      auto r = seenVariables.insert({v, i});
	      if (!r.second)
		{
		  IssueWarning("variable " << QUOTE(v) << " appears in both initial state " <<
			       QUOTE(initialStates[r.first->second].state) << " and initial state " <<
			       QUOTE(initialStates[i].state) << ".");
		  return;
		}
	    }
	}
    }
  else
    {
      if (!handleInitialState(initial->root(), initialVariableInfo))
	return;
    }
  //
  //	HACK: this is so we can use instantiate which expects ground terms
  //	to have sorts. FIXME
  //
  //goal->computeTrueSort(*initial); 
  //
  //	We also want to index goal variables so we can apply the renaming
  //	substitution, but we do not want to carry around the extra variables
  //	since they cannot play a role in narrowing, as they don't occur in
  //	the initial term.
  //
  goal->indexVariables(initialVariableInfo, 0);
  int nrVariables = initialVariableInfo.getNrVariables();
  for (int i = 0; i < nrVariables; ++i)
    {
      VariableDagNode* v = initialVariableInfo.index2Variable(i);
      DagNode* d = v;
      if (freshVariableGenerator->variableNameConflict(v->id(), NONE))
	{
	  IssueWarning("unsafe variable name " << QUOTE(d) << " in goal " << goal << ".");
	  return;
	}
      if (termDisjunction)
	{
	  auto p = seenVariables.find(d);
	  if (p != seenVariables.end())
	    {
	      IssueWarning("sharing a variable " << QUOTE(d) << " between initial state " <<
			   QUOTE(initialStates[p->second].state) << " and goal " <<
			   QUOTE(goal) << " is not allowed when narrowing a disjunction of initial states.");
	      return;
	    }
	}
    }
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

  problemOkay = true;
}

NarrowingSequenceSearch3::~NarrowingSequenceSearch3()
{
  delete unificationProblem;
  delete stateBeingExpanded;
  delete freshVariableGenerator;
  delete initial;
}

void
NarrowingSequenceSearch3::markReachableNodes()
{
  //
  //	Protect start states in termDisjuction mode
  //
  //goal->mark;  // maybe use this rather than DagRoot
  for (InitialState& i : initialStates)
    i.state->mark();
  if (protectedSubstitution)
    {
      int nrBindings = protectedSubstitution->nrFragileBindings();
      for (int i = 0; i < nrBindings; ++i)
	protectedSubstitution->value(i)->mark();
    }
}

bool
NarrowingSequenceSearch3::findNextUnifier()
{
  for (;;)
    {
      if (unificationProblem != nullptr)
	{
	  bool moreUnifiers = unificationProblem->findNextUnifier();
	  initial->transferCountFrom(*(unificationProblem->getContext()));
	  if (unificationProblem->isIncomplete())
	    incompleteFlag = true;
	  if (moreUnifiers)
	    {
	      currentUnifier = &(unificationProblem->getCurrentUnifier(nrFreeVariablesInUnifier, variableFamilyInUnifier));
	      if (variantFlags & KEEP_PATHS)
		{
		  //
		  //	We keep the path to a state that unifies with the goal since the user
		  //	can ask for such a path at any time. Otherwise a state will be deleted if it
		  //	becomes subsumed (if we're folding) or after all of its next states have been
		  //	generated.
		  //
		  stateCollection.lockPathToCurrentState();
		}
	      return true;
	    }	  
	  delete unificationProblem;
	  unificationProblem = nullptr;
	}
      //
      //	Get the next state and see if we can unify it with our goal.
      //
      nextInterestingState = findNextInterestingState();
      if (nextInterestingState == NONE)
	break;
      DagNode* stateDag;
      int variableFamily;
      Substitution* accumulatedSubstitution;
      stateCollection.getState(nextInterestingState, stateDag, variableFamily, accumulatedSubstitution);

      DagNode* goalDag = goal.getNode();  // no change under instantiation
      if (!termDisjunction)
	{
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
		bigger.bind(i, nullptr);
	      instantiatedGoal = goalDag->instantiate(bigger, false);
	    }
	  else
	    instantiatedGoal = goalDag->instantiate(*accumulatedSubstitution, false);
	  if (instantiatedGoal != nullptr)
	    goalDag = instantiatedGoal;
	}      
      //
      //	Need to variant unify the narrrowed dag in this state with the goal dag.
      //
      Vector<DagNode*> args(2);
      args[0] = goalDag;
      args[1] = stateDag;
      DagNode* pairDag = unificationPairSymbol->makeDagNode(args);
      RewritingContext* pairContext = initial->makeSubcontext(pairDag);

      const Vector<DagNode*> dummy;
      unificationProblem = (variantFlags & VariantUnificationProblem::FILTER_VARIANT_UNIFIERS) ?
	new FilteredVariantUnifierSearch(pairContext,  // will be deleted by VariantSearch
					 dummy,
					 freshVariableGenerator,
					 VariantSearch::UNIFICATION_MODE,
					 variableFamily) :
	new VariantSearch(pairContext,  // will be deleted by VariantSearch
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
  //
  //	If we are a =>* search we need tor try surviving initial states before we start narrowing.
  //
  while (nrInitialStatesToTry > 0)
    {
      --nrInitialStatesToTry;
      int stateToTry = counter - nrInitialStatesToTry;  // want to try 0,..., counter
      //
      //	We want to know if stateToTry has been subsumed; but because we have not started narrowing,
      //	it can't be locked, and thus if it has been subsumed it will have been deleted.
      //
      if (stateCollection.exists(stateToTry))
	return stateToTry;
    }

  if (stateBeingExpanded != nullptr)
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
	      stateBeingExpanded = nullptr;
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
	      stateBeingExpanded = nullptr;
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
	  replacementContextProtector.setNode(nullptr);  // withdraw protection
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
