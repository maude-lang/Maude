/*

    This file is part of the Maude 3 interpreter.

    Copyright 2026 SRI International, Menlo Park, CA 94025, USA.

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
//	List of all recognized transform result constructors.
//
//	We generate various chunks of linear (branching) code to handle
//	this hetrogeneous collection by macro expansion.
//	The format is:
//		MACRO(symbols name, symbols C++ class)
//
  MACRO(nilModuleListSymbol, Symbol)
  MACRO(moduleListSymbol, AU_Symbol)
  MACRO(nilTransformMessagesSymbol, Symbol)
  MACRO(transformMessagesSymbol, AU_Symbol)
  MACRO(advisorySymbol, Symbol)
  MACRO(warningSymbol, Symbol)
  MACRO(verboseSymbol, Symbol)
  MACRO(shareWith, MetaLevelOpSymbol)
