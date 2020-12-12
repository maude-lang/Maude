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
//      MetaInterpreters: search messages.
//

DagNode*
InterpreterManagerSymbol::getSearchResult(FreeDagNode* message,
					  ObjectSystemRewritingContext& context,
					  Interpreter* interpreter)
{
  //
  //	op getSearchResult : Oid Oid Qid Term Term Condition Qid Bound Nat -> Msg .
  //                          0   1   2   3    4       5      6    7    8
  //
  //	op getSearchResultAndPath : Oid Oid Qid Term Term Condition Qid Bound Nat -> Msg .
  //                                 0   1   2   3    4       5      6    7    8
  //
  Int64 solutionNr;
  if (metaLevel->downSaturate64(message->getArgument(8), solutionNr) && solutionNr >= 0)
    {
      DagNode* errorMessage;
      if (MetaModule* mm = getMetaModule(message, 2, interpreter, errorMessage))
	{
	  RewriteSequenceSearch* state;
	  Int64 lastSolutionNr;
	  if (mm->getCachedStateObject(message,
				       context,
				       solutionNr,
				       state,
				       lastSolutionNr))
	    mm->protect();  // use cached state so protect the module
	  else if ((state = makeRewriteSequenceSearch(mm, message, context)))
	    lastSolutionNr = -1;
	  else
	    return makeErrorReply("Bad search.", message);

	  DagNode* target = message->getArgument(1);
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextMatch();
	      if (!success)
		{
		  Vector<DagNode*> args(3);
		  args[0] = target;
		  args[1] = message->getArgument(0);
		  args[2] = upRewriteCount(state->getContext());
		  //
		  //	Account for any remaining rewrites.
		  //
		  context.addInCount(*(state->getContext())); 
		  delete state;
		  return noSuchResultMsg->makeDagNode(args);
		}
	      ++lastSolutionNr;
	    }
	  //
	  //	Desired solution found. Cache new state and generate reply.
	  //
	  //	op gotSearchResult : Oid Oid RewriteCount Term Type Substitution -> Msg .
	  //	                      0   1       2        3    4        5
	  //
	  //	op gotSearchResultAndPath : Oid Oid RewriteCount Term Type Substitution Trace -> Msg .
	  //	                             0   1       2        3    4        5         6
	  //
	  mm->insert(message, state, solutionNr);

	  bool includeTrace = (message->symbol() == getSearchResultAndPathMsg);
	  Vector<DagNode*> args(includeTrace ? 7 : 6);
	  args[0] = target;
	  args[1] = message->getArgument(0);
	  args[2] = upRewriteCount(state->getContext());

	  DagNode* dagNode = state->getStateDag(state->getStateNr());
	  PointerMap qidMap;
	  PointerMap dagNodeMap;
	  args[3] = metaLevel->upDagNode(dagNode, mm, qidMap, dagNodeMap);
	  args[4] = metaLevel->upType(dagNode->getSort(), qidMap);
	  args[5] = metaLevel->upSubstitution(*(state->getSubstitution()),
					      *(state->getGoal()),
					      mm,
					      qidMap,
					      dagNodeMap);
	  state->transferCountTo(context);
	  (void) mm->unprotect();
	  if (includeTrace)
	    {
	      args[6] = metaLevel->upTrace(*state, mm);
	      return gotSearchResultAndPathMsg->makeDagNode(args);
	    }
	  return gotSearchResultMsg->makeDagNode(args);
	}
      return errorMessage;
    }
  return makeErrorReply("Bad solution number.", message);
}
