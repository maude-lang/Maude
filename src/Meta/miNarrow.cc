/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//	MetaInterpreters: getOneStepNarrowing() message.
//

DagNode*
InterpreterManagerSymbol::getOneStepNarrowing(FreeDagNode* message,
					      ObjectSystemRewritingContext& context,
					      Interpreter* interpreter)		    
{				    
  //
  //	op getOneStepNarrowing : Oid Oid Qid Term TermList Qid VariantOptionSet Nat -> Msg .
  //                              0   1   2   3      4      5         6          7
  //
  Int64 solutionNr;
  if (metaLevel->downSaturate64(message->getArgument(7), solutionNr) && solutionNr >= 0)
    {
      DagNode* errorMessage;
      if (MetaModule* mm = getMetaModule(message, 2, interpreter, errorMessage))
	{
	  NarrowingSearchState2* state;
	  Int64 lastSolutionNr;
	  if (!(mm->getCachedStateObject(message, solutionNr, state, lastSolutionNr)))
	    {
	      //
	      //	No cached narrowing search so make one.
	      //
	      int variantFlags;
	      if (!(metaLevel->downVariantOptionSet(message->getArgument(6), variantFlags) &&
		    (variantFlags & ~(MetaLevel::DELAY | MetaLevel::FILTER)) == 0))
		return makeErrorReply("Bad option.", message);
	      state = makeNarrowingSearchState2(mm, message, context, variantFlags);
	      if (state == 0)
		return makeErrorReply("Bad narrowing problem.", message);
	      lastSolutionNr = -1;
	    }
	  mm->protect();
	  DagNode* target = message->getArgument(1);
	  //
	  //	Find requested solution.
	  //
	  bool actualStep = lastSolutionNr < solutionNr;  // making a step or using result from cache?
	  for (; lastSolutionNr < solutionNr; ++lastSolutionNr)
	    {
	      if (!(state->findNextNarrowing()))
		{
		  Vector<DagNode*> args(4);
		  args[0] = target;
		  args[1] = message->getArgument(0);
		  args[2] = upRewriteCount(state->getContext());
		  args[3] = metaLevel->upBool(!(state->isIncomplete()));
		  context.addInCount(*(state->getContext()));  // account for any remaining rewrites
		  delete state;
		  (void) mm->unprotect();
		  return noSuchResult3Msg->makeDagNode(args);
		}
	    }
	  mm->insert(message, state, solutionNr);
	  {
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
		    //	the version with (potentially) renamed variables. We do
		    //	this for consistancy since the replaced dag will be
		    //	expressed in those same variables.
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
			(void) mm->unprotect();
			return makeErrorReply("Aborted.", message);
		      }
		  }
	      }
	    //
	    //	Can't use dagNodeMapForContext after reduce since the from
	    //	dagNodes might be garbage collected or even rewritten in place.
	    //
	    PointerMap qidMap;
	    PointerMap dagNodeMapForContext;
	    DagRoot metaContext(metaLevel->upContext(replacementContext,
						     mm,
						     hole,
						     qidMap,
						     dagNodeMapForContext));
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
	    //cerr << "message = " << message << endl;
	    //cerr << "message ptr " << (void*) message << endl;
	    Vector<DagNode*> args(10);
	    args[0] = target;
	    args[1] = message->getArgument(0);
	    args[2] = upRewriteCount(stateContext);

	    PointerMap dagNodeMap;
	    DagNode* result = resultContext->root();
	    args[3] = metaLevel->upDagNode(result, mm, qidMap, dagNodeMap);
	    args[4] = metaLevel->upType(result->getSort(), qidMap);
	    args[5] = metaContext.getNode();

	    Rule* rule = state->getRule();
	    int label = rule->getLabel().id();
	    if (label == NONE)
	      label = Token::encode("");
	    args[6] = metaLevel->upQid(label, qidMap);
	    args[7] = metaLevel->upPartialSubstitution(unifier,
						       state->getVariableInfo(),
						       mm,
						       qidMap,
						       dagNodeMap);
	    args[8] = metaLevel->upSubstitution(unifier, *rule, mm, qidMap, dagNodeMap);
	    args[9] = metaLevel->upQid(FreshVariableSource::getBaseName(state->getVariableFamily()),
				       qidMap);

	    delete resultContext;
	    context.transferCountFrom(*stateContext);
	    (void) mm->unprotect();
	    return gotOneStepNarrowingMsg->makeDagNode(args);
	  }
	}
      return errorMessage;
    }
  return makeErrorReply("Bad solution number.", message);
}
