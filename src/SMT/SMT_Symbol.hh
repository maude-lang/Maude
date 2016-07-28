/*

    This file is part of the Maude 2 interpreter.

    Copyright 2014 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for symbols representing SMT operators.
//
#ifndef _SMT_Symbol_hh_
#define _SMT_Symbol_hh_
#include "freeSymbol.hh"
#include "SMT_Base.hh"

class SMT_Symbol : public FreeSymbol, public SMT_Base
{
public:
  enum OPERATORS
    {
      //
      //	Boolean stuff.
      //
      CONST_TRUE,
      CONST_FALSE,
      NOT,
      AND,
      OR,
      XOR,
      IMPLIES,
      //
      //	Polymorphic Boolean stuff.
      //
      EQUALS,
      NOT_EQUALS,
      ITE,
      //
      //	Integer stuff.
      //
      UNARY_MINUS,
      MINUS,
      PLUS,
      MULT,
      DIV,
      MOD,
      //
      //	Integer tests.
      //
      LT,
      LEQ,
      GT,
      GEQ,
      DIVISIBLE,
      //
      //	Stuff that is extra to reals.
      //
      REAL_DIVISION,
      TO_REAL,
      TO_INTEGER,
      IS_INTEGER
    };

  SMT_Symbol(int id, int arity);
  //
  //	Standard functionality for managing hooks.
  //
  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  void copyAttachments(Symbol* original, SymbolMap* map);
  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  //
  //	For SMT_Base.
  //
  void fillOutSMT_Info(SMT_Info& info);
  //
  //	Our main added functionality.
  //
  int getOperator() const;

private:
  static const char* operatorNames[];

  int op;
};

inline int
SMT_Symbol::getOperator() const
{
  return op;
}

#endif
