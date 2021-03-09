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
//	List of all recognized constructors by the time subsystem.
//
//	We generate various chunks of linear (branching) code to handle
//	this hetrogeneous collection by macro expansion.
//	The format is:
//		MACRO(symbols name, symbols C++ class, number of args)
//
  MACRO(succSymbol, SuccSymbol, 1)
  MACRO(minusSymbol, MinusSymbol, 1)
  MACRO(stringSymbol, StringSymbol, 0)
  MACRO(timerOidSymbol, FreeSymbol, 1)

  MACRO(oneShotSymbol, Symbol, 0)
  MACRO(periodicSymbol, Symbol, 0)
  MACRO(dateSymbol, Symbol, 3)
  MACRO(timeSymbol, Symbol, 3)
  MACRO(timeZoneInfoSymbol, Symbol, 4)

  MACRO(getTimeSinceEpochMsg, FreeSymbol, 2)
  MACRO(gotTimeSinceEpochMsg, Symbol, 3)
  MACRO(getDateAndTimeMsg, FreeSymbol, 3)
  MACRO(gotDateAndTimeMsg, Symbol, 6)
  MACRO(getLocalDateAndTimeMsg, FreeSymbol, 3)
  MACRO(gotLocalDateAndTimeMsg, Symbol, 7)

  MACRO(createTimerMsg, FreeSymbol, 2)
  MACRO(createdTimerMsg, Symbol, 3)
  MACRO(startTimerMsg, FreeSymbol, 4)
  MACRO(startedTimerMsg, Symbol, 2)
  MACRO(timeOutMsg, Symbol, 2)
  MACRO(stopTimerMsg, FreeSymbol, 2)
  MACRO(stoppedTimerMsg, Symbol, 2)
  MACRO(deleteTimerMsg, FreeSymbol, 2)
  MACRO(deletedTimerMsg, Symbol, 2)

  MACRO(timeErrorMsg, Symbol, 3)
