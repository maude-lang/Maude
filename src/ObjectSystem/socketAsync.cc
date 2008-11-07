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
//	PseudoThread callbacks to handle nonblocking I/O.
//

void
SocketManagerSymbol::doRead(int fd)
{
  //cerr << "SocketManagerSymbol::doRead()" << endl;
  SocketMap::iterator i = activeSockets.find(fd);
  Assert(i != activeSockets.end(), "read callback for nonexistent socket " << fd);
  ActiveSocket& as = i->second;
  if (as.state & WAITING_TO_READ)
    {
      FreeDagNode* message = safeCast(FreeDagNode*, as.lastMessage.getNode());
      ObjectSystemRewritingContext& context = *(as.originalContext);

      char buffer[READ_BUFFER_SIZE];
      ssize_t n;
      do
	n = read(fd, buffer, READ_BUFFER_SIZE);
      while (n == -1 && errno == EINTR);
      //cerr << "n = " << n << endl;

      if (n > 0)
	{
	  as.state = NOMINAL;
	  receivedMsgReply(buffer, n, message, context);
	  as.lastMessage.setNode(0);  // allow GC of last message
	}
      else
	{
	  if (n < 0)
	    {
	      const char* errText = strerror(errno);
	      DebugAdvisory("unexpected read() error in callback: " << errText);
	      closedSocketReply(fd, errText, message, context);
	    }
	  else
	    {
	      DebugAdvisory("read 0 bytes in callback");
	      closedSocketReply(fd, "", message, context);
	    }
	}
    }
  else if (as.state & WAITING_TO_ACCEPT)
    {
      ActiveSocket& as = activeSockets[fd];
      sockaddr_in sockName;
      socklen_t addrLen = sizeof(sockName);
      int r;
      do
	r = accept(fd, reinterpret_cast<sockaddr*>(&sockName), &addrLen);
      while (r == -1 && errno == EINTR);

      FreeDagNode* message = safeCast(FreeDagNode*, as.lastMessage.getNode());
      ObjectSystemRewritingContext& context = *(as.originalContext);

      as.state = LISTENING;
      if (r >= 0)
	{
	  if (setNonblockingFlag(r, message, context))
	    acceptedClientReply(inet_ntoa(sockName.sin_addr), r, message, context);
	}
      else
	{
	  //
	  //	What should we do with a socket that we failed to accept on?
	  //
	  const char* errText = strerror(errno);
	  DebugAdvisory("unexpected accept() error: " << errText);
	  errorReply(errText, message, context);
	}
      as.lastMessage.setNode(0);  // allow GC of last message
    }
  else
    {
      Assert(false, "unwanted read callback for socket " << fd);
    }
}

void
SocketManagerSymbol::doWrite(int fd)
{
  //cerr << "SocketManagerSymbol::doWrite()" << endl;
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

      FreeDagNode* message = safeCast(FreeDagNode*, as.lastMessage.getNode());
      ObjectSystemRewritingContext& context = *(as.originalContext);
      if (errorCode == 0)
	{
	  as.state = NOMINAL;
	  createdSocketReply(fd, message, context);
	  as.lastMessage.setNode(0);  // allow GC of last message
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
      FreeDagNode* message = safeCast(FreeDagNode*, as.lastMessage.getNode());
      ObjectSystemRewritingContext& context = *(as.originalContext);

      ssize_t n;
      do
	n = write(fd, as.unsent, as.nrUnsent);
      while (n == -1 && errno == EINTR);
      //cerr << "n = " << n << endl;

      if (n > 0)
	{
	  as.nrUnsent -= n;
	  //cerr << "as.nrUnsent = " << as.nrUnsent << endl;
	  if (as.nrUnsent == 0)
	    {
	      as.state = NOMINAL;
	      // clear  as.text
	      sentMsgReply(message, context);
	      as.lastMessage.setNode(0);  // allow GC of last message
	    }
	  else
	    {
	      as.unsent += n;
	      wantTo(WRITE, fd);
	    }
	}
      else
	{
	  if (n < 0)
	    {
	      const char* errText = strerror(errno);
	      DebugAdvisory("unexpected write() error in callback: " << errText);
	      closedSocketReply(fd, errText, message, context);
	    }
	  else
	    {
	      DebugAdvisory("wrote 0 bytes in callback");
	      closedSocketReply(fd, "", message, context);
	    }
	}
    }
  else
    {
      Assert(false, "unwanted write callback for socket " << fd);
    }
}

void
SocketManagerSymbol::doError(int fd)
{
  //cerr << "SocketManagerSymbol::doError()" << endl;
  SocketMap::iterator i = activeSockets.find(fd);
  if (i == activeSockets.end())  // handle gracefully - may have already been closed
    {
      DebugAdvisory("error callback for nonexistent socket " << fd);
    }
  else
    {
      //cerr << "valid socket\n";
      ActiveSocket& as = i->second;
      //cerr << "state = " << as.state << endl;
      if (as.state & (WAITING_TO_CONNECT | WAITING_TO_WRITE | WAITING_TO_READ))
	{
	  int errorCode;
	  socklen_t errorSize = sizeof(errorCode);
#ifdef NO_ASSERT
	  getsockopt(fd, SOL_SOCKET, SO_ERROR, &errorCode, &errorSize);
#else
	  int r = getsockopt(fd, SOL_SOCKET, SO_ERROR, &errorCode, &errorSize);
	  Assert(r == 0 && errorSize == sizeof(errorCode), "getsockopt() problem");
#endif
	  //cerr << "injecting closed message\n";
	  FreeDagNode* message = safeCast(FreeDagNode*, as.lastMessage.getNode());
	  ObjectSystemRewritingContext& context = *(as.originalContext);
	  closedSocketReply(fd, strerror(errorCode), message, context);
	}
      else
	{
	  Assert(false, "unwanted error callback for socket " << fd);
	}
    }
}

void
SocketManagerSymbol::doHungUp(int fd)
{
  //cerr << "SocketManagerSymbol::doHungUp()" << endl;
  SocketMap::iterator i = activeSockets.find(fd);
  if (i == activeSockets.end())  // handle gracefully - may have already been closed
    {
      DebugAdvisory("hung up callback for nonexistent socket " << fd);
    }
  else
    {
      //cerr << "valid socket\n";
      ActiveSocket& as = i->second;
      //cerr << "state = " << as.state << endl;
      if (as.state & (WAITING_TO_CONNECT | WAITING_TO_WRITE | WAITING_TO_READ))
	{
	  //cerr << "injecting closed message\n";
	  FreeDagNode* message = safeCast(FreeDagNode*, as.lastMessage.getNode());
	  ObjectSystemRewritingContext& context = *(as.originalContext);
	  closedSocketReply(fd, "hung up callback", message, context);
	}
      else
	{
	  Assert(false, "unwanted hung up callback for socket " << fd);
	}
    }
}
