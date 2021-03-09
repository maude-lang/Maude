/*

    This file is part of the Maude 3 interpreter.

    Copyright 2021 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class TimeManagerSymbol.
//

//      utility stuff
#include "timeStuff.hh"
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "NA_Theory.hh"
#include "S_Theory.hh"
#include "builtIn.hh"
#include "objectSystem.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "symbolMap.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//      built in class definitions
#include "succSymbol.hh"
#include "minusSymbol.hh"
#include "stringSymbol.hh"
#include "stringDagNode.hh"
#include "bindingMacros.hh"

//	object system class definitions
#include "objectSystemRewritingContext.hh"
#include "timeManagerSymbol.hh"

//	our stuff
#include "timeActions.cc"

TimeManagerSymbol::TimeManagerSymbol(int id)
  : ExternalObjectManagerSymbol(id)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolName = 0;
#include "timeSignature.cc"
#undef MACRO
}

bool
TimeManagerSymbol::attachData(const Vector<Sort*>& opDeclaration,
				const char* purpose,
				const Vector<const char*>& data)
{
  NULL_DATA(purpose, TimeManagerSymbol, data);
  return ExternalObjectManagerSymbol::attachData(opDeclaration, purpose, data);
}

bool
TimeManagerSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  Assert(symbol != 0, "null symbol for " << purpose);
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  BIND_SYMBOL(purpose, symbol, SymbolName, SymbolClass*)
#include "timeSignature.cc"
#undef MACRO
  return ExternalObjectManagerSymbol::attachSymbol(purpose, symbol);
}

void
TimeManagerSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  TimeManagerSymbol* orig = safeCast(TimeManagerSymbol*, original);
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  COPY_SYMBOL(orig, SymbolName, map, SymbolClass*)
#include "timeSignature.cc"
#undef MACRO
  ExternalObjectManagerSymbol::copyAttachments(original, map);
}

void
TimeManagerSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				      Vector<const char*>& purposes,
				      Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "TimeManagerSymbol";
  data.resize(nrDataAttachments + 1);
  ExternalObjectManagerSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
TimeManagerSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					  Vector<Symbol*>& symbols)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  APPEND_SYMBOL(purposes, symbols, SymbolName)
#include "timeSignature.cc"
#undef MACRO
  ExternalObjectManagerSymbol::getSymbolAttachments(purposes, symbols);
}

bool
TimeManagerSymbol::handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  DebugInfo("TimeManagerSymbol::handleManagerMessage(): saw " << message);
  Symbol* s = message->symbol();

  if (s == getTimeSinceEpochMsg)
    getTimeSinceEpoch(safeCast(FreeDagNode*, message), context);
  else if (s == getDateAndTimeMsg)
    getDateAndTime(safeCast(FreeDagNode*, message), context);
  else if (s == getLocalDateAndTimeMsg)
    getLocalDateAndTime(safeCast(FreeDagNode*, message), context);
  else if (s == createTimerMsg)
    createTimer(safeCast(FreeDagNode*, message), context);
  else
    return false;
  return true;
}

bool
TimeManagerSymbol::handleMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  DebugEnter(message);

  Symbol* s = message->symbol();
  if (s == startTimerMsg)
    startTimer(safeCast(FreeDagNode*, message), context);
  else if (s == stopTimerMsg)
    stopTimer(safeCast(FreeDagNode*, message), context);
  else if (s == deleteTimerMsg)
    deleteTimer(safeCast(FreeDagNode*, message), context);
  else
    return false;
  return true;
}

void
TimeManagerSymbol::cleanUp(DagNode* objectId)
{
  DebugEnter(objectId);

  int id;
  Timer* timer;
  getTimer(objectId, id, timer);
  //
  //	Check if we had a nonzero callbackRequestTime; this
  //	implies we have a callback request pending.
  //
  if (timer->callbackRequestTime.tv_sec != 0)
    {
      DebugInfo("cancelling pending callback");
      cancelCallback(timer->callbackHandle);
    }
  timerMap.erase(id);
}

void
TimeManagerSymbol::doCallback(long id)
{
  DebugEnter(id);

  TimerMap::iterator i = timerMap.find(id);
  Assert(i != timerMap.end(), "missing timer " << i->first);

  Timer& t = i->second;
  ObjectSystemRewritingContext& context = *(t.objectContext);
  FreeDagNode* message = safeCast(FreeDagNode*, t.lastStartMessage.getNode());
  if (t.period == 0)
    {
      //
      //	One shot mode so clear callbackRequestTime so we don't
      //	try to cancel this callback now it has happened.
      //	Also we clear lastStartMessage so it can be garbage collected.
      //
      t.callbackRequestTime.tv_sec = 0;
      t.callbackRequestTime.tv_nsec = 0;
      t.lastStartMessage.setNode(0);
    }
  else
    {
      //
      //	Periodic mode so set up the next callback.
      //
      t.callbackRequestTime.tv_sec += t.period / BILLION;
      t.callbackRequestTime.tv_nsec += t.period % BILLION;
      if (t.callbackRequestTime.tv_nsec >= BILLION)
	{
	  t.callbackRequestTime.tv_nsec -= BILLION;
	  t.callbackRequestTime.tv_sec += 1;
	}
      t.callbackHandle = requestCallback(t.callbackRequestTime, id);
    }
  trivialReply(timeOutMsg, message, context);
}
