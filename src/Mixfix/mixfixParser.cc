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

void
MixfixParser::makeOtfTranslations()
{
  otfTranslations.clear();
  //
  //	We scan through the original tokens, looking for things that
  //	might be on-the-fly variables of known sort, and make these
  //	into otf translations.
  //
  Index beyondEnd = currentOffset + sentence.size();
  for (Index i = currentOffset; i < beyondEnd; ++i)
    {
      int code = (*currentSentence)[i].code();
      int sp = Token::specialProperty(code);
      if (sp == Token::CONTAINS_COLON)
	{
	  //
	  //	Token looks like X:Bar so it may be an otf
	  //	variable of sort Bar or an otf variable of
	  //	sort Bar{...}...{...}
	  //
	  bool uncertain = (tokenSet.find(code) != NONE) ||
	    (otfTranslations.find(code) != otfTranslations.end());  // maybe X:Bar has some other meaning
	  int varName;
	  int sortName;
	  Token::split(code, varName, sortName);
	  //
	  //	We need to check
	  //	  Bar
	  //	  Bar{...}
	  //	  Bar{...}{...}
	  //	  ...
	  //	If more than one is a valid sort then we are uncertain about all translations,
	  //	but because they will all be added for the same location, it is enough to
	  //	flag at least one as uncertain.
	  //
	  if (Sort* sort = client.findSort(sortName))
	    {
	      //
	      //	Deal with Bar case.
	      //
	      makeOtfTranslation(varName, i, sort->getIndexWithinModule(), uncertain);
	      uncertain = true;  // any addition valid sort flagged as uncertain
	    }
	  //
	  //	Look for Bar {...}...{...}
	  //
	  Vector<Token> structuredSortName;
	  Index start = i + 1;
	  for (;;)
	    {
	      Index last = SharedTokens::skipBracePair(*currentSentence, start, beyondEnd);
	      if (last == NONE)
		break;
	      //
	      //	Saw legal syntax for {...} in parameterized sort.
	      //
	      structuredSortName.resize(last - i + 1);
	      structuredSortName[0].tokenize(sortName, (*currentSentence)[i].lineNumber());
	      for (Index j = start; j <= last; ++j)
		structuredSortName[j - i] = (*currentSentence)[j];
	      int structuredSortCode = Token::bubbleToPrefixNameCode(structuredSortName);

	      if (Sort* sort = client.findSort(structuredSortCode))
		{
		  makeOtfTranslation(varName, last, sort->getIndexWithinModule(), uncertain);
		  uncertain = true;  // any additional valid sort flagged as uncertain
		}
	      start = last + 1;
	    }
	}
      else if (sp == Token::ENDS_IN_COLON)
	{
	  //
	  //	Token looks like X: so it may be an otf variable of kind type.
	  //
	  int varName;
	  int sortName;
	  Token::split(code, varName, sortName);
	  Vector<int> sortNames;
	  Index last = SharedTokens::skipKindName(*currentSentence,
						  i + 1,
						  beyondEnd,
						  sortNames);
	  if (last != NONE)
	    {
	      if (ConnectedComponent* component = checkSortNames(sortNames))
		{
		  Sort* kind = component->sort(Sort::KIND);
		  bool uncertain = (tokenSet.find(code) != NONE) ||
		    (otfTranslations.find(code) != otfTranslations.end());
		  makeOtfTranslation(varName, i, kind->getIndexWithinModule(), uncertain);
		}
	    }
	}
    }
}

void
MixfixParser::makeOtfTranslation(int varName, int location, int sortIndex, bool uncertain)
{
  Vector<OtfDef>& translations = otfTranslations[varName];
  for (const OtfDef& d : translations)
    {
      if (d.sortIndex == sortIndex)
	return;  // saw the same definition earlier
    }
  translations.push_back({location, sortIndex, uncertain});
}

