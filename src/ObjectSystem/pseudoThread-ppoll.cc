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
//	ppoll() based multiplexing of signals and events on file
//	descriptors. This is more efficient than using pselect()
//	but ppoll() is non-POSIX.
//

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
      waitPointer = &zeroTime;  // must not block in ppoll()
    }

  static pollfd ufds[MAX_NR_FDS];
  int nfds = 0;
  //
  //	Construct array of active file descriptors.
  //
  for (int i = firstActive; i != NONE; i = fdInfo[i].nextActive)
    {
      Assert(fdInfo[i].flags != 0, "zero flags for " << i);
      //
      //	For each fd we want to watch, poll() needs its
      //	number and flags for the events we care about.
      //
      ufds[nfds].fd = i;
      ufds[nfds].events = fdInfo[i].flags;
      ++nfds;
    }
  //
  //	Now we look to see what happened.
  //
  //
  //	Decide on the signals to be atomically unblocked during ppoll().
  //
  sigset_t* maskForSystemCall = (normalSet != 0) ? normalSet : &oldset;
  int nrEvents = ppoll(ufds, nfds, waitPointer, maskForSystemCall);
  //
  //	We restore mask to what it was before we blocked SIGCHLD.
  //
  sigprocmask(SIG_SETMASK, &oldset, 0);
 
  if (nrEvents < 0)
    {
      Assert(errno == EINTR, "ppoll() failed with " << errno);
      return INTERRUPTED;  // treat all errors as interrupts
    }
  //
  //	Deal with the information returned by ppoll().
  //
  int returnValue = NOTHING_HAPPENED;
  if (nrEvents > 0)
    {
      //
      //	Dispatch pending events.
      //
      for (int i = 0; i < nfds; i++)
	{
	  int fd = ufds[i].fd;
	  FD_Info& info = fdInfo[fd];
	  short t = ufds[i].revents;
	  if (t != 0)
	    {
	      //
	      //	Before each callback we check if info.flags != 0
	      //	because an ealier callback might have called
	      //	clearFlags() to forget about this file descriptor.
	      //
	      Assert(!(t & POLLNVAL), "ppoll() said fd was not open!" << fd);
	      //
	      //	When we have an error we need to removed fd from
	      //	active list to avoid going into a spin.
	      //
	      if (t & POLLERR && info.flags != 0)
		{
		  info.flags = 0;
		  unlink(fd);
		  info.owner->doError(fd);
		  returnValue = EVENT_HANDLED;
		  continue;
		}
	      //
	      //	This is tricky. If we are wanting to read but
	      //	get disconnected:
	      //	Linux sets POLLHUP but not POLLIN;
	      //	BSD & Solaris set POLLIN but not POLLHUP;
	      //	DEC UNIX sets both flags even if there are still
	      //	  chars to read.
	      //
	      if (t & POLLIN && info.flags != 0)
		{
		  info.flags &= ~POLLIN;  // clear POLLIN flag
		  if (info.flags == 0)
		    unlink(fd);
		  info.owner->doRead(fd);
		  returnValue = EVENT_HANDLED;
		}
	      else if (t & POLLHUP && info.flags != 0)
		{
		  //
		  //	Need to removed fd from active list to avoid going
		  //	into a spin. But we don't want to do this until all
		  //	available chars have been read.
		  //
		  info.flags = 0;
		  unlink(fd);
		  info.owner->doHungUp(fd);
		  returnValue = EVENT_HANDLED;
		  continue;
		}
	      if (t & POLLOUT && info.flags != 0)
		{
		  info.flags &= ~POLLOUT;  // clear POLLOUT flag
		  if (info.flags == 0)
		    unlink(fd);
		  info.owner->doWrite(fd);
		  returnValue = EVENT_HANDLED;
		}
	    }
	}
      Assert(returnValue == EVENT_HANDLED, "ppoll() saw " << nrEvents
	     << " events but we didn't handle any");
    }
  return returnValue | childEvent;
}
