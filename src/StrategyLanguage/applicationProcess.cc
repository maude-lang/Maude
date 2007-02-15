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
//      Implementation for abstract class Strategy.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "strategyLanguage.hh"

//	interface class definitions
#include "term.hh"
#include "subproblem.hh"

//	core class definitions
#include "cachedDag.hh"
#include "rule.hh"
#include "rewritingContext.hh"

//	higher class definitions
#include "assignmentConditionFragment.hh"
#include "rewriteConditionFragment.hh"

//	strategy language class definitions
#include "strategicSearch.hh"
#include "strategyExpression.hh"
#include "applicationStrategy.hh"
#include "decompositionProcess.hh"
#include "matchProcess.hh"
#include "rewriteTask.hh"
#include "applicationProcess.hh"

ApplicationProcess::ApplicationProcess(StrategicSearch& searchObject,
				       DagNode* start,
				       ApplicationStrategy* strategy,
				       StrategyStackManager::StackId pending,
				       StrategicExecution* taskSibling,
				       StrategicProcess* insertionPoint)


  : StrategicProcess(taskSibling, insertionPoint),
    rewriteState(searchObject.getContext(), start, strategy->getLabel(), strategy->getTop() ? NONE : UNBOUNDED),
    pending(pending),
    strategy(strategy)
{
  Vector<CachedDag>& values = strategy->getValues();
  int nrValues = values.size();
  if (nrValues > 0)
    {
      Vector<Term*> tmpVariables(strategy->getVariables());
      Vector<DagRoot*> tmpValues(nrValues);
      for (int i = 0; i < nrValues; ++i)
	tmpValues[i] = values[i].getDagRoot();
      rewriteState->setInitialSubstitution(tmpVariables, tmpValues);
    }
}

StrategicExecution::Survival
ApplicationProcess::run(StrategicSearch& searchObject)
{
  if (rewriteState->findNextRewrite())
    {
      Rule* rule = rewriteState->getRule();
      if (rule->hasCondition())
	{
	  //
	  //	Need to check that we have the correct number of substrategies for our rule.
	  //
	  int nrStrategies = strategy->getStrategies().size();
	  int nrRewriteFragments = 0;
	  const Vector<ConditionFragment*>& condition = rule->getCondition();
	  FOR_EACH_CONST(i, Vector<ConditionFragment*>, condition)
	    {
	      if (dynamic_cast<RewriteConditionFragment*>(*i))
		++nrRewriteFragments;
	    }
	  if (nrStrategies != nrRewriteFragments)
	    return SURVIVE;  // might match a different rule on later runs
	  //
	  //	We need to check the condition in a fair way, given
	  //	that rewrite conditions may have to follow a given strategy
	  //	may not terminate. Also processing the condition may
	  //	bind substitution entries needed to build an instance of
	  //	the rhs of the rule.
	  //
	  if (resolveRemainingConditionFragments(searchObject,
						 rewriteState,
						 rewriteState->getPositionIndex(),
						 rewriteState->getExtensionInfo(),
						 rewriteState->getContext(),
						 rule,
						 0,
						 strategy->getStrategies(),
						 0,
						 pending,
						 this,
						 this) == SURVIVE)
	    return SURVIVE;
	}
      else
	{
	  if (strategy->getStrategies().size() > 0)
	    return SURVIVE;  // might match a different rule on later runs
	  if (DagNode* r = doRewrite(searchObject,
				     rewriteState,
				     rewriteState->getPositionIndex(),
				     rewriteState->getExtensionInfo(),
				     rewriteState->getContext(),
				     rule))
	    {
	      (void) new DecompositionProcess(r, pending, this, this);
	      return SURVIVE;  // stick around to look for another rewrite
	    }
	}
    }
  finished(this);  // need to unlink ourself from slave list before we request deletion
  return DIE;  // request deletion
}

DagNode*
ApplicationProcess::doRewrite(StrategicSearch& searchObject,
			      SharedRewriteSearchState::Ptr rewriteState,
			      PositionState::PositionIndex redexIndex,
			      ExtensionInfo* extensionInfo,
			      Substitution* substitution,
			      Rule* rule)
{
  //
  //	The rule for using substitution is tricky:
  //	It does not belong to us and we should never bind variables that
  //	occur in the rule pattern or any fragment pattern in case we
  //	disrupt matching in other branches of the search. But it is fine
  //	to bind constructed entries, protected or not, since they will
  //	simply be overwritten in other branches.
  //
  RewritingContext* baseContext = rewriteState->getContext();
  bool trace = RewritingContext::getTraceStatus();
  if (trace)
    {
      //
      //	We have a problem: the whole term that is being rewritten is in the
      //	rewriting context rewriteState->getContext() while the substitution we
      //	will use is in context which may be a different rewriting context with
      //	a different whole term.
      //
      //	We resolve the issue by creating a special context containing the correct
      //	whole term and substitution just for tracing.
      //
      RewritingContext* tracingContext = baseContext->makeSubcontext(baseContext->root());
      tracingContext->clone(*substitution);
      tracingContext->tracePreRuleRewrite(rewriteState->getDagNode(redexIndex), rule);
      delete tracingContext;
      if (baseContext->traceAbort())
	return 0;
    }
  //
  //	Instantiate the rhs of the rule with the substitution.
  //
  //cout << "ApplicationProcess::doRewrite() nrFragileBindings = " << substitution->nrFragileBindings() << endl;
  DagNode* replacement = rule->getRhsBuilder().construct(*substitution);
  //
  //	Rebuild the original term, using the replacement in place of the redex,
  //	also accounting for any extension info; count this a rule rewrite
  //
  RewriteSearchState::DagPair r = rewriteState->rebuildDag(replacement, extensionInfo, redexIndex);
  searchObject.getContext()->incrementRlCount();
  //
  //	Make a new subcontext to equationally reduce the new term.
  //
  RewritingContext* c = baseContext->makeSubcontext(r.first);
  if (trace)
    {
      c->tracePostRuleRewrite(r.second);
      if (c->traceAbort())
	{
	  delete c;
	  return 0;
	}
    }
  c->reduce();
  if (c->traceAbort())
    {
      delete c;
      return 0;
    }
  searchObject.getContext()->addInCount(*c);
  delete c;
  return searchObject.index2DagNode(searchObject.insert(r.first));
}

