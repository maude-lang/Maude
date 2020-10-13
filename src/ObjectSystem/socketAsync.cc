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
//	PseudoThread callbacks to handle nonblocking I/O.
//

void
SocketManagerSymbol::doRead(int fd)
{
  DebugInfo(" on " << fd);
  SocketMap::iterator i = activeSockets.find(fd);
  Assert(i != activeSockets.end(), "read callback for nonexistent socket " << fd);
  ActiveSocket& as = i->second;
  if (as.state & WAITING_TO_READ)
    {
      FreeDagNode* message = safeCast(FreeDagNode*, as.lastReadMessage.getNode());
      ObjectSystemRewritingContext& context = *(as.objectContext);

      char buffer[READ_BUFFER_SIZE];
      ssize_t n;
      do
	n = read(fd, buffer, READ_BUFFER_SIZE);
      while (n == -1 && errno == EINTR);
      DebugInfo("n = " << n);

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
		  closedSocketReply(fd, "", message, context);
		  return;
		}
	      as.seenEOF = true;
	    }
	  as.state &= ~WAITING_TO_READ;
	  receivedMsgReply(buffer, n, message, context);
	  as.lastReadMessage.setNode(0);  // allow GC of last read message
	}
      else
	{
	  //
	  //	An error on the socket. We expect that this is due to the
	  //	connection being broken, so we close the socket. No need
	  //	to clear the WAITING_TO_READ or the lastReadMessage
	  //	because closedSocketReply() will delete the whole object.
	  //
	  const char* errText = strerror(errno);
	  DebugInfo("unexpected read() error in callback: " << errText);
	  closedSocketReply(fd, errText, message, context);
	}
    }
  else if (as.state & WAITING_TO_ACCEPT)
    {
      sockaddr_in sockName;
      socklen_t addrLen = sizeof(sockName);
      int r;
      do
	r = accept(fd, reinterpret_cast<sockaddr*>(&sockName), &addrLen);
      while (r == -1 && errno == EINTR);

      FreeDagNode* message = safeCast(FreeDagNode*, as.lastReadMessage.getNode());
      ObjectSystemRewritingContext& context = *(as.objectContext);

      as.state = LISTENING;
      if (r >= 0)
	{
	  if (setNonblockingFlag(r, message, context))
	    {
	      acceptedClientReply(inet_ntoa(sockName.sin_addr), r, message, context);
	      activeSockets[r].state = NOMINAL;  // this creates the new ActiveSocket object
	    }
	}
      else
	{
	  //
	  //	We just clear our bit, return an error reply and
	  //	leave the socket intact so user can try a further acceptClient()
	  //
	  as.state &= ~WAITING_TO_ACCEPT;
	  const char* errorText = strerror(errno);
	  DebugAdvisory("unexpected accept() error: " << errorText);
	  errorReply(errorText, message, context);
	}
      as.lastReadMessage.setNode(0);  // allow GC of last message
    }
  else
    CantHappen("unwanted read callback for socket " << fd);
}

