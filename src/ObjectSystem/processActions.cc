/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020-2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Main process manipulation code.
//
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

bool
ProcessManagerSymbol::getChildProcess(DagNode* processArg, int& processId, ChildProcess*& cpp)
{
  if (processArg->symbol() == processOidSymbol)
    {
      DagNode* idArg = safeCast(FreeDagNode*, processArg)->getArgument(0);
      if (succSymbol->getSignedInt(idArg, processId))
	{
	  ProcessMap::iterator i = childProcesses.find(processId);
	  if (i != childProcesses.end())
	    {
	      cpp = &(i->second);
	      return true;
	    }
	}
    }
  return false;
}

bool
ProcessManagerSymbol::makeNonblockingSocketPair(int pair[2],
						FreeDagNode* message,
						ObjectSystemRewritingContext& context,
						bool readOnly)
{
  //
  //	pair[0] will be the parent side of the socket and it needs to
  //	be nonblocking because SocketManagerSymbol assumes nonblocking
  //	reads and writes.
  //
  //	We also set the close-on-execute flag so that the file descriptors
  //	aren't leaked into future children.
  //
  //	If the readOnly flag is set we shutdown() the write half of
  //	pair[0] for cleanliness.
  //
  const char* errText;
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, pair) != -1)
    {
      //
      //	Nonblocking is done by modifying the file status flags.
      //
      int flags = fcntl(pair[0], F_GETFL);
      if (flags != -1 && fcntl(pair[0], F_SETFL, flags | O_NONBLOCK) != -1)
	{
	  //
	  //	Close on execute is done by modifying the file descriptor flags.
	  //
	  int flags2 = fcntl(pair[0], F_GETFD);
	  if (flags2 != -1 && fcntl(pair[0], F_SETFD, flags2 | FD_CLOEXEC) != -1)
	    {
	      if (!readOnly || shutdown(pair[0], SHUT_WR) != -1)
		return true;
	    }
	}
      errText = strerror(errno);  // close() is allowed to change errno
      //
      //	Must not leak file descriptors.
      //
      close(pair[0]);
      close(pair[1]);
    }
  else
    errText = strerror(errno);
  //
  //	One of the system calls failed. Return error message.
  //
  errorReply(errText, message, context);
  return false;
}

bool
ProcessManagerSymbol::makeCloseOnExitPipe(int pair[2],
					  FreeDagNode* message,
					  ObjectSystemRewritingContext& context)
{
  //
  //	We create a pipe where the writing end will be closed on execve()
  //	to allow detection of successful execve().
  //
  const char* errText;
  if (pipe(pair) != -1)
    {
      //
      //	Close on execute is done by modifying the file descriptor flags.
      //
      int flags = fcntl(pair[WRITE_END], F_GETFD);
      if (flags != -1 && fcntl(pair[WRITE_END], F_SETFD, flags | FD_CLOEXEC) != -1)
	return true;
      errText = strerror(errno);  // close() is allowed to change errno
      //
      //	Must not leak file descriptors.
      //
      close(pair[READ_END]);
      close(pair[WRITE_END]);
    }
  else
    errText = strerror(errno);
  errorReply(errText, message, context);
  return false;
}