ConnectedComponent* 
MixfixParser::checkSortNames(const Vector<int>& sortNames)
{
  //
  //	Check that sorts exist and are all in the same connected component.
  //
  ConnectedComponent* component = nullptr;
  for (int sortName : sortNames)
    {
      Sort* s = client.findSort(sortName);
      if (s == nullptr)
	return nullptr;
      ConnectedComponent* c = s->component();
      if (component == nullptr)
	component = c;
      else if (component != c)
	return nullptr;
    }
  return component;
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
MixfixParser::extendedParse(int root, int& firstBad, int nrTokens)
{
  makeOtfTranslations();
  terminalLists.clear();
  //
  //	Wildcard variable names are fresh with respect to sentence.
  //
  usedNames.clear();
  //
  //	Translate tokens into terminals.
  //
  Vector<int> translations;
  for (Index i = 0; i < nrTokens; ++i)
    {
      translations.clear();
      Index j = currentOffset + i;
      int code = (*currentSentence)[j].code();
      //
      //	First do standard translation.
      //
      int terminal = tokenSet.find(code);
      if (terminal == NONE)
	terminal = translateSpecialToken(code);
      if (terminal != NONE)
	translations.push_back(terminal);
      //
      //	Then check for otf variable extended scope translations.
      //
      auto t = otfTranslations.find(code);
      if (t != otfTranslations.end())
	{
	  for (const OtfDef& d : t->second)
	    {
	      if (d.location < i)
		{
		  int otfTerminal = bareOtfVariableTerminals[d.sortIndex];
		  translations.push_back(otfTerminal);
		  if (d.uncertain)
		    {
		      Verbose("Inexact parsing triggered by " << Token::name(code) << ":" <<
			      client.getSorts()[d.sortIndex]);
		      inexact = true;
		    }
		}
	      else
		break;  // remaining translations will be later
	    }
	}
      //
      //	Check if we have one or more translations.
      //
      Index nrTranslations = translations.size();
      if (nrTranslations == 1)
	sentence[i] = translations[0];
      else if (nrTranslations > 1)
	{
	  sentence[i] = Parser::flip(terminalLists.size());
	  terminalLists.push_back(std::move(translations));
	}
      else
	{
	  //
	  //	We can't use Token::specialProperty() for starting with '_' because
	  //	is could coincide with other special properties; e.g. _X:Foo
	  //	We only recognize a wildcard if there is no other translation.
	  //	We limit named wildcards to tokens that are valid view names to
	  //	rule out edge cases.
	  //
	  if (wildcardTerminal != NONE &&
	      Token::name(code)[0] == '_' &&
	      Token::isValidViewName(code))
	    sentence[i] = wildcardTerminal;
	  else
	    {
	      firstBad = j;
	      return -1;  // bad token
	    }
	}
    }
  
#if PARSER_DEBUG
  cout << "extended parse: ";
  for (int i = 0; i < sentence.length(); i++)
    cout << sentence[i] << ' ';
  cout << ", " << root << '\n';
#endif

  nrParses = parser.parseSentence(sentence, root, terminalLists);
  DebugAdvisoryCheck(nrParses == 1, "New parser returned " << nrParses << " parses");
  if (nrParses == 0)  // no parse
    firstBad = currentOffset + parser.getErrorPosition();
  else if (inexact)
    {
      //cerr << Tty(Tty::BLUE) << "inexact parse" << Tty(Tty::RESET) << endl;
      //cerr << "nrParses = " << nrParses << endl;
      firstBad = -1;
      if (nrParses == 1)
	{
	  if (!checkParse(firstBad))
	    {
	      nrParses = 0;  // no consistent parses
	    }
	}
      else
	{
	  //
	  //	Ambiguous parse in bigger grammar.
	  //	Find first consistent parse.
	  //
	  while (!checkParse(firstBad))
	    {
	      //cerr << Tty(Tty::BLUE) << "parse failed check" << Tty(Tty::RESET) << endl;
	      if (!parser.extractNextParse())
		{
		  nrParses = 0;  // no consistent parses
		  return nrParses;
		}
	    }
	  //cerr << "found first consistent" << endl;
	  //
	  //	Save our consistent parse and see if there is another one for
	  //	ambiguity in the smaller grammar.
	  //
	  parser.saveParse();
	  while (parser.extractNextParse())
	    {
	      if (checkParse(firstBad))
		{
		  //
		  //	Found a second consistent parse.
		  //	Swap it with the first one.
		  //
		  parser.swapParse();
		  nrParses = 2;  // ambiguous
		  //cerr << "found second consistent" << endl;
		  return nrParses;
		}
	    }
	  //cerr << "no more consistent" << endl;
	  parser.swapParse();  // restore consistent parse
	  nrParses = 1;  // only found a single consistent parse.
	}
    }
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

bool
MixfixParser::checkParse(int& firstBad)
{
  seenSet.clear();
  if (!checkSubparse(ROOT_NODE, firstBad))
    return false;
  return true;
}

bool
MixfixParser::checkSubparse(int node, int& firstBad)
{
  Action& a = actions[parser.getProductionNumber(node)];
  switch (a.action)
    {
    case MAKE_OTF_VARIABLE_KNOWN_SORT:
      {
	int pos = currentOffset + parser.getFirstPosition(node);
	int varName = (*currentSentence)[pos].code();
	int baseName;
	int sortName;
	Token::split(varName, baseName, sortName);
	seenSet.insert({baseName, a.data});
	break;
      }
     case MAKE_OTF_VARIABLE:
      {
	int pos = currentOffset + parser.getFirstPosition(node);
	int varName = (*currentSentence)[pos].code();
	int baseName;
	int sortName;
	Token::split(varName, baseName, sortName);
	Sort* sort = client.findSort(sortName);
	Assert(sort != nullptr, "didn't find sort for " << Token::name(sortName));
	seenSet.insert({baseName, sort->getIndexWithinModule()});
	break;	
      }
    case MAKE_BARE_OTF_VARIABLE:
      {
	int pos = currentOffset + parser.getFirstPosition(node);
	int varName = (*currentSentence)[pos].code();
	if (seenSet.find({varName, a.data}) == seenSet.end())
	  {
	    if (pos > firstBad)
	      firstBad = pos;  // got further
	    return false;
	  }
	break;
      }
    default:
      {
	int nrChildren = parser.getNumberOfChildren(node);
	for (int i = 0; i < nrChildren; ++i)
	  {
	    if (!checkSubparse(parser.getChild(node, i), firstBad))
	      return false;
	  }
      }
    }
  return true;
}
