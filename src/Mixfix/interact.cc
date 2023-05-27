/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for handling user interaction.
//
#include <signal.h>

bool UserLevelRewritingContext::interactiveFlag = true;
bool UserLevelRewritingContext::ctrlC_Flag = false;
bool UserLevelRewritingContext::infoFlag = false;
bool UserLevelRewritingContext::stepFlag = false;
bool UserLevelRewritingContext::abortFlag = false;
int UserLevelRewritingContext::debugLevel = 0;
Int64 UserLevelRewritingContext::rewriteCountAtLastInterrupt = 0;
timespec UserLevelRewritingContext::timeAtLastInterrupt = {0, 0};

int yyparse(UserLevelRewritingContext::ParseResult*);
void cleanUpParser();
void cleanUpLexer();

void
UserLevelRewritingContext::clearDebug()
{
  //
  //	We clear the global traceFlag unless any exeception
  //	situations are still active.
  //
  setTraceStatus(interpreter.getFlag(Interpreter::EXCEPTION_FLAGS));
  stepFlag = false;
  abortFlag = false;
}

/*
void
UserLevelRewritingContext::clearInterrupt()
{
  setTraceStatus(interpreter.getFlag(Interpreter::EXCEPTION_FLAGS));
  ctrlC_Flag =  false;
}
*/

bool
UserLevelRewritingContext::interruptSeen()
{
  return interrupted();
}

void
UserLevelRewritingContext::ignoreCtrlC()
{
  static struct sigaction ctrlC_Handler;
  ctrlC_Handler.sa_handler = SIG_IGN;
  sigaction(SIGINT, &ctrlC_Handler, 0);
}

void
UserLevelRewritingContext::setHandlers(bool handleCtrlC)
{
  if (interactiveFlag && handleCtrlC)
    {
      static struct sigaction ctrlC_Handler;
      ctrlC_Handler.sa_handler = interruptHandler;
#ifdef SA_INTERRUPT
      //
      //	Avoid old BSD semantics which automatically restarts
      //	interrupted system calls.
      //
      ctrlC_Handler.sa_flags = SA_INTERRUPT;
#endif
      sigaction(SIGINT, &ctrlC_Handler, 0);
    }
  //
  //	We want to have requests for info to be minimally disruptive
  //	so we request system calls that are interrrupted be restarted.
  //
  static struct sigaction sigInfoHandler;
  sigInfoHandler.sa_handler = infoHandler;
  sigInfoHandler.sa_flags = SA_RESTART;
#ifdef SIGINFO
  sigaction(SIGINFO, &sigInfoHandler, 0);
#else
  sigaction(SIGUSR1, &sigInfoHandler, 0);
#endif

#ifdef NO_ASSERT
  //
  //	If we're not debugging we handle internal errors and stack overflows.
  //	Since these signals will be caught at most once, we can live with the
  //	non-portable semantics of signal().
  //
  BddUser::setErrorHandler(internalErrorHandler);  // BuDDy detects misuse of BDDs
  signal(SIGBUS, internalErrorHandler);  // misaligned memory access or nonexistent real memeory
  signal(SIGILL, internalErrorHandler);  // illegal instruction

#ifdef USE_LIBSIGSEGV
  //
  //	Stack overflows are reported as SIGSEGV signals and so we need to use the
  //	libsigsegv library to heuristically distinguish the two conditions.
  //
  size_t altStackSize = SIGSTKSZ;  // might not be a compile time constant
  void* altStack = malloc(altStackSize);
  if (altStack == 0)
    {
      cerr << "Fatal error: unable to allocate alternative stack." << endl;
      exit(OUT_OF_MEMORY);
    }
  sigsegv_install_handler(sigsegvHandler);  // illegal memory access or stack overflow
  stackoverflow_install_handler(stackOverflowHandler, altStack, altStackSize);
#else
  //
  //	If we can't use the library we will catch SIGSEGVs but not install
  //	an alternative stack so that that stack overflows will show up as
  //	segmentation faults.
  //
  signal(SIGSEGV, internalErrorHandler);
#endif
#endif
  //
  //	Because we can't ensure that when we write to a pipe or socket
  //	that the other end hasn't been closed, we need to do something
  //	about SIGPIPE. It doesn't make much sense to catch it since the
  //	code that made system call that generated the SIGPIPE should
  //	check for EPIPE and is best placed to handle it.
  //
  signal(SIGPIPE, SIG_IGN);
  //
  //	HACK: this should go somewhere else.
  //
  changePrompt();
  ioManager.setContPrompt("> ");
}

