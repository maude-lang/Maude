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
//	Code for rewriteTerm()/frewriteTerm()/erewrite() messages.
//

bool
InterpreterManagerSymbol::reduceTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      int id;
      if (metaLevel->downQid(message->getArgument(2), id))
	{
	  if (PreModule* pm = interpreter->getModule(id))
	    {
	      if (ImportModule* m = pm->getFlatModule())
		{
		   if (Term* t = metaLevel->downTerm(message->getArgument(3), m))
		     {
		       RewritingContext* objectContext = term2RewritingContext(t, context);
		       m->protect();
		       objectContext->reduce();
		       context.addInCount(*objectContext);

		       Vector<DagNode*> reply(5);
		       DagNode* target = message->getArgument(1);
		       reply[0] = target;
		       reply[1] = message->getArgument(0);
		       reply[2] = upRewriteCount(objectContext);

		       PointerMap qidMap;
		       PointerMap dagNodeMap;
		       DagNode* dagNode = objectContext->root();
		       reply[3] = metaLevel->upDagNode(dagNode, m, qidMap, dagNodeMap);
		       reply[4] = metaLevel->upType(dagNode->getSort(), qidMap);
		       context.bufferMessage(target, reducedTermMsg->makeDagNode(reply));

		       delete objectContext;
		       (void) m->unprotect();
		       return true;
		     }
		}
	    }
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::rewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      Int64 limit;
      if (metaLevel->downBound64(message->getArgument(2), limit))
	{
	  int id;
	  if (metaLevel->downQid(message->getArgument(3), id))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  if (ImportModule* m = pm->getFlatModule())
		    {
		      if (Term* t = metaLevel->downTerm(message->getArgument(4), m))
			{
			  RewritingContext* objectContext = term2RewritingContext(t, context);
			  m->protect();
			  m->resetRules();
			  objectContext->ruleRewrite(limit);
			  context.addInCount(*objectContext);

			  Vector<DagNode*> reply(5);
			  DagNode* target = message->getArgument(1);
			  reply[0] = target;
			  reply[1] = message->getArgument(0);
			  reply[2] = upRewriteCount(objectContext);

			  PointerMap qidMap;
			  PointerMap dagNodeMap;
			  DagNode* dagNode = objectContext->root();
			  reply[3] = metaLevel->upDagNode(dagNode, m, qidMap, dagNodeMap);
			  reply[4] = metaLevel->upType(dagNode->getSort(), qidMap);
			  context.bufferMessage(target, rewroteTermMsg->makeDagNode(reply));
			  //
			  //	We should somehow save state for continuation.
			  //
			  delete objectContext;
			  (void) m->unprotect();
			  return true;
			}
		    }
		}
	    }
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::frewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      Int64 limit;
      Int64 gas;
      if (metaLevel->downBound64(message->getArgument(2), limit) && limit != 0 &&
	  metaLevel->downSaturate64(message->getArgument(3), gas) && gas != 0)
	{
	  int id;
	  if (metaLevel->downQid(message->getArgument(4), id))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  if (ImportModule* m = pm->getFlatModule())
		    {
		      if (Term* t = metaLevel->downTerm(message->getArgument(5), m))
			{
			  RewritingContext* objectContext = term2RewritingContext(t, context);
			  m->protect();
			  m->resetRules();
			  objectContext->fairRewrite(limit, gas);
			  objectContext->root()->computeTrueSort(*objectContext);  // needed so we have well defined sorts
			  context.addInCount(*objectContext);

			  Vector<DagNode*> reply(5);
			  DagNode* target = message->getArgument(1);
			  reply[0] = target;
			  reply[1] = message->getArgument(0);
			  reply[2] = upRewriteCount(objectContext);

			  PointerMap qidMap;
			  PointerMap dagNodeMap;
			  DagNode* dagNode = objectContext->root();
			  reply[3] = metaLevel->upDagNode(dagNode, m, qidMap, dagNodeMap);
			  reply[4] = metaLevel->upType(dagNode->getSort(), qidMap);
			  context.bufferMessage(target, frewroteTermMsg->makeDagNode(reply));
			  //
			  //	We should somehow save state for continuation.
			  //
			  delete objectContext;
			  (void) m->unprotect();
			  return true;
			}
		    }
		}
	    }
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::erewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      Int64 limit;
      Int64 gas;
      if (metaLevel->downBound64(message->getArgument(2), limit) && limit != 0 &&
	  metaLevel->downSaturate64(message->getArgument(3), gas) && gas != 0)
	{
	  int id;
	  if (metaLevel->downQid(message->getArgument(4), id))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  if (ImportModule* m = pm->getFlatModule())
		    {
		      if (Term* t = metaLevel->downTerm(message->getArgument(5), m))
			{
			  RewritingContext* subContext = term2RewritingContext(t, context);
			  ObjectSystemRewritingContext* objectContext = safeCast(ObjectSystemRewritingContext*, subContext);
			  objectContext->setObjectMode(ObjectSystemRewritingContext::EXTERNAL);
			  m->protect();
			  m->resetRules();
			  objectContext->fairStart(limit, gas);
			  objectContext->externalRewrite();
			  objectContext->root()->computeTrueSort(*objectContext);  // needed so we have well defined sorts
			  context.addInCount(*objectContext);

			  Vector<DagNode*> reply(5);
			  DagNode* target = message->getArgument(1);
			  reply[0] = target;
			  reply[1] = message->getArgument(0);
			  reply[2] = upRewriteCount(objectContext);

			  PointerMap qidMap;
			  PointerMap dagNodeMap;
			  DagNode* dagNode = objectContext->root();
			  reply[3] = metaLevel->upDagNode(dagNode, m, qidMap, dagNodeMap);
			  reply[4] = metaLevel->upType(dagNode->getSort(), qidMap);
			  context.bufferMessage(target, erewroteTermMsg->makeDagNode(reply));
			  //
			  //	We should somehow save state for continuation.
			  //
			  delete objectContext;
			  DebugAdvisory("deleted context " << (void*) objectContext);
			  (void) m->unprotect();
			  return true;
			}
		    }
		}
	    }
	}
    }
  return false;
}

