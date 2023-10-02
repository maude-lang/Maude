/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for match and xmatch commands.
//

void
Interpreter::printDecisionTime(const Timer& timer)
{
  Int64 real;
  Int64 virt;
  Int64 prof;
  if (getFlag(SHOW_STATS) && getFlag(SHOW_TIMING) && timer.isRunning() && timer.getTimes(real, virt, prof))
    {
      cout << "Decision time: " << prof / 1000 << "ms cpu (" << real / 1000 << "ms real)\n";
      if (latexBuffer)
	latexBuffer->generateDecisionTime(prof, real);
    }
}

void
Interpreter::match(const Vector<Token>& bubble, bool withExtension, Int64 limit)
{
  VisibleModule* fm = currentModule->getFlatModule();
  Term* patternTerm;
  Term* subjectTerm;
  Vector<ConditionFragment*> condition;
  if (!(fm->parseMatchCommand(bubble, patternTerm, subjectTerm, condition)))
    return;

  Pattern* pattern = new Pattern(patternTerm, withExtension, condition);
  if (!(pattern->getUnboundVariables().empty()))
    {
      IssueWarning(*subjectTerm << ": variable " <<
		   QUOTE(pattern->index2Variable(pattern->getUnboundVariables().min())) <<
		   " is used before it is bound in condition of match command.");
      subjectTerm->deepSelfDestruct();
      delete pattern;
      return;
    }
  DagNode* subjectDag = makeDag(subjectTerm);

  bool showCommand = getFlag(SHOW_COMMAND);
  if (showCommand)
    {
      UserLevelRewritingContext::beginCommand();
      if (withExtension)
	cout << 'x';
      cout << "match ";
      if (limit != NONE)
	cout << '[' << limit << "] ";
      cout << "in " << currentModule << " : " << pattern->getLhs() <<
	" <=? " << subjectDag;
      if (condition.length() > 0)
	{
	  cout << " such that ";
	  MixfixModule::printCondition(cout, condition);
	}
      cout << " ." << endl;
    }
  if (latexBuffer != 0)
    {
      latexBuffer->generateMatch(showCommand,
				 withExtension,
				 pattern->getLhs(),
				 subjectDag,
				 condition,
				 limit);
    }

  startUsingModule(fm);
  UserLevelRewritingContext* context = new UserLevelRewritingContext(subjectDag);

  QUANTIFY_START();
  Timer timer(getFlag(SHOW_TIMING));
  subjectDag->computeTrueSort(*context);
  if (UserLevelRewritingContext::aborted())
    {
      delete context;
      delete pattern;
      fm->unprotect();
      return;
    }

  MatchSearchState* state = 
    new MatchSearchState(context,
			 pattern,
			 MatchSearchState::GC_PATTERN | MatchSearchState::GC_CONTEXT,
			 0,
			 withExtension ? 0 : NONE);

  doMatching(timer, fm, state, 0, limit);
}

void
Interpreter::doMatching(Timer& timer,
			VisibleModule* module,
			MatchSearchState* state,
			int solutionCount,
			int limit)
{
  RewritingContext* context = state->getContext();
  VariableInfo* variableInfo = state->getPattern();
  int i = 0;
  for (; i != limit; i++)
    {
      bool result = state->findNextMatch();
      if (UserLevelRewritingContext::aborted())
	break;
      //
      //	There might not be any rewriting happening to catch a
      //	^C so we check here for safety, though if it does
      //	happen, we can't drop into the debugger and have to
      //	treat it as an abort. We need to bail before outputing
      //	a matcher.
      //
      //	If there is rewriting happening to resolve a condition
      //	then we have a race condition and whether we drop into
      //	the debugger or just abort depends on the instant the ^C
      //	interrupt arrives. We tolerate this uncertainty because
      //	having Maude ignore ^C while spewing thousands of
      //	matchers that won't print correctly would be worse.
      //
      if (UserLevelRewritingContext::interrupted())
	break;

      if (!result)
	{
	  if (solutionCount == 0)
	    {
	      printDecisionTime(timer);
	      cout << "No match.\n";
	      if (latexBuffer)
		latexBuffer->generateNonResult("No match."); 
	    }
	  break;
	}

      ++solutionCount;
      if (solutionCount == 1)
	printDecisionTime(timer);
      cout << "\nMatcher " << solutionCount << '\n';
      ExtensionInfo* extensionInfo = state->getExtensionInfo();
      if (extensionInfo != 0)
	{
	  cout << "Matched portion = ";
	  if (extensionInfo->matchedWhole())
	    cout << "(whole)\n";
	  else
	    cout << extensionInfo->buildMatchedPortion() << '\n';
	}
      UserLevelRewritingContext::printSubstitution(*context, *variableInfo);
      if (latexBuffer)
	{
	  latexBuffer->generateMatchResult(state, solutionCount);
	  latexBuffer->generateSubstitution(*context, *variableInfo);
	}
    }
  QUANTIFY_STOP();

  if (latexBuffer)
    latexBuffer->cleanUp();
  clearContinueInfo();  // just in case debugger left info
  state->getContext()->clearCount();
  if (i == limit)
    {
      //
      //	The loop terminated because we hit user's limit so 
      //	continuation is still possible. We save the state,
      //	solutionCount and module, and set a continutation function.
      //
      savedState = state;
      savedSolutionCount = solutionCount;
      savedModule = module;
      continueFunc = &Interpreter::matchCont;
    }
  else
    {
      //
      //	Either user aborted or interrupted or we ran out of solutions;
      //	either way we need to tidy up.
      //
      delete state;
      module->unprotect();
    }
  UserLevelRewritingContext::clearDebug();
  MemoryCell::okToCollectGarbage();
}

void
Interpreter::matchCont(Int64 limit, bool /* debug */)
{
  MatchSearchState* state = safeCast(MatchSearchState*, savedState); 
  VisibleModule* fm = savedModule;
  savedState = 0;
  savedModule = 0;
  continueFunc = 0;
  if (latexBuffer)
    latexBuffer->generateContinue(getFlag(SHOW_COMMAND), limit);

  QUANTIFY_START();
  Timer timer(getFlag(SHOW_TIMING));
  doMatching(timer, fm, state, savedSolutionCount, limit);
}
