/*

    This file is part of the Maude 3 interpreter.

    Copyright 2021-2023 SRI International, Menlo Park, CA 94025, USA.

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
#include <errno.h>

void
TimeManagerSymbol::getTimer(DagNode* timerArg, int& id, Timer*& timer)
{
  DagNode* idArg = safeCast(FreeDagNode*, timerArg)->getArgument(0);
  DebugSave(ok, succSymbol->getSignedInt(idArg, id));
  Assert(ok, "bad timer number " << timerArg);

  TimerMap::iterator i = timerMap.find(id);
  Assert(i != timerMap.end(), "missing timer " << i->first);
  timer = &(i->second);
}

void
TimeManagerSymbol::getTimeSinceEpoch(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  timespec timeValue;
  DebugSave(r, clock_gettime(CLOCK_REALTIME, &timeValue));
  Assert(r == 0, "clock_gettime() failed: " << strerror(errno));

  mpz_class nanoSeconds(timeValue.tv_sec);
  nanoSeconds *= BILLION;
  nanoSeconds += timeValue.tv_nsec;

  Vector<DagNode*> reply(3);
  DagNode* target = message->getArgument(1);
  reply[0] = target;
  reply[1] = message->getArgument(0);
  reply[2] = succSymbol->makeNatDag(nanoSeconds);
  context.bufferMessage(target, gotTimeSinceEpochMsg->makeDagNode(reply));
}

void
TimeManagerSymbol::errorReply(const char* errorMessage,
			      FreeDagNode* originalMessage,
			      ObjectSystemRewritingContext& context)
{
  Vector<DagNode*> reply(3);
  reply[1] = originalMessage->getArgument(0);
  reply[2] = new StringDagNode(stringSymbol, errorMessage);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, timeErrorMsg->makeDagNode(reply));
}

DagNode*
TimeManagerSymbol::makeDate(const tm* timeData)
{
  Vector<DagNode*> date(3);
  //
  //	We do the addition on bignums to avoid wrapping in extreme cases.
  //
  mpz_class year(timeData->tm_year);
  year += BASE_YEAR;
  date[0] = succSymbol->makeNatDag(year);
  date[1] = succSymbol->makeNatDag(1 + timeData->tm_mon);
  date[2] = succSymbol->makeNatDag(timeData->tm_mday);
  return dateSymbol->makeDagNode(date);
}

DagNode*
TimeManagerSymbol::makeTime(const tm* timeData)
{
  Vector<DagNode*> time(3);
  time[0] = succSymbol->makeNatDag(timeData->tm_hour);
  time[1] = succSymbol->makeNatDag(timeData->tm_min);
  time[2] = succSymbol->makeNatDag(timeData->tm_sec);
  return timeSymbol->makeDagNode(time);
}

void
TimeManagerSymbol::getDateAndTime(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Int64 seconds;
  if (succSymbol->getScaledSignedInt64(message->getArgument(2), BILLION, seconds))
    {
      time_t secs = seconds;  // need to pass pointer to time_t
      tm result;
      if (gmtime_r(&secs, &result))
	{ 
	  Vector<DagNode*> reply(6);
	  DagNode* target = message->getArgument(1);
	  reply[0] = target;
	  reply[1] = message->getArgument(0);
	  reply[2] = makeDate(&result);
	  reply[3] = makeTime(&result);
	  reply[4] = succSymbol->makeNatDag(1 + result.tm_yday);
	  reply[5] = succSymbol->makeNatDag(1 + result.tm_wday);
	  context.bufferMessage(target, gotDateAndTimeMsg->makeDagNode(reply));
	}
      else
	errorReply("Time out-of-range.", message, context);
    }
  else
    errorReply("Bad time.", message, context);
}

DagNode*
TimeManagerSymbol::makeTimeZoneInfo(const tm* timeData)
{
  //
  //	C standard library variables encoding time zone information.
  //	We don't both with
  //	  extern int daylight;
  //	since we expect tzname[1] to be the null string if this is 0.
  //
  extern char *tzname[2];
  extern long timezone;

  Vector<DagNode*> time(4);
  time[0] = new StringDagNode(stringSymbol, tzname[0]);
  time[1] = new StringDagNode(stringSymbol, tzname[1]);
  //
  //	For some reason timezone is +ve west of the Prime Meridian whereas
  //	the opposite convention is used for timeData->tm_gmtoff.
  //	Also timeData->tm_gmtoff takes daylight saving into account while
  //	timezone does not.
  //	We use timezone because tm_gmtoff is an extension but negate it
  //	because the tm_gmtoff convention is more intuitive.
  //
  time[2] = minusSymbol->makeIntDag(- timezone);
  time[3] = minusSymbol->makeIntDag(timeData->tm_isdst);
  return timeZoneInfoSymbol->makeDagNode(time);
}

void
TimeManagerSymbol::getLocalDateAndTime(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Int64 seconds;
  if (succSymbol->getScaledSignedInt64(message->getArgument(2), BILLION, seconds))
    {
      time_t secs = seconds;  // need to pass pointer to time_t
      //
      //	We use localtimer() rather than localtimer_r() to ensure
      //	that the external time zone variables are set.
      //
      if (tm* result = localtime(&secs))
	{ 
	  Vector<DagNode*> reply(7);
	  DagNode* target = message->getArgument(1);
	  reply[0] = target;
	  reply[1] = message->getArgument(0);
	  reply[2] = makeDate(result);
	  reply[3] = makeTime(result);
	  reply[4] = succSymbol->makeNatDag(1 + result->tm_yday);
	  reply[5] = succSymbol->makeNatDag(1 + result->tm_wday);
	  reply[6] = makeTimeZoneInfo(result);
	  context.bufferMessage(target, gotLocalDateAndTimeMsg->makeDagNode(reply));
	}
      else
	errorReply(strerror(errno), message, context);
    }
  else
    errorReply("Bad time.", message, context);
}

int
TimeManagerSymbol::findUnusedId()
{
  int id = 0;
  for (auto& i : timerMap)
    {
      if (i.first != id)
	break;
      ++id;
    }
  return id;
}

void
TimeManagerSymbol::createdTimerReply(int id,
				     FreeDagNode* originalMessage,
				     ObjectSystemRewritingContext& context)
{
  
  Vector<DagNode*> reply(3);
  reply.resize(1);
  reply[0] = succSymbol->makeNatDag(id);
  DagNode* timerName = timerOidSymbol->makeDagNode(reply);
  context.addExternalObject(timerName, this);
  reply.resize(3);
  reply[2] = timerName;
  reply[1] = originalMessage->getArgument(0);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, createdTimerMsg->makeDagNode(reply));
}

void
TimeManagerSymbol::createTimer(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int id = findUnusedId();
  Timer& t = timerMap[id];
  t.callbackRequestTime.tv_sec = 0;
  t.callbackRequestTime.tv_nsec = 0;
  t.period = 0;
  t.objectContext = 0;
  createdTimerReply(id, message, context);
}

bool
TimeManagerSymbol::getMode(DagNode* mode, bool& periodic)
{
  Symbol* ms = mode->symbol();
  if (ms == oneShotSymbol)
    periodic = false;
  else if (ms == periodicSymbol)
    periodic = true;
  else
    return false;
  return true;
}

void
TimeManagerSymbol::startTimer(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  bool periodic;
  if (getMode(message->getArgument(2), periodic))
    {
      Int64 nanoseconds;
      if (succSymbol->getSignedInt64(message->getArgument(3), nanoseconds) && nanoseconds > 0)
	{
	  DagNode* timerArg = message->getArgument(0);
	  DagNode* idArg = safeCast(FreeDagNode*, timerArg)->getArgument(0);
	  int id;
	  DebugSave(ok, succSymbol->getSignedInt(idArg, id));
	  Assert(ok, "bad timer number " << timerArg);
	  
	  TimerMap::iterator i = timerMap.find(id);
	  Timer& t = i->second;

	  if (t.callbackRequestTime.tv_sec != 0)
	    {
	      //
	      //	We already had a callback request pending - need
	      //	to cancel it.
	      //
	      DebugInfo("cancelling pending callback");
	      cancelCallback(t.callbackHandle);
	    }
	  //
	  //	Start with the current time.
	  //
	  DebugSave(r, clock_gettime(CLOCK_REALTIME, &t.callbackRequestTime));
	  Assert(r == 0, "clock_gettime() failed: " << strerror(errno));
	  //
	  //	Increase it by nanoseconds.
	  //
	  t.callbackRequestTime.tv_sec += nanoseconds / BILLION;
	  t.callbackRequestTime.tv_nsec += nanoseconds % BILLION;
	  if (t.callbackRequestTime.tv_nsec >= BILLION)
	    {
	      t.callbackRequestTime.tv_nsec -= BILLION;
	      t.callbackRequestTime.tv_sec += 1;
	    }
	  //
	  //	If we are periodic, we will be bumping this value by nanoseconds
	  //	each time we get a callback, so we need to save nanoseconds.
	  //
	  t.period = periodic ? nanoseconds : 0;
	  //
	  //	We save the start message and the context so we can insert the
	  //	time out message.
	  //
	  t.lastStartMessage.setNode(message);
	  t.objectContext = &context;
	  //
	  //	Request callback at or after the time we calculated.
	  //
	  t.callbackHandle = requestCallback(t.callbackRequestTime, id);
	  trivialReply(startedTimerMsg, message, context);
	}
      else
	errorReply("Bad timer start value.", message, context);
    }
  else
    errorReply("Bad timer mode.", message, context);
}

void
TimeManagerSymbol::stopTimer(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  DagNode* timerArg = message->getArgument(0);
  int id;
  Timer* timer;
  getTimer(timerArg, id, timer);
  if (timer->callbackRequestTime.tv_sec != 0)
    {
      DebugInfo("cancelling pending callback");
      cancelCallback(timer->callbackHandle);
      timer->callbackRequestTime.tv_sec = 0;
      timer->callbackRequestTime.tv_nsec = 0;
      timer->lastStartMessage.setNode(0);
      trivialReply(stoppedTimerMsg, message, context);
    }
}
  
void
TimeManagerSymbol::deleteTimer(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  DagNode* timerArg = message->getArgument(0);
  int id;
  Timer* timer;
  getTimer(timerArg, id, timer);
  if (timer->callbackRequestTime.tv_sec != 0)
    {
      DebugInfo("cancelling pending callback");
      cancelCallback(timer->callbackHandle);
    }
  DebugInfo("deleting " << timerArg);
  timerMap.erase(id);
  context.deleteExternalObject(timerArg);
  trivialReply(deletedTimerMsg, message, context);
}
