/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//	List of all recognized constructors by the process subsystem.
//
//	We generate various chunks of linear (branching) code to handle
//	this hetrogeneous collection by macro expansion.
//	The format is:
//		MACRO(symbols name, symbols C++ class, number of args)
//
  MACRO(socketManagerSymbol, SocketManagerSymbol, 0)
  MACRO(succSymbol, SuccSymbol, 1)
  MACRO(stringSymbol, StringSymbol, 0)
  MACRO(nilStringListSymbol, Symbol, 0)
  MACRO(stringListSymbol, AU_Symbol, 2)
  MACRO(emptyProcessOptionSetSymbol, Symbol, 0)
  MACRO(normalExitSymbol, Symbol, 1)
  MACRO(terminatedBySignalSymbol, Symbol, 1)

  MACRO(processOidSymbol, FreeSymbol, 1)

  MACRO(createProcessMsg, FreeSymbol, 5)
  MACRO(createdProcessMsg, Symbol, 5)
  MACRO(signalProcessMsg, FreeSymbol, 3)
  MACRO(signaledProcessMsg, Symbol, 3)
  MACRO(waitForExitMsg, FreeSymbol, 2)
  MACRO(exitedMsg, Symbol, 3)
  MACRO(processErrorMsg, Symbol, 3)
