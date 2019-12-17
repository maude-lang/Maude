/*

    This file is part of the Maude 3 interpreter.

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
//      Implementation for class Timer.
//
#include <signal.h>
#include <sys/time.h>

//      utility stuff
#include "macros.hh"

#include "timer.hh"

bool Timer::osTimersStarted = false;

local_inline Int64
Timer::calculateMicroseconds(const itimerval& startTime,
			     const itimerval& stopTime)
{
  const Int64 M = 1000000;
  Int64 usec = startTime.it_value.tv_usec - stopTime.it_value.tv_usec +
    M * (startTime.it_value.tv_sec - stopTime.it_value.tv_sec);
  if (usec < 0)  // timer wrap around
    usec += M * CYCLE_LENGTH;
  return usec;
}

void
Timer::startOsTimers()
{
  //
  //	It would be nice to keep track of cycles for correct handling of
  //	very long time intervals, but this seems hard without introducing
  //	race conditions; so we just ignore all timer interrupts.
  //
  signal(SIGALRM, SIG_IGN);
  signal(SIGVTALRM, SIG_IGN);
  signal(SIGPROF, SIG_IGN);
  static itimerval init = { {CYCLE_LENGTH, 0}, {CYCLE_LENGTH, 0} };
  setitimer(ITIMER_REAL, &init, 0);
  setitimer(ITIMER_VIRTUAL, &init, 0);
  setitimer(ITIMER_PROF, &init, 0);
  osTimersStarted = true;
}

Timer::Timer(bool startRunning)
{
  realAcc = 0;
  virtAcc = 0;
  profAcc = 0;
  running = false;
  valid = true;
  if (startRunning)
    {
      if (!osTimersStarted)
	startOsTimers();
      running = true;
      getitimer(ITIMER_REAL, &realStartTime);
      getitimer(ITIMER_VIRTUAL, &virtStartTime);
      getitimer(ITIMER_PROF, &profStartTime);
    }
}

void
Timer::start()
{
  if (!running && valid)
    {
      if (!osTimersStarted)
	startOsTimers();
      running = true;
      getitimer(ITIMER_REAL, &realStartTime);
      getitimer(ITIMER_VIRTUAL, &virtStartTime);
      getitimer(ITIMER_PROF, &profStartTime);
    }
  else
    valid = false;
}

void 
Timer::stop()
{
  if (running && valid)
    {
      itimerval realStopTime;
      itimerval virtStopTime;
      itimerval profStopTime;
      getitimer(ITIMER_PROF, &profStopTime);
      getitimer(ITIMER_VIRTUAL, &virtStopTime);
      getitimer(ITIMER_REAL, &realStopTime);
      running = false;
      realAcc += calculateMicroseconds(realStartTime, realStopTime);
      virtAcc += calculateMicroseconds(virtStartTime, virtStopTime);
      profAcc += calculateMicroseconds(profStartTime, profStopTime);
    }
  else
    valid = false;
}

bool
Timer::getTimes(Int64& real, Int64& virt, Int64& prof) const
{
  if (valid)
    {
      real = realAcc;
      virt = virtAcc;
      prof = profAcc;
      if (running)
	{
	  itimerval realStopTime;
	  itimerval virtStopTime;
	  itimerval profStopTime;
	  getitimer(ITIMER_PROF, &profStopTime);
	  getitimer(ITIMER_VIRTUAL, &virtStopTime);
	  getitimer(ITIMER_REAL, &realStopTime);
	  real += calculateMicroseconds(realStartTime, realStopTime);
	  virt += calculateMicroseconds(virtStartTime, virtStopTime);
	  prof += calculateMicroseconds(profStartTime, profStopTime);
	}
      return true;
    }
  return false;
}