#ifdef USE_LIBSIGSEGV

void
UserLevelRewritingContext::stackOverflowHandler(int emergency, stackoverflow_context_t scp)
{
  //
  //	Assume machine state is bad - so use a system call.
  //
  static char message[] = "\nFatal error: stack overflow.\n\
This can happen because you have an infinite computation, say a runaway\n\
recursion, or are model checking an infinite model. It can also happen because\n\
the stacksize limit in your environment is set too low for the computation\n\
you are trying to do. You can find the value of your stacksize with the\n\
tcsh command 'limit stacksize' or the bash command 'ulimit -s'.\n\
\n\
Depending on your operating system configuration you may be able to\n\
increase your stacksize with the tcsh command 'unlimit stacksize'\n\
or the bash command 'ulimit -s unlimited'.\n\n";
  returnValueDump = write(STDERR_FILENO, message, sizeof(message) - 1);
  exit(STACK_OVERFLOW);  // calling _exit() loses the message on Macs
}

int
UserLevelRewritingContext::sigsegvHandler(void* /* fault_address */, int serious)
{
  if (serious)  // real segmentation fault
    internalErrorHandler(SIGSEGV);  // doesn't return
  return 0;  // must have been a stack overflow
}

#endif

void
UserLevelRewritingContext::internalErrorHandler(int /* signalNr */)
{
  //
  //	Assume machine state is bad - so use system calls.
  //
  static char message1[] = "\nMaude internal error.\nPlease submit a bug report to: ";
  static char message2[] = PACKAGE_BUGREPORT;
  static char message3[] = "\nPlease include the platform details, Maude version, and a file\n\
`crash.maude' that can be loaded to reproduce the crash (it may load\n\
other files). Do not bother trying to simplify your example unless the\n\
runtime to the bug being visible is greater than 10 seconds.\n\n";
  returnValueDump = write(STDERR_FILENO, message1, sizeof(message1) - 1);
  returnValueDump = write(STDERR_FILENO, message2, sizeof(message2) - 1);
  returnValueDump = write(STDERR_FILENO, message3, sizeof(message3) - 1);
  exit(INTERNAL_ERROR);  // calling _exit() loses the message on Macs
}

void
UserLevelRewritingContext::interruptHandler(int)
{
  //
  //	Note that a ^C situation is present.
  //
  ctrlC_Flag = true;
  //
  //	Set global trace flag to indicate an exception condition
  //	is present and slow route all execution.
  //
  setTraceStatus(true);
}

void
UserLevelRewritingContext::infoHandler(int)
{
  //
  //	Note that an info situation is present.
  //
  infoFlag = true;
  //
  //	Set global trace flag to indicate an exception condition
  //	is present and slow route all execution.
  //
  setTraceStatus(true);
}

void
UserLevelRewritingContext::interruptHandler2(...)
{
  // windowChangedFlag = true;
}

