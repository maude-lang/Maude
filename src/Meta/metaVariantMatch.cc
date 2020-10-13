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

bool
MetaLevelOpSymbol::metaVariantMatch(FreeDagNode* subject, RewritingContext& context)
{
  DebugEnter("subject = " << subject);
  //
  //	op metaVariantMatch : Module MatchingProblem TermList Qid VariantOptionSet Nat ~> Substitution? .
  //	                        0          1            2      3        4           5
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      if (metaLevel->downSaturate64(subject->getArgument(5), solutionNr) && solutionNr >= 0)
	{
	  VariantSearch* vs;
	  Int64 lastSolutionNr;
	  if (m->getCachedStateObject(subject, context, solutionNr, vs, lastSolutionNr))
	    m->protect();  // Use cached state
	  else
	    {
	      int variableFamilyName;
	      if (!(metaLevel->downQid(subject->getArgument(3), variableFamilyName)))
		return false;
	      int variableFamily = FreshVariableSource::getFamily(variableFamilyName);
	      if (variableFamily == NONE)
		return false;
  
	      int variantFlags;
	      if (!(metaLevel->downVariantOptionSet(subject->getArgument(4), variantFlags)) ||
		  variantFlags != 0)
		return false;
  
	      Vector<Term*> patterns;
	      Vector<Term*> subjects;
	      if (!metaLevel->downMatchingProblem(subject->getArgument(1), patterns, subjects, m))
		return false;

	      Vector<Term*> blockerTerms;
	      if (!metaLevel->downTermList(subject->getArgument(2), m, blockerTerms))
		{
		  for (Term* t : patterns)
		    t->deepSelfDestruct();
		  for (Term* t : subjects)
		    t->deepSelfDestruct();
		  return false;
		}

	      m->protect();
	      pair<DagNode*, DagNode*> mp = m->makeMatchProblemDags(patterns, subjects);
	      RewritingContext* patternContext =
		context.makeSubcontext(mp.first, UserLevelRewritingContext::META_EVAL);
	      RewritingContext* subjectContext =
		context.makeSubcontext(mp.second, UserLevelRewritingContext::META_EVAL);

	      Vector<DagNode*> blockerDags;
	      for (Term* t : blockerTerms)
		{
		  //
		  //	We don't really need to normalize but we do need to set hash values.
		  //
		  t = t->normalize(true);
		  blockerDags.append(t->term2Dag());
		  t->deepSelfDestruct();
		}
  
	      FreshVariableGenerator* freshVariableGenerator = new FreshVariableSource(m);
	      subjectContext->reduce();
	      vs = new VariantSearch(patternContext,
				     blockerDags,
				     freshVariableGenerator,
				     VariantSearch::MATCH_MODE |
				     VariantSearch::DELETE_FRESH_VARIABLE_GENERATOR |
				     VariantSearch::DELETE_LAST_VARIANT_MATCHING_PROBLEM |
				     VariantSearch::CHECK_VARIABLE_NAMES,
				     variableFamily);
	      patternContext->addInCount(*subjectContext);
	      //
	      //	The VariantMatchingProblem we make becomes responsible for deleting
	      //	subjectContext.
	      //
	      (void) vs->makeVariantMatchingProblem(subjectContext);
	      lastSolutionNr = -1;
	    }

	  VariantMatchingProblem* problem = vs->getLastVariantMatchingProblem();
	  DagNode* result;
	  for (; lastSolutionNr < solutionNr; ++lastSolutionNr)
	    {
	      if (!(problem->findNextMatcher()))
		{
		  bool incomplete = vs->isIncomplete();
		  delete vs;
		  result = metaLevel->upNoMatchSubst(incomplete);
		  goto fail;
		}
	      context.transferCountFrom(*(vs->getContext()));
	    }
	  m->insert(subject, vs, solutionNr);
	  {
	    const Vector<DagNode*>& matcher = problem->getCurrentMatcher();
	    const NarrowingVariableInfo& variableInfo = vs->getVariableInfo();
	    PointerMap qidMap;
	    PointerMap dagNodeMap;
	    result = metaLevel->upSubstitution(matcher,
					       variableInfo,
					       matcher.size(),
					       m,
					       qidMap,
					       dagNodeMap);
	  }
   
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}
