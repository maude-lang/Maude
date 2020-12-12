/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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

void
InterpreterManagerSymbol::nonblockingSendMessage(RemoteInterpreter& ri, const Rope& message)
{
  Assert(ri.nrUnsent == 0, "can't send another message while previous one is being sent");
  //
  //	Send as much as we can, and send the rest using call-backs.
  //
  int socketId = ri.ioSocket;
  Rope::size_type length = message.length();
  char* charArray = message.makeZeroTerminatedString();
  charArray[length] = EOT;  // change NUL to EOT

  ssize_t nrUnsent = length + 1;
  char* p = charArray;
  //
  //	Restart interrupted calls.
  //
  ssize_t n;
  do
    n = write(socketId, p, nrUnsent);
  while (n == -1 && errno == EINTR);
  //
  //	We treat a blocking situation as zero characters sent.
  //
  if (n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
    n = 0;
  if (n >= 0)
    {
      nrUnsent -= n;
      if (nrUnsent == 0)
	{
	  //
	  //	All characters sent.
	  //
	  delete [] charArray;
	  return;
	}
      else
	{
	  //
	  //	Need to send more characters when socket becomes
	  //	available for writing.
	  //
	  ri.nrUnsent = nrUnsent;
	  ri.charArray = charArray;
	  ri.unsent = charArray + n;
	  wantTo(WRITE, socketId);
	  return;
	}
    }
  else
    {
      //
      //	We had an error that wasn't due to an interrupt or
      //	not wanting to block. We must assume connection is broken.
      //	Not sure for the moment how to clean up.
      //
      CantHappen("write() failed");
    }
}

void
InterpreterManagerSymbol::doWrite(int fd)
{
  //
  //	A little inefficient but we only expect a handfull of intepreters
  //	in existence at one time.
  //
  RemoteInterpreter* ri = 0;
  for (auto& p : remoteInterpreters)
    {
      if (p.second.ioSocket == fd)
	ri = &p.second;
    }
  Assert(ri != 0, "couldn't find remote interpreter for doWrite() callback");
  //
  //	Restart interrupted calls.
  //	Because we're in a callback we expect to be able to write at
  //	at least one character without blocking.
  //
  ssize_t n;
  do
    n = write(fd, ri->unsent, ri->nrUnsent);
  while (n == -1 && errno == EINTR);
  Assert(n != 0, "unexpected zero characters written");
  Assert(!(n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)),
	 "unexpected would block in doWrite() callback");
  
  if (n >= 0)  // include 0 case here for safety, though it should not arise
    {
      ri->nrUnsent -= n;
      if (ri->nrUnsent == 0)
	{
	  //
	  //	Finished sending; tidy up.
	  //
	  delete [] ri->charArray;
	  ri->charArray = 0;
	  ri->unsent = 0;
	}
      else
	{
	  //
	  //	Need to move unsent pointers and request another callback.
	  //
	  ri->unsent += n;
	  wantTo(WRITE, fd);
	}
    }
  else
    {
      //
      //	We had an error that wasn't due to an interrupt or
      //	not wanting to block. We must assume connection is broken.
      //	Not sure for the moment how to clean up.
      //
      CantHappen("write() failed in callback");
    }
}

