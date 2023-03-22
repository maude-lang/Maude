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
//      Implementation for class StreamManagerSymbol.
//
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "NA_Theory.hh"
#include "S_Theory.hh"
#include "builtIn.hh"
#include "objectSystem.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "symbolMap.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//      built in class definitions
#include "succSymbol.hh"
#include "minusSymbol.hh"
#include "stringSymbol.hh"
#include "stringDagNode.hh"
#include "bindingMacros.hh"

//	object system class definitions
#include "objectSystemRewritingContext.hh"
#include "streamManagerSymbol.hh"

#include "../IO_Stuff/IO_Manager.hh"  // HACK
extern IO_Manager ioManager;  // HACK

StreamManagerSymbol::StreamManagerSymbol(int id)
  : ExternalObjectManagerSymbol(id)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolName = 0;
#include "streamSignature.cc"
#undef MACRO
}

bool
StreamManagerSymbol::attachData(const Vector<Sort*>& opDeclaration,
				const char* purpose,
				const Vector<const char*>& data)
{
  if (data.length() == 1)
    {
      const char* streamName = data[0];
      if (strcmp(streamName, "stdin") == 0)
	streamNr = STDIN_FILENO;
      else if(strcmp(streamName, "stdout") == 0)
	streamNr = STDOUT_FILENO;
      else if(strcmp(streamName, "stderr") == 0)
	streamNr = STDERR_FILENO;
      else
	return ExternalObjectManagerSymbol::attachData(opDeclaration, purpose, data);
      return true;
    }
  return ExternalObjectManagerSymbol::attachData(opDeclaration, purpose, data);
}

bool
StreamManagerSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  Assert(symbol != 0, "null symbol for " << purpose);
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  BIND_SYMBOL(purpose, symbol, SymbolName, SymbolClass*)
#include "streamSignature.cc"
#undef MACRO
  return ExternalObjectManagerSymbol::attachSymbol(purpose, symbol);
}

void
StreamManagerSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  StreamManagerSymbol* orig = safeCast(StreamManagerSymbol*, original);
  streamNr = orig->streamNr;
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  COPY_SYMBOL(orig, SymbolName, map, SymbolClass*)
#include "streamSignature.cc"
#undef MACRO
  ExternalObjectManagerSymbol::copyAttachments(original, map);
}

void
StreamManagerSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				      Vector<const char*>& purposes,
				      Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "StreamManagerSymbol";
  data.resize(nrDataAttachments + 1);
  data[nrDataAttachments].resize(1);
  data[nrDataAttachments][0] = (streamNr == STDIN_FILENO) ? "stdin" :
    ((streamNr == STDOUT_FILENO) ? "stdout" : "stderr");
  ExternalObjectManagerSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
StreamManagerSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					  Vector<Symbol*>& symbols)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  APPEND_SYMBOL(purposes, symbols, SymbolName)
#include "streamSignature.cc"
#undef MACRO
  ExternalObjectManagerSymbol::getSymbolAttachments(purposes, symbols);
}

bool
StreamManagerSymbol::handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  DebugEnter("saw " << message);
  Symbol* s = message->symbol();
  if (s == writeMsg)
    return write(safeCast(FreeDagNode*, message), context);
  if (s == getLineMsg)
    return getLine(safeCast(FreeDagNode*, message), context);
  if (s == cancelGetLineMsg)
    return cancelGetLine(safeCast(FreeDagNode*, message), context);
  return false;
}

bool
StreamManagerSymbol::handleMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  CantHappen("StreamManagerSymbol::handleMessage() : " << message);
  return false;
}

void
StreamManagerSymbol::cleanUp(DagNode* objectId)
{
  CantHappen("StreamManagerSymbol::cleanUp() : " << objectId);
}

void
StreamManagerSymbol::cleanUpManager(ObjectSystemRewritingContext& context)
{
  //
  //	This is called during the destructor for context to warn us
  //	that we no longer make use of its member functions.
  //
  //	It's possible that we already finished up any pending getLine()
  //	that was using this context, but it's also possible that we
  //	didn't and a future PseudoThread callback could cause memory
  //	corruption.
  //
  PendingGetLineMap::iterator p;
  if (findPendingGetLine(context, p))
    {
      //
      //	We had a pending getLine() that was intending to insert a
      //	reply into context. We must make sure this never happens.
      //
      int pipeFd = p->first;
      DebugInfo("cleaning up pending getLine() associated with fd " << pipeFd);
      //
      //	First we cancel any pending callback events from PseudoThread.
      //
      PseudoThread::clearFlags(pipeFd);
      //
      //	Close the read end of pipe.
      //
      close(pipeFd);
      //
      //	Delete the pending getLine().
      //
      pendingGetLines.erase(p);
    }
}