bool
UserLevelRewritingContext::handleInterrupt()
{
  //
  //	This is called because a slow/blocked system call was interrupted
  //	by a signal. We return true if we dealt with the issue and want
  //	to restart the system call and false to quit.
  //
  //	If ctrl-C has been pressed we want to quit the system call so Maude
  //	can respond to the user - might cause issues with files and sockets
  //	but we prefer Maude to be responsive.
  //
  //	If it was cause by an info request we want to continue as normal. If
  //	there are both ctrl-C and info events we treat it as a ctrl-C.
  //
  if (infoFlag)
    {
      //
      //	Interrupted by an info request.
      //
      printStatusReportCommon();
      cerr << "Waiting for external event." << endl;
      where(cerr);
      cerr << endl;
      infoFlag = false;
      //
      //	Clear the trace flag if nothing else is using it.
      //
      if (!ctrlC_Flag)
        setTraceStatus(interpreter.getFlag(Interpreter::EXCEPTION_FLAGS));
    }
  if (ctrlC_Flag)
    {
      //
      //	We treat doing an rewrite as restarting the two ctrl-C
      //	events to abort count.
      //
      Int64 currentRewriteCount = getTotalCount();
      if (rewriteCountAtLastInterrupt == currentRewriteCount)
	{
	  cerr << "\nSecond control-C on the same suspension." << endl;
	  cerr << "Aborting execution and returning to command line." << endl;
	  abortFlag = true;  // treat this as an abort
	  return false;
	}
      //
      //	Check if two ctrl-C events happened within a second.
      //
      timespec timeValue;
      DebugSave(r, clock_gettime(CLOCK_REALTIME, &timeValue));
      Assert(r == 0, "clock_gettime() failed: " << strerror(errno));
      time_t seconds = timeValue.tv_sec - timeAtLastInterrupt.tv_sec;
      if (seconds == 0 || (seconds == 1 && timeValue.tv_nsec < timeAtLastInterrupt.tv_nsec))
	{
	  cerr << "\nSecond control-C within a second." << endl;
      	  cerr << "Aborting execution and returning to command line." << endl;
	  abortFlag = true;  // treat this as an abort
	  return false;
	}
      timeAtLastInterrupt = timeValue;
      //
      //	Doesn't meet either criteria for returning to command line.
      //
      cerr << "\nControl-C while suspended on external event(s)." << endl;
      if (rewriteCountAtLastInterrupt != -1)
	{
	  Int64 diff = currentRewriteCount - rewriteCountAtLastInterrupt;
	  cerr << "Note that this is a" << Tty(Tty::RED) << " different " << Tty(Tty::RESET) <<
	    "suspension than the one that received a control-C " <<
	    diff << " rewrite" << pluralize(diff) << " ago." << Tty(Tty::RESET) << endl;
	}
      cerr << "A second control-C" << Tty(Tty::RED) <<
	" on the same suspension or within a second " <<
	Tty(Tty::RESET) << "will abort execution and return to command line." << endl;
      rewriteCountAtLastInterrupt = currentRewriteCount;
      ctrlC_Flag = false;  // cancel ctrlC
      return true;
    }
  return true;
}

bool
UserLevelRewritingContext::blockAndHandleInterrupts(sigset_t* normalSet)
{
  //
  //	This is called because we are about to make a blocking system
  //	call and could miss signals that are delivered before the call
  //	happens. We need to block those signals and then handle any
  //	that have already been delivered.
  //
  //	We first block ^C and info signals.
  //
  sigset_t blockSet;
  sigemptyset(&blockSet);
  sigaddset(&blockSet, SIGINT);
#ifdef SIGINFO
  sigaddset(&blockSet, SIGINFO);
#else
  sigaddset(&blockSet, SIGUSR1);
#endif
  sigprocmask(SIG_BLOCK, &blockSet, normalSet);
  //
  //	We now handle any that have already been delivered as usual.
  //	We rely on the caller to reset the signal mask to normalSet once
  //	it is done with its blocking call.
  //
  return handleInterrupt();
}

void
UserLevelRewritingContext::changePrompt()
{
  if (debugLevel == 0)
    {
      ioManager.setPrompt("Maude> ");
      setDebugMode(false);
    }
  else
    {
      string prompt = "Debug(";
      prompt += int64ToString(debugLevel);  // HACK: fix when we have decent stdc++ lib
      prompt += ")> ";
      ioManager.setPrompt(prompt);
      setDebugMode(true);
    }
}

