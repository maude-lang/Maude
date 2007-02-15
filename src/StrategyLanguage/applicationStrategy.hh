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
//      Class for rule application strategy.
//
#ifndef _applicationStrategy_hh_
#define _applicationStrategy_hh_
#include "strategyExpression.hh"
#include "cachedDag.hh"

class ApplicationStrategy : public StrategyExpression
{
public:
  //
  //	label == NONE means try all rules, labeled or not.
  //
  ApplicationStrategy(int label,
		      const Vector<Term*>& variables,
		      const Vector<Term*>& values,
		      const Vector<StrategyExpression*>& strategies);
  ~ApplicationStrategy();

  void setTop();
  bool getTop() const;
  int getLabel() const;
  const Vector<Term*>& getVariables() const;
  Vector<CachedDag>& getValues();
  const Vector<StrategyExpression*>& getStrategies() const;

  StrategicExecution::Survival decompose(StrategicSearch& searchObject, DecompositionProcess* remainder);

private:
  bool top;  					// restrict rewrites to top of term
  const int label;
  Vector<Term*> variables;			// substitution
  Vector<CachedDag> valueDags;
  Vector<StrategyExpression*> strategies;	// strategies for searches in conditions
};

inline void
ApplicationStrategy::setTop()
{
  top = true;
}

inline bool
ApplicationStrategy::getTop() const
{
  return top;
}

inline int
ApplicationStrategy::getLabel() const
{
  return label;
}

inline const Vector<Term*>&
ApplicationStrategy::getVariables() const
{
  return variables;
}

inline Vector<CachedDag>&
ApplicationStrategy::getValues()
{
  return valueDags;
}

inline const Vector<StrategyExpression*>&
ApplicationStrategy::getStrategies() const
{
  return strategies;
}

#endif
