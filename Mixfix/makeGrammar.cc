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
//	Functions for making a Maude grammar from a signature.
//

void
MixfixModule::makeGrammar(bool complexFlag)
{
  if (parser != 0)
    {
      if (!complexFlag || complexParser)
	return;
      delete parser;
    }

  parser = new MixfixParser(*this);
  complexParser = complexFlag;
  componentNonTerminalBase = (complexParser ? COMPLEX_BASE : SIMPLE_BASE) -
    iterSymbols.cardinality();
  if (complexFlag)
    makeComplexProductions();
  makeStatementProductions();
  makeConditionProductions();
  makeAttributeProductions();
  makeComponentProductions();
  makeSymbolProductions();
  makeVariableProductions();
  makeBoolProductions();
  makeLabelProductions();
  makeSpecialProductions();
  makePolymorphProductions();
  makeBubbleProductions();
}

void
MixfixModule::makeComplexProductions()
{
#if PARSER_DEBUG
  cout << "<Complex productions>\n";
#endif

  static Vector<int> rhs(3);

  rhs.contractTo(1);
  rhs[0] = arrowOne;
  parser->insertProduction(SEARCH_CONNECTIVE, rhs, 0, gatherAny,
			   MixfixParser::NOP, RewriteSequenceSearch::ONE_STEP);
  rhs[0] = arrowPlus;
  parser->insertProduction(SEARCH_CONNECTIVE, rhs, 0, gatherAny,
			   MixfixParser::NOP, RewriteSequenceSearch::AT_LEAST_ONE_STEP);
  rhs[0] = arrowStar;
  parser->insertProduction(SEARCH_CONNECTIVE, rhs, 0, gatherAny, 
			   MixfixParser::NOP, RewriteSequenceSearch::ANY_STEPS);
  rhs[0] = arrowBang;
  parser->insertProduction(SEARCH_CONNECTIVE, rhs, 0, gatherAny,
			   MixfixParser::NOP, RewriteSequenceSearch::NORMAL_FORM);

  rhs[0] = MATCH_PAIR;
  parser->insertProduction(MATCH_COMMAND, rhs, 0, gatherAny);
  rhs[0] = SEARCH_PAIR;
  parser->insertProduction(SEARCH_COMMAND, rhs, 0, gatherAny);

  rhs[0] = suchThat;
  parser->insertProduction(SUCH_THAT, rhs, 0, gatherAny);
  rhs.expandTo(2);
  rhs[0] = such;
  rhs[1] = that;
  parser->insertProduction(SUCH_THAT, rhs, 0, gatherAnyAny);

  rhs.expandTo(3);
  rhs[0] = MATCH_PAIR;
  rhs[1] = SUCH_THAT;
  rhs[2] = CONDITION;
  parser->insertProduction(MATCH_COMMAND, rhs, 0, gatherAnyAnyAny,
			   MixfixParser::CONDITIONAL_COMMAND);
  rhs[0] = SEARCH_PAIR;
  parser->insertProduction(SEARCH_COMMAND, rhs, 0, gatherAnyAnyAny,
			   MixfixParser::CONDITIONAL_COMMAND);
}

void
MixfixModule::makeAttributeProductions()
{
#if PARSER_DEBUG
  cout << "<Attribute productions>\n";
#endif

  static Vector<int> rhs(3);

  rhs.resize(3);
  rhs[0] = leftBracket;
  rhs[1] = ATTRIBUTE_LIST;
  rhs[2] = rightBracket;
  parser->insertProduction(ATTRIBUTE_PART, rhs, 0, gatherAny);
  //
  //	Attribute lists.
  //
  rhs.resize(1);
  rhs[0] = ATTRIBUTE;
  parser->insertProduction(ATTRIBUTE_LIST, rhs, 0, gatherAny);
  rhs.resize(2);
  rhs[1] = ATTRIBUTE_LIST;
  parser->insertProduction(ATTRIBUTE_LIST, rhs, 0, gatherAnyAny,
			   MixfixParser::MAKE_ATTRIBUTE_LIST);
  //
  //	Specific attributes.
  //
  rhs.resize(2);
  rhs[0] = label;
  rhs[1] = LABEL;
  parser->insertProduction(ATTRIBUTE, rhs, 0, gatherAny, MixfixParser::MAKE_LABEL_ATTRIBUTE);
  rhs[0] = metadata;
  rhs[1] = STRING_NT;
  parser->insertProduction(ATTRIBUTE, rhs, 0, gatherAny,
			   MixfixParser::MAKE_METADATA_ATTRIBUTE);
  rhs.resize(1);
  rhs[0] = nonexec;
  parser->insertProduction(ATTRIBUTE, rhs, 0, emptyGather,
			   MixfixParser::MAKE_NONEXEC_ATTRIBUTE);
  rhs[0] = otherwise;
  parser->insertProduction(ATTRIBUTE, rhs, 0, emptyGather,
			   MixfixParser::MAKE_OWISE_ATTRIBUTE);
  rhs[0] = owise;
  parser->insertProduction(ATTRIBUTE, rhs, 0, emptyGather,
			   MixfixParser::MAKE_OWISE_ATTRIBUTE);
}

