/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2014 SRI International, Menlo Park, CA 94025, USA.

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
//	Routines for pretty printing a term into a vector of token codes.
//

void
MixfixModule::bufferPrint(Vector<int>& buffer, Term* term, int printFlags)
{
  globalIndent = 0;  // HACK
  prettyPrint(buffer, term, UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false, printFlags);
}

void
MixfixModule::prefix(Vector<int>& buffer, bool needDisambig)
{
  if (needDisambig)
    buffer.append(leftParen);
}

void
MixfixModule::suffix(Vector<int>& buffer, Term* term, bool needDisambig, int printFlags)
{
  if (needDisambig)
    {
      Symbol* symbol = term->symbol();
      int sortIndex = term->getSortIndex();
      if (sortIndex <= Sort::KIND)
	sortIndex = chooseDisambiguator(symbol);
      buffer.append(rightParen);
      //
      //	sortIndex will never be the index of a kind.
      //
      printDotSort(buffer, symbol->rangeComponent()->sort(sortIndex), printFlags);
    }
}

void
MixfixModule::handleVariable(Vector<int>& buffer, Term* term, int printFlags)
{
  VariableTerm* v = safeCast(VariableTerm*, term);
  string fullName(Token::name(v->id()));
  fullName += ':';
  Sort* sort = v->getSort();
  if (sort->index() == Sort::KIND)
    {
      buffer.append(Token::encode(fullName.c_str()));
      printKind(buffer, sort, printFlags);
    }
  else
    printVarSort(buffer, fullName, sort, printFlags);
}

bool
MixfixModule::handleIter(Vector<int>& buffer, Term* term, SymbolInfo& si, bool rangeKnown, int printFlags)
{
  if (!(si.symbolType.hasFlag(SymbolType::ITER)))
    return false;
  if (si.symbolType.getBasicType() == SymbolType::SUCC_SYMBOL &&
      (printFlags & Interpreter::PRINT_NUMBER))
    {
      SuccSymbol* succSymbol = safeCast(SuccSymbol*, term->symbol());
      if (succSymbol->isNat(term))
	{
	  const mpz_class& nat = succSymbol->getNat(term);
	  bool needDisambig = !rangeKnown && (kindsWithSucc.size() > 1 || overloadedIntegers.count(nat));
	  prefix(buffer, needDisambig);
	  char* name = mpz_get_str(0, 10, nat.get_mpz_t());
	  buffer.append(Token::encode(name));
	  free(name);
	  suffix(buffer, term, needDisambig, printFlags);
	  return true;
	}
    }
  S_Term* st = safeCast(S_Term*, term);
  const mpz_class& number = st->getNumber();
  if (number == 1)
    return false;  // do default thing

  // NEED TO FIX: disambig; i.e. we might have a regular operator called f^2
  string prefixName;
  makeIterName(prefixName, term->symbol()->id(), number);
  printPrefixName(buffer, Token::encode(prefixName.c_str()), si, printFlags);
  buffer.append(leftParen);
  prettyPrint(buffer, st->getArgument(), PREFIX_GATHER, UNBOUNDED, 0, UNBOUNDED, 0, rangeKnown, printFlags);
  buffer.append(rightParen);
  return true;
}

bool
MixfixModule::handleMinus(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags)
{
  if (printFlags & Interpreter::PRINT_NUMBER)
    {
      const MinusSymbol* minusSymbol = safeCast(MinusSymbol*, term->symbol());
      if (minusSymbol->isNeg(term))
	{
	  mpz_class neg;
	  (void) minusSymbol->getNeg(term, neg);
	  bool needDisambig = !rangeKnown && (kindsWithMinus.size() > 1 || overloadedIntegers.count(neg));
	  prefix(buffer, needDisambig);

	  char* name = mpz_get_str(0, 10, neg.get_mpz_t());
	  buffer.append(Token::encode(name));
	  free(name);

	  suffix(buffer, term, needDisambig, printFlags);
	  return true;
	}
    }
  return false;
}

bool
MixfixModule::handleDivision(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags)
{
  if (printFlags & Interpreter::PRINT_RAT)
    {
      const DivisionSymbol* divisionSymbol = safeCast(DivisionSymbol*, term->symbol());
      if (divisionSymbol->isRat(term))
	{
	  pair<mpz_class, mpz_class> rat;
	  rat.second = divisionSymbol->getRat(term, rat.first);
	  bool needDisambig = !rangeKnown && (kindsWithDivision.size() > 1 || overloadedRationals.count(rat));
	  prefix(buffer, needDisambig);

	  char* nn = mpz_get_str(0, 10, rat.first.get_mpz_t());
	  string prefixName(nn);
	  free(nn);
	  prefixName += '/';
	  char* dn = mpz_get_str(0, 10, rat.second.get_mpz_t());
	  prefixName += dn;
	  free(dn);
	  buffer.append(Token::encode(prefixName.c_str()));

	  suffix(buffer, term, needDisambig, printFlags);
	  return true;
	}
    }
  return false;
}