UserLevelRewritingContext::ParseResult
UserLevelRewritingContext::commandLoop()
{
  ParseResult parseResult;
  for(;;)
    {

      //cerr << "start of command\n"; RootContainer::dump(cerr);
      cout.flush();
      parseResult = NORMAL;
      ioManager.startCommand();
      int parseError = yyparse(&parseResult);
      //cout << "parser result is: " << parseError << endl;
      if (parseError || ctrlC_Flag)
	{
	  if (interactiveFlag)
	    ++lineNumber;  // assume ^C or ^D and treat it as starting a new line
	  setTraceStatus(interpreter.getFlag(Interpreter::EXCEPTION_FLAGS));
	  cleanUpParser();
	  cleanUpLexer();
	  ctrlC_Flag = false;
	}
      else
	{
	  switch (parseResult)
	    {
	    case NORMAL:
	      break;
	    case RESUME:
	    case ABORT:
	    case STEP:
	    case WHERE:
	      {
		if (debugLevel > 0)
		  return parseResult;
		IssueWarning("not in debugger.");
		break;
	      }
	    case QUIT:
	      {
		//
		//	This is the only normal exit() that can be taken.
		//
		cout << "Bye.\n";
#ifndef NO_ASSERT
		//
		//	Look for memory bugs.
		//
		delete &interpreter;
#else
		//
		//	If we are not going to delete the interpreter
		//	in the interests of a quick exit, at least clean
		//	up any XML log we may have started.
		//
		interpreter.endXmlLog();
#endif
		exit(NORMAL_EXIT);
	      }
	    }
	}
    }
}

void
UserLevelRewritingContext::setInteractive(bool status)
{
  interactiveFlag = status;
}

void
UserLevelRewritingContext::beginCommand()
{
  if (!interactiveFlag)
    cout << "==========================================\n";
  rewriteCountAtLastInterrupt = -1;
}

void
UserLevelRewritingContext::printStatusReportCommon()
{
  struct timeval t;
  gettimeofday(&t, 0);
  time_t secs = t.tv_sec;
  cerr << "====> Maude status report on " << ctime(&secs);

  Int64 mbTotal = 0;
  Int64 eqTotal = 0;
  Int64 rlTotal = 0;
  Int64 nrTotal = 0;
  Int64 vnTotal = 0;
  for (UserLevelRewritingContext* p = this; p != 0; p = p->parent)
    {
      mbTotal += p->getMbCount();
      eqTotal += p->getEqCount();
      rlTotal += p->getRlCount();
      nrTotal += p->getNarrowingCount();
      vnTotal += p->getVariantNarrowingCount();
    }
  cerr << "membership applications: " << mbTotal <<
    "\nequational rewrites: " << eqTotal <<
    "\nrule rewrites: " << rlTotal <<
    "\nvariant narrowing steps: " << nrTotal <<
    "\nnarrowing steps: " << vnTotal <<
    "\ntotal: " << mbTotal + eqTotal + rlTotal + nrTotal + vnTotal << '\n';
}

void
UserLevelRewritingContext::printStatusReport(DagNode* subject, const PreEquation* pe)
{
  printStatusReportCommon();

  cerr << "About to apply ";
  if (const SortConstraint* mb = dynamic_cast<const SortConstraint*>(pe))
    cerr << "membership axiom:\n  " << mb << '\n';
  else if (const Equation* eq = dynamic_cast<const Equation*>(pe))
    cerr << "equation:\n  " << eq << '\n';
  else if (const Rule* rl = dynamic_cast<const Rule*>(pe))
    cerr << "rule:\n  " << rl << '\n';
  else
    CantHappen("unidentified statement");
  cerr << "on redex:\n" << subject << endl;
  where(cerr);
  cerr << endl;
}

