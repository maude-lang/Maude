/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Main socket manipulation code.
//
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

bool
SocketManagerSymbol::getPort(DagNode* portArg, int& port)
{
  return succSymbol->getSignedInt(portArg, port) && port <= MAX_PORT_NUMBER;
}

bool
SocketManagerSymbol::getActiveSocket(DagNode* socketArg, int& socketId, ActiveSocket*& asp)
{
  if (socketArg->symbol() == socketOidSymbol)
    {
      DagNode* idArg = safeCast(FreeDagNode*, socketArg)->getArgument(0);
      if (succSymbol->getSignedInt(idArg, socketId))
	{
	  SocketMap::iterator i = activeSockets.find(socketId);
	  if (i != activeSockets.end())
	    {
	      asp = &(i->second);
	      return true;
	    }
	}
    }
  return false;
}

bool
SocketManagerSymbol::getText(DagNode* textArg, Rope& text)
{
  if (textArg->symbol() == stringSymbol)
    {
      text = safeCast(StringDagNode*, textArg)->getValue();
      return true;
    }
  return false;
}

bool
SocketManagerSymbol::setNonblockingFlag(int fd,
					FreeDagNode* message,
					ObjectSystemRewritingContext& context)
{
  //
  //	Set nonblocking flag for a nascent socket; since it is not yet an external
  //	object we can just close it and generate an error reply if things don't work out.
  //
  //	We also set the close-on-execute file descriptor flag so this file
  //	descriptor is not leaked after a fork() and exec family call.
  //
  int flags = fcntl(fd, F_GETFL);
  if (flags != 1 && fcntl(fd, F_SETFL, flags | O_NONBLOCK) != -1)
    {
      int flags2 = fcntl(fd, F_GETFD);
      if (flags2 != -1 && fcntl(fd, F_SETFD, flags2 | FD_CLOEXEC) != -1)
	return true;
    }

  const char* errText = strerror(errno);
  DebugAdvisory("unexpected fcntl() error " << errText);
  close(fd);
  errorReply(errText, message, context);
  return false;
}

bool
SocketManagerSymbol::createClientTcpSocket(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Assert(message->getArgument(0)->symbol() == this, "misdirected message");
  int port;
  DagNode* addressArg = message->getArgument(2);

  if (getPort(message->getArgument(3), port) && addressArg->symbol() == stringSymbol)
    {
      //
      //	We accept the message.
      //

      //
      //	Look up the address.
      //
      const Rope& address = safeCast(StringDagNode*, addressArg)->getValue();
      char* addressStr = address.makeZeroTerminatedString();
      hostent* record = gethostbyname(addressStr);  // HACK - might block
      delete [] addressStr;
      if (record == 0)
	{
	  DebugAdvisory("unexpected gethostbyname(() error: " << strerror(errno));
	  errorReply("bad address", message, context);
	  return true;
	}
      //
      //	Create a socket.
      //
      int fd = socket(PF_INET, SOCK_STREAM, 0);
      if (fd == -1)
	{
	  const char* errText = strerror(errno);
	  DebugAdvisory("unexpected socket() error: " << errText);
	  errorReply(errText, message, context);
	  return true;
	}
      //
      //	Make it nonblocking. This is important because
      //	a write may be too big for the output buffer and
      //	a read could fail to get characters in certain edge
      //	cases such as a bad packet.
      //
      if (!setNonblockingFlag(fd, message, context))
	return true;
      //
      //	Try to connect to host.
      //
      sockaddr_in sockName;
      sockName.sin_family = AF_INET;
      sockName.sin_port = htons(port);
      sockName.sin_addr = *(reinterpret_cast<in_addr*>(record->h_addr_list[0]));  // HACK
      if (connect(fd, reinterpret_cast<sockaddr*>(&sockName), sizeof(sockName)) == 0)
	{
	  createdSocketReply(fd, message, context);  // instant success
	  activeSockets[fd].state = NOMINAL;  // this creates the ActiveSocket object
	}
      else if (errno == EINPROGRESS)
	{
	  //
	  //	Incomplete transaction on an asynchronous socket, so save details
	  //	so we know what to do when transaction completes.
	  //
	  ActiveSocket& as = activeSockets[fd];  // this creates the ActiveSocket object
	  as.state = WAITING_TO_CONNECT;
	  as.lastWriteMessage.setNode(message);
	  as.objectContext = &context;
	  //
	  //	Completion (could be success or failure) is indicated by the operation system
	  //	making the socket writable.
	  //
	  wantTo(WRITE, fd);
	}
      else
	{
	  //
	  //	Connect failed, so report error and pretend that socket never existed.
	  //
	  DebugAdvisory("unexpected connect() error: " << strerror(errno));
	  close(fd);
	  errorReply("failed to connect", message, context);
	}
      return true;
    }
  IssueAdvisory("socket manager declined malformed message " << QUOTE(message) << '.');
  return false;
}

