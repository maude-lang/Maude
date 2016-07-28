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
  delete savedUnificationProblem;
  delete savedRewriteSequenceSearch;
  delete savedStrategicSearch;
  savedContext = 0;
  savedMatchSearchState = 0;
  savedUnificationProblem = 0;
  savedRewriteSequenceSearch = 0;
  savedStrategicSearch = 0;
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
Interpreter::printModifiers(Int64 number, Int64 number2)
{
  if (number != NONE || number2 != NONE)
    {
      cout << '[';
      if (number != NONE)
	cout << number;
      if (number2 != NONE)
	cout << ", " << number2;
      cout << "] ";
    }
  cout << "in " << currentModule << " : ";
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
	"  rule rewrites: " << context.getRlCount() <<
	"  variant narrowing steps: " << context.getVariantNarrowingCount() <<
	"  narrowing steps: " << context.getNarrowingCount() << '\n';
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
	  cout << "reduce in " << currentModule << " : " << d << " ." << endl;
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
	  cout << "in " << currentModule << " : " << d << " ." << endl;
	  if (xmlBuffer != 0)
	    xmlBuffer->generateRewrite(d, limit);
	}
      
      UserLevelRewritingContext* context = new UserLevelRewritingContext(d);
      VisibleModule* fm = currentModule->getFlatModule();

      startUsingModule(fm);
      if (getFlag(AUTO_CLEAR_RULES))
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
	  printModifiers(limit, gas);
	  cout << d << " ." << endl;
	  if (xmlBuffer != 0)
	    xmlBuffer->generateFrewrite(d, limit, gas);
	}
      UserLevelRewritingContext* context = new UserLevelRewritingContext(d);
      context->setObjectMode(ObjectSystemRewritingContext::FAIR);
      VisibleModule* fm = currentModule->getFlatModule();

      startUsingModule(fm);
      if (getFlag(AUTO_CLEAR_RULES))
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
	      cout << "creduce in " << currentModule << " : " << d << " ." << endl;
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

#include "stackMachine.hh"
#include "stackMachineRhsCompiler.hh"
#include "frame.hh"

void
Interpreter::sreduce(const Vector<Token>& subject)
{
  if (Term* t = currentModule->getFlatModule()->parseTerm(subject))
    {
      //
      //	Partly normalize term (don't flatten).
      //
      t = t->normalize(false);
      //
      //	Set EAGER flag in eager terms since term2InstructionSequence() needs this
      //	to determine subterm sharability.
      //
      NatSet eagerVariables;
      Vector<int> problemVariables;
      t->markEager(0, eagerVariables, problemVariables);
      //
      //	Now compile a sequence of instructions to build the term, evaluating alond the way.
      //
      Instruction* instructionSequence = t->term2InstructionSequence();
      //
      //	If some function symbol didn't generate an instruction we get the null sequence to
      //	flag unsupported feature.
      //
      if (instructionSequence == 0)
	{
	  IssueWarning("sreduce unsupported operator (Maude VM compiler).");
	  return;
	}
      //
      //	Now run the sequence in a stack machine.
      //
      VisibleModule* fm = currentModule->getFlatModule();
      fm->stackMachineCompile();

      startUsingModule(fm);
      Timer timer(getFlag(SHOW_TIMING));

      if (getFlag(SHOW_COMMAND))
	{
	  UserLevelRewritingContext::beginCommand();
	  cout << "sreduce in " << currentModule << " : " << t << " ." << endl;
	}
      
      t->deepSelfDestruct();
      StackMachine sm;
      DagNode* r = sm.execute(instructionSequence);
      Int64 nrRewrites = sm.getEqCount();
      //
      //	End of stack based reduction.
      //
      if (getFlag(SHOW_STATS))
	{
	  cout << "rewrites: " << nrRewrites;
	  Int64 real;
	  Int64 virt;
	  Int64 prof;
	  if (getFlag(SHOW_TIMING) && timer.getTimes(real, virt, prof))
	    printTiming(nrRewrites, prof, real);
	  cout << '\n';
	}
      cout << "result " << r->getSort() << ": " << r << '\n';
      cout.flush();
      delete instructionSequence;
      (void) fm->unprotect();
    }
}

#include "SMT_Info.hh"
#include "variableGenerator.hh"

void
Interpreter::check(const Vector<Token>& subject)
{
  if (Term* term = currentModule->getFlatModule()->parseTerm(subject))
    {
      term = term->normalize(false);
      DagNode* d = term->term2Dag();
 
      if (getFlag(SHOW_COMMAND))
	{
	  UserLevelRewritingContext::beginCommand();
	  cout << "check in " << currentModule << " : " << d << " ." << endl;
	}

      VisibleModule* fm = currentModule->getFlatModule();
      startUsingModule(fm);

      const SMT_Info& smtInfo = fm->getSMT_Info();
      VariableGenerator vg(smtInfo);
      VariableGenerator::Result result = vg.checkDag(d);
      if (result == VariableGenerator::BAD_DAG)
	IssueWarning (*term << ": term " << QUOTE(term) << " is not a valid SMT Boolean expression.");
      else
	{
	  cout << "Result from sat solver is: " <<
	    ((result == VariableGenerator::SAT) ? "sat" :
	     ((result == VariableGenerator::UNSAT) ? "unsat" : "undecided")) << endl;
	}

      term->deepSelfDestruct();
      fm->unprotect();
    }
}

void
Interpreter::test(const Vector<Token>& subject)
{
}
