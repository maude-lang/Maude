/*

    This file is part of the Maude 3 interpreter.

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
//      Implementation for class NumberOpSymbol.
//
#include <iterator>

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "mpzSystem.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "ACU_Persistent.hh"
#include "ACU_Theory.hh"
#include "NA_Theory.hh"
#include "builtIn.hh"

//      interface class definitions
#include "term.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "dagArgumentIterator.hh"
#include "symbolMap.hh"

//      free theory class definitions
#include "freeSymbol.hh"
#include "freeDagNode.hh"

//      ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"

//      built in stuff
#include "bindingMacros.hh"
#include "succSymbol.hh"
#include "minusSymbol.hh"
#include "stringSymbol.hh"
#include "stringDagNode.hh"
#include "matrixOpSymbol.hh"

MatrixOpSymbol::MatrixOpSymbol(int id, int arity)
  : NumberOpSymbol(id, arity)
{
#define MACRO(SymbolName, SymbolClass, RequiredFlags, NrArgs) \
  SymbolName = 0;
#include "matrixOpSignature.cc"
#undef MACRO
}

bool
MatrixOpSymbol::attachData(const Vector<Sort*>& opDeclaration,
			   const char* purpose,
			   const Vector<const char*>& data)
{
  if (strcmp(purpose, "MatrixOpSymbol") == 0)
    return true;
  return NumberOpSymbol::attachData(opDeclaration, purpose, data);
}

bool
MatrixOpSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
#define MACRO(SymbolName, SymbolClass, RequiredFlags, NrArgs) \
  BIND_SYMBOL(purpose, symbol, SymbolName, SymbolClass*);
#include "matrixOpSignature.cc"
#undef MACRO
  return NumberOpSymbol::attachSymbol(purpose, symbol);
}

void
MatrixOpSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  MatrixOpSymbol* orig = safeCast(MatrixOpSymbol*, original);
#define MACRO(SymbolName, SymbolClass, RequiredFlags, NrArgs) \
  COPY_SYMBOL(orig, SymbolName, map, SymbolClass*);
#include "matrixOpSignature.cc"
#undef MACRO
  NumberOpSymbol::copyAttachments(original, map);
}

void
MatrixOpSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				   Vector<const char*>& purposes,
				   Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "MatrixOpSymbol";
  data.resize(nrDataAttachments + 1);
  (data[nrDataAttachments]).resize(1);
  data[nrDataAttachments][0] = "natSystemSolve";
  NumberOpSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
MatrixOpSymbol::getSymbolAttachments(Vector<const char*>& purposes,
				     Vector<Symbol*>& symbols)
{
#define MACRO(SymbolName, SymbolClass, RequiredFlags, NrArgs) \
  APPEND_SYMBOL(purposes, symbols, SymbolName);
#include "matrixOpSignature.cc"
#undef MACRO
  NumberOpSymbol::getSymbolAttachments(purposes, symbols);
}

bool
MatrixOpSymbol::downMatrixEntry(DagNode* dagNode, SparseMatrix& matrix, int& maxRowNr, int& maxColNr)
{
  if (dagNode->symbol() == matrixEntrySymbol)
    {
      FreeDagNode* d = safeCast(FreeDagNode*, dagNode);
      DagNode* arg = d->getArgument(0);
      if (arg->symbol() == indexPairSymbol)
	{
	  FreeDagNode* a = safeCast(FreeDagNode*, arg);
	  int rowNr;
	  int colNr;
	  if (getSuccSymbol()->getSignedInt(a->getArgument(0), rowNr) &&
	      getSuccSymbol()->getSignedInt(a->getArgument(1), colNr) &&
	      getNumber(d->getArgument(1), matrix[rowNr][colNr]))
	    {
	      if (rowNr > maxRowNr)
		maxRowNr = rowNr;
	      if (colNr > maxColNr)
		maxColNr = colNr;
	      return true;
	    }
	}
    }
  return false;
}

bool
MatrixOpSymbol::downMatrix(DagNode* dagNode, SparseMatrix& matrix, int& maxRowNr, int& maxColNr)
{
  Symbol* s = dagNode->symbol();
  if (s == matrixSymbol)
    {
      for (DagArgumentIterator i(dagNode); i.valid(); i.next())
	{
	  if (!downMatrixEntry(i.argument(), matrix, maxRowNr, maxColNr))
	    return false;
	}
    }
  else if (s != emptyMatrixSymbol)
    return downMatrixEntry(dagNode, matrix, maxRowNr, maxColNr);
  return true;
}

bool
MatrixOpSymbol::downVectorEntry(DagNode* dagNode, IntVec& vec, int& maxRowNr)
{
  if (dagNode->symbol() == vectorEntrySymbol)
    {
      FreeDagNode* d = safeCast(FreeDagNode*, dagNode);
      int index;
      if (getSuccSymbol()->getSignedInt(d->getArgument(0), index))
	{
	  if (index > maxRowNr)
	    {
	      vec.resize(index + 1);
	      for (int i = maxRowNr + 1; i < index; ++i)
		vec[i] = 0;
	      maxRowNr = index;
	    }
	  if (getNumber(d->getArgument(1), vec[index]))
	    return true;
	}
    }
  return false;
}

bool
MatrixOpSymbol::downVector(DagNode* dagNode, IntVec& vec, int& maxRowNr)
{
  vec.resize(maxRowNr + 1);
  for (int i = 0; i <= maxRowNr; ++i)
    vec[i] = 0;
  Symbol* s = dagNode->symbol();
  if (s == vectorSymbol)
    {
      for (DagArgumentIterator i(dagNode); i.valid(); i.next())
	{
	  if (!downVectorEntry(i.argument(), vec, maxRowNr))
	    return false;
	}
    }
  else if (s != emptyVectorSymbol)
    return downVectorEntry(dagNode, vec, maxRowNr);
  return true;
}

bool
MatrixOpSymbol::downAlgorithm(DagNode* dagNode, Algorithm& algorithm)
{
 if (dagNode->symbol() == stringSymbol)
   {
     const Rope& alg = safeCast(StringDagNode*, dagNode)->getValue();
     if (alg.empty())
       algorithm = SYSTEMS_CHOICE;
     else
       {
	 char *algStr = alg.makeZeroTerminatedString();
	 if (strcmp(algStr, "cd") == 0)
	   algorithm = CD;
	 else if (strcmp(algStr, "gcd") == 0)
	   algorithm = GCD;
	 else
	   {
	     delete [] algStr;
	     return false;
	   }
	 delete [] algStr;
       }
     return true;
   }
 return false;
}

DagNode*
MatrixOpSymbol::upSet(const Vector<DagNode*>& elts)
{
  int n = elts.size();
  if (n == 0)
    return emptyVectorSetSymbol->makeDagNode();
  return (n == 1) ? elts[0] : vectorSetSymbol->makeDagNode(elts);
}

DagNode*
MatrixOpSymbol::upVector(const IntVec& row)
{
  Vector<DagNode*> elts;
  Vector<DagNode*> pair(2);
  int nrRows = row.size();
  for (int i = 1; i < nrRows; i++)
    {
      const mpz_class& v = row[i];
      Assert(v >= 0, "-ve solution");
      if (v > 0)
	{
	  pair[0] = getSuccSymbol()->makeNatDag(i - 1);
	  pair[1] = getSuccSymbol()->makeNatDag(v);
	  elts.append(vectorEntrySymbol->makeDagNode(pair));
	}
    }
  int n = elts.size();
  if (n == 0)
    return emptyVectorSymbol->makeDagNode();
  return (n == 1) ? elts[0] : vectorSymbol->makeDagNode(elts);
}

bool
MatrixOpSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  FreeDagNode* d = safeCast(FreeDagNode*, subject);
  DagNode* m = d->getArgument(0);
  m->reduce(context);
  DagNode* v = d->getArgument(1);
  v->reduce(context);
  DagNode* a = d->getArgument(2);
  a->reduce(context);

  Algorithm algorithm;
  SparseMatrix matrix;
  IntVec vec;
  int maxRowNr = -1;
  int maxColNr = -1;
  if (downAlgorithm(a, algorithm) &&
      downMatrix(m, matrix, maxRowNr, maxColNr) &&
      maxRowNr >= 0 &&
      downVector(v, vec, maxRowNr))			      
    {
      Vector<DagNode*> homogenous;
      Vector<DagNode*> inhomogenous;
      //
      //	Build Diophantine system.
      //
      MpzSystem ds;
      int rowSize = maxColNr + 2;
      IntVec row(rowSize);
      for (int i = 0; i <= maxRowNr; i++)
	{
	  for (int j = 1; j < rowSize; j++)
	    row[j] = 0;

	  const mpz_class& v = vec[i];
	  const SparseVector& r = matrix[i];
	  //
	  //	If we have an equation with all zero coefficients and nonzero
	  //	constant term we can trivially fail.
	  //
	  if (r.empty() && v != 0)
	    goto fail;

	  row[0] = -v;
	  for (auto& j : r)
	    row[j.first + 1] = j.second;
	  ds.insertEqn(row);
	}
      for (int j = 1; j < rowSize; j++)
	row[j] = NONE;
      row[0] = 1;
      ds.setUpperBounds(row);
      //
      //	Extract solutions.
      //
      if (algorithm == GCD ||
	  (algorithm == SYSTEMS_CHOICE && maxColNr <= maxRowNr + 1))
	{
	  while (ds.findNextMinimalSolutionGcd(row))
	    {
	      if (row[0] == 0)
		homogenous.append(upVector(row));
	      else
		inhomogenous.append(upVector(row));
	    }
	}
      else
	{
	  while (ds.findNextMinimalSolution(row))
	    {
	      if (row[0] == 0)
		homogenous.append(upVector(row));
	      else
		inhomogenous.append(upVector(row));
	    }
	}
      //
      //	Build result dag.
      //
    fail:
      Vector<DagNode*> args(2);
      args[0] = upSet(inhomogenous);
      args[1] = inhomogenous.empty() ? args[0] : upSet(homogenous);
      return context.builtInReplace(subject, vectorSetPairSymbol->makeDagNode(args)); 
    }
  //
  //	NumberOpSymbol doesn't know how to deal with this.
  //
  return FreeSymbol::eqRewrite(subject, context);
}
