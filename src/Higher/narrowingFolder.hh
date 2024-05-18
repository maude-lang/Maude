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
//	Class for folding and maintaining the history of a narrowing search.
//	Both folding and keeping the history are optional. If neither are used,
//	we simply keep track of unexplored states.
//
//	The caller gives each state an index, and for each new state, holds the
//	index of the parent state or NONE if it is the initial state of the narrowing
//	search. A state's index is required to be larger than that of its parent.
//	When an existing state is ejected by the arrival of a new state that is more
//	general, all of its descendents are likewise ejected, just as if the search
//	that has already taken place had been pruned.
//
#ifndef _narrowingFolder_hh_
#define _narrowingFolder_hh_
#include <set>
#include <map>
#include "term.hh"
#include "lhsAutomaton.hh"
#include "simpleRootContainer.hh"
#include "narrowingVariableInfo.hh"

class NarrowingFolder : private SimpleRootContainer
{
  NO_COPYING(NarrowingFolder);

public:
  NarrowingFolder(bool fold, bool keepHistory);
  ~NarrowingFolder();

  bool insertState(int index, DagNode* state, int parentIndex);
  void addAccumulatedSubstitution(int index,
				  int variableFamily,
				  Substitution* accumulatedSubstitution);
  void addHistory(int index,
		  Rule* rule,
		  DagNode* narrowingContext,
		  DagNode* narrowingPosition,
		  const Substitution& unifier,
		  const NarrowingVariableInfo& variableInfo);
  bool keepingHistory() const;
  void getState(int index,
		DagNode*& state,
		int& variableFamily,
		Substitution*& accumulatedSubstitution) const;
  int getRootIndex(int index) const;
  int getDepth(int index) const;
  void getHistory(int index,
		  DagNode*& root,
		  DagNode*& position,
		  Rule*& rule,
		  const Substitution*& unifier,
		  const NarrowingVariableInfo*& unifierVariableInfo,
		  int& variableFamily,
		  DagNode*& newDag,
		  const Substitution*& accumulatedSubstitution,
		  int& parentIndex) const;
  //
  //	Returns NONE if there is no next surviving state.
  //
  int getNextSurvivingState(DagNode*& nextState,
			    Substitution*& nextStateAccumulatedSubstitution,
			    int& nextStateVariableFamily,
			    int& nextStateDepth);
  //
  //	Ensures all the states from the interesting state back to the root are never deleted.
  //
  void lockPathToState(int index);
  //
  //	If we don't explore a returned state, we can call this function to flag the last returned state,
  //	so it stays part of the frontier unless subsumed - though if we've stopped exploring states we
  //	don't expect a new state that could subsumed it to be entered. We can't use the locked flag
  //	because we might not be keeping history.
  //
  void setUnexplored();

  bool exists(int index) const;
  bool locked(int index) const;
  int getParent(int index) const;
  Vector<DagNode*> getMostGeneralStates() const;
  Vector<DagNode*> getUnreturnedStates() const;
  Vector<DagNode*> getReturnedButUnexploredStates() const;
  void dump(ostream& s);

private:
  struct RetainedState
  {
    RetainedState(DagNode* state, int parentIndex, int rootIndex, int depth, bool fold);
    ~RetainedState();
    bool subsumes(DagNode* state) const;
    void markedSubsumed();

    DagNode* const state;
    const int parentIndex;	// index of parent state
    const int rootIndex;	// index of root state
    const int depth;		// number of narrowing steps from root
    int variableFamily = NONE;
    Substitution* accumulatedSubstitution = nullptr;
    //
    //	Only used for folding.
    //
    Term* stateTerm;
    LhsAutomaton* matchingAutomaton;
    int nrMatchingVariables;  // number of variables needed for matching; includes any abstraction variables
    //
    //	Only used for history.
    //
    Rule* rule = nullptr;  // rule used for narrowing step that created this state
    DagNode* narrowingContext = nullptr;
    DagNode* narrowingPosition = nullptr;  //  pointer into old state
    Substitution* unifier = nullptr;  // variant unifier between parent state and rule lhs
    NarrowingVariableInfo variableInfo;  // variable info for the parent state part of the unifier
    //
    //	Only used for history if we are not folding.
    //
    int nrDescendants = 0;
    //
    //	The state is locked against deletion by any of the clean up mechanisms, but if it is subsumed
    //	we don't want to consider it for subsuming a new state or being subsumed in the future.
    //	An unexplored state can never be locked, and thus is deleted if subsumed.
    //
    bool locked = false;
    bool subsumed = false;
    //
    //	When we return a surviving state, we expect that the caller will explore it
    //	and insert all of its next states. But the caller may instead set this flag to
    //	record the state was not explored and remains part of the frontier.
    //	We can still remove this state by subsumption but we can't remove it just because
    //	it has no descendants since they might exist but weren't generated.
    //
    bool returnedButUnexplored = false;
  };

