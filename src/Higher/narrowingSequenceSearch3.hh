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
//	Class for breadth-first searching for sequences of narrowing steps using variant unification.
//
#ifndef _narrowingSequenceSearch3_hh_
#define _narrowingSequenceSearch3_hh_
#include "sequenceSearch.hh"
#include "narrowingFolder.hh"
#include "narrowingSearchState3.hh"
#include "dagRoot.hh"
#include "rewritingContext.hh"

class NarrowingSequenceSearch3 : public SequenceSearch, private SimpleRootContainer
{
  NO_COPYING(NarrowingSequenceSearch3);

public:
  //
  //	This extends VariantSearch::Flags and VariantUnificationProblem::Flags.
  //
  enum Flags
    {
     //
     //	Fold using matching.
     //
     FOLD = 0x10000,
     //
     //	Fold using variant subsumption (to be implemented).
     //
     VFOLD = 0x20000,
     //
     //	Keep the history of the current path. States can still be deleted once they are no longer on the
     //	current path, have no live descendents, and aren't needed for folding. This is intended for the
     //	metalevel where one cannot revisit old results without redoing the whole computation.
     //
     KEEP_HISTORY = 0x40000,
     //
     //	Keep the history of the path to every state (subsumed or otherwise) that provided a unifier with goal.
     //	Potentially very expensive in terms of memory. This is intended for the object level where the
     //	user can ask for the path to any state that yielded a unifier (to be implemented).
     //
     KEEP_PATHS = 0x80000,
    };
  //
  //	We take responsibility for protecting startStates and goal and deleting initial and freshVariableGenerator.
  //
  NarrowingSequenceSearch3(RewritingContext* initial,
			   const Vector<DagNode*>& startStates,
			   SearchType searchType,
			   DagNode* goal,
			   int maxDepth,  // NONE = unbounded
			   FreshVariableGenerator* freshVariableGenerator,
			   int variantFlags);
  ~NarrowingSequenceSearch3();

  bool findNextUnifier();
  //
  //	This is the mapping between original variable names and slot numbers
  //	in the various accumulated substitutions. It doesn't change.
  //
  const NarrowingVariableInfo& getInitialVariableInfo() const;
  //
  //	Get information about current state.
  //
  void getStateInfo(DagNode*& stateDag,
		    int& variableFamily,
		    DagNode*& initialStateDag,
		    Substitution*& accumulatedSubstitution) const;
  void getExtraStateInfo(int& index, int& depth) const;
  //
  //	Get information about some state in the history.
  //
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
  //	Get information about the current variant unifier between the current state and the (instantiated) goal.
  //
  const Vector<DagNode*>* getUnifier() const;
  int getUnifierVariableFamily() const;
  const NarrowingVariableInfo& getUnifierVariableInfo() const;

  RewritingContext* getContext() const;
  bool isIncomplete() const;
  int getNrInitialStates() const;
  int getVariantFlags() const;
  bool problemOK() const;
  bool locked(int index) const;
  int getStateParent(int index) const;
  //
  //	Most general states only make sense if we're folding and they are the set of states that
  //	have not been subsumed.
  //
  Vector<DagNode*> getMostGeneralStates() const;
  //
  //	The set of states that have not been visited for expansion. This is the frontier, possibly
  //	minus one state.
  //
  Vector<DagNode*> getUnvisitedStates() const;
  //
  //	In the =>1, =>+, =>* cases:
  //	There might be a state that was visited but not (fully) expanded. This could be because it
  //	is at max depth or because we haven't finished expanding it. The should also be part of
  //	the frontier if it exists. If it doesn't exist, we expect the frontier to be empty, because
  //	as soon as we exhaust the expansion of the state, we look for another state to expand.
  // 	stateBeingExpanded can only be NONE if there are no states in the frontier.
  //
  //	In the =>! case:
  //	We visit states, but might not expand them if they are max depth. In this case we can have
  //	more than one unexpanded state, but none of them will be partially expanded since they can
  //	only be considered for unification with the goal if they have no descendants.
  //
  Vector<DagNode*> getUnexpandedStates(bool& partiallyExpanded);

private:
  //
  //	If we have multiple state states, we need one of these structs for each such state.
  //
  struct InitialState
  {
    DagNode* state;
    NarrowingVariableInfo varInfo;
  };

  bool handleInitialState(DagNode* dagToNarrow, NarrowingVariableInfo& varInfo);
  void markReachableNodes();
  int findNextInterestingState();
  bool findNextNormalForm();
  //
  //	Initial stuff.
  //
  RewritingContext* initial;
  DagNode* goal;
  int nrInitialStatesToTry;
  const int maxDepth;
  const bool normalFormNeeded;
  const bool termDisjunction;
  FreshVariableGenerator* freshVariableGenerator;
  const int variantFlags;
  Vector <InitialState> initialStates;
  NarrowingFolder stateCollection;
  //
  //	Maps variables occurring in initial state which will be the
  //	from variables for the accumulated substitution.
  //
  NarrowingVariableInfo initialVariableInfo;
  //
  //	Used to temporarily protect the range of a substitution from the garbage collector.
  //
  Substitution* protectedSubstitution;
  //
  //	Pairing symbol for variant unification of state vs pattern.
  //
  Symbol* unificationPairSymbol;
  //
  //	State of search.
  //
  NarrowingSearchState3* stateBeingExpanded;
  int stateBeingExpandedIndex;
  int stateBeingExpandedDepth;
  bool expansionSuccessful;
  int nextInterestingState;
  int counter;
  int nrStatesExpanded;
  DagRoot replacementContextProtector;  // to protect replacementContext during reduce()
  //
  //	Final variant unification between goal and state.
  //
  VariantSearch* unificationProblem;
  const Vector<DagNode*>* currentUnifier;
  int nrFreeVariablesInUnifier;
  int variableFamilyInUnifier;

