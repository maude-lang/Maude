/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for metaNarrowingSearch() and metaNarrowingSearchPath() descent functions.
//

NarrowingSequenceSearch3*
MetaLevelOpSymbol::makeNarrowingSequenceSearch3(MetaModule* m,
						FreeDagNode* subject,
						RewritingContext& context,
						int variantFlags) const
{
  RewriteSequenceSearch::SearchType searchType;
  bool fold;
  int maxDepth;
  if (metaLevel->downSearchType(subject->getArgument(3), searchType) &&
      metaLevel->downFoldType(subject->getArgument(5), fold) &&
      metaLevel->downBound(subject->getArgument(4), maxDepth))
    {
      Term* s;
      Term* g;
      if (metaLevel->downTermPair(subject->getArgument(1), subject->getArgument(2), s, g, m))
	{
	  m->protect();

	  RewritingContext* subjectContext = term2RewritingContext(s, context);
	  g = g->normalize(true);
	  DagNode* goal = g->term2Dag();
	  g->deepSelfDestruct();

	  if (fold)
	    variantFlags |= NarrowingSequenceSearch3::FOLD;
	  return new NarrowingSequenceSearch3(subjectContext,
					      searchType,
					      goal,
					      maxDepth,
					      new FreshVariableSource(m, 0),
					      variantFlags);
	}
    }
  return 0;
}

