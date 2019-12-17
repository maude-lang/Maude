/*

    This file is part of the Maude 3 interpreter.

    Copyright 2018 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for both applyRule() messages.
//

RewriteSearchState*
InterpreterManagerSymbol::makeRewriteSearchState(ImportModule* m,
						 FreeDagNode* message,
						 RewritingContext& context,
						 bool atTop) const
{
  //
  //	Get depth bounds if we are in the general case
  //
  int minDepth = 0;
  int maxDepth = NONE;  // no extension
  if (!atTop)
    {
      if (metaLevel->downSaturate(message->getArgument(6), minDepth) &&
	  metaLevel->downBound(message->getArgument(7), maxDepth))
	{
	  if (maxDepth == NONE)
	    maxDepth = UNBOUNDED;  // NONE means no extension for RewriteSearchState
	}
      else
	return 0;
    }

  int label;
  Vector<Term*> variables;
  Vector<Term*> values;
  if (metaLevel->downQid(message->getArgument(4), label) &&
      metaLevel->downSubstitution(message->getArgument(5), m, variables, values))
    {
      Vector<DagRoot*> dags;
      m->protect();  // because dagifySubstitution() could enter the debugger
      if (MetaLevel::dagifySubstitution(variables, values, dags, context))
	{
	  if (Term* t = metaLevel->downTerm(message->getArgument(3), m))
	    {
	      RewritingContext* subjectContext = term2RewritingContext(t, context);
	      subjectContext->reduce();
	      RewriteSearchState* state = new RewriteSearchState(subjectContext,
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
	      return state;
	    }
	  //
	  //	Clean up dag roots before failing.
	  //
	  for (int i = dags.length() - 1; i >= 0; i--)
	    delete dags[i];
	}
      //
      //	Clean up substitution before failing.
      //
      for (int i = variables.length() - 1; i >= 0; i--)
	{
	  variables[i]->deepSelfDestruct();
	  values[i]->deepSelfDestruct();
	}
      (void) m->unprotect();
    }
  return 0;
}

bool
InterpreterManagerSymbol::applyRule(FreeDagNode* message, ObjectSystemRewritingContext& context, bool atTop)
{
  //
  //	op applyRule : Oid Oid Qid Term Qid Substition Nat -> Msg .
  //                    0   1   2   3    4      5       6
  //
  //	op applyRule : Oid Oid Qid Term Qid Substition Nat Bound Nat -> Msg .
  //                    0   1   2   3    4      5       6    7    8
  //
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      Int64 solutionNr;
      if (metaLevel->downSaturate64(message->getArgument(atTop ? 6 : 8), solutionNr) &&
	  solutionNr >= 0)
	{
	  int id;
	  if (metaLevel->downQid(message->getArgument(2), id))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  if (ImportModule* m = pm->getFlatModule())
		    {
		      RewriteSearchState* state;
		      Int64 lastSolutionNr;
		      MetaModule* mm = safeCast(MetaModule*, m);  // metaInterpreter can only store MetaModules	      
		      if (mm->getCachedStateObject(message, solutionNr, state, lastSolutionNr))
			m->protect();  // Use cached state
		      else if ((state = makeRewriteSearchState(m, message, context, atTop)))
			lastSolutionNr = -1;
		      else
			return false;

		      DagNode* target = message->getArgument(1);
		      DagNode* reply;
		      //
		      //	Find the rule rewrite we were asked for.
		      //
		      while (lastSolutionNr < solutionNr)
			{
			  bool success = state->findNextRewrite();
			  if (!success)
			    {
			      Vector<DagNode*> args(3);
			      args[0] = target;
			      args[1] = message->getArgument(0);
			      args[2] = upRewriteCount(state->getContext());
			      reply = noSuchResultMsg->makeDagNode(args);
			      context.addInCount(*(state->getContext()));  // account for any remaining rewrites
			      delete state;
			      goto done;
			    }
			  ++lastSolutionNr;
			}
		      mm->insert(message, state, solutionNr);
		      {
			//
			//	Do rule application.
			//
			Rule* rule = state->getRule();
			RewritingContext* stateContext = state->getContext();
			bool trace = RewritingContext::getTraceStatus();
			if (trace)
			  {
			    stateContext->tracePreRuleRewrite(state->getDagNode(), rule);
			    if (stateContext->traceAbort())
			      {
				(void) m->unprotect();
				return false;
			      }
			  }
			//
			//	Get replacement and make new top dag.
			//
			DagNode* replacement = state->getReplacement();
			if (!atTop) 
			  replacement = replacement->makeClone();  // make sure pointer is unique
			RewriteSearchState::DagPair top = state->rebuildDag(replacement);
			//
			//	If we are not atTop then we need to make a metaContext, before
			//	we reduce because reduction can change top.first
			//
			PointerMap qidMap;
			DagRoot metaContext;  // need protection since we will do a reduce
			if (!atTop)
			  {
			    //
			    //	Can't use dagNodeMap after reduce since the from dagNodes might
			    //	garbage collected or even rewritten in place.
			    //
			    PointerMap dagNodeMap;
			    metaContext.setNode(metaLevel->upContext(top.first, m, replacement, qidMap, dagNodeMap));
			  }
			//
			//	Handle reduction of result.
			//
			RewritingContext* resultContext =
			  context.makeSubcontext(top.first, UserLevelRewritingContext::META_EVAL);
			if (trace)
			  resultContext->tracePostRuleRewrite(top.second);
			resultContext->reduce();
			stateContext->addInCount(*resultContext);
			stateContext->incrementRlCount();
			//
			//	Make a reply.
			//
			//	op appliedRule : Oid Oid RewriteCount Term Type Substitution -> Msg  .
			//                        0   1      2         3    4        5
			//
			//	op appliedRule : Oid Oid RewriteCount Term Type Substitution Context -> Msg  .
			//                        0   1      2         3    4        5          6
			//
			Vector<DagNode*> args(atTop ? 6 : 7);
			args[0] = target;
			args[1] = message->getArgument(0);
			args[2] = upRewriteCount(stateContext);

			PointerMap dagNodeMap;
			DagNode* result = resultContext->root();
			args[3] = metaLevel->upDagNode(result, m, qidMap, dagNodeMap);
			args[4] = metaLevel->upType(result->getSort(), qidMap);
			args[5] = metaLevel->upSubstitution(*stateContext, *rule, m, qidMap, dagNodeMap);

			if (atTop)
			  reply = appliedRuleMsg->makeDagNode(args);
			else
			  {
			    args[6] = metaContext.getNode();
			    reply = appliedRule2Msg->makeDagNode(args);
			  }
			delete resultContext;
			state->transferCountTo(context);
		      }
		    done:
		      context.bufferMessage(target, reply);
		      (void) m->unprotect();
		      return true;
		    }
		}
	    }
	}
    }
  return false;
}
