/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020-2021 SRI International, Menlo Park, CA 94025, USA.

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
//	pselect() based multiplexing of signals and events on file
//	descriptors. This is somewhat inefficient, but POSIX compliant.
//
#include <sys/select.h>

int
PseudoThread::processFds(const timespec* waitPointer, sigset_t* normalSet)
{
  //
  //	*waitPointer is how long to wait for an event before returning.
  //	null pointer indicated infinite wait.
  //
  //	normalSet is signal mask that corresponds to signals we
  //	want to be able to interrupt a blocking system call. Some
  //	of these may be currently blocked to avoid losing the
  //	signals if they are delivered before the system call happens.
  //
 int childEvent = NOTHING_HAPPENED;
  //
  //	Block SIGCHLD.
  //
  sigset_t oldset;
  sigset_t newset;
  sigemptyset(&newset);
  sigaddset(&newset, SIGCHLD);
  sigprocmask(SIG_BLOCK, &newset, &oldset);
  //
  //	Deal with any child exit notifications that became due.
  //
  if (dispatchChildRequests())
    {
      childEvent |= EVENT_HANDLED;
      waitPointer = &zeroTime;  // must not block in pselect()
    }

  static fd_set readfds;
  static fd_set writefds;
  //
  //	Clear fd_sets to start with.
  //
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  //
  //	Fill fd_sets base on active file descriptors.
  //	We use flags based on poll()/ppoll() internally.
  //
  int maxFd = -1;
  for (int i = firstActive; i != NONE; i = fdInfo[i].nextActive)
    {
      //
      //	We can't assume that fds will be visited in ascending order.
      //
      short flags = fdInfo[i].flags;
      Assert(flags != 0, "zero flags for " << i);
      //
      //	For each fd we want to watch, poll() needs its
      //	number and flags for the events we care about.
      //
      if (flags & POLLIN)
	{
	  FD_SET(i, &readfds);
	  if (i > maxFd)
	    maxFd = i;
	}
      if (flags & POLLOUT)
	{
	  FD_SET(i, &writefds);
	  if (i > maxFd)
	    maxFd = i;
	}
    }
  //
  //	Now we look to see what happened.
  //
  //
  //	Decide on the signals to be atomically unblocked during pselect().
  //
  sigset_t* maskForSystemCall = (normalSet != 0) ? normalSet : &oldset;
  int nrEvents = pselect(maxFd + 1, &readfds, &writefds, 0, waitPointer, maskForSystemCall);
  //
  //	We restore mask to what it was before we blocked SIGCHLD.
  //
  sigprocmask(SIG_SETMASK, &oldset, 0);

  if (nrEvents < 0)
    {
      Assert(errno == EINTR, "pselect() failed with " << errno);
      return INTERRUPTED;  // treat all errors as interrupts
    }
  //
  //	Deal with the information returned by pselect().
  //
  int returnValue = NOTHING_HAPPENED;
  if (nrEvents > 0)
    {
      for (int i = firstActive; i != NONE; i = fdInfo[i].nextActive)
	{
	  FD_Info& info = fdInfo[i];
	  //
	  //	Before each callback we check if info.flags != 0
	  //	because an ealier callback might have called
	  //	clearFlags() to forget about this file descriptor.
	  //
	  if (FD_ISSET(i, &readfds) && info.flags != 0)
	    {
	      info.flags &= ~POLLIN;  // clear POLLIN flag
	      if (info.flags == 0)
		unlink(i);
	      info.owner->doRead(i);
	      returnValue = EVENT_HANDLED;
	    }
	  if (FD_ISSET(i, &writefds) && info.flags != 0)
	    {
	      info.flags &= ~POLLOUT;  // clear POLLOUT flag
	      if (info.flags == 0)
		unlink(i);
	      info.owner->doWrite(i);
	      returnValue = EVENT_HANDLED;
	    }
	}
      Assert(returnValue == EVENT_HANDLED, "pselect() saw " << nrEvents
	     << " events but we didn't handle any");
    }
  return returnValue | childEvent;
}
