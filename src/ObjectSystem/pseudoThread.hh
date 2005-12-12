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

  static int eventLoop();
  static void clearFlags(int fd);
  void wantTo(int flags, int fd, long timeOutAt = NONE);
  void requestCallBack(long notBefore = 0);

  virtual void doRead(int fd);
  virtual void doWrite(int fd);
  virtual void doError(int fd);
  virtual void doHungUp(int fd);
  virtual void doTimeOut(int fd);
  virtual void doCallBack();

private:
  enum Values
  {
    MAX_NR_FDS = 1024
  };

  struct FD_Info
  {
    PseudoThread* owner;
    short flags;
    short nextActive;
    short prevActive;
    long timeOutAt;
  };

  struct CallBackRequest
  {
    CallBackRequest(PseudoThread* client, long notBefore)
      : client(client),
	notBefore(notBefore)
    {
    }

    bool
    operator<(const CallBackRequest& c) const
    {
      return notBefore > c.notBefore;  // reversed!
    }

    PseudoThread* client;
    long notBefore;
  };

  typedef priority_queue<CallBackRequest> CallBackQueue;

  static long getTime();
  static long processCallBacks(int& returnValue);
  static int processFds(long wait);
  static void link(int fd);
  static void unlink(int fd);

  static FD_Info fdInfo[MAX_NR_FDS];
  static int firstActive;
  static CallBackQueue callBackQueue;
};

inline long
PseudoThread::getTime()
{
  timeval time;
  gettimeofday(&time, 0);
  return time.tv_sec;
}

#endif
