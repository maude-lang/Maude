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
//      Class for concatenation strategy combinator.
//
#ifndef _concatenationStrategy_hh_
#define _concatenationStrategy_hh_
#include "strategyExpression.hh"

class ConcatenationStrategy : public StrategyExpression
{
public:
  ConcatenationStrategy(const Vector<StrategyExpression*>& strategies);
  ~ConcatenationStrategy();

  const Vector<StrategyExpression*>& getStrategies() const;

  bool check(VariableInfo& indices, const TermSet& boundVars);
  void process();

  StrategicExecution::Survival decompose(StrategicSearch& searchObject, DecompositionProcess* remainder);

private:
  const Vector<StrategyExpression*> strategies;
};

inline const Vector<StrategyExpression*>&
ConcatenationStrategy::getStrategies() const
{
  return strategies;
}

#endif
