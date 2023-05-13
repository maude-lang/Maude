/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
//	List of all recognized constructors by the prng subsystem.
//
//	We generate various chunks of linear (branching) code to handle
//	this hetrogeneous collection by macro expansion.
//	The format is:
//		MACRO(symbols name, symbols C++ class, number of args)
//
  MACRO(succSymbol, SuccSymbol, 1)
  MACRO(stringSymbol, StringSymbol, 0)
  MACRO(prngOidSymbol, FreeSymbol, 1)

  MACRO(createPrngMsg, FreeSymbol, 3)
  MACRO(createdPrngMsg, Symbol, 3)
  MACRO(setSeedMsg, FreeSymbol, 3)
  MACRO(seedSetMsg, Symbol, 2)
  MACRO(getNextMsg, FreeSymbol, 2)
  MACRO(gotNextMsg, Symbol, 3)
  MACRO(deletePrngMsg, FreeSymbol, 2)
  MACRO(deletedPrngMsg, Symbol, 2)

  MACRO(prngErrorMsg, Symbol, 3)
