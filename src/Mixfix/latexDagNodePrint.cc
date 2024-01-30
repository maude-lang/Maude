/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
//	DagNode* -> ostream& LaTeX pretty printer.
//

void
MixfixModule::latexPrettyPrint(ostream& s, DagNode* dagNode)
{
  if (dagNode == 0)
    {
      s << "\\maudeMisc{(null DagNode*)}";
      return;
    }
  const PrintSettings& printSettings = interpreter;  // HACK
  MixfixModule* module = static_cast<MixfixModule*>(dagNode->symbol()->getModule());
  if (printSettings.getPrintFlag(PrintSettings::PRINT_GRAPH))
    {
      s << "$";
      module->latexGraphPrint(s, dagNode, printSettings);
      s << "$";
    }
  else
    {
      clearIndent();
      s << "$";
      ColoringInfo coloringInfo;
      if (printSettings.getPrintFlag(PrintSettings::PRINT_COLOR))
	{
	  computeGraphStatus(dagNode, coloringInfo.visited, coloringInfo.statusVec);
	  coloringInfo.reducedAbove = false;
	  coloringInfo.reducedDirectlyAbove = false;
	}
      module->latexPrettyPrint(s, printSettings, coloringInfo, dagNode, UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
      s << "$";
    }
}

void
MixfixModule::latexPrintDagNode(ostream& s, DagNode* dagNode)
{
  const PrintSettings& printSettings = interpreter;  // HACK
  MixfixModule* module = safeCastNonNull<MixfixModule*>(dagNode->symbol()->getModule());
  if (printSettings.getPrintFlag(PrintSettings::PRINT_GRAPH))
    module->latexGraphPrint(s, dagNode, printSettings);
  else
    {
      clearIndent();
      ColoringInfo coloringInfo;
      if (printSettings.getPrintFlag(PrintSettings::PRINT_COLOR))
	{
	  computeGraphStatus(dagNode, coloringInfo.visited, coloringInfo.statusVec);
	  coloringInfo.reducedAbove = false;
	  coloringInfo.reducedDirectlyAbove = false;
	}
      module->latexPrettyPrint(s, printSettings, coloringInfo, dagNode, UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
    }
}

const char*
MixfixModule::latexComputeColor(ColoringInfo& coloringInfo, DagNode* dagNode, const PrintSettings& printSettings)
{
  //
  //	For dags, automatically generated color (as opposed to format generated color) is based
  //	on ctor-ness and reduced-ness of each dag node.
  //
  if (printSettings.getPrintFlag(PrintSettings::PRINT_COLOR))
    {
      if (dagNode->isReduced())
	{
	  int index = coloringInfo.visited.pointer2Index(dagNode);
	  Assert(index != NONE, "didn't visit dag node");
	  int status = coloringInfo.statusVec[index];
	  if (!(status & CONSTRUCTOR))
	    return (status & STRANGENESS_BELOW) ? latexBlue : latexRed;
	}
      else
	{
	  return coloringInfo.reducedAbove ?
	    (coloringInfo.reducedDirectlyAbove ? latexMagenta : latexCyan) : latexGreen;
	}
    }
  return 0;
}

void
MixfixModule::latexSuffix(ostream& s, DagNode* dagNode, bool needDisambig, const char* color)
{
  if (color != 0)
    s << latexResetColor;
  if (needDisambig)
    {
      Symbol* symbol = dagNode->symbol();
      int sortIndex = dagNode->getSortIndex();
      if (sortIndex <= Sort::ERROR_SORT)
	sortIndex = chooseDisambiguator(symbol);
      s << "\\maudeRightParen\\maudeDisambigDot" << latexType(symbol->rangeComponent()->sort(sortIndex));
    }
}

bool
MixfixModule::latexHandleIter(ostream& s,
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
      SuccSymbol* succSymbol = safeCastNonNull<SuccSymbol*>(dagNode->symbol());
      if (succSymbol->isNat(dagNode))
	{
	  const mpz_class& nat = succSymbol->getNat(dagNode);
	  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
	    (!rangeKnown && (kindsWithSucc.size() > 1 || overloadedIntegers.count(nat)));
	  latexPrefix(s, needDisambig, color);
	  s << latexNumber(nat);
	  latexSuffix(s, dagNode, needDisambig, color);
	  return true;
	}
    }
  //
  //	Not going to print dagNode as a number, but might want to use special f^n(arg) representation.
  //
  S_DagNode* sd = safeCastNonNull<S_DagNode*>(dagNode);
  const mpz_class& number = sd->getNumber();
  if (number == 1)
    return false;  // do default thing
  
  bool needToDisambiguate;
  bool argumentRangeKnown;
  decideIteratedAmbiguity(rangeKnown, dagNode->symbol(), number, needToDisambiguate, argumentRangeKnown);
  if (needToDisambiguate)
    s << "\\maudeLeftParen";

  string prefixName = "\\maudeIter{" + Token::latexIdentifier(dagNode->symbol()->id()) + "}{" + number.get_str() + "}";
  if (color != 0)
    s << color << prefixName << latexResetColor;
  else
    latexPrintPrefixName(s, prefixName.c_str(), si, printSettings);
  if (printSettings.getPrintFlag(PrintSettings::PRINT_COLOR))
    {
      coloringInfo.reducedDirectlyAbove = dagNode->isReduced();
      coloringInfo.reducedAbove = coloringInfo.reducedAbove ||
	coloringInfo.reducedDirectlyAbove;
    }
  s << "\\maudeLeftParen";
  latexPrettyPrint(s, printSettings, coloringInfo, sd->getArgument(), PREFIX_GATHER, UNBOUNDED, 0, UNBOUNDED, 0, argumentRangeKnown);
  s << "\\maudeRightParen";
  suffix(s, dagNode, needToDisambiguate, color);
  return true;
}

bool
MixfixModule::latexHandleMinus(ostream& s,
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
	  latexPrefix(s, needDisambig, color);
	  s << latexNumber(neg);
	  latexSuffix(s, dagNode, needDisambig, color);
	  return true;
	}
    }
  return false;
}

