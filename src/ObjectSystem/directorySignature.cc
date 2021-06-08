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
//	List of all recognized constructors by the directory subsystem.
//
//	We generate various chunks of linear (branching) code to handle
//	this hetrogeneous collection by macro expansion.
//	The format is:
//		MACRO(symbols name, symbols C++ class, number of args)
//
  MACRO(succSymbol, SuccSymbol, 1)
  MACRO(stringSymbol, StringSymbol, 0)
  MACRO(directoryOidSymbol, FreeSymbol, 1)

  MACRO(fileEntrySymbol, Symbol, 0)
  MACRO(directoryEntrySymbol, Symbol, 0)
  MACRO(socketEntrySymbol, Symbol, 0)
  MACRO(pipeEntrySymbol, Symbol, 0)
  MACRO(charDeviceEntrySymbol, Symbol, 0)
  MACRO(blockDeviceEntrySymbol, Symbol, 0)
  MACRO(symbolicLinkEntrySymbol, Symbol, 1)
  MACRO(endOfDirectorySymbol, Symbol, 0)

  MACRO(openDirectoryMsg, FreeSymbol, 3)
  MACRO(openedDirectoryMsg, Symbol, 3)
  MACRO(closeDirectoryMsg, FreeSymbol, 2)
  MACRO(closedDirectoryMsg, Symbol, 2)
  MACRO(getDirectoryEntryMsg, FreeSymbol, 2)
  MACRO(gotDirectoryEntryMsg, Symbol, 4)

  MACRO(makeDirectoryMsg, Symbol, 3)
  MACRO(madeDirectoryMsg, Symbol, 2)
  MACRO(removeDirectoryMsg, Symbol, 3)
  MACRO(removedDirectoryMsg, Symbol, 2)
  MACRO(directoryErrorMsg, Symbol, 3)
