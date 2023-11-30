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
//	DagNode* -> ostream& pretty printer.
//

const char*
MixfixModule::computeColor(ColoringInfo& coloringInfo, DagNode* dagNode, const PrintSettings& printSettings)
{
  if (printSettings.getPrintFlag(PrintSettings::PRINT_COLOR))
    {
      if (dagNode->isReduced())
	{
	  int index = coloringInfo.visited.pointer2Index(dagNode);
	  Assert(index != NONE, "didn't visit dag node");
	  int status = coloringInfo.statusVec[index];
	  if (!(status & CONSTRUCTOR))
	    {
	      return Tty((status & STRANGENESS_BELOW) ?
			 Tty::BLUE : Tty::RED).ctrlSequence();
	    }
	}
      else
	{
	  return Tty(coloringInfo.reducedAbove ?
		     (coloringInfo.reducedDirectlyAbove ?
		      Tty::MAGENTA : Tty::CYAN) :
		     Tty::GREEN).ctrlSequence();
	}
    }
  return 0;
}

void
MixfixModule::suffix(ostream& s,
		     DagNode* dagNode,
		     bool needDisambig,
		     const char* color)
{
  if (color != 0)
    s << Tty(Tty::RESET);
  if (needDisambig)
    {
      Symbol* symbol = dagNode->symbol();
      int sortIndex = dagNode->getSortIndex();
      if (sortIndex <= Sort::ERROR_SORT)
	sortIndex = chooseDisambiguator(symbol);
      s << ")." << symbol->rangeComponent()->sort(sortIndex);
    }
}

bool
MixfixModule::handleIter(ostream& s,
			 ColoringInfo& coloringInfo,
			 DagNode* dagNode,
			 SymbolInfo& si,
			 bool rangeKnown,
			 const char* color,
			 const PrintSettings& printSettings)
{
  //
  //	Check if dagNode is headed by a iter symbol and if so handle
  //	any special printing requirements.
  //	Returns true if handled, false if default handling required.
  //
  if (!(si.symbolType.hasFlag(SymbolType::ITER)))
    return false;
  //
  //	Check if the top symbol is also a succ symbol and we have
  //	number printing turned on.
  //
  if (si.symbolType.getBasicType() == SymbolType::SUCC_SYMBOL &&
      printSettings.getPrintFlag(PrintSettings::PRINT_NUMBER))
    {
      //
      //	If dagNode corresponds to a number we want to
      //	print it as decimal number.
      //
      SuccSymbol* succSymbol = safeCast(SuccSymbol*, dagNode->symbol());
      if (succSymbol->isNat(dagNode))
	{
	  const mpz_class& nat = succSymbol->getNat(dagNode);
	  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
	    (!rangeKnown && (kindsWithSucc.size() > 1 || overloadedIntegers.count(nat)));
	  prefix(s, needDisambig, color);
	  s << nat;
	  suffix(s, dagNode, needDisambig, color);
	  return true;
	}
    }
  //
  //	Not going to print dagNode as a number, but might want to use special f^n(arg) representation.
  //
  S_DagNode* sd = safeCast(S_DagNode*, dagNode);
  const mpz_class& number = sd->getNumber();
  if (number == 1)
    return false;  // do default thing
  
  bool needToDisambiguate;
  bool argumentRangeKnown;
  decideIteratedAmbiguity(rangeKnown, dagNode->symbol(), number, needToDisambiguate, argumentRangeKnown);
  if (needToDisambiguate)
    s << '(';

  string prefixName;
  makeIterName(prefixName, dagNode->symbol()->id(), number);
  if (color != 0)
    s << color << prefixName << Tty(Tty::RESET);
  else
    printPrefixName(s, prefixName.c_str(), si, printSettings);
  s << '(';
  if (printSettings.getPrintFlag(PrintSettings::PRINT_COLOR))
    {
      coloringInfo.reducedDirectlyAbove = dagNode->isReduced();
      coloringInfo.reducedAbove = coloringInfo.reducedAbove ||
	coloringInfo.reducedDirectlyAbove;
    }
  prettyPrint(s, coloringInfo, printSettings, sd->getArgument(),
	      PREFIX_GATHER, UNBOUNDED, 0, UNBOUNDED, 0, argumentRangeKnown);
  s << ')';
  suffix(s, dagNode, needToDisambiguate, color);
  return true;
}

