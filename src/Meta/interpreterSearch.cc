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
//	Code for getSearchResult()/getSearchResultAndPath() messages.
//

RewriteSequenceSearch*
InterpreterManagerSymbol::makeRewriteSequenceSearch(ImportModule* m,
						    FreeDagNode* message,
						    RewritingContext& context) const
{
  RewriteSequenceSearch::SearchType searchType;
  int maxDepth;
  if (metaLevel->downSearchType(message->getArgument(6), searchType) &&
      metaLevel->downBound(message->getArgument(7), maxDepth))
    {
      Term* s;
      Term* g;
      if (metaLevel->downTermPair(message->getArgument(3), message->getArgument(4), s, g, m))
	{
	  Vector<ConditionFragment*> condition;
	  if (metaLevel->downCondition(message->getArgument(5), m, condition))
	    {
	      m->protect();
	      Pattern* goal = new Pattern(g, false, condition);
	      RewritingContext* subContext = term2RewritingContext(s, context);
	      return new RewriteSequenceSearch(subContext,
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
InterpreterManagerSymbol::getSearchResult(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  //
  //	op getSearchResult : Oid Oid Qid Term Term Condition Qid Bound Nat -> Msg .
  //                          0   1   2   3    4       5      6    7    8
  //
  //	op getSearchResultAndPath : Oid Oid Qid Term Term Condition Qid Bound Nat -> Msg .
  //                                 0   1   2   3    4       5      6    7    8
  //
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      Int64 solutionNr;
      if (metaLevel->downSaturate64(message->getArgument(8), solutionNr) &&
	  solutionNr >= 0)
	{
	  int id;
	  if (metaLevel->downQid(message->getArgument(2), id))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  if (ImportModule* m = pm->getFlatModule())
		    {
		      RewriteSequenceSearch* state;
		      Int64 lastSolutionNr;
		      MetaModule* mm = safeCast(MetaModule*, m);  // metaInterpreter can only store MetaModules
		      if (mm->getCachedStateObject(message,
						   context,
						   solutionNr,
						   state,
						   lastSolutionNr))
			m->protect();  // use cached state so protect the module
		      else if ((state = makeRewriteSequenceSearch(m, message, context)))
			lastSolutionNr = -1;
		      else
			return false;

		      DagNode* target = message->getArgument(1);
		      DagNode* reply;

		      while (lastSolutionNr < solutionNr)
			{
			  bool success = state->findNextMatch();
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
		      {
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
			args[3] = metaLevel->upDagNode(dagNode, m, qidMap, dagNodeMap);
			args[4] = metaLevel->upType(dagNode->getSort(), qidMap);
			args[5] = metaLevel->upSubstitution(*(state->getSubstitution()),
							    *(state->getGoal()),
							    m,
							    qidMap,
							    dagNodeMap);
			if (includeTrace)
			  {
			    args[6] = metaLevel->upTrace(*state, m);
			    reply = gotSearchResultAndPathMsg->makeDagNode(args);
			  }
			else
			  reply = gotSearchResultMsg->makeDagNode(args);
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
