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
//      Class for strategy invocation.
//
#ifndef _callStrategy_hh_
#define _callStrategy_hh_
#include "cachedDag.hh"
#include "variableInfo.hh"
#include "strategyExpression.hh"

class CallStrategy : public StrategyExpression
{
public:
  CallStrategy(RewriteStrategy* strat, Term* call);

  Term* getTerm() const;
  RewriteStrategy* getStrategy() const;

  bool check(VariableInfo& indices, const TermSet& boundVars);
  void process();

  StrategicExecution::Survival decompose(StrategicSearch& searchObject, DecompositionProcess* remainder);

  bool equal(const StrategyExpression& other) const;

private:
  RewriteStrategy* strategy;
  CachedDag callTerm;
  bool callDagIsReduced;

  static bool tailCall(DecompositionProcess* remainder);
};

inline Term*
CallStrategy::getTerm() const
{
  return callTerm.getTerm();
}

inline RewriteStrategy*
CallStrategy::getStrategy() const
{
  return strategy;
}

#endif