void
InterpreterManagerSymbol::doRead(int fd)
{
  //IssueAdvisory("doRead() for fd = " << fd);
  //
  //	A little inefficient but we only expect a handfull of intepreters
  //	in existence at one time.
  //
  RemoteInterpreter* ri = 0;
  for (auto& p : remoteInterpreters)
    {
      if (p.second.ioSocket == fd)
	{
	  ri = &p.second;
	  break;
	}
      else if (p.second.errSocket == fd)
	{
	  //
	  //	If we handled an event and didn't see an EOF condition
	  //	we need to re-request callback on error socket
	  //
	  if (!outputWholeErrorLines(&p.second))
	    wantTo(READ, fd);
	  return;
	}
    }
  Assert(ri != 0, "couldn't find remote interpreter for doRead() callback");
  Assert(ri->nrPendingReplies > 0, "wasn't expect a doRead() callback");
  //
  //	Drain any error messages and cancel callback if we saw EOF.
  //
  if (outputWholeErrorLines(ri))
    clearFlags(ri->errSocket);
  //
  //	We keep reading until we get a "would block" situation. This
  //	is because the writer maybe dumping megabytes of serialized
  //	DAG though the socket and we don't want to wait for multiple
  //	callbacks (one per fair traversal) to get it all.
  //
  bool sawEOF = false;
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
	  //	We got characters; look for an EOT that signals the end
	  //	of a DAG, then we can dispatch the message it encodes.
	  //
	  ssize_t messageStart = 0;
	  for (ssize_t i = 0; i < n; ++i)
	    {
	      if (buffer[i] == EOT)
		{
		  ri->incomingMessage += Rope(buffer + messageStart, i - messageStart);
		  remoteHandleReply(ri, ri->incomingMessage);
		  ri->incomingMessage.clear();
		  messageStart = i + 1;
		}
	    }
	  ssize_t nrCharsLeft = n - messageStart;
	  if (nrCharsLeft > 0)
	    {
	      //
	      //	Either we didn't see an EOT or there are characters beyond
	      //	the last EOT; the remaining characters the start of another DAG.
	      //	
	      ri->incomingMessage += Rope(buffer + messageStart, nrCharsLeft);
	      Assert(ri->nrPendingReplies > 0, "partial message yet no pending replies");
	    }
	}
      else
	{
	  if (n == 0)
	    {
	      //
	      //	Other end of socket must have closed.
	      //
	      sawEOF = true;
	      //IssueAdvisory("callback for fd = " << fd << " saw EOF");
	    }
	  else
	    {
	      //
	      //	We had an error that wasn't due to an interrupt or
	      //	not wanting to block. This shouldn't happen on the
	      //	first read but we expect it to happen eventually.
	      //
	      //IssueAdvisory("callback for fd = " << fd << " error =  " << strerror(errno));
	    }
	  break;
	}
    }
  if (!sawEOF && ri->nrPendingReplies > 0)
    {
      //
      //	Expecting more replies so keep reading socket.
      //
      wantTo(READ, fd);
    }
  else
    {
      //
      //	Don't hang waiting for nonexistent error messages.
      //
      clearFlags(ri->errSocket);
    }
}

bool
InterpreterManagerSymbol::outputWholeErrorLines(RemoteInterpreter* ri)
{
  //
  //	Read any characters available on the error socket and
  //	send any whole lines cerr. Return true if saw EOF condition
  //	and false otherwise.
  //
  int fd = ri->errSocket;
  for (;;)
    {
      char buffer[READ_BUFFER_SIZE];
      ssize_t n;
      do
	n = read(fd, buffer, READ_BUFFER_SIZE);
      while (n == -1 && errno == EINTR);
      //cerr << "---- read " << n << endl;
      if (n > 0)
	{
	  ssize_t messageStart = 0;
	  for (ssize_t i = 0; i < n; ++i)
	    {
	      if (buffer[i] == '\n')
		{
		  ri->incomingMessage += Rope(buffer + messageStart, i - messageStart);
		  //cerr << "----> ";
		  cerr << ri->incomingMessage << endl;
		  ri->incomingMessage.clear();
		  messageStart = i + 1;
		}
	    }
	  ssize_t nrCharsLeft = n - messageStart;
	  if (nrCharsLeft > 0)
	    {
	      ri->incomingMessage += Rope(buffer + messageStart, nrCharsLeft);
	      Assert(ri->nrPendingReplies > 0, "partial message yet no pending replies");
	    }
	}
      else
	{
	  if (n == 0)
	    {
	      //
	      //	Other end of socket must have closed.
	      //
	      //IssueAdvisory("fd = " << fd << "  saw EOF");
	      return true;
	    }
	  else
	    {
	      //
	      //	We don't know for sure that there are characters waiting
	      //	for us since we just clearing out any pending messages.
	      //
	      //IssueAdvisory("fd = " << fd << " error = " << strerror(errno));
	    }
	  break;
	}
    }
  return false;
}
