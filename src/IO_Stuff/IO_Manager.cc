/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2021 SRI International, Menlo Park, CA 94025, USA.

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
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

//      utility stuff
#include "macros.hh"
#include "rope.hh"

//	line editing stuff
#ifdef USE_TECLA
#if HAVE_SYS_TERMIOS_H
#include <sys/termios.h>
#endif
#include "libtecla.h"
#endif

//	IO Stuff class definitions
#include "autoWrapBuffer.hh"
#include "IO_Manager.hh"

pid_t IO_Manager::stdinOwner = 0;

IO_Manager::IO_Manager()
{
  gl = 0;
  line = 0;
  usePromptsAnyway = false;
  contFlag = false;
  wrapOut = 0;
  wrapErr = 0;
  savedOut = 0;
  savedErr = 0;
  firstUnused = 0;
  bufferEnd = 0;
  bufferSize = 0;
  buffer = 0;
}

void
IO_Manager::setCommandLineEditing(size_t lineLength, size_t historyLength)
{
#ifdef USE_TECLA
  gl = new_GetLine(lineLength, historyLength);
  //
  //	Tecla's behavior on Control-C is set as follows:
  //	GLS_UNBLOCK_SIG : ensures tecla receives a SIGINT even if we block it.
  //	  The reason we might block it is to avoid not responding to a SIGINT
  //	  that was delivered after we last checked the flag set in the signal
  //	  handler and before calling tecla.
  //	GLS_ABORT : abort partial line and return NULL (default action anyway)
  //	EINTR : set errno to this (default setting anyway)
  //
  gl_trap_signal(gl, SIGINT, GLS_UNBLOCK_SIG, GLS_ABORT, EINTR);
#endif
}

void
IO_Manager::setAutoWrap(bool lineWrapping)
{
  Assert(wrapOut == 0 && wrapErr == 0 && savedOut == 0 && savedErr == 0, "already set");
  //
  //	Set up autowrapping of standard output and standard error.
  //
  winsize w;
  {
    int columns = NONE;
    if (lineWrapping)
      {
	 columns = DEFAULT_COLUMNS;
	 if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0)
	   columns = w.ws_col;
      }
    wrapOut = new AutoWrapBuffer(cout.rdbuf(), columns, true, &waitUntilSafeToAccessStdin);
    savedOut = cout.rdbuf(wrapOut);
  }
  {
    int columns = NONE;
    if (lineWrapping)
      {
	columns = DEFAULT_COLUMNS;
	if (ioctl(STDERR_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0)
	  columns = w.ws_col;
      }
    //
    //	Because every character is flushed in cerr, we don't respect it otherwise
    //	we couldn't buffer in order to compute places to wrap.
    //
    wrapErr = new AutoWrapBuffer(cerr.rdbuf(), columns, false, &waitUntilSafeToAccessStdin);
    savedErr = cerr.rdbuf(wrapErr);
  }
}

void
IO_Manager::unsetAutoWrap()
{
  //
  //	Undo autowrapping of standard output and standard error if we
  //	originally wrapped them.
  //
  if (wrapOut != 0 && savedOut != 0)
    {
      (void) cout.rdbuf(savedOut);
      delete wrapOut;
      wrapOut = 0;
      savedOut = 0;
    }
  if (wrapErr != 0 && savedErr != 0)
    {
      (void) cerr.rdbuf(savedErr);
      delete wrapErr;
      wrapErr = 0;
      savedErr = 0;
    }
}

void
IO_Manager::waitUntilSafeToAccessStdin()
{
  //
  //	If a child process is reading from stdin we need to wait
  //	until it exits to avoid a race condition where tecla saves
  //	the terminal in a RAW state induced by the child.
  //
  if (stdinOwner)
    {
      if (stdinOwner == getpid())
	{
	  //
	  //	We're the child that is reading from stdin.
	  //
	  return;
	}
      //
      //	The child should exit once it is done with stdin
      //	so we wait for it, but leave it in a waitable state.
      //
      waitpid(stdinOwner, 0, 0);
      stdinOwner = 0;
    }
}

