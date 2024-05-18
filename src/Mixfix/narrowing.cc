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
	{
	  //
	  //	Tidy up and return.
	  //
	  delete state;
	  module->unprotect();
	  UserLevelRewritingContext::clearDebug();
	  return;
	}
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
      cout << "\nSolution " << solutionCount;
      int stateNr = NONE;
      if (state->getVariantFlags() & NarrowingSequenceSearch3::KEEP_PATHS)
	{
	  int dummy;
	  state->getExtraStateInfo(stateNr, dummy);
	  cout << " (state " << stateNr << ")";
	}
      cout << "\n";
      printStats(*context, prof, real, showTiming);

      DagNode* stateDag;
      int variableFamily;
      DagNode* initialStateDag;
      Substitution* accumulatedSubstitution;
      state->getStateInfo(stateDag, variableFamily, initialStateDag, accumulatedSubstitution);

      cout << "state: " << stateDag << endl;
      if (state->getNrInitialStates() > 1)
	cout << "initial state: " << initialStateDag << endl;
      cout << "accumulated substitution:" << endl;
      UserLevelRewritingContext::printSubstitution(*accumulatedSubstitution, state->getInitialVariableInfo());
      cout << "variant unifier:" << endl;
      UserLevelRewritingContext::printSubstitution(*(state->getUnifier()), state->getUnifierVariableInfo());
      if (latexBuffer)
	{
	  latexBuffer->generateSolutionNr(solutionCount, stateNr);
	  if (getFlag(SHOW_STATS))
	    latexBuffer->generateStats(*context, prof, real, showTiming, getFlag(SHOW_BREAKDOWN));
	  latexBuffer->generateState(stateDag);
	  if (state->getNrInitialStates() > 1)
	    latexBuffer->generateState(initialStateDag, "initial state:");
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
  //
  //	We always save these things even if we can't continue
  //	in order to allow inspection of the narrowing paths.
  //
  savedState = state;
  savedModule = module;
  if (i == limit)  // possible to continue
    {
      //
      //	The loop terminated because we hit user's limit so ontinuation is still possible.
      //	We save the state solutionCount and set a continutation function.
      //
      context->clearCount();
      savedSolutionCount = solutionCount;
      continueFunc = &Interpreter::vuNarrowingCont;
    }
  UserLevelRewritingContext::clearDebug();
}

void
Interpreter::showNarrowingSearchPath(int stateNr, bool showRule, NarrowingSequenceSearch3* savedNarrowingSequence) const
{
  if (stateNr < 0 || !(savedNarrowingSequence->locked(stateNr)))
    {
      IssueWarning("bad state number " << QUOTE(stateNr) << ".");
      return;
    }
  Vector<int> steps;
  for (int i = stateNr; i != NONE; i = savedNarrowingSequence->getStateParent(i))
    steps.push_back(i);
  for (Index i = steps.size() - 1; i >= 0; --i)
    {
      int index = steps[i];
      DagNode* root;
      DagNode* position;
      Rule* rule;
      const Substitution* unifier;
      const NarrowingVariableInfo* unifierVariableInfo;
      int variableFamily;
      DagNode* newDag;
      const Substitution* accumulatedSubstitution;
      int parentIndex;
      savedNarrowingSequence->getHistory(index,
					 root,
					 position,
					 rule,
					 unifier,
					 unifierVariableInfo,
					 variableFamily,
					 newDag,
					 accumulatedSubstitution,
					 parentIndex);
      if (parentIndex != NONE)
	{
	  if (showRule)
	    {
	      cout << "===[ " << rule << " ]===>\n";
	      cout << "variant unifier:" << endl;
	      UserLevelRewritingContext::printCompoundSubstitution(*unifier, *rule, *unifierVariableInfo, rule->getModule());
	    }
	  else
	    {
	      const Label& l = rule->getLabel();
	      cout << "---";
	      if (l.id() != NONE)
		cout << ' ' << &l << ' ';
	      cout << "--->\n";
	    }
	}
      cout << "state " << index << ", " << newDag->getSort() << ": " << newDag << '\n';
      cout << "accumulated substitution:" << endl;
      UserLevelRewritingContext::printSubstitution(*accumulatedSubstitution, savedNarrowingSequence->getInitialVariableInfo());
    }
  if (latexBuffer)
    {
      latexBuffer->generateNarrowingSearchPath(savedNarrowingSequence,
					       steps,
					       stateNr,
					       getFlag(SHOW_COMMAND),
					       showRule);
    }
}

