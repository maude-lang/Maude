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
//      Class for iteration strategies.
//
#ifndef _iterationStrategy_hh_
#define _iterationStrategy_hh_
#include "strategyExpression.hh"

class IterationStrategy : public StrategyExpression
{
public:
  IterationStrategy(StrategyExpression* child, bool zeroAllowed);
  ~IterationStrategy();

  StrategyExpression* getStrategy() const;
  bool getZeroAllowed() const;
  StrategicExecution::Survival decompose(StrategicSearch& searchObject, DecompositionProcess* remainder);

private:
  StrategyExpression* const child;
  StrategyExpression* const star;  // for a s+ expression we keep an s* expression here for decomposition
};

inline StrategyExpression*
IterationStrategy::getStrategy() const
{
  return child;
}

inline bool
IterationStrategy::getZeroAllowed() const
{
  return star == 0;
}

#endif