bool
MixfixModule::latexHandleDivision(ostream& s,
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
	  latexPrefix(s, needDisambig, color);
	  s << latexNumber(rat.first) << "/" << latexNumber(rat.second);
	  latexSuffix(s, dagNode, needDisambig, color);
	  return true;
	}
    }
  return false;
}

void
MixfixModule::latexHandleFloat(ostream& s,
			       DagNode* dagNode,
			       bool rangeKnown,
			       const char* color,
			       const PrintSettings& printSettings)
{
  double mfValue = safeCast(FloatDagNode*, dagNode)->getValue();
  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
    (!rangeKnown && (floatSymbols.size() > 1 || overloadedFloats.count(mfValue)));
  latexPrefix(s, needDisambig, color);
  s << "\\maudeNumber{" << doubleToString(mfValue) << "}";
  latexSuffix(s, dagNode, needDisambig, color);
}

void
MixfixModule::latexHandleString(ostream& s,
				DagNode* dagNode,
				bool rangeKnown,
				const char* color,
				const PrintSettings& printSettings)
{
  string strValue;
  Token::ropeToString(safeCast(StringDagNode*, dagNode)->getValue(), strValue);
  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
    (!rangeKnown && (stringSymbols.size() > 1 || overloadedStrings.count(strValue)));
  latexPrefix(s, needDisambig, color);
  s << latexString(strValue);
  latexSuffix(s, dagNode, needDisambig, color);
}

void
MixfixModule::latexHandleQuotedIdentifier(ostream& s,
					  DagNode* dagNode,
					  bool rangeKnown,
					  const char* color,
					  const PrintSettings& printSettings)
{
  int qidCode = safeCast(QuotedIdentifierDagNode*, dagNode)->getIdIndex();
  bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
    (!rangeKnown && (quotedIdentifierSymbols.size() > 1 || overloadedQuotedIdentifiers.count(qidCode)));
  latexPrefix(s, needDisambig, color);
  s << latexQid(qidCode);
  latexSuffix(s, dagNode, needDisambig, color);
}

