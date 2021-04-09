/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2010 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for strategy definitions
//
#ifndef _strategyDefinition_hh_
#define _strategyDefinition_hh_
#include "strategyLanguage.hh"
#include "preEquation.hh"

class StrategyDefinition : public PreEquation
{
public:
  StrategyDefinition(int label,
		     RewriteStrategy* strategy,
		     Term* lhs,
		     StrategyExpression* rhs,
		     const Vector<ConditionFragment*>& condition = noCondition);
  ~StrategyDefinition();

  using PreEquation::getLhs;
  StrategyExpression* getRhs() const;
  RewriteStrategy* getStrategy() const;

  //
  // Gets the strategy definition context specification.
  //
  // The vector includes as many entries as variables in the context,
  // each containing the index of the left-hand side variable to be
  // initialized with.
  //
  // This context specification is calculated by check().
  //
  const Vector<int>& getContextSpec() const;

  void check();
  void preprocess();
  void compile(bool compileLhs = true);
  long fastNrVariables() const;
  void print(ostream& s) const;

  // void stackMachineCompile();
  // Instruction* getInstructionSequence() const;

private:
  int traceBeginTrial(DagNode* subject, RewritingContext& context) const;

  RewriteStrategy* strategy;
  StrategyExpression* rhs;
  Vector<int> contextSpec;
};

inline StrategyExpression*
StrategyDefinition::getRhs() const
{
  return rhs;
}

inline RewriteStrategy*
StrategyDefinition::getStrategy() const
{
  return strategy;
}

inline const Vector<int>&
StrategyDefinition::getContextSpec() const
{
  return contextSpec;
}

//
//      Output function for StrategyDefinition must be defined by library user.
//
ostream& operator<<(ostream& s, const StrategyDefinition* equation);

#endif
