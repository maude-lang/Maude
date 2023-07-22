/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for symbols for successor operation.
//
#ifndef _succSymbol_hh_
#define _succSymbol_hh_
#include <gmpxx.h>
#include "S_Symbol.hh"
#include "cachedDag.hh"

class SuccSymbol : public S_Symbol
{
public:
  SuccSymbol(int id);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachTerm(const char* purpose, Term* term);
  void copyAttachments(Symbol* original, SymbolMap* map);
  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getTermAttachments(Vector<const char*>& purposes,
			  Vector<Term*>& terms);
  void postInterSymbolPass();
  void reset();

#if SIZEOF_LONG < 8
  DagNode* makeNatDag(Int64 nat)
  {
    mpz_class bigNat;
    mpz_import(bigNat.get_mpz_t(), 1, 1, sizeof(nat), 0, 0, &nat);
    return makeNatDag(bigNat);
  }
#endif
  //
  //	Functions special to SuccSymbol.
  //
  Term* makeNatTerm(const mpz_class& nat);
  DagNode* makeNatDag(const mpz_class& nat);
  DagNode* makeNatDag64(uint_fast64_t nat);
  bool isNat(const Term* term) const;
  bool isNat(const DagNode* dagNode) const;
  const mpz_class& getNat(const Term* term) const;
  const mpz_class& getNat(const DagNode* dagNode) const;
  bool getSignedInt(const DagNode* dagNode, int& value) const;
  bool getSignedInt64(const DagNode* dagNode, Int64& value) const;
  bool rewriteToNat(DagNode* subject, RewritingContext& context, const mpz_class& result);
  //
  //	In case the number won't fill in an Int64 but a scaled version will
  //	so we do the scaling before converting. Scaling always rounds down.
  //
  bool getScaledSignedInt64(const DagNode* dagNode,
			    const mpz_class& scaleFactor,
			    Int64& value) const;

private:
  CachedDag zeroTerm;
};

#endif
