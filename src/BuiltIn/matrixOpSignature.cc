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
//	List of constructors specific to MatrixOpSymbol.
//
//	We generate various chunks of linear (branching) code to handle
//	this hetrogeneous collection by macro expansion.
//	The format is:
//		MACRO(symbols name, symbols C++ class, required type flags, number of args)
//
  MACRO(stringSymbol, StringSymbol, SymbolType::STRING, 0)

  MACRO(emptyVectorSymbol, Symbol, 0, 0)
  MACRO(vectorEntrySymbol, FreeSymbol, 0, 2)
  MACRO(vectorSymbol, ACU_Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)

  MACRO(emptyMatrixSymbol, Symbol, 0, 0)
  MACRO(matrixEntrySymbol, FreeSymbol, 0, 2)
  MACRO(matrixSymbol, ACU_Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
  MACRO(indexPairSymbol, FreeSymbol, 0, 2)

  MACRO(emptyVectorSetSymbol, Symbol, 0, 0)
  MACRO(vectorSetSymbol, ACU_Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
  MACRO(vectorSetPairSymbol, FreeSymbol, 0, 2)
