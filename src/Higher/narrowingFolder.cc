/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class NarrowingFolder.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "subproblem.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "subproblemAccumulator.hh"
#include "variableInfo.hh"

//	higher class definitions
#include "narrowingFolder.hh"

NarrowingFolder::NarrowingFolder(bool fold, bool keepHistory)
  : fold(fold),
    keepHistory(keepHistory)
{
  currentStateIndex = -1;
}

NarrowingFolder::~NarrowingFolder()
{
  for (auto& i : mostGeneralSoFar)
    delete i.second;
}

void
NarrowingFolder::markReachableNodes()
{
  for (auto& i : mostGeneralSoFar)
    {
      i.second->state->mark();
      if (DagNode* d = i.second->narrowingContext)
	d->mark();
      if (Substitution* subst = i.second->accumulatedSubstitution)
	{
	  int nrBindings = subst->nrFragileBindings();
	  for (int j = 0; j < nrBindings; ++j)
	    subst->value(j)->mark();
	}
      if (Substitution* subst = i.second->unifier)
	{
	  int nrBindings = subst->nrFragileBindings();
	  for (int j = 0; j < nrBindings; ++j)
	    {
	      //
	      //	The unifier has a zero'd out gap between
	      //	the rule variables and the dag variables.
	      //
	      if (DagNode* d = subst->value(j))
		d->mark();
	    }
	}
    }
}

bool
NarrowingFolder::insertState(int index, DagNode* state, int parentIndex)
{
  if (fold)
    {
      //
      //	See if state is subsumed by an existing state.
      //
      for (auto& i : mostGeneralSoFar)
	{
	  if (i.second->subsumes(state))
	    {
	      DebugAdvisory("new state " << index << " subsumed by " << i.first);
	      return false;
	    }
	}
    }
  DebugAdvisory("new state " << index << " added");
  RetainedState* newState = new RetainedState(state, parentIndex, fold);
  int depth = 0;
  if (parentIndex != NONE)
    {
      RetainedStateMap::const_iterator j = mostGeneralSoFar.find(parentIndex);
      Assert(j != mostGeneralSoFar.end(), "couldn't find state with index " << parentIndex);
      depth = j->second->depth + 1;
    }
  newState->depth = depth;
  if (fold)
    {
      //
      //	Compute ancestor set.
      //
      StateSet ancestors;
      for (int i = parentIndex; i != NONE; )
	{
	  ancestors.insert(i);
	  RetainedStateMap::const_iterator j = mostGeneralSoFar.find(i);
	  Assert(j != mostGeneralSoFar.end(), "couldn't find state with index " << i);
	  i = j->second->parentIndex;
	}
      //
      //	See if newState can evict an existing state.
      //
      StateSet existingStatesSubsumed;
      RetainedStateMap::iterator i = mostGeneralSoFar.begin();
      while (i != mostGeneralSoFar.end())
	{
	  RetainedStateMap::iterator next = i;
	  ++next;
	  if (ancestors.find(i->first) == ancestors.end())  // can't mess with ancestors of new state
	    {
	      RetainedState* potentialVictim = i->second;
	      if (existingStatesSubsumed.find(potentialVictim->parentIndex) !=
		  existingStatesSubsumed.end())
		{
		  //
		  //	Our parent was subsumed so we are also subsumed.
		  //
		  DebugAdvisory("new state evicted descendent of an older state " << i->first);
		  existingStatesSubsumed.insert(i->first);
		  delete potentialVictim;
		  mostGeneralSoFar.erase(i);
		}
	      else if (newState->subsumes(potentialVictim->state))
		{
		  //
		  //	Direct subsumption by new state.
		  //
		  DebugAdvisory("new state evicted an older state " << i->first);
		  existingStatesSubsumed.insert(i->first);
		  delete potentialVictim;
		  mostGeneralSoFar.erase(i);
		}
	    }
	  i = next;
	}
    }
  else
    {
      //
      //	Since we're not folding, if we are keeping history we
      //	need to keep track of number of descendant to know when
      //	a state will never appear in the history of a returned
      //	state.
      //
      if (keepHistory && parentIndex != NONE)
	++(mostGeneralSoFar[parentIndex]->nrDescendants);
    }
  mostGeneralSoFar.insert(RetainedStateMap::value_type(index, newState));
  return true;
}

void
NarrowingFolder::addHistory(int index,
			    Rule* rule,
			    DagNode* narrowingContext,
			    DagNode* narrowingPosition,
			    const Substitution& unifier,
			    const NarrowingVariableInfo& variableInfo)
{
  RetainedStateMap::iterator i = mostGeneralSoFar.find(index);
  Assert(i != mostGeneralSoFar.end(), "couldn't find state with index " << index);
  i->second->rule = rule;
  i->second->narrowingContext = narrowingContext;
  i->second->narrowingPosition = narrowingPosition;
  //
  //	We make a copy of the unifier and the variableInfo.
  //
  int nrBindings = unifier.nrFragileBindings();
  Substitution* unifierCopy = new Substitution(nrBindings);
  for (int i = 0; i < nrBindings; ++i)
    unifierCopy->bind(i, unifier.value(i));
  i->second->unifier = unifierCopy;
  //
  //	We rely in the variable dag nodes originally coming from the previous
  //	state which is preserved and protected from GC in keepHistory mode.
  //
  i->second->variableInfo.copy(variableInfo);
}

