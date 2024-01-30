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
//	Code for smt-search commands.
//

void
Interpreter::doSmtSearch(Timer& timer,
			 VisibleModule* module,
			 SMT_RewriteSequenceSearch* state,
			 Int64 solutionCount,
			 Int64 limit)
{
  
  Int64 i = 0;
  for (; i != limit; i++)
    {
      bool result = state->findNextMatch();
      if (UserLevelRewritingContext::aborted())
	break;

      RewritingContext* context = state->getContext();
      Int64 real = 0;
      Int64 virt = 0;
      Int64 prof = 0;
      bool showTiming = getFlag(SHOW_TIMING) && timer.getTimes(real, virt, prof);
      bool showStats = getFlag(SHOW_STATS);
      if (!result)
	{
	  
	  const char* reply = (solutionCount == 0) ? "No solution." : "No more solutions.";
	  cout << "\n" << reply << endl;
	  if (showStats)
	    printStats(*context, prof, real, showTiming);
	  if (latexBuffer != 0)
	    {
	      latexBuffer->generateNonResult(*context,
					     reply,
					     prof,
					     real,
					     showStats,
					     showTiming,
					     getFlag(SHOW_BREAKDOWN));
	    } 
	  break;
	}

      ++solutionCount;
      cout << "\nSolution " << solutionCount << endl;
      printStats(*context, prof, real, showTiming);
      int stateNr = state->getCurrentStateNumber();
      DagNode* d = state->getState(stateNr);
      cout << "state: " << d << endl;
      UserLevelRewritingContext::printSubstitution(*(state->getSubstitution()),
						   *state,
						   state->getSMT_VarIndices());
      cout << "where " << state->getFinalConstraint() << endl;
      if (latexBuffer != 0)
	{
	  latexBuffer->generateSmtResult(state,
					 solutionCount,
					 prof,
					 real,
					 showStats,
					 showTiming,
					 getFlag(SHOW_BREAKDOWN));
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
      continueFunc = &Interpreter::smtSearchCont;
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
Interpreter::smtSearchCont(Int64 limit, bool debug)
{
  SMT_RewriteSequenceSearch* state = safeCast(SMT_RewriteSequenceSearch*, savedState);
  VisibleModule* fm = savedModule;
  savedState = 0;
  savedModule = 0;
  continueFunc = 0;

  if (debug)
    UserLevelRewritingContext::setDebug();

  QUANTIFY_START();
  Timer timer(getFlag(SHOW_TIMING));
  doSmtSearch(timer, fm, state, savedSolutionCount, limit);
}
