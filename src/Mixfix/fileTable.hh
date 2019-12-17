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
//      Class for table of files that we have seen.
//
#ifndef _fileTable_hh_
#define _fileTable_hh_
#include "stringTable.hh"
#include "token.hh"

class FileTable
{
public:
  enum SpecialLineNumbers
  {
    SYSTEM_CREATED = -1,	// line number for things created by the system
    META_LEVEL_CREATED = -2,	// line number for things created in the metalevel
    COMMAND_LINE = -3,		// line number for command line argument
    AUTOMATIC = -1,
  };

  FileTable();

  void openFile(int lineNumber, const char* name, bool silent);
  void closeFile(int lineNumber);
  void beginModule(Token type, Token name);
  void endModule(int lineNumber);
  void abortEverything(int lineNumber);
  void printLineNumber(ostream& s, int lineNumber);
  bool outputLine();

private:
  NO_COPYING(FileTable);

  struct Entry
  {
    int absoluteLineNumber;
    union
    {
      int fileNameIndex;
      int moduleNameIndex;
    };
    union
    {
      int relativeLineNumber;
      int moduleTypeIndex;
    };
  };

  int binarySearch(Vector<Entry>& table, int lineNumber);

  StringTable fileNames;
  Vector<Entry> fileChanges;
  Vector<Entry> moduleChanges;
  Vector<int> fileStack;
  Vector<int> moduleStack;
  int firstSilent;
};

#endif
