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
//      Class for managing yucky I/O issues.
//
#ifndef _IO_Manager_hh_
#define _IO_Manager_hh_

struct GetLine;  // to avoid sucking in the tecla header file

class IO_Manager
{
  NO_COPYING(IO_Manager);

public:
  enum Defaults
  {
    MAX_LINE_LENGTH = 1024,
    MAX_HISTORY_LENGTH = 4096,
    DEFAULT_COLUMNS = 80,
    DEFAULT_LINES = 25
  };

  IO_Manager();

  void setAutoWrap();
  void setCommandLineEditing(size_t maxLineLength = MAX_LINE_LENGTH,
			     size_t maxHistoryLength = MAX_HISTORY_LENGTH);
  void setPrompt(const string& newPrompt);
  void setContPrompt(const string& newContPrompt);
  void startCommand();
  int getInput(char* buf, int maxSize, FILE* stream);

private:
  GetLine* gl;
  const char* line;
  bool contFlag;
  string prompt;
  string contPrompt;
  AutoWrapBuffer* wrapOut;
  AutoWrapBuffer* wrapErr;
};

inline void
IO_Manager::setPrompt(const string& newPrompt)
{
  prompt = newPrompt;
}

inline void
IO_Manager::setContPrompt(const string& newContPrompt)
{
  contPrompt = newContPrompt;
}

inline void
IO_Manager::startCommand()
{
  contFlag = false;
}

#endif