bool
StreamManagerSymbol::write(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  if (streamNr == STDOUT_FILENO || streamNr == STDERR_FILENO)
    {
      if ((streamNr == STDOUT_FILENO) ?
	  IO_Manager::safeToAccessStdout() :
	  IO_Manager::safeToAccessStderr())
	{
	  DagNode* textArg = message->getArgument(2);
	  if (textArg->symbol() == stringSymbol)
	    {
	      Rope text = safeCast(StringDagNode*, textArg)->getValue();
	      if (!(text.empty()))
		{
		  if (streamNr == STDOUT_FILENO)
		    cout << text << flush;
		  else
		    cerr << text;
		  trivialReply(wroteMsg, message, context);
		}
	      else
		errorReply("Empty string.", message, context);
	    }
	  else
	    errorReply("Bad string.", message, context);
	  return true;
	}
      //
      //	Terminal is busy. We leave the message in the configuration
      //	but we don't want to produce an advisory.
      //
    }
  else
    IssueAdvisory(message->getArgument(0) << " declined message " << message);
  return false;
}

bool
StreamManagerSymbol::getLine(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  if (streamNr == STDIN_FILENO)
    {
      if (IO_Manager::safeToAccessStdin())
	{
	  DagNode* promptArg = message->getArgument(2);
	  if (promptArg->symbol() == stringSymbol)
	    {
	      if (isatty(STDIN_FILENO))
		nonblockingGetLine(message, context);
	      else
		{
		  //
		  //	Can't safely read lines from a file in a separate
		  //	process because we don't update the file position.
		  //
		  const Rope& prompt = safeCast(StringDagNode*, promptArg)->getValue();
		  Rope line = ioManager.getLineFromStdin(prompt);
		  gotLineReply(line, message, context);
		}
	    }
	  else
	    errorReply("Bad string.", message, context);
	  return true;
	}
      //
      //	Terminal is busy. We leave the message in the configuration
      //	but we don't want to produce an advisory.
      //
    }
  else
    IssueAdvisory(message->getArgument(0) << " declined message " << message);
  return false;
}

bool
StreamManagerSymbol::findPendingGetLine(ObjectSystemRewritingContext& context,
					PendingGetLineMap::iterator& ref)
{
  //
  //	We find the pending getLine() transaction that was created in
  //	the given context (there may be others in different contexts
  //	thanks to the debugger).
  //
  for (PendingGetLineMap::iterator p = pendingGetLines.begin();
       p != pendingGetLines.end(); ++p)
    {
      if (p->second.objectContext == &context)
	{
	  ref = p;
	  return true;
	}
    }
  return false;
}

bool
StreamManagerSymbol::cancelGetLine(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  if (streamNr == STDIN_FILENO)
    {
      PendingGetLineMap::iterator p;
      if (findPendingGetLine(context, p))
	{
	  //
	  //	Child is still reading from stdin; try to kill it.
	  //
	  if (kill(p->second.childPid, SIGTERM) == 0)
	    {
	      //
	      //	Killed it; wait for exit.
	      //
	      IO_Manager::waitUntilSafeToAccessStdin();
	      //
	      //	Kill any pending callbacks for pipe.
	      //
	      int pipeFd = p->first;
	      PseudoThread::clearFlags(pipeFd);
	      //
	      //	Close the read end of pipe.
	      //
	      close(pipeFd);
	      //
	      //	Inform sender of successful cancel.
	      //
	      trivialReply(canceledGetLineMsg, message, context);
	      //
	      //	Delete the pending getLine()
	      //
	      pendingGetLines.erase(p);
	    }
	}
      //
      //	If there wasn't a getLine() in progress in our current context,
      //	perhaps one was never started or it completed before we had a
      //	chance to kill it. Either way we quietly drop the message.
      //
      return true;
    }
  IssueAdvisory(message->getArgument(0) << " declined message " << message);
  return false;
}

void
StreamManagerSymbol::gotLineReply(const Rope& line,
				  FreeDagNode* originalMessage,
				  ObjectSystemRewritingContext& context)
{
  Vector<DagNode*> reply(3);
  reply[1] = originalMessage->getArgument(0);
  reply[2] = new StringDagNode(stringSymbol, line);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, gotLineMsg->makeDagNode(reply));
}

void
StreamManagerSymbol::errorReply(const char* errorMessage,
				FreeDagNode* originalMessage,
				ObjectSystemRewritingContext& context)
{
  Vector<DagNode*> reply(3);
  reply[1] = originalMessage->getArgument(0);
  reply[2] = new StringDagNode(stringSymbol, errorMessage);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, streamErrorMsg->makeDagNode(reply));
}

