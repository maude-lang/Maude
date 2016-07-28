/*

    This file is part of the Maude 2 interpreter.

    Copyright 2014 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class SMT_RewriteSearchState.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "variable.hh"
#include "higher.hh"
#include "SMT.hh"

//      interface class definitions
#include "term.hh"
#include "subproblem.hh"
#include "lhsAutomaton.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"
#include "rule.hh"

//      variable class definitions
#include "variableSymbol.hh"
#include "variableDagNode.hh"

//      higher class definitions
#include "equalityConditionFragment.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//	SMT stuff
#include "SMT_Info.hh"
#include "SMT_EngineWrapper.hh"
#include "SMT_RewriteSearchState.hh"

SMT_RewriteSearchState::SMT_RewriteSearchState(RewritingContext* context,
					       DagNode* constraint,
					       const SMT_Info& smtInfo,
					       SMT_EngineWrapper* engine,
					       const mpz_class& avoidVariableNumber,
					       int flags)
  : context(context),
    constraint(constraint),
    smtInfo(smtInfo),
    engine(engine),
    avoidVariableNumber(avoidVariableNumber),
    flags(flags)
{
  ruleIndex = -1;
  matchingSubproblem = 0;
  newState = 0;
  newConstraint = 0;
}

SMT_RewriteSearchState::~SMT_RewriteSearchState()
{
  delete matchingSubproblem;
  if (flags & GC_ENGINE)
    delete engine;
  if (flags & GC_CONTEXT)
    delete context;
}

void
SMT_RewriteSearchState::markReachableNodes()
{
  constraint->mark();
  if (newConstraint)
    newConstraint->mark();
  if (newState)
    newState->mark();
}

bool
SMT_RewriteSearchState::findNextRewrite()
{
  if (ruleIndex > -1)
    {
      //
      //	Get rid of assertions from last solution.
      //
      engine->pop();
      //
      //	We already selected a rule and matched it in at least one way; see if
      //	it can be matched in another way.
      //
      if (nextSolution())
	return true;
      ++ruleIndex;
    }
  else
    {
      //
      //	Starting from scratch.
      //
      if (!checkAndConvertState())
	{
	  DebugAdvisory("failed to convert initial state");
	  return false;
	}
      DebugAdvisory("converted state = " << state << " constraint = " << constraint);
      ruleIndex = 0;
    }

  Symbol* topSymbol = state->symbol();
  const Vector<Rule*>& rules = topSymbol->getRules();
  for (int nrRules = rules.length(); ruleIndex < nrRules; ++ruleIndex)
    {
      currentRule = rules[ruleIndex];
      DebugAdvisory("Considering rule: " << currentRule);
      context->clear(currentRule->getNrProtectedVariables());
      LhsAutomaton* a = currentRule->getNonExtLhsAutomaton();
      if (a->match(state, *context, matchingSubproblem, 0))
	{
	  if (matchingSubproblem == 0 || matchingSubproblem->solve(true, *context))
	    {
	      if (checkConsistancy())
		{
		  if (RewritingContext::getTraceStatus())
		    {
		      context->tracePreRuleRewrite(state, currentRule);
		      if (context->traceAbort())
			return false;
		      context->tracePostRuleRewrite(newState);
		      if (context->traceAbort())
			return false;
		    }
		  return true;
		}
	      DebugAdvisory("first match failed consistancy check");
	      if (nextSolution())
		return true;
	    }
	  else
	    DebugAdvisory("solve() failed");
	}
      else
	DebugAdvisory("match() failed");
    }
  return false;
}

bool
SMT_RewriteSearchState::nextSolution()
{
  //
  //	Clear bindings of variables that were bound to fresh variables.
  //
  FOR_EACH_CONST(i, NatSet, boundToFresh)
    context->bind(*i, 0);
  boundToFresh.clear();
  //
  //	If we had a matching problem we may be able to solve it in a new way.
  //
  if (matchingSubproblem != 0)
    {
      while (matchingSubproblem->solve(false, *context))
	{
	  if (checkConsistancy())
	    return true;
	  DebugAdvisory("next match failed consistancy check");
	  //
	  //	Clear bindings of variables that were bound to fresh variables.
	  //
	  FOR_EACH_CONST(i, NatSet, boundToFresh)
	    context->bind(*i, 0);
	  boundToFresh.clear();
	}
      delete matchingSubproblem;
      matchingSubproblem = 0;
    }
  return false;
}

bool
SMT_RewriteSearchState::checkConsistancy()
{
  //
  //	First we find all unbound variables occuring in the rule and
  //	bind them to fresh variables.
  //
  int nrRealVariables = currentRule->getNrRealVariables();
  newVariableNumber = avoidVariableNumber;
  for (int i = 0; i < nrRealVariables; ++i)
    {
      DagNode* binding = context->value(i);
      if (binding == 0)
	{
	  ++newVariableNumber;
	  DagNode* newVariable = engine->makeFreshVariable(currentRule->index2Variable(i), newVariableNumber);
	  context->bind(i, newVariable);
	  DebugAdvisory("variable " << currentRule->index2Variable(i) << " is unbound and so is bound to fresh variable " << newVariable);
	  boundToFresh.insert(i);
	}
      else
	DebugAdvisory("variable " << currentRule->index2Variable(i) << " is bound to " << binding);
    }
  engine->push();
  //
  //	There is no requirement that a rule has a condition. But if it has we need to instantiate it
  //	and check it.
  //
  DagNode* condition = 0;
  if (currentRule->hasCondition())
    {
      if (!instantiateCondition(currentRule->getCondition(), condition))
	{
	  engine->pop();
	  return false;
	}
      //
      //	If condition was optimized away it will be zero.
      //
      if (condition != 0 && engine->assertDag(condition) != SMT_EngineWrapper::SAT)
	{
	  engine->pop();  // get rid of failed SMT state
	  return false;
	}
    }
  //
  //	Either there was no condition or the new constraint which it generated was satisfiable in the presence
  //	of the accumulated constraints to date. We need to generate a new state and a new constraint.
  //
  newState = currentRule->getRhsBuilder().construct(*context);
  if (condition == 0)
    {
      //
      //	No condition so new constraint is old constraint.
      //
      newConstraint = constraint;
    }
  else if (constraint->symbol() == smtInfo.getTrueSymbol())
    {
      //
      //	Old constraint is trivial so new constraint is condition.
      //
      newConstraint = condition;
    }
  else
    {
      //
      //	Have both a non-trivial constraint and a condition so conjunct them.
      //
        Vector<DagNode*> args(2);
	args[0] = constraint;
	args[1] = condition;
	newConstraint = smtInfo.getConjunctionOperator()->makeDagNode(args);
    }
  DebugAdvisory("New state = " << newState);
  DebugAdvisory("New constraint = " << newConstraint);

  newState->computeTrueSort(*context);
  newConstraint->computeTrueSort(*context);
  //
  //	No equation rewriting should happen during rewriting module SMT so we
  //	call garbage collector here if needed.
  //
  MemoryCell::okToCollectGarbage();
  return true;
}

bool
SMT_RewriteSearchState::checkAndConvertState()
{
  state = context->root();
  state->computeTrueSort(*context);
  constraint->computeTrueSort(*context);
  return engine->assertDag(constraint) == SMT_EngineWrapper::SAT;
}

bool
SMT_RewriteSearchState::instantiateCondition(const Vector<ConditionFragment*>& condition, DagNode*& instantiation)
{
  //
  //	Build a dag that representing the SMT conjunction of the instantiated condition fragments.
  //	Null dag pointer represents trivially true condition.
  //	Return true on success and false if there was a problem.
  //
  instantiation = 0;
  FOR_EACH_CONST(i, Vector<ConditionFragment*>, condition)
    {
      EqualityConditionFragment* cf = dynamic_cast<EqualityConditionFragment*>(*i);
      if (cf == 0)
	{
	  IssueWarning("non-equality condition fragment");
	  return false;
	}
      
      DagNode* lhs;
      DagNode* rhs;
      cf->buildInstances(*context, lhs, rhs);
      if (lhs->equal(rhs))
	continue;  // optimize out equal dags

      DagNode* clause;
      if (rhs->symbol() == smtInfo.getTrueSymbol())
	clause = lhs;  // optimize QF = true
      else if (lhs->symbol() == smtInfo.getTrueSymbol())
	clause = rhs;  // optimize true = QF
      else
	{
	  Symbol* eqOp = smtInfo.getEqualityOperator(lhs, rhs);
	  if (eqOp == 0)
	    {
	      IssueWarning("bad equality condition fragment");
	      return false;
	    }
	  Vector<DagNode*> args(2);
	  args[0] = lhs;
	  args[1] = rhs;
	  clause = eqOp->makeDagNode(args);
	}
      if (instantiation == 0)
	instantiation = clause;
      else
	{
	  Vector<DagNode*> args(2);
	  args[0] = instantiation;
	  args[1] = clause;
	  instantiation = smtInfo.getConjunctionOperator()->makeDagNode(args);
	}
    }
  return true;
}
