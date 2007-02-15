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
//      Class for mixfix parser with actions.
//
#ifndef _mixfixParser_hh_
#define _mixfixParser_hh_
#include <map>
#ifdef SCP
#include "scp_parser.hh"
#else
#include "parser.hh"
#endif
#include "intSet.hh"
#include "token.hh"

class MixfixParser
{
  NO_COPYING(MixfixParser);

public:
  enum SemanticActions
  {
    NOP,
    PASS_THRU,
    //
    //	Term construction actions.
    //
    MAKE_TERM,
    ASSOC_LIST,
    //
    //	Special kinds of terms.
    //
    MAKE_VARIABLE,
    MAKE_VARIABLE_FROM_ALIAS,
    MAKE_NATURAL,
    MAKE_INTEGER,
    MAKE_RATIONAL,
    MAKE_FLOAT,
    MAKE_QUOTED_IDENTIFIER,
    MAKE_STRING,
    MAKE_SORT_TEST,  // HACK used for condition fragment as well
    MAKE_POLYMORPH,
    MAKE_ITER,
    MAKE_BUBBLE,
    //
    //	Statement construction actions.
    //
    MAKE_MB,
    MAKE_CMB,
    MAKE_EQ,
    MAKE_CEQ,
    MAKE_RL,
    MAKE_CRL,
    MAKE_LABEL,
    MAKE_ATTRIBUTE_PART,
    //
    //	Condition construction actions.
    //
    FRAGMENT_LIST,
    MAKE_TRUE,
    MAKE_EQUALITY,
    MAKE_ASSIGNMENT,
    MAKE_REWRITE,
    //
    //	Attribute construction actions.
    //
    MAKE_LABEL_ATTRIBUTE,
    MAKE_METADATA_ATTRIBUTE,
    MAKE_NONEXEC_ATTRIBUTE,
    MAKE_OWISE_ATTRIBUTE,
    MAKE_ATTRIBUTE_LIST,
    //
    //	Command construction actions.
    //
    CONDITIONAL_COMMAND,
    //
    //	Strategy expression construction actions
    //
    MAKE_TRIVIAL,
    MAKE_ALL,
    MAKE_APPLICATION,
    MAKE_TOP,
    MAKE_CONCATENATION,
    MAKE_UNION,
    MAKE_ITERATION,
    MAKE_BRANCH,
    MAKE_TEST,
    MAKE_STRATEGY_LIST,

    MAKE_SUBSTITUTION
  };

  MixfixParser(MixfixModule& client);
  //
  //	Functions to construct parser.
  //
  void insertProduction(int lhs,
			const Vector<int>& rhs,
			int prec,
			const Vector<int>& gather,
			int action = NOP,
			int data = NONE,
			int data2 = NONE);
  void insertBubbleProduction(int lhs,
			      int lowerBound,
			      int upperBound,
			      int leftParenCode,
			      int rightParenCode,
			      const Vector<int>& excluded,
			      int bubbleSpecIndex);
  void insertSpecialTerminal(int tokenProperty, int codeToUse);
  void insertVariableTerminal(int sortNameCode, int codeToUse);
  void insertIterSymbolTerminal(int iterSymbolNameCode, int codeToUse);
  //
  //	Function that parses a vector of tokens to a parse tree.
  //
  int parseSentence(const Vector<Token>& original,
		    int root,
		    int& firstBad,
		    int begin,
		    int nrTokens);
  //
  //	Functions that analyse the parse tree.
  //
  void makeTerms(Term*& first, Term*& second);
  void insertStatement();
  void makeMatchCommand(Term*& pattern,
			Term*& subject,
			Vector<ConditionFragment*>& condition);
  void makeUnifyCommand(Term*& lhs, Term*& rhs);
  void makeSearchCommand(Term*& initial,
			 int& searchType,
			 Term*& target,
			 Vector<ConditionFragment*>& condition);
  void makeStrategyCommand(Term*& subject, StrategyExpression*& strategy);

  void makeAssignment(int node, Vector<Term*>& variables, Vector<Term*>& values);
  void makeSubstitution(int node, Vector<Term*>& variables, Vector<Term*>& values);

  //
  //	Functions to get info about the parser.
  //
  const IntSet& getTokenSet();  // HACK
  int getNrProductions();

private:
  typedef map<int,int> IntMap;
  enum Flags
  {
    NONEXEC = 1,
    OWISE = 2
  };

  struct Action
  {
    int data;		// main data
    short action;	// action to take on data
    short data2;	// auxillary data for actions that need it
  };

  Sort* getSort(int node);
  Term* makeTerm(int node);
  StrategyExpression* makeStrategy(int node);
  ConditionFragment* makeConditionFragment(int node);
  void makeCondition(int node, Vector<ConditionFragment*>& condition);
  void makeStatement(int node);
  void makeAttributePart(int node, int& label, int& metadata, FlagSet& flags);
  void makeStatementPart(int node, int label, int metadata, FlagSet& flags);
  void makeStrategyList(int node, Vector<StrategyExpression*>& strategies);

  int translateSpecialToken(int code);

  MixfixModule& client;
  Parser parser;			// CFG parser
  IntSet tokens;			// mapping between token codes and terminal numbers
  Vector<Action> actions;		// action associated with each production
  Vector<int> specialTerminals;		// special terminals for tokens with special properties
  IntMap variableTerminals;		// special terminals for on-the-fly variables
  IntMap iterSymbolTerminals;		// special terminals for tokens like f^42
  bool bubblesAllowed;			// do we allow bubbles of unknown tokens
  //
  //	We store the tokens we are parsing here to avoid passing extra parameters
  //	when recursing down a parse tree.
  //
  const Vector<Token>* currentSentence;	// actual tokens so we can deal with special tokens
  int currentOffset;			// start of parsed tokens
  Vector<int> sentence;			// sentence translated into terminal numbers
  int nrParses;
};

inline int
MixfixParser::getNrProductions()
{
  return actions.length();
}

#endif
