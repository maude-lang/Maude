/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017-2021 SRI International, Menlo Park, CA 94025, USA.

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
//	List of all recognized constructors by the file subsystem.
//
//	We generate various chunks of linear (branching) code to handle
//	this hetrogeneous collection by macro expansion.
//	The format is:
//		MACRO(symbols name, symbols C++ class, number of args)
//
  MACRO(succSymbol, SuccSymbol, 1)
  MACRO(minusSymbol, MinusSymbol, 1)
  MACRO(stringSymbol, StringSymbol, 0)
  MACRO(fileOidSymbol, FreeSymbol, 1)

  MACRO(startSymbol, Symbol, 0)
  MACRO(currentSymbol, Symbol, 0)
  MACRO(endSymbol, Symbol, 0)

  MACRO(openFileMsg, FreeSymbol, 4)
  MACRO(openedFileMsg, Symbol, 3)
  MACRO(removeFileMsg, FreeSymbol, 3)
  MACRO(removedFileMsg, Symbol, 2)

  MACRO(getLineMsg, FreeSymbol, 2)
  MACRO(gotLineMsg, Symbol, 3)
  MACRO(getCharsMsg, FreeSymbol, 3)
  MACRO(gotCharsMsg, Symbol, 3)

  MACRO(writeMsg, FreeSymbol, 3)
  MACRO(wroteMsg, Symbol, 2)
  MACRO(flushMsg, FreeSymbol, 2)
  MACRO(flushedMsg, Symbol, 2)

  MACRO(setPositionMsg, FreeSymbol, 4)
  MACRO(positionSetMsg, Symbol, 2)
  MACRO(getPositionMsg, FreeSymbol, 2)
  MACRO(positionGotMsg, Symbol, 3)

  MACRO(closeFileMsg, FreeSymbol, 2)
  MACRO(closedFileMsg, Symbol, 3)
  MACRO(fileErrorMsg, Symbol, 3)