bool
SocketManagerSymbol::createServerTcpSocket(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Assert(message->getArgument(0)->symbol() == this, "misdirected message");
  int port;
  int backlog;

  if (getPort(message->getArgument(2), port) &&
      succSymbol->getSignedInt(message->getArgument(3), backlog) &&
      backlog > 0)
    {
      //
      //	Create a socket.
      //
      int fd = socket(PF_INET, SOCK_STREAM, 0);
      if (fd == -1)
	{
	  const char* errText = strerror(errno);
	  DebugAdvisory("unexpected socket() error: " << errText);
	  errorReply(errText, message, context);
	  return true;
	}
      //
      //	Make it non-blocking.
      //
      if (!setNonblockingFlag(fd, message, context))
	return true;
      {
	//
	//	Set SO_REUSEADDR so port can be immediately reused following the close()
	//	of this socket.
	//
	int value = 1;
	if (setsockopt(fd, SOL_SOCKET,  SO_REUSEADDR,  &value, sizeof(value)) == -1)
	  {
	    const char* errText = strerror(errno);
	    DebugAdvisory("setsockopt(SO_REUSEADDR) failed: " << errText);
	    errorReply(errText, message, context);
	    close(fd);
	    return true;
	  }
      }
      {
	//
	//	Bind it to the local port.
	//
	sockaddr_in sockName;
	sockName.sin_family = AF_INET;
	sockName.sin_port = htons(port);
	sockName.sin_addr.s_addr = htonl(INADDR_ANY);  // HACK - what is the portable way to set this?
	//
	//	Make sure we use the OS call and not std::bind() from C++1x
	//
	if (::bind(fd, reinterpret_cast<sockaddr*>(&sockName), sizeof(sockName)) == -1)
	  {
	    const char* errText = strerror(errno);
	    DebugAdvisory("unexpected bind() error with fd " << fd << ": " << errText);
	    errorReply(errText, message, context);
	    close(fd);
	    return true;
	  }
      }
      //
      //	Start listening for connections.
      //
      if (listen(fd, backlog) == -1)
	{
	  const char* errText = strerror(errno);
	  DebugAdvisory("unexpected listen() error: " << errText);
	  errorReply(errText, message, context);
	  close(fd);
	  return true;
	}
      //
      //	Return a message now that we have a bound and listening server socket.
      //
      createdSocketReply(fd, message, context);
      activeSockets[fd].state = LISTENING;  // this creates the ActiveSocket object
     return true;
    }
  IssueAdvisory("socket manager declined malformed message " << QUOTE(message) << '.');
  return false;
}

