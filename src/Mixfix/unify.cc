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
//	Code for unify and xunify commands.
//

void
Interpreter::unify(const Vector<Token>& bubble, Int64 limit)
{
  VisibleModule* fm = currentModule->getFlatModule();
  Vector<Term*> lhs;
  Vector<Term*> rhs;
  if (!(fm->parseUnifyCommand(bubble, lhs, rhs)))
    return;

  if (getFlag(SHOW_COMMAND))
    {
      UserLevelRewritingContext::beginCommand();
      cout << "unify ";
      if (limit != NONE)
	cout << '[' << limit << "] ";
      cout << "in " << currentModule << " : ";
      int nrPairs = lhs.size();
      for (int i = 0; i < nrPairs; ++i)
	cout << lhs[i] << " =? " << rhs[i] << ((i == nrPairs - 1) ? " ." : " /\\ ");
      cout << endl;
    }

  startUsingModule(fm);

#ifdef QUANTIFY_REWRITING
  quantify_start_recording_data();
#endif

  Timer timer(getFlag(SHOW_TIMING));
  UnificationProblem* problem = new UnificationProblem(lhs, rhs, new FreshVariableSource(fm));
  if (problem->problemOK())
    doUnification(timer, fm, problem, 0, limit);
  else
    delete problem;
#ifdef QUANTIFY_REWRITING
  else
    quantify_stop_recording_data();
#endif
}

void
Interpreter::doUnification(Timer& timer,
			   VisibleModule* module,
			   UnificationProblem* problem,
			   int solutionCount,
			   int limit)
{
  int i = 0;
  for (; i != limit; i++)
    {
      if (!problem->findNextUnifier())
	{
	  if (solutionCount == 0)
	    {
	      printDecisionTime(timer);
	      cout << "No unifier.\n";
	    }
	  break;
	}

      ++solutionCount;
      if (solutionCount == 1)
	printDecisionTime(timer);
      cout << "\nSolution " << solutionCount << '\n';
      UserLevelRewritingContext::printSubstitution(problem->getSolution(), problem->getVariableInfo());
      if (UserLevelRewritingContext::interrupted())
	break;
    }

#ifdef QUANTIFY_REWRITING
  quantify_stop_recording_data();
#endif

  clearContinueInfo();  // just in case debugger left info
  if (i == limit)  // possible to continue
    {
      savedUnificationProblem = problem;
      savedSolutionCount = solutionCount;
      savedModule = module;
      continueFunc = &Interpreter::unifyCont;
    }
  else  // tidy up
    {
      delete problem;
      module->unprotect();
    }
  UserLevelRewritingContext::clearDebug();
  MemoryCell::okToCollectGarbage();
}

void
Interpreter::unifyCont(Int64 limit, bool /* debug */)
{
  UnificationProblem* problem = savedUnificationProblem;
  VisibleModule* fm = savedModule;
  savedUnificationProblem = 0;
  savedModule = 0;
  continueFunc = 0;

#ifdef QUANTIFY_REWRITING
  quantify_start_recording_data();
#endif

  Timer timer(getFlag(SHOW_TIMING));
  doUnification(timer, fm, problem, savedSolutionCount, limit);
}
