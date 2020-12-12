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
//	MetaInterpreters: getVariantMatcher() message.
//

DagNode*
InterpreterManagerSymbol::getVariantMatcher(FreeDagNode* message,
					    ObjectSystemRewritingContext& context,
					    Interpreter* interpreter)
{
  //
  //	op getVariantMatcher : Oid Oid Qid MatchingProblem TermList Qid VariantOptionSet Nat -> Msg .
  //                            0   1   2         3           4      5         6          7
  //
  Int64 solutionNr;
  if (metaLevel->downSaturate64(message->getArgument(7), solutionNr) && solutionNr >= 0)
    {
      DagNode* errorMessage;
      if (MetaModule* mm = getMetaModule(message, 2, interpreter, errorMessage))
	{
	  VariantSearch* vs;
	  Int64 lastSolutionNr;
	  if (mm->getCachedStateObject(message, solutionNr, vs, lastSolutionNr))
	    mm->protect();  // use cached state so protect the module
	  else
	    {
	      //
	      //	No cached variant search so make one.
	      //
	      int variableFamilyName;
	      if (!(metaLevel->downQid(message->getArgument(5), variableFamilyName)))
		return makeErrorReply("Bad variable family.", message);
	      int variableFamily = FreshVariableSource::getFamily(variableFamilyName);
	      if (variableFamily == NONE)
		return makeErrorReply("Bad variable family.", message);
 
	      int variantFlags;
	      if (!(metaLevel->downVariantOptionSet(message->getArgument(6), variantFlags) &&
		    variantFlags == 0))
		return makeErrorReply("Bad option.", message);
	      //
	      //	Pull down blocker terms.
	      //
	      Vector<Term*> blockerTerms;
	      if (!(metaLevel->downTermList(message->getArgument(4), mm, blockerTerms)))
		return makeErrorReply("Bad reducibility constraint.", message);
	      //
	      //	Convert blocker terms to blocker dags.
	      //
	      Vector<DagNode*> blockerDags;
	      for (Term* t : blockerTerms)
		{
		  //
		  //	We don't really need to normalize but we do need to
		  //	set hash values.
		  //
		  t = t->normalize(true);
		  blockerDags.append(t->term2Dag());
		  t->deepSelfDestruct();
		}
	      //
	      //	Pull down matching problem.
	      //
	      Vector<Term*> patterns;
	      Vector<Term*> subjects;
	      if (!metaLevel->downMatchingProblem(message->getArgument(3), patterns, subjects, mm))
		return makeErrorReply("Bad matching problem.", message);
	      pair<DagNode*, DagNode*> mp = mm->makeMatchProblemDags(patterns, subjects);
	      RewritingContext* patternContext =
		context.makeSubcontext(mp.first, UserLevelRewritingContext::META_EVAL);
	      RewritingContext* subjectContext =
		context.makeSubcontext(mp.second, UserLevelRewritingContext::META_EVAL);
	      //
	      //	reduce() and VariantSearch() will do rewriting so we need to
	      //	protect the module from being overwritten in the debugger.
	      //
	      mm->protect();
	      subjectContext->reduce();
	      FreshVariableGenerator* freshVariableGenerator = new FreshVariableSource(mm);
	      vs = new VariantSearch(patternContext,
				     blockerDags,
				     freshVariableGenerator,
				     VariantSearch::MATCH_MODE |
				     VariantSearch::DELETE_FRESH_VARIABLE_GENERATOR |
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

	  DagNode* target = message->getArgument(1);
	  //
	  //	Now we compute the requested variant matcher.
	  //
	  VariantMatchingProblem* problem = vs->getLastVariantMatchingProblem();
	  for (; lastSolutionNr < solutionNr; ++lastSolutionNr)
	    {
	      if (!(problem->findNextMatcher()))
		{
		  Vector<DagNode*> args(4);
		  args[0] = target;
		  args[1] = message->getArgument(0);
		  args[2] = upRewriteCount(vs->getContext());
		  args[3] = metaLevel->upBool(!(vs->isIncomplete()));
		  context.addInCount(*(vs->getContext()));
		  delete vs;
		  (void) mm->unprotect();
		  return noSuchResult3Msg->makeDagNode(args);
		}
	    }
	  //
	  //	Got the solution we wanted so cache the state.
	  //
	  mm->insert(message, vs, solutionNr);
	  {
	    const Vector<DagNode*>& matcher = problem->getCurrentMatcher();
	    const NarrowingVariableInfo& variableInfo = vs->getVariableInfo();
	    PointerMap qidMap;
	    PointerMap dagNodeMap;
	    Vector<DagNode*> args(4);
	    args[0] = target;
	    args[1] = message->getArgument(0);
	    args[2] = upRewriteCount(vs->getContext());
	    args[3] = metaLevel->upSubstitution(matcher,
						variableInfo,
						matcher.size(),
						mm,
						qidMap,
						dagNodeMap);
	    context.transferCountFrom(*(vs->getContext()));
	    (void) mm->unprotect();
	    return gotVariantMatcherMsg->makeDagNode(args);
	  }
	}
      return errorMessage;
    }
  return makeErrorReply("Bad solution number.", message);
}
