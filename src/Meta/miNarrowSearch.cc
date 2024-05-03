/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	MetaInterpreters: getNarrowingSearchResult() and getNarrowingSearchResultAndPath() messages.
//

NarrowingSequenceSearch3*
InterpreterManagerSymbol::makeNarrowingSequenceSearch3(ImportModule* m,
						       FreeDagNode* message,
						       RewritingContext& context,
						       int variantFlags) const
{
  RewriteSequenceSearch::SearchType searchType;
  bool fold;
  int maxDepth;
  if (metaLevel->downSearchType(message->getArgument(5), searchType) &&
      metaLevel->downFoldType(message->getArgument(7), fold) &&
      metaLevel->downBound(message->getArgument(6), maxDepth))
    {
      Term* s;
      Term* g;
      if (metaLevel->downTermPair(message->getArgument(3), message->getArgument(4), s, g, m))
	{
	  m->protect();

	  RewritingContext* subjectContext = term2RewritingContext(s, context);
	  g = g->normalize(true);
	  DagNode* goal = g->term2Dag();
	  g->deepSelfDestruct();

	  if (fold)
	    variantFlags |= NarrowingSequenceSearch3::FOLD;
	  Vector<DagNode*> startStates;
	  NarrowingSequenceSearch3* nss = new NarrowingSequenceSearch3(subjectContext,
								       startStates,
								       searchType,
								       goal,
								       maxDepth,
								       new FreshVariableSource(m, 0),
								       variantFlags);
	  if (nss->problemOK())
	    return nss;
	  else
	    delete nss;
	}
    }
  return 0;
}