void
MixfixModule::makeStatementProductions()
{
#if PARSER_DEBUG
  cout << "<Statement productions>\n";
#endif

  static Vector<int> rhs(5);
  //
  //	Statements with and without attributes.
  //
  rhs.resize(1);
  rhs[0] = STATEMENT_PART;
  parser->insertProduction(STATEMENT, rhs, 0, gatherAny);
  rhs.resize(2);
  rhs[1] = ATTRIBUTE_PART;
  parser->insertProduction(STATEMENT, rhs, 0, gatherAnyAny, MixfixParser::MAKE_ATTRIBUTE_PART);
  //
  //	Unconditional statement parts.
  //
  rhs.resize(2);
  rhs[0] = mb;
  rhs[1] = MB_BODY;
  parser->insertProduction(STATEMENT_PART, rhs, 0, gatherAny, MixfixParser::MAKE_MB);
  
  rhs[0] = eq;
  rhs[1] = EQ_BODY;
  parser->insertProduction(STATEMENT_PART, rhs, 0, gatherAny, MixfixParser::MAKE_EQ);

  rhs[0] = rl;
  rhs[1] = RL_BODY;
  parser->insertProduction(STATEMENT_PART, rhs, 0, gatherAny, MixfixParser::MAKE_RL);
  //
  //	Conditional statement parts.
  //
  rhs.resize(4);
  rhs[2] = ifToken;
  rhs[3] = CONDITION;

  rhs[0] = cmb;
  rhs[1] = MB_BODY;
  parser->insertProduction(STATEMENT_PART, rhs, 0, gatherAnyAny, MixfixParser::MAKE_CMB);
  
  rhs[0] = ceq;
  rhs[1] = EQ_BODY;
  parser->insertProduction(STATEMENT_PART, rhs, 0, gatherAnyAny, MixfixParser::MAKE_CEQ);

  rhs[0] = cq;
  rhs[1] = EQ_BODY;
  parser->insertProduction(STATEMENT_PART, rhs, 0, gatherAnyAny, MixfixParser::MAKE_CEQ);

  rhs[0] = crl;
  rhs[1] = RL_BODY;
  rhs[3] = RULE_CONDITION;
  parser->insertProduction(STATEMENT_PART, rhs, 0, gatherAnyAny, MixfixParser::MAKE_CRL);
  //
  //	Statement bodies without labels.
  //
  rhs.resize(1);
  rhs[0] = COLON_PAIR;
  parser->insertProduction(MB_BODY, rhs, 0, gatherAny);

  rhs[0] = EQUALITY_PAIR;
  parser->insertProduction(EQ_BODY, rhs, 0, gatherAny);

  rhs[0] = ARROW_PAIR;
  parser->insertProduction(RL_BODY, rhs, 0, gatherAny);
  //
  //	Statement bodies with labels.
  //
  rhs.resize(1);
  rhs.expandTo(5);
  rhs[0] = leftBracket;
  rhs[1] = LABEL;
  rhs[2] = rightBracket;
  rhs[3] = colon;

  rhs[4] = COLON_PAIR;
  parser->insertProduction(MB_BODY, rhs, 0, gatherAnyAny, MixfixParser::MAKE_LABEL);

  rhs[4] = EQUALITY_PAIR;
  parser->insertProduction(EQ_BODY, rhs, 0, gatherAnyAny, MixfixParser::MAKE_LABEL);

  rhs[4] = ARROW_PAIR;
  parser->insertProduction(RL_BODY, rhs, 0, gatherAnyAny, MixfixParser::MAKE_LABEL);
}

