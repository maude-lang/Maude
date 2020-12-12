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
//	MetaInterpreters: both applyRule() messages.
//

DagNode*
InterpreterManagerSymbol::applyRule(FreeDagNode* message,
				    ObjectSystemRewritingContext& context,
				    bool atTop,
				    Interpreter* interpreter)
{
  //
  //	op applyRule : Oid Oid Qid Term Qid Substition Nat -> Msg .
  //                    0   1   2   3    4      5       6
  //
  //	op applyRule : Oid Oid Qid Term Qid Substition Nat Bound Nat -> Msg .
  //                    0   1   2   3    4      5       6    7    8
  //
  Int64 solutionNr;
  if (metaLevel->downSaturate64(message->getArgument(atTop ? 6 : 8), solutionNr) && solutionNr >= 0)
    {
      DagNode* errorMessage;
      if (MetaModule* mm = getMetaModule(message, 2, interpreter, errorMessage))
	{
	  RewriteSearchState* state;
	  Int64 lastSolutionNr;
	  if (mm->getCachedStateObject(message, solutionNr, state, lastSolutionNr))
	    mm->protect();  // Use cached state
	  else if ((state = makeRewriteSearchState(mm, message, context, atTop)))
	    lastSolutionNr = -1;
	  else
	    return makeErrorReply("Bad rule application.", message);

	  DagNode* target = message->getArgument(1);
	  //
	  //	Find the rule rewrite we were asked for.
	  //
	  for (; lastSolutionNr < solutionNr; ++lastSolutionNr)
	    {
	      if (!(state->findNextRewrite()))
		{
		  Vector<DagNode*> args(3);
		  args[0] = target;
		  args[1] = message->getArgument(0);
		  args[2] = upRewriteCount(state->getContext());
		  context.addInCount(*(state->getContext()));  // account for any remaining rewrites
		  delete state;
		  (void) mm->unprotect();
		  return noSuchResultMsg->makeDagNode(args);
		}
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
		    (void) mm->unprotect();
		    return makeErrorReply("Aborted.", message);  // HACK
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
		metaContext.setNode(metaLevel->upContext(top.first,
							 mm,
							 replacement,
							 qidMap,
							 dagNodeMap));
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
	    args[3] = metaLevel->upDagNode(result, mm, qidMap, dagNodeMap);
	    args[4] = metaLevel->upType(result->getSort(), qidMap);
	    args[5] = metaLevel->upSubstitution(*stateContext, *rule, mm, qidMap, dagNodeMap);

	    delete resultContext;
	    state->transferCountTo(context);
	    (void) mm->unprotect();
	    if (atTop)
	      return appliedRuleMsg->makeDagNode(args);
	    else
	      {
		args[6] = metaContext.getNode();
		return appliedRule2Msg->makeDagNode(args);
	      }
	  }
	}
      return errorMessage;
    }
  return makeErrorReply("Bad solution number.", message);
}