bool
MixfixModule::handleMinus(ostream& s,
			  DagNode* dagNode,
			  bool rangeKnown,
			  const char* color,
			  const PrintSettings& printSettings)
{
  if (printSettings.getPrintFlag(PrintSettings::PRINT_NUMBER))
    {
      const MinusSymbol* minusSymbol = safeCast(MinusSymbol*, dagNode->symbol());
      if (minusSymbol->isNeg(dagNode))
	{
	  mpz_class neg;
	  (void) minusSymbol->getNeg(dagNode, neg);
	  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
	    (!rangeKnown && (kindsWithMinus.size() > 1 || overloadedIntegers.count(neg)));
	  prefix(s, needDisambig, color);
	  s << neg;
	  suffix(s, dagNode, needDisambig, color);
	  return true;
	}
    }
  return false;
}

bool
MixfixModule::handleDivision(ostream& s,
			     DagNode* dagNode,
			     bool rangeKnown,
			     const char* color,
			     const PrintSettings& printSettings)
{
  if (printSettings.getPrintFlag(PrintSettings::PRINT_RAT))
    {
      const DivisionSymbol* divisionSymbol = safeCast(DivisionSymbol*, dagNode->symbol());
      if (divisionSymbol->isRat(dagNode))
	{
	  pair<mpz_class, mpz_class> rat;
	  rat.second = divisionSymbol->getRat(dagNode, rat.first);
	  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
	    (!rangeKnown && (kindsWithDivision.size() > 1 || overloadedRationals.count(rat)));
	  prefix(s, needDisambig, color);
	  s << rat.first << '/' << rat.second;
	  suffix(s, dagNode, needDisambig, color);
	  return true;
	}
    }
  return false;
}

void
MixfixModule::handleFloat(ostream& s,
			  DagNode* dagNode,
			  bool rangeKnown,
			  const char* color,
			  const PrintSettings& printSettings)
{
  double mfValue = safeCast(FloatDagNode*, dagNode)->getValue();
  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
    (!rangeKnown && (floatSymbols.size() > 1 || overloadedFloats.count(mfValue)));
  prefix(s, needDisambig, color);
  s << doubleToString(mfValue);
  suffix(s, dagNode, needDisambig, color);
}

void
MixfixModule::handleString(ostream& s,
			   DagNode* dagNode,
			   bool rangeKnown,
			   const char* color,
			   const PrintSettings& printSettings)
{
  string strValue;
  Token::ropeToString(safeCast(StringDagNode*, dagNode)->getValue(), strValue);
  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
    (!rangeKnown && (stringSymbols.size() > 1 || overloadedStrings.count(strValue)));
  prefix(s, needDisambig, color);
  s << strValue;
  suffix(s, dagNode, needDisambig, color);
}

void
MixfixModule::handleQuotedIdentifier(ostream& s,
				     DagNode* dagNode,
				     bool rangeKnown,
				     const char* color,
				     const PrintSettings& printSettings)
{
  int qidCode = safeCast(QuotedIdentifierDagNode*, dagNode)->getIdIndex();
  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
    (!rangeKnown && (quotedIdentifierSymbols.size() > 1 || overloadedQuotedIdentifiers.count(qidCode)));
  prefix(s, needDisambig, color);
  s << '\'' << Token::name(qidCode);
  suffix(s, dagNode, needDisambig, color);
}

void
MixfixModule::handleVariable(ostream& s,
			     DagNode* dagNode,
			     bool rangeKnown,
			     const char* color,
			     const PrintSettings& printSettings)
{
  VariableDagNode* v = safeCast(VariableDagNode*, dagNode);
  Sort* sort = safeCast(VariableSymbol*, dagNode->symbol())->getSort();
  pair<int, int> p(v->id(), sort->id());
  bool needDisambig = !rangeKnown && overloadedVariables.count(p);  // kinds not handled
  prefix(s, needDisambig, color);
  printVariable(s, p.first, sort, printSettings);
  suffix(s, dagNode, needDisambig, color);
}

void
MixfixModule::handleSMT_Number(ostream& s,
			       DagNode* dagNode,
			       bool rangeKnown,
			       const char* color,
			       const PrintSettings& printSettings)
{
  //
  //	Get value.
  //
  SMT_NumberDagNode* n = safeCast(SMT_NumberDagNode*, dagNode);
  const mpq_class& value = n->getValue();
  //
  //	Look up the index of our sort.
  //
  Symbol* symbol = dagNode->symbol();
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
      prefix(s, needDisambig, color);
      s << integer;
      suffix(s, dagNode, needDisambig, color);
    }
  else
    {
      Assert(t == SMT_Info::REAL, "SMT number sort expected");
      pair<mpz_class, mpz_class> rat(value.get_num(), value.get_den());
      bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
	(!rangeKnown && (kindsWithDivision.size() > 1 || overloadedRationals.count(rat)));
      prefix(s, needDisambig, color);
      s << rat.first << '/' << rat.second;
      suffix(s, dagNode, needDisambig, color);
    }
}

