/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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

void
Interpreter::variantMatch(const Vector<Token>& bubble, Int64 limit, bool debug)
{
  VisibleModule* fm = currentModule->getFlatModule();
  Vector<Term*> lhs;
  Vector<Term*> rhs;
  Vector<Term*> constraint;

  if (!(fm->parseVariantMatchCommand(bubble, lhs, rhs, constraint)))
    return;

  if (getFlag(SHOW_COMMAND))
    {
      UserLevelRewritingContext::beginCommand();
      if (debug)
	cout << "debug ";
      cout << "variant match ";
      if (limit != NONE)
	cout << '[' << limit << "] ";
      cout << "in " << currentModule << " : ";
      int nrPairs = lhs.size();
      for (int i = 0; i < nrPairs; ++i)
	cout << lhs[i] << " <=? " << rhs[i] << ((i == nrPairs - 1) ? " " : " /\\ ");
      if (constraint.empty())
	cout << "." << endl;
      else
	{
	  cout << "such that ";
	  const char* sep = "";
	  for (const Term* t : constraint)
	    {
	      cout << sep << t;
	      sep = ", ";
	    }
	  cout << " irreducible ." << endl;
	}
    }

  startUsingModule(fm);
  FreshVariableGenerator* freshVariableGenerator = new FreshVariableSource(fm);
  if (debug)
    UserLevelRewritingContext::setDebug();

  pair<DagNode*, DagNode*> mp = fm->makeMatchProblemDags(lhs, rhs);
  UserLevelRewritingContext* patternContext = new UserLevelRewritingContext(mp.first);
  UserLevelRewritingContext* subjectContext = new UserLevelRewritingContext(mp.second);

  Vector<DagNode*> blockerDags;
  for (Term* t : constraint)
    {
      t = t->normalize(true);  // we don't really need to normalize but we do need to set hash values
      blockerDags.append(t->term2Dag());
      t->deepSelfDestruct();
    }
  //
  //	Responsibility for deleting patternContext and freshVariableGenerator is passed
  //	to ~VariantSearch().
  //
  Timer timer(getFlag(SHOW_TIMING));
  subjectContext->reduce();
  VariantSearch* vs =
    new VariantSearch(patternContext,
		      blockerDags,
		      freshVariableGenerator,
		      VariantSearch::MATCH_MODE |
		      VariantSearch::DELETE_FRESH_VARIABLE_GENERATOR |
		      VariantSearch::DELETE_LAST_VARIANT_MATCHING_PROBLEM |
		      VariantSearch::CHECK_VARIABLE_NAMES);
  if (vs->problemOK())
    {
      patternContext->addInCount(*subjectContext);
      //
      //	The VariantMatchingProblem we make becomes responsible for deleting
      //	subjectContext.
      //
      (void) vs->makeVariantMatchingProblem(subjectContext);
      //
      //	No more rewriting or narrowing happens after variants have been
      //	generated for pattern so we do this just once.
      //
      printStats(timer, *patternContext, getFlag(SHOW_TIMING));
      doVariantMatching(timer, fm, vs, 0, limit);
    }
  else
    {
      delete vs;
      fm->unprotect();
    }
}

void
Interpreter::doVariantMatching(Timer& timer,
			       VisibleModule* module,
			       VariantSearch* state,
			       Int64 solutionCount,
			       Int64 limit)
{
  RewritingContext* context = state->getContext();
  const NarrowingVariableInfo& variableInfo = state->getVariableInfo();
  VariantMatchingProblem* problem = state->getLastVariantMatchingProblem();
  
  Int64 i = 0;
  for (; i != limit; i++)
    {
      bool moreMatchers = problem->findNextMatcher();
      if (UserLevelRewritingContext::aborted())
	break;
      //
      //	There won't be any narrowing happening to catch a
      //	^C so we check here for safety, though if it does
      //	happen, we can't drop into the debugger and have to
      //	treat it as an abort. We need to bail before outputing
      //	a matcher.
      //
      if (UserLevelRewritingContext::interrupted())
	break;

      if (!moreMatchers)
	{
	  cout << ((solutionCount == 0) ? "\nNo matchers.\n" : "\nNo more matchers.\n");
	  if (problem->freshVariablesNeeded())
	    IssueAdvisory("Non-regular variant equation(s) resulted in fresh variable(s).");
	  if (state->isIncomplete())
	    IssueWarning("Some matchers may have been missed due to incomplete unification algorithm(s).");
	  break;
	}

      const Vector<DagNode*>& matcher = problem->getCurrentMatcher();
      ++solutionCount;
      cout << "\nMatcher " << solutionCount << endl;
      UserLevelRewritingContext::printSubstitution(matcher, variableInfo);
    }
 
  clearContinueInfo();  // just in case debugger left info
  if (i == limit)  
    {
      //
      //	The loop terminated because we hit user's limit so 
      //	continuation is still possible. We save the state,
      //	solutionCount and module, and set a continutation function.
      //
      context->clearCount();
      savedState = state;
      savedSolutionCount = solutionCount;
      savedModule = module;
      continueFunc = &Interpreter::variantMatchCont;
    }
  else
    {
      //
      //	Either user aborted or we ran out of solutions; either
      //	way we need to tidy up.
      //
      delete state;
      module->unprotect();
    }
  UserLevelRewritingContext::clearDebug();
}

void
Interpreter::variantMatchCont(Int64 limit, bool /* debug */)
{
  VariantSearch* state = safeCast(VariantSearch*, savedState);
  VisibleModule* fm = savedModule;
  savedState = 0;
  savedModule = 0;
  continueFunc = 0;
  //
  //	Currently debug mode only affects rewriting and narrowing
  //	and all rewriting and narrowing should have be done before
  //	the first solution was output, so we ignore the debug flag.
  //
  Timer timer(getFlag(SHOW_TIMING));
  doVariantMatching(timer, fm, state, savedSolutionCount, limit);
}