void
MixfixModule::makeConditionProductions()
{
#if PARSER_DEBUG
  cout << "<Condition productions>\n";
#endif

  static Vector<int> rhs(5);
  //
  //	Condition fragments.
  //
  rhs.resize(1);
  rhs[0] = EQUALITY_PAIR;
  parser->insertProduction(CONDITION_FRAGMENT, rhs, 0, gatherAny, MixfixParser::MAKE_EQUALITY);
  parser->insertProduction(RULE_CONDITION_FRAGMENT, rhs, 0, gatherAny, MixfixParser::MAKE_EQUALITY);
  rhs[0] = COLON_PAIR;
  parser->insertProduction(CONDITION_FRAGMENT, rhs, 0, gatherAny, MixfixParser::MAKE_SORT_TEST);
  parser->insertProduction(RULE_CONDITION_FRAGMENT, rhs, 0, gatherAny, MixfixParser::MAKE_SORT_TEST);
  rhs[0] = ASSIGN_PAIR;
  parser->insertProduction(CONDITION_FRAGMENT, rhs, 0, gatherAny, MixfixParser::MAKE_ASSIGNMENT);
  parser->insertProduction(RULE_CONDITION_FRAGMENT, rhs, 0, gatherAny, MixfixParser::MAKE_ASSIGNMENT);
  rhs[0] = ARROW_PAIR;
  parser->insertProduction(RULE_CONDITION_FRAGMENT, rhs, 0, gatherAny, MixfixParser::MAKE_REWRITE);
  //
  //	Simple conditions.
  //
  rhs[0] = CONDITION_FRAGMENT;
  parser->insertProduction(CONDITION, rhs, 0, gatherAny);
  rhs[0] = RULE_CONDITION_FRAGMENT;
  parser->insertProduction(RULE_CONDITION, rhs, 0, gatherAny);
  //
  //	Complex conditions.
  //
  rhs.resize(3);
  rhs[0] = CONDITION_FRAGMENT;
  rhs[1] = wedge;
  rhs[2] = CONDITION;
  parser->insertProduction(CONDITION, rhs, 0, gatherAnyAny, MixfixParser::FRAGMENT_LIST);
  rhs[0] = RULE_CONDITION_FRAGMENT;
  rhs[2] = RULE_CONDITION;
  parser->insertProduction(RULE_CONDITION, rhs, 0, gatherAnyAny, MixfixParser::FRAGMENT_LIST);
}