bool
MetaLevelOpSymbol::metaNarrowingSearch(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaNarrowingSearch : Module Term Term Qid Bound Qid VariantOptionSet Nat ~>
  //	                         NarrowingSearchResult? .
  //
  //	Arguments:
  //	  Module to work in
  //	  Term to narrow (after reducing)
  //	  Term that is pattern to be reached
  //	  Qid giving search type
  //	  Bound on depth of search
  //	  Qid giving folding strategy
  //	  VariantOptionSet giving options to be used for variant unification
  //	  Nat giving which of many solutions is wanted
  //
  //	A successful narrowing application yields a 6-tuple:
  //	  op [_,_,_,_,_,_] : Term Type Substitution Qid Substitution Qid -> NarrowingSearchResult [ctor] .
  //	where the arguments are:
  //	  Final term after narrowing and reducing
  //	  Type of Term
  //	  Accumuated Substitution
  //	  Qid giving fresh variable family used to express result Term and accumulated Substitution
  //	  Unifier between narrowed term and instantiated pattern
  //	  Qid giving fresh variable family used to express unifier
  //
  int variantFlags;
  Int64 solutionNr;
  if (metaLevel->downVariantOptionSet(subject->getArgument(6), variantFlags) &&
      (variantFlags & ~(MetaLevel::DELAY | MetaLevel::FILTER)) == 0 &&
      metaLevel->downSaturate64(subject->getArgument(7), solutionNr) &&
      solutionNr >= 0)
    {
      if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
	{
	  NarrowingSequenceSearch3* state;
	  Int64 lastSolutionNr;
	  if (m->getCachedStateObject(subject, context, solutionNr, state, lastSolutionNr))
	    m->protect(); 
	  else if ((state = makeNarrowingSequenceSearch3(m, subject, context, variantFlags)))
	    lastSolutionNr = -1;
	  else
	    return false;

	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextUnifier();
	      context.transferCountFrom(*(state->getContext()));
	      if (!success)
		{
		  result = metaLevel->upNarrowingSearchFailure(state->isIncomplete());
		  delete state;
		  goto fail;
		}
	      ++lastSolutionNr;
	    }
	  m->insert(subject, state, solutionNr);

	  DagNode* stateDag;
	  int stateVariableFamily;
	  Substitution* accumulatedSubstitution;
	  state->getStateInfo(stateDag, stateVariableFamily, accumulatedSubstitution);

	  result = metaLevel->upNarrowingSearchResult(stateDag,
						      *accumulatedSubstitution,
						      state->getInitialVariableInfo(),
						      FreshVariableSource::getBaseName(stateVariableFamily),
						      *(state->getUnifier()),
						      state->getUnifierVariableInfo(),
						      FreshVariableSource::getBaseName(state->getUnifierVariableFamily()),
						      m);
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}


bool
MetaLevelOpSymbol::metaNarrowingSearchPath(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaNarrowingSearchPath : Module Term Term Qid Bound Qid VariantOptionSet Nat ~>
  //	                             NarrowingSearchPathResult? .
  //
  //	Arguments:
  //	  Module to work in
  //	  Term to narrow (after reducing)
  //	  Term that is pattern to be reached
  //	  Qid giving search type
  //	  Bound on depth of search
  //	  Qid giving folding strategy
  //	  VariantOptionSet giving options to be used for variant unification
  //	  Nat giving which of many solutions is wanted
  //
  //	A successful narrowing application yields a 6-tuple:
  //	  op [_,_,_,_,_,_] : Term Type Substitution NarrowingTrace Substitution Qid ->
  //	                     NarrowingSearchPathResult [ctor] .
  //	where the arguments are:
  //	  Initial term after renaming and reducing
  //	  Type of term
  //	  Substitution corresponding to initial renaming
  //	  Trace of narrowing steps
  //	  Unifier between narrowed term and instantiated pattern
  //	  Qid giving fresh variable family used to express unifier
  //
  //	The trace is a list of narrowing steps:
  //	  op [_,_,_,_, _,_,_] : Context Qid Substitution Qid Term Type Substitution ->
  //	                        NarrowingStep [ctor] .
  //	where the arguments are:
  //	  Context where narrowing happens
  //	  Label of rule used for narrowing
  //	  Variant unifier used for narrowing
  //	  Qid giving fresh variable family used to express unifier
  //	  New term (after reducing)
  //	  Type of term
  //	  Accumulated substitution
  //
  int variantFlags;
  Int64 solutionNr;
  if (metaLevel->downVariantOptionSet(subject->getArgument(6), variantFlags) &&
      (variantFlags & ~(MetaLevel::DELAY | MetaLevel::FILTER)) == 0 &&
      metaLevel->downSaturate64(subject->getArgument(7), solutionNr) &&
      solutionNr >= 0)
    {
      if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
	{
	  NarrowingSequenceSearch3* state;
	  Int64 lastSolutionNr;
	  if (m->getCachedStateObject(subject, context, solutionNr, state, lastSolutionNr))
	    m->protect();
	  else if ((state = makeNarrowingSequenceSearch3(m,
							 subject,
							 context,
							 variantFlags |
							 NarrowingSequenceSearch3::KEEP_HISTORY)))
	    lastSolutionNr = -1;
	  else
	    return false;

	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextUnifier();
	      context.transferCountFrom(*(state->getContext()));
	      if (!success)
		{
		  result = metaLevel->upNarrowingSearchPathFailure(state->isIncomplete());
		  delete state;
		  goto fail;
		}
	      ++lastSolutionNr;
	    }
	  m->insert(subject, state, solutionNr);
	  result = makeNarrowingSearchPathResult(m, state);

	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

DagNode*
MetaLevelOpSymbol::makeNarrowingSearchPathResult(MetaModule* m, NarrowingSequenceSearch3* state) const
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

  const Substitution* initialRenaming;
  DagNode* initialDag;
  {
    //
    //	Initial state isn't produced by narrowing so lacks a context, rule,
    //	unifier, unifierVariableInfo or parent. We're not interesting in the variable family since
    //	it will always be #.
    //
    DagNode* dummyRoot;
    DagNode* dummyHole;
    Rule* dummyRule;
    const Substitution* dummyUnifier;
    const NarrowingVariableInfo* dummyUnifierVariableInfo;
    int dummyVariableFamily;
    int dummyParentIndex;

    state->getHistory(index,
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
  return  metaLevel->upNarrowingSearchPathResult(initialDag,
						 *initialRenaming,
						 initialVariableInfo,
						 narrowingTrace,
						 *(state->getUnifier()),
						 state->getUnifierVariableInfo(),
						 FreshVariableSource::getBaseName(state->getUnifierVariableFamily()),
						 m,
						 qidMap,
						 dagNodeMap);
}
