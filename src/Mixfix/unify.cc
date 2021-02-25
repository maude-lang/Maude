/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2021 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for unify command.
//

void
Interpreter::unify(const Vector<Token>& bubble, Int64 limit, bool irredundant)
{
  VisibleModule* fm = currentModule->getFlatModule();
  Vector<Term*> lhs;
  Vector<Term*> rhs;
  if (!(fm->parseUnifyCommand(bubble, lhs, rhs)))
    return;

  if (getFlag(SHOW_COMMAND))
    {
      UserLevelRewritingContext::beginCommand();
      if (irredundant)
	cout << "irredundant ";
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
  FreshVariableSource* freshVariableSource = new FreshVariableSource(fm);
  UnificationProblem* problem = irredundant ?
    new IrredundantUnificationProblem(lhs, rhs, freshVariableSource) :
    new UnificationProblem(lhs, rhs, freshVariableSource);
  if (problem->problemOK())
    doUnification(timer, fm, problem, 0, limit);
  else
    {
      delete problem;
      fm->unprotect();
#ifdef QUANTIFY_REWRITING
      quantify_stop_recording_data();
#endif
    }
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
      bool result = problem->findNextUnifier();
      //
      //	If the user interrupted, we need to bail before outputing a unifier
      //	(which won't display correctly anyway).
      //
      if (UserLevelRewritingContext::interrupted())
	break;
 
      if (!result)
	{
	  if (solutionCount == 0)
	    {
	      printDecisionTime(timer);
	      cout << "No unifier.\n";
	    }
	  if (problem->isIncomplete())
	    IssueWarning("Some unifiers may have been missed due to incomplete unification algorithm(s).");
	  break;
	}

      ++solutionCount;
      if (solutionCount == 1)
	printDecisionTime(timer);
      cout << "\nUnifier " << solutionCount << '\n';
      UserLevelRewritingContext::printSubstitution(problem->getSolution(), problem->getVariableInfo());
    }

#ifdef QUANTIFY_REWRITING
  quantify_stop_recording_data();
#endif

  clearContinueInfo();  // just in case debugger left info
  if (i == limit)  // possible to continue
    {
      savedState = problem;
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
  UnificationProblem* problem = safeCast(UnificationProblem*, savedState);
  VisibleModule* fm = savedModule;
  savedState = 0;
  savedModule = 0;
  continueFunc = 0;

#ifdef QUANTIFY_REWRITING
  quantify_start_recording_data();
#endif

  Timer timer(getFlag(SHOW_TIMING));
  doUnification(timer, fm, problem, savedSolutionCount, limit);
}