bool
UserLevelRewritingContext::handleDebug(DagNode* subject, const PreEquation* pe)
{
  //
  //	Handle unusual situations that are common to all rewrite types:
  //	(a) Abort
  //	(b) Info interrupt
  //	(c) Breakpoints
  //	(d) ^C interrupt
  //	(e) Single stepping
  //	In the latter 3 cases we drop into the debugger.
  //
  if (abortFlag)
    return true;
  if (infoFlag)
    {
      printStatusReport(subject, pe);
      infoFlag = false;
      //
      //	If we are only slow routed by an INFO signal we want
      //	to make sure we take the fast route now that we've made
      //	our report.
      //
      setTraceStatus(interpreter.getFlag(Interpreter::EXCEPTION_FLAGS));
    }
  bool broken = 0;
  Symbol* brokenSymbol = 0;
  if (interpreter.getFlag(Interpreter::BREAK))
    {
      Symbol* s = subject->symbol();
      if (interpreter.breakId(s->id()))
	{
	  broken = true;
	  brokenSymbol = s;
	}
      else
	{
	  if (pe != 0 && interpreter.breakId(pe->getLabel().id()))
	    broken = true;
	}
    }
  if (!(ctrlC_Flag || stepFlag || broken))
    return !(interpreter.getFlag(Interpreter::TRACE));  // normal tracing
  ++debugLevel;
  changePrompt();
  if (ctrlC_Flag)
    {
      if (!interactiveFlag)
	{
	  cout << '\n';
	  cleanUpLexer();
	}
      ctrlC_Flag = false;
    }
  else if (broken)
    {
      if (brokenSymbol != 0)
	cout << "break on symbol: " << brokenSymbol << '\n';
      else
	{
	  if (const SortConstraint* mb = dynamic_cast<const SortConstraint*>(pe))
	    cout << "break on labeled membership axiom:\n" << mb << '\n';
	  else if (const Equation* eq = dynamic_cast<const Equation*>(pe))
	    cout << "break on labeled equation:\n" << eq << '\n';
	  else if (const Rule* rl = dynamic_cast<const Rule*>(pe))
	    cout << "break on labeled rule:\n" << rl << '\n';
	  else if (const StrategyDefinition* sdef = dynamic_cast<const StrategyDefinition*>(pe))
	    cout << "break on labeled strategy definition:\n" << sdef << '\n';
	  else
	    CantHappen("unidentified statement");
	}
    }
  stepFlag = false;
  setTraceStatus(interpreter.getFlag(Interpreter::EXCEPTION_FLAGS));
  for(;;)
    {
      switch (commandLoop())
	{
	case RESUME:
	  {
	    --debugLevel;
	    changePrompt();
	    return !(interpreter.getFlag(Interpreter::TRACE));
	  }
	case ABORT:
	  {
	    --debugLevel;
	    changePrompt();
	    abortFlag = true;
	    setTraceStatus(true);
	    return true;
	  }
	case STEP:
	  {
	    --debugLevel;
	    changePrompt();
	    stepFlag = true;
	    setTraceStatus(true);
	    return false;
	  }
	case WHERE:
	  {
	    where(cout);
	    break;
	  }
	default:
	  CantHappen("bad value in case");
	}
    }
  return true;  // never executed
}

void
UserLevelRewritingContext::where(ostream& s)
{
  //
  //	In the case of a race condition between info and ctrl-C
  //	we want ctrl-C not to terminate the info but a second
  //	ctrl-C could. Also, once we are done we want to restore
  //	an existing ctrl-C so we can honor it.
  //
  bool savedCtrlC_Flag = ctrlC_Flag;
  ctrlC_Flag = false;

  static const char* purposeString[] =
  {
    "which arose while checking a condition during the evaluation of:",
    "which arose while doing a sort computation during the evaluation of:",
    "which arose in some unknown manner during the evaluation of:",
    "which arose while executing a top level command.",
    "which arose while doing a meta-evaluation requested by:"
  };
  s << "Current term is:\n";
  for (UserLevelRewritingContext* p = this; p != 0; p = p->parent)
    {
      s << p->root() << '\n';
      //
      //	The user might reasonably terminate a huge list
      //	of parent evaluations.
      //
      if (ctrlC_Flag)
	break;
      s << purposeString[p->purpose] << '\n';
    }
  ctrlC_Flag = savedCtrlC_Flag;
}

bool
UserLevelRewritingContext::traceAbort()
{
  return abortFlag;
}
