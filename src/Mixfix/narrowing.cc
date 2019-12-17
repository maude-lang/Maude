/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for narrowing commands.
//

void
Interpreter::doNarrowing(Timer& timer,
			 VisibleModule* module,
			 NarrowingSequenceSearch* state,
			 Int64 solutionCount,
			 Int64 limit)
{
  RewritingContext* context = state->getContext();
  const VariableInfo* variableInfo = state->getGoal();
  Int64 i = 0;
  for (; i != limit; i++)
    {
      bool result = state->findNextMatch();
      if (UserLevelRewritingContext::aborted())
	break;
      if (!result)
	{
	  cout << ((solutionCount == 0) ? "\nNo solution.\n" : "\nNo more solutions.\n");
	  printStats(timer, *context, getFlag(SHOW_TIMING));
	  if (state->isIncomplete())
	    IssueWarning("Some solutions may have been missed due to incomplete unification algorithm(s).");
	  break;
	}

      ++solutionCount;
      cout << "\nSolution " << solutionCount << "\n";
      printStats(timer, *context, getFlag(SHOW_TIMING));

      DagNode* d = state->getStateDag();
      cout << "state: " << d << endl;
      UserLevelRewritingContext::printSubstitution(*(state->getSubstitution()), *variableInfo);
    }
  QUANTIFY_STOP();

  clearContinueInfo();  // just in case debugger left info
  if (i == limit)
    {
      //
      //	The loop terminated because we hit user's limit so 
      //	continuation is still possible. We save the state,
      //	solutionCount and module, and set a continutation function.
      //
      state->getContext()->clearCount();
      savedState = state;
      savedSolutionCount = solutionCount;
      savedModule = module;
      continueFunc = &Interpreter::narrowingCont;
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
Interpreter::narrowingCont(Int64 limit, bool debug)
{
  NarrowingSequenceSearch* state = safeCast(NarrowingSequenceSearch*, savedState);
  VisibleModule* fm = savedModule;
  savedState = 0;
  savedModule = 0;
  continueFunc = 0;

  if (debug)
    UserLevelRewritingContext::setDebug();
  QUANTIFY_START();
  Timer timer(getFlag(SHOW_TIMING));
  doNarrowing(timer, fm, state, savedSolutionCount, limit);
}

void
Interpreter::vuNarrowingCont(Int64 limit, bool debug)
{
  NarrowingSequenceSearch3* state = safeCast(NarrowingSequenceSearch3*, savedState);
  VisibleModule* fm = savedModule;
  savedState = 0;
  savedModule = 0;
  continueFunc = 0;

  if (debug)
    UserLevelRewritingContext::setDebug();
  QUANTIFY_START();
  Timer timer(getFlag(SHOW_TIMING));
  doVuNarrowing(timer, fm, state, savedSolutionCount, limit);
}


void
Interpreter::doVuNarrowing(Timer& timer,
			   VisibleModule* module,
			   NarrowingSequenceSearch3* state,
			   Int64 solutionCount,
			   Int64 limit)
{
  RewritingContext* context = state->getContext();
  Int64 i = 0;
  for (; i != limit; i++)
    {
      bool result = state->findNextUnifier();
      if (UserLevelRewritingContext::aborted())
	break;
      if (!result)
	{
	  cout << ((solutionCount == 0) ? "\nNo solution.\n" : "\nNo more solutions.\n");
	  printStats(timer, *context, getFlag(SHOW_TIMING));
	  if (state->isIncomplete())
	    IssueWarning("Some solutions may have been missed due to incomplete unification algorithm(s).");
	  break;
	}

      ++solutionCount;
      cout << "\nSolution " << solutionCount << "\n";
      printStats(timer, *context, getFlag(SHOW_TIMING));

      DagNode* stateDag;
      int variableFamily;
      Substitution* accumulatedSubstitution;
      state->getStateInfo(stateDag, variableFamily, accumulatedSubstitution);

      cout << "state: " << stateDag << endl;
      cout << "accumulated substitution:" << endl;
      UserLevelRewritingContext::printSubstitution(*accumulatedSubstitution, state->getInitialVariableInfo());
      cout << "variant unifier:" << endl;
      UserLevelRewritingContext::printSubstitution(*(state->getUnifier()), state->getUnifierVariableInfo());
     }
  QUANTIFY_STOP();

  clearContinueInfo();  // just in case debugger left info
  if (i == limit)  // possible to continue
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
      continueFunc = &Interpreter::vuNarrowingCont;
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
