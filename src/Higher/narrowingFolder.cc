/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017-2024 SRI International, Menlo Park, CA 94025, USA.

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
#include "higher.hh"

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
#include "variantSearch.hh"
#include "narrowingFolder.hh"

NarrowingFolder::NarrowingFolder(RewritingContext* context,
				 FreshVariableGenerator* freshVariableGenerator,
				 bool fold,
				 bool vfold,
				 bool keepHistory)
  : fold(fold),
    vfold(vfold),
    keepHistory(keepHistory),
    context(context),
    freshVariableGenerator(freshVariableGenerator)
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

void
NarrowingFolder::dump(ostream& s)
{
  for (auto& i : mostGeneralSoFar)
    {
      s << i.first << " : " << i.second->state <<
	" parent=" << i.second->parentIndex <<
	" depth=" << i.second->depth <<
	" locked=" << i.second->locked <<
	" subsumed=" << i.second->subsumed << endl;
    }
}

Vector<DagNode*>
NarrowingFolder::getReturnedButUnexploredStates() const
{
  //
  //	We return all states that were marked as returnedButUnexplored and haven't been deleted or marked as subsumed.
  //
  Vector<DagNode*> unexploredStates;
  for (auto p : mostGeneralSoFar)
    {
      if (p.second->returnedButUnexplored && !(p.second->subsumed))
	unexploredStates.push_back(p.second->state);
    }
  return unexploredStates;
}

Vector<DagNode*>
NarrowingFolder::getUnreturnedStates() const
{
  Vector<DagNode*> unreturnedStates;
  //
  //	Return all states that have neither been returned as the next state, nor subsumed.
  //	Since they may have been considered for unification with the goal, they could be locked and subsumed.
  //
  for (RetainedStateMap::const_iterator i = mostGeneralSoFar.upper_bound(currentStateIndex); i != mostGeneralSoFar.end(); ++i)
    {
      DebugInfo("unvisited state " << i->first << " : " << i->second->state);
      if (!(i->second->subsumed))
	unreturnedStates.push_back(i->second->state);
    }
  return unreturnedStates;
}

Vector<DagNode*>
NarrowingFolder::getMostGeneralStates() const
{
  Assert(fold || vfold, "not folding");
  Vector<DagNode*> mostGeneralStates;
  //
  //	If we're folding, all states should be most general unless they've been locked and subsumed.
  //
  for (auto i : mostGeneralSoFar)
    {
      if (!(i.second->subsumed))
	mostGeneralStates.push_back(i.second->state);
    }
  return mostGeneralStates;
}

void
NarrowingFolder::doSubsumption(RetainedStateMap::iterator& subsumedStateIter,
			       StateSet& existingStatesSubsumed,
			       int subsumersParent,
			       const StateSet&  subsumersAncestors)
{
  //
  //	Record this state as subsumed in current subsumption analysis.
  //
  int victimIndex = subsumedStateIter->first;
  RetainedState* victimState = subsumedStateIter->second;
  existingStatesSubsumed.insert(victimIndex);
  //
  //	If the victim is locked, it is needed for printing the path to a success.
  //	Otherwise if it is the subsumer's parent, its needed to compute the subsumer's accumulated subsitution.
  //	Otherwise if we're keeping history and it is in the subsumer's ancestors, its potentially needed for a path.
  //
  if (victimState->locked ||
      victimIndex == subsumersParent ||
      (keepHistory && !(subsumersAncestors.find(victimIndex) == subsumersAncestors.end())))
    victimState->markedSubsumed();
  else
    {
      //
      //	No reason to keep this subsumed state.
      //
      delete victimState;
      mostGeneralSoFar.erase(subsumedStateIter);
    }
}