  typedef map<int, RetainedState*> RetainedStateMap;
  typedef set<int> StateSet;

  void markReachableNodes();
  void cleanGraph();
  void doSubsumption(RetainedStateMap::iterator& subsumedStateIter,
		     StateSet& existingStatesSubsumed,
		     int subsumersParent,
		     const StateSet&  subsumersAncestors);


  const bool fold;  // we do folding to prune less general states
  const bool keepHistory;  // we keep the history of how we arrived at each state
  int currentStateIndex;
  RetainedStateMap mostGeneralSoFar;
};

inline void
NarrowingFolder::addAccumulatedSubstitution(int index,
					    int variableFamily,
					    Substitution* accumulatedSubstitution)
{
  RetainedStateMap::iterator i = mostGeneralSoFar.find(index);
  Assert(i != mostGeneralSoFar.end(), "couldn't find state with index " << index);
  i->second->variableFamily = variableFamily;
  i->second->accumulatedSubstitution = accumulatedSubstitution;
}

inline bool
NarrowingFolder::keepingHistory() const
{
  return keepHistory;
}

inline void
NarrowingFolder::getState(int index,
			  DagNode*& state,
			  int& variableFamily,
			  Substitution*& accumulatedSubstitution) const
{
  RetainedStateMap::const_iterator i = mostGeneralSoFar.find(index);
  Assert(i != mostGeneralSoFar.end(), "couldn't find state with index " << index);
  state = i->second->state;
  variableFamily = i->second->variableFamily;
  accumulatedSubstitution = i->second->accumulatedSubstitution;
}

inline int
NarrowingFolder::getDepth(int index) const
{
  RetainedStateMap::const_iterator i = mostGeneralSoFar.find(index);
  Assert(i != mostGeneralSoFar.end(), "couldn't find state with index " << index);
  return i->second->depth;
}

inline int
NarrowingFolder::getRootIndex(int index) const
{
  RetainedStateMap::const_iterator i = mostGeneralSoFar.find(index);
  Assert(i != mostGeneralSoFar.end(), "couldn't find state with index " << index);
  return i->second->rootIndex;
}

inline void
NarrowingFolder::getHistory(int index,
			    DagNode*& root,
			    DagNode*& position,
			    Rule*& rule,
			    const Substitution*& unifier,
			    const NarrowingVariableInfo*& unifierVariableInfo,
			    int& variableFamily,
			    DagNode*& newDag,
			    const Substitution*& accumulatedSubstitution,
			    int& parentIndex) const
{
  RetainedStateMap::const_iterator i = mostGeneralSoFar.find(index);
  Assert(i != mostGeneralSoFar.end(), "couldn't find state with index " << index);

  root = i->second->narrowingContext;
  position = i->second->narrowingPosition;
  rule = i->second->rule;
  unifier = i->second->unifier;
  unifierVariableInfo = &(i->second->variableInfo);
  variableFamily = i->second->variableFamily;
  newDag = i->second->state;
  accumulatedSubstitution = i->second->accumulatedSubstitution;
  parentIndex = i->second->parentIndex;
}

inline bool
NarrowingFolder::exists(int index) const
{
  return mostGeneralSoFar.find(index) != mostGeneralSoFar.end();
}

inline bool
NarrowingFolder::locked(int index) const
{
  auto i =  mostGeneralSoFar.find(index);
  return i == mostGeneralSoFar.end() ? false : i->second->locked;
}

inline int
NarrowingFolder::getParent(int index) const
{
  auto i =  mostGeneralSoFar.find(index);
  Assert(i != mostGeneralSoFar.end(), "couldn't find state with index " << index);
  return i->second->parentIndex;
}

inline void
NarrowingFolder::RetainedState::markedSubsumed()
{
  //
  //	Normally we delete a subsumed state, but if it is locked because it is on a path
  //	that produced a solution, we minimize it and mark it instead.
  //
  delete matchingAutomaton;
  matchingAutomaton = nullptr;
  if (stateTerm)
    {
      stateTerm->deepSelfDestruct();
      stateTerm = nullptr;
    }
  subsumed = true;
}

inline void
NarrowingFolder::setUnexplored()
{
  auto i =  mostGeneralSoFar.find(currentStateIndex);
  Assert(i != mostGeneralSoFar.end(), "couldn't find state with index " << currentStateIndex);
  i->second->returnedButUnexplored = true;
}

#endif