bool
ProcessManagerSymbol::createProcess(FreeDagNode* message,
				    ObjectSystemRewritingContext& context)
{
  Assert(message->getArgument(0)->symbol() == this, "misdirected message");
  //
  //	We will allow (but not insist on) a full path to the executable file.
  //
  DagNode* executableArg = message->getArgument(2);
  if (executableArg->symbol() != stringSymbol)
    {
      IssueAdvisory("process manager declined malformed message " << QUOTE(message) << '.');
      return false;
    }
  //
  //	Check we have a list of strings and count the length.
  //
  DagNode* argumentsArg = message->getArgument(3);
  int nrArgs = checkStringList(argumentsArg);
  if (nrArgs == -1)
    {
      IssueAdvisory("process manager declined malformed message " << QUOTE(message) << '.');
      return false;
    }
  //
  //	Currently we don't support any options but eventually we
  //	will stuff enviroment variable changes in here.
  //
  DagNode* optionsArg = message->getArgument(4);
  if (optionsArg->symbol() != emptyProcessOptionSetSymbol)
    {
      IssueAdvisory("process manager declined malformed message " << QUOTE(message) << '.');
      return false;
    }
  //
  //	Request looks well formed. Now see if we're allowed to
  //	created processes.
  //
  if (!allowProcesses)
    {
      IssueAdvisory("execution of arbitrary binaries disabled.");
      errorReply("process creation disabled", message, context);
      return true;
    }
  //
  //	Socket pair to communicate with stdin/stdout of created process.
  //
  int ioSockets[2];
  if (!makeNonblockingSocketPair(ioSockets, message, context, false))
    return true;
  //
  //	Socket pair to communicate with stderr of created process.
  //
  int errSockets[2];
  if (!makeNonblockingSocketPair(errSockets, message, context, true))
    {
      //
      //	Must not leak file descriptors.
      //
      close(ioSockets[0]);
      close(ioSockets[1]);
      return true;
    }
  //
  //	Now we need to deal with the issue that the child may
  //	fail to execute a new image and we need to recognize this
  //	in the parent. We do this with a close on execute pipe.
  //
  int failureReturnPipe[2];
  if(!makeCloseOnExitPipe(failureReturnPipe, message, context))
    {
      //
      //	Must not leak file descriptors.
      //
      close(errSockets[0]);
      close(errSockets[1]);
      close(ioSockets[0]);
      close(ioSockets[1]);
      return true;
    }
  //
  //	Now we spawn a child process.
  //
  pid_t pid = fork();
  if (pid == -1)
    {
      const char* errText = strerror(errno);
      //
      //	Must not leak file descriptors.
      //
      close(failureReturnPipe[READ_END]);
      close(failureReturnPipe[WRITE_END]);
      close(errSockets[0]);
      close(errSockets[1]);
      close(ioSockets[0]);
      close(ioSockets[1]);
      errorReply(errText, message, context);
      return true;
    }
  if (pid == 0)
    {
      //
      //	We are the child.
      //
      close(ioSockets[0]);  // close parent end
      close(failureReturnPipe[READ_END]);  // close read end
      dup2(ioSockets[1], STDIN_FILENO);  // replace stdin with ioSockets[1]
      dup2(ioSockets[1], STDOUT_FILENO);  // replace stdout with ioSockets[1]
      close(errSockets[0]);  // close parent end
      dup2(errSockets[1], STDERR_FILENO);  // replace stderr with errSockets[1]
      //
      //	Make the argument vector.
      //
      //	We never free the strings we allocate via makeZeroTerminatedString()
      //	because we will either execvp() or exit().
      //
      const Rope& rope = safeCast(StringDagNode*, executableArg)->getValue();
      const char* file = rope.makeZeroTerminatedString();
      char* const* argv = makeStringArray(executableArg, argumentsArg, nrArgs);
      //
      //	Replace Maude binary with file; on success failureReturnPipe[WRITE_END]
      //	will be closed, enabling the parent to see our success.
      //
      (void) execvp(file, argv);
      //
      //	We can only reach here if the execvp() call failed.
      //	We pass an error message back though failureReturnPipe.
      //
      const char* errText = strerror(errno);
      size_t length = strlen(errText);
      while (length > 0)
	{
	  ssize_t nrChars = write(failureReturnPipe[WRITE_END], errText, length);
	  if (nrChars < 0)
	    break;
	  length -= nrChars;
	  errText += nrChars;
	}
      close(failureReturnPipe[WRITE_END]);  // just to be sure characters are in pipe
      exit(1);
    }
  //
  //	We are the parent.
  //
  close(failureReturnPipe[WRITE_END]);  // close write end
  close(errSockets[1]);  // close child end
  close(ioSockets[1]);  // close child end
  //
  //	Now did the child succeed in executing a new image?
  //	We read the failureReturnPipe up to EOF to find out.
  //
  Rope errorMessage;
  char buffer[ERROR_BUFFER_SIZE];
  for (;;)
    {
      int nrChar = read(failureReturnPipe[READ_END], buffer, ERROR_BUFFER_SIZE);
      if (nrChar <= 0)
	break;
      errorMessage += Rope(buffer, nrChar);
    }
  if (!(errorMessage.empty()))
    {
      //
      //	Non-empty error message means execvp() failed in the child.
      //	Close file descriptors and inject a processError() reply.
      //
      close(failureReturnPipe[READ_END]);
      close(errSockets[0]);
      close(ioSockets[0]);
      errorReply(errorMessage, message, context);
      return true;
    }
  //
  //	No characters before EOF indicates execvp() succeeded.
  //	We pass control of the sockets over to SocketManagerSymbol.
  //
  DagNode* ioSocketName = socketManagerSymbol->manageSocket(ioSockets[0],
							    false,
							    false,
							    context);
  DagNode* errSocketName = socketManagerSymbol->manageSocket(errSockets[0],
							     false,
							     true,
							     context);
  //
  //	Make new ChildProcess record.
  //
  childProcesses[pid].ioSocket = ioSockets[0];
  childProcesses[pid].errSocket = errSockets[0];

  Vector<DagNode*> reply(5);
  reply.resize(1);
  reply[0] = succSymbol->makeNatDag(pid);
  DagNode* processName = processOidSymbol->makeDagNode(reply);
  context.addExternalObject(processName, this);

  reply.resize(5);
  reply[2] = processName;
  reply[3] = ioSocketName;
  reply[4] = errSocketName;
  reply[1] = message->getArgument(0);
  DagNode* target = message->getArgument(1);
  reply[0] = target;
  DagNode* replyMsg = createdProcessMsg->makeDagNode(reply);
  DebugAdvisory("placing " << replyMsg << " in the buffer");
  context.bufferMessage(target, replyMsg);
  return true;
}

