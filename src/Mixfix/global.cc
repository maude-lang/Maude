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
//	Misc global data and functions.
//

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

int lineNumber = 1;
FileTable fileTable;
DirectoryManager directoryManager;
Interpreter& interpreter = *(new Interpreter);
//Interpreter interpreter;
IO_Manager ioManager;

string executableDirectory;

bool alwaysAdviseFlag = false;

bool
findFile(const string& userFileName, string& directory, string& fileName, int lineNr)
{
  static char const* const ext[] = {".maude", ".fm", ".obj", 0};

  string::size_type p = userFileName.rfind('/');
  if (p == string::npos)
    {
      fileName = userFileName;
      directory = ".";
      if (directoryManager.checkAccess(directory, fileName, R_OK, ext))
	return true;
      if (directoryManager.searchPath(MAUDE_LIB, directory, fileName, R_OK, ext))
	return true;
      if (!(executableDirectory.empty()) &&
	  directoryManager.checkAccess(executableDirectory, fileName, R_OK, ext))
	{
	  directory = executableDirectory;
	  return true;
	}
    }
  else if (p + 1 < userFileName.length())
    {
      directoryManager.realPath(userFileName.substr(0, p), directory);
      fileName = userFileName.substr(p + 1);
      if (directoryManager.checkAccess(directory, fileName, R_OK, ext))
	return true;
    }
  IssueWarning(LineNumber(lineNr) <<
	       ": unable to locate file: " << QUOTE(userFileName));
  return false;
}
