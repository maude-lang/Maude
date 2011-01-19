/*

    This file is part of the Maude 2 interpreter.

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
#ifndef _stategicSearch_hh_
#define _stategicSearch_hh_
#include<list>
#include "hashConsSet.hh"
#include "strategicTask.hh"
#include "strategyStackManager.hh"

class StrategicSearch : public HashConsSet, public StrategyStackManager, private StrategicTask
{
  NO_COPYING(StrategicSearch);

public:
  //
  //	We take ownership of initial and strategy and delete them in our dtor.
  //
  StrategicSearch(RewritingContext* initial, StrategyExpression* strategy);
  ~StrategicSearch();

  DagNode* findNextSolution();
  RewritingContext* getContext();

private:
  //
  //	Virtual in StrategicTask.
  //
  Survival executionSucceeded(int resultIndex, StrategicProcess* insertionPoint);
  Survival executionsExhausted(StrategicProcess* insertionPoint);

  RewritingContext* initial;
  StrategyExpression* strategy;
  bool exhausted;
  int solutionIndex;
  StrategicProcess* nextToRun;
};

inline RewritingContext*
StrategicSearch::getContext()
{
  return initial;
}

#endif
