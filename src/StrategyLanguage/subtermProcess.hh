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
//	Class for the matchrew main pattern search.
//
#ifndef _subtermProcess_hh_
#define _subtermProcess_hh_
#include <memory>
#include "strategicProcess.hh"

class SubtermProcess : public StrategicProcess
{
  NO_COPYING(SubtermProcess);

public:
  SubtermProcess(MatchSearchState* matchState,
		 SubtermStrategy* strategy,
		 VariableBindingsManager::ContextId varBinds,
		 StrategicExecution* taskSibling,
		 StrategicProcess* insertionPoint);

  Survival run(StrategicSearch& searchObject);

private:
  // Will be shared with the matchrew tasks it will create
  shared_ptr<MatchSearchState> matchState;
  SubtermStrategy* strategy;
  StrategyStackManager::StackId pending;
};

#endif