void
Interpreter::showFrontierStates()
{
  NarrowingSequenceSearch3* savedNarrowingSequence = dynamic_cast<NarrowingSequenceSearch3*>(savedState);
  if (!savedNarrowingSequence)
    {
      IssueWarning("no narrowing state forest.");
      return;
    }
  const char* sep = "";
  //
  //	In the =>1, =>+, =>* cases we may have visited a state without (fully) expanding it
  //	when we pause due to hitting the solution bound or halt due to hitting the depth bound.
  //	This state belongs in the frontier.
  //
  //	In the =>! case we may have visited states, determined they have a descendant but
  //	not expanded them to avoid exceeding a depth bound. They also belong in the frontier.
  //
  bool partiallyExpanded;
  Vector<DagNode*> visitedButUnexpanded = savedNarrowingSequence->getUnexpandedStates(partiallyExpanded);
  for (DagNode* d : visitedButUnexpanded)
    {
      cout << sep;
      if (partiallyExpanded)
	cout << Tty(Tty::RED) << d << Tty(Tty::RESET);  // paused due to solution bound in =>1, =>+, =>* cases
      else
	cout << d;  // halted due to depth bound, or in =>! case
      sep = " /\\\n";
    }
  //
  //	It's possible to have unvisited states even if we don't have a visited but unexpanded state.
  //	This happens if we pause due to solution bound before we finished unifying with initial states;
  //	At this point we have not started narrowing and therefore have visited no states, but we will
  //	have at least one initial state that is unvisited.
  //
  Vector<DagNode*> unvisitedStates = savedNarrowingSequence->getUnvisitedStates();
  for (DagNode* d : unvisitedStates)
    {
      cout << sep << d;
      sep = " /\\\n";
    }
  //
  //	If we exhausted the search space before hitting a bound, this means the frontier is empty.
  //
  if (*sep == '\0')
    cout << Tty(Tty::RED) << "*** frontier is empty ***" << Tty(Tty::RESET);
  cout << endl;
  if (latexBuffer)
    {
      latexBuffer->generateStateSet(getFlag(SHOW_COMMAND),
				    "show frontier states",
				    visitedButUnexpanded,
				    partiallyExpanded,
				    unvisitedStates,
				    "\\color{red}*** frontier is empty ***\\color{black}");
    }
}

void
Interpreter::showMostGeneralStates()
{
  NarrowingSequenceSearch3* savedNarrowingSequence = dynamic_cast<NarrowingSequenceSearch3*>(savedState);
  if (!savedNarrowingSequence)
    {
      IssueWarning("no narrowing state forest.");
      return;
    }
  if (!(savedNarrowingSequence->getVariantFlags() & (NarrowingSequenceSearch3::FOLD | NarrowingSequenceSearch3::VFOLD)))
    {
      IssueWarning("most general states are only computed when folding.");
      return;
    }
  Vector<DagNode*> mostGeneralStates = savedNarrowingSequence->getMostGeneralStates();
  const char* sep = "";
  for (DagNode* d : mostGeneralStates)
    {
      cout << sep << d;
      sep = " /\\\n";
    }
  cout << endl;
  if (latexBuffer)
    {
      latexBuffer->generateStateSet(getFlag(SHOW_COMMAND),
				    "show most general states",
				    Vector<DagNode*>(),
				    false,
				    mostGeneralStates,
				    nullptr);
    }
}
