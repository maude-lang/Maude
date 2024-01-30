/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017-2024 SRI International, Menlo Park, CA 94025, USA.

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
Interpreter::variantUnify(const Vector<Token>& bubble, Int64 limit, bool filtered, bool debug)
{
  VisibleModule* fm = currentModule->getFlatModule();
  Vector<Term*> lhs;
  Vector<Term*> rhs;
  Vector<Term*> constraint;
  if (!(fm->parseVariantUnifyCommand(bubble, lhs, rhs, constraint)))
    return;

  bool showCommand = getFlag(SHOW_COMMAND);
  if (showCommand)
    {
      UserLevelRewritingContext::beginCommand();
      if (debug)
	cout << "debug ";
      if (filtered)
	cout << "filtered ";
      cout << "variant unify ";
      if (limit != NONE)
	cout << '[' << limit << "] ";
      cout << "in " << currentModule << " : ";
      int nrPairs = lhs.size();
      for (int i = 0; i < nrPairs; ++i)
	cout << lhs[i] << " =? " << rhs[i] << ((i == nrPairs - 1) ? " " : " /\\ ");
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
  if (latexBuffer != 0)
    {
      latexBuffer->generateVariantUnify(showCommand,
					filtered,
					lhs,
					rhs,
					constraint,
					limit,
					debug);
    }

  startUsingModule(fm);
  FreshVariableGenerator* freshVariableGenerator = new FreshVariableSource(fm);
  if (debug)
    UserLevelRewritingContext::setDebug();

  DagNode* d = fm->makeUnificationProblemDag(lhs, rhs);
  UserLevelRewritingContext* context = new UserLevelRewritingContext(d);

  Vector<DagNode*> blockerDags;
  for (Term* t : constraint)
    {
      t = t->normalize(true);  // we don't really need to normalize but we do need to set hash values
      blockerDags.append(t->term2Dag());
      t->deepSelfDestruct();
    }
  //
  //	Responsibility for deleting context and freshVariableGenerator is passed to ~VariantSearch().
  //
  bool showTiming = getFlag(SHOW_TIMING);
  Timer timer(showTiming);
  VariantSearch* vs = filtered ?
    new FilteredVariantUnifierSearch(context,
				     blockerDags,
				     freshVariableGenerator,
				     VariantSearch::IRREDUNDANT_MODE |
				     VariantSearch::DELETE_FRESH_VARIABLE_GENERATOR |
				     VariantSearch::CHECK_VARIABLE_NAMES) :
    new VariantSearch(context,
		      blockerDags,
		      freshVariableGenerator,
		      VariantSearch::UNIFICATION_MODE |
		      VariantSearch::DELETE_FRESH_VARIABLE_GENERATOR |
		      VariantSearch::CHECK_VARIABLE_NAMES);
  if (vs->problemOK())
    {
      if (filtered)
	{
	  //
	  //	All computation is done up-front so there is only one time value.
	  //
	  printStats(timer, *context, showTiming);
	}
      doVariantUnification(timer, fm, vs, 0, limit);
    }
  else
    {
      delete vs;
      fm->unprotect();
    }
}

void
Interpreter::doVariantUnification(Timer& timer,
				  VisibleModule* module,
				  VariantSearch* state,
				  Int64 solutionCount,
				  Int64 limit)
{
  RewritingContext* context = state->getContext();
  const NarrowingVariableInfo& variableInfo = state->getVariableInfo();
  FilteredVariantUnifierSearch* filteredState = dynamic_cast<FilteredVariantUnifierSearch*>(state);

  Int64 i = 0;
  for (; i != limit; i++)
    {
      bool moreUnifiers = state->findNextUnifier();
      if (UserLevelRewritingContext::aborted())
	break;
      //
      //	There might not be any narrowing happening to catch a
      //	^C so we check here for safety, though if it does
      //	happen, we can't drop into the debugger and have to
      //	treat it as an abort. We need to bail before outputing
      //	a unifier.
      //
      //	If there is narrowing happening (because there are variant
      //	equations) then we have a race condition and whether we drop
      //	into the debugger or just abort depends on the instant the ^C
      //	interrupt arrives. We tolerate this uncertainty because
      //	having Maude ignore ^C while spewing thousands of
      //	variant unifiers that won't print correctly would be worse.
      //
      if (UserLevelRewritingContext::interrupted())
	break;

      if (!moreUnifiers)
	{
	  const char* message = (solutionCount == 0) ? "No unifiers." : "No more unifiers.";
	  cout << '\n' <<  message << '\n';
	  if (latexBuffer)
	    latexBuffer->generateNonResult(message);
	  if (!filteredState)
	    printStats(timer, *context, getFlag(SHOW_TIMING));
	  //
	  //	Check for incompleteness.
	  //
	  if (state->isIncomplete())
	    {
	      const char* message = "Some unifiers may have been missed due to incomplete unification algorithm(s).";
	      IssueWarning(message);
	      if (latexBuffer)
		latexBuffer->generateWarning(message);
	    }
	  //
	  //	Check for incomplete filtering.
	  //
	  if (filteredState)
	    {
	      if (filteredState->filteringIncomplete())
		{
		  const char* message = "Filtering was incomplete due to incomplete unification algorithm(s).";
		  IssueWarning(message);
		  if (latexBuffer)
		    latexBuffer->generateWarning(message);
		}
	      else
		{
		  const char* message = "Filtering was complete.";
		  IssueAdvisory(message);
		  if (latexBuffer)
		    latexBuffer->generateAdvisory(message);
		}
	    }
	  break;
	}

      int nrFreeVariables;
      int variableFamily;
      const Vector<DagNode*>& unifier = state->getCurrentUnifier(nrFreeVariables, variableFamily);
      ++solutionCount;
      cout << "\nUnifier " << solutionCount << endl;
      if (latexBuffer)
	latexBuffer->generateResult("Unifier", solutionCount);
      if (!filteredState)
	printStats(timer, *context, getFlag(SHOW_TIMING));
      UserLevelRewritingContext::printSubstitution(unifier, variableInfo);
      if (latexBuffer)
	latexBuffer->generateSubstitution(unifier, variableInfo);
    }
 
  if (latexBuffer)
    latexBuffer->cleanUp();
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
      continueFunc = &Interpreter::variantUnifyCont;
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
Interpreter::variantUnifyCont(Int64 limit, bool debug)
{
  VariantSearch* state = safeCast(VariantSearch*, savedState);
  VisibleModule* fm = savedModule;
  savedState = 0;
  savedModule = 0;
  continueFunc = 0;
  if (latexBuffer)
    latexBuffer->generateContinue(getFlag(SHOW_COMMAND), limit, debug);
  if (debug)
    UserLevelRewritingContext::setDebug();
  Timer timer(getFlag(SHOW_TIMING));
  doVariantUnification(timer, fm, state, savedSolutionCount, limit);
}
