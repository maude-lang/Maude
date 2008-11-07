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
//	Code for handling user interaction.
//
#include <signal.h>

bool UserLevelRewritingContext::interactiveFlag = true;
bool UserLevelRewritingContext::ctrlC_Flag = false;
bool UserLevelRewritingContext::stepFlag = false;
bool UserLevelRewritingContext::abortFlag = false;
int UserLevelRewritingContext::debugLevel = 0;

int yyparse(void*);
void cleanUpParser();
void cleanUpLexer();

void
UserLevelRewritingContext::clearDebug()
{
  setTraceStatus(interpreter.getFlag(Interpreter::EXCEPTION_FLAGS));
  stepFlag = false;
  abortFlag = false;
}

void
UserLevelRewritingContext::clearInterrupt()
{
  setTraceStatus(interpreter.getFlag(Interpreter::EXCEPTION_FLAGS));
  ctrlC_Flag =  false;
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
#ifdef NO_ASSERT
  //
  //	If we're not debugging we handle internal errors and stack overflows.
  //
  signal(SIGBUS, internalErrorHandler);  // misaligned memory access or nonexistent real memeory
  signal(SIGILL, internalErrorHandler);  // illegal instruction

#ifdef USE_LIBSIGSEGV
  //
  //	Stack overflows are reported as SIGSEGV signals and so we need to use the
  //	libsigsegv library to heuristically distinguish the two conditions.
  //
  static char altStack[SIGSTKSZ];
  sigsegv_install_handler(sigsegvHandler);  // illegal memory access or stack overflow
  stackoverflow_install_handler(stackOverflowHandler, altStack, sizeof(altStack));
#else
  //
  //	If we can't use the library we will will catch SIGSEGVs but not install
  //	an alternative stack so that that stack overflows will show up as
  //	segmentation faults.
  //
  signal(SIGSEGV, internalErrorHandler);
#endif
#endif
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
recursion, or model checking an infinite model. It can also happen because\n\
the stacksize limit in your environment is set too low for the computation\n\
you are trying to do. You can find the value of your stacksize with the\n\
tcsh command 'limit stacksize' or the bash command 'ulimit -s'.\n\
\n\
Depending on your operating system configuration you may be able to\n\
increase your stacksize with the tcsh command 'unlimit stacksize'\n\
or the bash command 'ulimit -s unlimited'.\n\n";
  write(STDERR_FILENO, message, sizeof(message) - 1);
  _exit(1);  // don't call atexit() functions with a bad machine state
}

int
UserLevelRewritingContext::sigsegvHandler(void *fault_address, int serious)
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
other files).\n\n";
  write(STDERR_FILENO, message1, sizeof(message1) - 1);
  write(STDERR_FILENO, message2, sizeof(message2) - 1);
  write(STDERR_FILENO, message3, sizeof(message3) - 1);
  _exit(1);  // don't call atexit() functions with a bad machine state
}

void
UserLevelRewritingContext::interruptHandler(int)
{
  ctrlC_Flag =  true;
  setTraceStatus(true);
}

void
UserLevelRewritingContext::interruptHandler2(...)
{
  // windowChangedFlag = true;
}

void
UserLevelRewritingContext::changePrompt()
{
  if (debugLevel == 0)
    ioManager.setPrompt("Maude> ");
  else
    {
      string prompt = "Debug(";
      prompt += int64ToString(debugLevel);  // HACK: fix when we have decent stdc++ lib
      prompt += ")> ";
      ioManager.setPrompt(prompt);
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
      bool parseError = yyparse(&parseResult);
      //cout << "parser result is: " << parseError << endl;
      if (parseError || ctrlC_Flag)
	{
	  cout << '\n';  // terminate any partially typed in line
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
		exit(0);
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
}

bool
UserLevelRewritingContext::handleDebug(const DagNode* subject, const PreEquation* pe)
{
  if (abortFlag)
    return true;
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
	    where();
	    break;
	  }
	default:
	  CantHappen("bad value in case");
	}
    }
  return true;  // never executed
}

void
UserLevelRewritingContext::where()
{
  static const char* purposeString[] =
  {
    "which arose while checking a condition during the evaluation of:",
    "which arose while doing a sort computation during the evaluation of:",
    "which arose in some unknown manner during the evaluation of:",
    "which arose while executing a top level command.",
    "which arose while doing a meta-evaluation requested by:"
  };
  cout << "Current term is:\n";
  for (UserLevelRewritingContext* p = this; p != 0; p = p->parent)
    {
      cout << p->root() << '\n';
      if (ctrlC_Flag)
	{
	  ctrlC_Flag = false;
	  return;
	}
      cout << purposeString[p->purpose] << '\n';
    }
}

bool
UserLevelRewritingContext::traceAbort()
{
  return abortFlag;
}
