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
//	List of all recognized constructors by the meta-interpreter subsystem.
//
//	We generate various chunks of linear (branching) code to handle
//	this hetrogeneous collection by macro expansion.
//	The format is:
//		MACRO(symbols name, symbols C++ class, number of args)
//
  MACRO(stringSymbol, StringSymbol, 0)
  MACRO(emptyInterpereterOptionSetSymbol, FreeSymbol, 0)
  MACRO(newProcessSymbol, FreeSymbol, 0)
  MACRO(interpreterOidSymbol, FreeSymbol, 1)
  MACRO(createInterpreterMsg, FreeSymbol, 3)
  MACRO(createdInterpreterMsg, FreeSymbol, 3)
  MACRO(insertModuleMsg, FreeSymbol, 3)
  MACRO(insertedModuleMsg, FreeSymbol, 2)
  MACRO(showModuleMsg, FreeSymbol, 4)
  MACRO(showingModuleMsg, FreeSymbol, 3)
  MACRO(insertViewMsg, FreeSymbol, 3)
  MACRO(insertedViewMsg, FreeSymbol, 2)
  MACRO(showViewMsg, FreeSymbol, 3)
  MACRO(showingViewMsg, FreeSymbol, 3)
  MACRO(printTermMsg, FreeSymbol, 7)
  MACRO(printedTermMsg, FreeSymbol, 3)
  MACRO(printTermToStringMsg, FreeSymbol, 7)
  MACRO(printedTermToStringMsg, FreeSymbol, 3)
  MACRO(parseTermMsg, FreeSymbol, 6)
  MACRO(parsedTermMsg, FreeSymbol, 3)
  MACRO(getLesserSortsMsg, FreeSymbol, 4)
  MACRO(gotLesserSortsMsg, FreeSymbol, 3)
  MACRO(getMaximalSortsMsg, FreeSymbol, 4)
  MACRO(gotMaximalSortsMsg, FreeSymbol, 3)
  MACRO(getMinimalSortsMsg, FreeSymbol, 4)
  MACRO(gotMinimalSortsMsg, FreeSymbol, 3)
  MACRO(compareTypesMsg, FreeSymbol, 5)
  MACRO(comparedTypesMsg, FreeSymbol, 5)
  MACRO(getKindMsg, FreeSymbol, 4)
  MACRO(gotKindMsg, FreeSymbol, 3)
  MACRO(getKindsMsg, FreeSymbol, 3)
  MACRO(gotKindsMsg, FreeSymbol, 3)
  MACRO(getGlbTypesMsg, FreeSymbol, 4)
  MACRO(gotGlbTypesMsg, FreeSymbol, 3)
  MACRO(getMaximalAritySetMsg, FreeSymbol, 6)
  MACRO(gotMaximalAritySetMsg, FreeSymbol, 3)
  MACRO(normalizeTermMsg, FreeSymbol, 4)
  MACRO(normalizedTermMsg, FreeSymbol, 4)
  MACRO(reduceTermMsg, FreeSymbol, 4)
  MACRO(reducedTermMsg, FreeSymbol, 5)
  MACRO(rewriteTermMsg, FreeSymbol, 5)
  MACRO(rewroteTermMsg, FreeSymbol, 5)
  MACRO(frewriteTermMsg, FreeSymbol, 6)
  MACRO(frewroteTermMsg, FreeSymbol, 5)
  MACRO(erewriteTermMsg, FreeSymbol, 6)
  MACRO(erewroteTermMsg, FreeSymbol, 5)
  MACRO(srewriteTermMsg, FreeSymbol, 7)
  MACRO(srewroteTermMsg, FreeSymbol, 5)
  MACRO(getSearchResultMsg, FreeSymbol, 9)
  MACRO(gotSearchResultMsg, FreeSymbol, 6)
  MACRO(getSearchResultAndPathMsg, FreeSymbol, 9)
  MACRO(gotSearchResultAndPathMsg, FreeSymbol, 7)
  MACRO(applyRuleMsg, FreeSymbol, 7)
  MACRO(appliedRuleMsg, FreeSymbol, 6)
  MACRO(applyRule2Msg, FreeSymbol, 9)
  MACRO(appliedRule2Msg, FreeSymbol, 7)
  MACRO(getMatchMsg, FreeSymbol, 7)
  MACRO(gotMatchMsg, FreeSymbol, 4)
  MACRO(getXmatchMsg, FreeSymbol, 9)
  MACRO(gotXmatchMsg, FreeSymbol, 5)
  MACRO(getUnifierMsg, FreeSymbol, 6)
  MACRO(gotUnifierMsg, FreeSymbol, 4)
  MACRO(getDisjointUnifierMsg, FreeSymbol, 6)
  MACRO(gotDisjointUnifierMsg, FreeSymbol, 5)
  MACRO(getIrredundantUnifierMsg, FreeSymbol, 6)
  MACRO(gotIrredundantUnifierMsg, FreeSymbol, 4)
  MACRO(getIrredundantDisjointUnifierMsg, FreeSymbol, 6)
  MACRO(gotIrredundantDisjointUnifierMsg, FreeSymbol, 5)
  MACRO(getVariantMsg, FreeSymbol, 8)
  MACRO(gotVariantMsg, FreeSymbol, 8)
  MACRO(getVariantUnifierMsg, FreeSymbol, 8)
  MACRO(gotVariantUnifierMsg, FreeSymbol, 5)
  MACRO(getDisjointVariantUnifierMsg, FreeSymbol, 8)
  MACRO(gotDisjointVariantUnifierMsg, FreeSymbol, 6)
  MACRO(getVariantMatcherMsg, FreeSymbol, 8)
  MACRO(gotVariantMatcherMsg, FreeSymbol, 4)
  MACRO(getOneStepNarrowingMsg, FreeSymbol, 8)
  MACRO(gotOneStepNarrowingMsg, FreeSymbol, 10)
  MACRO(getNarrowingSearchResultMsg, FreeSymbol, 10)
  MACRO(gotNarrowingSearchResultMsg, FreeSymbol, 9)
  MACRO(getNarrowingSearchResultAndPathMsg, FreeSymbol, 10)
  MACRO(gotNarrowingSearchResultAndPathMsg, FreeSymbol, 9)
  MACRO(noSuchResultMsg, FreeSymbol, 3)
  MACRO(noSuchResult2Msg, FreeSymbol, 3)
  MACRO(noSuchResult3Msg, FreeSymbol, 4)
  MACRO(quitMsg, FreeSymbol, 2)
  MACRO(byeMsg, FreeSymbol, 2)
  MACRO(interpreterErrorMsg, FreeSymbol, 3)
  MACRO(interpreterExitMsg, FreeSymbol, 3)
