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
//      Implementation for class MixfixParser.
//
#define PARSER_DEBUG 0

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "flagSet.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "S_Theory.hh"
#include "NA_Theory.hh"
#include "builtIn.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"
#include "SMT.hh"

//      interface class definitions
#include "term.hh"

//      core class definitions
#include "equation.hh"
#include "rule.hh"
#include "sortConstraint.hh"
#include "rewriteStrategy.hh"
#include "strategyDefinition.hh"
#include "conditionFragment.hh"

//	variable class definitions
#include "variableSymbol.hh"
#include "variableTerm.hh"

//	ACU theory class definitions
#include "ACU_Symbol.hh"

//	S theory class definitions
#include "S_Symbol.hh"
#include "S_Term.hh"

//	builtin class definitions
#include "floatTerm.hh"
#include "floatSymbol.hh"
#include "stringSymbol.hh"
#include "stringTerm.hh"
#include "succSymbol.hh"
#include "minusSymbol.hh"
#include "divisionSymbol.hh"

//	SMT class definitions
#include "SMT_NumberSymbol.hh"
#include "SMT_NumberTerm.hh"

//	higher class definitions
#include "equalityConditionFragment.hh"
#include "sortTestConditionFragment.hh"
#include "assignmentConditionFragment.hh"
#include "rewriteConditionFragment.hh"

//	strategy languages class definitions
#include "trivialStrategy.hh"
#include "applicationStrategy.hh"
#include "concatenationStrategy.hh"
#include "unionStrategy.hh"
#include "iterationStrategy.hh"
#include "branchStrategy.hh"
#include "testStrategy.hh"
#include "subtermStrategy.hh"
#include "callStrategy.hh"
#include "oneStrategy.hh"

//	front end class definitions
#include "mixfixModule.hh"
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierTerm.hh"
#include "objectConstructorSymbol.hh"
#include "mixfixParser.hh"

#define ROOT_NODE	(0)

//	our stuff
#include "extendedParse.cc"
#include "makeParse.cc"

MixfixParser::MixfixParser(MixfixModule& client,
			   bool complexFlag,
			   int componentNonTerminalBase,
			   int numberOfTypes,
			   int nextNonTerminalCode,
			   int nrTokensGuess)
  : client(client),
    complexParser(complexFlag),
    componentNonTerminalBase(componentNonTerminalBase),
    numberOfTypes(numberOfTypes),
    tokenSet(nrTokensGuess),
    specialTerminals(Token::LAST_PROPERTY),
    componentTerminals(client.getConnectedComponents().size()),
    bareOtfVariableTerminals(client.getSorts().size())
{
  nextNonTerminal = nextNonTerminalCode;
  bubblesAllowed = false;
}

MixfixParser::~MixfixParser()
{
  DebugInfo("this = " << this);
}

void
MixfixParser::insertProduction(int lhs,
			       const Vector<int>& rhs,
			       int prec,
			       const Vector<int>& gather,
			       int action,
			       int data,
			       int data2)
{
  int rhsLength = rhs.length();
  productionRhs.resize(rhsLength);
  int ntCount = 0;
  for (int i = 0; i < rhsLength; i++)
    {
      int s = rhs[i];
      if (s < 0)
	ntCount++;
      productionRhs[i] = s < 0 ? s : tokenToIndex(s);
    }
  
#ifndef NO_ASSERT
  int gatherSize = gather.size();
  if (ntCount != gatherSize)
    {
      cout << "production: " << lhs << " ::= ";
      for (int i = 0; i < productionRhs.length(); i++)
	cout << productionRhs[i] << ' ';
      cout << "\t\tprec: " << prec << "\tgather: ";
      for (int i = 0; i < gatherSize; i++)
	cout << gather[i] << ' ';
      cout << "(action = " << action << ", data = " << data << ", data2 = " << data2 <<")\n";
    }
#endif

  parser.insertProd(lhs, productionRhs, prec, gather);
  int nrActions = actions.length();
  actions.expandBy(1);
  Action& a = actions[nrActions];
  a.action = action;
  a.data = data;
  a.data2 = data2;
}

