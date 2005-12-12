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
//      Abstract base class for maps from symbols to symbols.
//
#ifndef _symbolMap_hh_
#define _symbolMap_hh_

class SymbolMap
{
public:
  //
  //	If translate() returns 0, this indicates that the symbol will
  //	be translated by a term an that the entire construction will be
  //	handled by calling makeTranslatedTerm() the orginal term.
  //
  virtual Symbol* translate(Symbol* symbol) = 0;
  virtual Term* translateTerm(const Term* term) = 0;
  //
  //	This functions returns the symbol that has the same name and
  //	and domain/range sort names as that of its argument, but in
  //	the target module (into which translations are being made).
  //
  virtual Symbol* findTargetVersionOfSymbol(Symbol* symbol) = 0;
};

#endif