void
MixfixModule::makeComponentProductions()
{
#if PARSER_DEBUG
  cout << "<Component productions>\n";
#endif

  static Vector<int> rhsOne(1);
  static Vector<int> rhsPair(3);
  static Vector<int> rhsParens(3);
  static Vector<int> rhsSort(5);
  static Vector<int> rhsKind(7);
  static Vector<int> rhsKindVariable(4);
  static Vector<int> rhsDotSort(4);
  static Vector<int> rhsList(3);

  rhsParens[0] = leftParen;
  rhsParens[2] = rightParen;
  rhsSort[0] = leftParen;
  rhsSort[2] = rightParen;
  rhsSort[3] = dot;

  rhsKind[0] = leftParen;
  rhsKind[2] = rightParen;
  rhsKind[3] = dot;
  rhsKind[4] = leftBracket;
  rhsKind[6] = rightBracket;

  rhsKindVariable[0] = ENDS_IN_COLON_NT;
  rhsKindVariable[1] = leftBracket;
  rhsKindVariable[3] = rightBracket;

  rhsDotSort[0] = leftParen;
  rhsDotSort[2] = rightParen;
  rhsList[1] = comma;

  const Vector<ConnectedComponent*>& components = getConnectedComponents();
  int nrComponents = components.length();
  for (int i = 0; i < nrComponents; i++)
    {
      int termNt = nonTerminal(i, TERM_TYPE);
      int sortNt = nonTerminal(i, SORT_TYPE);
      int dotSortNt = nonTerminal(i, DOT_SORT_TYPE);
      int assocListNt = nonTerminal(i, ASSOC_LIST_TYPE);
      int sortListNt = nonTerminal(i, SORT_LIST_TYPE);
      const ConnectedComponent* component = components[i];
      int nrSorts = component->nrSorts();
      //variableTokenCode.expandTo(getSorts().length());
      for (int j = 1; j < nrSorts; j++)  // skip error sort
	{
	  const Sort* sort = component->sort(j);
	  int sortNameCode = sort->id();
	  int sortIndex = sort->getIndexWithinModule();
	  //
	  //	Terminals for sorts and dotted sorts.
	  //
	  rhsOne[0] = sortNameCode;
	  parser->insertProduction(sortNt, rhsOne, 0, emptyGather,
				   MixfixParser::NOP, sortIndex);  // CHECK sort index needed?
	  rhsOne[0] = Token::dotNameCode(sortNameCode);
	  parser->insertProduction(dotSortNt, rhsOne, 0, emptyGather);
	  //
	  //	Terminal for as yet unseen variables of our sort.
	  //
	  string sortName(Token::name(sortNameCode));
	  int t = Token::encode((sortName + " variable").c_str());
	  rhsOne[0] = t;
	  //cout << "HERE t = " << t << "\n";
	  parser->insertProduction(termNt, rhsOne, 0, emptyGather,
				   MixfixParser::MAKE_VARIABLE, sortIndex);
	  parser->insertVariableTerminal(sortIndex, t);
	  //cout << "END\n";
	}
      //
      //	Syntax for on the fly kind variable:
      //	<FooTerm> ::= <ENDS_IN_COLON> [ <FooSortList> ]
      //
      int sortIndex = component->sort(Sort::ERROR_SORT)->getIndexWithinModule();
      rhsKindVariable[2] = sortListNt;
      parser->insertProduction(termNt, rhsKindVariable, 0, gatherAnyAny,
			       MixfixParser::MAKE_VARIABLE, sortIndex);
      //
      //	Syntax for term from unknown component:
      //	<TERM> ::= <FooTerm>
      //
      if (!(bubbleComponents.contains(i)))
	{
	  rhsOne[0] = termNt;
	  parser->insertProduction(TERM, rhsOne, 0, gatherAny, MixfixParser::PASS_THRU);
	}
      //
      //	Syntax for equality, assign and arrow pairs:
      //	<EQUALITY_PAIR> ::= <FooTerm> = <FooTerm>
      //	<ASSIGN_PAIR> ::= <FooTerm> := <FooTerm>
      //	<ARROW_PAIR> ::= <FooTerm> => <FooTerm>
      //
      rhsPair[0] = rhsPair[2] = termNt;
      rhsPair[1] = equals;
      parser->insertProduction(EQUALITY_PAIR, rhsPair, 0, gatherAnyAny);
      rhsPair[1] = assign;
      parser->insertProduction(ASSIGN_PAIR, rhsPair, 0, gatherAnyAny);
      rhsPair[1] = arrow;
      parser->insertProduction(ARROW_PAIR, rhsPair, 0, gatherAnyAny);
      if (complexParser)
	{
	  //
	  //	Syntax for match and search pairs:
	  //	<MATCH_PAIR> ::= <FooTerm> <=? <FooTerm>
	  //	<SEARCH_PAIR> ::= <FooTerm> <SEARCH_CONNECTIVE> <FooTerm>
	  //
	  rhsPair[1] = Token::encode("<=?");
	  parser->insertProduction(MATCH_PAIR, rhsPair, 0, gatherAnyAny);
	  rhsPair[1] = SEARCH_CONNECTIVE;
	  parser->insertProduction(SEARCH_PAIR, rhsPair, 0, gatherAnyAnyAny);
	}
      //
      //	Syntax for colon pairs:
      //	<COLON_PAIR> ::= <FooTerm> : <FooSort>
      //	<COLON2_PAIR> ::= <FooTerm> :: <FooSort>
      //	<COLON3_PAIR> ::= <FooTerm> :: <FooSort>
      //
      rhsPair[2] = sortNt;
      rhsPair[1] = colon;
      parser->insertProduction(COLON_PAIR, rhsPair, 0, gatherAny0);
      rhsPair[1] = colon2;
      parser->insertProduction(COLON2_PAIR, rhsPair, 0, gatherAny0);
      rhsPair[1] = colon3;
      parser->insertProduction(COLON3_PAIR, rhsPair, 0, gatherAny0);
      //
      //	Syntax for parentheses:
      //	<FooTerm> ::= ( <FooTerm> )
      //
      if (!(bubbleComponents.contains(i)))
	{
	  rhsParens[1] = termNt;
	  parser->insertProduction(termNt, rhsParens, 0, gatherAny, MixfixParser::PASS_THRU);
	}
      //
      //	Syntax for sort disambiguation:
      //	<FooTerm> ::= ( <FooTerm> ) . <FooSort>
      //	<FooTerm> ::= ( <FooTerm> ) . [ <FooSortList> ]
      //	<FooTerm> ::= ( <FooTerm> ) <FooDotSort>
      //
      rhsSort[1] = termNt;
      rhsSort[4] = sortNt;
      parser->insertProduction(termNt, rhsSort, 0, gatherAny0, MixfixParser::PASS_THRU);
      rhsKind[1] = termNt;
      rhsKind[5] = sortListNt;
      parser->insertProduction(termNt, rhsKind, 0, gatherAnyAny, MixfixParser::PASS_THRU);
      rhsDotSort[1] = termNt;
      rhsDotSort[3] = dotSortNt;
      parser->insertProduction(termNt, rhsDotSort, 0, gatherAny0, MixfixParser::PASS_THRU);
      //
      //	Syntax for sort lists:
      //	<FooSortList> ::= <FooSort>
      //	<FooSortList> ::= <FooSortList> , <FooSort>
      //
      rhsOne[0] = sortNt;
      parser->insertProduction(sortListNt, rhsOne, 0, gatherAny);
      rhsList[0] = sortListNt;
      rhsList[2] = sortNt;
      parser->insertProduction(sortListNt, rhsList, 0, gatherAnyAny);
      //
      //	Syntax for flattened assoc arg lists:
      //	<FooAssocList> ::= <FooTerm> , <FooTerm>
      //	<FooAssocList> ::= <FooAssocList> , <FooTerm>
      //
      rhsList[0] = termNt;
      rhsList[2] = termNt;
      parser->insertProduction(assocListNt, rhsList, PREFIX_GATHER, gatherPrefixPrefix,
			       MixfixParser::ASSOC_LIST);
      //      rhsList[2] = assocListNt;
      rhsList[0] = assocListNt;
      parser->insertProduction(assocListNt, rhsList, PREFIX_GATHER, gatherPrefixPrefix,
			       MixfixParser::ASSOC_LIST);
    }
}

