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
//	List of all recognized constructors by the socket subsystem.
//
//	We generate various chunks of linear (branching) code to handle
//	this hetrogeneous collection by macro expansion.
//	The format is:
//		MACRO(symbols name, symbols C++ class, number of args)
//
  MACRO(succSymbol, SuccSymbol, 1)
  MACRO(stringSymbol, StringSymbol, 0)
  MACRO(socketOidSymbol, FreeSymbol, 1)

  MACRO(createClientTcpSocketMsg, FreeSymbol, 4)
  MACRO(createServerTcpSocketMsg, FreeSymbol, 4)
  MACRO(createdSocketMsg, Symbol, 3)

  MACRO(acceptClientMsg, FreeSymbol, 2)
  MACRO(acceptedClientMsg, Symbol, 4)
  MACRO(sendMsg, FreeSymbol, 3)
  MACRO(sentMsg, Symbol, 2)
  MACRO(receiveMsg, FreeSymbol, 2)
  MACRO(receivedMsg, Symbol, 3)
  MACRO(closeSocketMsg, FreeSymbol, 2)
  MACRO(closedSocketMsg, Symbol, 3)
  MACRO(socketErrorMsg, Symbol, 3)
