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
//	Class for applying rls.
//
#ifndef _applicationProcess_hh_
#include <memory>
#define _applicationProcess_hh_
#include "strategicProcess.hh"
#include "rewriteSearchState.hh"
#include "strategyStackManager.hh"

class ApplicationProcess : public StrategicProcess
{
  NO_COPYING(ApplicationProcess);

public:
  ApplicationProcess(StrategicSearch& searchObject,
		     int startIndex,
		     ApplicationStrategy* strategy,
		     StrategyStackManager::StackId pending,
		     StrategicExecution* taskSibling,
		     StrategicProcess* insertionPoint);

  ~ApplicationProcess();

  Survival run(StrategicSearch& searchObject);

  static StrategicExecution::Survival
    resolveRemainingConditionFragments(StrategicSearch& searchObject,
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
				       StrategicProcess* other);

private:
  static int doRewrite(StrategicSearch& searchObject,
		       shared_ptr<RewriteSearchState> rewriteState,
		       PositionState::PositionIndex redexIndex,
		       ExtensionInfo* extensionInfo,
		       Substitution* substitution,
		       Rule* rule);

  shared_ptr<RewriteSearchState> rewriteState;
  StrategyStackManager::StackId pending;
  ApplicationStrategy* strategy;

  Vector<DagRoot*> instedSubstitution;
};

#endif
