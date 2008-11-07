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

//      interface class definitions
#include "term.hh"

//      core class definitions
#include "equation.hh"
#include "rule.hh"
#include "sortConstraint.hh"
#include "conditionFragment.hh"

//	variable class definitions
#include "variableSymbol.hh"
#include "variableTerm.hh"

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

//	front end class definitions
#include "mixfixModule.hh"
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierTerm.hh"
#include "mixfixParser.hh"

#ifdef SCP
#define ROOT_NODE	(parser.getRootNode())
#else
#define ROOT_NODE	(0)
#endif

const IntSet&
MixfixParser::getTokenSet()  // HACK
{
  return tokens;
}

MixfixParser::MixfixParser(MixfixModule& client)
  : client(client),
    specialTerminals(Token::LAST_PROPERTY)
{
  bubblesAllowed = false;
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
  static Vector<int> rhs2;

  int rhsLength = rhs.length();
  rhs2.resize(rhsLength);
  for (int i = 0; i < rhsLength; i++)
    {
      int s = rhs[i];
      rhs2[i] = s < 0 ? s : tokens.insert(s);
    }

#if PARSER_DEBUG
  cout << "production: " << lhs << " ::= ";
  for (int i = 0; i < rhs2.length(); i++)
    cout << rhs2[i] << ' ';
  cout << "\t\tprec: " << prec << "\tgather: ";
  for (int i = 0; i < gather.length(); i++)
    cout << gather[i] << ' ';
  cout << "(action = " << action <<
    ", data = " << data <<
    ", data2 = " << data2 <<")\n";
#endif

  parser.insertProd(lhs, rhs2, prec, gather);
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
  int left = (leftParenCode >= 0) ? tokens.insert(leftParenCode) : NONE;
  int right = (rightParenCode >= 0) ? tokens.insert(rightParenCode) : NONE;
  int nrExcluded = excluded.length();
  Vector<int> excludedTerminals(nrExcluded);
  for (int i = 0; i < nrExcluded; i++)
    excludedTerminals[i]= tokens.insert(excluded[i]);

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
MixfixParser::insertSpecialTerminal(int tokenProperty, int codeToUse)
{
  specialTerminals[tokenProperty] = tokens.insert(codeToUse);
}

void
MixfixParser::insertVariableTerminal(int sortNameCode, int codeToUse)
{
  variableTerminals[sortNameCode] = tokens.insert(codeToUse);
}

void
MixfixParser::insertIterSymbolTerminal(int iterSymbolNameCode, int codeToUse)
{
  iterSymbolTerminals[iterSymbolNameCode] = tokens.insert(codeToUse);
}

int
MixfixParser::translateSpecialToken(int code)
{
  int sp = Token::specialProperty(code);
  if (sp == Token::CONTAINS_COLON)
    {
      int varName;
      int sortName;
      Token::split(code, varName, sortName);
      IntMap::const_iterator i = variableTerminals.find(sortName);
      if (i != variableTerminals.end())
	return i->second;
    }
  else if (sp == Token::ITER_SYMBOL)
    {
      int opName;
      mpz_class dummy;
      Token::split(code, opName, dummy);
      IntMap::const_iterator i = iterSymbolTerminals.find(opName);
      if (i != iterSymbolTerminals.end())
	return i->second;
    }
  else if (sp != NONE)
    return specialTerminals[sp];
  if (bubblesAllowed)
    return tokens.cardinality();
  return NONE;
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
  for (int i = 0; i < nrTokens; i++)
    {
      int j = begin + i;
      int code = original[j].code();
      int terminal = tokens.int2Index(code);
      if (terminal == NONE)
	{
	  terminal = translateSpecialToken(code);
	  if (terminal == NONE)
	    {
	      firstBad = j;
	      return -1;
	    }
	}
      sentence[i] = terminal;
    }

#if PARSER_DEBUG
  cout << "parse: ";
  for (int i = 0; i < sentence.length(); i++)
    cout << sentence[i] << ' ';
  cout << ", " << root << '\n';
#endif
  nrParses = parser.parseSentence(sentence, root);
  DebugAdvisoryCheck(nrParses == 1, "MSCP10 returned " << nrParses << " parses");
  if (nrParses < 0)
    nrParses = INT_MAX;  // assume a wrap around error
  else if (nrParses == 0)  // no parse
    {
#ifdef SCP
      int nrErrors = parser.getNumberOfErrors();
      DebugAdvisory("MSCP10 found " << nrErrors << " errors");
      if (nrErrors >= 1)
	{
	  int errorPos = parser.getErrorPosition(1);
	  Assert(errorPos >= 0 && errorPos <= nrTokens,
		 "parser return bad error position " << errorPos);
	  firstBad = begin + errorPos;
	}
      else
	firstBad = begin + nrTokens;  // this shouldn't happen but it does :(
#else
      firstBad = begin + nrTokens;  // HACK
#endif
    }
#if PARSER_DEBUG
  parser.printCurrentParse();
#endif
  return nrParses;
}

void
MixfixParser::makeTerms(Term*& first, Term*& second)
{
  Assert(nrParses > 0, "no parses");
  int node = ROOT_NODE;
  first = makeTerm(node);
  second = 0;
  if (nrParses > 1)
    {
#ifdef SCP
      (void) parser.nextAnalysis();
#else
      (void) parser.extractNextParse();
#endif
      second  = makeTerm(node);
    }
}

void
MixfixParser::insertStatement()
{
  Assert(nrParses > 0, "no parses");
  makeStatement(ROOT_NODE);
}

void
MixfixParser::makeMatchCommand(Term*& pattern,
			       Term*& subject,
			       Vector<ConditionFragment*>& condition)
{
  Assert(nrParses > 0, "no parses");
  int node = ROOT_NODE;
  int matchPair = parser.getChild(node, 0);
  pattern = makeTerm(parser.getChild(matchPair, 0));
  subject = makeTerm(parser.getChild(matchPair, 1));

  if (actions[parser.getProductionNumber(node)].action == CONDITIONAL_COMMAND)
    makeCondition(parser.getChild(node, 2), condition);
}

void
MixfixParser::makeUnifyCommand(Vector<Term*>& lhs, Vector<Term*>& rhs)
{
  Assert(nrParses > 0, "no parses");
  Assert(lhs.empty() && rhs.empty(), "return vectors should be empty");

  for (int node = ROOT_NODE;; node = parser.getChild(node, 1))
    {
      int unifyPair = parser.getChild(node, 0);
      lhs.append(makeTerm(parser.getChild(unifyPair, 0)));
      rhs.append(makeTerm(parser.getChild(unifyPair, 1)));
      if (actions[parser.getProductionNumber(node)].action != UNIFY_LIST)
	break;
    }
}

void
MixfixParser::makeSearchCommand(Term*& initial,
				int& searchType,
				Term*& target,
				Vector<ConditionFragment*>& condition)
{
  Assert(nrParses > 0, "no parses");
  int node = ROOT_NODE;
  int searchPair = parser.getChild(node, 0);
  initial = makeTerm(parser.getChild(searchPair, 0));
  int arrowType = parser.getChild(searchPair, 1);
  searchType = actions[parser.getProductionNumber(arrowType)].data;
  target = makeTerm(parser.getChild(searchPair, 2));

  if (actions[parser.getProductionNumber(node)].action == CONDITIONAL_COMMAND)
    makeCondition(parser.getChild(node, 2), condition);
}

void
MixfixParser::makeStrategyCommand(Term*& subject, StrategyExpression*& strategy)
{
  Assert(nrParses > 0, "no parses");
  int node = ROOT_NODE;
  int term = parser.getChild(node, 0);
  int strat = parser.getChild(node, 1);
  subject = makeTerm(term);
  strategy = makeStrategy(strat);
}

void
MixfixParser::makeAssignment(int node, Vector<Term*>& variables, Vector<Term*>& values)
{
  Term* var = makeTerm(parser.getChild(node, 0));
  if (dynamic_cast<VariableTerm*>(var))
    {
      Term* val = makeTerm(parser.getChild(node, 1));
      variables.append(var);
      values.append(val);
    }
  else
    {
      IssueWarning(*var << ": " << var << " is not a variable - ignoring assignment.");
      var->deepSelfDestruct();
    }
}

void
MixfixParser::makeSubstitution(int node, Vector<Term*>& variables, Vector<Term*>& values)
{
  while (actions[parser.getProductionNumber(node)].action == MAKE_SUBSTITUTION)
    {
      makeAssignment(parser.getChild(node, 0), variables, values);
      node = parser.getChild(node, 1);
    }
  Assert(actions[parser.getProductionNumber(node)].action == PASS_THRU, "unexpected action");
  makeAssignment(parser.getChild(node, 0), variables, values);
}

StrategyExpression*
MixfixParser::makeStrategy(int node)
{
  StrategyExpression* s;
  Action& a = actions[parser.getProductionNumber(node)];
  switch (a.action)
    {
    case PASS_THRU:
      {
	return makeStrategy(parser.getChild(node, 0));
      }
    case MAKE_TRIVIAL:
      {
	s = new TrivialStrategy(a.data);
	break;
      }
    case MAKE_ALL:
      {
	Vector<Term*> variables;
	Vector<Term*> values;
	Vector<StrategyExpression*> strategies;
	s = new ApplicationStrategy(UNDEFINED, variables, values, strategies);
	break;
      }
    case MAKE_APPLICATION:
      {
	int label = actions[parser.getProductionNumber(parser.getChild(node, 0))].data;
	Vector<Term*> variables;
	Vector<Term*> values;
	Vector<StrategyExpression*> strategies;
	int child = 1;
	if (a.data)
	  {
	    makeSubstitution(parser.getChild(node, 1), variables, values);
	    ++child;
	  }
	if (a.data2)
	  makeStrategyList(parser.getChild(node, child), strategies);
	s = new ApplicationStrategy(label, variables, values, strategies);
	break;
      }
    case MAKE_TOP:
      {
	s = makeStrategy(parser.getChild(node, 0));
	if (ApplicationStrategy* a = dynamic_cast<ApplicationStrategy*>(s))
	  a->setTop();
	else
	  {
	    int pos = currentOffset + parser.getFirstPosition(node);
	    IssueWarning(LineNumber((*currentSentence)[pos].lineNumber()) <<
			 ": use of top strategy modifier on a non-application strategy ignored.");
	  }
	break;
      }
    case MAKE_CONCATENATION:
    case MAKE_UNION:
      {
	Vector<StrategyExpression*> strategies;
	do
	  {
	    strategies.append(makeStrategy(parser.getChild(node, 0)));
	    node = parser.getChild(node, 1);
	  }
	while (actions[parser.getProductionNumber(node)].action == a.action);
	strategies.append(makeStrategy(node));
	if (a.action == MAKE_CONCATENATION)
	  s = new ConcatenationStrategy(strategies);
	else
	  s = new UnionStrategy(strategies);
	break;
      }
    case MAKE_ITERATION:
      {
	s = new IterationStrategy(makeStrategy(parser.getChild(node, 0)),
				  actions[parser.getProductionNumber(node)].data);
	break;
      }
    case MAKE_BRANCH:
      {
	BranchStrategy::Action successAction = static_cast<BranchStrategy::Action>(actions[parser.getProductionNumber(node)].data);
	BranchStrategy::Action failureAction = static_cast<BranchStrategy::Action>(actions[parser.getProductionNumber(node)].data2);
	int child = 0;
	StrategyExpression* successStrategy =
	  (successAction == BranchStrategy::NEW_STRATEGY) ? makeStrategy(parser.getChild(node, ++child)) : 0;
	StrategyExpression* failureStrategy =
	  (failureAction == BranchStrategy::NEW_STRATEGY) ? makeStrategy(parser.getChild(node, ++child)) : 0;
	s = new BranchStrategy(makeStrategy(parser.getChild(node, 0)),
			       successAction,
			       successStrategy,
			       failureAction,
			       failureStrategy);
	break;
      }
    case MAKE_TEST:
      {
	Vector<ConditionFragment*> condition;
	if (parser.getNumberOfChildren(node) > 1)  // such that clause
	  makeCondition(parser.getChild(node, 2), condition);
	s = new TestStrategy(makeTerm(parser.getChild(node, 0)), actions[parser.getProductionNumber(node)].data, condition);
	break;
      }
    default:
      {
	s = 0;  // to avoid uninitialized variable warning
	CantHappen("bad action " << a.action);
      }
    }
  return s;
}

void
MixfixParser::makeStrategyList(int node, Vector<StrategyExpression*>& strategies)
{
  while (actions[parser.getProductionNumber(node)].action == MAKE_STRATEGY_LIST)
    {
      strategies.append(makeStrategy(parser.getChild(node, 0)));
      node = parser.getChild(node, 1);
    }
  Assert(actions[parser.getProductionNumber(node)].action == PASS_THRU, "unexpected action");
  strategies.append(makeStrategy(parser.getChild(node, 0)));
}

Sort*
MixfixParser::getSort(int node)
{
  return client.getSorts()[actions[parser.getProductionNumber(node)].data];
}

Term*
MixfixParser::makeTerm(int node)
{
  Term* t;
  Vector<Term*> args;

  int pos = currentOffset + parser.getFirstPosition(node);
  Action& a = actions[parser.getProductionNumber(node)];

  switch (a.action)
    {
    case PASS_THRU:
      {
	return makeTerm(parser.getChild(node, 0));
      }
    case MAKE_TERM:
      {
	Symbol* symbol = client.getSymbols()[a.data];
	int nrArgs = symbol->arity();
	if (nrArgs > 0)
	  {
	    int t = parser.getChild(node, 0);
	    if (actions[parser.getProductionNumber(t)].action == ASSOC_LIST)
	      {
		/*
		  do
		  {
		  args.append(makeTerm(parser.getChild(t, 0)));
		  t = parser.getChild(t, 1);
		  }
		  while (actions[parser.getProductionNumber(t)] == ASSOC_LIST);
		  args.append(makeTerm(t));
		  */
		do
		  {
		    args.append(makeTerm(parser.getChild(t, 1)));
		    t = parser.getChild(t, 0);
		  }
		while (actions[parser.getProductionNumber(t)].action == ASSOC_LIST);
		args.append(makeTerm(t));
		int n = args.length() - 1;
		for (int i = n / 2; i >= 0; i--)
		  {
		    Term* t = args[i];
		    args[i] = args[n - i];
		    args[n - i] = t;
		  }
	      }
	    else
	      {  
		for (int i = 0; i < nrArgs; i++)
		  args.append(makeTerm(parser.getChild(node, i)));
	      }
	  }
	t = symbol->makeTerm(args);
	break;
      }
    case MAKE_NATURAL:
      {
	SuccSymbol* symbol = safeCast(SuccSymbol*, client.getSymbols()[a.data]);
	mpz_class nat((*currentSentence)[pos].name(), 10);
	t = symbol->makeNatTerm(nat);
	break;
      }
    case MAKE_INTEGER:
      {
	MinusSymbol* symbol = safeCast(MinusSymbol*, client.getSymbols()[a.data]);
	mpz_class integer((*currentSentence)[pos].name(), 10);
	t = symbol->makeIntTerm(integer);
	break;
      }
    case MAKE_RATIONAL:
      {
	DivisionSymbol* symbol = safeCast(DivisionSymbol*, client.getSymbols()[a.data]);
	const char* name = (*currentSentence)[pos].name();
	char* s = new char[strlen(name) + 1];
	strcpy(s, name);
	char* p = index(s, '/');
	Assert(p != 0, "no /");
	*p = '\0';
	mpz_class numerator(s, 10);
	mpz_class denominator(p + 1, 10);
	delete [] s;
	t = symbol->makeRatTerm(numerator, denominator);
	break;
      }
    case MAKE_FLOAT:
      {
	FloatSymbol* symbol =
	  static_cast<FloatSymbol*>(client.getSymbols()[a.data]);
	double value = Token::codeToDouble((*currentSentence)[pos].code());
	t = new FloatTerm(symbol, value);
	break;
      }
    case MAKE_STRING:
      {
	StringSymbol* symbol = static_cast<StringSymbol*>(client.getSymbols()[a.data]);
	t = new StringTerm(symbol, Token::codeToRope((*currentSentence)[pos].code()));
	break;
      }
    case MAKE_QUOTED_IDENTIFIER:
      {
	QuotedIdentifierSymbol* symbol =
	  static_cast<QuotedIdentifierSymbol*>(client.getSymbols()[a.data]);
	const char* name = (*currentSentence)[pos].name();
	t = new QuotedIdentifierTerm(symbol, Token::encode(name + 1));  // strip the quote
	break;
      }
    case MAKE_SORT_TEST:
      {
	int colonPair = parser.getChild(node, 0);
	args.append(makeTerm(parser.getChild(colonPair, 0)));
	Sort* sort = getSort(parser.getChild(colonPair, 1));
	t = client.instantiateSortTest(sort, a.data)->makeTerm(args);
	break;
      }
    case MAKE_VARIABLE:
      {
	Sort* sort = client.getSorts()[a.data];
	VariableSymbol* symbol = safeCast(VariableSymbol*, client.instantiateVariable(sort));
	int varName;
	int sortName;
	Token::split((*currentSentence)[pos].code(), varName, sortName);
	Assert(sortName == NONE ||
	       sortName == sort->id() ||
	       Token::auxProperty(sort->id()) == Token::AUX_STRUCTURED_SORT,
	       "sort name clash");
	t = new VariableTerm(symbol, varName);
	break;
      }
    case MAKE_VARIABLE_FROM_ALIAS:
      {
	Sort* sort = client.getSorts()[a.data];
	VariableSymbol* symbol = safeCast(VariableSymbol*, client.instantiateVariable(sort));
	t = new VariableTerm(symbol, (*currentSentence)[pos].code());
	break;
      }
    case MAKE_POLYMORPH:
      {
	Symbol* symbol = client.instantiatePolymorph(a.data2, a.data);
	int nrArgs = symbol->arity();
	for (int i = 0; i < nrArgs; i++)
	  args.append(makeTerm(parser.getChild(node, i)));
	t = symbol->makeTerm(args);
	break;
      }
    case MAKE_ITER:
      {
	S_Symbol* symbol = safeCast(S_Symbol*, client.getSymbols()[a.data]);
	int opName;
	mpz_class number;
	Token::split((*currentSentence)[pos].code(), opName, number);
	Assert(opName == symbol->id(), "iter symbol name clash");
	Term* arg = makeTerm(parser.getChild(node, 1));
	t = new S_Term(symbol, number, arg);
	break;
      }
#ifdef BUBBLES
    case MAKE_BUBBLE:
      {
	return client.makeBubble(a.data,
				 *currentSentence,
				 currentOffset + parser.getFirstPosition(node),
				 currentOffset + parser.getLastPosition(node) - 1);  // HACK last position
      }
#endif
    default:
      {
	CantHappen("bad action");
	return 0;  // to avoid uninitialized variable warning
      }
    }
  t->setLineNumber((*currentSentence)[pos].lineNumber());
  return t;
}

void
MixfixParser::makeCondition(int node, Vector<ConditionFragment*>& condition)
{
  while (actions[parser.getProductionNumber(node)].action == FRAGMENT_LIST)
    {
      condition.append(makeConditionFragment(parser.getChild(node, 0)));
      node = parser.getChild(node, 1);
    }
  condition.append(makeConditionFragment(parser.getChild(node, 0)));
}

ConditionFragment*
MixfixParser::makeConditionFragment(int node)
{
  ConditionFragment* f;
  Action& a = actions[parser.getProductionNumber(node)];
  switch (a.action)
    {
    case PASS_THRU:
      {
	f = makeConditionFragment(parser.getChild(node, 0));
	break;
      }
    case MAKE_TRUE:
      {
	f = new EqualityConditionFragment(makeTerm(parser.getChild(node, 0)),
					  client.makeTrueTerm());
	break;
      }
    case MAKE_EQUALITY:
      {
	int equalityPair = parser.getChild(node, 0);
	f = new EqualityConditionFragment(makeTerm(parser.getChild(equalityPair, 0)),
					  makeTerm(parser.getChild(equalityPair, 1)));
	break;
      }
    case MAKE_SORT_TEST:
      {
	int colonPair = parser.getChild(node, 0);
	f = new SortTestConditionFragment(makeTerm(parser.getChild(colonPair, 0)),
					  getSort(parser.getChild(colonPair, 1)));
	break;
      }
    case MAKE_ASSIGNMENT:
      {
	int assignPair = parser.getChild(node, 0);
	f = new AssignmentConditionFragment(makeTerm(parser.getChild(assignPair, 0)),
					    makeTerm(parser.getChild(assignPair, 1)));
	break;
      }
    case MAKE_REWRITE:
      {
	int arrowPair = parser.getChild(node, 0);
	f = new RewriteConditionFragment(makeTerm(parser.getChild(arrowPair, 0)),
					 makeTerm(parser.getChild(arrowPair, 1)));
	break;
      }
    default:
      {
	f = 0;  // to avoid uninitialized variable warning
	CantHappen("bad action");
      }
    }
  return f;
}

void
MixfixParser::makeStatement(int node)
{
  int label = NONE;
  int metadata = NONE;
  FlagSet flags;
  Vector<int> printNames;
  Vector<Sort*> printSorts;
  if (actions[parser.getProductionNumber(node)].action == MAKE_ATTRIBUTE_PART)
    makeAttributePart(parser.getChild(node, 1), label, metadata, flags, printNames, printSorts);
  makeStatementPart(parser.getChild(node, 0), label, metadata, flags, printNames, printSorts);
}

void
MixfixParser::makeAttributePart(int node,
				int& label,
				int& metadata,
				FlagSet& flags,
				Vector<int>& printNames,
				Vector<Sort*>& printSorts)
{
  for (int listNode = parser.getChild(node, 0);; listNode = parser.getChild(listNode, 1))
    {
      int attrNode = parser.getChild(listNode, 0);
      switch (actions[parser.getProductionNumber(attrNode)].action)
	{
	case MAKE_LABEL_ATTRIBUTE:
	  {
	    int labelNode = parser.getChild(attrNode, 0);
	    label = actions[parser.getProductionNumber(labelNode)].data;
	    break;
	  }
	case MAKE_METADATA_ATTRIBUTE:
	  {
	    int metaDataNode = parser.getChild(attrNode, 0);
	    int pos = currentOffset + parser.getFirstPosition(metaDataNode);
	    metadata = (*currentSentence)[pos].code();
	    break;
	  }
	case MAKE_NONEXEC_ATTRIBUTE:
	  {
	    flags.setFlags(NONEXEC);
	    break;
	  }
	case MAKE_OWISE_ATTRIBUTE:
	  {
	    flags.setFlags(OWISE);
	    break;
	  }
	case MAKE_PRINT_ATTRIBUTE:
	  {
	    flags.setFlags(PRINT);
	    if (parser.getNumberOfChildren(attrNode) > 0)  // nonempty
	      makePrintList(parser.getChild(attrNode, 0), printNames, printSorts);
	    break;
	  }
	}
      if (actions[parser.getProductionNumber(listNode)].action != MAKE_ATTRIBUTE_LIST)
	break;
    }
}

void
MixfixParser::makePrintList(int node, Vector<int>& names, Vector<Sort*>& sorts)
{
  typedef pair<int, int> IntPair;  // HACK
  for (int listNode = node;; listNode = parser.getChild(listNode, 1))
    {
      //
      //	listNode is either
      //	  <PRINT_LIST> ::= <PRINT_ITEM>
      //	or
      //	  <PRINT_LIST> ::= <PRINT_ITEM> <PRINT_LIST>
      //
      //	printItemNode is either
      //	  <PRINT_ITEM> ::= <STRING_NT>
      //	or
      //	  <PRINT_ITEM> ::= <VARIABLE>
      //
      int printItemNode = parser.getChild(listNode, 0);

      switch (actions[parser.getProductionNumber(printItemNode)].action)
	{
	case MAKE_STRING:
	  {
	    int pos = currentOffset + parser.getFirstPosition(printItemNode);
	    int code = (*currentSentence)[pos].code();
	    DebugAdvisory("string = "  << (*currentSentence)[pos]);
	    names.append(code);
	    sorts.append(0);
	    break;
	  }
	case MAKE_VARIABLE:
	  {
	    //
	    //	The sort name is embedded in the action but we may have the get
	    //	the variable base name by splitting a token.
	    //
	    int variableNode = parser.getChild(printItemNode, 0);
	    DebugAdvisory("variableNode = "  << variableNode);
	    Action& a = actions[parser.getProductionNumber(variableNode)];
	    int pos = currentOffset + parser.getFirstPosition(variableNode);
	    int varName = (*currentSentence)[pos].code();
	    if (a.action != MAKE_VARIABLE_FROM_ALIAS)
	      {
		//
		//	Full variable name given, need to split of base.
		//
		int baseName;
		int sortName;
		Token::split(varName, baseName, sortName);
		varName = baseName;
	      }
	    names.append(varName);
	    sorts.append(client.getSorts()[a.data]);
	    break;
	  }
	default:
	  CantHappen("unexpected item in print list");
	}
      if (actions[parser.getProductionNumber(listNode)].action != MAKE_PRINT_LIST)
	break;
    }
}

void
MixfixParser::makeStatementPart(int node,
				int label,
				int metadata,
				FlagSet& flags,
				const Vector<int>& printNames,
				const Vector<Sort*>& printSorts)
{
  Vector<ConditionFragment*> condition;
  int action = actions[parser.getProductionNumber(node)].action;
  if (action == MAKE_CMB || action == MAKE_CEQ || action == MAKE_CRL)
    makeCondition(parser.getChild(node, 1), condition);

  int bodyNode = parser.getChild(node, 0);
  int pairNode = parser.getChild(bodyNode, 0);
  if (actions[parser.getProductionNumber(bodyNode)].action == MAKE_LABEL)
    {
      label = actions[parser.getProductionNumber(pairNode)].data;
      pairNode = parser.getChild(bodyNode, 1);
    }

  //
  //	Get line number of statement keyword.
  //
  int lineNumber = (*currentSentence)[currentOffset].lineNumber();

  switch (action)
    {
    case MAKE_MB:
    case MAKE_CMB:
      {
	WarningCheck(!(flags.getFlag(OWISE)),
		     LineNumber(lineNumber) <<
		     ": owise attribute not allowed for membership axioms.");
	Term* lhs = makeTerm(parser.getChild(pairNode, 0));
	Sort* sort = getSort(parser.getChild(pairNode, 1));
	SortConstraint* sc = new SortConstraint(label, lhs, sort, condition);
	if (flags.getFlag(NONEXEC))
	  sc->setNonexec();
	sc->setLineNumber(lineNumber);
	client.insertSortConstraint(sc);
	if (metadata != NONE)
	  client.insertMetadata(MixfixModule::MEMB_AX, sc, metadata);
	if (flags.getFlag(PRINT))
	  client.insertPrintAttribute(MixfixModule::MEMB_AX, sc, printNames, printSorts);
	break;
      }
    case MAKE_EQ:
    case MAKE_CEQ:
      {
	Term* lhs = makeTerm(parser.getChild(pairNode, 0));
	Term* rhs = makeTerm(parser.getChild(pairNode, 1));
	Equation* eq = new Equation(label, lhs, rhs, flags.getFlag(OWISE), condition);
	if (flags.getFlag(NONEXEC))
	  eq->setNonexec();
	eq->setLineNumber(lineNumber);
	client.insertEquation(eq);
	if (metadata != NONE)
	  client.insertMetadata(MixfixModule::EQUATION, eq, metadata);
	if (flags.getFlag(PRINT))
	  client.insertPrintAttribute(MixfixModule::EQUATION, eq, printNames, printSorts);
	break;
      }
    case MAKE_RL:
    case MAKE_CRL:
      {
	WarningCheck(!(flags.getFlag(OWISE)),
		     LineNumber(lineNumber) <<
		     ": owise attribute not allowed for rules.");
	Term* lhs = makeTerm(parser.getChild(pairNode, 0));
	Term* rhs = makeTerm(parser.getChild(pairNode, 1));
	Rule* rl = new Rule(label, lhs, rhs, condition);
	if (flags.getFlag(NONEXEC))
	  rl->setNonexec();
	rl->setLineNumber(lineNumber);
	client.insertRule(rl);
	if (metadata != NONE)
	  client.insertMetadata(MixfixModule::RULE, rl, metadata);
	if (flags.getFlag(PRINT))
	  client.insertPrintAttribute(MixfixModule::RULE, rl, printNames, printSorts);
	break;
      }
    default:
      CantHappen("bad action");
    }
}