void
MixfixModule::makeSymbolProductions()
{
#if PARSER_DEBUG
  cout << "<Symbol productions>\n";
#endif

  Vector<int> rhs;
  Vector<int> gather;
  const Vector<Symbol*>& symbols = getSymbols();
  int nrSymbols = symbols.length();
  for (int i = 0; i < nrSymbols; i++)
    {
      Symbol* symbol = symbols[i];
      SymbolInfo& si = symbolInfo[i];
      if (si.symbolType.isCreatedOnTheFly())
	continue;  // created-on-the-fly symbol
      int rangeNt = nonTerminal(symbol->rangeComponent()->getIndexWithinModule(), TERM_TYPE);
      //
      //	First make syntax for prefix form.
      //
      rhs.resize(1);
      rhs[0] = symbol->id();
      int nrArgs = symbol->arity();
      if (nrArgs == 0)
	{
	  switch (si.symbolType.getBasicType())
	    {
	    case SymbolType::FLOAT:
	      {
		rhs[0] = FLOAT_NT;
		parser->insertProduction(rangeNt, rhs, 0, gatherAny, MixfixParser::MAKE_FLOAT, i);
		break;
	      }
	    case SymbolType::QUOTED_IDENTIFIER:
	      {
		rhs[0] = QUOTED_ID;
		parser->insertProduction(rangeNt, rhs, 0, gatherAny, MixfixParser::MAKE_QUOTED_IDENTIFIER, i);
		break;
	      }
	    case SymbolType::STRING:
	      {
		rhs[0] = STRING_NT;
		parser->insertProduction(rangeNt, rhs, 0, gatherAny, MixfixParser::MAKE_STRING, i);
		break;
	      }
	    default:
	      {
		parser->insertProduction(rangeNt, rhs, 0, emptyGather, MixfixParser::MAKE_TERM, i);
		break;
	      }
	    }
	}
      else if (si.symbolType.hasFlag(SymbolType::ASSOC))
	{
	  rhs.append(leftParen);
	  rhs.append(nonTerminal(domainComponentIndex(symbol, 0), ASSOC_LIST_TYPE));
	  rhs.append(rightParen);
	  parser->insertProduction(rangeNt, rhs, 0, gatherPrefix, MixfixParser::MAKE_TERM, i);
	}
      else
	{
	  rhs.append(leftParen);
	  gather.resize(0);
	  for (int j = 0; j < nrArgs; j++)
	    {
	      gather.append(PREFIX_GATHER);
	      rhs.append(nonTerminal(domainComponentIndex(symbol, j), TERM_TYPE));
	      rhs.append(j == nrArgs - 1 ? rightParen : comma);
	    }
	  parser->insertProduction(rangeNt, rhs, 0, gather, MixfixParser::MAKE_TERM, i);
	  //
	  //	If symbol has the iter attribute, add syntax
	  //	<rangeTerm> ::= <fooIterSymbol> ( <rangeTerm> )
	  //	to handle foo^n(t) case.
	  //
	  if (si.symbolType.hasFlag(SymbolType::ITER))
	    {
	      rhs.resize(0);
	      rhs.append(iterSymbolNonTerminal(iterSymbols.int2Index(symbol->id())));
	      rhs.append(leftParen);
	      rhs.append(rangeNt);
	      rhs.append(rightParen);
	      gather.resize(0);
	      gather.append(PREFIX_GATHER);
	      gather.append(PREFIX_GATHER);
	      parser->insertProduction(rangeNt, rhs, 0, gather, MixfixParser::MAKE_ITER, i);
	      //
	      //	If symbl is a successor symbol, add the syntax
	      //	<rangeTerm> ::= SMALL_NAT
	      //	to handle decimal natural number syntax.
	      //
	      if (si.symbolType.getBasicType() == SymbolType::SUCC_SYMBOL)
		{
		  rhs.resize(1);
		  rhs[0] = SMALL_NAT;
		  parser->insertProduction(rangeNt, rhs, 0, gatherAny, MixfixParser::MAKE_NATURAL, i);
		}
	    }
	  else if (si.symbolType.getBasicType() == SymbolType::MINUS_SYMBOL)
	    {
	      rhs.resize(1);
	      rhs[0] = SMALL_NEG;
	      parser->insertProduction(rangeNt, rhs, 0, gatherAny, MixfixParser::MAKE_INTEGER, i);
	    }
	  else if (si.symbolType.getBasicType() == SymbolType::DIVISION_SYMBOL)
	    {
	      rhs.resize(1);
	      rhs[0] = RATIONAL;
	      parser->insertProduction(rangeNt, rhs, 0, gatherAny, MixfixParser::MAKE_RATIONAL, i);
	    }
	}

      int nrItems = si.mixfixSyntax.length();
      if (nrItems > 0)
	{
	  //
	  //	Make syntax for mixfix form.
	  //
	  rhs.resize(nrItems);
	  int k = 0;
	  for (int j = 0; j < nrItems; j++)
	    {
	      int t = si.mixfixSyntax[j];
	      if (t == underscore)
		{
		  rhs[j] = nonTerminal(domainComponentIndex(symbol, k), TERM_TYPE);
		  ++k;
		}
	      else
		rhs[j] = t;
	    }
	  parser->insertProduction(rangeNt, rhs, si.prec, si.gather, MixfixParser::MAKE_TERM, i);
	}
    }
}

