/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for metaSearch()/metaSearchPath()/metaSmtSearch() descent functions.
//

RewriteSequenceSearch*
MetaLevelOpSymbol::makeRewriteSequenceSearch(MetaModule* m,
					     FreeDagNode* subject,
					     RewritingContext& context) const
{
  RewriteSequenceSearch::SearchType searchType;
  int maxDepth;
  if (metaLevel->downSearchType(subject->getArgument(4), searchType) &&
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
	  if (m->getCachedStateObject(subject, context, solutionNr, state, lastSolutionNr))
	    m->protect();  // Use cached state
	  else if ((state = makeRewriteSequenceSearch(m, subject, context)))
	    lastSolutionNr = -1;
	  else
	    return false;

	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextMatch();
	      state->transferCountTo(context);
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
	  if (m->getCachedStateObject(subject, context, solutionNr, state, lastSolutionNr))
	    m->protect();  // Use cached state
	  else if ((state = makeRewriteSequenceSearch(m, subject, context)))
	    lastSolutionNr = -1;
	  else
	    return false;

	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextMatch();
	      state->transferCountTo(context);
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

SMT_RewriteSequenceSearch*
MetaLevelOpSymbol::makeSMT_RewriteSequenceSearch(MetaModule* m,
						 FreeDagNode* subject,
						 RewritingContext& context) const
{
  DagNode* metaVarNumber = subject->getArgument(5);
  RewriteSequenceSearch::SearchType searchType;
  int maxDepth;
  if (metaLevel->isNat(metaVarNumber) &&
      metaLevel->downSearchType(subject->getArgument(4), searchType) &&
      searchType != SequenceSearch::NORMAL_FORM &&
      metaLevel->downBound(subject->getArgument(6), maxDepth))
    {
      Term* startTerm;
      Term* target;
      if (metaLevel->downTermPair(subject->getArgument(1), subject->getArgument(2), startTerm, target, m))
	{
	  if (m->findSMT_Symbol(target) == 0)  // target shouldn't have SMT operators
	    {
	      VariableInfo variableInfo;
	      if (MixfixModule::findNonlinearVariable(target, variableInfo) == 0)  // target shouldn't have nonlinear variables
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
	      if (m->getCachedStateObject(subject, context, solutionNr, smtState, lastSolutionNr))
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
	      result = metaLevel->upSmtResult(smtState->getState(smtState->getCurrentStateNumber()),
					      *(smtState->getSubstitution()),
					      *smtState,
					      smtState->getSMT_VarIndices(),
					      smtState->getFinalConstraint(),
					      smtState->getMaxVariableNumber(),
					      m);
	    fail:
	      (void) m->unprotect();
	      return context.builtInReplace(subject, result);
	    }
	}
    }
  return false;
}
