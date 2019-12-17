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
//	Code for getMatch()/getXmatch() messages.
//

MatchSearchState*
InterpreterManagerSymbol::makeMatchSearchState(ImportModule* m,
					       FreeDagNode* message,
					       RewritingContext& context) const
{
  Term* p;
  Term* s;
  if (metaLevel->downTermPair(message->getArgument(3), message->getArgument(4), p, s, m))
    {
      Vector<ConditionFragment*> condition;
      if (metaLevel->downCondition(message->getArgument(5), m, condition))
	{
	  m->protect();
	  Pattern* pattern = new Pattern(p, false, condition);
	  RewritingContext* subjectContext = term2RewritingContext(s, context);
	  subjectContext->root()->computeTrueSort(*subjectContext);
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
InterpreterManagerSymbol::getMatch(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  //
  //	op getMatch : Oid Oid Qid Term Term Condition Nat -> Msg .
  //                   0   1   2   3    4       5      6
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
		      MatchSearchState* state;
		      Int64 lastSolutionNr;
		      MetaModule* mm = safeCast(MetaModule*, m);  // metaInterpreter can only store MetaModules
		      if (mm->getCachedStateObject(message,
						   context,
						   solutionNr,
						   state,
						   lastSolutionNr))
			m->protect();  // use cached state so protect the module
		      else if ((state = makeMatchSearchState(m, message, context)))
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
			//	op gotMatch : Oid Oid RewriteCount Substitution -> Msg .
			//	               0   1       2            3
			//
			mm->insert(message, state, solutionNr);

			Vector<DagNode*> args(4);
			args[0] = target;
			args[1] = message->getArgument(0);
			args[2] = upRewriteCount(state->getContext());

			VariableInfo* variableInfo = state->getPattern();
			Substitution* substitution = state->getContext();
			PointerMap qidMap;
			PointerMap dagNodeMap;
			args[3] = metaLevel->upSubstitution(*substitution,
							    *variableInfo,
							    m,
							    qidMap,
							    dagNodeMap);
			reply = gotMatchMsg->makeDagNode(args);
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

MatchSearchState*
InterpreterManagerSymbol::makeMatchSearchState2(ImportModule* m,
						FreeDagNode* subject,
						RewritingContext& context) const
{
  int minDepth;
  int maxDepth;
  if (metaLevel->downSaturate(subject->getArgument(6), minDepth) &&
      metaLevel->downBound(subject->getArgument(7), maxDepth))
    {
      //
      //	Pattern and subject might be in different kinds because
      //	we are searching a subject using extension.
      //
      if (Term* p = metaLevel->downTerm(subject->getArgument(3), m))
        {
          if (Term* s = metaLevel->downTerm(subject->getArgument(4), m))
	    {
	      Vector<ConditionFragment*> condition;
	      if (metaLevel->downCondition(subject->getArgument(5), m, condition))
		{
		  if (maxDepth == NONE)
		    maxDepth = UNBOUNDED;  // NONE means no extension for MatchSearchState; we want unbounded
		  m->protect();
		  Pattern* pattern = new Pattern(p, true, condition);
		  RewritingContext* subjectContext = term2RewritingContext(s, context);
		  subjectContext->root()->computeTrueSort(*subjectContext);
		  return new MatchSearchState(subjectContext,
					      pattern,
					      MatchSearchState::GC_PATTERN |
					      MatchSearchState::GC_CONTEXT |
					      MatchSearchState::GC_SUBSTITUTION,
					      minDepth,
					      maxDepth);
		}
	      s->deepSelfDestruct();
	    }
	  p->deepSelfDestruct();
	}
    }
  return 0;
}

bool
InterpreterManagerSymbol::getXmatch(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  //
  //	op getXmatch : Oid Oid Qid Term Term Condition Nat Bound Nat -> Msg .
  //                    0   1   2   3    4       5      6    7    8
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
		      MatchSearchState* state;
		      Int64 lastSolutionNr;
		      MetaModule* mm = safeCast(MetaModule*, m);  // metaInterpreter can only store MetaModules
		      if (mm->getCachedStateObject(message,
						   context,
						   solutionNr,
						   state,
						   lastSolutionNr))
			m->protect();  // use cached state so protect the module
		      else if ((state = makeMatchSearchState2(m, message, context)))
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
			//	op gotXmatch : Oid Oid RewriteCount Substitution Context -> Msg .
			//	                0   1       2            3          4
			//
			mm->insert(message, state, solutionNr);

			Vector<DagNode*> args(5);
			args[0] = target;
			args[1] = message->getArgument(0);
			args[2] = upRewriteCount(state->getContext());

			Substitution* substitution = state->getContext();
			Pattern* pattern = state->getPattern();
			//
			//	Need to construct the context where pattern was recongnized.
			//	We make a dummy variable and reconstruct the context around it.
			//
			Sort* sort = pattern->getLhs()->getSort();  // safe sort to use for hole variable
			VariableSymbol* vs = safeCast(VariableSymbol*, m->instantiateVariable(sort));
			DagNode* hole = new VariableDagNode(vs, 0, UNDEFINED);
			PositionState::DagPair top = state->rebuildDag(hole);

			PointerMap qidMap;
			PointerMap dagNodeMap;
			args[3] = metaLevel->upSubstitution(*substitution,
							    *pattern,
							    m,
							    qidMap,
							    dagNodeMap);
			args[4] = metaLevel->upContext(top.first, m, hole, qidMap, dagNodeMap);

			reply = gotXmatchMsg->makeDagNode(args);
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
