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
//      Class for managing a collection of persistent stacks of strategies with sharing.
//
#ifndef _strategyStackManager_hh_
#define _strategyStackManager_hh_

class StrategyStackManager
{
public:
  typedef int StackId;

  enum Constants
  {
    EMPTY_STACK = 0
  };

  StrategyStackManager();

  static StackId emptyStack();
  bool empty(StackId stackId) const;
  StrategyExpression* top(StackId stackId) const;
  StackId pop(StackId stackId) const;
  StackId push(StackId stackId, StrategyExpression* strategy);
  
private:
  struct Entry
  {
    StrategyExpression* strategy;
    StackId restOfStack;
    //
    //	This fields are use solelyto keep a linked list of entries having
    //	the same restOfStack so we can minimize the creation of duplicate
    //	entries.
    //
    StackId firstSuccessor;	// first stack that has us as restOfStack
    StackId nextPeer;		// next stack that shares our restOfStack
  };
  
  Vector<Entry> stackTable;
};

inline bool
StrategyStackManager::empty(StackId stackId) const
{
  return stackId == EMPTY_STACK;
}

inline StrategyExpression* 
StrategyStackManager::top(StackId stackId) const
{
  return stackTable[stackId].strategy;
}

inline StrategyStackManager::StackId
StrategyStackManager::pop(StackId stackId) const
{
  return stackTable[stackId].restOfStack;
}

#endif