void
SocketManagerSymbol::doWrite(int fd)
{
  DebugInfo(" on " << fd);
  SocketMap::iterator i = activeSockets.find(fd);
  Assert(i != activeSockets.end(), "write callback for nonexistent socket " << fd);
  ActiveSocket& as = i->second;
  if (as.state & WAITING_TO_CONNECT)
    {
      int errorCode;
      socklen_t errorSize = sizeof(errorCode);
#ifdef NO_ASSERT
      (void) getsockopt(fd, SOL_SOCKET, SO_ERROR, &errorCode, &errorSize);
#else
      int r = getsockopt(fd, SOL_SOCKET, SO_ERROR, &errorCode, &errorSize);
      Assert(r == 0 && errorSize == sizeof(errorCode), "getsockopt() problem");
#endif

      FreeDagNode* message = safeCast(FreeDagNode*, as.lastWriteMessage.getNode());
      ObjectSystemRewritingContext& context = *(as.objectContext);
      if (errorCode == 0)
	{
	  createdSocketReply(fd, message, context);
	  as.state = NOMINAL;
	  as.lastWriteMessage.setNode(0);  // allow GC of last message
	}
      else
	{
	  close(fd);
	  errorReply("failed to connect", message, context);
	  activeSockets.erase(i);
	}
    }
  else if (as.state & WAITING_TO_WRITE)
    {
      FreeDagNode* message = safeCast(FreeDagNode*, as.lastWriteMessage.getNode());
      ObjectSystemRewritingContext& context = *(as.objectContext);

      ssize_t n;
      do
	n = write(fd, as.unsent, as.nrUnsent);
      while (n == -1 && errno == EINTR);
      Assert(n != 0, "unexpected zero characters written");
      Assert(!(n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)),
	     "unexpected would block in doWrite() callback");
      
      if (n >= 0)  // include 0 case here for safety, though it should not arise
	{
	  as.nrUnsent -= n;
	  if (as.nrUnsent == 0)
	    {
	      as.state &= ~WAITING_TO_WRITE;
	      delete [] as.textArray;
	      as.textArray = 0;
	      sentMsgReply(message, context);
	    }
	  else
	    {
	      as.unsent += n;  // move the pointer to the unsent characters
	      wantTo(WRITE, fd);
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
	  as.state &= ~WAITING_TO_WRITE;
 	  as.lastWriteMessage.setNode(0);  // allow GC of last message
	  //
	  //	Delete character array here so we don't lose
	  //	the pointer in a future send().
	  //
	  delete [] as.textArray;
	  as.textArray = 0;
	}
    }
  else
    CantHappen("unwanted write callback for socket " << fd);
}

void
SocketManagerSymbol::doError(int fd)
{
  DebugInfo(" on " << fd);
  //
  //	Get error message.
  //
  int errorCode;
  socklen_t errorSize = sizeof(errorCode);
#ifdef NO_ASSERT
  getsockopt(fd, SOL_SOCKET, SO_ERROR, &errorCode, &errorSize);
#else
  int r = getsockopt(fd, SOL_SOCKET, SO_ERROR, &errorCode, &errorSize);
  Assert(r == 0 && errorSize == sizeof(errorCode), "getsockopt() problem");
#endif
  const char* errorMessage = strerror(errorCode);
  //
  //	We use the same code for errors and hangups.
  //
  handleException(fd, errorMessage);
}

void
SocketManagerSymbol::doHungUp(int fd)
{
  DebugInfo(" on " << fd);
  handleException(fd, "Hung up");
}

void
SocketManagerSymbol::handleException(int fd, const char* errorText)
{
  SocketMap::iterator i = activeSockets.find(fd);
  if (i == activeSockets.end()) 
    {
      //
      //	Handle gracefully - socket may already have been closed.
      //
      DebugAdvisory("error callback for nonexistent socket " << fd);
      return;
    }
  ActiveSocket& as = i->second;
  ObjectSystemRewritingContext& context = *(as.objectContext);
  if (as.state & (WAITING_TO_ACCEPT))
    {
      FreeDagNode* message = safeCast(FreeDagNode*, as.lastReadMessage.getNode());
      errorReply(errorText, message, context);
      //
      //	We cleanup rather than delete socket object so its still
      //	nominally usable.
      //
      as.state &= ~WAITING_TO_ACCEPT;
      as.lastReadMessage.setNode(0);  // allow GC of last read message
    }
  else if (as.state & (WAITING_TO_CONNECT))
    {
      //
      //	A failed connect means we've never actually returned
      //	a socket object to the user.
      //
      FreeDagNode* message = safeCast(FreeDagNode*, as.lastWriteMessage.getNode());
      close(fd);
      errorReply(errorText, message, context);
      activeSockets.erase(i);
    }
  else
    {
      //
      //	Could have both a read and a write pending. We need to
      //	handle write first because read will destroy the socket object.
      //
      if (as.state & (WAITING_TO_WRITE))
	{
	  FreeDagNode* message = safeCast(FreeDagNode*, as.lastWriteMessage.getNode());
	  errorReply(errorText, message, context);
	  //
	  //	We cleanup rather than delete the socket object so its still
	  //	nominally usable.
	  //
	  as.state &= ~WAITING_TO_WRITE;
 	  as.lastWriteMessage.setNode(0);  // allow GC of last message
	  delete [] as.textArray;
	  as.textArray = 0;
	}
      if (as.state & (WAITING_TO_READ))
	{
	  FreeDagNode* message = safeCast(FreeDagNode*, as.lastReadMessage.getNode());
	  closedSocketReply(fd, errorText, message, context);
	}
    }
}
