/*

    This file is part of the Maude 2 interpreter.

    Copyright 2004 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for symbols for built in operations on matrices of numbers.
//
#ifndef _matrixOpSymbol_hh_
#define _matrixOpSymbol_hh_
#include <map>
#include "numberOpSymbol.hh"

class MatrixOpSymbol : public NumberOpSymbol
{
public:
  MatrixOpSymbol(int id, int arity);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);

  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getSymbolAttachments(Vector<const char*>& purposes,
			    Vector<Symbol*>& symbols);

  bool eqRewrite(DagNode* subject, RewritingContext& context);

private:
  enum Algorithm
    {
      SYSTEMS_CHOICE,
      CD,
      GCD
    };

  typedef map<int, mpz_class> SparseVector;
  typedef map<int, SparseVector> SparseMatrix;
  typedef Vector<mpz_class> IntVec;

  bool downMatrixEntry(DagNode* dagNode, SparseMatrix& matrix, int& maxRowNr, int& maxColNr);
  bool downMatrix(DagNode* dagNode, SparseMatrix& matrix, int& maxRowNr, int& maxColNr);
  bool downVectorEntry(DagNode* dagNode, IntVec& vec, int& maxRowNr);
  bool downVector(DagNode* dagNode, IntVec& vec, int& maxRowNr);
  bool downAlgorithm(DagNode* dagNode, Algorithm& algorithm);
  DagNode* upSet(const Vector<DagNode*>& elts);
  DagNode* upVector(const IntVec& row);

#define MACRO(SymbolName, SymbolClass, RequiredFlags, NrArgs) \
  SymbolClass* SymbolName;
#include "matrixOpSignature.cc"
#undef MACRO
};

#endif