int
NarrowingFolder::getNextSurvivingState(DagNode*& nextState,
				       Substitution*& nextStateAccumulatedSubstitution,
				       int& nextStateVariableFamily,
				       int& nextStateDepth)
{
  cleanGraph();
  RetainedStateMap::const_iterator nextStateIterator =
    mostGeneralSoFar.upper_bound(currentStateIndex);
  if (nextStateIterator == mostGeneralSoFar.end())
    return NONE;

  currentStateIndex = nextStateIterator->first;
  nextState = nextStateIterator->second->state;
  nextStateAccumulatedSubstitution = nextStateIterator->second->accumulatedSubstitution;
  nextStateVariableFamily = nextStateIterator->second->variableFamily;
  nextStateDepth = nextStateIterator->second->depth;
  return currentStateIndex;
}

void
NarrowingFolder::cleanGraph()
{
  //
  //	Clear the state graph, on the assumption that the current state has already
  //	been fully expanded.
  //
  if (fold)
    return;  // folding takes care of state deletions
  if (currentStateIndex == NONE)
    return;  // no current state to consider for deletion

  RetainedStateMap::iterator stateIterator = mostGeneralSoFar.find(currentStateIndex);
  if (!keepHistory)
    {
      //
      //	Since we are not keeping history, we should not need the
      //	current state again since we are moving on to the next state.
      //
      delete stateIterator->second;
      mostGeneralSoFar.erase(stateIterator);
      return;
    }
  //
  //	We are keeping history and so we need to determine whether the
  //	current state and its ancestors sill appear on an active path.
  //
  while (stateIterator->second->nrDescendants == 0)
    {
      //
      //	No desendant left in graph so we will never
      //	be asked for this state on a path and can delete it.
      //
      int parentIndex = stateIterator->second->parentIndex;
      delete stateIterator->second;
      mostGeneralSoFar.erase(stateIterator);
      if (parentIndex == NONE)
	break;
      //
      //	This may have left the parent state without descendants
      //	and subject to deletion.
      //
      stateIterator = mostGeneralSoFar.find(parentIndex);
      --(stateIterator->second->nrDescendants);
    }
}

NarrowingFolder::RetainedState::RetainedState(DagNode* state, int parentIndex, bool fold)
  : state(state),
    parentIndex(parentIndex)
{
  if (fold)
    {
      //
      //	Make term version of state.
      //
      Term* t = state->symbol()->termify(state);
      //
      //	Even thoug t should be in normal form we need to set hash values.
      //
      t = t->normalize(true);
      VariableInfo variableInfo;
      t->indexVariables(variableInfo); 
      t->symbol()->fillInSortInfo(t);
      t->analyseCollapses();

      NatSet boundUniquely;
      bool subproblemLikely;

      t->determineContextVariables();
      t->insertAbstractionVariables(variableInfo);

      matchingAutomaton = t->compileLhs(false, variableInfo, boundUniquely, subproblemLikely);
      stateTerm = t;
      nrMatchingVariables = variableInfo.getNrProtectedVariables();  // may also have some
      								     // abstraction variables
    }
  else
    {
      stateTerm = 0;
      matchingAutomaton = 0;
      nrMatchingVariables = 0;
    }
  //
  //	These will be filled out later.
  //
  variableFamily = NONE;
  accumulatedSubstitution = 0;
  //
  //	These will be filled out later if we're keeping the history.
  //
  rule = 0;
  unifier = 0;
  narrowingContext = 0;
  narrowingPosition = 0;
  //
  //	This may be updated if we are keeping the history and we aren't folding.
  //
  nrDescendants = 0;
}

NarrowingFolder::RetainedState::~RetainedState()
{
  delete accumulatedSubstitution;
  delete unifier;
  delete matchingAutomaton;
  if (stateTerm)
    stateTerm->deepSelfDestruct();
}

bool
NarrowingFolder::RetainedState::subsumes(DagNode* state) const
{
  MemoryCell::okToCollectGarbage();  // otherwise we have huge accumulation of junk from matching
  int nrSlotsToAllocate = nrMatchingVariables;
  if (nrSlotsToAllocate == 0)
    nrSlotsToAllocate = 1;  // substitutions subject to clear() must always have at least one slot
  
  RewritingContext matcher(nrSlotsToAllocate);
  matcher.clear(nrMatchingVariables);
  Subproblem* subproblem = 0;

  bool result = matchingAutomaton->match(state, matcher, subproblem) &&
    (subproblem == 0 || subproblem->solve(true, matcher));
  delete subproblem;
  return result;
}