ssize_t
IO_Manager::getInput(char* buf, size_t maxSize, FILE* stream)
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
  //
  //	In case we have a child process that is accessing stding.
  //
  waitUntilSafeToAccessStdin();

#ifdef USE_TECLA
  if (usingTecla())
    {
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
	  contFlag = true;
	  if (line == 0)
	    return 0;
	}
      
      size_t n;
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
#endif
  //
  //	Read from stdin without using tecla.
  //	It's important that we only read a line at a time from stdin,
  //	now that we allow Maude programs to read from stdin. Otherwise
  //	characters past \n could be buffered by the lexer.
  //
  if (usePromptsAnyway)
    {
      //
      //	We don't generate continuation prompts in this case to avoid breaking IOP.
      //
      if (!contFlag)
	{
	  cout << prompt.c_str() << flush;
	  contFlag = true;
	}
    }
  return readFromStdin(buf, maxSize);
}

ssize_t
IO_Manager::readFromStdin(char* buf, size_t maxSize)
{
  //
  //	We bypass stdlib because we want to get whatever is available
  //	from a pipe without waiting for a \n to avoid breaking IOP.
  //
  //	But we can't use read() in place of this function because
  //	we don't want lexer swallowing characters past \n since they
  //	may be wanted by the object system standard stream functionality.
  //
  //	Instead we keep a local buffer. We return whatever is in the
  //	local buffer up to \n, maxSize, or the end of the buffer.
  //	If the local buffer is empty, we refill it with whatever
  //	is available from read(), and we don't make another call to
  //	read() (which could block on a pipe or socket) until the local
  //	buffer is empty.
  //
  if (firstUnused >= bufferEnd)
    {
      //
      //	No buffered characters, need to do a read() and maybe block.
      //
      if (bufferSize < maxSize)
	{
	  //
	  //	Need to reallocate our buffer first.
	  //
	  delete [] buffer;
	  buffer = new char[maxSize];
	  bufferSize = maxSize;
	}

      firstUnused = 0;
      bufferEnd = read(STDIN_FILENO, buffer, maxSize);
      if (bufferEnd <= 0)
	{
	  if (isatty(STDIN_FILENO))
	    cout << '\n' << flush;
	  return bufferEnd;  // EOF or error
	}
    }
  //
  //	Return the buffered characters, up to \n, maxSize or end of local buffer.
  //
  size_t  i = 0;
  for (;;)
    {
      char c = buffer[firstUnused];
      buf[i] = c;
      ++firstUnused;
      ++i;
      if (c == '\n' || i == maxSize || firstUnused == bufferEnd)
	break;
    }
  return i;
}

Rope
IO_Manager::getLineFromStdin(const Rope& prompt)
{
  //
  //	In case we have a child process that is accessing stding.
  //
  waitUntilSafeToAccessStdin();
  //
  //	Get a line as a Rope, possibly using Tecla.
  //
#ifdef USE_TECLA
  if (usingTecla() && isatty(STDIN_FILENO))
    {
      char* promptString = prompt.makeZeroTerminatedString();
      line = gl_get_line(gl, promptString, NULL, -1);  //  ignore any partial line left in line
      delete [] promptString;
      if (line == 0)
	return Rope();  // return empty rope on error or eof
      Rope result(line);
      line = 0;
      return result;
    }
#endif
  //
  //	Non-Tecla case. Either Tecla not compiled it, or disabled or
  //	we are getting the line from a file or pipe.
  //	We keep reading, respecting buffered characters, until we get to \n or EOF.
  //
  char* promptString = prompt.makeZeroTerminatedString();
  cout << promptString << flush;
  delete [] promptString;
  //
  //	We keep reading and accumulating characters until we hit \n, EOF or error.
  //
  Rope result;
  for (;;)
    {
      char buf[BUFFER_SIZE];
      ssize_t nrRead = readFromStdin(buf, BUFFER_SIZE);
      if (nrRead <= 0)
	break;
      Rope t(buf, nrRead);
      result += t;
      if (buf[nrRead - 1] == '\n')
	break;
    }
  return result;
}
