/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	Term* -> ostream& LaTeX pretty printer.
//

void
MixfixModule::latexPrettyPrint(ostream& s, Term* term, bool rangeKnown)
{
  clearIndent();
  const PrintSettings& printSettings = interpreter;  // HACK
  MixfixModule* module = safeCastNonNull<MixfixModule*>(term->symbol()->getModule());
  module->latexPrettyPrint(s, printSettings, term, UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, rangeKnown);
  latexClearColor(s);
}

void
MixfixModule::latexPrintStrategyTerm(ostream& s, RewriteStrategy* rs, Term* term) const
{
  s << Token::latexIdentifier(rs->id());
  if (rs->arity() > 0 || ruleLabels.find(rs->id()) != ruleLabels.end())
    {
      s << "\\maudeLeftParen";
      const char* sep = "";
      for (ArgumentIterator it(*term); it.valid(); it.next())
	{
	  s << sep;
	  latexPrettyPrint(s, it.argument());
	  sep = "\\maudeComma";
	}
      s << "\\maudeRightParen";
    }
}

const char*
MixfixModule::latexComputeColor(SymbolType st, const PrintSettings& printSettings)
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
	    return st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID) ? latexMagenta : latexRed;
	  else
	    return st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID) ? latexCyan : latexGreen;
	}
      if (st.hasFlag(SymbolType::COMM))
	return latexBlue;
      if (st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID | SymbolType::IDEM))
	return latexYellow;
    }
  return 0;
}

void
MixfixModule::latexSuffix(ostream& s, Term* term, bool needDisambig)
{
  if (needDisambig)
    s << "\\maudeRightParen\\maudeDisambigDot" << latexType(disambiguatorSort(term));
}

bool
MixfixModule::latexHandleIter(ostream& s,
			      Term* term,
			      const SymbolInfo& si,
			      bool rangeKnown,
			      const PrintSettings& printSettings)
{
  //
  //	Check if term is headed by a iter symbol and if so handle
  //	any special printing requirements.
  //	Returns true if handled, false if default handling required.
  //
  if (!(si.symbolType.hasFlag(SymbolType::ITER)))
    return false;
  //
  //	Check if the top symbol is also a succ symbol and we have number printing turned on.
  //
  if (si.symbolType.getBasicType() == SymbolType::SUCC_SYMBOL && printSettings.getPrintFlag(PrintSettings::PRINT_NUMBER))
    {
      //
      //	If term  corresponds to a number we want to print it as decimal number.
      //
      SuccSymbol* succSymbol = safeCastNonNull<SuccSymbol*>(term->symbol());
      if (succSymbol->isNat(term))
	{
	  const mpz_class& nat = succSymbol->getNat(term);
	  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
	    (!rangeKnown && (kindsWithSucc.size() > 1 || overloadedIntegers.count(nat)));
	  latexPrefix(s, needDisambig);
	  s << latexNumber(nat);
	  latexSuffix(s, term, needDisambig);
	  return true;
	}
    }
  //
  //	Not going to print term as a number, but might want to use special f^n(arg) representation.
  //
  S_Term* st = safeCastNonNull<S_Term*>(term);
  const mpz_class& number = st->getNumber();
  if (number == 1)
    return false;  // do default thing
  
  bool needToDisambiguate;
  bool argumentRangeKnown;
  decideIteratedAmbiguity(rangeKnown, term->symbol(), number, needToDisambiguate, argumentRangeKnown);
  latexPrefix(s, needToDisambiguate);
  
  string prefixName = "\\maudeIter{" + Token::latexIdentifier(term->symbol()->id()) + "}{" + number.get_str() + "}";
  latexPrintPrefixName(s, prefixName.c_str(), si, printSettings);
  s << "\\maudeLeftParen";
  latexPrettyPrint(s, printSettings, st->getArgument(), PREFIX_GATHER, UNBOUNDED, 0, UNBOUNDED, 0, argumentRangeKnown);
  s << "\\maudeRightParen";

  latexSuffix(s, term, needToDisambiguate);
  return true;
}

bool
MixfixModule::latexHandleMinus(ostream& s,
			       Term* term,
			       bool rangeKnown,
			       const PrintSettings& printSettings) const
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
	  latexPrefix(s, needDisambig);
	  s << latexNumber(neg);
	  latexSuffix(s, term, needDisambig);
	  return true;
	}
    }
  return false;
}

bool
MixfixModule::latexHandleDivision(ostream& s,
				  Term* term,
				  bool rangeKnown,
				  const PrintSettings& printSettings) const
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
	  latexPrefix(s, needDisambig);
	  s << latexNumber(rat.first) << "/" << latexNumber(rat.second);
	  latexSuffix(s, term, needDisambig);
	  return true;
	}
    }
  return false;
}

