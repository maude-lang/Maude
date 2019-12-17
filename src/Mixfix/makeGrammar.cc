/*

    This file is part of the Maude 3 interpreter.

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
  //
  //	NonTerminals are allocated as follows:
  //	  fixed one-off:	-1,..., componentNonTerminalBase + 1
  //	  per component:	componentNonTerminalBase,..., componentNonTerminalBase - NUMBER_OF_TYPES * #components + 1
  //	  on demand:		componentNonTerminalBase - NUMBER_OF_TYPES * #components,... downwards
  //
  componentNonTerminalBase = complexParser ? COMPLEX_BASE : SIMPLE_BASE;
  nextNonTerminal = componentNonTerminalBase - NUMBER_OF_TYPES * getConnectedComponents().length() + 1;
  if (complexFlag)
    {
      makeComplexProductions();
      makeStrategyLanguageProductions();
    }
  makeLabelProductions();
  makeStatementProductions();
  makeConditionProductions();
  makeAttributeProductions();
  makeParameterizedSortProductions();
  makeComponentProductions();
  makeSymbolProductions();
  makeVariableProductions();
  makeBoolProductions();
  makeSpecialProductions();
  makePolymorphProductions();
  makeBubbleProductions();
  //
  //	These two data structures are only used for making the grammar and must be cleared in
  //	case we ever need to regenerate the grammar, since the nonterminals they contain will
  //	no longer be valid.
  //
  iterSymbols.clear();
  leadTokens.clear();
}

void
MixfixModule::makeParameterizedSortProductions()
{
  FOR_EACH_CONST(i, SortMap, sortNames)
    {
      int name= i->first;
      if (Token::auxProperty(name) == Token::AUX_STRUCTURED_SORT)
	{
	  //
	  //	Need to make a non-terminal for each lead token of a parameterized sort
	  //	so we can parse on-the-fly variables.
	  //
	  Vector<int> parts;
	  Token::splitParameterizedSort(name, parts);
	  int lead = parts[0];
	  pair<IntMap::iterator, bool> p = leadTokens.insert(IntMap::value_type(lead, GARBAGE));
	  if (p.second)
	    {
	      //
	      //	First time we've seen this lead token so we need to make a nonterminal,
	      //	terminal and production for it so we can parse on-the-fly variables for
	      //	for parameterized sorts starting with this token.
	      //
	      int nt = newNonTerminal();
	      string leadString(Token::name(lead));
	      int t = Token::encode((leadString + " variable").c_str());

	      p.first->second = nt;
	      Vector<int> rhs(1);
	      rhs[0] = t;
	      //cout << "para " << nt << " ::= " << rhs[0] << endl;
	      parser->insertProduction(nt, rhs, 0, emptyGather);
	      parser->insertVariableTerminal(lead, t);
	    }
	}
    }
}

void
MixfixModule::makeComplexProductions()
{
#if PARSER_DEBUG
  cout << "<Complex productions>\n";
#endif

  static Vector<int> rhs(3);

  rhs.resize(1);
  rhs[0] = arrowOne;
  parser->insertProduction(SEARCH_CONNECTIVE, rhs, 0, emptyGather,
			   MixfixParser::NOP, RewriteSequenceSearch::ONE_STEP);
  rhs[0] = arrowPlus;
  parser->insertProduction(SEARCH_CONNECTIVE, rhs, 0, emptyGather,
			   MixfixParser::NOP, RewriteSequenceSearch::AT_LEAST_ONE_STEP);
  rhs[0] = arrowStar;
  parser->insertProduction(SEARCH_CONNECTIVE, rhs, 0, emptyGather, 
			   MixfixParser::NOP, RewriteSequenceSearch::ANY_STEPS);
  rhs[0] = arrowBang;
  parser->insertProduction(SEARCH_CONNECTIVE, rhs, 0, emptyGather,
			   MixfixParser::NOP, RewriteSequenceSearch::NORMAL_FORM);

  rhs[0] = MATCH_PAIR;
  parser->insertProduction(MATCH_COMMAND, rhs, 0, gatherAny);
  rhs[0] = UNIFY_PAIR;
  parser->insertProduction(UNIFY_COMMAND, rhs, 0, gatherAny);
  rhs[0] = SEARCH_PAIR;
  parser->insertProduction(SEARCH_COMMAND, rhs, 0, gatherAny);
  rhs[0] = TERM;
  parser->insertProduction(GET_VARIANTS_COMMAND, rhs, 0, gatherAny);
  rhs[0] = UNIFY_COMMAND;
  parser->insertProduction(VARIANT_UNIFY_COMMAND, rhs, 0, gatherAny);

  rhs[0] = suchThat;
  parser->insertProduction(SUCH_THAT, rhs, 0, emptyGather);

  rhs.resize(2);
  rhs[0] = such;
  rhs[1] = that;
  parser->insertProduction(SUCH_THAT, rhs, 0, emptyGather);

  rhs.resize(3);
  rhs[0] = MATCH_PAIR;
  rhs[1] = SUCH_THAT;
  rhs[2] = CONDITION;
  parser->insertProduction(MATCH_COMMAND, rhs, 0, gatherAnyAnyAny,
			   MixfixParser::CONDITIONAL_COMMAND);

  rhs[0] = SEARCH_PAIR;
  rhs[2] = RULE_CONDITION;
  parser->insertProduction(SEARCH_COMMAND, rhs, 0, gatherAnyAnyAny,
			   MixfixParser::CONDITIONAL_COMMAND);

  rhs[0] = UNIFY_PAIR;
  rhs[1] = wedge;
  rhs[2] = UNIFY_COMMAND;
  parser->insertProduction(UNIFY_COMMAND, rhs, 0, gatherAnyAny, MixfixParser::UNIFY_LIST);

  rhs.resize(4);
  rhs[0] = TERM;
  rhs[1] = SUCH_THAT;
  rhs[2] = TERM_LIST;
  rhs[3] = irreducible;
  parser->insertProduction(GET_VARIANTS_COMMAND, rhs, 0, gatherAnyAnyAny,
			   MixfixParser::CONDITIONAL_COMMAND);

  rhs.resize(4);
  rhs[0] = UNIFY_COMMAND;
  rhs[1] = SUCH_THAT;
  rhs[2] = TERM_LIST;
  rhs[3] = irreducible;
  parser->insertProduction(VARIANT_UNIFY_COMMAND, rhs, 0, gatherAnyAnyAny,
			   MixfixParser::CONDITIONAL_COMMAND);
  //
  //	Hetrogeneous term lists.
  //
  rhs.resize(1);
  rhs[0] = TERM;
  parser->insertProduction(TERM_LIST, rhs, 0, gatherAny, MixfixParser::PASS_THRU);
  rhs.resize(3);
  rhs[0] = TERM;
  rhs[1] = comma;
  rhs[2] = TERM_LIST;
  parser->insertProduction(TERM_LIST, rhs, PREFIX_GATHER, gatherPrefixPrefix, MixfixParser::MAKE_TERM_LIST);
  //
  //	Substitutions.
  //
  rhs.resize(1);
  rhs[0] = ASSIGNMENT;
  parser->insertProduction(SUBSTITUTION, rhs, 0, gatherAny, MixfixParser::PASS_THRU);
  rhs.resize(3);
  rhs[0] = ASSIGNMENT;
  rhs[1] = comma;
  rhs[2] = SUBSTITUTION;
  parser->insertProduction(SUBSTITUTION, rhs, PREFIX_GATHER, gatherPrefixPrefix, MixfixParser::MAKE_SUBSTITUTION);
}

void
MixfixModule::makeStrategyLanguageProductions()
{
#if PARSER_DEBUG
  cout << "<Strategy Language productions>\n";
#endif

  static Vector<int> rhs(3);
  //
  //	We assume that rules have been flattened in. For each label occuring in a rule,
  //	we add a production.
  //
  rhs.resize(1);
  {
    set<int> handled;
    for (int label : ruleLabels)
      {
	if (label != NONE && handled.find(label) == handled.end())
	  {
	    rhs[0] = label;
	    parser->insertProduction(RULE_LABEL, rhs, 0, emptyGather, MixfixParser::NOP, label);
	    handled.insert(label);
	  }
      }
  }

  //
  //	Create grammar productions for each named strategy.
  //	(as any other prefix operator)
  //
  {
    const Vector<RewriteStrategy*> &strategies = getStrategies();
    for (size_t i = 0; i < strategies.size(); i++)
      {
	Symbol * symbol = strategies[i]->getSymbol();
	int nrArgs = symbol->arity();

	rhs.resize(1);
	rhs[0] = strategies[i]->id();

	if (nrArgs == 0)
	  {
	    //
	    // Strategy without parameters can be written <strategy name> when no rule with that name exits.
	    //
	    if (ruleLabels.find(rhs[0]) == ruleLabels.end())
	      parser->insertProduction(STRATEGY_CALL_EXPRESSION, rhs, 0, emptyGather, MixfixParser::NOP, i);

	    //
	    // Strategy without parameters can be written <strategy name>()
	    //
	    rhs.resize(3);
	    rhs[1] = leftParen;
	    rhs[2] = rightParen;
	    parser->insertProduction(STRATEGY_CALL_EXPRESSION, rhs, 0, emptyGather, MixfixParser::NOP, i);
	  }
	else
	  {
	    Vector<int> gather;
	    //
	    // Strategies wiht parameters are read in a prefixed form
	    //
	    rhs.append(leftParen);
	    gather.resize(0);

	    for (int j = 0; j < nrArgs; j++)
	      {
		gather.append(PREFIX_GATHER);
		rhs.append(nonTerminal(domainComponentIndex(symbol, j), TERM_TYPE));
		rhs.append(j == nrArgs - 1 ? rightParen : comma);
	      }
	    parser->insertProduction(STRATEGY_CALL_EXPRESSION, rhs, 0, gather, MixfixParser::NOP, i);
	  }
      }
  }

  rhs.resize(1);
  rhs[0] = fail;
  parser->insertProduction(STRATEGY_EXPRESSION, rhs, 0, emptyGather, MixfixParser::MAKE_TRIVIAL, false);

  rhs[0] = idle;
  parser->insertProduction(STRATEGY_EXPRESSION, rhs, 0, emptyGather, MixfixParser::MAKE_TRIVIAL, true);

  rhs[0] = all;
  parser->insertProduction(STRATEGY_EXPRESSION, rhs, 0, emptyGather, MixfixParser::MAKE_ALL);

  {
    //
    //	<strategy expression> = <label> [ <substitution> ] { <strategy list> }
    //
    Vector<int> rhs(7);
    rhs[0] = RULE_LABEL;
    rhs[1] = leftBracket;
    rhs[2] = SUBSTITUTION;
    rhs[3] = rightBracket;
    rhs[4] = leftBrace;
    rhs[5] = STRATEGY_LIST;
    rhs[6] = rightBrace;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_BASIC_PREC, gatherAnyAnyAny,
			     MixfixParser::MAKE_APPLICATION, true, true);
    //
    //	<strategy expression> = <label> [ <substitution> ]
    //
    rhs.resize(4);
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_BASIC_PREC, gatherAnyAny,
			     MixfixParser::MAKE_APPLICATION, true, false);
    //
    //	<strategy expression> = <label> { <strategy list> }
    //
    rhs[1] = leftBrace;
    rhs[2] = STRATEGY_LIST;
    rhs[3] = rightBrace;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_BASIC_PREC, gatherAnyAny,
			     MixfixParser::MAKE_APPLICATION, false, true);
    //
    //	<strategy expression> = <label>
    //
    rhs.resize(1);
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_BASIC_PREC, gatherAny,
			     MixfixParser::MAKE_APPLICATION, false, false);

	//
	// <call expression> = <strategy call expression>
	//
	rhs.resize(1);
	rhs[0] = STRATEGY_CALL_EXPRESSION;
	parser->insertProduction(STRATEGY_EXPRESSION, rhs, 0, gatherAny,
				MixfixParser::MAKE_CALL);
  }
  {
    //
    //	<strategy list> = <strategy expression> , <strategy list>
    //
    Vector<int> rhs(3);
    rhs[0] = STRATEGY_EXPRESSION;
    rhs[1] = comma;
    rhs[2] = STRATEGY_LIST;
    parser->insertProduction(STRATEGY_LIST, rhs, INFIX_PREC, gatherAnyAny,
			     MixfixParser::MAKE_STRATEGY_LIST);
    //
    //	<strategy list> = <strategy expression>
    //
    rhs.resize(1);
    parser->insertProduction(STRATEGY_LIST, rhs, 0, gatherAny, MixfixParser::PASS_THRU);
  }
  {
    //
    //	<strategy expression> = top ( <strategy expression> )
    //	<strategy expression> = one ( <strategy expression> )
    //	<strategy expression> = not ( <strategy expression> )
    //	<strategy expression> = test ( <strategy expression> )
    //	<strategy expression> = try ( <strategy expression> )
    //
    Vector<int> rhs(4);
    rhs[0] = top;
    rhs[1] = leftParen;
    rhs[2] = STRATEGY_EXPRESSION;
    rhs[3] = rightParen;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, 0, gatherAny, MixfixParser::MAKE_TOP);

    rhs[0] = one;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, 0, gatherAny, MixfixParser::MAKE_ONE);

    rhs[0] = notToken;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, 0, gatherAny,
			     MixfixParser::MAKE_BRANCH, BranchStrategy::FAIL, BranchStrategy::IDLE);
    rhs[0] = test;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, 0, gatherAny,
			     MixfixParser::MAKE_BRANCH, BranchStrategy::IDLE, BranchStrategy::FAIL);
    rhs[0] = tryToken;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, 0, gatherAny,
			     MixfixParser::MAKE_BRANCH, BranchStrategy::PASS_THRU, BranchStrategy::IDLE);
  }
  {
    //
    //	<strategy expression> = <strategy expression> ; <strategy expression>
    //	<strategy expression> = <strategy expression> | <strategy expression>
    //	<strategy expression> = <strategy expression> orelse <strategy expression>
    //
    //	We force these to right associate by manipulating the gathers.
    //
    Vector<int> gather(2);
    gather[0] = STRAT_SEQ_PREC - 1;
    gather[1] = STRAT_SEQ_PREC;
    Vector<int> rhs(3);
    rhs[0] = STRATEGY_EXPRESSION;
    rhs[1] = semicolon;
    rhs[2] = STRATEGY_EXPRESSION;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_SEQ_PREC, gather, MixfixParser::MAKE_CONCATENATION);
    gather[0] = STRAT_UNION_PREC - 1;
    gather[1] = STRAT_UNION_PREC;
    rhs[1] = pipe;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_UNION_PREC, gather, MixfixParser::MAKE_UNION);
    gather[0] = STRAT_ORELSE_PREC - 1;
    gather[1] = STRAT_ORELSE_PREC;
    rhs[1] = orelse;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_ORELSE_PREC, gather,
			     MixfixParser::MAKE_BRANCH, BranchStrategy::PASS_THRU, BranchStrategy::NEW_STRATEGY);
  }
  {
    //
    //	<strategy expression> = <strategy expression> +
    //	<strategy expression> = <strategy expression> *
    //	<strategy expression> = <strategy expression> !
    //
    Vector<int> gather(1);
    gather[0] = 0;
    Vector<int> rhs(2);
    rhs[0] = STRATEGY_EXPRESSION;
    rhs[1] = plus;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, 0, gather, MixfixParser::MAKE_ITERATION, false);
    rhs[1] = star;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, 0, gather, MixfixParser::MAKE_ITERATION, true);
    rhs[1] = bang;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, 0, gather,
			     MixfixParser::MAKE_BRANCH, BranchStrategy::ITERATE, BranchStrategy::IDLE);
  }
  {
    //
    //	<strategy expression> = <strategy expression> ? <strategy expression> : <strategy expression>
    //
    Vector<int> gather(3);
    Vector<int> rhs(5);
    gather[0] = STRAT_BRANCH_PREC -1;
    gather[1] = ANY;
    gather[2] = STRAT_BRANCH_PREC;
    rhs[0] = STRATEGY_EXPRESSION;
    rhs[1] = query;
    rhs[2] = STRATEGY_EXPRESSION;
    rhs[3] = colon;
    rhs[4] = STRATEGY_EXPRESSION;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_BRANCH_PREC, gather, MixfixParser::MAKE_BRANCH,
			     BranchStrategy::NEW_STRATEGY, BranchStrategy::NEW_STRATEGY);
  }
  {
    //
    //	<strategy expression> = match <term> such that <condition>
    //	<strategy expression> = xmatch <term> such that <condition>
    //	<strategy expression> = amatch <term> such that <condition>
    //	<strategy expression> = match <term>
    //	<strategy expression> = xmatch <term>
    //	<strategy expression> = amatch <term>
    //
    Vector<int> rhs(4);
    rhs[0] = match;
    rhs[1] = TERM;
    rhs[2] = SUCH_THAT;
    rhs[3] = CONDITION;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_TEST_PREC, gatherAnyAnyAny, MixfixParser::MAKE_TEST, -1);
    rhs[0] = xmatch;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_TEST_PREC, gatherAnyAnyAny, MixfixParser::MAKE_TEST, 0);
    rhs[0] = amatch;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_TEST_PREC, gatherAnyAnyAny, MixfixParser::MAKE_TEST, UNBOUNDED);
    rhs.resize(2);
    rhs[0] = match;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_TEST_PREC, gatherAny, MixfixParser::MAKE_TEST, -1);
    rhs[0] = xmatch;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_TEST_PREC, gatherAny, MixfixParser::MAKE_TEST, 0);
    rhs[0] = amatch;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_TEST_PREC, gatherAny, MixfixParser::MAKE_TEST, UNBOUNDED);
  }
  {

    //
    //	<using pair> = <term> using <strategy expression>
    //	<using list> = <using pair> , <using list>
    //	<using list> = <using pair>
    //
    Vector<int> rhs(3);
    rhs[0] = TERM;
    rhs[1] = usingToken;
    rhs[2] = STRATEGY_EXPRESSION;
    Vector<int> gather(2);
    gather[0] = ANY;
    gather[1] = STRAT_USING_PREC - 1;  // require strategy be tightly bound to avoid certain ambiguities
    parser->insertProduction(USING_PAIR, rhs, 0, gather, MixfixParser::MAKE_USING_PAIR);
    rhs[0] = USING_PAIR;
    rhs[1] = comma;
    rhs[2] = USING_LIST;
    parser->insertProduction(USING_LIST, rhs, 0, gatherAnyAny, MixfixParser::MAKE_USING_LIST);
    rhs.resize(1);
    parser->insertProduction(USING_LIST, rhs, 0, gatherAny, MixfixParser::PASS_THRU);
  }
  {
    //
    //	<strategy expression> = (/x/a)matchrew <term> such that <condition> by <using list>
    //
    //	<strategy expression> = (/x/a)amatchrew <term> by <using list>
    //
    Vector<int> rhs(6);
    rhs[0] = matchrew;
    rhs[1] = TERM;
    rhs[2] = SUCH_THAT;
    rhs[3] = CONDITION;
    rhs[4] = by;
    rhs[5] = USING_LIST;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_REW_PREC, gatherAny4, MixfixParser::MAKE_REW, -1);
    rhs[0] = xmatchrew;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_REW_PREC, gatherAny4, MixfixParser::MAKE_REW, 0);
    rhs[0] = amatchrew;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_REW_PREC, gatherAny4, MixfixParser::MAKE_REW, UNBOUNDED);
    rhs.resize(4);
    rhs[0] = matchrew;
    rhs[2] = by;
    rhs[3] = USING_LIST;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_REW_PREC, gatherAnyAny, MixfixParser::MAKE_REW, -1);
    rhs[0] = xmatchrew;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_REW_PREC, gatherAnyAny, MixfixParser::MAKE_REW, 0);
    rhs[0] = amatchrew;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, STRAT_REW_PREC, gatherAnyAny, MixfixParser::MAKE_REW, UNBOUNDED);
  }
  {
    //
    //	<strategy expression> = ( <strategy expression> )
    //
    Vector<int> rhs(3);
    rhs[0] = leftParen;
    rhs[1] = STRATEGY_EXPRESSION;
    rhs[2] = rightParen;
    parser->insertProduction(STRATEGY_EXPRESSION, rhs, 0, gatherAny, MixfixParser::PASS_THRU);
  }
  //
  //	<strategy command> = <term> using <strategy expression>
  //
  rhs.resize(3);
  rhs[0] = TERM;
  rhs[1] = usingToken;
  rhs[2] = STRATEGY_EXPRESSION;
  parser->insertProduction(STRATEGY_COMMAND, rhs, 0, gatherAnyAny);
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
  rhs[0] = print;
  rhs[1] = PRINT_LIST;
  parser->insertProduction(ATTRIBUTE, rhs, 0, gatherAny,
			   MixfixParser::MAKE_PRINT_ATTRIBUTE);

  rhs.resize(1);
  parser->insertProduction(ATTRIBUTE, rhs, 0, emptyGather,
			   MixfixParser::MAKE_PRINT_ATTRIBUTE);

  rhs[0] = nonexec;
  parser->insertProduction(ATTRIBUTE, rhs, 0, emptyGather,
			   MixfixParser::MAKE_NONEXEC_ATTRIBUTE);
  rhs[0] = otherwise;
  parser->insertProduction(ATTRIBUTE, rhs, 0, emptyGather,
			   MixfixParser::MAKE_OWISE_ATTRIBUTE);
  rhs[0] = owise;
  parser->insertProduction(ATTRIBUTE, rhs, 0, emptyGather,
			   MixfixParser::MAKE_OWISE_ATTRIBUTE);
  rhs[0] = variant;
  parser->insertProduction(ATTRIBUTE, rhs, 0, emptyGather,
			   MixfixParser::MAKE_VARIANT_ATTRIBUTE);
  rhs[0] = narrowing;
  parser->insertProduction(ATTRIBUTE, rhs, 0, emptyGather,
			   MixfixParser::MAKE_NARROWING_ATTRIBUTE);
  //
  //	Print items.
  //
  rhs[0] = STRING_NT;
  parser->insertProduction(PRINT_ITEM, rhs, 0, gatherAny, MixfixParser::MAKE_STRING);
  rhs[0] = VARIABLE;
  parser->insertProduction(PRINT_ITEM, rhs, 0, gatherAny, MixfixParser::MAKE_VARIABLE);
  //
  //	Print lists.
  //
  rhs[0] = PRINT_ITEM;
  parser->insertProduction(PRINT_LIST, rhs, 0, gatherAny);
  rhs.resize(2);
  rhs[1] = PRINT_LIST;
  parser->insertProduction(PRINT_LIST, rhs, 0, gatherAnyAny,
			   MixfixParser::MAKE_PRINT_LIST);
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

  rhs[0] = sd;
  rhs[1] = SD_BODY;
  parser->insertProduction(STATEMENT_PART, rhs, 0, gatherAny, MixfixParser::MAKE_SD);
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

  rhs[0] = csd;
  rhs[1] = SD_BODY;
  rhs[3] = CONDITION;
  parser->insertProduction(STATEMENT_PART, rhs, 0, gatherAnyAny, MixfixParser::MAKE_CSD);
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

  rhs[0] = STRATEGY_PAIR;
  parser->insertProduction(SD_BODY, rhs, 0, gatherAny);

  //
  //	Strategy pair (can be SD_BODY directly, with some changes)
  //	<strategy pair> ::= <strategy call expr> := <strategy expression>
  //
  rhs.resize(3);
  rhs[0] = STRATEGY_CALL_EXPRESSION;
  rhs[1] = assign;
  rhs[2] = STRATEGY_EXPRESSION;
  parser->insertProduction(STRATEGY_PAIR, rhs, 0, gatherAnyAny);
  //
  //	Statement bodies with labels.
  //
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
  rhs[0] = COLON_PAIR;
  parser->insertProduction(CONDITION_FRAGMENT, rhs, 0, gatherAny, MixfixParser::MAKE_SORT_TEST);
  rhs[0] = ASSIGN_PAIR;
  parser->insertProduction(CONDITION_FRAGMENT, rhs, 0, gatherAny, MixfixParser::MAKE_ASSIGNMENT);
  rhs[0] = ARROW_PAIR;
  parser->insertProduction(RULE_CONDITION_FRAGMENT, rhs, 0, gatherAny, MixfixParser::MAKE_REWRITE);
  rhs[0] = CONDITION_FRAGMENT;
  parser->insertProduction(RULE_CONDITION_FRAGMENT, rhs, 0, gatherAny, MixfixParser::PASS_THRU);
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
	  parser->insertProduction(sortNt, rhsOne, 0, emptyGather, MixfixParser::NOP, sortIndex);
	  rhsOne[0] = Token::dotNameCode(sortNameCode);
	  parser->insertProduction(dotSortNt, rhsOne, 0, emptyGather);
	  if (Token::auxProperty(sortNameCode) == Token::AUX_STRUCTURED_SORT)
	    {
	      //
	      //	Syntax for multitoken versions of parameterized sorts.
	      //
	      Vector<int> parts;
	      Token::splitParameterizedSort(sortNameCode, parts);
	      parser->insertProduction(sortNt, parts, 0, emptyGather, MixfixParser::NOP, sortIndex);
	      int lead = parts[0];
	      parts[0] = Token::dotNameCode(lead);
	      parser->insertProduction(dotSortNt, parts, 0, emptyGather);
	      //
	      //	Syntax for unseen variable of multitoken sort.
	      //
	      parts[0] = leadTokens[lead];  // nonterminal
	      parser->insertProduction(termNt, parts, 0, gatherAny,
				       MixfixParser::MAKE_VARIABLE, sortIndex);
	      parser->insertProduction(VARIABLE, parts, 0, gatherAny,
				       MixfixParser::MAKE_VARIABLE, sortIndex);
	    }
	  //
	  //	Syntax for yet unseen variables of our sort.
	  //
	  IntMap::const_iterator p = leadTokens.find(sortNameCode);
	  if (p != leadTokens.end())
	    {
	      //cerr << "(" << p->first << ", " << p->second << ")" << endl;
	      rhsOne[0] = p->second;
	      //cerr << termNt << " ::= " << rhsOne[0] << endl;
	      parser->insertProduction(termNt, rhsOne, 0, gatherAny,
				       MixfixParser::MAKE_VARIABLE, sortIndex);
	      parser->insertProduction(VARIABLE, rhsOne, 0, gatherAny,
				       MixfixParser::MAKE_VARIABLE, sortIndex);
	    }
	  else
	    {
	      string sortName(Token::name(sortNameCode));
	      int t = Token::encode((sortName + " variable").c_str());
	      parser->insertVariableTerminal(sortNameCode, t);
	      rhsOne[0] = t;
	      //cerr << termNt << " ::= " << rhsOne[0] << endl;
	      parser->insertProduction(termNt, rhsOne, 0, emptyGather,
				       MixfixParser::MAKE_VARIABLE, sortIndex);
	      parser->insertProduction(VARIABLE, rhsOne, 0, emptyGather,
				       MixfixParser::MAKE_VARIABLE, sortIndex);
	    }
	}
      //
      //	Syntax for on the fly kind variable:
      //	<FooTerm> ::= <ENDS_IN_COLON> [ <FooSortList> ]
      //
      int sortIndex = component->sort(Sort::ERROR_SORT)->getIndexWithinModule();
      rhsKindVariable[2] = sortListNt;
      parser->insertProduction(termNt, rhsKindVariable, 0, gatherAnyAny,
			       MixfixParser::MAKE_VARIABLE, sortIndex);
      parser->insertProduction(VARIABLE, rhsKindVariable, 0, gatherAnyAny,
			       MixfixParser::MAKE_VARIABLE, sortIndex);
      //
      //	Syntax for term from unknown component:
      //	<TERM> ::= <FooTerm>
      //	We don't support this for kinds that contain bubbles.
      //
      if (bubbleComponents.find(i) == bubbleComponents.end())
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
	  //	Syntax for match and search pairs, assignments:
	  //	<MATCH_PAIR> ::= <FooTerm> <=? <FooTerm>
	  //	<SEARCH_PAIR> ::= <FooTerm> <SEARCH_CONNECTIVE> <FooTerm>
	  //	<ASSIGNMENT> ::= <FooTerm> <- <FooTerm>
	  //
	  rhsPair[1] = matches;
	  parser->insertProduction(MATCH_PAIR, rhsPair, 0, gatherAnyAny);
	  rhsPair[1] = unifies;
	  parser->insertProduction(UNIFY_PAIR, rhsPair, 0, gatherAnyAny);
	  rhsPair[1] = SEARCH_CONNECTIVE;
	  parser->insertProduction(SEARCH_PAIR, rhsPair, 0, gatherAnyAnyAny);
	  rhsPair[1] = assignment;
	  Vector<int> gather(2);
	  gather[0] = ASSIGNMENT_PREC;
	  gather[1] = ASSIGNMENT_PREC;
	  parser->insertProduction(ASSIGNMENT, rhsPair, ASSIGNMENT_PREC, gather);
	}
      //
      //	Syntax for colon pairs:
      //	<COLON_PAIR> ::= <FooTerm> : <FooSort>
      //	<COLON2_PAIR> ::= <FooTerm> :: <FooSort>
      //
      rhsPair[2] = sortNt;
      rhsPair[1] = colon;
      parser->insertProduction(COLON_PAIR, rhsPair, 0, gatherAny0);
      if (bubbleComponents.find(i) == bubbleComponents.end())
	{
	  rhsPair[1] = colon2;
	  parser->insertProduction(COLON2_PAIR, rhsPair, 0, gatherAny0);
	}
      //
      //	Syntax for parentheses:
      //	<FooTerm> ::= ( <FooTerm> )
      //	We don't support this for kinds that contain bubbles to avoid
      //	a trivial ambiguity with parentheses inside the bubble.
      //
      if (bubbleComponents.find(i) == bubbleComponents.end())
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
	    case SymbolType::SMT_NUMBER_SYMBOL:
	      {
		Sort* sort = symbol->getRangeSort();
		SMT_Info::SMT_Type t = getSMT_Info().getType(sort);
		Assert(t != SMT_Info::NOT_SMT, "bad SMT sort " << sort);
		if (t == SMT_Info::INTEGER)
		  {
		    rhs[0] = ZERO;
		    parser->insertProduction(rangeNt, rhs, 0, gatherAny, MixfixParser::MAKE_SMT_NUMBER, i);
		    rhs[0] = SMALL_NAT;
		    parser->insertProduction(rangeNt, rhs, 0, gatherAny, MixfixParser::MAKE_SMT_NUMBER, i);
		    rhs[0] = SMALL_NEG;
		    parser->insertProduction(rangeNt, rhs, 0, gatherAny, MixfixParser::MAKE_SMT_NUMBER, i);
		  }
		else
		  {
		    Assert(t == SMT_Info::REAL, "SMT number sort expected");
		    rhs[0] = RATIONAL;
		    parser->insertProduction(rangeNt, rhs, 0, gatherAny, MixfixParser::MAKE_SMT_NUMBER, i);
		  }
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
	      //
	      //	An iter symbol with the same name may have already created a nonterminal;
	      //	otherwise we need to create one.
	      //
	      pair<IntMap::iterator, bool> p = iterSymbols.insert(IntMap::value_type(symbol->id(), GARBAGE));
	      if (p.second)
		p.first->second = newNonTerminal();
	      rhs.clear();
	      rhs.append(p.first->second);
	      rhs.append(leftParen);
	      rhs.append(rangeNt);
	      rhs.append(rightParen);
	      gather.resize(0);
	      gather.append(PREFIX_GATHER);
	      gather.append(PREFIX_GATHER);
	      parser->insertProduction(rangeNt, rhs, 0, gather, MixfixParser::MAKE_ITER, i);
	      //
	      //	If symbol is a successor symbol, add the syntax
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
      parser->insertProduction(nonTerminal(sort, TERM_TYPE), rhs, 0, emptyGather,
			       MixfixParser::MAKE_VARIABLE_FROM_ALIAS, sort->getIndexWithinModule());
      parser->insertProduction(VARIABLE, rhs, 0, emptyGather,
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
      int rangeNt = nonTerminal(boolSort, TERM_TYPE);
      //
      //	Add extra syntactic sugar to allow a condition fragment to just be a bool.
      //
      rhs[0] = rangeNt;
      parser->insertProduction(CONDITION_FRAGMENT, rhs, 0, gatherAny, MixfixParser::MAKE_TRUE);

      if (falseSymbol != 0 &&
	  falseSymbol->rangeComponent() == trueSymbol->rangeComponent())
	{
	  //
	  //	Add syntax for sort test operators
	  //
	  rhs[0] = COLON2_PAIR;
	  parser->insertProduction(rangeNt, rhs, 0, gatherAny, MixfixParser::MAKE_SORT_TEST, 1);
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
  FOR_EACH_CONST(i, set<int>, potentialLabels)
    {
      int label = *i;
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
	    IntMap::const_iterator t = leadTokens.find(sortName);
	    if (t != leadTokens.end())
	      {
		rhs[0] = code;
		parser->insertProduction(t->second, rhs, 0, emptyGather);
	      }
	    else if (Sort* sort = findSort(sortName))
	      {
		int sortIndex = sort->getIndexWithinModule();
		int componentIndex = sort->component()->getIndexWithinModule();
		rhs[0] = code;
		parser->insertProduction(nonTerminal(componentIndex, TERM_TYPE),
					 rhs, 0, emptyGather, MixfixParser::MAKE_VARIABLE, sortIndex);
		parser->insertProduction(VARIABLE,
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
	    IntMap::const_iterator t = iterSymbols.find(opName);
	    if (t != iterSymbols.end())
	      {
		rhs[0] = code;
		parser->insertProduction(t->second, rhs, 0, emptyGather);
	      }
	  }
	}
    }
  //
  //	Productions for so far unseen tokens that have special properties
  //
  {
    int t = smallNat;
    parser->insertSpecialTerminal(Token::SMALL_NAT, t);
    rhs[0] = t;
    parser->insertProduction(SMALL_NAT, rhs, 0, emptyGather);
  }
  {
    int t = zero;
    parser->insertSpecialTerminal(Token::ZERO, t);
    rhs[0] = t;
    parser->insertProduction(ZERO, rhs, 0, emptyGather);
  }
  {
    int t = smallNeg;
    parser->insertSpecialTerminal(Token::SMALL_NEG, t);
    rhs[0] = t;
    parser->insertProduction(SMALL_NEG, rhs, 0, emptyGather);
  }
  {
    int t = rational;
    parser->insertSpecialTerminal(Token::RATIONAL, t);
    rhs[0] = t;
    parser->insertProduction(RATIONAL, rhs, 0, emptyGather);
  }
  {
    int t = floatToken;
    parser->insertSpecialTerminal(Token::FLOAT, t);
    rhs[0] = t;
    parser->insertProduction(FLOAT_NT, rhs, 0, emptyGather);
  }
  {
    int t = quotedIdentifier;
    parser->insertSpecialTerminal(Token::QUOTED_IDENTIFIER, t);
    rhs[0] = t;
    parser->insertProduction(QUOTED_ID, rhs, 0, emptyGather);
  }
  {
    int t = stringToken;
    parser->insertSpecialTerminal(Token::STRING, t);
    rhs[0] = t;
    parser->insertProduction(STRING_NT, rhs, 0, emptyGather);
  }
  {
    int t = endsInColon;
    parser->insertSpecialTerminal(Token::ENDS_IN_COLON, t);
    rhs[0] = t;
    parser->insertProduction(ENDS_IN_COLON_NT, rhs, 0, emptyGather);
  }
  {
    FOR_EACH_CONST(i, IntMap, iterSymbols)
      {
	int iterSymbolNameCode = i->first;
	string str("[ ");
	str += Token::name(iterSymbolNameCode);
	str += " ]";
	int t = Token::encode(str.c_str());
	parser->insertIterSymbolTerminal(iterSymbolNameCode, t);
	rhs[0] = t;
	parser->insertProduction(i->second, rhs, 0, emptyGather);
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
      //cerr << Token::name(p.name) << ' ' << p.polyArgs << endl;
      int nrArgs = p.domainAndRange.length() - 1;
      //int type = p.symbolInfo.symbolType.getBasicType();
      //
      //	Prefix syntax.
      //
      rhs.resize(1);
      rhs[0] = p.name.code();
      gather.resize(nrArgs);
      if (nrArgs > 0)
	{
	  rhs.resize(2 + 2 * nrArgs);
	  rhs[1] = leftParen;
	  for (int j = 0; j < nrArgs; j++)
	    {
	      gather[j] = PREFIX_GATHER;
	      const Sort* s = p.domainAndRange[j];
	      if (s != 0)
		rhs[2 + 2 * j] = nonTerminal(s, TERM_TYPE);
	      rhs[3 + 2 * j] = (j == nrArgs - 1) ? rightParen : comma;
	    }
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
		  const Sort* s = p.domainAndRange[underscores.length()];
		  if (s != 0)
		    mixfixRhs[j] = nonTerminal(s, TERM_TYPE);
		  underscores.append(j);
		}
	      else
		mixfixRhs[j] = t;
	    }
	}
      //
      //	Now duplicate syntax in each connected component that does not contain bubbles.
      //
     for (int j = 0; j < nrComponents; j++)
	{
	  if (bubbleComponents.find(j) == bubbleComponents.end())
	    {
	      int termNt = nonTerminal(j, TERM_TYPE);
	      const Sort* s = p.domainAndRange[nrArgs];
	      int rangeNt = (s == 0) ? termNt : nonTerminal(s, TERM_TYPE);
	      for (int k = 0; k < nrArgs; k++)
		{
		  if (p.domainAndRange[k] == 0)
		    rhs[2 + 2 * k] = termNt;
		}
	      parser->insertProduction(rangeNt, rhs, 0, gather,
				       MixfixParser::MAKE_POLYMORPH, j, i);
	      if (nrItems > 0)
		{
		  for (int k = 0; k < nrArgs; k++)
		    {
		      if (p.domainAndRange[k] == 0)
			mixfixRhs[underscores[k]] = termNt;
		    }
		  parser->insertProduction(rangeNt,
					   mixfixRhs,
					   p.symbolInfo.prec,
					   p.symbolInfo.gather,
					   MixfixParser::MAKE_POLYMORPH,
					   j, i);
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

  //#ifdef BUBBLES
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
  //#endif
}
