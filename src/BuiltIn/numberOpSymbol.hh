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
//      Class for symbols for built in operations on numbers.
//
#ifndef _numberOpSymbol_hh_
#define _numberOpSymbol_hh_
#include "freeSymbol.hh"
#include "cachedDag.hh"

class NumberOpSymbol : public FreeSymbol
{
public:
  NumberOpSymbol(int id, int arity);

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

  void postInterSymbolPass();
  void reset();
  bool eqRewrite(DagNode* subject, RewritingContext& context);
  //
  //	Functions special to NumberOpSymbol.
  //
  DagNode* makeNegDag(const mpz_class& integer);
  bool isNeg(const DagNode* dagNode) const;
  const mpz_class& getNeg(const DagNode* dagNode, mpz_class& result) const;

protected:
  SuccSymbol* getSuccSymbol() const;

private:
  enum ImplementationConstants
  {
    EXPONENT_BOUND = 1000000	// max allowed exponent to limit runaway memory use
  };

  int op;
  SuccSymbol* succSymbol;
  MinusSymbol* minusSymbol;
  CachedDag trueTerm;
  CachedDag falseTerm;
};

inline SuccSymbol*
NumberOpSymbol::getSuccSymbol() const
{
  return succSymbol;
}

#endif