bool
StreamManagerSymbol::makeNonblockingPipe(int pair[2],
					 FreeDagNode* message,
					 ObjectSystemRewritingContext& context)
{
  //
  //	We create a pipe where the read end will not block. Also we
  //	close the read end on execute so that the file descriptor will
  //	not leak into future children that execute a new binary.
  //
  const char* errText;
  if (pipe(pair) != -1)
    {
      //
      //	Nonblocking is done by modifying the file status flags.
      //
      int flags = fcntl(pair[READ_END], F_GETFL);
      if (flags != -1 && fcntl(pair[READ_END], F_SETFL, flags | O_NONBLOCK) != -1)
	{
	  //
	  //	Close on execute is done by modifying the file descriptor flags.
	  //
	  int flags2 = fcntl(pair[READ_END], F_GETFD);
	  if (flags2 != -1 && fcntl(pair[READ_END], F_SETFD, flags2 | FD_CLOEXEC) != -1)
	    return true;
	}
      errText = strerror(errno);  // close() is allowed to change errno
      //
      //	Must not leak file descriptors.
      //
      close(pair[READ_END]);
      close(pair[WRITE_END]);
    }
  else
    errText = strerror(errno);
  //
  //	One of the system calls failed. Return error message.
  //
  errorReply(errText, message, context);
  return false;
}

void
StreamManagerSymbol::interruptHandler(int)
{
  //
  //	We expect that we will have an incomplete line bacause we're not
  //	using Tecla so we force a newline before exiting.
  //
  if (::write(STDIN_FILENO, "\n", 1))  // exiting the subprocess so we *really* don't care about the result
    ;
  exit(0);
}

void
StreamManagerSymbol::nonblockingGetLine(FreeDagNode* message,
					ObjectSystemRewritingContext& context)
{
  int resultReturnPipe[2];
  if (!makeNonblockingPipe(resultReturnPipe, message, context))
    return;
  //
  //	Because when we fork() we will have copies of the any buffered
  //	cout characters in two processes, we need to make sure nothing is buffered.
  //
  cout << flush;
  //
  //	fork() a child to do the getLine() without blocking the parent
  //	process.
  //
  pid_t pid = fork();
  if (pid == -1)
    {
      const char* errText = strerror(errno);
      //
      //	Must not leak file descriptors.
      //
      close(resultReturnPipe[READ_END]);
      close(resultReturnPipe[WRITE_END]);
      errorReply(errText, message, context);
      return;
    }

  if (pid == 0)
    {
      //
      //	We are the child.
      //
      close(resultReturnPipe[READ_END]);
      DagNode* promptArg = message->getArgument(2);
      const Rope& prompt = safeCast(StringDagNode*, promptArg)->getValue();
      if (ioManager.usingTecla())
	{
	  //
	  //	It's possible that a control-C arrived before the
	  //	fork() or will arrive before Tecla is ready to respond
	  //	by aborting the line.
	  //
	  //	Note that since Tecla has already installed its handlers
	  //	it's not safe to mess with sigaction()
	  //
	  //	To handle this we first block the signal.
	  //
	  sigset_t blockSet;
	  sigemptyset(&blockSet);
	  sigaddset(&blockSet, SIGINT);
	  sigprocmask(SIG_BLOCK, &blockSet, 0);
	}
      else
	{
	  //
	  //	Not using Tecla so if we get a SIGINT (^C from the user)
	  //	or a SIGTERM from the parent we want to move to a new
	  //	line and exit.
	  //
	  static struct sigaction signalHandler;
	  signalHandler.sa_handler = interruptHandler;
	  sigaction(SIGINT, &signalHandler, 0);
	  sigaction(SIGTERM, &signalHandler, 0);
	}
      //
      //	Check if we've already caught a control-C.
      //
      if (context.interruptSeen())
      	exit(0);
      //
      //	Tecla is initialized so that it unblocks the SIGINT once
      //	it is ready to deal with it.
      //
      //	There is no need to unblock it ourselves after Tecla
      //	returns because we're going to exit() anyway.
      //
      //	Now we can read a line.
      //
      Rope line = ioManager.getLineFromStdin(prompt);
      //
      //	Then we need to write the line to the pipe.
      //
      char* charArray = line.makeZeroTerminatedString();
      ssize_t nrUnsent = line.length();
      char* p = charArray;
      while (nrUnsent > 0)
	{
	  ssize_t n;
	  do
	    n = ::write(resultReturnPipe[WRITE_END], p, nrUnsent);
	  while (n == -1 && errno == EINTR);
	  Assert(n != -1, "unexpected write failure: " << strerror(errno));
	  p += n;
	  nrUnsent -= n;
	}
      //
      //	No need to
      //	  delete [] charArray;
      //	or
      //	  close(resultReturnPipe[WRITE_END]);
      //	because we're terminating.
      //
      exit(0);
    }
  //
  //	We are the parent.
  //
  close(resultReturnPipe[WRITE_END]);
  //
  //	Because the child will be accessing the terminal we must
  //	not access it ourselves until the child exits.
  //
  IO_Manager::setStdinOwner(pid);
  //
  //	We assume that reading a line will necessarily move stdout to a new line
  //	so we need to reset the line wrapping in our process.
  //
  ioManager.resetStdoutWrapping();
  //
  //	Set up asynchronous read of pipe.
  //	We record the information in a map from the read file descriptor
  //	to a PendingGetLine struct.
  //
  int pipeFd = resultReturnPipe[READ_END];
  PendingGetLine& p = pendingGetLines[pipeFd];
  p.lastGetLineMessage.setNode(message);
  p.objectContext = &context;
  p.childPid = pid;
  wantTo(READ, pipeFd);
  //
  //	Because the pending getLine() is not associated with an ephemeral
  //	external object that will clean it up when the context is destructed
  //	we need to request a callback for the manager itself.
  //
  context.requestCleanUpOnDestruction(this);
}

