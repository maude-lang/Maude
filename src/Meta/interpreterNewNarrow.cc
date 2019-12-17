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
//	Code for getOneStepNarrow() message.
//

NarrowingSearchState2* 
InterpreterManagerSymbol::makeNarrowingSearchState2(ImportModule* m,
						    FreeDagNode* message,
						    RewritingContext& context) const
{
  int variableFamilyName;
  if (metaLevel->downQid(message->getArgument(5), variableFamilyName))
    {
      int variableFamily = FreshVariableSource::getFamily(variableFamilyName);
      if (variableFamily == NONE)
	return 0;

      if (Term* t = metaLevel->downTerm(message->getArgument(3), m))
	{
	  Vector<Term*> blockerTerms;
	  if (!metaLevel->downTermList(message->getArgument(4), m, blockerTerms))
	    {
	      t->deepSelfDestruct();
	      return 0;
	    }
	  m->protect();

	  RewritingContext* subjectContext = term2RewritingContext(t, context);
	  subjectContext->reduce();

	  Vector<DagNode*> blockerDags; 
	  FOR_EACH_CONST(i, Vector<Term*>, blockerTerms)
	    {
	      Term* t = *i;
	      t = t->normalize(true);  // we don't really need to normalize but we do need to set hash values
	      blockerDags.append(t->term2Dag());
	      t->deepSelfDestruct();
	    }

	  return new NarrowingSearchState2(subjectContext,
					   blockerDags,
					   new FreshVariableSource(m, 0),
					   variableFamily);
	}
    }
  return 0;
}

bool
InterpreterManagerSymbol::getOneStepNarrowing(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  //
  //	op getOneStepNarrowing : Oid Oid Qid Term TermList Qid Nat -> Msg .
  //                              0   1   2   3      4      5   6  
  //
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      Int64 solutionNr;
      if (metaLevel->downSaturate64(message->getArgument(6), solutionNr) &&
	  solutionNr >= 0)
	{
	  int id;
	  if (metaLevel->downQid(message->getArgument(2), id))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  if (ImportModule* m = pm->getFlatModule())
		    {
		      NarrowingSearchState2* state;
		      Int64 lastSolutionNr;
		      MetaModule* mm = safeCast(MetaModule*, m);  // metaInterpreter can only store MetaModules

		      if (mm->getCachedStateObject(message, solutionNr, state, lastSolutionNr))
			m->protect();  // Use cached state
		      else if ((state = makeNarrowingSearchState2(m, message, context)))
			lastSolutionNr = -1;
		      else
			return false;

		      DagNode* target = message->getArgument(1);
		      DagNode* reply;

		      bool actualStep = lastSolutionNr < solutionNr;  // making a step or using result from cache?
		      while (lastSolutionNr < solutionNr)
			{
			  bool success = state->findNextNarrowing();
			  if (!success)
			    {
			      Vector<DagNode*> args(4);
			      args[0] = target;
			      args[1] = message->getArgument(0);
			      args[2] = upRewriteCount(state->getContext());
			      args[3] = metaLevel->upBool(!(state->isIncomplete()));
			      reply = noSuchResult3Msg->makeDagNode(args);
			      context.addInCount(*(state->getContext()));  // account for any remaining rewrites
			      delete state;
			      goto done;
			    }
			  ++lastSolutionNr;
			}
		      {
			mm->insert(message, state, solutionNr);
			const Substitution& unifier = state->getSubstitution();
			//
			//	getNarrowedDag() guarantees that hole appears exactly once
			//	in the returned dag, even if the rule used had a bare variable
			//	for its rhs, and that bare variable could share its binding
			//	with other variables in the dag being narrowed.
			//
			DagNode* hole;
			DagNode* replacementContext;
			DagNode* narrowedDag = state->getNarrowedDag(hole, replacementContext);
			//
			//	If we're going to trace, we show the trace of the narrowing
			//	step before any reductions we do on it.
			//
			RewritingContext* stateContext = state->getContext();
			if (actualStep)
			  {
			    stateContext->incrementNarrowingCount();
			    if (RewritingContext::getTraceStatus())
			      {
				//
				//	We pass the active term and variable info. This means
				//	the version with (potentially) renamed variables. We do this for
				//	consistancy since the replaced dag will be expressed in those
				//	same variables.
				//
				RewritingContext* subjectContext = state->getActiveContext();
				const NarrowingVariableInfo& narrowingVariableInfo = state->getActiveVariableInfo();
				subjectContext->traceNarrowingStep(state->getRule(),
								   state->getReplacedDag(),
								   hole,  // replacement
								   &narrowingVariableInfo,  // original variable names
								   &unifier,
								   narrowedDag);
				if (subjectContext->traceAbort())
				  {
				    (void) m->unprotect();
				    return false;
				  }
			      }
			  }
			//
			//	Can't use dagNodeMapForContext after reduce since the from dagNodes might be
			//	garbage collected or even rewritten in place.
			//
			PointerMap qidMap;
			PointerMap dagNodeMapForContext;
			DagRoot metaContext(metaLevel->upContext(replacementContext, m, hole, qidMap, dagNodeMapForContext));
			//
			//	Need to reduce narrowedDag.
			//
			RewritingContext* resultContext =
			  context.makeSubcontext(narrowedDag, UserLevelRewritingContext::META_EVAL);
			resultContext->reduce();
			stateContext->addInCount(*resultContext);
			//
			//	op gotOneStepNarrowing : Oid Oid RewriteCount Term Type Context Qid Substitution Substitution Qid -> Msg .
			//	                          0   1       2        3    4      5     6        7            8       9
			//
			Vector<DagNode*> args(10);
			args[0] = target;
			args[1] = message->getArgument(0);
			args[2] = upRewriteCount(stateContext);

			PointerMap dagNodeMap;
			DagNode* result = resultContext->root();
			args[3] = metaLevel->upDagNode(result, m, qidMap, dagNodeMap);
			args[4] = metaLevel->upType(result->getSort(), qidMap);
			args[5] = metaContext.getNode();

			Rule* rule = state->getRule();
			int label = rule->getLabel().id();
			if (label == NONE)
			  label = Token::encode("");
			args[6] = metaLevel->upQid(label, qidMap);

			args[7] = metaLevel->upPartialSubstitution(unifier, state->getVariableInfo(), m, qidMap, dagNodeMap);
			args[8] = metaLevel->upSubstitution(unifier, *rule, m, qidMap, dagNodeMap);
			args[9] = metaLevel->upQid(FreshVariableSource::getBaseName(state->getVariableFamily()), qidMap);

			reply = gotOneStepNarrowingMsg->makeDagNode(args);
			delete resultContext;
			context.transferCountFrom(*stateContext);
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