void
MixfixModule::handleFloat(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags)
{
  double mfValue = safeCast(FloatTerm*, term)->getValue();
  bool needDisambig = !rangeKnown && (floatSymbols.size() > 1 || overloadedFloats.count(mfValue));
  prefix(buffer, needDisambig);
  buffer.append(Token::doubleToCode(mfValue));
  suffix(buffer, term, needDisambig, printFlags);
}

void
MixfixModule::handleString(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags)
{
  string strValue;
  Token::ropeToString(safeCast(StringTerm*, term)->getValue(), strValue);
  bool needDisambig = !rangeKnown && (stringSymbols.size() > 1 || overloadedStrings.count(strValue));
  prefix(buffer, needDisambig);
  buffer.append(Token::encode(strValue.c_str()));
  suffix(buffer, term, needDisambig, printFlags);
}

void
MixfixModule::handleQuotedIdentifier(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags)
{
  int qidCode = safeCast(QuotedIdentifierTerm*, term)->getIdIndex();
  bool needDisambig = !rangeKnown && (quotedIdentifierSymbols.size() > 1 || overloadedQuotedIdentifiers.count(qidCode));
  prefix(buffer, needDisambig);
  buffer.append(Token::quoteNameCode(qidCode));
  suffix(buffer, term, needDisambig, printFlags);
}

void
MixfixModule::handleSMT_NumberSymbol(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags)
{
  //
  //	Get value.
  //
  SMT_NumberTerm* n = safeCast(SMT_NumberTerm*, term);
  const mpq_class& value = n->getValue();
  //
  //	Look up the index of our sort.
  //
  Symbol* symbol = term->symbol();
  Sort* sort = symbol->getRangeSort();
  //
  //	Determine whether we need disambiguation.
  //
  bool needDisambig;
  SMT_Info::SMT_Type t = getSMT_Info().getType(sort);
  Assert(t != SMT_Info::NOT_SMT, "bad SMT sort " << sort);
  if (t == SMT_Info::INTEGER)
   {
      const mpz_class& integer = value.get_num();
      needDisambig = !rangeKnown && (kindsWithSucc.size() > 1 || overloadedIntegers.count(integer));
    }
  else
    {
      Assert(t == SMT_Info::REAL, "SMT number sort expected");
      pair<mpz_class, mpz_class> rat(value.get_num(), value.get_den());
      needDisambig = !rangeKnown && (kindsWithDivision.size() > 1 || overloadedRationals.count(rat));
    }

  prefix(buffer, needDisambig);
  buffer.append(getSMT_NumberToken(value, sort));
  suffix(buffer, term, needDisambig, printFlags);
}

