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
//      Class for symbols for the unary minus operation.
//
#ifndef _minusSymbol_hh_
#define _minusSymbol_hh_
#include "numberOpSymbol.hh"

class MinusSymbol : public NumberOpSymbol
{
public:
  MinusSymbol(int id);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  //
  //	Functions special to MinusSymbol.
  //
  DagNode* makeNegDag(const mpz_class& integer);
  bool isNeg(const DagNode* dagNode) const;
  const mpz_class& getNeg(const DagNode* dagNode, mpz_class& result) const;
  Term* makeIntTerm(const mpz_class& integer);
  bool isNeg(/* const */ Term* term) const;
  const mpz_class& getNeg(/* const */ Term* term, mpz_class& result) const;
  bool getSignedInt64(const DagNode* dagNode, Int64& value) const;
};

#endif
