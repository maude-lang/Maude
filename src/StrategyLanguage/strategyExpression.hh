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
//      Abstract base class for expressions in the strategy language.
//
#ifndef _strategyExpression_hh_
#define _strategyExpression_hh_
#include "strategicExecution.hh"

#include "term.hh"
#include "termSet.hh"

class StrategyExpression
{
public:
  virtual ~StrategyExpression() {}

  //
  // Recursively check that the variables in the expression are bound.
  //
  // NOTE A common VariableInfo has not been used because some expressions have
  // patterns which have their own variable account. We pass variable terms and
  // as const-reference because the set only change in one case.
  //
  virtual bool check(VariableInfo& vars, const TermSet& boundVars) = 0;

  //
  // Process the strategy expression when the module type information is closed.
  // Patterns are compiled, and terms are filled with type information.
  //
  // check must have been called before.
  //
  virtual void process() = 0;

  //
  // When calling decompose, process must have been called before.
  //
  virtual StrategicExecution::Survival decompose(StrategicSearch& searchObject, DecompositionProcess* remainder) = 0;

  //
  // Compare two strategy expressions with an incomplete equality relation.
  //
  // By default, memory positions are compared.
  //
  virtual bool equal(const StrategyExpression& other) const;
};

ostream& operator<<(ostream& s, StrategyExpression* strategy);

#endif