int
ProcessManagerSymbol::checkStringList(DagNode* stringList)
{
  //
  //	Returns length of list or -1 if bad.
  //
  Symbol* s = stringList->symbol();
  if (s == stringListSymbol)
    {
      //
      //	List of >=2 strings.
      //
      int count = 0;
      for (DagArgumentIterator i(stringList); i.valid(); i.next())
	{
	  if (i.argument()->symbol() != stringSymbol)
	    return -1;
	  ++count;
	}
      return count;
    }
  else if (s == stringSymbol)
    return 1;  // singleton case
  else if (s == nilStringListSymbol)
    return 0;  // empty case
  return -1;
}

char* const*
ProcessManagerSymbol::makeStringArray(DagNode* zeroArgument,
				      DagNode* remainingArguments,
				      int nrRemainingArguments)
{
  char** array = new char*[1 + nrRemainingArguments + 1];
  const Rope& rope = safeCast(StringDagNode*, zeroArgument)->getValue();
  array[0] = rope.makeZeroTerminatedString();

  int index = 1;
  Symbol* s = remainingArguments->symbol();
  if (s == stringListSymbol)
    {
      //
      //	List of >=2 strings.
      //
      for (DagArgumentIterator i(remainingArguments); i.valid(); i.next(), ++index)
	{
	  const Rope& rope = safeCast(StringDagNode*, i.argument())->getValue();
	  array[index] = rope.makeZeroTerminatedString();
	}
    }
  else if (s == stringSymbol)
    {
      //
      //	Singleton case.
      //
      const Rope& rope = safeCast(StringDagNode*, remainingArguments)->getValue();
      array[index] = rope.makeZeroTerminatedString();
      ++index;
    }

  array[index] = 0;
  return array;
}

