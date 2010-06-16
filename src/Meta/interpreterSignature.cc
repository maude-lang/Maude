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
//	List of all recognized constructors by the meta-interpreter subsystem.
//
//	We generate various chunks of linear (branching) code to handle
//	this hetrogeneous collection by macro expansion.
//	The format is:
//		MACRO(symbols name, symbols C++ class, number of args)
//
  MACRO(succSymbol, SuccSymbol, 1)
  MACRO(interpreterOidSymbol, FreeSymbol, 1)
  MACRO(createInterpreterMsg, FreeSymbol, 2)
  MACRO(createdInterpreterMsg, FreeSymbol, 3)
  MACRO(insertModuleMsg, FreeSymbol, 3)
  MACRO(insertedModuleMsg, FreeSymbol, 2)
  MACRO(showModuleMsg, FreeSymbol, 4)
  MACRO(showingModuleMsg, FreeSymbol, 3)
  MACRO(reduceTermMsg, FreeSymbol, 4)
  MACRO(reducedTermMsg, FreeSymbol, 5)
  MACRO(rewriteTermMsg, FreeSymbol, 5)
  MACRO(rewroteTermMsg, FreeSymbol, 5)
  MACRO(frewriteTermMsg, FreeSymbol, 6)
  MACRO(frewroteTermMsg, FreeSymbol, 5)
  MACRO(continueMsg, FreeSymbol, 3)
  MACRO(quitMsg, FreeSymbol, 2)
  MACRO(byeMsg, FreeSymbol, 2)
