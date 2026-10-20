/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2026 SRI International, Menlo Park, CA 94025, USA.

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
//	Auxiliary functions needed by deserializer.
//
#include "stringSymbol.hh"
#include "stringDagNode.hh"
#include "variableSymbol.hh"
#include "variableDagNode.hh"
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierDagNode.hh"
#include "floatSymbol.hh"
#include "floatDagNode.hh"
#include "SMT_NumberSymbol.hh"
#include "SMT_NumberDagNode.hh"

static void
getInputFromRope(char* buf, yy_size_t& result, yy_size_t max_size)
{
  Rope::size_type available = inputRope.end() - current;
  if (available == 0)
    {
      endOfRope = true;
      result = 1;
      buf[0] = '\004';  // EOT character
    }
  else
    {
      result = (available > max_size) ? max_size : available;
      for (Rope::size_type i = 0; i != result; ++i, ++current)
        buf[i] = *current;
    }
}

//
//	We do some checks for corruption, but it's possible that a corrupt
//	serialization could be a serialization of a different dag.
//	We're mostly concerned about not crashing the receiver.
//

static DagNode*
makeDagNode(MixfixModule* m)
{
  Symbol* symbol;
  if (opType == EAGER_SORT_TEST || opType == LAZY_SORT_TEST)
    {

      int nrSorts = m->getSorts().size();
      if (savedIndex >= nrSorts)
	return nullptr;  // corrupt - not a valid sort index
      Sort* sort = m->getSorts()[savedIndex];
      symbol = m->instantiateSortTest(sort, opType == EAGER_SORT_TEST);
    }
  else
    {
      if (opType == POLYMORPH)
	{
	  if (savedIndex >= m->getNrPolymorphs())
	    return nullptr;  // corrupt - not a valid polymorph index
	  int nrKinds = m->getConnectedComponents().size();
	  if (savedKindIndex >= nrKinds)
	    return nullptr;  // corrupt - not a valid kind index
	  symbol = m->instantiatePolymorph(savedIndex, savedKindIndex);
	}
      else
	{
	  int nrSymbols = m->getSymbols().size();
	  if (savedIndex >= nrSymbols)
	    return nullptr;  // corrupt - not a valid symbol index
	  symbol = m->getSymbols()[savedIndex];
	}
      SymbolType type = m->getSymbolType(symbol);
      if (type.hasFlag(SymbolType::ITER))
	{
	  S_Symbol* s = safeCastNonNull<S_Symbol*>(symbol);
	  return new S_DagNode(s, savedBigNum, argList[0]);
	}
    }
  return symbol->makeDagNode(argList);
}

static DagNode*
makeVariableDagNode(MixfixModule* m, int code)
{
  int nrSorts = m->getSorts().size();
  if (savedIndex >= nrSorts)
    return nullptr;  // corrupt - not a valid sort index
  Sort* sort = m->getSorts()[savedIndex];
  VariableSymbol* symbol = safeCastNonNull<VariableSymbol*>(m->instantiateVariable(sort));
  return new VariableDagNode(symbol, code, NONE);
}

static DagNode*
makeQuotedIdentifierDagNode(MixfixModule* m, int code)
{
  int nrSymbols = m->getSymbols().size();
  if (savedIndex >= nrSymbols)
    return nullptr;  // corrupt - not a valid symbol index
  Symbol* symbol = m->getSymbols()[savedIndex];
  QuotedIdentifierSymbol* s = dynamic_cast<QuotedIdentifierSymbol*>(symbol);
  if (s == nullptr)
    return nullptr;  // corrupt - not a valid qid symbol
  return new QuotedIdentifierDagNode(s, code);
}

static DagNode*
makeStringDagNode(MixfixModule* m, const char* quotedString)
{
  int nrSymbols = m->getSymbols().size();
  if (savedIndex >= nrSymbols)
    return nullptr;  // corrupt - not a valid symbol index
  Symbol* symbol = m->getSymbols()[savedIndex];
  StringSymbol* s = dynamic_cast<StringSymbol*>(symbol);
  if (s == nullptr)
    return nullptr;  // corrupt - not a valid string symbol
  return new StringDagNode(s, Token::stringToRope(quotedString));
}

static DagNode*
makeFloatDagNode(MixfixModule* m, const char* bitsAsInt64)
{
  int nrSymbols = m->getSymbols().size();
  if (savedIndex >= nrSymbols)
    return nullptr;  // corrupt - not a valid symbol index
  Symbol* symbol = m->getSymbols()[savedIndex];
  FloatSymbol* s = dynamic_cast<FloatSymbol*>(symbol);
  if (s == nullptr)
    return nullptr;  // corrupt - not a valid string symbol

  bool error;
  Int64 dBits = stringToInt64(bitsAsInt64, error);
  if (error)
    return nullptr;  // corrupt - not a valid 64 bit int
  //
  //	Currently we don't validate the bits in a float.
  //
  double d;
  memcpy(&d, &dBits, sizeof(d));
  return new FloatDagNode(s, d);
}

static DagNode*
makeSMT_NumberDagNode(MixfixModule* m, const char* stringRep)
{
  int nrSymbols = m->getSymbols().size();
  if (savedIndex >= nrSymbols)
    return nullptr;  // corrupt - not a valid symbol index
  Symbol* symbol = m->getSymbols()[savedIndex];
  SMT_NumberSymbol* s = dynamic_cast<SMT_NumberSymbol*>(symbol);
  if (s == nullptr)
    return nullptr;  // corrupt - not a valid string symbol
  //
  //	Currently we don't validate the string as an SMT number.
  //
  mpq_class value(stringRep);
  return new SMT_NumberDagNode(s, value);
}