void
MixfixModule::latexHandleFloat(ostream& s,
			       Term* term,
			       bool rangeKnown,
			       const PrintSettings& printSettings) const
{
  double mfValue = safeCastNonNull<FloatTerm*>(term)->getValue();
  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
    (!rangeKnown && (floatSymbols.size() > 1 || overloadedFloats.count(mfValue)));
  latexPrefix(s, needDisambig);
  s << "\\maudeNumber{" << doubleToString(mfValue) << "}";
  latexSuffix(s, term, needDisambig);
}

void
MixfixModule::latexHandleString(ostream& s,
				Term* term,
				bool rangeKnown,
				const PrintSettings& printSettings) const
{
  string strValue;
  Token::ropeToString(safeCastNonNull<StringTerm*>(term)->getValue(), strValue);
  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
    (!rangeKnown && (stringSymbols.size() > 1 || overloadedStrings.count(strValue)));
  latexPrefix(s, needDisambig);
  s << latexString(strValue);
  latexSuffix(s, term, needDisambig);
}

void
MixfixModule::latexHandleQuotedIdentifier(ostream& s,
					  Term* term,
					  bool rangeKnown,
					  const PrintSettings& printSettings) const
{
  int qidCode = safeCastNonNull<QuotedIdentifierTerm*>(term)->getIdIndex();
  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
    (!rangeKnown && (quotedIdentifierSymbols.size() > 1 || overloadedQuotedIdentifiers.count(qidCode)));
  latexPrefix(s, needDisambig);
  s << latexQid(qidCode);
  latexSuffix(s, term, needDisambig);
}

void
MixfixModule::latexHandleVariable(ostream& s,
				  Term* term,
				  bool rangeKnown,
				  const PrintSettings& printSettings) const
{
  VariableTerm* v = safeCastNonNull<VariableTerm*>(term);
  Sort* sort = safeCastNonNull<VariableSymbol*>(term->symbol())->getSort();
  pair<int, int> p(v->id(), sort->id());
  bool needDisambig = !rangeKnown && overloadedVariables.count(p);  // kinds not handled
  latexPrefix(s, needDisambig);
  s << Token::latexIdentifier(v->id());
  
  if (printSettings.getPrintFlag(PrintSettings::PRINT_WITH_ALIASES))
    {
      AliasMap::const_iterator i = variableAliases.find(v->id());
      if (i != variableAliases.end() && (*i).second == sort)
	{
	  latexSuffix(s, term, needDisambig);
	  return;
	}
    }
  s << "\\maudeVariableColon" << latexType(sort);
  latexSuffix(s, term, needDisambig);
}

void
MixfixModule::latexHandleSMT_Number(ostream& s,
				    Term* term,
				    bool rangeKnown,
				    const PrintSettings& printSettings)
{
  //
  //	Get value.
  //
  SMT_NumberTerm* n = safeCastNonNull<SMT_NumberTerm*>(term);
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
      latexPrefix(s, needDisambig);
      s << latexNumber(integer);
      latexSuffix(s, term, needDisambig);
    }
  else
    {
      Assert(t == SMT_Info::REAL, "SMT number sort expected");
      pair<mpz_class, mpz_class> rat(value.get_num(), value.get_den());
      bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
	(!rangeKnown && (kindsWithDivision.size() > 1 || overloadedRationals.count(rat)));
      latexPrefix(s, needDisambig);
      s << latexNumber(rat.first) << "/" << latexNumber(rat.second);
      latexSuffix(s, term, needDisambig);
    }
}