bool
NarrowingFolder::insertState(int index, DagNode* state, int parentIndex, int variableFamily)
{
  DebugEnter("index = " << index << "  state = " << state << "  parentIndex = " << parentIndex << "  variableFamily = " << variableFamily);
  if (fold || vfold)
    {
      DebugInfo("number of most general states = " << mostGeneralSoFar.size());
      //
      //	See if state is subsumed by an existing state.
      //
      for (auto& i : mostGeneralSoFar)
	{
	  DebugInfo("looking a potential subsumer " << i.second->state << Tty(Tty::RESET));
	  DebugAdvisoryCheck(i.second->subsumed, "but potential subsumer is marked subsumed");
	  if (!i.second->subsumed && i.second->subsumes(state))
	    {
	      DebugAdvisory("new state " << index << " subsumed by " << i.first);
	      Verbose("New state " << state << " subsumed by " << i.second->state);
	      return false;
	    }
	}
    }
  //
  //	Create a new retained state.
  //
  DebugAdvisory("new state " << index << " added");
  int rootIndex = index;
  int depth = 0;
  if (parentIndex != NONE)
    {
      RetainedStateMap::const_iterator j = mostGeneralSoFar.find(parentIndex);
      Assert(j != mostGeneralSoFar.end(), "couldn't find state with index " << parentIndex);
      rootIndex = j->second->rootIndex;
      depth = j->second->depth + 1;
    }
  RetainedState* newState = new RetainedState(state, parentIndex, rootIndex, depth, variableFamily, this);

  if (fold || vfold)
    {
      //
      //	Compute ancestor set.
      //
      StateSet ancestors;
      for (int i = parentIndex; i != NONE; )
	{
	  ancestors.insert(i);
	  RetainedStateMap::const_iterator j = mostGeneralSoFar.find(i);
	  //
	  // Grandparents need not exist if parent swallowed its tail
	  //
	  // Assert(j != mostGeneralSoFar.end(), "couldn't find state with index " << i);
	  if (j == mostGeneralSoFar.end())
	    break;
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
	  if (!i->second->subsumed)  // only look at states that haven't been subsumed
	    {
	      RetainedState* potentialVictim = i->second;
	      if (existingStatesSubsumed.find(potentialVictim->parentIndex) != existingStatesSubsumed.end())
		{
		  //
		  //	Our parent was subsumed so we are also subsumed.
		  //
		  DebugAdvisory("new state evicted descendent of an older state " << i->first);
		  Verbose("New state " << state <<
			  " evicted descendent of an older state " << i->second->state <<
			  " by subsuming an ancestor.");
		  existingStatesSubsumed.insert(i->first);
		  doSubsumption(i, existingStatesSubsumed, parentIndex, ancestors);
		}
	      else if (newState->subsumes(potentialVictim->state))
		{
		  //
		  //	Direct subsumption by new state.
		  //
		  DebugAdvisory("new state evicted an older state " << i->first);
		  Verbose("New state " << state << " subsumed older state " << i->second->state);
		  existingStatesSubsumed.insert(i->first);
		  doSubsumption(i, existingStatesSubsumed, parentIndex, ancestors);
		}
	    }
	  i = next;
	}
    }
  else
    {
      //
      //	Since we're not folding, if we are keeping history we
      //	need to keep track of number of descendants to know when
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
  //
  //	We're done with the current state, so we might want to delete it.
  //
  cleanGraph();
  //
  //	Find the next state if there is one.
  //
  for (;;)
    {
      //
      //	Find next state.
      //
      RetainedStateMap::const_iterator nextStateIterator = mostGeneralSoFar.upper_bound(currentStateIndex);
      if (nextStateIterator == mostGeneralSoFar.end())
	break;
      currentStateIndex = nextStateIterator->first;
      //
      //	The tricky thing is if it produced a solution, we may have locked it, and if so it could have
      //	subsequently been subsumed without being deleted, in which case we must skip over it.
      //	Because such a state must be locked, there's no point calling cleanGraph().
      //
      if (!nextStateIterator->second->subsumed)
	{
	  nextState = nextStateIterator->second->state;
	  nextStateAccumulatedSubstitution = nextStateIterator->second->accumulatedSubstitution;
	  nextStateVariableFamily = nextStateIterator->second->variableFamily;
	  nextStateDepth = nextStateIterator->second->depth;
	  DebugInfo("****** returning " << currentStateIndex);
	  return currentStateIndex;
	}
      Assert(nextStateIterator->second->locked, "unvisited state " << nextStateIterator->first << " subsumed but not locked");
    }
  return NONE;
}

void
NarrowingFolder::cleanGraph()
{
  //
  //	Clear the state graph, on the assumption that the current state has already
  //	been fully expanded.
  //
  if (fold || vfold)
    return;  // folding takes care of state deletions
  if (currentStateIndex == NONE)
    return;  // no current state to consider for deletion

  auto stateIterator = mostGeneralSoFar.find(currentStateIndex);
  if (stateIterator->second->returnedButUnexplored)
    {
      //
      //	The caller set a flag to say they didn't explore the state we just returned
      //	so it remains in the frontier and we can't delete it except by subsumption.
      //
      return;
    }
  if (!keepHistory)
    {
      //
      //	Since we are not keeping history and not folding we have no further need of a fully
      //	explored state. We cannot lock states if we are not keeping history.
      //
      Assert(!stateIterator->second->locked, "shouldn't have locked states if not keeping history");
      delete stateIterator->second;
      mostGeneralSoFar.erase(stateIterator);
      return;
    }
  //
  //	We are keeping history and so we need to determine whether the
  //	current state and its ancestors sill appear on an active path.
  //	We do this by walking up to the root, until we find a state with existing descendents
  //	or which has been locked.
  //
  while (!stateIterator->second->locked && stateIterator->second->nrDescendants == 0)
    {
      //
      //	No descendent left in graph so we will never
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

void
NarrowingFolder::lockPathToState(int index)
{
  Assert(keepHistory, "can't lock path if not keeping history");
  //
  //	We lock all the states on the current path back to the root state, so that they are never deleted and
  //	are thus available for showing the complete path to this state at any point before our object is deleted.
  //
  RetainedStateMap::iterator stateIterator = mostGeneralSoFar.find(index);
  DebugInfo("considering " << index);
  while(!stateIterator->second->locked)
    {
      DebugInfo("locking");;
      stateIterator->second->locked = true;
      int parentIndex = stateIterator->second->parentIndex;
      if (parentIndex == NONE)
	break;
      DebugInfo("considering " << parentIndex);
      stateIterator = mostGeneralSoFar.find(parentIndex);
    }
}

NarrowingFolder::RetainedState::RetainedState(DagNode* state,
					      int parentIndex,
					      int rootIndex,
					      int depth,
					      int variableFamily,
					      const NarrowingFolder* owner)
  : state(state),
    parentIndex(parentIndex),
    rootIndex(rootIndex),
    depth(depth),
    variableFamily(variableFamily)
{
  if (owner->fold)
    {
      //
      //	Make term version of state.
      //
      Term* t = state->symbol()->termify(state);
      //
      //	Even though it should be in normal form we need to set hash values.
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
  else if (owner->vfold)
    {
      DebugInfo("Making subsumption checker for " << state);
      //
      //	We compute variant subsumption by generating the variants of the term and
      //	then matching potential victims.
      //
      //
      const Vector<DagNode*> noBlockerDags;
      subsumptionChecker = new VariantSearch(owner->context->makeSubcontext(state),  // VariantSearch will delete this
					     noBlockerDags,
					     owner->freshVariableGenerator,
					     VariantSearch::SUBSUMPTION_MODE,
					     variableFamily);
    }
}

NarrowingFolder::RetainedState::~RetainedState()
{
  delete accumulatedSubstitution;
  delete unifier;
  delete matchingAutomaton;
  if (stateTerm)
    stateTerm->deepSelfDestruct();
  delete subsumptionChecker;
}

bool
NarrowingFolder::RetainedState::subsumes(DagNode* state) const
{
  DebugEnter("trying to subsume " << state << " by " << this->state);
  MemoryCell::okToCollectGarbage();  // otherwise we have huge accumulation of junk from matching
  if (subsumptionChecker)
    {
      DebugInfo("vfold case");
      bool result = subsumptionChecker->isSubsumed(state);
      DebugInfo("result is " << result);
      return result;
    }
  DebugInfo("fold case");
  int nrSlotsToAllocate = nrMatchingVariables;
  if (nrSlotsToAllocate == 0)
    nrSlotsToAllocate = 1;  // substitutions subject to clear() must always have at least one slot
  
  RewritingContext matcher(nrSlotsToAllocate);
  matcher.clear(nrMatchingVariables);
  Subproblem* subproblem = 0;

  bool result = matchingAutomaton->match(state, matcher, subproblem) &&
    (subproblem == 0 || subproblem->solve(true, matcher));
  DebugInfo("result is " << result);
  delete subproblem;
  return result;
}
