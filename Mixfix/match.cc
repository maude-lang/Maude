/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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
Interpreter::printMatchTiming(const Timer& timer)
{
  Int64 real;
  Int64 virt;
  Int64 prof;
  if (getFlag(SHOW_STATS) && getFlag(SHOW_TIMING) &&
      timer.isRunning() && timer.getTimes(real, virt, prof))
    {
      cout << "Decision time: " << prof / 1000 << "ms cpu (" <<
	real / 1000 << "ms real)\n";
    }
}

// #include "lhsAutomaton.hh"

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

  if (getFlag(SHOW_COMMAND))
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
      cout << " .\n";
    }

  // pattern->getLhsAutomaton()->dump(cerr, *pattern, 0);

  startUsingModule(fm);
  UserLevelRewritingContext* context = new UserLevelRewritingContext(subjectDag);

#ifdef QUANTIFY_REWRITING
  quantify_start_recording_data();
#endif

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
      if (!result)
	{
	  if (solutionCount == 0)
	    {
	      printMatchTiming(timer);
	      cout << "No match.\n";
	    }
	  break;
	}

      ++solutionCount;
      if (solutionCount == 1)
	printMatchTiming(timer);
      cout << "\nSolution " << solutionCount << '\n';
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
      if (UserLevelRewritingContext::interrupted())
	break;
    }

#ifdef QUANTIFY_REWRITING
  quantify_stop_recording_data();
#endif

  clearContinueInfo();  // just in case debugger left info
  state->getContext()->clearCount();
  if (i == limit)  // possible to continue
    {
      savedMatchSearchState = state;
      savedSolutionCount = solutionCount;
      savedModule = module;
      continueFunc = &Interpreter::matchCont;
    }
  else  // tidy up
    {
      delete state;
      module->unprotect();
    }
  UserLevelRewritingContext::clearDebug();
  MemoryCell::okToCollectGarbage();
}

void
Interpreter::matchCont(Int64 limit, bool /* debug */)
{
  MatchSearchState* state = savedMatchSearchState;
  VisibleModule* fm = savedModule;
  savedMatchSearchState = 0;
  savedModule = 0;
  continueFunc = 0;

#ifdef QUANTIFY_REWRITING
  quantify_start_recording_data();
#endif

  Timer timer(getFlag(SHOW_TIMING));
  doMatching(timer, fm, state, savedSolutionCount, limit);
}
