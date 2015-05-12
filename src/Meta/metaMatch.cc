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
//	Code for metaMatch() and metaXmatch() descent functions.
//

local_inline bool
MetaLevelOpSymbol::getCachedMatchSearchState(MetaModule* m,
					     FreeDagNode* subject,
					     RewritingContext& context,
					     Int64 solutionNr,
					     MatchSearchState*& state,
					     Int64& lastSolutionNr)
{
  if (solutionNr > 0)
    {
      CacheableState* cachedState;
      if (m->remove(subject, cachedState, lastSolutionNr))
	{
	  if (lastSolutionNr <= solutionNr)
	    {
	      state = safeCast(MatchSearchState*, cachedState);
	      //
	      //	The parent context pointer of the root context in the
	      //	NarrowingSequenceSearch is possibly stale.
	      //
	      safeCast(UserLevelRewritingContext*, state->getContext())->
		beAdoptedBy(safeCast(UserLevelRewritingContext*, &context));
	      return true;
	    }
	  delete cachedState;
	}
    }
  return false;
}

local_inline MatchSearchState* 
MetaLevelOpSymbol::makeMatchSearchState(MetaModule* m,
					FreeDagNode* subject,
					RewritingContext& context) const
{
  Term* p;
  Term* s;
  if (metaLevel->downTermPair(subject->getArgument(1), subject->getArgument(2), p, s, m))
    {
      Vector<ConditionFragment*> condition;
      if (metaLevel->downCondition(subject->getArgument(3), m, condition))
	{
	  DebugAdvisory("makeMatchSearchState() pattern =\n" << p << "\nsubject =\n" << s);
	  m->protect();
	  Pattern* pattern = new Pattern(p, false, condition);
	  RewritingContext* subjectContext = term2RewritingContext(s, context);
	  subjectContext->root()->computeTrueSort(*subjectContext);
	  context.addInCount(*subjectContext);
	  return new MatchSearchState(subjectContext,
				      pattern,
				      MatchSearchState::GC_PATTERN |
				      MatchSearchState::GC_CONTEXT |
				      MatchSearchState::GC_SUBSTITUTION);
	}
      s->deepSelfDestruct();
      p->deepSelfDestruct();
    }
  return 0;
}

bool
MetaLevelOpSymbol::metaMatch(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaMatch : Module Term Term Condition Nat ~> Substitution? .
  //
  Int64 solutionNr;
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      if (metaLevel->downSaturate64(subject->getArgument(4), solutionNr))
	{
	  MatchSearchState* state;
	  Int64 lastSolutionNr;
	  if (getCachedMatchSearchState(m, subject, context, solutionNr, state, lastSolutionNr))
	    m->protect();  // Use cached state
	  else if ((state = makeMatchSearchState(m, subject, context)))
	    lastSolutionNr = -1;
	  else
	    return false;

	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextMatch();
	      state->transferCount(context);
	      if (!success)
		{
		  delete state;
		  result = metaLevel->upNoMatchSubst();
		  goto fail;
		}
	      ++lastSolutionNr;
	    }
	  m->insert(subject, state, solutionNr);
	  {
	    VariableInfo* variableInfo = state->getPattern();
	    Substitution* substitution = state->getContext();
	    PointerMap qidMap;
	    PointerMap dagNodeMap;
	    result = metaLevel->upSubstitution(*substitution,
					       *variableInfo,
					       m,
					       qidMap,
					       dagNodeMap);
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

local_inline MatchSearchState*
MetaLevelOpSymbol::makeMatchSearchState2(MetaModule* m,
                                         FreeDagNode* subject,
                                         RewritingContext& context) const
{
  int minDepth;
  int maxDepth;
  if (metaLevel->downSaturate(subject->getArgument(4), minDepth) &&
      metaLevel->downBound(subject->getArgument(5), maxDepth))
    {
      if (Term* p = metaLevel->downTerm(subject->getArgument(1), m))
        {
          if (Term* s = metaLevel->downTerm(subject->getArgument(2), m))
            {
	      Vector<ConditionFragment*> condition;
	      if (metaLevel->downCondition(subject->getArgument(3), m, condition))
		{
		  if (maxDepth == NONE)
		    maxDepth = UNBOUNDED;  // NONE means no extension for MatchSearchState
		  m->protect();
		  Pattern* pattern = new Pattern(p, true, condition);
		  RewritingContext* subjectContext = term2RewritingContext(s, context);
		  subjectContext->root()->computeTrueSort(*subjectContext);
		  context.addInCount(*subjectContext);
		  return new MatchSearchState(subjectContext,
					      pattern,
					      MatchSearchState::GC_PATTERN |
					      MatchSearchState::GC_CONTEXT |
					      MatchSearchState::GC_SUBSTITUTION,
					      minDepth,
					      maxDepth);
		}
	    }
	}
    }
  return 0;
}

bool
MetaLevelOpSymbol::metaXmatch(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaXmatch : Module Term Term Condition Nat Bound Nat ~> MatchPair? .
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      if (metaLevel->downSaturate64(subject->getArgument(6), solutionNr) &&
	  solutionNr >= 0)
	{
	  MatchSearchState* state;
	  Int64 lastSolutionNr;
	  if (getCachedMatchSearchState(m, subject, context, solutionNr, state, lastSolutionNr))
	    m->protect();  // Use cached state
	  else if ((state = makeMatchSearchState2(m, subject, context)))
	    lastSolutionNr = -1;
	  else
	    return false;

	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextMatch();
	      state->transferCount(context);
	      if (!success)
		{
		  delete state;
		  result = metaLevel->upNoMatchPair();
		  goto fail;
		}
	      ++lastSolutionNr;
	    }
	  m->insert(subject, state, solutionNr);
	  {
	    Pattern* pattern = state->getPattern();
	    Substitution* substitution = state->getContext();
	    Sort* sort = pattern->getLhs()->getSort();  // HACK
	    VariableSymbol* vs = safeCast(VariableSymbol*, m->instantiateVariable(sort));
	    DagNode* hole = new VariableDagNode(vs, 0, UNDEFINED);
	    RewriteSearchState::DagPair top = state->rebuildDag(hole);
	    result = metaLevel->upMatchPair(*substitution,
					    *pattern,
					    top.first,
					    hole,
					    m);
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}
