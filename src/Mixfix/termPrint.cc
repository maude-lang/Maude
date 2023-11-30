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
//	Term* -> ostream& pretty printer.
//

const char*
MixfixModule::computeColor(SymbolType st, const PrintSettings& printSettings)
{
  //
  //	For terms, automatically generated color (as opposed to format generated color) is based
  //	on the axioms of each operator symbol.
  //	  AC   : red
  //	  ACU  : magenta
  //	  A    : green
  //	  AU   : cyan
  //	  C/CU : blue
  //	  U    : yellow
  //
  if (printSettings.getPrintFlag(PrintSettings::PRINT_COLOR))
    {
      if (st.hasFlag(SymbolType::ASSOC))
	{
	  if (st.hasFlag(SymbolType::COMM))
	    return Tty(st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID) ? Tty::MAGENTA : Tty::RED).ctrlSequence();
	  else
	    return Tty(st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID) ? Tty::CYAN : Tty::GREEN).ctrlSequence();
	}
      if (st.hasFlag(SymbolType::COMM))
	return Tty(Tty::BLUE).ctrlSequence();
      if (st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID | SymbolType::IDEM))
	return Tty(Tty::YELLOW).ctrlSequence();
    }
  return 0;
}

void
MixfixModule::suffix(ostream& s, Term* term, bool needDisambig)
{
  if (needDisambig)
    s << ")." << disambiguatorSort(term);
}

bool
MixfixModule::handleIter(ostream& s,
			 Term* term,
			 SymbolInfo& si,
			 bool rangeKnown,
			 const PrintSettings& printSettings)
{
  if (!(si.symbolType.hasFlag(SymbolType::ITER)))
    return false;
  if (si.symbolType.getBasicType() == SymbolType::SUCC_SYMBOL &&
      printSettings.getPrintFlag(PrintSettings::PRINT_NUMBER))
    {
      SuccSymbol* succSymbol = safeCast(SuccSymbol*, term->symbol());
      if (succSymbol->isNat(term))
	{
	  const mpz_class& nat = succSymbol->getNat(term);
	  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
	    (!rangeKnown && (kindsWithSucc.size() > 1 || overloadedIntegers.count(nat)));
	  prefix(s, needDisambig);
	  s << nat;
	  suffix(s, term, needDisambig);
	  return true;
	}
    }
  S_Term* st = safeCast(S_Term*, term);
  const mpz_class& number = st->getNumber();
  if (number == 1)
    return false;  // do default thing

  bool needToDisambiguate;
  bool argumentRangeKnown;
  decideIteratedAmbiguity(rangeKnown, term->symbol(), number, needToDisambiguate, argumentRangeKnown);
  if (needToDisambiguate)
    s << '(';

  string prefixName;
  makeIterName(prefixName, term->symbol()->id(), number);
  printPrefixName(s, prefixName.c_str(), si, printSettings);
  s << '(';
  prettyPrint(s, printSettings, st->getArgument(),
	      PREFIX_GATHER, UNBOUNDED, 0, UNBOUNDED, 0, argumentRangeKnown);
  s << ')';
  suffix(s, term, needToDisambiguate);
  return true;
}

bool
MixfixModule::handleMinus(ostream& s,
			  Term* term,
			  bool rangeKnown,
			  const PrintSettings& printSettings)
{
  if (printSettings.getPrintFlag(PrintSettings::PRINT_NUMBER))
    {
      const MinusSymbol* minusSymbol = safeCast(MinusSymbol*, term->symbol());
      if (minusSymbol->isNeg(term))
	{
	  mpz_class neg;
	  (void) minusSymbol->getNeg(term, neg);
	  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
	    (!rangeKnown && (kindsWithMinus.size() > 1 || overloadedIntegers.count(neg)));
	  prefix(s, needDisambig);
	  s << neg;
	  suffix(s, term, needDisambig);
	  return true;
	}
    }
  return false;
}

bool
MixfixModule::handleDivision(ostream& s,
			     Term* term,
			     bool rangeKnown,
			     const PrintSettings& printSettings)
{
  if (printSettings.getPrintFlag(PrintSettings::PRINT_RAT))
    {
      const DivisionSymbol* divisionSymbol = safeCast(DivisionSymbol*, term->symbol());
      if (divisionSymbol->isRat(term))
	{
	  pair<mpz_class, mpz_class> rat;
	  rat.second = divisionSymbol->getRat(term, rat.first);
	  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
	    (!rangeKnown && (kindsWithDivision.size() > 1 || overloadedRationals.count(rat)));
	  prefix(s, needDisambig);
	  s << rat.first << '/' << rat.second;
	  suffix(s, term, needDisambig);
	  return true;
	}
    }
  return false;
}

