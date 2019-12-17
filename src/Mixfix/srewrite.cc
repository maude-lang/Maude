/*

    This file is part of the Maude 3 interpreter.

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
//	Code for srewrite command.
//

void
Interpreter::sRewrite(const Vector<Token>& subjectAndStrategy, Int64 limit, bool debug, bool depthSearch)
{
  VisibleModule* fm = currentModule->getFlatModule();
  Term* subject;
  StrategyExpression* strategy;
  if (!(fm->parseStrategyCommand(subjectAndStrategy, subject, strategy)))
    return;

  TermSet nothing;
  VariableInfo vinfo;
  if (!strategy->check(vinfo, nothing))
    {
      subject->deepSelfDestruct();
      delete strategy;
      return;
    }

  DagNode* subjectDag = makeDag(subject);

  if (getFlag(SHOW_COMMAND))
    {
      UserLevelRewritingContext::beginCommand();
      if (debug)
	cout << "debug ";
      cout << (depthSearch ? "dsrewrite " : "srewrite ");
      if (limit != NONE)
	cout << '[' << limit << "] ";

      cout << "in " << currentModule << " : " << subjectDag <<
	" using " << strategy << " ." << endl;
    }
  if (xmlBuffer != 0)
    xmlBuffer->generateSRewrite(subjectDag, strategy, limit, depthSearch);

  startUsingModule(fm);

  strategy->process();

  if (debug)
    UserLevelRewritingContext::setDebug();

  Timer timer(getFlag(SHOW_TIMING));
  UserLevelRewritingContext* context = new UserLevelRewritingContext(subjectDag);
  context->reduce();
  if (context->traceAbort())
    {
      delete context;
      delete strategy;
      (void) fm->unprotect();
      UserLevelRewritingContext::clearDebug();
      return;
    }

  Assert(context->root() != 0, "null root");
  // pass ownership of context and strategy
  StrategicSearch* state = depthSearch ? new DepthFirstStrategicSearch(context, strategy)
				       : static_cast<StrategicSearch*>(new
					   FairStrategicSearch(context, strategy));
  doStrategicSearch(timer, fm, state, 0, limit, depthSearch);
}

void
Interpreter::doStrategicSearch(Timer& timer,
			       VisibleModule* module,
			       StrategicSearch* state,
			       Int64 solutionCount,
			       Int64 limit,
			       bool depthSearch)
{
  RewritingContext* context = state->getContext();
  Int64 i = 0;
  for (; i != limit; ++i)  // limit could be -1 for "no limit"
    {
      DagNode* d = state->findNextSolution();
      if (context->traceAbort())
	break;
      if (d == 0)
	{
	  cout << endl << (solutionCount > 0
	    ? "No more solutions." : "No solution.") << endl;
	  if (getFlag(SHOW_STATS))
	    printStats(timer, *context, getFlag(SHOW_TIMING));
	  break;
	}

      ++solutionCount;
      cout << "\nSolution " << solutionCount << '\n';
      if (getFlag(SHOW_STATS))
	printStats(timer, *context, getFlag(SHOW_TIMING));
      cout << "result " << d->getSort() << ": " << d << '\n';
      if (xmlBuffer != 0)
	{
	  xmlBuffer->generateResult(*context,
				    timer,
				    getFlag(SHOW_STATS),
				    getFlag(SHOW_TIMING),
				    getFlag(SHOW_BREAKDOWN));
	}
    }
  clearContinueInfo();  // just in case debugger left info
  if (i == limit)
    {
      //
      //	The loop terminated because we hit user's limit so
      //	continuation is still possible. We save the state,
      //	solutionCount and module, and set a continuation function.
      //
      context->clearCount();
      savedState = state;
      savedSolutionCount = solutionCount;
      savedModule = module;
      continueFunc = depthSearch ? &Interpreter::dsRewriteCont : &Interpreter::sRewriteCont;
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
Interpreter::sRewriteCont(Int64 limit, bool debug)
{
  StrategicSearch* state = safeCast(StrategicSearch*, savedState);
  VisibleModule* fm = savedModule;
  savedState = 0;
  savedModule = 0;
  continueFunc = 0;

  if (debug)
    UserLevelRewritingContext::setDebug();
  if (xmlBuffer != 0 && getFlag(SHOW_COMMAND))
    xmlBuffer->generateContinue("srewrite", fm, limit);

  Timer timer(getFlag(SHOW_TIMING));
  doStrategicSearch(timer, fm, state, savedSolutionCount, limit, false);
}

void
Interpreter::dsRewriteCont(Int64 limit, bool debug)
{
  StrategicSearch* state = safeCast(StrategicSearch*, savedState);
  VisibleModule* fm = savedModule;
  savedState = 0;
  savedModule = 0;
  continueFunc = 0;

  if (debug)
    UserLevelRewritingContext::setDebug();
  if (xmlBuffer != 0 && getFlag(SHOW_COMMAND))
    xmlBuffer->generateContinue("dsrewrite", fm, limit);

  Timer timer(getFlag(SHOW_TIMING));
  doStrategicSearch(timer, fm, state, savedSolutionCount, limit, true);
}
