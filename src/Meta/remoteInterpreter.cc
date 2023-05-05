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
//      Remote metaInterpreters: parent side
//

// FIXME: This needs to live somewhere else, and perhaps be used by ProcessManagerSymbol
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

int
makeNonblockingSocketPair(int pair[2], bool readOnly)
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
  //	pair[0] and the read half of pair[1] for cleanliness.
  //
  //	On success we return 0. On failure we return errno > 0.
  //
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, pair) != -1)
    {
      int flags = fcntl(pair[0], F_GETFL);
      if (flags != -1 && fcntl(pair[0], F_SETFL, flags | O_NONBLOCK) != -1)
	{
	  int flags2 = fcntl(pair[0], F_GETFD);
	  if (flags2 != -1 && fcntl(pair[0], F_SETFD, flags2 | FD_CLOEXEC) != -1)
	    {
	      if (!readOnly || shutdown(pair[0], SHUT_WR) != -1)
		return 0;
	    }
	}
      //
      //	close() can't clear errno but perhaps it could change it
      //	in some unforseen circumstance, so we take a copy for safety.
      //
      int errorNumber = errno;
      //
      //	Must not leak file descriptors.
      //
      close(pair[0]);
      close(pair[1]);
      return errorNumber;
    }
  Assert(errno != 0, "errno shouldn't be zero if a system call failed");
  return errno;
}

DagNode*
InterpreterManagerSymbol::makeErrorReply(const Rope& errorMessage,
					 FreeDagNode* originalMessage)
{
  Vector<DagNode*> reply(3);
  reply[0] = originalMessage->getArgument(1);
  reply[1] = originalMessage->getArgument(0);
  reply[2] = new StringDagNode(stringSymbol, errorMessage);
  return interpreterErrorMsg->makeDagNode(reply);
}

bool
InterpreterManagerSymbol::createRemoteInterpreter(FreeDagNode* originalMessage,
						  ObjectSystemRewritingContext& context,
						  int id)
{
  //
  //	Socket pair for communicating with new process.
  //
  int ioSockets[2];
  if (int errNr = makeNonblockingSocketPair(ioSockets, false))
    {
      errorReply(strerror(errNr), originalMessage, context);
      return true;
    }
  int errSockets[2];
  if (int errNr = makeNonblockingSocketPair(errSockets, true))
    {
      //
      //	Must not leak file descriptors.
      //
      close(ioSockets[0]);
      close(ioSockets[1]);
      errorReply(strerror(errNr), originalMessage, context);
      return true;
    }
  //
  //	Fork a new Maude interpreter.
  //
  pid_t pid = fork();
  if (pid == -1)
    {
      //
      //	Must not leak file descriptors.
      //
      close(errSockets[0]);
      close(errSockets[1]);
      close(ioSockets[0]);
      close(ioSockets[1]);
      errorReply(strerror(errno), originalMessage, context);
      return true;
    }
  Symbol* s = originalMessage->symbol();
  MixfixModule* m = safeCast(MixfixModule*, s->getModule());
  if (pid == 0)
    {
      //
      //	We're the child.
      //
      UserLevelRewritingContext::ignoreCtrlC();  // don't respond to ctrl-C
      extern IO_Manager ioManager;  // HACK
      ioManager.unsetAutoWrap();  // don't do wrapping on our output
      //globalAdvisoryFlag = false;  // limit crap that we dump on screen
      close(ioSockets[0]);
      close(errSockets[0]);  // close parent end
      dup2(errSockets[1], STDERR_FILENO);  // replace stderr with errSockets[1]
      //
      //	We disable slow-route stuff; in particular
      //	tracing, break points, profiling and the print attribute.
      //
      RewritingContext::setTraceStatus(false);
      becomeRemoteInterpreter(ioSockets[1], m, context);  // doesn't return
    }
  //
  //	We're the parent; record details of process and sockets.
  //
  RemoteInterpreter& ri = remoteInterpreters[id];
  close(ioSockets[1]);
  close(errSockets[1]);  // close child end
  ri.processId = pid;
  ri.ioSocket = ioSockets[0];
  ri.errSocket = errSockets[0];
  ri.charArray = 0;
  ri.unsent = 0;
  ri.nrUnsent = 0;
  ri.nrPendingReplies = 0;
  ri.savedContext = &context;
  ri.module = m;
  ri.owner.setNode(originalMessage->getArgument(1));
  //
  //	We want to know if child exits unexpectedly.
  //
  //requestChildExitCallback(pid);
  //
  //	Make reply.
  //
  Vector<DagNode*> reply(3);
  reply.resize(1);
  reply[0] = metaLevel->upNat(id);
  DagNode* interpreterName = interpreterOidSymbol->makeDagNode(reply);
  context.addExternalObject(interpreterName, this);
  reply.resize(3);
  reply[2] = interpreterName;
  reply[1] = originalMessage->getArgument(0);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  DagNode* response = createdInterpreterMsg->makeDagNode(reply);
  DebugAdvisory("createRemoteInterpreter() response: " << response);
  context.bufferMessage(target, response);
  return true;
}