void
MixfixParser::insertBubbleProduction(int lhs,
				     int lowerBound,
				     int upperBound,
				     int leftParenCode,
				     int rightParenCode,
				     const Vector<int>& excluded,
				     int bubbleSpecIndex)
{
  int left = (leftParenCode >= 0) ? tokenToIndex(leftParenCode) : NONE;
  int right = (rightParenCode >= 0) ? tokenToIndex(rightParenCode) : NONE;
  int nrExcluded = excluded.length();
  Vector<int> excludedTerminals(nrExcluded);
  for (int i = 0; i < nrExcluded; i++)
    excludedTerminals[i]= tokenToIndex(excluded[i]);

#if PARSER_DEBUG
  cout << "insertProd(" << lhs <<
    ", " << lowerBound << ", " << upperBound << ", "
       << left << ", " << right << ", ";
  for (int i = 0; i < nrExcluded; i++)
    cout << excludedTerminals[i] << ' ';
  cout << ")\n";
#endif

  parser.insertProd(lhs, lowerBound, upperBound, left, right, excludedTerminals);
  int nrActions = actions.length();
  actions.expandBy(1);
  Action& a = actions[nrActions];
  a.action = MAKE_BUBBLE;
  a.data = bubbleSpecIndex;
  a.data2 = NONE;
  bubblesAllowed = true;
}

int
MixfixParser::translateSpecialToken(int code)
{
  int sp = Token::specialProperty(code);
  if (sp == Token::CONTAINS_COLON)
    {
      //
      //	We have an unrecognized token that looks like X:Foo
      //
      int varName;
      int sortName;
      Token::split(code, varName, sortName);
      //
      //	If :Foo has its own lead terminal because Foo{...}
      //	is a sort, use that.
      //
      auto i = leadTerminals.find(sortName);
      if (i != leadTerminals.end())
	return i->second;
      //
      //	Otherwise if Foo is a sort, use its component terminal.
      //
      if (Sort* sort = client.findSort(sortName))
	return componentTerminals[sort->component()->getIndexWithinModule()];
    }
  else if (sp == Token::ITER_SYMBOL)
    {
      int opName;
      mpz_class dummy;
      Token::split(code, opName, dummy);
      auto i = iterSymbolTerminals.find(opName);
      if (i != iterSymbolTerminals.end())
	return i->second;
    }
  else if (sp != NONE)
    return specialTerminals[sp];
  //
  //	If we're parsing with bubbles, they can take anything, so we map otherwise
  //	unrecognized tokens to a special out-of-band value.
  //
  if (bubblesAllowed)
    return tokenSet.size();
  return NONE;
}

int
MixfixParser::classicParse(int root, int& firstBad, int nrTokens)
{
  //
  //	Parse using classic (Maude version <= 3.5.1) conventions.
  //	Return value is:
  //	  -1 : bad token at position firstBad
  //	  0  : tokens good, but parse fails at position firstBad
  //	  1  : exactly one parse
  //	  2  : two or more parses - ambiguous
  //
  //	Translate tokens into terminals.
  //
  for (Index i = 0; i < nrTokens; ++i)
    {
      Index j = currentOffset + i;
      int code = (*currentSentence)[j].code();
      int terminal = tokenSet.find(code);
      if (terminal == NONE)
	{
	  terminal = translateSpecialToken(code);
	  if (terminal == NONE)
	    {
	      firstBad = j;
	      return -1;  // bad token
	    }
	}
      sentence[i] = terminal;
    }
  
#if PARSER_DEBUG
  cout << "classic parse: ";
  for (int i = 0; i < sentence.length(); i++)
    cout << sentence[i] << ' ';
  cout << ", " << root << '\n';
#endif

  nrParses = parser.parseSentence(sentence, root);
  DebugAdvisoryCheck(nrParses == 1, "New parser returned " << nrParses << " parses");
  if (nrParses == 0)  // no parse
    firstBad = currentOffset + parser.getErrorPosition();
  
#if PARSER_DEBUG
  parser.printCurrentParse();
#endif
  
  return nrParses;
}

int
MixfixParser::parseSentence(const Vector<Token>& original,
			    int root,
			    int& firstBad,
			    int begin,
			    int nrTokens)
{
  currentSentence = &original;
  currentOffset = begin;
  sentence.resize(nrTokens);
  inexact = false;

  if (classicParse(root, firstBad, nrTokens) > 0 || bubblesAllowed)
    return nrParses;
  return extendedParse(root, firstBad, nrTokens);
}
