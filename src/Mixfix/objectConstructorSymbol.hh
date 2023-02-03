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
//      Class for symbols used to construct objects.
//
//	Used to implement empty attribute list syntax and omod -> mod translation.
//
#ifndef _objectConstructorSymbol_hh_
#define _objectConstructorSymbol_hh_
#include "freeSymbol.hh"

class ObjectConstructorSymbol : public FreeSymbol
{
public:
  ObjectConstructorSymbol(int id);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);
  void copyAttachments(Symbol* original, SymbolMap* map);
  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getSymbolAttachments(Vector<const char*>& purposes,
			    Vector<Symbol*>& symbols);
  //
  //	Functions special to ObjectConstructorSymbol.
  //
  ACU_Symbol* getAttributeSetSymbol() const;

private:
  ACU_Symbol* attributeSetSymbol = 0;
};

inline ACU_Symbol*
ObjectConstructorSymbol::getAttributeSetSymbol() const
{
  return attributeSetSymbol;
}

#endif
