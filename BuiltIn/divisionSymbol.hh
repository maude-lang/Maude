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
//      Class for symbols for exact division operation on numbers.
//
#ifndef _divisionSymbol_hh_
#define _divisionSymbol_hh_
#include "freeSymbol.hh"
#include "cachedDag.hh"

class DivisionSymbol : public FreeSymbol
{
public:
  DivisionSymbol(int id);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);
  void copyAttachments(Symbol* original, SymbolMap* map);

  bool eqRewrite(DagNode* subject, RewritingContext& context);
  //
  //	Functions special to DivisionSymbol.
  //
  DagNode* makeRatDag(const mpz_class& nr, const mpz_class& dr);
  bool isRat(const DagNode* dagNode) const;
  const mpz_class& getRat(const DagNode* dagNode, mpz_class& numerator) const;
  Term* makeRatTerm(const mpz_class& nr, const mpz_class& dr);
  bool isRat(/* const */ Term* term) const;
  const mpz_class& getRat(/* const */ Term* term, mpz_class& numerator) const;
 
  SuccSymbol* succSymbol;
  MinusSymbol* minusSymbol;
};

#endif