void
MixfixModule::makeVariableProductions()
{
#if PARSER_DEBUG
  cout << "<variable productions>\n";
#endif

  static Vector<int> rhs(1);
  for (AliasMap::const_iterator i = variableAliases.begin(); i != variableAliases.end(); ++i)
    {
      rhs[0] = (*i).first;
      Sort* sort = (*i).second;
      int termNt = nonTerminal(sort->component()->getIndexWithinModule(), TERM_TYPE);
      parser->insertProduction(termNt, rhs, 0, emptyGather,
			       MixfixParser::MAKE_VARIABLE_FROM_ALIAS, sort->getIndexWithinModule());
    }
}

void
MixfixModule::makeBoolProductions()
{
  static Vector<int> rhs(1);
#if PARSER_DEBUG
  cout << "<Bool productions>\n";
#endif

  if (trueSymbol != 0)
    {
      int rangeNt = nonTerminal(boolSort->component()->getIndexWithinModule(), TERM_TYPE);
      //
      //	Add extra syntactic sugar to allow a condition fragment to just be a bool.
      //
      rhs[0] = rangeNt;
      parser->insertProduction(CONDITION_FRAGMENT, rhs, 0, gatherAny, MixfixParser::MAKE_TRUE);
      parser->insertProduction(RULE_CONDITION_FRAGMENT, rhs, 0, gatherAny, MixfixParser::MAKE_TRUE);

      if (falseSymbol != 0)
	{
	  //
	  //	Add syntax for sort test operators
	  //
	  rhs[0] = COLON2_PAIR;
	  parser->insertProduction(rangeNt, rhs, 0, gatherAny, MixfixParser::MAKE_SORT_TEST, 1);
	  rhs[0] = COLON3_PAIR;
	  parser->insertProduction(rangeNt, rhs, 0, gatherAny, MixfixParser::MAKE_SORT_TEST, 0);
	}
    }
}

void
MixfixModule::makeLabelProductions()
{
#if PARSER_DEBUG
  cout << "<Label productions>\n";
#endif

  static Vector<int> rhs(1);
  int nrLabels = labels.cardinality();
  for (int i = 0; i < nrLabels; i++)
    {
      int label = labels.index2Int(i);
      rhs[0] = label;
      parser->insertProduction(LABEL, rhs, 0, emptyGather, MixfixParser::MAKE_LABEL, label);
    }
}

