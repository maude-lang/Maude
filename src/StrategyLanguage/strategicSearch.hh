/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2006 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for performing a search using a strategy.
//
#ifndef _strategicSearch_hh_
#define _strategicSearch_hh_
#include "cacheableState.hh"
#include "hashConsSet.hh"
#include "strategicTask.hh"
#include "strategyStackManager.hh"
#include "variableBindingsManager.hh"

class StrategicSearch
 : public CacheableState,
   public HashConsSet,
   public StrategyStackManager,
   public VariableBindingsManager,
   protected StrategicTask
{
  NO_COPYING(StrategicSearch);

public:
  //
  //	We take ownership of initial and strategy and delete them in our dtor.
  //
  StrategicSearch(RewritingContext* initial, StrategyExpression* strategy);
  ~StrategicSearch();

  virtual DagNode* findNextSolution() = 0;
  DagNode* lastSolution() const;
  RewritingContext* getContext();

private:
  //
  //	Virtual in StrategicTask.
  //
  Survival executionSucceeded(int resultIndex, StrategicProcess* insertionPoint);
  Survival executionsExhausted(StrategicProcess* insertionPoint);

  StrategyExpression* strategy;

protected:
  RewritingContext* initial;
  bool exhausted;
  int solutionIndex;
};

inline DagNode*
StrategicSearch::lastSolution() const
{
  return solutionIndex != NONE ? getCanonical(solutionIndex) : 0;
}

inline RewritingContext*
StrategicSearch::getContext()
{
  return initial;
}

#endif