bool
SocketManagerSymbol::acceptClient(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int socketId;
  ActiveSocket* asp;
  DagNode* socketName = message->getArgument(0);
  if (getActiveSocket(socketName, socketId, asp))
    {
      ActiveSocket& as = *asp;
      if (as.state == LISTENING)
	{
	  sockaddr_in sockName;
	  socklen_t addrLen = sizeof(sockName);
	  int r;
	  do
	    r = accept(socketId, reinterpret_cast<sockaddr*>(&sockName), &addrLen);
	  while (r == -1 && errno == EINTR);
	  if (r >= 0)
	    {
	      if (setNonblockingFlag(r, message, context))
		{
		  acceptedClientReply(inet_ntoa(sockName.sin_addr), r, message, context);
		  activeSockets[r].state = NOMINAL;  // this creates the new ActiveSocket object
		}
	    }
	  else if (errno == EAGAIN)
	    {
	      as.state = WAITING_TO_ACCEPT;
	      as.lastReadMessage.setNode(message);
	      as.objectContext = &context;
	      wantTo(READ, socketId);
	    }
	  else
	    {
	      //
	      //	We just return an error reply and leave the socket
	      //	intact so that the user can try a further acceptClient()
	      //
	      const char* errText = strerror(errno);
	      DebugAdvisory("unexpected accept() error: " << errText);
	      errorReply(errText, message, context);
	    }
	  return true;
	}
      IssueAdvisory(socketName << " declined message " << QUOTE(message) << '.');
      return false;
    }
  IssueAdvisory("no socket to receive message " << QUOTE(message) << '.');
  return false;
}

bool
SocketManagerSymbol::send(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int socketId;
  ActiveSocket* asp;
  Rope text;
  DagNode* socketName = message->getArgument(0);
  if (getActiveSocket(socketName, socketId, asp) &&
      !(asp->readOnly) &&
      getText(message->getArgument(2), text))
    {
      ActiveSocket& as = *asp;
      //
      //	Check that all the state bits other than WAITING_TO_READ are clear.
      //
      if ((as.state & ~WAITING_TO_READ) == 0)
	{
	  if (text.empty())
	    {
	      //
	      //	We treat sending the empty string as a request
	      //	to close the write side of the socket and cause
	      //	an EOF condition for the reader.
	      //
	      if (shutdown(socketId, SHUT_WR) == 0)
		{
		  DebugInfo("shutdown() succeeded for " << socketId);
		  asp->readOnly = true;
		  sentMsgReply(message, context);
		}
	      else
		{
		  const char* errorText = strerror(errno);
		  DebugInfo("shutdown() error for " << socketId <<
			      " : " << errorText);
		  errorReply(errorText, message, context);
		}
	      return true;
	    }
	  as.textArray = text.makeZeroTerminatedString();
	  as.unsent = as.textArray;
	  as.nrUnsent = text.length();
	  Assert(as.nrUnsent > 0, "no characters to send");
	  //
	  //	Write some characters to the socket; we might get interrupted
	  //	and have to restart.
	  //
	  ssize_t n;
	  do
	    n = write(socketId, as.unsent, as.nrUnsent);
	  while (n == -1 && errno == EINTR);
	  //
	  //	We treat a blocking situation as zero characters sent.
	  //
	  if (n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
	    n = 0;
	  if (n >= 0)
	    {
	      as.nrUnsent -= n;
	      if (as.nrUnsent == 0)
		{
		  //
		  //	All characters sent.
		  //
		  sentMsgReply(message, context);
		  //
		  //	We have responsibility for deleting the char array
		  //	we got from Rope::makeZeroTerminatedString()
		  //
		  delete [] as.textArray;
		  as.textArray = 0;
		}
	      else
		{
		  //
		  //	At least some characters not yet sent.
		  //
		  as.state |= WAITING_TO_WRITE;
		  as.lastWriteMessage.setNode(message);
		  as.objectContext = &context;
		  as.unsent += n;
		  wantTo(WRITE, socketId);
		}
	    }
	  else
	    {
	      //
	      //	We had an error that was not due to an interrupt
	      //	or no characters available. We assume that this
	      //	means a broken connection. However we don't want
	      //	to close the socket as it may still be possible
	      //	to read characters. Instead we just inform the
	      //	requesting object of the error.
	      //
	      const char* errorText = strerror(errno);
	      DebugInfo("unexpected write() error : " << errorText);
	      errorReply(errorText, message, context);
	      //
	      //	Delete character array here so we don't lose
	      //	the pointer in a future send().
	      //
	      delete [] as.textArray;
	      as.textArray = 0;
	    }
	  return true;
	}
      IssueAdvisory(socketName << " declined message " << QUOTE(message) << '.');
      return false;
    }
  IssueAdvisory("no socket to receive message " << QUOTE(message) << '.');
  return false;
}

bool
SocketManagerSymbol::receive(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int socketId;
  ActiveSocket* asp;
  DagNode* socketName = message->getArgument(0);
  if (getActiveSocket(socketName, socketId, asp))
    {
      ActiveSocket& as = *asp;
      //
      //	Check that all the state bits other than WAITING_TO_WRITE are clear.
      //
      if ((as.state & ~WAITING_TO_WRITE) == 0)
	{
	  char buffer[READ_BUFFER_SIZE];
	  ssize_t n;
	  do
	    n = read(socketId, buffer, READ_BUFFER_SIZE);
	  while (n == -1 && errno == EINTR);

	  if (n >= 0)
	    {
	      //
	      //	We also deal with the n == 0 case here. This happens when
	      //	the far end of the socket is closed for writing; this
	      //	could be because the socket has be closed or because the
	      //	other end has shutdown() the write half of the connection.
	      //	Because the latter is sometimes used to indicate an EOF
	      //	condition while still allowing data to be returned, we
	      //	return 0 bytes to the user rather than closing the socket.
	      //
	      if (n == 0)
		{
		  if (as.seenEOF)
		    {
		      //
		      //	If we've already returned 0 characters once
		      //	we close the socket this time to minimize
		      //	incompatibility with legacy behavior.
		      //
		      closedSocketReply(socketId, "", message, context);
		      return true;
		    }
		  as.seenEOF = true;
		}
	      receivedMsgReply(buffer, n, message, context);
	    }
	  else
	    {
	      if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
		  //
		  //	No characters available to read so wait for them.
		  //
		  as.state |= WAITING_TO_READ;
		  as.lastReadMessage.setNode(message);
		  as.objectContext = &context;
		  wantTo(READ, socketId);
		}
	      else
		{
		  //
		  //	An error on the socket. We expect that this
		  //	is due to the connection being broken, so we
		  //	close the socket.
		  //
		  const char* errText = strerror(errno);
		  DebugInfo("unexpected read() error: " << errText);
		  closedSocketReply(socketId, errText, message, context);
		}
	    }
	  return true;
	}
      else
	DebugAdvisory("as.state = " << as.state);
      IssueAdvisory(socketName << " declined message " << QUOTE(message) << '.');
      return false;
    }
  IssueAdvisory("no socket to receive message " << QUOTE(message) << '.');
  return false;
}

