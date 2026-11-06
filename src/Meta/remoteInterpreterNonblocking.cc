/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020-2026 SRI International, Menlo Park, CA 94025, USA.

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
      DebugInfo("write() failed");
      //
      //	We had an error that wasn't due to an interrupt or
      //	wanting to block. We expect that this happened because
      //	the interpreter has exited, or is part way though
      //	exiting, and its end of the socket is closed.
      //
      Assert(errno == EPIPE, "unexpected error in write() " << strerror(errno));
      //
      //	Because we may have miss the SIGCHLD, we check
      //	to see if the interpreter has fully exited.
      //
      int pid = ri.processId;
      int wstatus;
      if (waitpid(pid, &wstatus, WNOHANG) == pid)
	{
	  //
	  //	The interpreter exited. Because our caller should have
	  //	requested a notification, there is a race between the
	  //	exit and the request for notification - the SIGCHLD
	  //	could have been missed.
	  //	We first cancel the request, and then call the callback
	  //	to handle the exit ourself.
	  //
	  cancelChildExitCallback(pid);
	  doChildExit(pid, wstatus);
	}
      else
	{
	  //
	  //	The socket is closed but the interpreter has not finished
	  //	exiting. Our request for notification won the race and
	  //	we can rely on PseudoThread::eventLoop() make the callback.
	  //
	}
    }
}

void
InterpreterManagerSymbol::doWrite(int fd)
{
  //
  //	A little inefficient but we only expect a handfull of intepreters
  //	in existence at one time.
  //
  RemoteInterpreter* ri = nullptr;
  for (auto& p : remoteInterpreters)
    {
      if (p.second.ioSocket == fd)
	ri = &p.second;
    }
  Assert(ri != nullptr, "couldn't find remote interpreter for doWrite() callback");
  //
  //	Restart interrupted calls.
  //	Because we're in a callback we expect to be able to write at
  //	at least one character without blocking, but it's not guaranteed.
  //
  ssize_t n;
  do
    n = write(fd, ri->unsent, ri->nrUnsent);
  while (n == -1 && errno == EINTR);
  Assert(n != 0, "unexpected zero characters written");
  
  if (n >= 0)  // include 0 case here for safety, though it should not arise
    {
      ri->nrUnsent -= n;
      if (ri->nrUnsent == 0)
	{
	  //
	  //	Finished sending; tidy up.
	  //
	  delete [] ri->charArray;
	  ri->charArray = nullptr;
	  ri->unsent = nullptr;
	}
      else
	{
	  //
	  //	Need to move unsent pointer and request another callback.
	  //
	  ri->unsent += n;
	  wantTo(WRITE, fd);
	}
    }
  else
    {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
	{
	  //
	  //	Not ready to write(), even though OS hinted otherwise.
	  //	Leave things unchanged and ask for another callback.
	  //
	  wantTo(WRITE, fd);
	}
      DebugInfo("write() failed");
      Assert(errno == EPIPE, "unexpected error in write() " << strerror(errno));
      //
      //	We had an error that wasn't due to an interrupt or
      //	wanting to block. We expect that this happened because
      //	the interpreter has exited, or is in the process
      //	of exiting, and its end of the socket is closed.
      //
      //	Becasue we're in a callback, we can assume that our
      //	request for a child exit callback is active. If the child
      //	exited before our callback was active then POSIX guarantees
      //	that the write() in nonblockingSendMessage() would have
      //	failed with EPIPE and we would not have requested this callback.
      //
      //	Thus we can do nothing and allows the child exit callback
      //	to handle the cleanup.
      //
    }
}

void
InterpreterManagerSymbol::doRead(int fd)
{
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
  //	Before we process any output dag, we drain any error messages and
  //	cancel callback if we saw EOF.
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
	      DebugAdvisory("callback for fd = " << fd << " saw EOF");
	    }
	  else
	    {
	      //
	      //	We had an error that wasn't due to an interrupt.
	      //	Our socket is nonblocking so we expect to see
	      //	EAGAIN or EWOULDBLOCK eventually, maybe even on the
	      //	first read because doRead() is only a hint that
	      //	characters are waiting.
	      //
	      DebugAdvisory("callback for fd = " << fd << " error =  " << strerror(errno));
	    }
	  break;
	}
    }
  //
  //	We broke out of the loop because we encountered EOF or an error.
  //
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
      //	Either we saw an EOF; or we got an error and we aren't looking
      //	for more replies (so our child exit request is canceled).
      //
      //	It could be that the interpreter exited. If we aren't looking
      //	for more replies, we will discover this should we interact with it
      //	in the future. If we are looking for more replies, our child
      //	exit callback request should be active and PseudoThread::eventLoop()
      //	will deal with it.
      //
      //	Clear our request to read on the error socket so that eventLoop()
      //	won't block on it, once everything else is done.
      //
      clearFlags(ri->errSocket);
    }
}

bool
InterpreterManagerSymbol::outputWholeErrorLines(RemoteInterpreter* ri)
{
  //
  //	Read any characters available on the error socket and
  //	send any whole lines cerr. Return true if we saw EOF condition
  //	and false otherwise.
  //
  int fd = ri->errSocket;
  for (;;)
    {
      //
      //	Read some characters, if available, restarting a call interrupted
      //	by a signal.
      //
      char buffer[READ_BUFFER_SIZE];
      ssize_t n;
      do
	n = read(fd, buffer, READ_BUFFER_SIZE);
      while (n == -1 && errno == EINTR);
 
      if (n > 0)
	{
	  ssize_t messageStart = 0;
	  for (ssize_t i = 0; i < n; ++i)
	    {
	      if (buffer[i] == '\n')
		{
		  ri->incomingErrorLine += Rope(buffer + messageStart, i - messageStart);
		  cerr << ri->incomingErrorLine << endl;
		  ri->incomingErrorLine.clear();
		  messageStart = i + 1;
		}
	    }
	  ssize_t nrCharsLeft = n - messageStart;
	  if (nrCharsLeft > 0)
	    {
	      ri->incomingErrorLine += Rope(buffer + messageStart, nrCharsLeft);
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
	      return true;
	    }
	  else
	    {
	      //
	      //	We don't know for sure that there are characters waiting
	      //	for us since we just clearing out any pending messages.
	      //
	    }
	  break;
	}
    }
  return false;
}
