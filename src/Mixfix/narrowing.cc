/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017-2023 SRI International, Menlo Park, CA 94025, USA.

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
      Int64 real = 0;
      Int64 virt = 0;
      Int64 prof = 0;
      bool showTiming = getFlag(SHOW_TIMING) && timer.getTimes(real, virt, prof);
      if (!result)
	{
	  const char* reply = (solutionCount == 0) ? "No solution." : "No more solutions.";
	  cout << "\n" << reply << endl;
	  printStats(*context, prof, real, showTiming);
	  if (state->isIncomplete())
	    IssueWarning("Some solutions may have been missed due to incomplete unification algorithm(s).");
	  if (latexBuffer)
	    {
	      latexBuffer->generateNonResult(*context,
					     reply,
					     prof,
					     real,
					     getFlag(SHOW_STATS),
					     showTiming,
					     getFlag(SHOW_BREAKDOWN));
	      if (state->isIncomplete())
		latexBuffer->generateWarning("Some solutions may have been missed due to incomplete unification algorithm(s).");
	    }
	  break;
	}

      ++solutionCount;
      cout << "\nSolution " << solutionCount << "\n";
      printStats(*context, prof, real, showTiming);

      DagNode* d = state->getStateDag();
      cout << "state: " << d << endl;
      UserLevelRewritingContext::printSubstitution(*(state->getSubstitution()), *variableInfo);
      if (latexBuffer)
	{
	  latexBuffer->generateSolutionNr(solutionCount);
	  if (getFlag(SHOW_STATS))
	    latexBuffer->generateStats(*context, prof, real, showTiming, getFlag(SHOW_BREAKDOWN));
	  latexBuffer->generateState(d);
	  latexBuffer->generateSubstitution(*(state->getSubstitution()), *variableInfo);
	}
    }
  QUANTIFY_STOP();

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
  if (latexBuffer)
    latexBuffer->generateContinue(getFlag(SHOW_COMMAND), limit, debug);

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
  if (latexBuffer)
    latexBuffer->generateContinue(getFlag(SHOW_COMMAND), limit, debug);

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
      Int64 real = 0;
      Int64 virt = 0;
      Int64 prof = 0;
      bool showTiming = getFlag(SHOW_TIMING) && timer.getTimes(real, virt, prof);
      if (!result)
	{
	  const char* reply = (solutionCount == 0) ? "No solution." : "No more solutions.";
	  cout << "\n" << reply << endl;
	  printStats(*context, prof, real, showTiming);
	  if (state->isIncomplete())
	    IssueWarning("Some solutions may have been missed due to incomplete unification algorithm(s).");
	  if (latexBuffer)
	    {
	      latexBuffer->generateNonResult(*context,
					     reply,
					     prof,
					     real,
					     getFlag(SHOW_STATS),
					     showTiming,
					     getFlag(SHOW_BREAKDOWN));
	      if (state->isIncomplete())
		latexBuffer->generateWarning("Some solutions may have been missed due to incomplete unification algorithm(s).");
	    }
	  break;
	}

      ++solutionCount;
      cout << "\nSolution " << solutionCount << "\n";
      printStats(*context, prof, real, showTiming);

      DagNode* stateDag;
      int variableFamily;
      Substitution* accumulatedSubstitution;
      state->getStateInfo(stateDag, variableFamily, accumulatedSubstitution);

      cout << "state: " << stateDag << endl;
      cout << "accumulated substitution:" << endl;
      UserLevelRewritingContext::printSubstitution(*accumulatedSubstitution, state->getInitialVariableInfo());
      cout << "variant unifier:" << endl;
      UserLevelRewritingContext::printSubstitution(*(state->getUnifier()), state->getUnifierVariableInfo());
      if (latexBuffer)
	{
	  latexBuffer->generateSolutionNr(solutionCount);
	  if (getFlag(SHOW_STATS))
	    latexBuffer->generateStats(*context, prof, real, showTiming, getFlag(SHOW_BREAKDOWN));
	  latexBuffer->generateState(stateDag);
	  latexBuffer->generateHeading("accumulated substitution:");
	  latexBuffer->generateSubstitution(*accumulatedSubstitution, state->getInitialVariableInfo());
	  latexBuffer->generateHeading("variant unifier:");
	  latexBuffer->generateSubstitution(*(state->getUnifier()), state->getUnifierVariableInfo());
	}
     }
  QUANTIFY_STOP();

  if (latexBuffer)
    latexBuffer->cleanUp();
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