bool
SocketManagerSymbol::closeSocket(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int socketId;
  ActiveSocket* asp;
  DagNode* socketName = message->getArgument(0);
  if (getActiveSocket(socketName, socketId, asp) && !(asp->disallowClose))
    {
      closedSocketReply(socketId, "", message, context);
      return true;
    }
  IssueAdvisory("no socket to receive message " << QUOTE(message) << '.');
  return false;
}

void
SocketManagerSymbol::cleanUp(DagNode* objectId)
{
  //
  //	This function belongs to the ExternalObjectManagerSymbol
  //	interface and is called by the object system code when a Maude
  //	object goes away so that the corresponded lower level entitie(s)
  //	can be made to go away.
  //
  int socketId;
  ActiveSocket* asp;
  if (getActiveSocket(objectId, socketId, asp))
    {
      DebugInfo("cleaning up " << objectId);
      //
      //	In case we were had a pending write.
      //
      delete [] asp->textArray;
      asp->textArray = 0;
      close(socketId);
      activeSockets.erase(socketId);
      //
      //	If the fd was registered as active with PseudoThread it needs
      //	go away to avoid trying to poll() a nonexistent fd.
      //
      PseudoThread::clearFlags(socketId);
    }
  else
    CantHappen("no socket for " << objectId);
}