DagNode*
InterpreterManagerSymbol::getNarrowingSearchResult(FreeDagNode* message,
						   ObjectSystemRewritingContext& context,
						   bool returnPath,
						   Interpreter* interpreter)
{
  //
  //	op getNarrowingSearchResult :        Oid Oid Qid Term Term Qid Bound Qid VariantOptionSet Nat
  //	                                     -> Msg .
  //	op getNarrowingSearchResultAndPath : Oid Oid Qid Term Term Qid Bound Qid VariantOptionSet Nat
  //	                                     -> Msg .
  //	                                      0   1   2   3    4    5    6    7         8          9
  //
  Int64 solutionNr;
  if (metaLevel->downSaturate64(message->getArgument(9), solutionNr) && solutionNr >= 0)
    {
      DagNode* errorMessage;
      if (MetaModule* m = getMetaModule(message, 2, interpreter, errorMessage))
	{
	  NarrowingSequenceSearch3* state;
	  Int64 lastSolutionNr;
	  if (!(m->getCachedStateObject(message, solutionNr, state, lastSolutionNr)))
	    {
	      //
	      //	No cached narrowing search so make one.
	      //
	      int variantFlags;
	      if (!(metaLevel->downVariantOptionSet(message->getArgument(8), variantFlags) &&
		    (variantFlags & ~(MetaLevel::DELAY | MetaLevel::FILTER)) == 0))
		return makeErrorReply("Bad option.", message);
	      if (returnPath)
		variantFlags |= NarrowingSequenceSearch3::KEEP_HISTORY;
	      state = makeNarrowingSequenceSearch3(m, message, context, variantFlags);
	      if (state == 0)
		return makeErrorReply("Bad narrowing search problem.", message);
	      lastSolutionNr = -1;
	    }
	  m->protect();
	  DagNode* target = message->getArgument(1);
	  //
	  //	Find requested solution.
	  //
	  for (; lastSolutionNr < solutionNr; ++lastSolutionNr)
	    {
	      if (!(state->findNextUnifier()))
		{
		  Vector<DagNode*> args(4);
		  args[0] = target;
		  args[1] = message->getArgument(0);
		  args[2] = upRewriteCount(state->getContext());
		  args[3] = metaLevel->upBool(!(state->isIncomplete()));
		  context.addInCount(*(state->getContext()));  // account for any remaining rewrites
		  delete state;
		  (void) m->unprotect();
		  return noSuchResult3Msg->makeDagNode(args);
		}
	    }
	  m->insert(message, state, solutionNr);
	  {
	    RewritingContext* stateContext = state->getContext();
	    //
	    //  op gotNarrowingSearchResult :        Oid Oid RewriteCount Term Type Substitution Qid            Substitution Qid -> Msg .
	    //  op gotNarrowingSearchResultAndPath : Oid Oid RewriteCount Term Type Substitution NarrowingTrace Substitution Qid -> Msg .
	    //                                        0   1       2        3    4        5        6                   7       8
	    //
	    Vector<DagNode*> args(9);
	    //
	    //	The first 3 arguments are the same in both messages.
	    //
	    args[0] = target;
	    args[1] = message->getArgument(0);
	    args[2] = upRewriteCount(stateContext);

	    PointerMap qidMap;
	    PointerMap dagNodeMap;
	    //
	    //	The last 2 arguments are the same in both messages.
	    //
	    const Vector<DagNode*>* unifier = state->getUnifier();
	    args[7] = metaLevel->upSubstitution(*unifier,
						state->getUnifierVariableInfo(),
						unifier->size(),
						m,
						qidMap,
						dagNodeMap);
	    args[8] =
	      metaLevel->upQid(FreshVariableSource::getBaseName(state->getUnifierVariableFamily()),
			       qidMap);
	    //
	    //	The middle 4 arguments differ in semantics between the two messages.
	    //
	    const NarrowingVariableInfo& initialVariableInfo = state->getInitialVariableInfo();
	    if (returnPath)
	      {
		//
		//	In the path case, we return:
		//	(3) the start term with variables replaced by fresh variables from the # family;
		//	(4) the type of this term;
		//	(5) the map from original variables to the fresh variables; and
		//	(6) the sequence of narrowing steps.
		//
		//	The last of these steps will contain the result term with its type,
		//	the accumulated substitution and the fresh variable family used to express it.
		//
		const Substitution* initialRenaming;
		DagNode* initialDag;
		{
		  //
		  //	Initial state isn't produced by narrowing so lacks a context, rule,
		  //	unifier, unifierVariableInfo or parent. We're not interesting in the
		  //	variable family since it will always be #.
		  //
		  DagNode* dummyRoot;
		  DagNode* dummyHole;
		  Rule* dummyRule;
		  const Substitution* dummyUnifier;
		  const NarrowingVariableInfo* dummyUnifierVariableInfo;
		  int dummyVariableFamily;
		  int dummyParentIndex;

		  state->getHistory(0,
				    dummyRoot,
				    dummyHole,
				    dummyRule,
				    dummyUnifier,
				    dummyUnifierVariableInfo,
				    dummyVariableFamily,
				    initialDag,
				    initialRenaming,
				    dummyParentIndex);
		}
		args[3] = metaLevel->upDagNode(initialDag, m, qidMap, dagNodeMap);
		args[4] = metaLevel->upType(initialDag->getSort(), qidMap);
		args[5] = metaLevel->upSubstitution(*initialRenaming,
						    initialVariableInfo,
						    m,
						    qidMap,
						    dagNodeMap);
		args[6] = makeNarrowingSearchPath(m, state);
		(void) m->unprotect();
		return gotNarrowingSearchResultAndPathMsg->makeDagNode(args);
	      }
	    else
	      {
		//
		//	In the no path case, we return:
		//	(3) the result term;
		//	(4) the type of the result term;
		//	(5) the accumulated substitution; and
		//	(6) the fresh variable family used to express the result term and
		//	    accumulated substitution.
		//
		DagNode* stateDag;
		int stateVariableFamily;
		DagNode* dummy;
		Substitution* accumulatedSubstitution;

		state->getStateInfo(stateDag, stateVariableFamily, dummy, accumulatedSubstitution);
		args[3] = metaLevel->upDagNode(stateDag, m, qidMap, dagNodeMap);
		args[4] = metaLevel->upType(stateDag->getSort(), qidMap);
		args[5] = metaLevel->upSubstitution(*accumulatedSubstitution,
						    initialVariableInfo,
						    m,
						    qidMap,
						    dagNodeMap);
		args[6] = metaLevel->upQid(FreshVariableSource::getBaseName(stateVariableFamily),
					   qidMap);
		(void) m->unprotect();
		return gotNarrowingSearchResultMsg->makeDagNode(args);
	      }
	  }
	}
      return errorMessage;
    }
  return makeErrorReply("Bad solution number.", message);
}

DagNode*
InterpreterManagerSymbol::makeNarrowingSearchPath(ImportModule* m, NarrowingSequenceSearch3* state) const
{
  const NarrowingVariableInfo& initialVariableInfo = state->getInitialVariableInfo();
  int index;
  int depth;
  state->getExtraStateInfo(index, depth);

  Vector<DagNode*> narrowingTrace(depth);
  PointerMap qidMap;
  PointerMap dagNodeMap;
  for (int i = depth - 1; i >= 0; --i)
    {
      DagNode* root;
      DagNode* hole;
      Rule* rule;
      const Substitution* unifier;
      const NarrowingVariableInfo* unifierVariableInfo;
      int variableFamily;
      DagNode* newDag;
      const Substitution* accumulatedSubstitution;
      int parentIndex;

      state->getHistory(index,
			root,
			hole,
			rule,
			unifier,
			unifierVariableInfo,
			variableFamily,
			newDag,
			accumulatedSubstitution,
			parentIndex);

      narrowingTrace[i] = metaLevel->upNarrowingStep(root,
						     hole,
						     rule,
						     *unifier,
						     *unifierVariableInfo,
						     FreshVariableSource::getBaseName(variableFamily),
						     newDag,
						     *accumulatedSubstitution,
						     initialVariableInfo,
						     m,
						     qidMap,
						     dagNodeMap);
      index = parentIndex;
    }
  return metaLevel->upNarrowingSearchPath(narrowingTrace);
}
