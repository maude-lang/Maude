/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2010 SRI International, Menlo Park, CA 94025, USA.

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
//	main() function and misc functions.
//
#include "sys/param.h"
#ifdef ALPHA
#include <stropts.h>
#endif

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "tty.hh"


//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"
 
//      interface class definitions
#include "term.hh"
 
//      core class definitions
#include "lineNumber.hh"

//      built in stuff
#include "randomOpSymbol.hh"

//      system class definitions
#include "IO_Manager.hh"

//	mixfix frontend definitions
#include "token.hh"
#include "userLevelRewritingContext.hh"
#include "fileTable.hh"
#include "directoryManager.hh"
#include "mixfixModule.hh"
#include "interpreter.hh"
#include "global.hh"

int
main(int argc, char* argv[])
{
  //
  //	Global function declatations
  //
  void printBanner(ostream& s);
  void printHelp(const char* name);
  void printVersion();
  void createRootBuffer(FILE* fp, bool forceInteractive);
  bool includeFile(const string& directory, const string& fileName, bool silent, int lineNr);
  extern Vector<char*> pendingFiles;
  const char* isFlag(const char* arg, const char* flag);
  bool findExecutableDirectory(string& directory, string& executable);
  bool findPrelude(string& directory, string& fileName);
  void checkForPending();

  bool lineWrapping = true;
  bool handleCtrlC = true;
  bool readPrelude = true;
  bool forceInteractive = false;
  bool outputBanner = true;
  int ansiColor = UNDECIDED;
  int useTecla = UNDECIDED;

  for (int i = 1; i < argc; i++)
    {
      char* arg = argv[i];
      if (arg[0] == '-')
	{
	  if (const char* s = isFlag(arg, "-xml-log="))
	    interpreter.beginXmlLog(s);
	  else if (const char* s = isFlag(arg, "-random-seed="))
	    RandomOpSymbol::setGlobalSeed(strtoul(s, 0, 0));
	  else if (strcmp(arg, "--help") == 0)
	    printHelp(argv[0]);
	  else if (strcmp(arg, "--version") == 0)
	    printVersion();
	  else if (strcmp(arg, "-no-mixfix") == 0)
	    interpreter.setPrintFlag(Interpreter::PRINT_MIXFIX, false);
	  else if (strcmp(arg, "-ansi-color") == 0)
	    ansiColor = true;
	  else if (strcmp(arg, "-no-ansi-color") == 0)
	    ansiColor = false;
	  else if (strcmp(arg, "-tecla") == 0)
	    useTecla = true;
	  else if (strcmp(arg, "-no-tecla") == 0)
	    useTecla = false;
	  else if (strcmp(arg, "-no-prelude") == 0)
	    readPrelude = false;
	  else if (strcmp(arg, "-no-banner") == 0)
	    outputBanner = false;
	  else if (strcmp(arg, "-no-advise") == 0)
	    {
	      if (!alwaysAdviseFlag)
		globalAdvisoryFlag = false;
	    }
	  else if (strcmp(arg, "-always-advise") == 0)
	    {
	      alwaysAdviseFlag = true;
	      globalAdvisoryFlag = true;
	    }
	  else if (strcmp(arg, "-no-wrap") == 0)
	    lineWrapping = false;
	  else if (strcmp(arg, "-batch") == 0)
	    handleCtrlC = false;
	  else if (strcmp(arg, "-interactive") == 0)
	    forceInteractive = true;
	  else if (strcmp(arg, "-print-to-stderr") == 0)
	    UserLevelRewritingContext::setPrintAttributeStream(&cerr);
	  else if (strcmp(arg, "-show-pid") == 0)
	    cerr << getpid() << endl;
	  else if (strcmp(arg, "-erewrite-loop-mode") == 0)
	    interpreter.setFlag(Interpreter::EREWRITE_LOOP_MODE, true);
	  else
	    {
	      IssueWarning(LineNumber(FileTable::COMMAND_LINE) <<
			   ": unrecognised flag: " << QUOTE(arg));
	    }
	}
      else
	pendingFiles.append(arg);
    }

  if (lineWrapping)
    ioManager.setAutoWrap();

  if (ansiColor == UNDECIDED)
    {
      //
      //	By default we allow ANSI escape codes unless
      //	environment variable TERM is set to dumb; or
      //	our standard output is not a terminal.
      //
      ansiColor = true;
      const char* term = getenv("TERM");
      if ((term != 0 && strcmp("dumb", term) == 0) ||
	  isatty(STDOUT_FILENO) == 0)
	ansiColor = false;
    }
  Tty::setEscapeSequencesAllowed(ansiColor);

  bool inputIsTerminal = (isatty(STDIN_FILENO) == 1);
  if (useTecla == UNDECIDED)
    {
      //
      //	By default we use tecla for input unless
      //	environment variable TERM is set to emacs or dumb;
      //	or our standard input is not a terminal.
      //
      useTecla = true;
      const char* term = getenv("TERM");
      if ((term != 0 && (strcmp("emacs", term) == 0 ||
			 strcmp("dumb", term) == 0)) ||
	  !inputIsTerminal)
	useTecla = false;
    }
  //
  //	Make sure we flush cout before we output any error messages so things hit the tty in a consistent order.
  //
  (void) cerr.tie(&cout);

  if (outputBanner)
    printBanner(cout);
  createRootBuffer(stdin, forceInteractive);
  UserLevelRewritingContext::setHandlers(handleCtrlC);
  if (useTecla)
    ioManager.setCommandLineEditing();
  if (inputIsTerminal || forceInteractive)
    {
      //
      //	Prompt for input from stdio, even if useTecla == false, or Tecla is not linked
      //
      ioManager.setUsePromptsAnyway();
    }
  directoryManager.initialize();
  string executable(argv[0]);
  findExecutableDirectory(executableDirectory, executable);
  if (readPrelude)
    {
      string directory;
      string fileName(PRELUDE_NAME);
      if (findPrelude(directory, fileName))
	includeFile(directory, fileName, true, FileTable::AUTOMATIC);
    }
  else
    checkForPending();  // because we won't hit an EOF
  (void) UserLevelRewritingContext::commandLoop();
  return 0;
}

