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
//      Class for symbols for built in operations on strings.
//
#ifndef _stringOpSymbol_hh_
#define _stringOpSymbol_hh_
#include "rope.hh"
#include "freeSymbol.hh"
#include "cachedDag.hh"

class StringOpSymbol : public FreeSymbol
{
public:
  StringOpSymbol(int id, int arity);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);
  bool attachTerm(const char* purpose, Term* term);
  void copyAttachments(Symbol* original, SymbolMap* map);
  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getSymbolAttachments(Vector<const char*>& purposes,
			    Vector<Symbol*>& symbols);
  void getTermAttachments(Vector<const char*>& purposes,
			  Vector<Term*>& terms);

  bool eqRewrite(DagNode* subject, RewritingContext& context);
  void postInterSymbolPass();
  void reset();

private:
  enum
  {
    //
    //	Tha actual maximum number of significant digits in the exact
    //	decimal represention of a IEEE-754 double is hard to calculate
    //	but must be < 1074 since there can be at most 1074 binary places
    //	and with 1074 binary places the first multiplications will produce
    //	leading zeros.
    //
    MAX_FLOAT_DIGITS = 1074
  };
  bool rewriteToString(DagNode* subject, RewritingContext& context, const Rope& result);
  static bool ropeToNumber(const Rope& subject,
			   int base,
			   mpz_class& numerator,
			   mpz_class& denominator);
  static Rope substring(const Rope& subject, Rope::size_type index, Rope::size_type length);
  static int fwdFind(const Rope& subject, const Rope& pattern, Rope::size_type start);
  static int revFind(const Rope& subject, const Rope& pattern, Rope::size_type start);
  static Rope upperCase(const Rope& subject);
  static Rope lowerCase(const Rope& subject);

  int op;
  StringSymbol* stringSymbol;
  SuccSymbol* succSymbol;
  MinusSymbol* minusSymbol;
  DivisionSymbol* divisionSymbol;
  FloatSymbol* floatSymbol;
  Symbol* decFloatSymbol;
  CachedDag trueTerm;
  CachedDag falseTerm;
  CachedDag notFoundTerm;
};

#endif
