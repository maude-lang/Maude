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
//      Class for i/o event driven pseudo threads.
//
#ifndef _pseudoThread_hh_
#define _pseudoThread_hh_
#include <sys/poll.h>
#include <queue>
#include "timeStuff.hh"

class PseudoThread
{
  NO_COPYING(PseudoThread);

public:
  PseudoThread() {}

  enum Flags
  {
    READ = POLLIN,
    WRITE = POLLOUT
  };

  enum ReturnStatus
  {
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
  //	If a call back was made or we were interrupted by a signal or there are no pending call backs, return appropriate code.
  //	Otherwise sleep until interrupted or a call back happens.
  //
  static int eventLoop();
  //
  //	Clear any requests for call backs on a given fd.
  //
  static void clearFlags(int fd);
  //
  //	Request a call back when reading or writing on a given fd is possible.
  //	timeOutAt is specified as an absolute number of seconds since the Unix Epoch, or NONE for never.
  //	Only one object can wait on a given fd.
  //
  void wantTo(int flags, int fd, long timeOutAt = NONE);
  //
  //	Request a call back at or after a given time.
  //	notBefore is specified as an absolute number of seconds since the Unix Epoch.
  //
  void requestCallBack(long notBefore);
  //
  //	Call back functions.
  //
  virtual void doRead(int fd);  // a read is possible (or the other end of a socket was closed for some OS)
  virtual void doWrite(int fd);  // a write is possible
  virtual void doError(int fd);   // a error happened
  virtual void doHungUp(int fd);  // the other end of a socket was closed when wanting to do a write (for some OS, when wanting to do a read)
  virtual void doTimeOut(int fd);  // timeOutAt time reached on an fd
  virtual void doCallBack();  // notBefore time reached for a requested call back

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
    long timeOutAt;
  };

  struct CallBackRequest
  {
    CallBackRequest(PseudoThread* client, long notBefore);

    bool operator<(const CallBackRequest& c) const;

    PseudoThread* client;
    long notBefore;
  };

  typedef priority_queue<CallBackRequest> CallBackQueue;

  static long getTime();
  static long processCallBacks(int& returnValue);
  static int processFds(long wait);
  static void link(int fd);
  static void unlink(int fd);
  //
  //	All data is shared between PseudoThread objects since it refers to a common set of fds and a global call back queue.
  //
  static FD_Info fdInfo[MAX_NR_FDS];
  static int firstActive;
  static CallBackQueue callBackQueue;
};

inline
PseudoThread::CallBackRequest::CallBackRequest(PseudoThread* client, long notBefore)
  : client(client),
    notBefore(notBefore)
{
}

inline bool
PseudoThread::CallBackRequest::operator<(const CallBackRequest& c) const
{
  return notBefore > c.notBefore;  // reversed!
}

inline long
PseudoThread::getTime()
{
  timeval time;
  gettimeofday(&time, 0);
  return time.tv_sec;
}

#endif
