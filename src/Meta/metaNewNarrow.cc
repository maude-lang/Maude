/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2017 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for metaNarrowingApply() descent function.
//

NarrowingSearchState2*
MetaLevelOpSymbol::makeNarrowingSearchState2(MetaModule* m,
					     FreeDagNode* subject,
					     RewritingContext& context) const
{
  int variableFamilyName;
  if (metaLevel->downQid(subject->getArgument(3), variableFamilyName))
    {
      int variableFamily = FreshVariableSource::getFamily(variableFamilyName);
      if (variableFamily == NONE)
	return 0;

      if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
	{
	  Vector<Term*> blockerTerms;
	  if (!metaLevel->downTermList(subject->getArgument(2), m, blockerTerms))
	    {
	      t->deepSelfDestruct();
	      return 0;
	    }
	  m->protect();

	  RewritingContext* subjectContext = term2RewritingContext(t, context);
	  subjectContext->reduce();
	  context.addInCount(*subjectContext);

	  Vector<DagNode*> blockerDags; 
	  FOR_EACH_CONST(i, Vector<Term*>, blockerTerms)
	    {
	      Term* t = *i;
	      t = t->normalize(true);  // we don't really need to normalize but we do need to set hash values
	      blockerDags.append(t->term2Dag());
	      t->deepSelfDestruct();
	    }

	  return new NarrowingSearchState2(subjectContext,
					   blockerDags,
					   new FreshVariableSource(m, 0),
					   variableFamily);
	}
    }
  return 0;
}

bool
MetaLevelOpSymbol::metaNarrowingApply(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaNarrowingApply : Module Term TermList Qid Nat -> NarrowingResult?
  //
  //	Arguments:
  //	  Module to work in
  //	  Term to narrow (after reducing)
  //	  TermList of blocker terms for variant unification
  //	  Qid giving fresh variable family that might appear in Term (and which will be avoided for result)
  //	  Nat giving which of many solutions is wanted
  //
  //	A successful narrowing application yields a 7-tuple:
  //	  op {_,_,_,_,_,_,_} : Term Type Context Qid Substitution Substitution Qid -> NarrowingResult [ctor] .
  //	where the arguments are:
  //	  Term after narrowing and reducing
  //	  Type of Term
  //	  Context in the original term where narrowing took place
  //	  Qid giving label of rule used
  //	  Substitution into the original term
  //	  Substitution into the rule used for narrowing
  //	  Qid giving fresh variable family used to express result Term and Substitutions
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      if (metaLevel->downSaturate64(subject->getArgument(4), solutionNr) &&
	  solutionNr >= 0)
	{
	  NarrowingSearchState2 *state;
	  Int64 lastSolutionNr;
	  if (m->getCachedStateObject(subject, context, solutionNr, state, lastSolutionNr))
	    m->protect(); 
	  else if ((state = makeNarrowingSearchState2(m, subject, context)))
	    lastSolutionNr = -1;
	  else
	    return false;

	  DagNode* result;
	  bool actualStep = lastSolutionNr < solutionNr;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextNarrowing();
	      context.transferCountFrom(*(state->getContext()));
	      if (!success)
		{
		  result = metaLevel->upNarrowingApplyFailure(state->isIncomplete());
		  delete state;
		  goto fail;
		}
	      ++lastSolutionNr;
	    }
	  m->insert(subject, state, solutionNr);
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
	    if (actualStep)
	      {
		context.incrementNarrowingCount();
		if (RewritingContext::getTraceStatus())
		  {
		    //
		    //	We pass the active term and variable info. This means
		    //	the version with (potentially) renamed variables. We do this for
		    //	consistancy since the replaced dag will be expressed in those
		    //	same variables.
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
			(void) m->unprotect();
			return false;
		      }
		  }
	      }
	    //
	    //	Can't use dagNodeMap after reduce since the from dagNodes might be
	    //	garbage collected or even rewritten in place.
	    //
	    PointerMap qidMap;
	    PointerMap dagNodeMap;
	    DagRoot metaContext(metaLevel->upContext(replacementContext, m, hole, qidMap, dagNodeMap));
	    //
	    //	Need to reduce narrowedDag.
	    //
	    RewritingContext* resultContext =
	      context.makeSubcontext(narrowedDag, UserLevelRewritingContext::META_EVAL);
	    resultContext->reduce();
	    context.addInCount(*resultContext);

	    result = metaLevel->upNarrowingApplyResult(resultContext->root(),
						       metaContext.getNode(),
						       unifier,
						       state->getRule(),
						       state->getVariableInfo(),
						       FreshVariableSource::getBaseName(state->getVariableFamily()),
						       m);
	    delete resultContext;
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}
