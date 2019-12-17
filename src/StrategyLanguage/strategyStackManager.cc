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
//      Implementation for class StrategyStackManager.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"

//	strategy language class definitions
#include "strategyExpression.hh"
#include "strategyStackManager.hh"

StrategyStackManager::StrategyStackManager()
  : stackTable(1)
{
  //
  //	Create empty stack entry.
  //
  Entry& e = stackTable[0];
  e.strategy = 0;
  e.restOfStack = NONE;
  e.firstSuccessor = NONE;
  e.nextPeer = NONE;
}

StrategyStackManager::StackId
StrategyStackManager::push(StackId stackId, StrategyExpression* strategy)
{
  for (StackId i = stackTable[stackId].firstSuccessor; i != NONE; i = stackTable[i].nextPeer)
    {
      //
      //	Ideally we would like to compare strategies for equality rather than pointers
      //	but for the moment we just use pointers for simplicity. We don't expect too many
      //	sucessors to a given stack otherwise hashing might be a win.
      //
      if (stackTable[i].strategy->equal(*strategy))
	return i;
    }
  StackId newStackId = stackTable.size();
  stackTable.expandBy(1);
  Entry& e = stackTable[newStackId];
  e.strategy = strategy;
  e.restOfStack = stackId;
  e.firstSuccessor = NONE;
  e.nextPeer = stackTable[stackId].firstSuccessor;
  stackTable[stackId].firstSuccessor = newStackId;
  return newStackId;
}