StrategicExecution::Survival
ApplicationProcess::resolveRemainingConditionFragments(StrategicSearch& searchObject,
						       SharedRewriteSearchState::Ptr rewriteState,
						       PositionState::PositionIndex redexIndex,
						       ExtensionInfo* extensionInfo,
						       Substitution* substitutionSoFar,
						       Rule* rule,
						       int fragmentNr,
						       const Vector<StrategyExpression*>& strategies,
						       int strategyNr,
						       StrategyStackManager::StackId pending,
						       StrategicExecution* taskSibling,
						       StrategicProcess* other)
{
  const Vector<ConditionFragment*>& fragments = rule->getCondition();
  for (int nrFragments = fragments.size(); fragmentNr < nrFragments; ++fragmentNr)
    {
      ConditionFragment* fragment = fragments[fragmentNr];
      if (RewriteConditionFragment* rf = dynamic_cast<RewriteConditionFragment*>(fragment))
	{
	  (void) new RewriteTask(searchObject,
				 rewriteState,
				 redexIndex,
				 extensionInfo,
				 substitutionSoFar,
				 rule,
				 fragmentNr,
				 strategies,
				 strategyNr,
				 pending,
				 taskSibling,
				 other);
	  return SURVIVE;
	}
      else if (AssignmentConditionFragment* af = dynamic_cast<AssignmentConditionFragment*>(fragment))
	{
	  //
	  //	Find the L := R fragment that we are going to test.
	  //
	  AssignmentConditionFragment* acf = safeCast(AssignmentConditionFragment*, (rule->getCondition())[fragmentNr]);
	  //
	  //	Make a subcontext, construct and evalutate the instance of R.
	  //
	  RewritingContext* newContext = rewriteState->getContext()->
	    makeSubcontext(acf->makeRhsInstance(*substitutionSoFar), RewritingContext::CONDITION_EVAL);
	  newContext->reduce();
	  //
	  //	We transfer, rather than simply add in the rewrite count because MatchProcess may do
	  //	some more rewriting with newContext.
	  //
	  searchObject.getContext()->transferCount(*newContext);
	  newContext->clone(*substitutionSoFar);
	  Subproblem* subproblem;
	  if (acf->matchRoot(*newContext, subproblem))
	    {
	      (void) new MatchProcess(rewriteState,
				      redexIndex,
				      extensionInfo,
				      newContext,  // MatchProcess takes over ownership of newContext
				      subproblem,  // MatchProcess takes over ownership of subproblem
				      rule,
				      fragmentNr,
				      strategies,
				      strategyNr,
				      pending,
				      taskSibling,
				      other);
	    }
	  else
	    {
	      delete subproblem;
	      delete newContext;
	    }
	  return SURVIVE;
	}
      //
      //	Since we don't recognize the fragment as needing special treatment, assume it
      //	is branch free and solve it with a specially created context (to have the correct
      //	root and substitution) and dummy state stack.
      //
      RewritingContext* baseContext = rewriteState->getContext();
      RewritingContext* solveContext = baseContext->makeSubcontext(baseContext->root());
      solveContext->clone(*substitutionSoFar);
      stack<ConditionState*> dummy;
      bool success = fragment->solve(true, *solveContext, dummy);
      searchObject.getContext()->addInCount(*solveContext);
      if (!success)
	{
	  //
	  //	A fragment failed so this branch of the search is pruned. But the process
	  //	or task that called us should survive to generate other possibilities.
	  //
	  delete solveContext;
	  return SURVIVE;
	}
      //
      //	solve() may have bound some protected slots in solveContext as part of
      //	making instantiations; for example eager copies of variables. These
      //	must be copied back into substitutionSoFar.
      //
      substitutionSoFar->clone(*solveContext);
      delete solveContext;
    }
  //
  //	The condition succeeded so now we need to do the rewrite and resume the strategy.
  //
  if (DagNode* r = doRewrite(searchObject, rewriteState, redexIndex, extensionInfo, substitutionSoFar, rule))
    {
      (void) new DecompositionProcess(r, pending, taskSibling, other);
      return SURVIVE;  // stick around to look for another rewrite
    }
  return DIE;  // only happens when we are aborting
}