void
MixfixModule::latexHandleVariable(ostream& s,
				  DagNode* dagNode,
				  bool rangeKnown,
				  const char* color,
				  const PrintSettings& printSettings)
{
  VariableDagNode* v = safeCast(VariableDagNode*, dagNode);
  Sort* sort = safeCast(VariableSymbol*, dagNode->symbol())->getSort();
  pair<int, int> p(v->id(), sort->id());
  bool needDisambig = !rangeKnown && overloadedVariables.count(p);  // kinds not handled
  latexPrefix(s, needDisambig, color);
  s << Token::latexIdentifier(v->id());

  if (printSettings.getPrintFlag(PrintSettings::PRINT_WITH_ALIASES))
    {
      AliasMap::const_iterator i = variableAliases.find(v->id());
      if (i != variableAliases.end() && (*i).second == sort)
	{
	  latexSuffix(s, dagNode, needDisambig, color);
	  return;
	}
    }
  s << "\\maudeVariableColon" << latexType(sort);
  latexSuffix(s, dagNode, needDisambig, color);
}

void
MixfixModule::latexHandleSMT_Number(ostream& s,
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
      latexPrefix(s, needDisambig, color);
      s << latexNumber(integer);
      latexSuffix(s, dagNode, needDisambig, color);
    }
  else
    {
      Assert(t == SMT_Info::REAL, "SMT number sort expected");
      pair<mpz_class, mpz_class> rat(value.get_num(), value.get_den());
      bool needDisambig = printSettings.getPrintFlag(PrintSettings::PRINT_DISAMBIG_CONST) ||
	(!rangeKnown && (kindsWithDivision.size() > 1 || overloadedRationals.count(rat)));
      latexPrefix(s, needDisambig, color);
      s << latexNumber(rat.first) << "/" << latexNumber(rat.second);
      latexSuffix(s, dagNode, needDisambig, color);
    }
}