void
StreamManagerSymbol::doRead(int fd)
{
  //
  //	We find the getLine() transaction that is waiting on fd.
  //
  PendingGetLineMap::iterator p = pendingGetLines.find(fd);
  Assert(p != pendingGetLines.end(), "couldn't find pendingGetLine for " << fd);
  //
  //	We keep reading until we get a "would block" or EOF situation.
  //
  for (;;)
    {
      char buffer[READ_BUFFER_SIZE];
      ssize_t n;
      //
      //	Get as many characters as are available.
      //	Restart interrupted calls to read().
      //
      do
	n = read(fd, buffer, READ_BUFFER_SIZE);
      while (n == -1 && errno == EINTR);
      if (n > 0)
	{
	  //
	  //	We got characters; add to the incomingText.
	  //
	  p->second.incomingText += Rope(buffer, n);
	}
      else if (n == 0)
	{
	  //
	  //	EOF - assume child must have exited so finish up.
	  //
	  finishUp(p);
	  break;
	}
      else
	{
	  Assert(errno == EAGAIN || errno == EWOULDBLOCK,
		 "unexpected read() error: " << strerror(errno));
	  //
	  //	We've read all the characters but the child hasn't yet
	  //	closed the write end of the pipe so we're not seeing an EOF
	  //	condition.
	  //
	  //	There is a possibility that the child will write more
	  //	characters in the case of a very long line that couldn't be
	  //	sent in a single write().
	  //
	  //	It's also possible that the child was just about to
	  //	close() the write end when we did our read().
	  //
	  //	Either way we request another read event so when something
	  //	happens on the pipe we can respond appropriately.
	  //
	  wantTo(READ, fd);
	  break;
	}
    }
}

void
StreamManagerSymbol::doHungUp(int fd)
{
  //
  //	If the write end of the pipe has been closed, and there are no characters
  //	left to read, Linux will generate a POLLHUP event rather than a POLLIN
  //	event and a read() of length 0 that we see on BSD.
  //	Other operating systems will do both.
  //
  //	This situation arises if the user entered ^D so there has never been
  //	characters to read; or in a race where the child doesn't close() the
  //	write end of the pipe before the parent read() has returned an error
  //	(EAGAIN or WOULDBLOCK) to indicate that no characters are currently
  //	available.
  //
  PendingGetLineMap::iterator p = pendingGetLines.find(fd);
  if (p != pendingGetLines.end())
    {
      //
      //	We haven't finished up the get line.
      //
      finishUp(p);
    }
}

void
StreamManagerSymbol::finishUp(PendingGetLineMap::iterator p)
{
  //
  //	We've seen the write end of the pipe closed, either by read() returning
  //	0 characters indicating an EOF or by a POLLHUP event.
  //
  //	Close the read end of the pipe.
  //
  close(p->first);
  //
  //	We wait for child to exit if we haven't already (say while in the debugger,
  //	before accessing stdin).
  //
  IO_Manager::waitUntilSafeToAccessStdin();
  //
  //	Generate the gotLine() reply.
  //
  FreeDagNode* message = safeCast(FreeDagNode*, p->second.lastGetLineMessage.getNode());
  gotLineReply(p->second.incomingText, message, *(p->second.objectContext));
  //
  //	Remove the transaction from the pending getLine map.
  //
  pendingGetLines.erase(p);
}
