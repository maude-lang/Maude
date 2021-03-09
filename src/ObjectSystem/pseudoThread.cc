/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2021 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class PseudoThread.
//
#include <unistd.h>
#include <errno.h>

//      utility stuff
#include "macros.hh"
#include "pseudoThread.hh"

//	our stuff
#include "pseudoThreadSignal.cc"
#ifdef HAVE_PPOLL
#include "pseudoThread-ppoll.cc"
#else
#include "pseudoThread-pselect.cc"
#endif

const timespec PseudoThread::zeroTime = {0, 0};
PseudoThread::FD_Info PseudoThread::fdInfo[MAX_NR_FDS];
int PseudoThread::firstActive = NONE;
PseudoThread::CallbackMap PseudoThread::callbackMap;

PseudoThread::CallbackHandle
PseudoThread::requestCallback(const timespec& notBefore, long clientData)
{
  return callbackMap.insert(CallbackMap::value_type(notBefore, CallbackRequest(this, clientData)));
}

void
PseudoThread::cancelCallback(const CallbackHandle& handle)
{
  Assert(handle->second.client == this, "client mismatch");
  callbackMap.erase(handle);
}

bool
PseudoThread::processCallbacks(int& returnValue, timespec& wait)
{
  //
  //	We handle any callbacks that are due.
  //	If there are more callbacks pending we put the relative
  //	time from now to the earliest of these in wait and return true.
  //	If there are no callbacks pending we return false.
  //
  timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  do
    {
      CallbackMap::iterator firstEvent = callbackMap.begin();
      if (now < firstEvent->first)
	{
	  //
	  //	Earliest event has time greater than now.
	  //	Compute the time to wait as a timespec.
	  //
	  timespecSubtract(firstEvent->first, now, wait);
	  return true;
	}
      PseudoThread* client = firstEvent->second.client;
      long clientData = firstEvent->second.clientData;
      callbackMap.erase(firstEvent);
      client->doCallback(clientData);
      returnValue |= EVENT_HANDLED;
    }
  while (!(callbackMap.empty()));
  return false;
}

int
PseudoThread::eventLoop(bool block, sigset_t* normalSet)
{
  //
  //	If block is true we make use of normalSet and
  //	return for one or more of 3 reasons:
  //	  NOTHING_PENDING: no clients waiting for callbacks
  //	  INTERRUPTED: a non-ignored signal arrived
  //	  EVENT_HANDLED: at least one callback was made
  //	If block is false, we ignore normalSet and can also
  //	return:
  //	  NOTHING_HAPPENED: events pending but nothing ready
  //
  int returnValue = NOTHING_HAPPENED;
  do
    {
      //
      //	If we have timed call backs, dispatch any that are due, and
      //	compute how long until the next is ready to go.
      //
      timespec wait;
      bool callbacksPending = false;
      if (!(callbackMap.empty()))
	{
	  //
	  //	See if any timed callbacks can be handled and
	  //	find out if there are any still pending.
	  //
	  callbacksPending = processCallbacks(returnValue, wait);
	  //
	  //	If we handled a timed callback, we shouldn't block
	  //	so that we return and let caller deal with any changes.
	  //
	  if (returnValue != NOTHING_HAPPENED)
	    block = false;
	}
      //
      //	If there are events still to happen, proccess them,
      //	suspending until they happen if we are in blocking mode.
      //
      //	Events can be:
      //	(1) File descriptor events
      //	(2) Child exits
      //
      //	We also do a suspend if there are timed callbacks and
      //	we're in blocking mode.
      //
      if (firstActive != NONE ||  // file descriptor events
	  !(childRequests.empty()) ||  // child exit callbacks
	  (callbacksPending && block))  // timed callbacks AND we're blocking
	{
	  //
	  //	Calculate how long to wait for an fd or interrupt event.
	  //
	  const timespec* waitPointer = 0;  // block with infinite wait
	  if (block)
	    {
	      if (callbacksPending)
		waitPointer = &wait;  // block until earliest timed callback
	      DebugInfo("wait = " << wait.tv_sec << " returnValue = " << returnValue);
	    }
	  else
	    waitPointer = &zeroTime;  // don't block
	  //
	  //	Process fd and interrupt events.
	  //
	  //	processFds() can return NOTHING_HAPPENED if
	  //	we're not blocking (zero wait time) or we timed out.
	  //
	  returnValue |= processFds(waitPointer, normalSet);
	  //
	  //	Do we still have events pending?
	  //
	  if (firstActive == NONE &&  // no file descriptor events
	      childRequests.empty() &&  // no child exit events
	      !callbacksPending)  // no timed callbacks
	    returnValue |= NOTHING_PENDING;
	}
      else
	{
	  //
	  //	We aren't waiting for file descriptor events or child exits.
	  //	We could have timed callbacks pending if we are not in block mode.
	  //
	  if (!callbacksPending)
	    returnValue |= NOTHING_PENDING;
	}
      DebugInfo("returnValue = " << returnValue);
      //
      //	It's possible we have events pending (say a timed callback)
      //	and we got here without handling any events (processFds() timed out).
      //	If this case, if we're still blocking, we loop to handle the events
      //	since we don't want to return until we handle an event, get interrupted
      //	or have no pending events.
      //
    }
  while (block && returnValue == NOTHING_HAPPENED);
  return returnValue;
}

void
PseudoThread::clearFlags(int fd)
{
  //
  //	Forget about this file descriptor.
  //
  if (fdInfo[fd].flags != 0)
    {
      fdInfo[fd].flags = 0;
      unlink(fd);
    }
}

void
PseudoThread::wantTo(int flags, int fd)
{
  Assert(flags != 0, "empty flags for " << fd);
  int t = fdInfo[fd].flags;
  if (t == 0)
    {
      link(fd);
      fdInfo[fd].owner = this;
    }
  else
    Assert(fdInfo[fd].owner == this, "owner mismatch for " << fd);
  fdInfo[fd].flags = t | flags;
}

void
PseudoThread::doRead(int fd)
{
  CantHappen("failed to do read on " << fd);
}

void
PseudoThread::doWrite(int fd)
{
  CantHappen("failed to do write on " << fd);
}

void
PseudoThread::doError(int fd)
{
  CantHappen("failed to do error on " << fd);
}

void
PseudoThread::doHungUp(int fd)
{
  CantHappen("failed to do hung up on " << fd);
}

void
PseudoThread::doCallback(long /* clientData */)
{
  CantHappen("failed to do call back");
}

void
PseudoThread::link(int fd)
{
  fdInfo[fd].prevActive = NONE;
  fdInfo[fd].nextActive = firstActive;
  if (firstActive != NONE)
    fdInfo[firstActive].prevActive = fd;
  firstActive = fd;
}

void
PseudoThread::unlink(int fd)
{
  int prev = fdInfo[fd].prevActive;
  int next = fdInfo[fd].nextActive;
  if (next != NONE)
    fdInfo[next].prevActive = prev;
  if (prev != NONE)
    fdInfo[prev].nextActive = next;
  else
    firstActive = next;
}
