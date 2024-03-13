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
//	Class for task that solves a rewrite condition fragment by initiating
//	a search with a substrategy.
//
#ifndef _rewriteTask_hh_
#define _rewriteTask_hh_
#include <memory>
#include "strategicTask.hh"

class RewriteTask : public StrategicTask
{
  NO_COPYING(RewriteTask);

public:
  RewriteTask(StrategicSearch& searchObject,
	      shared_ptr<RewriteSearchState> rewriteState,
	      PositionState::PositionIndex redexIndex,
	      ExtensionInfo* extensionInfo,
	      Substitution* substitutionSoFar,
	      Rule* rule,
	      int fragmentNr,
	      const Vector<StrategyExpression*>& strategies,
	      int strategyNr,
	      StrategyStackManager::StackId pending,
	      StrategicExecution* taskSibling,
	      StrategicProcess* insertionPoint);
  ~RewriteTask();
  //
  //	Call-backs for interesting events.
  //
  virtual Survival executionSucceeded(int resultindex, StrategicProcess* insertionPoint);
  virtual Survival executionsExhausted(StrategicProcess* insertionPoint);

private:
  const HashConsSet& hashConsSet;			// reference to shared hash cons set
  shared_ptr<RewriteSearchState> rewriteState;		// smart pointer to rewrite state that found our redex
  const PositionState::PositionIndex redexIndex;	// index of redex withing rewrite state
  ExtensionInfo* extensionInfoCopy;			// copy of extension info from original match
  Rule* const rule;					// pointer to rule whose lhs matched
  const int fragmentNr;					// number of condition fragment within rule
  const Vector<StrategyExpression*>& strategies;	// vector of strategies for rewrite fragments
  const int strategyNr;					// number of next strategy to use
  StrategyStackManager::StackId pending;       		// continuation once we finally do a rewrite

  RewriteConditionFragment* rcf;
  //
  //	We generate a fresh subcontext to evaluate the lhs of the rewrite condition fragment.
  //
  RewritingContext* newContext;
};

#endif
