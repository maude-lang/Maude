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
//	Code for commands that do rewriting.
//
#ifdef QUANTIFY_REWRITING
#include "quantify.h"
#endif

void
Interpreter::clearContinueInfo()
{
  delete savedContext;
  delete savedMatchSearchState;
  delete savedRewriteSequenceSearch;
  savedContext = 0;
  savedMatchSearchState = 0;
  savedRewriteSequenceSearch = 0;
  continueFunc = 0;
  if (savedModule != 0)
    {
      (void) savedModule->unprotect();
      savedModule = 0;
    }
}

DagNode*
Interpreter::makeDag(Term* subjectTerm)
{
  subjectTerm = subjectTerm->normalize(false);
  NatSet eagerVariables;
  Vector<int> problemVariables;
  subjectTerm->markEager(0, eagerVariables, problemVariables);
  DagNode* d = subjectTerm->term2Dag();
  subjectTerm->deepSelfDestruct();
  return d;
}

DagNode*
Interpreter::makeDag(const Vector<Token>& subject)
{
  if (Term* s = currentModule->getFlatModule()->parseTerm(subject))
    return makeDag(s);
  return 0;
}

void
Interpreter::startUsingModule(VisibleModule* module)
{
  clearContinueInfo();
  UserLevelRewritingContext::clearTrialCount();
  if (getFlag(AUTO_CLEAR_MEMO))
    module->clearMemo();
  if (getFlag(AUTO_CLEAR_PROFILE))
    module->clearProfile();
  module->protect();
}

void
Interpreter::beginRewriting(bool debug)
{
  if (debug)
    UserLevelRewritingContext::setDebug();
#ifdef QUANTIFY_REWRITING
  quantify_start_recording_data();
#endif
}

void
Interpreter::printTiming(Int64 nrRewrites, Int64 cpu, Int64 real)
{
  cout << " in " << cpu / 1000 << "ms cpu (" << real / 1000 << "ms real) (";
  if (cpu > 0)
    cout << (1000000 * nrRewrites) / cpu;
  else
    cout << '~';
  cout << " rewrites/second)";
}

void
Interpreter::printStats(const Timer& timer, RewritingContext& context, bool timingFlag)
{
  Int64 nrRewrites = context.getTotalCount();
  cout << "rewrites: " << nrRewrites;
  Int64 real;
  Int64 virt;
  Int64 prof;
  if (timingFlag && timer.getTimes(real, virt, prof))
    printTiming(nrRewrites, prof, real);
  cout << '\n';
  if (getFlag(SHOW_BREAKDOWN))
    {
      cout << "mb applications: " << context.getMbCount() <<
	"  equational rewrites: " << context.getEqCount() <<
	"  rule rewrites: " << context.getRlCount() << '\n';
    }
}

void
Interpreter::endRewriting(Timer& timer,
			  UserLevelRewritingContext* context,
			  VisibleModule* module,
			  ContinueFuncPtr cf)
{
  timer.stop();
#ifdef QUANTIFY_REWRITING
  quantify_stop_recording_data();
#endif
  clearContinueInfo();  // just in case debugger left info
  if (UserLevelRewritingContext::aborted())
    {
      delete context;
      (void) module->unprotect();
    }
  else
    {
      if (getFlag(SHOW_STATS))
	printStats(timer, *context, getFlag(SHOW_TIMING));
      DagNode* r = context->root();
      cout << "result " << r->getSort() << ": " << r << '\n';
      cout.flush();
      if (xmlBuffer != 0)
	{
	  xmlBuffer->generateResult(*context,
				    timer,
				    getFlag(SHOW_STATS),
				    getFlag(SHOW_TIMING),
				    getFlag(SHOW_BREAKDOWN));
	}
      
      if (cf == 0)
	{
	  delete context;
	  (void) module->unprotect();
	}
      else
	{
	  savedContext = context;
	  savedModule = module;
	  continueFunc = cf;
	}
    }
  UserLevelRewritingContext::clearDebug();  // even if we didn't start in debug mode
}

#if 1
//int CONVERT_THRESHOLD = 8;
//int MERGE_THRESHOLD = 16;

void
Interpreter::reduce(const Vector<Token>& subject, bool debug)
{
  if (DagNode* d = makeDag(subject))
    {
      UserLevelRewritingContext* context = new UserLevelRewritingContext(d);
      if (getFlag(SHOW_COMMAND))
	{
	  UserLevelRewritingContext::beginCommand();
	  cout << "reduce in " << currentModule << " : " << d << " .\n";
	  if (xmlBuffer != 0)
	    xmlBuffer->generateReduce(d);
	}
      VisibleModule* fm = currentModule->getFlatModule();
      startUsingModule(fm);
      beginRewriting(debug);
      Timer timer(getFlag(SHOW_TIMING));
      context->reduce();
      endRewriting(timer, context, fm);
    }
}
#else
int CONVERT_THRESHOLD;
int MERGE_THRESHOLD;

