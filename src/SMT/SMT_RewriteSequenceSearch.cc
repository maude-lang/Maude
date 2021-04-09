/*

    This file is part of the Maude 3 interpreter.

    Copyright 2014-2021 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class SMT_RewriteSequenceSearch.
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
#include "SMT_RewriteSequenceSearch.hh"

SMT_RewriteSequenceSearch::SMT_RewriteSequenceSearch(RewritingContext* initial,
						     SearchType searchType,
						     Term* target,
						     const Vector<ConditionFragment*>& condition,
						     const SMT_Info& smtInfo,
						     SMT_EngineWrapper* engine,
						     int maxDepth,
						     const mpz_class& avoidVariableNumber)
  : Pattern(target, false),
    smtInfo(smtInfo),
    engine(engine),
    maxDepth((searchType == ONE_STEP) ? 1 : maxDepth),
    states(1)
{
  //
  //	Start with clean engine.
  //
  engine->clearAssertions();
  //
  //	Ensure that initial state has sort info.
  //
  initial->root()->computeTrueSort(*initial);
  //
  //	Make initial state object.
  //
  State* s = new State();
  s->avoidVariableNumber = avoidVariableNumber;
  s->context = initial;
  s->constraint = makeConstraintFromCondition(target, condition);
  s->search = 0;
  s->rule = 0;
  s->parent = NONE;
  s->depth = 0;
  //
  //	Misc initialization.
  //
  states[0] = s;
  stateToExplore = -1 /*0*/;
  matchState = 0;
  finalConstraint = 0;
  findSMT_Variables();
  needToTryInitialState = (searchType == ANY_STEPS);
  //
  //	Delete condition now since we are done with it.
  //
  for (ConditionFragment* cf : condition)
    delete cf;
}

SMT_RewriteSequenceSearch::~SMT_RewriteSequenceSearch()
{
  //
  //	FIXME: Need to deal with contexts in some way
  //
  for (State* s : states)
    {
      delete s->search;
      delete s->context;
      delete s;
    }
  delete matchState;
  delete engine;
}

void
SMT_RewriteSequenceSearch::markReachableNodes()
{
  //
  //	Protect dagnode versions of any SMT variables in the pattern.
  //
  for (auto& i : smtVarDags)
    i.second->mark();
  //
  //	Constraints aren't otherwise protected once the search object
  //	they were passed to is deleted.
  //
  for (State* s : states)
    s->constraint->mark();
  //
  //	Need to protect any final constraint we made.
  //
  if (finalConstraint != 0)
    finalConstraint->mark();
}

DagNode*
SMT_RewriteSequenceSearch::makeConstraintFromCondition(Term* target,
						       const Vector<ConditionFragment*>& condition)
{
  Vector<DagNode*> args(2);
  DagNode* constraint = 0;

  for (ConditionFragment* cf : condition)
    {
      //
      //	Check to see that condition fragment is of the form t1 = t2.
      //
      EqualityConditionFragment* fragment = dynamic_cast<EqualityConditionFragment*>(cf);
      if (fragment == 0)
	{
	  IssueWarning(*target << ": condition fragment " << cf <<
		       " not supported for searching modulo SMT.");
	  continue;
	}
      //
      //	Dagify and optimize out equal case.
      //
      fragment->normalize(false);
      DagNode* lhs = fragment->getLhs()->term2Dag();
      DagNode* rhs = fragment->getRhs()->term2Dag();
      if (lhs->equal(rhs))
	continue;
      //
      //	Generate an SMT clause.
      //
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
	      IssueWarning(*(fragment->getLhs()) <<
			   ": no SMT equality operator available for condition fragment " << cf);
	      continue;
	    }
	  args[0] = lhs;
	  args[1] = rhs;
	  clause = eqOp->makeDagNode(args);
	}
      //
      //	Conjunct with existing constraint.
      //
      if (constraint == 0)
	constraint = clause;
      else
	{
	  args[0] = constraint;
	  args[1] = clause;
	  constraint = smtInfo.getConjunctionOperator()->makeDagNode(args);
	}
    }
  //
  //	Default to true.
  //
  return constraint == 0 ? smtInfo.getTrueSymbol()->makeDagNode() : constraint;
}

void
SMT_RewriteSequenceSearch::findSMT_Variables()
{
  //
  //	Find any SMT variables in the pattern, make dagnode versions and record their indices.
  //
  int nrVariables = getNrRealVariables();
  for (int i = 0; i < nrVariables; ++i)
    {
      VariableTerm* v = safeCast(VariableTerm*, index2Variable(i));
      VariableSymbol* vs = safeCast(VariableSymbol*, v->symbol());
      SMT_Info::SMT_Type type = smtInfo.getType(vs->getSort());
      if (type != SMT_Info::NOT_SMT)
	{
	  smtVarIndices.insert(i);
	  smtVarDags[i] = v->dagify2();
	}
    }
  DebugAdvisory("found " << smtVarDags.size() << " SMT variables");
}

