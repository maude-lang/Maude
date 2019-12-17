/*

    This file is part of the Maude 3 interpreter.

    Copyright 2014 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for searching for sequences of rewrites modulo SMT.
//
#ifndef _SMT_RewriteSequenceSearch_hh_
#define _SMT_RewriteSequenceSearch_hh_
#include <map>
#include "gmpxx.h"
#include "natSet.hh"
#include "sequenceSearch.hh"
#include "simpleRootContainer.hh"
#include "SMT_RewriteSearchState.hh"
#include "matchSearchState.hh"
#include "pattern.hh"

class SMT_RewriteSequenceSearch :
  public Pattern,
  public SequenceSearch,
  private SimpleRootContainer
{
public:
  //
  //	Takes responsibility for deleting:
  //	  inital
  //	  target
  //	  condition fragments in condition
  //	  engine
  //
  SMT_RewriteSequenceSearch(RewritingContext* initial,
			    SearchType searchType,
			    Term* target,
			    const Vector<ConditionFragment*>& condition,
			    const SMT_Info& smtInfo,
			    SMT_EngineWrapper* engine,
			    int maxDepth = -1,
			    const mpz_class& avoidVariableNumber = 0);
  ~SMT_RewriteSequenceSearch();

  bool findNextMatch();
  //
  //	Information particular to most recent match.
  //
  const Substitution* getSubstitution() const;
  DagNode* getFinalConstraint() const;  // conjunction of constraints from state and constraints from match
  const mpz_class& getMaxVariableNumber() const;  // largest fresh variable appearing in substitution or constraint

  const NatSet& getSMT_VarIndices() const;
  RewritingContext* getContext() const;

  int findNextState();  // returns NONE or state number

  DagNode* getState(int stateNr) const;
  DagNode* getConstraint(int stateNr) const;
  Rule* getStateRule(int stateNr) const;  // rule used to generate state
  int getStateParent(int stateNr) const;  // index of parent state or NONE for initial state

  int getCurrentStateNumber() const;

private:
  struct State
  {
    mpz_class avoidVariableNumber;
    SMT_RewriteSearchState* search;
    RewritingContext* context;  // context containing state dag
    DagNode* constraint;
    int parent;
    Rule* rule;
    int depth;
  };

  typedef map<int, DagNode*> SMT_VarDags;
  typedef Vector<State*> StateVec;

  void markReachableNodes();
  DagNode* makeConstraintFromCondition(Term* target, const Vector<ConditionFragment*>& condition);
  void findSMT_Variables();

  int makeNewState();
  bool checkMatchConstraint();
  //
  //	Data passed at initialization.
  //
  const SMT_Info& smtInfo;  // information about SMT sort; might get folded into wrapper
  SMT_EngineWrapper* const engine;  // wrapper to call the SMT engine
  const int maxDepth;
  //
  //	Information abound SMT variables in target, computed at initialization.
  //
  NatSet smtVarIndices;
  SMT_VarDags smtVarDags;
  //
  //	Runtime data structures.
  //
  StateVec states;
  bool needToTryInitialState;
  int stateToExplore;
  int newStateNr;
  MatchSearchState* matchState;
  DagNode* finalConstraint;  // conjunction of state constraint and match constraint
};


inline const Substitution*
SMT_RewriteSequenceSearch::getSubstitution() const
{
  return matchState->getContext();
}

inline DagNode*
SMT_RewriteSequenceSearch::getFinalConstraint() const
{
  return finalConstraint;
}

inline const mpz_class&
SMT_RewriteSequenceSearch::getMaxVariableNumber() const
{
  DebugAdvisory("getMaxVariableNumber() newStateNumber = " << newStateNr << " returning " << states[newStateNr]->avoidVariableNumber);
  return states[newStateNr]->avoidVariableNumber;
}

inline const NatSet&
SMT_RewriteSequenceSearch::getSMT_VarIndices() const
{
  return smtVarIndices;
}

inline RewritingContext*
SMT_RewriteSequenceSearch::getContext() const
{
  return states[0]->context;
}
   
inline int
SMT_RewriteSequenceSearch::getCurrentStateNumber() const
{
  return newStateNr;
}

inline DagNode*
SMT_RewriteSequenceSearch::getState(int stateNr) const
{
  return states[stateNr]->context->root();
}

inline DagNode*
SMT_RewriteSequenceSearch::getConstraint(int stateNr) const
{
  return states[stateNr]->constraint;
}

inline int
SMT_RewriteSequenceSearch::getStateParent(int stateNr) const
{
  return states[stateNr]->parent;
}

inline Rule*
SMT_RewriteSequenceSearch::getStateRule(int stateNr) const
{
  return states[stateNr]->rule;
}

#endif