void
MixfixModule::latexPrettyPrint(ostream& s,
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
  const SymbolInfo& si = symbolInfo[symbol->getIndexWithinModule()];
  //
  //	Check for special i/o representation.
  //
  if (latexHandleIter(s, term, si, rangeKnown, printSettings))
    return;
  int basicType = si.symbolType.getBasicType();
  switch (basicType)
    {
    case SymbolType::MINUS_SYMBOL:
      {
	if (latexHandleMinus(s, term, rangeKnown, printSettings))
	  return;
	break;
      }
    case SymbolType::DIVISION_SYMBOL:
      {
	if (latexHandleDivision(s, term, rangeKnown, printSettings))
	  return;
	break;
      }
    case SymbolType::FLOAT:
      {
	latexHandleFloat(s, term, rangeKnown, printSettings);
	return;
      }
    case SymbolType::STRING:
      {
	latexHandleString(s, term, rangeKnown, printSettings);
	return;
      }
    case SymbolType::QUOTED_IDENTIFIER:
      {
	latexHandleQuotedIdentifier(s, term, rangeKnown, printSettings);
	return;
      }
    case SymbolType::VARIABLE:
      {
	latexHandleVariable(s, term, rangeKnown, printSettings);
	return;
      }
    case SymbolType::SMT_NUMBER_SYMBOL:
      {
	latexHandleSMT_Number(s, term, rangeKnown, printSettings);
	return;
      }
    default:
      break;
    }
  //
  //	Default case where no special i/o representation applies.
  //
  const char* color = latexComputeColor(si.symbolType, printSettings);
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

 latexPrefix(s, needDisambig);
 bool printConceal = printSettings.concealedSymbol(symbol->id());
 if (si.symbolType.hasFlag(SymbolType::LATEX) && !printConceal && printSettings.getPrintFlag(PrintSettings::PRINT_LATEX))
   {
     //
     //	We don't put parentheses or sort disambiguation around user's latex code.
     //	Nor do we support automatic color based on symbol properties because this would affect subterms.
     //	It's completely the user's responsibility.
     //
     ArgumentIterator a(*term);
     latexAttributePrint(s, printSettings, symbol, a);
       return;
   }
 if (nrArgs == 0 && Token::auxProperty(symbol->id()) == Token::AUX_STRUCTURED_SORT)
   latexPrintStructuredConstant(s, symbol, color, printSettings);
 else if ((printSettings.getPrintFlag(PrintSettings::PRINT_MIXFIX) && !si.mixfixSyntax.empty() && !printConceal) ||
	  basicType == SymbolType::SORT_TEST)
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
       s << "\\maudeLeftParen";
     int nrTails = 1;
     int pos = 0;
     ArgumentIterator a(*term);
     int moreArgs = a.valid();
     for (int arg = 0; moreArgs; ++arg)
       {
	 Term* t = a.argument();
	 a.next();
	 moreArgs = a.valid();
	 pos = latexPrintTokens(s, si, pos, color, printSettings);
	 if (arg == nrArgs - 1 && moreArgs)
	   {
	     ++nrTails;
	     arg = 0;
	     if (needAssocParen)
	       s << "\\maudeLeftParen";
	     pos = latexPrintTokens(s, si, 0, color, printSettings);
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
	 latexPrettyPrint(s, printSettings, t, si.gather[arg], lc, lcc, rc, rcc, argRangeKnown);
	 if (UserLevelRewritingContext::interrupted())
	   return;
       }
     latexPrintTails(s, si, pos, nrTails, needAssocParen, true, color, printSettings);
     if (needParen)
       s << "\\maudeRightParen";
   }
 else
   {
     //
     //	Prefix case.
     //
     string prefixName = Token::latexIdentifier(symbol->id());
     if (color != 0)
       s << color << prefixName << latexResetColor;
     else
       latexPrintPrefixName(s, prefixName.c_str(), si, printSettings);
     ArgumentIterator a(*term);
     if (a.valid())
       {
	 if (printConceal)
	   s << "\\maudeLeftParen\\maudeEllipsis\\maudeRightParen";
	 else
	   {
	     int nrTails = 1;
	     s << "\\maudeLeftParen";
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
		     if (color != 0)
		       s << color << prefixName << latexResetColor;
		     else
		       latexPrintPrefixName(s, prefixName.c_str(), si, printSettings);
		     s << "\\maudeLeftParen";
		   }
		 latexPrettyPrint(s, printSettings, t, PREFIX_GATHER, UNBOUNDED, 0, UNBOUNDED, 0, argRangeKnown);
		 if (!moreArgs)
		   break;
		 s << "\\maudeComma";
	       }
	     while (nrTails-- > 0)
	       {
		 if (UserLevelRewritingContext::interrupted())
		   return;
		 s << "\\maudeRightParen";
	       }
	   }
       }
   }
 latexSuffix(s, term, needDisambig);
}

void
MixfixModule::latexAttributePrint(ostream& s,
				  const PrintSettings& printSettings,
				  Symbol* symbol,
				  ArgumentIterator& a)
{
  const SymbolInfo& si = symbolInfo[symbol->getIndexWithinModule()];
  if (si.symbolType.hasFlag(SymbolType::ASSOC))
    {
      //
      //	We need to convert flattened form in to right associative form on-the-fly.
      //
      Term* firstArg = a.argument();
      a.next();
      if (a.valid())
        {
	  //
	  //	Traverse latex macro.
	  //    Call latexPrettyPrint(s, printSettings, firstArg) for occurrences of #1
	  //	Recursive call latexAttributePrint(s, printSettings, s, a) for occurrences of #2
	  //
	  for (int i : si.latexMacroUnpacked)
	    {
	      if (i >= 0)
		s << static_cast<char>(i);
	      else
		{
		  if (i == -1)
		    {
		      //
		      //	First argument.
		      //
		      latexPrettyPrint(s, printSettings, firstArg, UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, true);  // optimistic
		    }
		  else
		    {
		      //
		      //	Second argument; make a recursive call to deal with the rest of the arguments.
		      //
		      Assert(i == -2, "bad argument number");
		      latexAttributePrint(s, printSettings, symbol, a);
		    }
		}
	    }
	}
      else
	{
	  //
	  //	Final argument, symbol not present.
	  //
	  latexPrettyPrint(s, printSettings, firstArg, UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, true);  // optimistic
	  
	}
    }
  else
    {
      Vector<Term*> args;
      for (; a.valid(); a.next())
	args.push_back(a.argument());
      for (int i : si.latexMacroUnpacked)
	{
	  if (i >= 0)
	    s << static_cast<char>(i);
	  else
	    {
	      latexPrettyPrint(s, printSettings, args[-1 - i], UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, true);  // optimistic
	    }
	}
    }
}