void
MixfixModule::prettyPrint(Vector<int>& buffer,
			  Term* term,
			  int requiredPrec,
			  int leftCapture,
			  const ConnectedComponent* leftCaptureComponent,
			  int rightCapture,
			  const ConnectedComponent* rightCaptureComponent,
			  bool rangeKnown,
			  int printFlags)
{
  Symbol* symbol = term->symbol();
  int index = symbol->getIndexWithinModule();
  SymbolInfo& si = symbolInfo[index];
  //
  //	Check for special i/o representation.
  //
  if (handleIter(buffer, term, si, rangeKnown, printFlags))
    return;
  int basicType = si.symbolType.getBasicType();
  switch (basicType)
    {
    case SymbolType::MINUS_SYMBOL:
      {
	if (handleMinus(buffer, term, rangeKnown, printFlags))
	  return;
	break;
      }
    case SymbolType::DIVISION_SYMBOL:
      {
	if (handleDivision(buffer, term, rangeKnown, printFlags))
	  return;
	break;
      }
    case SymbolType::FLOAT:
      {
	handleFloat(buffer, term, rangeKnown, printFlags);
	return;
      }
    case SymbolType::STRING:
      {
	handleString(buffer, term, rangeKnown, printFlags);
	return;
      }
    case SymbolType::QUOTED_IDENTIFIER:
      {
	handleQuotedIdentifier(buffer, term, rangeKnown, printFlags);
	return;
      }
    case SymbolType::VARIABLE:
      {
	handleVariable(buffer, term, printFlags);
	return;
      }
    case SymbolType::SMT_NUMBER_SYMBOL:
      {
	handleSMT_NumberSymbol(buffer, term, rangeKnown, printFlags);
	return;
      }
    default:
      break;
    }

  int iflags = si.iflags;
  bool needDisambig = !rangeKnown && ambiguous(iflags);
  bool argRangeKnown = rangeOfArgumentsKnown(iflags, rangeKnown, needDisambig);
  int nrArgs = symbol->arity();

  prefix(buffer, needDisambig);
  if (((printFlags & Interpreter::PRINT_MIXFIX) && si.mixfixSyntax.length() != 0) ||
	   (basicType == SymbolType::SORT_TEST))
    {
      //
      //	Mixfix case.
      //
      bool printWithParens = printFlags & Interpreter::PRINT_WITH_PARENS;
      bool needParen = !needDisambig &&
	(printWithParens || requiredPrec < si.prec ||
	 ((iflags & LEFT_BARE) && leftCapture <= si.gather[0] &&
	  leftCaptureComponent == symbol->domainComponent(0)) ||
	 ((iflags & RIGHT_BARE) && rightCapture <= si.gather[nrArgs - 1] &&
	  rightCaptureComponent == symbol->domainComponent(nrArgs - 1)));
      bool needAssocParen = si.symbolType.hasFlag(SymbolType::ASSOC) &&
	(printWithParens || si.gather[1] < si.prec ||
	 ((iflags & LEFT_BARE) && (iflags & RIGHT_BARE) &&
	  si.prec <= si.gather[0]));
      if (needParen)
	buffer.append(leftParen);
      int nrTails = 1;
      int pos = 0;
      ArgumentIterator a(*term);
      int moreArgs = a.valid();
      for (int arg = 0; moreArgs; arg++)
	{
	  Term* t = a.argument();
	  a.next();
	  moreArgs = a.valid();
	  pos = printTokens(buffer, si, pos, printFlags);
	  if (arg == nrArgs - 1 && moreArgs)
	    {
	      ++nrTails;
	      arg = 0;
	      if (needAssocParen)
		buffer.append(leftParen);
	      pos = printTokens(buffer, si, 0, printFlags);
	    }
	  int lc = UNBOUNDED;
	  const ConnectedComponent* lcc = 0;
	  int rc = UNBOUNDED;
	  const ConnectedComponent* rcc = 0;
	  if (arg == 0 && (iflags & LEFT_BARE))
	    {
	      rc = si.prec;
	      rcc = symbol->domainComponent(0);
	      if (!needParen && !needDisambig)
		{
		  lc = leftCapture;
		  lcc = leftCaptureComponent;
		}
	    }
	  else if (!moreArgs && (iflags & RIGHT_BARE))
	    {
	      lc = si.prec;
	      lcc = symbol->domainComponent(nrArgs - 1);
	      if (!needParen && !needDisambig)
		{
		  rc = rightCapture;
		  rcc = rightCaptureComponent;
		}
	    }
	  prettyPrint(buffer, t, si.gather[arg], lc, lcc, rc, rcc, argRangeKnown, printFlags);
	}
      printTails(buffer, si, pos, nrTails, needAssocParen, printFlags);
      if (needParen)
	buffer.append(rightParen);
    }
  else
    {
      //
      //	Prefix case.
      //
      int id = symbol->id();
      printPrefixName(buffer, id, si, printFlags);
      ArgumentIterator a(*term);
      if (a.valid())
	{
	  int nrTails = 1;
	  buffer.append(leftParen);
	  for (int arg = 0;; arg++)
	    {
	      Term* t = a.argument();
	      a.next();
	      int moreArgs = a.valid();
	      if (arg >= nrArgs - 1 &&
		  !(printFlags & Interpreter::PRINT_FLAT) &&
		  moreArgs)
		{
		  ++nrTails;
		  printPrefixName(buffer, id, si, printFlags);
		  buffer.append(leftParen);
		}
	      prettyPrint(buffer, t, PREFIX_GATHER, UNBOUNDED, 0, UNBOUNDED, 0, argRangeKnown, printFlags);
	      if (!moreArgs)
		break;
	      buffer.append(comma);
	    }
	  while (nrTails-- > 0)
	    buffer.append(rightParen);
	}
    }
  suffix(buffer, term, needDisambig, printFlags);
}

void
MixfixModule::printKind(Vector<int>& buffer, const Sort* kind, int printFlags)
{
  Assert(kind != 0, "null kind");
  ConnectedComponent* c = kind->component();
  Assert(c != 0, "null conponent");

  buffer.append(leftBracket);
  printSort(buffer, c->sort(1), printFlags);
  int nrMax = c->nrMaximalSorts();
  for (int i = 2; i <= nrMax; i++)
    {
      buffer.append(comma);
      printSort(buffer, c->sort(i), printFlags);
    }
  buffer.append(rightBracket);
}