StrategicSearch*
InterpreterManagerSymbol::makeStrategicSearch(MetaModule* m,
					      FreeDagNode* message,
					      RewritingContext& context,
					      bool depthFirst) const
{
  if (Term* t = metaLevel->downTerm(message->getArgument(3), m))
    {
      if (StrategyExpression* s = metaLevel->downStratExpr(message->getArgument(4), m))
	{
	  TermSet boundVars;
	  VariableInfo vinfo;
	  if (s->check(vinfo, boundVars))
	    {
	      // Prepares the strategy
	      m->protect();
	      s->process();

	      // Prepares the term
	      RewritingContext* objectContext = term2RewritingContext(t, context);
	      objectContext->reduce();
	      if (depthFirst)
		return new DepthFirstStrategicSearch(objectContext, s);
	      else
		return new FairStrategicSearch(objectContext, s);
	    }
	  else
	    {
	      DebugAdvisory("bad metastrategy (check)" << QUOTE(s));
	      delete s;
	      t->deepSelfDestruct();
	    }
	}
      else
	{
	  DebugAdvisory("bad metastrategy " << QUOTE(message->getArgument(4)));
	  t->deepSelfDestruct();
	}
    }
  else
    {
      DebugAdvisory("bad metaterm " << QUOTE(message->getArgument(1)));
    }
  return 0;
}

bool
InterpreterManagerSymbol::srewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  //
  //	op srewriteTerm : Oid Oid Qid Term Strategy SrewriteOption Nat -> Msg .
  //	                   0   1   2   3       4          5         6
  //
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      bool depthFirst;
      Int64 solutionNr;
      if (metaLevel->downSrewriteOption(message->getArgument(5), depthFirst) &&
	  metaLevel->downSaturate64(message->getArgument(6), solutionNr) &&
	  solutionNr >= 0)
	{
	  int id;
	  if (metaLevel->downQid(message->getArgument(2), id))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  if (ImportModule* m = pm->getFlatModule())
		    {
		      StrategicSearch* state;
		      Int64 lastSolutionNr;
		      MetaModule* mm = safeCast(MetaModule*, m);  // metaInterpreter can only store MetaModules
		      if (mm->getCachedStateObject(message,
						   context,
						   solutionNr,
						   state,
						   lastSolutionNr))
			m->protect();  // use cached state so protect the module
		      else if ((state = makeStrategicSearch(mm, message, context, depthFirst)))
			lastSolutionNr = -1;
		      else
			return false;

		      DagNode* target = message->getArgument(1);
		      DagNode* reply;
		      
		      DagNode* result = state->lastSolution();
		      while (lastSolutionNr < solutionNr)
			{
			  result = state->findNextSolution();
			  if (result == 0)
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
			//	op srewroteTerm : Oid Oid RewriteCount Term Type -> Msg  .
			//	                   0   1       2        3    4
			//
			mm->insert(message, state, solutionNr);

			Vector<DagNode*> args(5);
			args[0] = target;
			args[1] = message->getArgument(0);
			args[2] = upRewriteCount(state->getContext());

			PointerMap qidMap;
			PointerMap dagNodeMap;
			args[3] = metaLevel->upDagNode(result, m, qidMap, dagNodeMap);
			args[4] = metaLevel->upType(result->getSort(), qidMap);
			reply = srewroteTermMsg->makeDagNode(args);
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