bool
ProcessManagerSymbol::waitForExit(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  DebugEnter("message = " << message);
  //
  //	If we already saw a waitForExit() for this process, either
  //	it completed and removed the child from ProcessMap OR we
  //	have a pending waitForExit() in which case we fill out
  //	the waitContext to allow for an async reply. Either
  //	way we don't accept a second waitForExit() message.
  //
  DagNode* processName = message->getArgument(0);
  int processId;
  ChildProcess* cpp;
  if (getChildProcess(processName, processId, cpp))
    {
      //
      //	If we already have an incomplete waitForExit() we don't
      //	accept a second one.
      //
      if (cpp->waitContext != 0)
	{
	  IssueAdvisory(processName << " declined extra waitForExit() message.");
	  return false;
	}
      //
      //	We need to request a callback before we call waitpid() because
      //	callbacks rely on SIGCHLD and the signal could arrive between
      //	the waitpid() call and the callback request and be lost.
      //
      requestChildExitCallback(processId);
      //
      //	If child already exited, there won't be a signal and hence
      //	no callback.
      //
      int wstatus;
      pid_t exitPid = waitpid(processId, &wstatus, WNOHANG);
      Assert(exitPid != -1, "unexpect error from waitpid() for process " << processId);
      if (exitPid != 0)
	{
	  Assert(exitPid == processId, "waitpid() for " << processId << " got " << exitPid);
	  //
	  //	Child status changes; make sure it is an exit.
	  //
	  if (WIFEXITED(wstatus))
	    {
	      //
	      //	Normal exit with exit code.
	      //
	      cancelChildExitCallback(processId);
	      int exitCode = WEXITSTATUS(wstatus);
	      exitedReply(processId, exitCode, message, context);
	      return true;
	    }
	  else if (WIFSIGNALED(wstatus))
	    {
	      //
	      //	Terminated by signal.
	      //
	      cancelChildExitCallback(processId);
	      int terminatingSignal = WTERMSIG(wstatus);
	      exitedReply(processId, ~terminatingSignal, message, context);
	      return true;
	    }
	  else
	    {
	      //
	      //	Some other status change. This means child was alive when
	      //	waitpid() was called so we can rely on the callback.
	      //
	    }
	}
      else
	{
	  //
	  //	No status change. This means child was alive when
	  //	waitpid() was called so we can rely on the callback.
	  //
	}
      //
      //	We will rely on the callback we just requested so we need to
      //	save the context and message for later.
      //
      cpp->waitContext = &context;
      cpp->waitMessage.setNode(message);
      return true;
    }
  IssueAdvisory("no process to receive message " << QUOTE(message) << '.');
  return false;
}

int
ProcessManagerSymbol::getSignalNumber(const char* signalString)
{
#define MACRO(signalName)			  \
  if (strcmp(signalString, #signalName) == 0)	  \
    return signalName;
#include "supportedSignals.cc"
#undef MACRO
  return NONE;
}

bool
ProcessManagerSymbol::signalProcess(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  DagNode* signalName = message->getArgument(2);
  if (signalName->symbol() == stringSymbol)
    {
      const Rope& rope = safeCast(StringDagNode*, signalName)->getValue();
      char* signalString = rope.makeZeroTerminatedString();
      int signalNumber = getSignalNumber(signalString);
      delete [] signalString;
      if (signalNumber != NONE)
	{
	  DagNode* processName = message->getArgument(0);
	  int processId;
	  ChildProcess* cpp;
	  if (getChildProcess(processName, processId, cpp))
	    {
	      kill(processId, signalNumber);
	      //
	      //	Make signaledProcess() message.
	      //
	      DagNode* target = message->getArgument(1);
	      Vector<DagNode*> reply(2);
	      reply[0] = target;
	      reply[1] = processName;
	      context.bufferMessage(target, signaledProcessMsg->makeDagNode(reply));
	      return true;
	    }
	  IssueAdvisory("no process to receive message " << QUOTE(message) << '.');
	}
    }
  else
    IssueAdvisory("malformed message " << QUOTE(message) << '.');
  return false;
}