void
MixfixModule::handleFloat(ostream& s,
			  Term* term,
			  bool rangeKnown,
			  const PrintSettings& printSettings)
{
  double mfValue = safeCast(FloatTerm*, term)->getValue();
  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
    (!rangeKnown && (floatSymbols.size() > 1 || overloadedFloats.count(mfValue)));
  prefix(s, needDisambig);
  s << doubleToString(mfValue);
  suffix(s, term, needDisambig);
}

void
MixfixModule::handleString(ostream& s,
			   Term* term,
			   bool rangeKnown,
			   const PrintSettings& printSettings)
{
  string strValue;
  Token::ropeToString(safeCast(StringTerm*, term)->getValue(), strValue);
  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
    (!rangeKnown && (stringSymbols.size() > 1 || overloadedStrings.count(strValue)));
  prefix(s, needDisambig);
  s << strValue;
  suffix(s, term, needDisambig);
}

void
MixfixModule::handleQuotedIdentifier(ostream& s,
				     Term* term,
				     bool rangeKnown,
				     const PrintSettings& printSettings)
{
  int qidCode = safeCast(QuotedIdentifierTerm*, term)->getIdIndex();
  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
    (!rangeKnown && (quotedIdentifierSymbols.size() > 1 || overloadedQuotedIdentifiers.count(qidCode)));
  prefix(s, needDisambig);
  s << '\'' << Token::name(qidCode);
  suffix(s, term, needDisambig);
}

void
MixfixModule::handleVariable(ostream& s,
			     Term* term,
			     bool rangeKnown,
			     const PrintSettings& printSettings)
{
  VariableTerm* v = safeCast(VariableTerm*, term);
  Sort* sort = safeCast(VariableSymbol*, term->symbol())->getSort();
  pair<int, int> p(v->id(), sort->id());
  bool needDisambig = !rangeKnown && overloadedVariables.count(p);  // kinds not handled
  prefix(s, needDisambig);
  printVariable(s, p.first, sort, printSettings);
  suffix(s, term, needDisambig);
}

void
MixfixModule::handleSMT_Number(ostream& s,
			       Term* term,
			       bool rangeKnown,
			       const PrintSettings& printSettings)
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
  //	Figure out what SMT sort we correspond to.
  //
  SMT_Info::SMT_Type t = getSMT_Info().getType(sort);
  Assert(t != SMT_Info::NOT_SMT, "bad SMT sort " << sort);
  if (t == SMT_Info::INTEGER)
    {
      const mpz_class& integer = value.get_num();
      bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
	(!rangeKnown && (kindsWithSucc.size() > 1 || overloadedIntegers.count(integer)));
      prefix(s, needDisambig);
      s << integer;
      suffix(s, term, needDisambig);
    }
  else
    {
      Assert(t == SMT_Info::REAL, "SMT number sort expected");
      pair<mpz_class, mpz_class> rat(value.get_num(), value.get_den());
      bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
	(!rangeKnown && (kindsWithDivision.size() > 1 || overloadedRationals.count(rat)));
      prefix(s, needDisambig);
      s << rat.first << '/' << rat.second;
      suffix(s, term, needDisambig);
    }
}