void
MixfixModule::makeSpecialProductions()
{
#if PARSER_DEBUG
  cout << "<Special productions>\n";
#endif

  static Vector<int> rhs(1);
  //
  //	Productions for existing tokens that have special properties.
  //
  const IntSet& tokens = parser->getTokenSet();  // HACK
  int nrTokens = tokens.cardinality();
  for (int i = 0; i < nrTokens; i++)
    { 
      int code = tokens.index2Int(i);
      switch (Token::specialProperty(code))
	{
	case Token::SMALL_NAT:
	  {
	    rhs[0] = code;
	    parser->insertProduction(SMALL_NAT, rhs, 0, emptyGather);
	    break;
	  }
	case Token::ZERO:
	  {
	    rhs[0] = code;
	    parser->insertProduction(ZERO, rhs, 0, emptyGather);
	    break;
	  }
	case Token::SMALL_NEG:
	  {
	    rhs[0] = code;
	    parser->insertProduction(SMALL_NEG, rhs, 0, emptyGather);
	    break;
	  }
	case Token::RATIONAL:
	  {
	    rhs[0] = code;
	    parser->insertProduction(RATIONAL, rhs, 0, emptyGather);
	    break;
	  }
	case Token::FLOAT:
	  {
	    rhs[0] = code;
	    parser->insertProduction(FLOAT_NT, rhs, 0, emptyGather);
	    break;
	  }
	case Token::QUOTED_IDENTIFIER:
	  {
	    rhs[0] = code;
	    parser->insertProduction(QUOTED_ID, rhs, 0, emptyGather);
	    break;
	  }
	case Token::STRING:
	  {
	    rhs[0] = code;
	    parser->insertProduction(STRING_NT, rhs, 0, emptyGather);
	    break;
	  }
	case Token::CONTAINS_COLON:
	  {
	    int varName;
	    int sortName;
	    Token::split(code, varName, sortName);
	    if (Sort* sort = findSort(sortName))
	      {
		int sortIndex = sort->getIndexWithinModule();
		int componentIndex = sort->component()->getIndexWithinModule();
		rhs[0] = code;
		parser->insertProduction(nonTerminal(componentIndex, TERM_TYPE),
			   rhs, 0, emptyGather, MixfixParser::MAKE_VARIABLE, sortIndex);
	      }
	    break;
	  }
	case Token::ENDS_IN_COLON:
	  {
	    rhs[0] = code;
	    parser->insertProduction(ENDS_IN_COLON_NT, rhs, 0, emptyGather);
	    break;
	  }
	case Token::ITER_SYMBOL:
	  {
	    int opName;
	    mpz_class dummy;
	    Token::split(code, opName, dummy);
	    int index = iterSymbols.int2Index(opName);
	    if (index != NONE)
	      {
		rhs[0] = code;
		parser->insertProduction(iterSymbolNonTerminal(index),
					 rhs, 0, emptyGather);
	      }
	  }
	}
    }
  //
  //	Productions for so far unseen tokens that have special properties
  //
  {
    int t = Token::encode("[ SMALL_NAT ]");
    parser->insertSpecialTerminal(Token::SMALL_NAT, t);
    rhs[0] = t;
    parser->insertProduction(SMALL_NAT, rhs, 0, emptyGather);
  }
  {
    int t = Token::encode("[ ZERO ]");
    parser->insertSpecialTerminal(Token::ZERO, t);
    rhs[0] = t;
    parser->insertProduction(ZERO, rhs, 0, emptyGather);
  }
  {
    int t = Token::encode("[ SMALL_NEG ]");
    parser->insertSpecialTerminal(Token::SMALL_NEG, t);
    rhs[0] = t;
    parser->insertProduction(SMALL_NEG, rhs, 0, emptyGather);
  }
  {
    int t = Token::encode("[ RATIONAL ]");
    parser->insertSpecialTerminal(Token::RATIONAL, t);
    rhs[0] = t;
    parser->insertProduction(RATIONAL, rhs, 0, emptyGather);
  }
  {
    int t = Token::encode("[ FLOAT ]");
    parser->insertSpecialTerminal(Token::FLOAT, t);
    rhs[0] = t;
    parser->insertProduction(FLOAT_NT, rhs, 0, emptyGather);
  }
  {
    int t = Token::encode("[ QUOTED_IDENTIFIER ]");
    parser->insertSpecialTerminal(Token::QUOTED_IDENTIFIER, t);
    rhs[0] = t;
    parser->insertProduction(QUOTED_ID, rhs, 0, emptyGather);
  }
  {
    int t = Token::encode("[ STRING ]");
    parser->insertSpecialTerminal(Token::STRING, t);
    rhs[0] = t;
    parser->insertProduction(STRING_NT, rhs, 0, emptyGather);
  }
  {
    int t = Token::encode("[ ENDS_IN_COLON ]");
    parser->insertSpecialTerminal(Token::ENDS_IN_COLON, t);
    rhs[0] = t;
    parser->insertProduction(ENDS_IN_COLON_NT, rhs, 0, emptyGather);
  }
  {
    int nrIterSymbols = iterSymbols.cardinality();
    for (int i = 0; i < nrIterSymbols; i++)
      {
	string str("[ ");
	str += Token::name(iterSymbols.index2Int(i));
	str += " ]";
	int t = Token::encode(str.c_str());
	parser->insertIterSymbolTerminal(i, t);
	rhs[0] = t;
	parser->insertProduction(iterSymbolNonTerminal(i), rhs, 0, emptyGather);
      }
  }
}

