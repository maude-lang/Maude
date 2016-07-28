/*

    This file is part of the Maude 2 interpreter.

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
//	Code for metaSearch()/metaSearchPath() descent functions.
//

local_inline bool
MetaLevelOpSymbol::downSearchType(DagNode* arg, SequenceSearch::SearchType& searchType) const
{
  int qid;
  if (metaLevel->downQid(arg, qid))
    {
      if (qid == Token::encode("+"))
	searchType = SequenceSearch::AT_LEAST_ONE_STEP;
      else if (qid == Token::encode("*"))
	searchType = SequenceSearch::ANY_STEPS;
      else if (qid == Token::encode("!"))
	searchType = SequenceSearch::NORMAL_FORM;
      else
	return false;
      return true;
    }
  return false;
}

local_inline bool
MetaLevelOpSymbol::getCachedRewriteSequenceSearch(MetaModule* m,
						  FreeDagNode* subject,
						  RewritingContext& context,
						  Int64 solutionNr,
						  RewriteSequenceSearch*& search,
						  Int64& lastSolutionNr)
{
  CacheableState* cachedState;
  if (m->remove(subject, cachedState, lastSolutionNr))
    {
      DebugAdvisory("looking for solution #" << solutionNr << " and found cached solution #" << lastSolutionNr);
      if (lastSolutionNr <= solutionNr)
	{
	  search = safeCast(RewriteSequenceSearch*, cachedState);
	  //
	  //	The parent context pointer of the root context in the
	  //	RewriteSequenceSearch object may be stale, since the
	  //	object may have been cached in a different context.
	  //
	  safeCast(UserLevelRewritingContext*, search->getContext())->
	    beAdoptedBy(safeCast(UserLevelRewritingContext*, &context));
	  return true;
	}
      delete cachedState;
    }
  return false;
}

RewriteSequenceSearch*
MetaLevelOpSymbol::makeRewriteSequenceSearch(MetaModule* m,
					     FreeDagNode* subject,
					     RewritingContext& context) const
{
  RewriteSequenceSearch::SearchType searchType;
  int maxDepth;
  if (downSearchType(subject->getArgument(4), searchType) &&
      metaLevel->downBound(subject->getArgument(5), maxDepth))
    {
      Term* s;
      Term* g;
      if (metaLevel->downTermPair(subject->getArgument(1), subject->getArgument(2), s, g, m))
	{
	  Vector<ConditionFragment*> condition;
	  if (metaLevel->downCondition(subject->getArgument(3), m, condition))
	    {
	      m->protect();
	      Pattern* goal = new Pattern(g, false, condition);
	      RewritingContext* subjectContext = term2RewritingContext(s, context);
	      context.addInCount(*subjectContext);
	      return new RewriteSequenceSearch(subjectContext,
					       searchType,
					       goal,
					       maxDepth);
	    }
	  g->deepSelfDestruct();
	  s->deepSelfDestruct();
	}
    }
  return 0;
}

bool
MetaLevelOpSymbol::metaSearch(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaSearch : Module Term Term Condition Qid Bound Nat ~> ResultTriple? .
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      if (metaLevel->downSaturate64(subject->getArgument(6), solutionNr) &&
	  solutionNr >= 0)
	{
	  RewriteSequenceSearch* state;
	  Int64 lastSolutionNr;
	  if (getCachedRewriteSequenceSearch(m, subject, context, solutionNr, state, lastSolutionNr))
	    m->protect();  // Use cached state
	  else if ((state = makeRewriteSequenceSearch(m, subject, context)))
	    lastSolutionNr = -1;
	  else
	    return false;

	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextMatch();
	      state->transferCount(context);
	      Verbose("metaSearch: visited " << state->getNrStates() << " states.");
	      if (!success)
		{
		  delete state;
		  result = metaLevel->upFailureTriple();
		  goto fail;
		}
	      ++lastSolutionNr;
	    }
	  m->insert(subject, state, solutionNr);
	  {
	    result = metaLevel->upResultTriple(state->getStateDag(state->getStateNr()),
					       *(state->getSubstitution()),
					       *(state->getGoal()),
					       m);
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaSearchPath(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaSearchPath : Module Term Term Condition Qid Bound Nat ~> Trace? .
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      if (metaLevel->downSaturate64(subject->getArgument(6), solutionNr) &&
	  solutionNr >= 0)
	{
	  RewriteSequenceSearch* state;
	  Int64 lastSolutionNr;
	  if (getCachedRewriteSequenceSearch(m, subject, context, solutionNr, state, lastSolutionNr))
	    m->protect();  // Use cached state
	  else if ((state = makeRewriteSequenceSearch(m, subject, context)))
	    lastSolutionNr = -1;
	  else
	    return false;

	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextMatch();
	      state->transferCount(context);
	      Verbose("metaSearchPath: visited " << state->getNrStates() << " states.");
	      if (!success)
		{
		  delete state;
		  result = metaLevel->upFailureTrace();
		  goto fail;
		}
	      ++lastSolutionNr;
	    }
	  m->insert(subject, state, solutionNr);
	  result = metaLevel->upTrace(*state, m);
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

#include "SMT_Info.hh"
#include "variableGenerator.hh"
#include "SMT_RewriteSequenceSearch.hh"

local_inline bool
MetaLevelOpSymbol::getCachedSMT_RewriteSequenceSearch(MetaModule* m,
						   FreeDagNode* subject,
						   RewritingContext& context,
						   Int64 solutionNr,
						   SMT_RewriteSequenceSearch*& search,
						   Int64& lastSolutionNr)
{
  CacheableState* cachedState;
  if (m->remove(subject, cachedState, lastSolutionNr))
    {
      if (lastSolutionNr <= solutionNr)
	{
	  search = safeCast(SMT_RewriteSequenceSearch*, cachedState);
	  //
	  //	The parent context pointer of the root context in the
	  //	NarrowingSequenceSearch is possibly stale.
	  //
	  safeCast(UserLevelRewritingContext*, search->getRootContext())->
	    beAdoptedBy(safeCast(UserLevelRewritingContext*, &context));
	  DebugAdvisory("   !!! Found cached SMT_RewriteSequenceSearch !!!");
	  return true;
	}
      delete cachedState;
    }
  return false;
}

SMT_RewriteSequenceSearch*
MetaLevelOpSymbol::makeSMT_RewriteSequenceSearch(MetaModule* m,
						 FreeDagNode* subject,
						 RewritingContext& context) const
{
  DagNode* metaVarNumber = subject->getArgument(5);
  RewriteSequenceSearch::SearchType searchType;
  int maxDepth;
  if (metaLevel->isNat(metaVarNumber) &&
      downSearchType(subject->getArgument(4), searchType) &&
      searchType != SequenceSearch::NORMAL_FORM &&
      metaLevel->downBound(subject->getArgument(6), maxDepth))
    {
      Term* startTerm;
      Term* target;
      if (metaLevel->downTermPair(subject->getArgument(1), subject->getArgument(2), startTerm, target, m))
	{
	  Vector<ConditionFragment*> condition;
	  if (metaLevel->downCondition(subject->getArgument(3), m, condition))
	    {
	      m->protect();

	      const mpz_class& varNumber = metaLevel->getNat(metaVarNumber);
	      RewritingContext* startContext = term2RewritingContext(startTerm, context);
	      const SMT_Info& smtInfo = m->getSMT_Info();
	      VariableGenerator* vg = new VariableGenerator(smtInfo);
	      DebugAdvisory("   !!! Made cached SMT_RewriteSequenceSearch !!!");

	      return new SMT_RewriteSequenceSearch(startContext,  // pass responsibility for deletion
						   searchType,
						   target,  // pass responsibility for deletion
						   condition,  // pass responsibility for deletion
						   smtInfo,
						   vg,  // pass responsibility for deletion
						   maxDepth,
						   varNumber);
	    }
	}
    }
  return 0;
}

bool
MetaLevelOpSymbol::metaSmtSearch(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaSmtSearch : Module Term Term Condition Qid Nat Bound Nat ~> SmtResult?
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      if (m->validForSMT_Rewriting())
	{
	  Int64 solutionNr;
	  if (metaLevel->downSaturate64(subject->getArgument(7), solutionNr) &&
	      solutionNr >= 0)
	    {
	      SMT_RewriteSequenceSearch* smtState;
	      Int64 lastSolutionNr;
	      if (getCachedSMT_RewriteSequenceSearch(m, subject, context, solutionNr, smtState, lastSolutionNr))
		m->protect();  // Use cached state
	      else if ((smtState = makeSMT_RewriteSequenceSearch(m, subject, context)))
		lastSolutionNr = -1;
	      else
		return false;

	      DagNode* result;
	      while (lastSolutionNr < solutionNr)
		{
		  bool success = smtState->findNextMatch();
		  if (!success)
		    {
		      delete smtState;
		      result = metaLevel->upSmtFailure();
		      goto fail;
		    }
		  context.incrementRlCount();
		  ++lastSolutionNr;
		}
	      m->insert(subject, smtState, solutionNr);
	      {
		/*
		DagNode* state = smtState->getState(smtState->getCurrentStateNumber());
		const Substitution& substitution = *(smtState->getSubstitution());
		const VariableInfo& variableInfo = *smtState;
		const NatSet& smtVariables = smtState->getSMT_VarIndices();
		DagNode* constraint = smtState->getFinalConstraint();
		const mpz_class& variableNumber = smtState->getMaxVariableNumber();

		result = metaLevel->upSmtResult(state,
						substitution,
						variableInfo,
						smtVariables,
						constraint,
						variableNumber,
						m);
		*/

		result = metaLevel->upSmtResult(smtState->getState(smtState->getCurrentStateNumber()),
						*(smtState->getSubstitution()),
						*smtState,
						smtState->getSMT_VarIndices(),
						smtState->getFinalConstraint(),
						smtState->getMaxVariableNumber(),
						m);
	      }
	    fail:
	      (void) m->unprotect();
	      return context.builtInReplace(subject, result);
	    }
	}
    }
  return false;
}
