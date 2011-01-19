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
//	Class for a process that tries to decompose a strategy to reach a leaf.
//
#ifndef _decompositionProcess_hh_
#define _decompositionProcess_hh_
#include "strategicProcess.hh"
#include "strategyStackManager.hh"

class DecompositionProcess : public StrategicProcess
{
public:
  DecompositionProcess(int dagIndex,
		       StrategyStackManager::StackId pending,
		       StrategicExecution* taskSibling,
		       StrategicProcess* other);
  //
  //	We can't pass const here because we need to touch original to
  //	handle double linking both on the task list and the process queue.
  //
  DecompositionProcess(DecompositionProcess* original);

  Survival run(StrategicSearch& searchObject);
  void pushStrategy(StrategyStackManager& stackManager, StrategyExpression* strategy);
  StrategyStackManager::StackId getPending() const;
  int getDagIndex() const;

private:
  int dagIndex;
  StrategyStackManager::StackId pending;
};

inline void
DecompositionProcess::pushStrategy(StrategyStackManager& stackManager, StrategyExpression* strategy)
{
  pending = stackManager.push(pending, strategy);
}

inline StrategyStackManager::StackId
DecompositionProcess::getPending() const
{
  return pending;
}

inline int
DecompositionProcess::getDagIndex() const
{
  return dagIndex;
}

#endif
