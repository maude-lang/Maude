/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for finding a sequence of matches and condition solutions.
//
#ifndef _SearchState_hh_
#define _SearchState_hh_
#include <stack>
#include "positionState.hh"
#include "rewritingContext.hh"

class SearchState : public PositionState
{
  NO_COPYING(SearchState);

public:
  enum Flags
  {
    GC_CONTEXT = 2,	// delete context in our dtor
    GC_SUBSTITUTION = 4	// delete substitution in our dtor
  };

  SearchState(RewritingContext* context,
	      int flags = 0,
	      int minDepth = 0,
	      int maxDepth = -1);
  virtual ~SearchState();

  bool findFirstSolution(const PreEquation* preEqn, LhsAutomaton* automaton);
  bool findNextSolution();

  RewritingContext* getContext() const;
  void transferCount(RewritingContext& recipient);

  void setInitialSubstitution(Vector<Term*>& variables, Vector<DagRoot*>& values);

private:
  bool initSubstitution(const VariableInfo& varInfo);

  RewritingContext* const context;
  const PreEquation* preEquation;
  //
  //	Initial (partial) substitution.
  //
  Vector<Term*> substVariables;
  Vector<DagRoot*> substValues;
  //
  //	For backtracking over matches.
  //
  Subproblem* matchingSubproblem;
  //
  //	For backtracking of solutions to a rule condition.
  //
  int trialRef;
  stack<ConditionState*> conditionStack;
};

inline RewritingContext*
SearchState::getContext() const
{
  return context;
}

inline void
SearchState::transferCount(RewritingContext& recipient)
{
  recipient.addInCount(*context);
  context->clearCount();
}

inline void
SearchState::setInitialSubstitution(Vector<Term*>& variables, Vector<DagRoot*>& values)
{
  substVariables.swap(variables);
  substValues.swap(values);
}

#endif