void
MixfixModule::makePolymorphProductions()
{
#if PARSER_DEBUG
  cout << "<Polymorph productions>\n";
#endif

  static Vector<int> rhs;
  static Vector<int> gather;
  static Vector<int> mixfixRhs;
  static Vector<int> underscores;

  const Vector<ConnectedComponent*>& components = getConnectedComponents();
  int nrComponents = components.length();
  int nrPolymorphs = polymorphs.length();
  for (int i = 0; i < nrPolymorphs; i++)
    {
      Polymorph& p = polymorphs[i];
      int nrArgs = p.domainAndRange.length() - 1;
      bool branchSymbol = (p.symbolInfo.symbolType.getBasicType() == SymbolType::BRANCH_SYMBOL);
      Sort* specialSort = p.domainAndRange[branchSymbol ? 0 : 2];
      int specialNt = nonTerminal(specialSort->component()->getIndexWithinModule(), TERM_TYPE);
      //
      //	Prefix syntax.
      //
      rhs.resize(1);
      rhs[0] = p.name;
      rhs.append(leftParen);
      gather.contractTo(0);
      for (int j = 0; j < nrArgs; j++)
	{
	  gather.append(PREFIX_GATHER);
	  rhs.append(specialNt);
	  rhs.append(j == nrArgs - 1 ? rightParen : comma);
	}
      //
      //	Mixfix syntax.
      //
      int nrItems = p.symbolInfo.mixfixSyntax.length();
      if (nrItems > 0)
	{
	  mixfixRhs.resize(nrItems);
	  underscores.contractTo(0);
	  for (int j = 0; j < nrItems; j++)
	    {
	      int t = p.symbolInfo.mixfixSyntax[j];
	      if (t == underscore)
		{
		  mixfixRhs[j] = specialNt;
		  underscores.append(j);
		}
	      else
		mixfixRhs[j] = t;
	    }
	}
      //
      //	Now duplicate syntax in each connected component.
      //
      for (int j = 0; j < nrComponents; j++)
	{
	  if (!(bubbleComponents.contains(j)))
	    {
	      int termNt = nonTerminal(j, TERM_TYPE);
	      int rangeNt = branchSymbol ? termNt : specialNt;
	      for (int k = branchSymbol ? 1 : 0; k < nrArgs; k++)
		rhs[2 + 2 * k] = termNt;
	      parser->insertProduction(rangeNt, rhs, 0, gather,
				       MixfixParser::MAKE_POLYMORPH, j, i);
	      if (nrItems > 0)
		{
		  for (int k = branchSymbol ? 1 : 0; k < nrArgs; k++)
		    mixfixRhs[underscores[k]] = termNt;
		  parser->insertProduction(rangeNt,
					   mixfixRhs,
					   p.symbolInfo.prec,
					   p.symbolInfo.gather,
					   MixfixParser::MAKE_POLYMORPH,
					   j,
					   i);
		}
	    }
	}
    }
}

void
MixfixModule::makeBubbleProductions()
{
#if PARSER_DEBUG
  cout << "<Bubble productions>\n";
#endif

#ifdef BUBBLES
  int nrBubbleSpecs = bubbleSpecs.length();
  for (int i = 0; i < nrBubbleSpecs; i++)
    {
      BubbleSpec& b = bubbleSpecs[i];
      parser->insertBubbleProduction(nonTerminal(b.componentIndex, TERM_TYPE),
				     b.lowerBound,
				     b.upperBound,
				     b.leftParenToken,
				     b.rightParenToken,
				     b.excludedTokens,
				     i);
    }
#endif
}
