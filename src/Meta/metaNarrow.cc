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
//	Code for metaNarrow descent function.
//

local_inline bool
MetaLevelOpSymbol::getCachedNarrowingSequenceSearch(MetaModule* m,
						    FreeDagNode* subject,
						    RewritingContext& context,
						    Int64 solutionNr,
						    NarrowingSequenceSearch*& search,
						    Int64& lastSolutionNr)
{
  if (solutionNr > 0)
    {
      CacheableState* cachedState;
      if (m->remove(subject, cachedState, lastSolutionNr))
	{
	  if (lastSolutionNr <= solutionNr)
	    {
	      search = safeCast(NarrowingSequenceSearch*, cachedState);
	      //
	      //	The parent context pointer of the root context in the
	      //	NarrowingSequenceSearch is possibly stale.
	      //
	      safeCast(UserLevelRewritingContext*, search->getContext())->
		beAdoptedBy(safeCast(UserLevelRewritingContext*, &context));
	      return true;
	    }
	  delete cachedState;
	}
    }
  return false;
}

NarrowingSequenceSearch*
MetaLevelOpSymbol::makeNarrowingSequenceSearch(MetaModule* m,
					       FreeDagNode* subject,
					       RewritingContext& context) const
{
  RewriteSequenceSearch::SearchType searchType;
  int maxDepth;
  if (downSearchType(subject->getArgument(3), searchType) &&
      metaLevel->downBound(subject->getArgument(4), maxDepth))
    {
      Term* s;
      Term* g;
      if (metaLevel->downTermPair(subject->getArgument(1), subject->getArgument(2), s, g, m))
	{
	  m->protect();
	  Pattern* goal = new Pattern(g, false);
	  RewritingContext* subjectContext = term2RewritingContext(s, context);
	  context.addInCount(*subjectContext);
	  return new NarrowingSequenceSearch(subjectContext,
					     searchType,
					     goal,
					     maxDepth,
					     NarrowingSearchState::ALLOW_NONEXEC,
					     new FreshVariableSource(m, 0));
	  // not needed unless we support conditions
	  //g->deepSelfDestruct();
	  //s->deepSelfDestruct();
	}
    }
  return 0;
}

bool
MetaLevelOpSymbol::metaNarrow(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaNarrow : Module Term Term Qid Bound Nat ~> ResultTriple? .
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      if (metaLevel->downSaturate64(subject->getArgument(5), solutionNr) &&
	  solutionNr >= 0)
	{
	  NarrowingSequenceSearch* state;
	  Int64 lastSolutionNr;
	  if (getCachedNarrowingSequenceSearch(m, subject, context, solutionNr, state, lastSolutionNr))
	    m->protect();  // Use cached state
	  else if ((state = makeNarrowingSequenceSearch(m, subject, context)))
	    lastSolutionNr = -1;
	  else
	    return false;

	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextMatch();
	      //state->transferCount(context);
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
	    result = metaLevel->upResultTriple(state->getStateDag(),
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

NarrowingSequenceSearch*
MetaLevelOpSymbol::makeNarrowingSequenceSearch2(MetaModule* m,
						FreeDagNode* subject,
						RewritingContext& context) const
{
  RewriteSequenceSearch::SearchType searchType;
  int maxDepth;
  bool singlePosition;
  if (downSearchType(subject->getArgument(2), searchType) &&
      metaLevel->downBound(subject->getArgument(3), maxDepth) &&
      metaLevel->downBool(subject->getArgument(4), singlePosition))
    {
      if (Term* s = metaLevel->downTerm(subject->getArgument(1), m))
	{
	  m->protect();
	  RewritingContext* subjectContext = term2RewritingContext(s, context);
	  context.addInCount(*subjectContext);
	  return new NarrowingSequenceSearch(subjectContext,
					     searchType,
					     0,
					     maxDepth,
					     (singlePosition ?
					     NarrowingSearchState::ALLOW_NONEXEC | NarrowingSearchState::SINGLE_POSITION :
					     NarrowingSearchState::ALLOW_NONEXEC),
					     new FreshVariableSource(m, 0));
	}
    }
  return 0;
}

bool
MetaLevelOpSymbol::metaNarrow2(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaNarrow : Module Term Qid Bound Bool Nat ~> ResultPair? .
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      if (metaLevel->downSaturate64(subject->getArgument(5), solutionNr) &&
	  solutionNr >= 0)
	{
	  NarrowingSequenceSearch* state;
	  Int64 lastSolutionNr;
	  if (getCachedNarrowingSequenceSearch(m, subject, context, solutionNr, state, lastSolutionNr))
	    m->protect();  // Use cached state
	  else if ((state = makeNarrowingSequenceSearch2(m, subject, context)))
	    lastSolutionNr = -1;
	  else
	    return false;

	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextMatch();
	      //state->transferCount(context);
	      if (!success)
		{
		  delete state;
		  result = metaLevel->upFailurePair();
		  goto fail;
		}
	      ++lastSolutionNr;
	    }
	  m->insert(subject, state, solutionNr);
	  //
	  //	We up the dag using variable mapping so each result gets previously unused variables.
	  //
	  metaLevel->startVariableMapping(state->getVariableTotalForPreviouslyReturnedStates() - m->getMinimumSubstitutionSize(),
					  state->getFreshVariableGenerator());
	  result = metaLevel->upResultPair(state->getStateDag(), m);
	  metaLevel->stopVariableMapping();

	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}
