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
//	DagNode* -> ostream& pretty printer.
//

const char*
MixfixModule::computeColor(ColoringInfo& coloringInfo, DagNode* dagNode)
{
  if (interpreter.getPrintFlag(Interpreter::PRINT_COLOR))
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
MixfixModule::prefix(ostream& s, bool needDisambig, const char* color)
{
  if (needDisambig)
    s << '(';
  if (color != 0)
    s << color;
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
			 const char* color)
{
  if (!(si.symbolType.hasFlag(SymbolType::ITER)))
    return false;
  if (si.symbolType.getBasicType() == SymbolType::SUCC_SYMBOL &&
      interpreter.getPrintFlag(Interpreter::PRINT_NUMBER))
    {
      SuccSymbol* succSymbol = safeCast(SuccSymbol*, dagNode->symbol());
      if (succSymbol->isNat(dagNode))
	{
	  const mpz_class& nat = succSymbol->getNat(dagNode);
	  bool needDisambig = !rangeKnown &&
	    (kindsWithSucc.size() > 1 || overloadedIntegers.count(nat));
	  prefix(s, needDisambig, color);
	  s << nat;
	  suffix(s, dagNode, needDisambig, color);
	  return true;
	}
    }
  S_DagNode* sd = safeCast(S_DagNode*, dagNode);
  const mpz_class& number = sd->getNumber();
  if (number == 1)
    return false;  // do default thing
  
  // NEED TO FIX: disambig
  string prefixName;
  makeIterName(prefixName, dagNode->symbol()->id(), number);
  if (color != 0)
    s << color << prefixName << Tty(Tty::RESET);
  else
    printPrefixName(s, prefixName.c_str(), si);
  s << '(';
  if (interpreter.getPrintFlag(Interpreter::PRINT_COLOR))
    {
      coloringInfo.reducedDirectlyAbove = dagNode->isReduced();
      coloringInfo.reducedAbove = coloringInfo.reducedAbove ||
	coloringInfo.reducedDirectlyAbove;
    }
  prettyPrint(s, coloringInfo, sd->getArgument(),
	      PREFIX_GATHER, UNBOUNDED, 0, UNBOUNDED, 0, rangeKnown);
  s << ')';
  return true;
}

bool
MixfixModule::handleMinus(ostream& s,
			  DagNode* dagNode,
			  bool rangeKnown,
			  const char* color)
{
  if (interpreter.getPrintFlag(Interpreter::PRINT_NUMBER))
    {
      const MinusSymbol* minusSymbol = safeCast(MinusSymbol*, dagNode->symbol());
      if (minusSymbol->isNeg(dagNode))
	{
	  mpz_class neg;
	  (void) minusSymbol->getNeg(dagNode, neg);
	  bool needDisambig = !rangeKnown &&
	    (kindsWithMinus.size() > 1 || overloadedIntegers.count(neg));
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
			     const char* color)
{
  if (interpreter.getPrintFlag(Interpreter::PRINT_RAT))
    {
      const DivisionSymbol* divisionSymbol = safeCast(DivisionSymbol*, dagNode->symbol());
      if (divisionSymbol->isRat(dagNode))
	{
	  pair<mpz_class, mpz_class> rat;
	  rat.second = divisionSymbol->getRat(dagNode, rat.first);
	  bool needDisambig = !rangeKnown &&
	    (kindsWithDivision.size() > 1 || overloadedRationals.count(rat));
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
			  const char* color)
{
  double mfValue = safeCast(FloatDagNode*, dagNode)->getValue();
  bool needDisambig = !rangeKnown &&
    (floatSymbols.size() > 1 || overloadedFloats.count(mfValue));
  prefix(s, needDisambig, color);
  s << doubleToString(mfValue);
  suffix(s, dagNode, needDisambig, color);
}

void
MixfixModule::handleString(ostream& s,
			   DagNode* dagNode,
			   bool rangeKnown,
			   const char* color)
{
  string strValue;
  Token::ropeToString(safeCast(StringDagNode*, dagNode)->getValue(), strValue);
  bool needDisambig = !rangeKnown &&
    (stringSymbols.size() > 1 || overloadedStrings.count(strValue));
  prefix(s, needDisambig, color);
  s << strValue;
  suffix(s, dagNode, needDisambig, color);
}

void
MixfixModule::handleQuotedIdentifier(ostream& s,
				     DagNode* dagNode,
				     bool rangeKnown,
				     const char* color)
{
  int qidCode = safeCast(QuotedIdentifierDagNode*, dagNode)->getIdIndex();
  bool needDisambig = !rangeKnown &&
    (quotedIdentifierSymbols.size() > 1 ||
     overloadedQuotedIdentifiers.count(qidCode));
  prefix(s, needDisambig, color);
  s << '\'' << Token::name(qidCode);
  suffix(s, dagNode, needDisambig, color);
}

void
MixfixModule::handleVariable(ostream& s,
			     DagNode* dagNode,
			     bool rangeKnown,
			     const char* color)
{
  VariableDagNode* v = safeCast(VariableDagNode*, dagNode);
  Sort* sort = safeCast(VariableSymbol*, dagNode->symbol())->getSort();
  pair<int, int> p(v->id(), sort->id());
  bool needDisambig = !rangeKnown && overloadedVariables.count(p);  // kinds not handled
  prefix(s, needDisambig, color);
  printVariable(s, p.first, sort);
  suffix(s, dagNode, needDisambig, color);
}

void
MixfixModule::prettyPrint(ostream& s,
			  ColoringInfo& coloringInfo,
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

  const char* color = computeColor(coloringInfo, dagNode);
  Symbol* symbol = dagNode->symbol();
  SymbolInfo& si = symbolInfo[symbol->getIndexWithinModule()];
  //
  //	Check for special i/o representation.
  //
  if (handleIter(s, coloringInfo, dagNode, si, rangeKnown, color))
    return;
  int basicType = si.symbolType.getBasicType();
  switch (basicType)
    {
    case SymbolType::MINUS_SYMBOL:
      {
	if (handleMinus(s, dagNode, rangeKnown, color))
	  return;
	break;
      }
    case SymbolType::DIVISION_SYMBOL:
      {
	if (handleDivision(s, dagNode, rangeKnown, color))
	  return;
	break;
      }
    case SymbolType::FLOAT:
      {
	handleFloat(s, dagNode, rangeKnown, color);
	return;
      }
    case SymbolType::STRING:
      {
	handleString(s, dagNode, rangeKnown, color);
	return;
      }
    case SymbolType::QUOTED_IDENTIFIER:
      {
	handleQuotedIdentifier(s, dagNode, rangeKnown, color);
	return;
      }
    case SymbolType::VARIABLE:
      {
	handleVariable(s, dagNode, rangeKnown, color);
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
  bool argRangeKnown = !(iflags & ADHOC_OVERLOADED) ||
    (!(iflags & RANGE_OVERLOADED) && (rangeKnown || needDisambig));
  int nrArgs = symbol->arity();
  if (interpreter.getPrintFlag(Interpreter::PRINT_COLOR))
    {
      coloringInfo.reducedDirectlyAbove = dagNode->isReduced();
      coloringInfo.reducedAbove = coloringInfo.reducedAbove ||
	coloringInfo.reducedDirectlyAbove;
    }
  if (needDisambig)
    s << '(';
  bool printConceal = interpreter.concealedSymbol(symbol);
  if ((interpreter.getPrintFlag(Interpreter::PRINT_MIXFIX) && si.mixfixSyntax.length() != 0 && !printConceal) ||
      (basicType == SymbolType::SORT_TEST))
    {
      bool printWithParens = interpreter.getPrintFlag(Interpreter::PRINT_WITH_PARENS);
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
	  pos = printTokens(s, si, pos, color);
	  if (arg == nrArgs - 1 && moreArgs)
	    {
	      ++nrTails;
	      arg = 0;
	      if (needAssocParen)
		s << '(';
	      pos = printTokens(s, si, 0, color);
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
	  prettyPrint(s, coloringInfo, d,
		      si.gather[arg], lc, lcc, rc, rcc,
		      argRangeKnown);
	  if (UserLevelRewritingContext::interrupted())
	    return;
	}
      printTails(s, si, pos, nrTails, needAssocParen, true, color);
      if (UserLevelRewritingContext::interrupted())
	return;
      if (needParen)
	s << ')';
    }
  else
    {
      const char* prefixName = Token::name(symbol->id());
      if (color != 0)
	s << color << prefixName << Tty(Tty::RESET);
      else
	printPrefixName(s, prefixName, si);
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
		      !(interpreter.getPrintFlag(Interpreter::PRINT_FLAT)) &&
		      moreArgs)
		    {
		      ++nrTails;
		      if (color != 0)
			s << color << prefixName << Tty(Tty::RESET);
		      else
			printPrefixName(s, prefixName, si);
		      s << '(';
		    }
		  prettyPrint(s, coloringInfo, d,
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
