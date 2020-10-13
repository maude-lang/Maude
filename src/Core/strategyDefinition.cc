/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class StrategyDefinition.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "term.hh"

//	core class definitions
#include "strategyDefinition.hh"
#include "termBag.hh"
#include "rewriteStrategy.hh"
#include "rewritingContext.hh"

//	strategy language definitions
#include "strategyLanguage.hh"
#include "strategyExpression.hh"

StrategyDefinition::StrategyDefinition(int label,
				       RewriteStrategy* strategy,
				       Term* lhs,
				       StrategyExpression* rhs,
				       const Vector<ConditionFragment*>& condition)
  : PreEquation(label, lhs, condition),
    strategy(strategy),
    rhs(rhs)
{
  Assert(rhs, "null rhs");
}

StrategyDefinition::~StrategyDefinition()
{
  delete rhs;
}

void
StrategyDefinition::check()
{
  NatSet boundVariables;
  PreEquation::check(boundVariables);

  // Non-executable strategy definitions are not processed
  if (isNonexec())
    return;

  // First, check unbound variables in the condition

  const NatSet& unboundVariables = getUnboundVariables();

  if (!unboundVariables.empty())
    {
      IssueWarning(*this << ": variable " <<
		   QUOTE(index2Variable(getUnboundVariables().min())) <<
		   " is used before it is bound in strategy definition:\n" <<
		   this);
      markAsBad();
      return;
    }

  // The RHS expression is not indexed with a single VariableInfo because
  // it has different indices depending on the static context

  TermSet boundVars;
  VariableInfo vinfo;

  for (int index = 0; index < getNrRealVariables(); index++)
    if (boundVariables.contains(index))
      boundVars.insert(index2Variable(index));

  if (!rhs->check(vinfo, boundVars))
    // Errors are printed inside the check methods
    markAsBad();
  else
    {
      // Builds the context creation vector

      size_t rhsVars = vinfo.getNrRealVariables();
      contextSpec.expandTo(rhsVars);

      for (size_t i = 0; i < rhsVars; i++)
	{
	  Term* var = vinfo.index2Variable(i);
	  int lhsIndex = variable2Index(static_cast<VariableTerm*>(var));

	  contextSpec[i] = lhsIndex;
	}
    }
}

void
StrategyDefinition::preprocess()
{
  PreEquation::preprocess();
  addConditionVariables(getLhs()->occursBelow());
  rhs->process();

  // Strategy expressions do not have type, so we do not check
  // if the strategy declaration type is consistent
}

void
StrategyDefinition::compile(bool compileLhs)
{
  if (isCompiled())
    return;
  setCompiled();

  TermBag availableTerms;
  compileBuild(availableTerms, false);
  compileMatch(compileLhs, false);

  // fast = hasCondition() ? DEFAULT : getNrProtectedVariables();
}

int
StrategyDefinition::traceBeginTrial(DagNode* subject, RewritingContext& context) const
{
  return context.traceBeginSdTrial(subject, this);
}

void
StrategyDefinition::print(ostream& s) const
{
  s << this;
}
