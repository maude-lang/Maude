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
//      Implementation for abstract class StrategicTask.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"

//	strategy language class definitions
#include "strategicTask.hh"

StrategicTask::StrategicTask(StrategicTask* master)
  : StrategicExecution(master),
    slaveList(this)
{
}

StrategicTask::StrategicTask(StrategicExecution* sibling)
  : StrategicExecution(sibling),
    slaveList(this)
{
}

StrategicTask::~StrategicTask()
{
  //
  //	Once we disappear, our slaves will have no place to report to, so
  //	we must delete them first.
  //
  StrategicExecution* next;
  for (StrategicExecution* i = slaveList.getNextSlave(); i != &slaveList; i = next)
    {
      next = i->getNextSlave();
      delete i;
    }
}

/*
bool
StrategicTask::alreadySeen(int dagIndex, StrategyStackManager::StackId stackId)
{
  State s(dagIndex, stackId);
  return !(seenSet.insert(s).second);
}
*/

bool
StrategicTask::alreadySeen(int dagIndex, StrategyStackManager::StackId stackId)
{
  State s(dagIndex, stackId);
  pair<SeenSet::iterator, bool> p = seenSet.insert(s);  // to avoid UMR in purify
  return !(p.second);
}
