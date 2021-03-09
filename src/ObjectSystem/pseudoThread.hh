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
//      Class for i/o event driven pseudo threads.
//
#ifndef _pseudoThread_hh_
#define _pseudoThread_hh_
#include <poll.h>
#include <signal.h>
#include <queue>
#include "timeStuff.hh"
#include "vector.hh"
#include <map>
class PseudoThread
{
  NO_COPYING(PseudoThread);

private:
  struct CallbackRequest
  {
    CallbackRequest(PseudoThread* client, long clientData);

    PseudoThread* client;
    long clientData;
  };

  //
  //	We would prefer to use priority_queue<> to have constant
  //	time access to the least element, but this template
  //	doesn't allow deletion of other elements for callback
  //	request cancellation.
  //
  typedef multimap<timespec, CallbackRequest> CallbackMap;

public:
  typedef CallbackMap::iterator CallbackHandle;

  PseudoThread() {}

  enum Flags
  {
    READ = POLLIN,
    WRITE = POLLOUT
  };

  enum ReturnStatus
  {
    //
    //	Clients are waiting for events but nothing happened.
    //
    NOTHING_HAPPENED = 0,
    //
    //	No clients are waiting for events.
    //
    NOTHING_PENDING = 1,
    //
    //	poll() was aborted by a signal.
    //
    INTERRUPTED = 2,
    //
    //	Events were processed during the eventLoop() call.
    //
    EVENT_HANDLED = 4
  };
  
  //
  //	Make any pending call backs.
  //	If a call back was made or we were interrupted by a signal or
  //	there are no pending call backs, return appropriate code.
  //	Otherwise sleep until interrupted or a call back happens.
  //
  //	If we are in blocking mode, normalSet can be passed to indicate
  //	that some signals were blocked to avoid them being missed before
  //	a blocking system call. normalSet will be installed during
  //	the blocking system call.
  //
  static int eventLoop(bool block = true, sigset_t* normalSet = 0);
  //
  //	Clear any requests for callbacks on a given fd.
  //
  static void clearFlags(int fd);
  //
  //	Request a callback when reading or writing on a given fd is possible.
  //	Only one object can wait on a given fd.
  //
  void wantTo(int flags, int fd);
  //
  //	Request a callback at or after a given time.
  //	notBefore is specified as an absolute timespec since the Unix Epoch.
  //
  CallbackHandle requestCallback(const timespec& notBefore, long clientData);
  //
  //	Cancel a previous request. Canceling a callback that has already
  //	been delivered or cancelled will result in memory corruption.
  //
  void cancelCallback(const CallbackHandle& handle);
  //
  //	Request a call back when a child exits and cancel a previous request.
  //
  void requestChildExitCallback(pid_t childPid);
  void cancelChildExitCallback(pid_t childPid);
  //
  //	Call back functions.
  //
  virtual void doRead(int fd);  // a read is possible (or the other end of a socket was closed for some OS)
  virtual void doWrite(int fd);  // a write is possible
  virtual void doError(int fd);   // a error happened
  virtual void doHungUp(int fd);  // the other end of a socket was closed when wanting to do a write (for some OS, when wanting to do a read)
  virtual void doCallback(long clientData);  // notBefore time reached for a requested call back
  virtual void doChildExit(pid_t childPid);

private:
  enum Values
  {
    MAX_NR_FDS = 1024
  };

  struct FD_Info
  {
    PseudoThread* owner;  // only one owner allowed per fd
    short flags;
    short nextActive;  // maintain a linked list of active fds (those with non-zero flags)
    short prevActive;
  };


  struct ChildRequest
  {
    ChildRequest() {} // need for Vector template
    ChildRequest(PseudoThread* client, pid_t childPid);
    
    PseudoThread* client;
    pid_t pid;
    bool exited;
  };

  static bool processCallbacks(int& returnValue, timespec& wait);
  static int processFds(const timespec* waitPointer, sigset_t* normalSet);
  static void link(int fd);
  static void unlink(int fd);

  static void sigchldHandler(int signalNr, siginfo_t* info, void* context);
  static bool dispatchChildRequests();
  //
  //	All data is shared between PseudoThread objects since it refers to
  //	a common set of fds and a global callback map.
  //
  static const timespec zeroTime;
  static FD_Info fdInfo[MAX_NR_FDS];
  static int firstActive;
  static CallbackMap callbackMap;
  //
  //	Data for handling SIGCHLD.
  //
  static Vector<ChildRequest> childRequests;
  static bool installedSigchldHandler;
  static bool exitedFlag;
};

inline
PseudoThread::CallbackRequest::CallbackRequest(PseudoThread* client, long clientData)
  : client(client),
    clientData(clientData)
{
}

inline
PseudoThread::ChildRequest::ChildRequest(PseudoThread* client, pid_t pid)
  : client(client),
    pid(pid)
{
  exited = false;
}

#endif