void
MixfixModule::printSort(Vector<int>& buffer, const Sort* sort, int printFlags)
{
  int name = sort->id();
  if (Token::auxProperty(name) == Token::AUX_STRUCTURED_SORT &&
      interpreter.getPrintFlag(Interpreter::PRINT_MIXFIX))
    {
      Vector<int> parts;
      Token::splitParameterizedSort(name, parts);
      FOR_EACH_CONST(i, Vector<int>, parts)
	buffer.append(*i);
    }
  else
    buffer.append(name);
}

void
MixfixModule::printDotSort(Vector<int>& buffer, const Sort* sort, int printFlags)
{
  int name = sort->id();
  if (Token::auxProperty(name) == Token::AUX_STRUCTURED_SORT &&
      interpreter.getPrintFlag(Interpreter::PRINT_MIXFIX))
    {
      Vector<int> parts;
      Token::splitParameterizedSort(name, parts);
      parts[0] = Token::dotNameCode(parts[0]);
      FOR_EACH_CONST(i, Vector<int>, parts)
	buffer.append(*i);
    }
  else
    buffer.append(Token::dotNameCode(name));
}

void
MixfixModule::printVarSort(Vector<int>& buffer, string& fullName, const Sort* sort, int printFlags)
{
  int name = sort->id();
  if (Token::auxProperty(name) == Token::AUX_STRUCTURED_SORT &&
      interpreter.getPrintFlag(Interpreter::PRINT_MIXFIX))
    {
      Vector<int> parts;
      Token::splitParameterizedSort(name, parts);
      fullName += Token::name(parts[0]);
      parts[0] = Token::encode(fullName.c_str());
      FOR_EACH_CONST(i, Vector<int>, parts)
	buffer.append(*i);
    }
  else
    {
      fullName += Token::name(name);
      buffer.append(Token::encode(fullName.c_str()));
    }
}

int
MixfixModule::printTokens(Vector<int>& buffer, const SymbolInfo& si, int pos, int printFlags)
{
  bool hasFormat = (printFlags & Interpreter::PRINT_FORMAT) && (si.format.length() > 0);
  for (;;)
    {
      int token = si.mixfixSyntax[pos++];
      if (token == underscore)
	break;
      if (hasFormat)
	handleFormat(buffer, si.format[pos - 1]);
      buffer.append(token);
    }
  if (hasFormat)
    handleFormat(buffer, si.format[pos - 1]);
  return pos;
}

void
MixfixModule::printTails(Vector<int>& buffer,
			 const SymbolInfo& si,
			 int pos,
			 int nrTails,
			 bool needAssocParen,
			 int printFlags)
{
  bool hasFormat = (printFlags & Interpreter::PRINT_FORMAT) && (si.format.length() > 0);
  int mixfixLength = si.mixfixSyntax.length();
  for (int i = 0;;)
    {
      for (int j = pos; j < mixfixLength; j++)
	{
	  if (hasFormat)
	    handleFormat(buffer, si.format[j]);
	  buffer.append(si.mixfixSyntax[j]);
	}
      if (hasFormat)
	handleFormat(buffer, si.format[mixfixLength]);
      if (++i == nrTails)
	break;
      if (needAssocParen)
	buffer.append(rightParen);
    }
}

void
MixfixModule::printPrefixName(Vector<int>& buffer, int prefixName, SymbolInfo& si, int printFlags)
{
  if ((printFlags & Interpreter::PRINT_FORMAT) && (si.format.length() == 2))
    {
      handleFormat(buffer, si.format[0]);
      buffer.append(prefixName);
      handleFormat(buffer, si.format[1]);
    }
  else
    buffer.append(prefixName);
}

void
MixfixModule::handleFormat(Vector<int>& buffer, int spaceToken)
{
  for (const char* cmd = Token::name(spaceToken); *cmd; cmd++)
    {
      switch (*cmd)
	{
	case '+':
	  {
	    ++globalIndent;
	    break;
	  }
	case '-':
	  {
	    if (globalIndent > 0)
	      --globalIndent;
	    break;
	  }
	case 'i':
	  {
	    if (globalIndent > 0)
	      {
		int t = Token::encode("\\s");
		for (int i = 0; i < globalIndent; i++)
		  buffer.append(t);
	      }
	    break;
	  }
#define MACRO(m, t)	case m:
#include "ansiEscapeSequences.cc"
#undef MACRO
	case 'o':
	case 'n':
	case 't':
	case 's':
	  {
	    static char strg[3] = "\\!";  // HACK
	    strg[1] = *cmd;
	    buffer.append(Token::encode(strg));
	    break;
	  }
	}
    }
}