void
MixfixModule::prettyPrint(ostream& s,
			  ColoringInfo& coloringInfo,
			  const PrintSettings& printSettings,
			  DagNode* dagNode,
			  int requiredPrec,
			  int leftCapture,
			  const ConnectedComponent* leftCaptureComponent,
			  int rightCapture,
			  const ConnectedComponent* rightCaptureComponent,
			  bool rangeKnown)
{
  if (UserLevelRewritingContext::interrupted())
    return;

#ifndef NO_ASSERT
  if (dagNode == 0)
    {
      s << "!!! NULL POINTER !!!";
      return;
    }
#endif

  const char* color = computeColor(coloringInfo, dagNode, printSettings);
  Symbol* symbol = dagNode->symbol();
  SymbolInfo& si = symbolInfo[symbol->getIndexWithinModule()];
  //
  //	Check for special i/o representation.
  //
  if (handleIter(s, coloringInfo, dagNode, si, rangeKnown, color, printSettings))
    return;
  int basicType = si.symbolType.getBasicType();
  switch (basicType)
    {
    case SymbolType::MINUS_SYMBOL:
      {
	if (handleMinus(s, dagNode, rangeKnown, color, printSettings))
	  return;
	break;
      }
    case SymbolType::DIVISION_SYMBOL:
      {
	if (handleDivision(s, dagNode, rangeKnown, color, printSettings))
	  return;
	break;
      }
    case SymbolType::FLOAT:
      {
	handleFloat(s, dagNode, rangeKnown, color, printSettings);
	return;
      }
    case SymbolType::STRING:
      {
	handleString(s, dagNode, rangeKnown, color, printSettings);
	return;
      }
    case SymbolType::QUOTED_IDENTIFIER:
      {
	handleQuotedIdentifier(s, dagNode, rangeKnown, color, printSettings);
	return;
      }
    case SymbolType::VARIABLE:
      {
	handleVariable(s, dagNode, rangeKnown, color, printSettings);
	return;
      }
    case SymbolType::SMT_NUMBER_SYMBOL:
      {
	handleSMT_Number(s, dagNode, rangeKnown, color, printSettings);
	return;
      }
    default:
      break;
    }
  //
  //	Default case where no special i/o representation applies.
  //
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

  if (printSettings.getPrintFlag(PrintSettings::PRINT_COLOR))
    {
      coloringInfo.reducedDirectlyAbove = dagNode->isReduced();
      coloringInfo.reducedAbove = coloringInfo.reducedAbove ||
	coloringInfo.reducedDirectlyAbove;
    }
  if (needDisambig)
    s << '(';
  bool printConceal = printSettings.concealedSymbol(symbol->id());
  if ((printSettings.getPrintFlag(PrintSettings::PRINT_MIXFIX) && !si.mixfixSyntax.empty() && !printConceal) ||
      (basicType == SymbolType::SORT_TEST))
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
      DagArgumentIterator a(*dagNode);
      int moreArgs = a.valid();
      for (int arg = 0; moreArgs; arg++)
	{
	  DagNode* d = a.argument();
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
	  prettyPrint(s, coloringInfo, printSettings, d,
		      si.gather[arg], lc, lcc, rc, rcc,
		      argRangeKnown);
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
      DagArgumentIterator a(*dagNode);
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
		  DagNode* d = a.argument();
		  a.next();
		  int moreArgs = a.valid();
		  if (arg >= nrArgs - 1 &&
		      !(printSettings.getPrintFlag(PrintSettings::PRINT_FLAT)) &&
		      moreArgs)
		    {
		      ++nrTails;
		      if (color != 0)
			s << color << prefixName << Tty(Tty::RESET);
		      else
			printPrefixName(s, prefixName, si, printSettings);
		      s << '(';
		    }
		  prettyPrint(s, coloringInfo, printSettings, d,
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
  if (needDisambig)
    {
      int sortIndex = dagNode->getSortIndex();
      if (sortIndex <= Sort::ERROR_SORT)
	sortIndex = chooseDisambiguator(symbol);
      s << ")." << symbol->rangeComponent()->sort(sortIndex);
    }
}