void
MixfixModule::latexPrettyPrint(ostream& s,
			       const PrintSettings& printSettings,
			       ColoringInfo& coloringInfo,
			       DagNode* dagNode,
			       int requiredPrec,
			       int leftCapture,
			       const ConnectedComponent* leftCaptureComponent,
			       int rightCapture,
			       const ConnectedComponent* rightCaptureComponent,
			       bool rangeKnown)
{
  Assert(dagNode != 0, "null pointer");
  if (UserLevelRewritingContext::interrupted())
    return;

  Symbol* symbol = dagNode->symbol();
  SymbolInfo& si = symbolInfo[symbol->getIndexWithinModule()];
  const char* color = latexComputeColor(coloringInfo, dagNode, printSettings);
  //
  //	Check for special i/o representation.
  //
  if (latexHandleIter(s, coloringInfo, dagNode, si, rangeKnown, color, printSettings))
    return;
  int basicType = si.symbolType.getBasicType();
  switch (basicType)
    {
    case SymbolType::MINUS_SYMBOL:
      {
	if (latexHandleMinus(s, dagNode, rangeKnown, color, printSettings))
	  return;
	break;
      }
    case SymbolType::DIVISION_SYMBOL:
      {
	if (latexHandleDivision(s, dagNode, rangeKnown, color, printSettings))
	  return;
	break;
      }
    case SymbolType::FLOAT:
      {
	latexHandleFloat(s, dagNode, rangeKnown, color, printSettings);
	return;
      }
    case SymbolType::STRING:
      {
	latexHandleString(s, dagNode, rangeKnown, color, printSettings);
	return;
      }
    case SymbolType::QUOTED_IDENTIFIER:
      {
	latexHandleQuotedIdentifier(s, dagNode, rangeKnown, color, printSettings);
	return;
      }
    case SymbolType::VARIABLE:
      {
	latexHandleVariable(s, dagNode, rangeKnown, color, printSettings);
	return;
      }
    case SymbolType::SMT_NUMBER_SYMBOL:
      {
	latexHandleSMT_Number(s, dagNode, rangeKnown, color, printSettings);
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

  bool printConceal = printSettings.concealedSymbol(symbol->id());
  if (si.symbolType.hasFlag(SymbolType::LATEX) && !printConceal)
    {
      //
      //	We don't put parentheses or sort disambiguation around user's latex code.
      //
      if (color != 0)
	s << color;
      DagArgumentIterator a(*dagNode);
      latexAttributePrint(s, printSettings, coloringInfo, symbol, a);
      if (color != 0)
	s << latexResetColor;
      return;
    }
  //
  //	We handle disambiguation, but not color with latexPrefix()/latexSuffix().
  //
  latexPrefix(s, needDisambig);
  if (nrArgs == 0 && Token::auxProperty(symbol->id()) == Token::AUX_STRUCTURED_SORT)
    latexPrintStructuredConstant(s, symbol, color, printSettings);
    //s << latexStructuredConstant(symbol->id());  // FIXME: need to handle color and format
  else if ((printSettings.getPrintFlag(PrintSettings::PRINT_MIXFIX) && !si.mixfixSyntax.empty() && !printConceal) ||
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
	s << "\\maudeLeftParen";
      int nrTails = 1;
      int pos = 0;
      DagArgumentIterator a(*dagNode);
      int moreArgs = a.valid();
      for (int arg = 0; moreArgs; ++arg)
	{
	  DagNode* d = a.argument();
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
	  latexPrettyPrint(s, printSettings, coloringInfo, d, si.gather[arg], lc, lcc, rc, rcc, argRangeKnown);
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
      	s << color;
      latexPrintPrefixName(s, prefixName.c_str(), si, printSettings);
      if (color != 0)
	s << latexResetColor;
      DagArgumentIterator a(*dagNode);
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
		  DagNode* d = a.argument();
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
		  latexPrettyPrint(s, printSettings, coloringInfo, d,
				   PREFIX_GATHER, UNBOUNDED, 0, UNBOUNDED, 0,
				   argRangeKnown);
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
  latexSuffix(s, dagNode, needDisambig);
}

void
MixfixModule::latexAttributePrint(ostream& s,
				  const PrintSettings& printSettings,
				  ColoringInfo& coloringInfo,
				  Symbol* symbol,
				  DagArgumentIterator& a)
{
  const SymbolInfo& si = symbolInfo[symbol->getIndexWithinModule()];
  if (si.symbolType.hasFlag(SymbolType::ASSOC))
    {
      //
      //	We need to convert flattened form in to right associative form on-the-fly.
      //
      DagNode* firstArg = a.argument();
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
		      latexPrettyPrint(s, printSettings, coloringInfo, firstArg,
				       UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, true);  // optimistic
		    }
		  else
		    {
		      //
		      //	Second argument; make a recursive call to deal with the rest of the arguments.
		      //
		      Assert(i == -2, "bad argument number");
		      latexAttributePrint(s, printSettings, coloringInfo, symbol, a);
		    }
		}
	    }
	}
      else
	{
	  //
	  //	Final argument, symbol not present.
	  //
	  latexPrettyPrint(s, printSettings, coloringInfo, firstArg,
			   UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, true);  // optimistic
	  
	}
    }
  else
    {
      Vector<DagNode*> args;
      for (; a.valid(); a.next())
	args.push_back(a.argument());
      for (int i : si.latexMacroUnpacked)
	{
	  if (i >= 0)
	    s << static_cast<char>(i);
	  else
	    {
	      latexPrettyPrint(s, printSettings, coloringInfo, args[-1 - i],
			       UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, true);  // optimistic
	    }
	}
    }
}
