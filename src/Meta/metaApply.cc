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
//	Code for metaApply and metaXapply descent functions.
//

local_inline bool
MetaLevelOpSymbol::getCachedRewriteSearchState(MetaModule* m,
					       FreeDagNode* subject,
					       RewritingContext& context,
					       Int64 solutionNr,
					       RewriteSearchState*& state,
					       Int64& lastSolutionNr)
{
  if (solutionNr > 0)
    {
      SearchState* s;
      if (m->remove(subject, context, s, lastSolutionNr))
	{
	  if (lastSolutionNr < solutionNr)
	    {
	      state = safeCast(RewriteSearchState*, s);
	      return true;
	    }
	  delete s;
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaApply(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaApply: Module Term Qid Substitution Nat ~> ResultTriple? .
  //    op {_,_,_} : Term Type Substitution -> ResultTriple [ctor] .
  //
  Int64 solutionNr;
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      if (metaLevel->downSaturate64(subject->getArgument(4), solutionNr))
	{
	  RewriteSearchState* state;
	  Int64 lastSolutionNr;
	  if (getCachedRewriteSearchState(m, subject, context, solutionNr, state, lastSolutionNr))
	    m->protect();  // Use cached state
	  else
	    {
	      //
	      //	Need to create state from scratch
	      //
	      int label;
	      Vector<Term*> variables;
	      Vector<Term*> values;
	      if (metaLevel->downQid(subject->getArgument(2), label) &&
		  metaLevel->downSubstitution(subject->getArgument(3), m, variables, values))
		{
		  Vector<DagRoot*> dags;
		  m->protect();
		  if (noDuplicates(variables) &&
		      dagifySubstitution(variables, values, dags, context))
		    {
		      if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
			{
			  RewritingContext* subjectContext = term2RewritingContext(t, context);
			  subjectContext->reduce();
			  context.addInCount(*subjectContext);
			  state = new RewriteSearchState(subjectContext,
							 label,
							 RewriteSearchState::GC_CONTEXT |
							 RewriteSearchState::GC_SUBSTITUTION |
							 RewriteSearchState::ALLOW_NONEXEC);
			  if (variables.length() > 0)
			    state->setInitialSubstitution(variables, dags);
			  for (int i = values.length() - 1; i >= 0; i--)
			    values[i]->deepSelfDestruct();
			  lastSolutionNr = -1;
			  goto ok;
			}
		      for (int i = dags.length() - 1; i >= 0; i--)
			delete dags[i];
		    }
		  for (int i = variables.length() - 1; i >= 0; i--)
		    {
		      variables[i]->deepSelfDestruct();
		      values[i]->deepSelfDestruct();
		    }
		  m->unprotect();
		}
	      return false;
	    }
	ok:
	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextRewrite();
	      state->transferCount(context);
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
	    Rule* rule = state->getRule();
	    bool trace = RewritingContext::getTraceStatus();
	    if (trace)
	      {
		RewritingContext* originalContext = state->getContext();
		originalContext->tracePreRuleRewrite(state->getDagNode(), rule);
		if (originalContext->traceAbort())
		  {
		    (void) m->unprotect();
		    return false;
		  }
	      }
	    DagNode* replacement = state->getReplacement();
	    Substitution* substitution = state->getContext();
	    RewriteSearchState::DagPair top = state->rebuildDag(replacement);
	    RewritingContext* resultContext =
	      context.makeSubcontext(top.first, UserLevelRewritingContext::META_EVAL);
	    if (trace)
	      resultContext->tracePostRuleRewrite(replacement);
	    resultContext->reduce();
	    context.addInCount(*resultContext);
	    context.incrementRlCount();
	    result = metaLevel->upResultTriple(resultContext->root(),
					       *substitution,
					       *rule,
					       m);
	    delete resultContext;
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaXapply(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaXapply : Module Term Qid Substitution Nat Bound Nat ~> Result4Tuple? .
  //	op {_,_,_,_} : Term Type Substitution Context -> Result4Tuple [ctor] .
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      if (metaLevel->downSaturate64(subject->getArgument(6), solutionNr))
	{
	  RewriteSearchState* state;
	  Int64 lastSolutionNr;
	  if (getCachedRewriteSearchState(m, subject, context, solutionNr, state, lastSolutionNr))
	    m->protect();  // Use cached state
	  else
	    {
	      //
	      //	Need to create state from scratch
	      //
	      int minDepth;
	      int maxDepth;
	      int label;
	      Vector<Term*> variables;
	      Vector<Term*> values;
	      if (metaLevel->downSaturate(subject->getArgument(4), minDepth) &&
		  metaLevel->downBound(subject->getArgument(5), maxDepth) &&
		  metaLevel->downQid(subject->getArgument(2), label) &&
		  metaLevel->downSubstitution(subject->getArgument(3), m, variables, values))
		{
		  if (maxDepth == NONE)
		    maxDepth = UNBOUNDED;  // NONE means no extension for RewriteSearchState
		  Vector<DagRoot*> dags;
		  m->protect();
		  if (noDuplicates(variables) &&
		      dagifySubstitution(variables, values, dags, context))
		    {
		      if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
			{
			  RewritingContext* subjectContext = term2RewritingContext(t, context);
			  subjectContext->reduce();
			  context.addInCount(*subjectContext);
			  state = new RewriteSearchState(subjectContext,
							 label,
							 RewriteSearchState::GC_CONTEXT |
							 RewriteSearchState::GC_SUBSTITUTION |
							 RewriteSearchState::ALLOW_NONEXEC,
							 minDepth,
							 maxDepth);
			  if (variables.length() > 0)
			    state->setInitialSubstitution(variables, dags);
			  for (int i = values.length() - 1; i >= 0; i--)
			    values[i]->deepSelfDestruct();
			  lastSolutionNr = -1;
			  goto ok;
			}
		      for (int i = dags.length() - 1; i >= 0; i--)
			delete dags[i];
		    }
		  for (int i = variables.length() - 1; i >= 0; i--)
		    {
		      variables[i]->deepSelfDestruct();
		      values[i]->deepSelfDestruct();
		    }
		  m->unprotect();
		}
	      return false;
	    }
	ok:
	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextRewrite();
	      state->transferCount(context);
	      if (!success)
		{
		  delete state;
		  result = metaLevel->upFailure4Tuple();
		  goto fail;
		}
	      ++lastSolutionNr;
	    }
	  m->insert(subject, state, solutionNr);
	  {
	    Rule* rule = state->getRule();
	    bool trace = RewritingContext::getTraceStatus();
	    if (trace)
	      {
		RewritingContext* originalContext = state->getContext();
		originalContext->tracePreRuleRewrite(state->getDagNode(), rule);
		if (originalContext->traceAbort())
		  {
		    (void) m->unprotect();
		    return false;
		  }
	      }
	    DagNode* replacement = state->getReplacement()->makeClone();  // for unique ptr
	    Substitution* substitution = state->getContext();
	    RewriteSearchState::DagPair top = state->rebuildDag(replacement);
	    PointerMap qidMap;
	    PointerMap dagNodeMap;
	    DagRoot metaContext(metaLevel->upContext(top.first, m, replacement, qidMap, dagNodeMap));
	    RewritingContext* resultContext =
	      context.makeSubcontext(top.first, UserLevelRewritingContext::META_EVAL);
	    if (trace)
	      resultContext->tracePostRuleRewrite(top.second);
	    resultContext->reduce();
	    context.addInCount(*resultContext);
	    context.incrementRlCount();
	    result = metaLevel->upResult4Tuple(resultContext->root(),
					       *substitution,
					       *rule,
					       metaContext.getNode(),
					       m);
	    delete resultContext;
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}
