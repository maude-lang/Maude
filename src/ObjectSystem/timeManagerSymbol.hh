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
//      Class to handle time manager symbols.
//
#ifndef _timeManagerSymbol_hh_
#define _timeManagerSymbol_hh_
#include <map>
#include "externalObjectManagerSymbol.hh"
#include "pseudoThread.hh"

class TimeManagerSymbol
  : public ExternalObjectManagerSymbol,
    public PseudoThread
{
  NO_COPYING(TimeManagerSymbol);

public:
  TimeManagerSymbol(int id);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);

  void copyAttachments(Symbol* original, SymbolMap* map);

  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);
  //
  //	Overridden methods from ExternalObjectManagerSymbol.
  //
  bool handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context);
  bool handleMessage(DagNode* message, ObjectSystemRewritingContext& context);
  void cleanUp(DagNode* objectId);

private:
  enum Special
    {
     BASE_YEAR = 1900,
     BILLION = 1000000000
    };

  struct Timer
  {
    timespec callbackRequestTime;	// tv_sec = 0 implies no callback pending
    Int64 period;		// 0 = oneShot
    DagRoot lastStartMessage;	// message that started count
    ObjectSystemRewritingContext* objectContext;	// for message injection
    CallbackHandle callbackHandle;	// for cancellation
  };

  typedef map<int, Timer> TimerMap;

  //
  //	Overridden method from PseudoThread.
  //
  void doCallback(long id);

  void getTimeSinceEpoch(FreeDagNode* message, ObjectSystemRewritingContext& context);
  void getDateAndTime(FreeDagNode* message, ObjectSystemRewritingContext& context);
  void getLocalDateAndTime(FreeDagNode* message, ObjectSystemRewritingContext& context);
  void createTimer(FreeDagNode* message, ObjectSystemRewritingContext& context);
  void startTimer(FreeDagNode* message, ObjectSystemRewritingContext& context);
  void stopTimer(FreeDagNode* message, ObjectSystemRewritingContext& context);
  void deleteTimer(FreeDagNode* message, ObjectSystemRewritingContext& context);

  DagNode* makeDate(const tm* timeData);
  DagNode* makeTime(const tm* timeData);
  DagNode* makeTimeZoneInfo(const tm* timeData);

  void getTimer(DagNode* timerArg, int& id, Timer*& timer);
  int findUnusedId();
  bool getMode(DagNode* mode, bool& periodic);

  void createdTimerReply(int id,
			 FreeDagNode* originalMessage,
			 ObjectSystemRewritingContext& context);
  void errorReply(const char* errorMessage,
		  FreeDagNode* originalMessage,
		  ObjectSystemRewritingContext& context);

#define MACRO(SymbolName, SymbolClass, NrArgs)	\
  SymbolClass* SymbolName;
#include "timeSignature.cc"
#undef MACRO

  TimerMap timerMap;
};

#endif