ostream& operator<<(ostream& s, const LineNumber& lineNumber)
{
  fileTable.printLineNumber(s, lineNumber.getLineNumber());
  return s;
}

const char*
isFlag(const char* arg, const char* flag)
{
  char f;
  do
    {
      f = *flag++;
      if (f == 0)
        return arg;
    }
  while (f == *arg++);
  return 0;
}

void
printHelp(const char* name)
{
  cout <<
    "Maude interpreter\n" <<
    "Usage: " << name << " [options] [files]\n" <<
    "Options:\n" <<
    "  --help\t\tDisplay this information\n" <<
    "  --version\t\tDisplay version number\n" <<
    "  -no-prelude\t\tDo not read in the standard prelude\n" <<
    "  -no-banner\t\tDo not output banner on startup\n" <<
    "  -no-advise\t\tNo advisories on startup\n" <<
    "  -always-advise\tAlways show advisories regardless\n" <<
    "  -no-mixfix\t\tDo not use mixfix notation for output\n" <<
    "  -no-wrap\t\tDo not automatic line wrapping for output\n" <<
    "  -ansi-color\t\tUse ANSI control sequences\n" <<
    "  -no-ansi-color\tDo not use ANSI control sequences\n" <<
    "  -tecla\t\tUse tecla command line editing\n" <<
    "  -no-tecla\t\tDo not use tecla command line editing\n" <<
    "  -batch\t\tRun in batch mode\n" <<
    "  -interactive\t\tRun in interactive mode\n" <<
    "  -print-to-stderr\tPrint attribute should use stderr rather than stdout\n" <<
    "  -random-seed=<int>\tSet seed for random number generator\n" <<
    "  -xml-log=<filename>\tSet file in which to produce an xml log\n" <<
    "  -get-pid\t\tPrint process id to stderr before printing banner\n" <<
    "  -erewrite-loop-mode\tUse external object rewriting for loop mode\n" <<
    "\n" <<
    "Send bug reports to: " << PACKAGE_BUGREPORT << endl;
  exit(0);
}

void
printVersion()
{
  cout << PACKAGE_VERSION << endl;
  exit(0);
}

bool
findExecutableDirectory(string& directory, string& executable)
{
  string::size_type p = executable.rfind('/');
  if (p == string::npos)
    return directoryManager.searchPath("PATH", directory, executable, X_OK);
  else
    {
      directoryManager.realPath(executable.substr(0, p), directory);
      return directoryManager.checkAccess(directory, executable, X_OK);
    }
}

bool
findPrelude(string& directory, string& fileName)
{
  if (directoryManager.searchPath(MAUDE_LIB, directory, fileName, R_OK))
    return true;
  if (!(executableDirectory.empty()) &&
      directoryManager.checkAccess(executableDirectory, fileName, R_OK))
    {
      directory = executableDirectory;
      return true;
    }
  if (directoryManager.checkAccess(".", fileName, R_OK))
    {
      directory = ".";
      return true;
    }
  IssueWarning(LineNumber(FileTable::AUTOMATIC) <<
	       ": unable to locate file: " << QUOTE(fileName));
  return false;
}