void
MixfixModule::prettyPrint(ostream& s,
			  const PrintSettings& printSettings,
			  Term* term,
			  int requiredPrec,
			  int leftCapture,
			  const ConnectedComponent* leftCaptureComponent,
			  int rightCapture,
			  const ConnectedComponent* rightCaptureComponent,
			  bool rangeKnown)
{
  if (UserLevelRewritingContext::interrupted())
    return;

  Symbol* symbol = term->symbol();
  SymbolInfo& si = symbolInfo[symbol->getIndexWithinModule()];
  //
  //	Check for special i/o representation.
  //
  if (handleIter(s, term, si, rangeKnown, printSettings))
    return;
  int basicType = si.symbolType.getBasicType();
  switch (basicType)
    {
    case SymbolType::MINUS_SYMBOL:
      {
	if (handleMinus(s, term, rangeKnown, printSettings))
	  return;
	break;
      }
    case SymbolType::DIVISION_SYMBOL:
      {
	if (handleDivision(s, term, rangeKnown, printSettings))
	  return;
	break;
      }
    case SymbolType::FLOAT:
      {
	handleFloat(s, term, rangeKnown, printSettings);
	return;
      }
    case SymbolType::STRING:
      {
	handleString(s, term, rangeKnown, printSettings);
	return;
      }
    case SymbolType::QUOTED_IDENTIFIER:
      {
	handleQuotedIdentifier(s, term, rangeKnown, printSettings);
	return;
      }
    case SymbolType::VARIABLE:
      {
	handleVariable(s, term, rangeKnown, printSettings);
	return;
      }
    case SymbolType::SMT_NUMBER_SYMBOL:
      {
	handleSMT_Number(s, term, rangeKnown, printSettings);
	return;
      }
    default:
      break;
    }
  //
  //	Default case where no special i/o representation applies.
  //
  const char* color = computeColor(si.symbolType, printSettings);
  int iflags = si.iflags;
  bool needDisambig = !rangeKnown && ambiguous(iflags);
  bool argRangeKnown = false;
  int nrArgs = symbol->arity();
  if (nrArgs == 0)
    {
      if (printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST))
	needDisambig = true;
    }
  else
    argRangeKnown = rangeOfArgumentsKnown(iflags, rangeKnown, needDisambig);

  prefix(s, needDisambig);
  bool printConceal = printSettings.concealedSymbol(symbol->id());
  if ((printSettings.getPrintFlag(PrintSettings::PRINT_MIXFIX) && !si.mixfixSyntax.empty() && !printConceal) || basicType == SymbolType::SORT_TEST)
    {
      //
      //	Mixfix case.
      //
      bool printWithParens = printSettings.getPrintFlag(PrintSettings::PRINT_WITH_PARENS);
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
	s << '(';
      int nrTails = 1;
      int pos = 0;
      ArgumentIterator a(*term);
      int moreArgs = a.valid();
      for (int arg = 0; moreArgs; arg++)
	{
	  Term* t = a.argument();
	  a.next();
	  moreArgs = a.valid();
	  pos = printTokens(s, si, pos, color, printSettings);
	  if (arg == nrArgs - 1 && moreArgs)
	    {
	      ++nrTails;
	      arg = 0;
	      if (needAssocParen)
		s << '(';
	      pos = printTokens(s, si, 0, color, printSettings);
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
	  prettyPrint(s, printSettings, t, si.gather[arg], lc, lcc, rc, rcc, argRangeKnown);
	  if (UserLevelRewritingContext::interrupted())
	    return;
	}
      printTails(s, si, pos, nrTails, needAssocParen, true, color, printSettings);
      if (UserLevelRewritingContext::interrupted())
	return;
      if (needParen)
	s << ')';
    }
  else
    {
      //
      //	Prefix case.
      //
      const char* prefixName = Token::name(symbol->id());
      if (color != 0)
	s << color << prefixName << Tty(Tty::RESET);
      else
	printPrefixName(s, prefixName, si, printSettings);
      ArgumentIterator a(*term);
      if (a.valid())
	{
	  if (printConceal)
	    s << "(...)";
	  else
	    {
	      int nrTails = 1;
	      s << '(';
	      for (int arg = 0;; arg++)
		{
		  Term* t = a.argument();
		  a.next();
		  int moreArgs = a.valid();
		  if (arg >= nrArgs - 1 &&
		      !(printSettings.getPrintFlag(PrintSettings::PRINT_FLAT)) &&
		      moreArgs)
		    {
		      ++nrTails;
		      printPrefixName(s, prefixName, si, printSettings);
		      s << '(';
		    }
		  prettyPrint(s, printSettings, t,
			      PREFIX_GATHER, UNBOUNDED, 0, UNBOUNDED, 0,
			      argRangeKnown);
		  if (UserLevelRewritingContext::interrupted())
		    return;
		  if (!moreArgs)
		    break;
		  s << ", ";
		}
	      while (nrTails-- > 0)
		{
		  if (UserLevelRewritingContext::interrupted())
		    return;
		  s << ')';
		}
	    }
	}
    }
  suffix(s, term, needDisambig);
}
