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
//	MetaInterpreters: getMatch()/getXmatch() message.
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

DagNode*
InterpreterManagerSymbol::getMatch(FreeDagNode* message,
				   ObjectSystemRewritingContext& context,
				   Interpreter* interpreter)
{
  //
  //	op getMatch : Oid Oid Qid Term Term Condition Nat -> Msg .
  //                   0   1   2   3    4       5      6
  //
  Int64 solutionNr;
  if (metaLevel->downSaturate64(message->getArgument(6), solutionNr) && solutionNr >= 0)
    {
      DagNode* errorMessage;
      if (MetaModule* mm = getMetaModule(message, 2, interpreter, errorMessage))
	{
	  MatchSearchState* state;
	  Int64 lastSolutionNr;
	  if (mm->getCachedStateObject(message, context, solutionNr, state, lastSolutionNr))
	    mm->protect();  // use cached state so protect the module
	  else if ((state = makeMatchSearchState(mm, message, context)))
	    lastSolutionNr = -1;
	  else
	    return makeErrorReply("Bad matching problem.", message);

	  DagNode* target = message->getArgument(1);
	  //
	  //	Now we compute the requested matcher.
	  //
	  for (; lastSolutionNr < solutionNr; ++lastSolutionNr)
	    {
	      if (!(state->findNextMatch()))
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
	  //
	  //	Got the solution we wanted so cache the state.
	  //
	  mm->insert(message, state, solutionNr);
	  {
	    //
	    //	op gotMatch : Oid Oid RewriteCount Substitution -> Msg .
	    //	               0   1       2            3
	    //
	    Vector<DagNode*> args(4);
	    args[0] = target;
	    args[1] = message->getArgument(0);
	    args[2] = upRewriteCount(state->getContext());
	    Substitution* substitution = state->getContext();
	    Pattern* pattern  = state->getPattern();
	    PointerMap qidMap;
	    PointerMap dagNodeMap;
	    args[3] = metaLevel->upSubstitution(*substitution, *pattern, mm, qidMap, dagNodeMap);
	    state->transferCountTo(context);
	    (void) mm->unprotect();
	    return gotMatchMsg->makeDagNode(args);
	  }
	}
      return errorMessage;
    }
  return makeErrorReply("Bad solution number.", message);
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
      //        Pattern and subject might be in different kinds because
      //        we are searching a subject using extension.
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

DagNode*
InterpreterManagerSymbol::getXmatch(FreeDagNode* message,
				    ObjectSystemRewritingContext& context,
				    Interpreter* interpreter)
{
  //
  //	op getXmatch : Oid Oid Qid Term Term Condition Nat Bound Nat -> Msg .
  //                    0   1   2   3    4       5      6    7    8
  //
  Int64 solutionNr;
  if (metaLevel->downSaturate64(message->getArgument(8), solutionNr) && solutionNr >= 0)
    {
      DagNode* errorMessage;
      if (MetaModule* mm = getMetaModule(message, 2, interpreter, errorMessage))
	{
	  MatchSearchState* state;
	  Int64 lastSolutionNr;
	  if (mm->getCachedStateObject(message, context, solutionNr, state, lastSolutionNr))
	    mm->protect();  // use cached state so protect the module
	  else if ((state = makeMatchSearchState2(mm, message, context)))
	    lastSolutionNr = -1;
	  else
	    return makeErrorReply("Bad matching problem.", message);

	  DagNode* target = message->getArgument(1);
	  for (; lastSolutionNr < solutionNr; ++lastSolutionNr)
	    {
	      if (!(state->findNextMatch()))
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
	  //
	  //	Got the solution we wanted so cache the state.
	  //
	  mm->insert(message, state, solutionNr);
	  {
	    //
	    //	op gotXmatch : Oid Oid RewriteCount Substitution Context -> Msg .
	    //	                0   1       2            3          4
	    //
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
	    VariableSymbol* vs = safeCast(VariableSymbol*, mm->instantiateVariable(sort));
	    DagNode* hole = new VariableDagNode(vs, 0, UNDEFINED);
	    PositionState::DagPair top = state->rebuildDag(hole);

	    PointerMap qidMap;
	    PointerMap dagNodeMap;
	    args[3] = metaLevel->upSubstitution(*substitution, *pattern, mm, qidMap, dagNodeMap);
	    args[4] = metaLevel->upContext(top.first, mm, hole, qidMap, dagNodeMap);
	    state->transferCountTo(context);
	    (void) mm->unprotect();
	    return gotXmatchMsg->makeDagNode(args);
	  }
	}
      return errorMessage;
    }
  return makeErrorReply("Bad solution number.", message);
}