bool
SMT_RewriteSequenceSearch::findNextMatch()
{
  if (matchState != 0)
    goto tryMatch;
  //
  //	Need to examine try matching a new state.
  //
  for(;;)
    {
      {
	newStateNr = findNextState();
	if (newStateNr == NONE)
	  break;
	RewritingContext* context = states[newStateNr]->context;
	matchState = new MatchSearchState(context->makeSubcontext(context->root()),
					  this,
					  MatchSearchState::GC_CONTEXT);
      }
    tryMatch:
      while (matchState->findNextMatch())
	{
	  if (checkMatchConstraint())
	    return true;
	}
      //
      //	All remaining matches failed constraint check.
      //
      delete matchState;
    }
  matchState = 0;
  return false;
}

bool
SMT_RewriteSequenceSearch::checkMatchConstraint()
{
  //
  //	We have a matching substitution, but some of the bound variables may be SMT
  //	in which case they may be mentioned in the existing condition and we
  //	need to check that equality implied by the binding is satisfiable.
  //
  Vector<DagNode*> args(2);
  const Substitution* substitution = matchState->getContext();
  DagNode* matchConstraint = 0;

  for (auto& i : smtVarDags)
    {
      DagNode* lhs = i.second;
      DagNode* rhs = substitution->value(i.first); 
      //
      //	Make equality constraint.
      //
      Vector<DagNode*> args(2);
      args[0] = lhs;
      args[1] = rhs;
      DagNode* equalityConstraint = smtInfo.getEqualityOperator(lhs, rhs)->makeDagNode(args);
      //
      //	Conjunct it in if needed.
      //
      if (matchConstraint == 0)
	matchConstraint = equalityConstraint;
      else
	{
	  args[0] = matchConstraint;
	  args[1] = equalityConstraint;
	  matchConstraint = smtInfo.getConjunctionOperator()->makeDagNode(args);
	}
      DebugAdvisory("matchConstraint: " << matchConstraint);
    }

  finalConstraint = states[newStateNr]->constraint;  // accumulated constraint in current state
  if (matchConstraint != 0)
    {
      //
      //	We assume that the accumulated constraints associated with the current state
      //	are already asserted in engine.
      //
      if (engine->checkDag(matchConstraint) != SMT_EngineWrapper::SAT) 
	return false;
      args[0] = finalConstraint;
      args[1] = matchConstraint;
      finalConstraint = smtInfo.getConjunctionOperator()->makeDagNode(args);
    }
  return true;
}

int
SMT_RewriteSequenceSearch::findNextState()
{
  if (needToTryInitialState)
    {
      //
      //	Special case: return the initial state.
      //
      needToTryInitialState = false;  // don't do this again
      if (engine->assertDag(states[0]->constraint) == SMT_EngineWrapper::SAT)
	return 0;
    }
  //
  //	First we look for a new rewrite from the state we are already exploring.
  //
  if (stateToExplore >= 0)
    {
      State* s = states[stateToExplore];
      if (s->search != 0 && s->search->findNextRewrite())
	{
	  //
	  //	Found a rewrite. Now create a new state based on the pair.
	  //
	  getContext()->incrementRlCount();
	  return makeNewState();
	}
      //
      //	Search object exhausted so clean up.
      //
      delete s->search;
      s->search = 0;
    }
  //
  //	Now we need to start expanding the next unexplored state (the initial
  //	state if stateToExplore == -1).
  //
  int nrStates = states.size();
  for (++stateToExplore; stateToExplore < nrStates; ++stateToExplore)
    {
      State* n = states[stateToExplore];
      if (n->depth == maxDepth)
	break;  // all larger number states should be at least this depth so give up
      engine->clearAssertions();  // start clean
      n->search = new SMT_RewriteSearchState(n->context,
					     n->constraint,
					     smtInfo,
					     engine,
					     n->avoidVariableNumber);
      DebugAdvisory("---> Made SMT_RewriteSearchState to explore state " << stateToExplore);
      if (n->search->findNextRewrite())
	{
	  //
	  //	Found a rewrite. Now create a new state based on the pair.
	  //
	  getContext()->incrementRlCount();
	  return makeNewState();
	}
      //
      //	No rewrites available from this state so clean up.
      //
      delete n->search;
      n->search = 0;
    }

  return NONE;
}

int
SMT_RewriteSequenceSearch::makeNewState()
{
  State* parent = states[stateToExplore];
  SMT_RewriteSearchState *pr = parent->search;

  int nrStates = states.size();
  State* n = new State();

  n->context = parent->context->makeSubcontext(pr->getNewState());
  n->constraint = pr->getNewConstraint();
  n->avoidVariableNumber = pr->getMaxVariableNumber();
  DebugAdvisory("assigned " << pr->getMaxVariableNumber() << " to state " << nrStates);
  n->parent = stateToExplore;
  n->depth = parent->depth + 1;
  n->search = 0;
  n->rule = parent->search->getRule();
  states.append(n);
  DebugAdvisory("checking value " << states[nrStates]->avoidVariableNumber);
  return nrStates;
}