  bool incompleteFlag;
  bool problemOkay;
};

inline bool
NarrowingSequenceSearch3::problemOK() const
{
  return problemOkay;
}

inline const Vector<DagNode*>*
NarrowingSequenceSearch3::getUnifier() const
{
  return currentUnifier;
}

inline const NarrowingVariableInfo&
NarrowingSequenceSearch3::getUnifierVariableInfo() const
{
  return unificationProblem->getVariableInfo();
}

inline const NarrowingVariableInfo&
NarrowingSequenceSearch3::getInitialVariableInfo() const
{
  return termDisjunction ? initialStates[stateCollection.getRootIndex(nextInterestingState)].varInfo : initialVariableInfo;
}

inline void
NarrowingSequenceSearch3::getStateInfo(DagNode*& stateDag,
				       int& variableFamily,
				       DagNode*& initialStateDag,
				       Substitution*& accumulatedSubstitution) const
{
  stateCollection.getState(nextInterestingState, stateDag, variableFamily, accumulatedSubstitution);
  
  initialStateDag = termDisjunction ? initialStates[stateCollection.getRootIndex(nextInterestingState)].state : initial->root();
}

inline void
NarrowingSequenceSearch3::getExtraStateInfo(int& index, int& depth) const
{
  index = nextInterestingState;
  depth = stateCollection.getDepth(nextInterestingState);
}

inline void
NarrowingSequenceSearch3::getHistory(int index,
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
  stateCollection.getHistory(index,
			     root,
			     position,
			     rule,
			     unifier,
			     unifierVariableInfo,
			     variableFamily,
			     newDag,
			     accumulatedSubstitution,
			     parentIndex);
}

inline Vector<DagNode*>
NarrowingSequenceSearch3::getMostGeneralStates() const
{
  return stateCollection.getMostGeneralStates();
}

inline Vector<DagNode*>
NarrowingSequenceSearch3::getUnvisitedStates() const
{
  return stateCollection.getUnreturnedStates();
}

inline Vector<DagNode*>
NarrowingSequenceSearch3::getUnexpandedStates(bool& partiallyExpanded)
{
  if (normalFormNeeded)
    {
      //
      //	If we have a stateBeingExpanded, we know it has no descendants, otherwise we would have
      //	gone on to the next state. If we don't have a stateBeingExpanded we may still have
      //	states that had descendants but we didn't expand them out to avoid exceeding a depth bound.
      //
      partiallyExpanded = false;
      return stateCollection.getReturnedButUnexploredStates();
    }
  //
  //	If we don't have state being expanded for other search types then we have no visited but unexpanded states.
  //
  if (stateBeingExpandedIndex == NONE)
    {
      partiallyExpanded = false;
      return Vector<DagNode*>();
    }
  //
  //	If we made the NarrowingSearchState to expand it, it must be partly expanded.
  //
  partiallyExpanded = (stateBeingExpanded != nullptr);
  //
  //	Because we haven't asked for another surviving state, stateBeingExpandedIndex shouldn't have been
  //	garbage collected by being considered fully expanded; furthermore the descendents of
  //	of a state are not allowed to subsume it, so even if we have partly expanded it, it
  //	should still exist in stateCollection.
  //
  Assert(stateCollection.exists(stateBeingExpandedIndex), "state " << stateBeingExpandedIndex << " doesn't exist");
  //
  //	Not very efficient but it is just done once to see the frontier.
  //
  Vector<DagNode*> singleton(1);
  int variableFamily;
  Substitution* accumulatedSubstitution;
  stateCollection.getState(stateBeingExpandedIndex, singleton[0], variableFamily, accumulatedSubstitution);
  return singleton;
}

inline int
NarrowingSequenceSearch3::getUnifierVariableFamily() const
{
  return variableFamilyInUnifier;
}

inline RewritingContext*
NarrowingSequenceSearch3::getContext() const
{
  return initial;
}

inline bool
NarrowingSequenceSearch3::isIncomplete() const
{
  //
  //	Returns true if any incompleteness has been encountered so far.
  //
  return incompleteFlag;
}

inline int
NarrowingSequenceSearch3::getNrInitialStates() const
{
  return termDisjunction ? initialStates.size() : 1;
}

inline int
NarrowingSequenceSearch3::getVariantFlags() const
{
  return variantFlags;
}

inline bool
NarrowingSequenceSearch3::locked(int index) const
{
  return stateCollection.locked(index);
}

inline int
NarrowingSequenceSearch3::getStateParent(int index) const
{
  return stateCollection.getParent(index);
}

#endif
