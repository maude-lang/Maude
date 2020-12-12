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
//      Class for managing yucky I/O issues.
//
#ifndef _IO_Manager_hh_
#define _IO_Manager_hh_
#include "autoWrapBuffer.hh"

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
  void unsetAutoWrap();
  void setCommandLineEditing(size_t maxLineLength = MAX_LINE_LENGTH,
			     size_t maxHistoryLength = MAX_HISTORY_LENGTH);
  void setUsePromptsAnyway();
  void setPrompt(const string& newPrompt);
  void setContPrompt(const string& newContPrompt);
  void startCommand();
  ssize_t getInput(char* buf, size_t maxSize, FILE* stream);
  Rope getLineFromStdin(const Rope& prompt);

private:
  enum Sizes
    {
      BUFFER_SIZE = 4096
    };

  ssize_t readFromStdin(char* buf, size_t maxSize);

  GetLine* gl;
  const char* line;
  bool usePromptsAnyway;  // use prompts even if command line editing disabled
  bool contFlag;
  string prompt;
  string contPrompt;
  
  AutoWrapBuffer* wrapOut;
  AutoWrapBuffer* wrapErr;
  streambuf* savedOut;
  streambuf* savedErr;

  ssize_t firstUnused;
  ssize_t bufferEnd;
  size_t bufferSize;
  char* buffer;
};

inline void
IO_Manager::setUsePromptsAnyway()
{
  usePromptsAnyway = true;
}

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
