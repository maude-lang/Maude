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
//	Routines for pretty printing a term into a vector of token codes.
//

void
MixfixModule::bufferPrint(Vector<int>& buffer, Term* term)
{
  globalIndent = 0;
  prettyPrint(buffer, term, UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
}

bool
MixfixModule::handleIter(Vector<int>& buffer,
			 Term* term,
			 SymbolInfo& si,
			 bool rangeKnown)
{
  if (!(si.symbolType.hasFlag(SymbolType::ITER)))
    return false;
  if (si.symbolType.getBasicType() == SymbolType::SUCC_SYMBOL &&
      interpreter.getFlag(Interpreter::PRINT_NUMBER))
    {
      SuccSymbol* succSymbol = safeCast(SuccSymbol*, term->symbol());
      if (succSymbol->isNat(term))
	{
	  char* name = mpz_get_str(0, 10, succSymbol->getNat(term).get_mpz_t());
	  buffer.append(Token::encode(name));
	  free(name);
	  return true;
	}
    }
  S_Term* st = safeCast(S_Term*, term);
  const mpz_class& number = st->getNumber();
  if (number == 1)
    return false;  // do default thing

  string prefixName;
  makeIterName(prefixName, term->symbol()->id(), number);
  printPrefixName(buffer, Token::encode(prefixName.c_str()), si);
  buffer.append(leftParen);
  prettyPrint(buffer, st->getArgument(), PREFIX_GATHER, UNBOUNDED, 0, UNBOUNDED, 0, rangeKnown);
  buffer.append(rightParen);
  return true;
}


bool
MixfixModule::handleMinus(Vector<int>& buffer,
			  Term* term,
			  SymbolInfo& si)
{
  if (si.symbolType.getBasicType() == SymbolType::MINUS_SYMBOL &&
      interpreter.getFlag(Interpreter::PRINT_NUMBER))
    {
      const MinusSymbol* minusSymbol = safeCast(MinusSymbol*, term->symbol());
      if (minusSymbol->isNeg(term))
	{
	  mpz_class neg;
	  char* name = mpz_get_str(0, 10, minusSymbol->getNeg(term, neg).get_mpz_t());
	  buffer.append(Token::encode(name));
	  free(name);
	  return true;
	}
    }
  return false;
}

bool
MixfixModule::handleDivision(Vector<int>& buffer,
			     Term* term,
			     SymbolInfo& si)
{
  if (si.symbolType.getBasicType() == SymbolType::DIVISION_SYMBOL &&
      interpreter.getFlag(Interpreter::PRINT_RAT))
    {
      const DivisionSymbol* divisionSymbol = safeCast(DivisionSymbol*, term->symbol());
      if (divisionSymbol->isRat(term))
	{
	  mpz_class numerator;
	  const mpz_class& denominator = divisionSymbol->getRat(term, numerator);
	  char* nn = mpz_get_str(0, 10, numerator.get_mpz_t());
	  string prefixName(nn);
	  free(nn);
	  prefixName += '/';
	  char* dn = mpz_get_str(0, 10, denominator.get_mpz_t());
	  prefixName += dn;
	  free(dn);
	  buffer.append(Token::encode(prefixName.c_str()));
	  return true;
	}
    }
  return false;
}

void
MixfixModule::prettyPrint(Vector<int>& buffer,
			  Term* term,
			  int requiredPrec,
			  int leftCapture,
			  const ConnectedComponent* leftCaptureComponent,
			  int rightCapture,
			  const ConnectedComponent* rightCaptureComponent,
			  bool rangeKnown)
{
  Symbol* symbol = term->symbol();
  int nrArgs = symbol->arity();
  int index = symbol->getIndexWithinModule();
  SymbolInfo& si = symbolInfo[index];
  int basicType = si.symbolType.getBasicType();
  int iflags = si.iflags;
  bool needDisambig = false;
  double mfValue;
  string strValue;
  int qidCode;

  if (iflags & PSEUDO_VARIABLE)
    needDisambig = !rangeKnown;
  else if (basicType == SymbolType::FLOAT)
    {
      mfValue = static_cast<FloatTerm*>(term)->getValue();
      needDisambig = !rangeKnown &&
	(floatSymbols.size() > 1 || overloadedFloats.count(mfValue));
    }
  else if (iflags & PSEUDO_FLOAT)
    {
      needDisambig = !rangeKnown &&
	(floatSymbols.size() > 0 || (iflags & DOMAIN_OVERLOADED));
    }
  else if (basicType == SymbolType::STRING)
    {
      Token::ropeToString(static_cast<StringTerm*>(term)->getValue(), strValue);
      needDisambig = !rangeKnown &&
	(stringSymbols.size() > 1 || overloadedStrings.count(strValue));
    }
  else if (iflags & PSEUDO_STRING)
    {
      needDisambig = !rangeKnown &&
	(stringSymbols.size() > 0 || (iflags & DOMAIN_OVERLOADED));
    }
  else if (basicType == SymbolType::QUOTED_IDENTIFIER)
    {
      qidCode = static_cast<QuotedIdentifierTerm*>(term)->getIdIndex();
      needDisambig = !rangeKnown &&
	(quotedIdentifierSymbols.size() > 1 ||
	 overloadedQuotedIdentifiers.count(qidCode));
    }
  else if (iflags & PSEUDO_QUOTED_IDENTIFIER)
    {
      needDisambig = !rangeKnown &&
	(quotedIdentifierSymbols.size() > 0 || (iflags & DOMAIN_OVERLOADED));
    }
  else
    {
      needDisambig = !rangeKnown && (iflags & DOMAIN_OVERLOADED);
      rangeKnown = !(iflags & ADHOC_OVERLOADED) ||
	((rangeKnown | needDisambig) && !(iflags & RANGE_OVERLOADED));
    }

  if (needDisambig)
    buffer.append(leftParen);
  if (basicType == SymbolType::VARIABLE) // HACK
    {
      VariableTerm* v = safeCast(VariableTerm*, term);
      string fullName(Token::name(v->id()));
      Sort* sort = v->getSort();
      fullName += ':';
      fullName += Token::name(sort->id());  // not correct for kinds
      buffer.append(Token::encode(fullName.c_str()));
    }
  else if (basicType == SymbolType::FLOAT)
    buffer.append(Token::doubleToCode(mfValue));
  else if (basicType == SymbolType::STRING)
    buffer.append(Token::encode(strValue.c_str()));
  else if (basicType == SymbolType::QUOTED_IDENTIFIER)
    buffer.append(Token::quoteNameCode(qidCode));
  else if (handleIter(buffer, term, si, rangeKnown))
    ;
  else if (handleMinus(buffer, term, si))
    ;
  else if (handleDivision(buffer, term, si))
    ;
  else if ((printMixfix && si.mixfixSyntax.length() != 0) || (basicType == SymbolType::SORT_TEST))
    {
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
	  pos = printTokens(buffer, si, pos);
	  if (arg == nrArgs - 1 && moreArgs)
	    {
	      ++nrTails;
	      arg = 0;
	      if (needAssocParen)
		buffer.append(leftParen);
	      pos = printTokens(buffer, si, 0);
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
	  prettyPrint(buffer, t, si.gather[arg], lc, lcc, rc, rcc, rangeKnown);
	}
      printTails(buffer, si, pos, nrTails, needAssocParen);
      if (needParen)
	buffer.append(rightParen);
    }
  else
    {
      int id = symbol->id();
      printPrefixName(buffer, id, si);
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
	      if (arg >= nrArgs - 1 && !printFlat && moreArgs)
		{
		  ++nrTails;
		  printPrefixName(buffer, id, si);
		  buffer.append(leftParen);
		}
	      prettyPrint(buffer, t, PREFIX_GATHER, UNBOUNDED, 0, UNBOUNDED, 0, rangeKnown);
	      if (!moreArgs)
		break;
	      buffer.append(comma);
	    }
	  while (nrTails-- > 0)
	    buffer.append(rightParen);
	}
    }
  if (needDisambig)
    {
      int sortIndex = term->getSortIndex();
      if (sortIndex <= Sort::ERROR_SORT)
	sortIndex = chooseDisambiguator(symbol);
      buffer.append(rightParen);
      // THIS IS NOT CORRECT FOR KINDS
      buffer.append(Token::dotNameCode(symbol->rangeComponent()->sort(sortIndex)->id()));
    }
}

int
MixfixModule::printTokens(Vector<int>& buffer, const SymbolInfo& si, int pos)
{
  bool hasFormat = printFormat && (si.format.length() > 0);
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
			 bool needAssocParen)
{
  bool hasFormat = printFormat && (si.format.length() > 0);
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
	buffer.append(leftParen);
    }
}

void
MixfixModule::printPrefixName(Vector<int>& buffer, int prefixName, SymbolInfo& si)
{
  if (printFormat && (si.format.length() == 2))
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