bool
InterpreterManagerSymbol::remoteHandleMessage(FreeDagNode* message,
					      ObjectSystemRewritingContext& context,
					      RemoteInterpreter* r)
{
  if (r->unsent != 0)
    {
      //
      //	We already have a send of a message in process so we
      //	can't start sending another one - leave it in the configuration.
      //
      return false;
    }
  DebugAdvisory("sending request " << message);
  Rope text = r->module->serialize(message);
  nonblockingSendMessage(*r, text);
  //
  //	We don't want to wait for a reply; instead we just increment
  //	the count of replies we are owed, and if this is the first one,
  //	request a callback.
  //
  if (++(r->nrPendingReplies) == 1)
    {
      //
      //	It's important that we read the error socket
      //	otherwise child could stall when the error socket is
      //	full and before it has produced a reply.
      //
      wantTo(READ, r->ioSocket);
      wantTo(READ, r->errSocket);
      requestChildExitCallback(r->processId);
    }
  DebugAdvisory("nrPendingReplies = " << r->nrPendingReplies);
  return true;
}

void
InterpreterManagerSymbol::remoteHandleReply(RemoteInterpreter* r, const Rope& reply)
{
  DagNode* replyDag = r->module->deserialize(reply);
  DagNode* target = safeCast(FreeDagNode*, replyDag)->getArgument(0);
  r->savedContext->bufferMessage(target, replyDag);
  --(r->nrPendingReplies);
  if (r->nrPendingReplies == 0)
    cancelChildExitCallback(r->processId); // so we don't hang waiting for exit
  DebugAdvisory("got a reply: " << replyDag <<
		" nrPendingReplies = " << r->nrPendingReplies);
}

void
InterpreterManagerSymbol::doChildExit(pid_t childPid)
{
  //
  //	First we wait for the child.
  //
  int wstatus;
  DebugSave(exitPid, waitpid(childPid, &wstatus, 0));
  Assert(exitPid == childPid, "unexpected return value " << exitPid <<
	 " from waidpid() for process " << childPid);
  //
  //	A little inefficient but we only expect a handfull of intepreters
  //	in existence at one time.
  //
  RemoteInterpreterMap::iterator i = remoteInterpreters.begin();
  for (; i->second.processId != childPid; ++i)
    ;
  //
  //	Clean up.
  //
  DebugInfo("remote interpreter " << i->first << " exited with code " << wstatus);
  {
    int fd = i->second.ioSocket;
    DebugInfo("closing i/o socket " << fd);
    close(fd);
    clearFlags(fd);
  }
  {
    int fd = i->second.errSocket;
    DebugInfo("closing error socket " << fd);
    close(fd);
    clearFlags(fd);
  }
  //
  //	Make error message.
  //
  const char* errorMessage = "Unknown cause.";  // catch all; shouldn't happen
  if (WIFEXITED(wstatus))
    {
      if (WEXITSTATUS(wstatus) == UserLevelRewritingContext::STACK_OVERFLOW)
	errorMessage = "Stack overflow.";
      else if (WEXITSTATUS(wstatus) == UserLevelRewritingContext::INTERNAL_ERROR)
	errorMessage = "Internal error.";
      else if (WEXITSTATUS(wstatus) == UserLevelRewritingContext::SOCKET_CLOSED)
	errorMessage = "Socket closed.";
    }
  else if(WIFSIGNALED(wstatus))
    errorMessage = "Killed by signal.";

  Vector<DagNode*> reply(3);
  reply.resize(1);
  reply[0] = metaLevel->upNat(i->first);
  DagNode* interpreterName = interpreterOidSymbol->makeDagNode(reply);
  reply.resize(3);
  DagNode* target = i->second.owner.getNode();
  reply[0] = target;
  reply[1] = interpreterName;
  reply[2] = new StringDagNode(stringSymbol, errorMessage);
  DagNode* response = interpreterExitMsg->makeDagNode(reply);
  i->second.savedContext->bufferMessage(target, response);
  i->second.savedContext->deleteExternalObject(interpreterName);
  //
  //	Delete interpreter.
  //
  delete i->second.charArray;
  remoteInterpreters.erase(i);
}