void
Interpreter::reduce(const Vector<Token>& subject, bool debug)
{
  if (DagNode* d = makeDag(subject))
    {
      for (int i = 0; i < 2; ++i)
	{
	  for (CONVERT_THRESHOLD = 0; CONVERT_THRESHOLD <= 20; CONVERT_THRESHOLD += 1)
	    {
	      for (MERGE_THRESHOLD = 0; MERGE_THRESHOLD <= 20; MERGE_THRESHOLD += 1)
		{
		  cout << "CONVERT_THRESHOLD = " << CONVERT_THRESHOLD <<
		    "\tMERGE_THRESHOLD = " << MERGE_THRESHOLD << endl;
		  DagNode* d = makeDag(subject);
		  UserLevelRewritingContext* context = new UserLevelRewritingContext(d);
		  VisibleModule* fm = currentModule->getFlatModule();
		  startUsingModule(fm);
		  beginRewriting(debug);
		  Timer timer(getFlag(SHOW_TIMING));
		  context->reduce();
		  endRewriting(timer, context, fm);
		}
	    }
	}
    }
}
#endif

void
Interpreter::rewrite(const Vector<Token>& subject, Int64 limit, bool debug)
{
  if (DagNode* d = makeDag(subject))
    {
      if (getFlag(SHOW_COMMAND))
	{
	  UserLevelRewritingContext::beginCommand();
	  cout << "rewrite ";
	  if (limit != NONE)
	    cout  << '[' << limit << "] ";
	  cout << "in " << currentModule << " : " << d << " .\n";
	  if (xmlBuffer != 0)
	    xmlBuffer->generateRewrite(d, limit);
	}
      
      UserLevelRewritingContext* context = new UserLevelRewritingContext(d);
      VisibleModule* fm = currentModule->getFlatModule();

      startUsingModule(fm);
      fm->resetRules();
      beginRewriting(debug);
      Timer timer(getFlag(SHOW_TIMING));
      context->ruleRewrite(limit);
      endRewriting(timer, context, fm, &Interpreter::rewriteCont);
    }
}

void
Interpreter::rewriteCont(Int64 limit, bool debug)
{
  UserLevelRewritingContext* context = savedContext;
  VisibleModule* fm = savedModule;
  savedContext = 0;
  savedModule = 0;
  continueFunc = 0;
  if (xmlBuffer != 0 && getFlag(SHOW_COMMAND))
    xmlBuffer->generateContinue("rewrite", fm, limit);
  context->clearCount();
  beginRewriting(debug);
  Timer timer(getFlag(SHOW_TIMING));
  context->ruleRewrite(limit);
  endRewriting(timer, context, fm, &Interpreter::rewriteCont);
}

void
Interpreter::fRewrite(const Vector<Token>& subject, Int64 limit, Int64 gas, bool debug)
{
  if (DagNode* d = makeDag(subject))
    {
      if (getFlag(SHOW_COMMAND))
	{
	  UserLevelRewritingContext::beginCommand();
	  cout << "frewrite ";
	  if (limit != NONE)
	    {
	      if (gas == NONE)
		cout  << '[' << limit << "] ";
	      else
		cout  << '[' << limit << ", " << gas << "] ";
	    }
	  cout << "in " << currentModule << " : " << d << " .\n";
	  if (xmlBuffer != 0)
	    xmlBuffer->generateFrewrite(d, limit, gas);
	}
      UserLevelRewritingContext* context = new UserLevelRewritingContext(d);
      context->setObjectMode(ObjectSystemRewritingContext::FAIR);
      VisibleModule* fm = currentModule->getFlatModule();

      startUsingModule(fm);
      fm->resetRules();
      beginRewriting(debug);
      Timer timer(getFlag(SHOW_TIMING));
      context->fairRewrite(limit, (gas == NONE) ? 1 : gas);
      endRewriting(timer, context, fm, &Interpreter::fRewriteCont);
    }
}

void
Interpreter::fRewriteCont(Int64 limit, bool debug)
{
  UserLevelRewritingContext* context = savedContext;
  VisibleModule* fm = savedModule;
  savedContext = 0;
  savedModule = 0;
  continueFunc = 0;
  if (xmlBuffer != 0 && getFlag(SHOW_COMMAND))
    xmlBuffer->generateContinue("frewrite", fm, limit);
  context->clearCount();
  beginRewriting(debug);
  Timer timer(getFlag(SHOW_TIMING));
  context->fairContinue(limit);
  endRewriting(timer, context, fm, &Interpreter::fRewriteCont);
}

void
Interpreter::cont(Int64 limit, bool debug)
{
  if (limit != 0)
    {
      if (continueFunc != 0)
	(this->*continueFunc)(limit, debug);
      else
	IssueWarning("can't continue.");
    }
}

void
Interpreter::creduce(const Vector<Token>& subject)
{
#ifdef COMPILER
  if (DagNode* d = makeDag(subject))
    {
      if (makeExecutable(currentModule, getFlag(COMPILE_COUNT)))
	{
	  if (getFlag(SHOW_COMMAND))
	    {
	      UserLevelRewritingContext::beginCommand();
	      cout << "creduce in " << currentModule << " : " << d << " .\n";
	    }
	  //
	  //	Write dag to file, run executable and read result from file.
	  //
	  outputGraph(d);
	  runExecutable();
	  Int64 nrRewrites;
	  Int64 cpu;
	  Int64 real;
	  DagNode* r = inputGraph(nrRewrites, cpu, real);
	  //
	  //	Print number of rewrites, timing and result.
	  //
	  if (getFlag(SHOW_STATS))
	    {
	      cout << "rewrites: " << nrRewrites;
	      if (getFlag(SHOW_TIMING))
		printTiming(nrRewrites, cpu, real);
	      cout << '\n';
	    }
	  cout << "result " << r->getSort() << ": " << r << '\n';
	  cout.flush();
	}
    }
#endif
}
