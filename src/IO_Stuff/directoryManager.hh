/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for managing current directory stack.
//
#ifndef _directoryManager_hh_
#define _directoryManager_hh_
#include <sys/types.h>
#include <sys/stat.h>
#include <map>
#include "stringTable.hh"

class DirectoryManager
{
public:
  void initialize();
  bool cd(const string& directory);
  int pushd(const string& directory);
  const char* popd(int prevLength = UNDEFINED);
  const char* getCwd();

  bool checkAccess(const string& directory,
		   string& fileName,
		   int mode,
		   char const* const ext[] = 0);
  bool searchPath(const char* pathVar,
		  string& directory,
		  string& fileName,
		  int mode,
		  char const* const ext[] = 0);
  void realPath(const string& path, string& resolvedPath);
  void visitFile(const string& fileName);
  bool alreadySeen(const string& directory, const string& fileName);

private:
  //
  //	We keep track of visited files by their directory path index and inode number.
  //	This is because different files on different file systems could potentially
  //	have the same inode number. If a file is deleted and its inode number reused,
  //	it will have a later modified time so we don't worry about this case.
  //
  typedef map<pair<int, ino_t>, time_t> VisitedMap;

  bool checkAccess(const string& fullName, int mode);

  StringTable directoryNames;
  Vector<int> directoryStack;
  VisitedMap visitedMap;
};

#endif
