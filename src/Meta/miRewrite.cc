/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020-2021 SRI International, Menlo Park, CA 94025, USA.

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
//      MetaInterpreters: reduceTerm()/rewriteTerm()/frewriteTerm()/
//	erewriteTerm()/srewriteTerm() messages.
//

DagNode*
InterpreterManagerSymbol::reduceTerm(FreeDagNode* message,
				     ObjectSystemRewritingContext& context,
				     Interpreter* interpreter)
{
  DagNode* errorMessage;
  if (MetaModule* m = getMetaModule(message, 2, interpreter, errorMessage))
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
	  delete objectContext;
	  (void) m->unprotect();
	  return reducedTermMsg->makeDagNode(reply);
	}
      return makeErrorReply("Bad term.", message);
    }
  return errorMessage;
}

DagNode*
InterpreterManagerSymbol::rewriteTerm(FreeDagNode* message,
				      ObjectSystemRewritingContext& context,
				      Interpreter* interpreter)
{
  Int64 limit;
  if (metaLevel->downBound64(message->getArgument(2), limit))
    {
      DagNode* errorMessage;
      if (MetaModule* m = getMetaModule(message, 3, interpreter, errorMessage))
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
	      //
	      //	We should somehow save state for continuation.
	      //
	      delete objectContext;
	      (void) m->unprotect();
	      return rewroteTermMsg->makeDagNode(reply);
	    }
	  return makeErrorReply("Bad term.", message);
	}
      return errorMessage;
    }
  return makeErrorReply("Bad limit.", message);
}

DagNode*
InterpreterManagerSymbol::frewriteTerm(FreeDagNode* message,
				       ObjectSystemRewritingContext& context,
				       Interpreter* interpreter)
{
  Int64 limit;
  if (metaLevel->downBound64(message->getArgument(2), limit) && limit != 0)
    {
      Int64 gas;
      if (metaLevel->downSaturate64(message->getArgument(3), gas) && gas != 0)
	{
	  DagNode* errorMessage;
	  if (MetaModule* m = getMetaModule(message, 4, interpreter, errorMessage))
	    {
	      if (Term* t = metaLevel->downTerm(message->getArgument(5), m))
		{
		  RewritingContext* objectContext = term2RewritingContext(t, context);
		  m->protect();
		  m->resetRules();
		  objectContext->fairRewrite(limit, gas);
		  //
		  //	Needed so we have well defined sorts since fair rewriting
		  //	could stop anywhere.
		  //
		  objectContext->root()->computeTrueSort(*objectContext);
		  context.addInCount(*objectContext);

		  Vector<DagNode*> reply(5);
		  reply[0] = message->getArgument(1);
		  reply[1] = message->getArgument(0);
		  reply[2] = upRewriteCount(objectContext);

		  PointerMap qidMap;
		  PointerMap dagNodeMap;
		  DagNode* dagNode = objectContext->root();
		  reply[3] = metaLevel->upDagNode(dagNode, m, qidMap, dagNodeMap);
		  reply[4] = metaLevel->upType(dagNode->getSort(), qidMap);
		  //
		  //	We should somehow save state for continuation.
		  //
		  delete objectContext;
		  (void) m->unprotect();
		  return frewroteTermMsg->makeDagNode(reply);
		}
	      return makeErrorReply("Bad term.", message);
	    }
	  return errorMessage;
	}
      return makeErrorReply("Bad gas.", message);
    }
  return makeErrorReply("Bad limit.", message);
}


DagNode*
InterpreterManagerSymbol::erewriteTerm(FreeDagNode* message,
				       ObjectSystemRewritingContext& context,
				       Interpreter* interpreter)
{
  Int64 limit;
  if (metaLevel->downBound64(message->getArgument(2), limit) && limit != 0)
    {
      Int64 gas;
      if (metaLevel->downSaturate64(message->getArgument(3), gas) && gas != 0)
	{
	  DagNode* errorMessage;
	  if (MetaModule* m = getMetaModule(message, 4, interpreter, errorMessage))
	    {
	      if (Term* t = metaLevel->downTerm(message->getArgument(5), m))
		{
		  RewritingContext* subContext = term2RewritingContext(t, context);
		  ObjectSystemRewritingContext* objectContext =
		    safeCast(ObjectSystemRewritingContext*, subContext);
		  objectContext->setObjectMode(ObjectSystemRewritingContext::EXTERNAL);
		  m->protect();
		  m->resetRules();
		  objectContext->fairStart(limit, gas);
		  objectContext->externalRewrite();
		  //
		  //	Needed so we have well defined sorts since fair rewriting
		  //	could stop anywhere.
		  //
		  objectContext->root()->computeTrueSort(*objectContext);
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
		  //
		  //	We should somehow save state for continuation.
		  //
		  delete objectContext;
		  (void) m->unprotect();
		  return erewroteTermMsg->makeDagNode(reply);
		}
	      return makeErrorReply("Bad term.", message);
	    }
	  return errorMessage;
	}
      return makeErrorReply("Bad gas.", message);
    }
  return makeErrorReply("Bad limit.", message);
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

DagNode*
InterpreterManagerSymbol::srewriteTerm(FreeDagNode* message,
				       ObjectSystemRewritingContext& context,
				       Interpreter* interpreter)
{
  //
  //	op srewriteTerm : Oid Oid Qid Term Strategy SrewriteOption Nat -> Msg .
  //	                   0   1   2   3       4          5         6
  //
  bool depthFirst;
  if (metaLevel->downSrewriteOption(message->getArgument(5), depthFirst))
    {
      Int64 solutionNr;
      if (metaLevel->downSaturate64(message->getArgument(6), solutionNr) && solutionNr >= 0)
	{
	  DagNode* errorMessage;
	  if (MetaModule* mm = getMetaModule(message, 2, interpreter, errorMessage))
	    {
	      StrategicSearch* state;
	      Int64 lastSolutionNr;
	      if (mm->getCachedStateObject(message,
					   context,
					   solutionNr,
					   state,
					   lastSolutionNr))
		mm->protect();  // use cached state so protect the module
	      else if ((state = makeStrategicSearch(mm, message, context, depthFirst)))
		lastSolutionNr = -1;
	      else
		return makeErrorReply("Bad strategy.", message);

	      DagNode* target = message->getArgument(1);
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
		      //
		      //	Account for any remaining rewrites.
		      //
		      context.addInCount(*(state->getContext()));
		      delete state;
		      (void) mm->unprotect();
		      return noSuchResultMsg->makeDagNode(args);
		    }
		  ++lastSolutionNr;
		}
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
	      args[3] = metaLevel->upDagNode(result, mm, qidMap, dagNodeMap);
	      args[4] = metaLevel->upType(result->getSort(), qidMap);
	      context.transferCountFrom(*(state->getContext()));
	      (void) mm->unprotect();
	      return srewroteTermMsg->makeDagNode(args);
	    }
	  return errorMessage;
	}
      return makeErrorReply("Bad solution number.", message);
    }
  return makeErrorReply("Bad option.", message);
}
