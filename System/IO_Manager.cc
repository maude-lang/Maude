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
//      Implementation for class IO_Manager.
//

#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/termios.h>
#include <errno.h>
#include "libtecla.h"

//      utility stuff
#include "macros.hh"

#include "autoWrapBuffer.hh"
#include "IO_Manager.hh"

IO_Manager::IO_Manager()
{
  gl = 0;
  line = 0;
  contFlag = false;
  wrapOut = 0;
  wrapErr = 0;
}

void
IO_Manager::setCommandLineEditing(size_t lineLength, size_t historyLength)
{
  gl = new_GetLine(lineLength, historyLength);
  gl_trap_signal(gl, SIGINT, 0, GLS_ABORT, EINTR);
}

void
IO_Manager::setAutoWrap()
{
  //
  //	Set up autowrapping of standard output and standard error.
  //
  winsize w;

  int columns = DEFAULT_COLUMNS;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0)
    columns = w.ws_col;
  // cout << "out columns " << columns << '\n';
  wrapOut = new AutoWrapBuffer(cout.rdbuf(), columns);
  (void) cout.rdbuf(wrapOut);

  columns = DEFAULT_COLUMNS;
  if (ioctl(STDERR_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0)
    columns = w.ws_col;
  // cout << "err columns " << columns << '\n';
  wrapErr = new AutoWrapBuffer (cerr.rdbuf(), columns);
  (void) cerr.rdbuf(wrapErr);
}

int
IO_Manager::getInput(char* buf, int maxSize, FILE* stream)
{
  if (stream != stdin)
    {
      //
      //	Some stdio libraries (notably that of linux) have a nasty habit
      //	of restarting slow system calls aborted by signals. We avoid
      //	this behaviour by doing input directly from the OS.
      //
      return read(fileno(stream), buf, maxSize);
    }

  if (gl == 0)
    {
      //
      //	Reading from stdin without using tecla.
      //
      if (!contFlag)
	{
	  
	  fputs(prompt.c_str(), stdout);  // HACK: bypass line wrapper
	  fflush(stdout);
	}
      contFlag = true;
      return read(fileno(stream), buf, maxSize);
    }

  if (line == 0)
    {
    
      line = gl_get_line(gl,
			 contFlag ? contPrompt.c_str() : prompt.c_str(),
			 NULL,
			 -1);
      GlTerminalSize ts = gl_terminal_size(gl, DEFAULT_COLUMNS, DEFAULT_LINES);
      if (wrapOut != 0)
	wrapOut->setLineWidth(ts.ncolumn);
      if (wrapErr != 0)
	wrapErr->setLineWidth(ts.ncolumn);
#ifdef TECLA_DEBUG
      // use printf() to avoid line wrapping
      if (line == 0)
	printf("returned 0\n");
      else
	printf("returned(%s)\n", line);
#endif
      contFlag = true;
      if (line == 0)
	return 0;
    }

  int n;
  for (n = 0;; n++)
    {
      char c = *line;
      if (c == '\0')
	{
	  line = 0;
	  break;
	}
      if (n == maxSize)
	break;
      *buf++ = c;
      ++line;
    }
  return n;
}
