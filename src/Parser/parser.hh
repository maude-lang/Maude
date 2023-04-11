/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017-2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for Earley-Leo style general context free parser with extreme
//	prunning heuristic; we do not support epsilon productions.
//
//	We extend the algorithm in:
//	  Joop M. I. M. Leo, A general context-free parsing algorithm running in linear
//	  time on every LR(k) grammar without using lookahead, Theoretical Computer Science,
//	  83:165-176, 1991.
//	to handle the prec/gather and bubble extensions needed by Maude.
//
#ifndef _parser_hh_
#define _parser_hh_
#include <map>

class Parser
{
public:
  Parser();

  void insertProd(int nonTerminal,
		  const Vector<int>& rhs,
		  const int prec,
		  const Vector<int>& gather);
  void insertProd(int nonTerminal,
		  int lowerBound,
		  int upperBound,
		  int leftParen,
		  int rightParen,
		  const Vector<int>& excludedTerminals);

  int parseSentence(const Vector<int>& sentence, int root);
  bool extractNextParse();
  int getProductionNumber(int node);
  int getChild(int node, int sonNr);
  int getFirstPosition(int node);
  int getLastPosition(int node);  // actually one beyond last token
  void dump();

  int getNumberOfChildren(int node);

//
//	If no parses are found, this returns the index of the first token at which
//	no legal parse is possible, or sentence.size() if we ran out of tokens
//	with parses still possible.
//
  int getErrorPosition();

private:
  enum Special
    {
      BUBBLE_TERMINAL = 1000000000  // special terminal symbol for bubbles
    };

  struct Pair
  {
    int symbol;
    int prec;
  };

  struct Expansion
  {
    int prec;
    int firstExpansionCall;
  };

  struct Rule
  {
    union
    {
      int nextRule;		// for lists of rules with the same rhs[0] symbol; we're done with this by the time we fill out equal
      int equal;		// |
    };				// |
    int smaller;		//  } indices of rules for 3 branches of decision tree
    int bigger;			// |
    int prec;
    int nonTerminal;
    int nrNonTerminals;		// number of nonterminals appear in rhs
    Vector<Pair> rhs;
  };

  struct Bubble
  {
    int nonTerminal;
    int lowerBound;
    int upperBound;
    int leftParen;
    int rightParen;
    int ruleNr;  // for rule we need to fix up
    int nextBubble;  // for the same nonterminal
    Vector<int> excludedTerminals;
  };

  struct Continuation
  {
    int ruleNr;
    int rhsPosition;
    int startTokenNr;
    int nextContinuation;
  };

  struct Call
  {
    int nonTerminal;
    int maxPrec;
    int firstContinuation;
    int nextCall;
  };

  struct Return
  {
    int ruleNr;
    int startTokenNr;
    int nextReturn;
  };

  struct DeferredReturn
  {
    //
    //	A Deterministic Reduction Path is instantiated as a Vector of deferred returns.
    //
    int ruleNr;
    int startTokenNr;
  };

  struct MemoItem
  {
    //
    //	Entry point to a Deterministic Reduction Path.
    //
    //	The first two items are what we are looking for when we encounter the path.
    //
    int nonTerminal;
    int maxPrec;
    //
    //	The next two items are the final return at the top of the path.
    //
    int ruleNr;
    int startTokenNr;
    //
    //	The final item is a link to the next MemoItem at this token position.
    //
    int nextMemoItem;
  };

  struct ParseNode
  {
    int ruleNr;
    int startTokenNr;
    int nextReturnToCheck;
    int endTokenNr;
    int firstSon;
    int nextSibling;
  };

  static int flip(int i);
  static bool ruleLt(Rule* const& r1, Rule* const& r2);

  typedef pair<int, int> IntPair;
  typedef pair<int, bool> IntBoolPair;
  typedef map<int, int> SymbolToPrecMap;
  typedef map<int, SymbolToPrecMap> PrecToExpansionMap;
  typedef Vector<PrecToExpansionMap> Expansions; 
  //
  //	Compile
  //
  void buildExpansionTables();
  void buildTerminalDecisionTrees();
  void buildNonTerminalDecisionTrees();
  int buildDecisionTree(Vector<Rule*>& ruleTable, Vector<int>& starts, int first, int last);
  bool mergeInMap(SymbolToPrecMap& target, const SymbolToPrecMap& donor, int targetNonterminal, int targetPrec);
  int makeExpansionList(const SymbolToPrecMap mapForm, int nonterminal);
  void dumpMap(PrecToExpansionMap& m, int nonterminal);
  //
  //	Pass 1.
  //
  void doReturns(int tokenNr, const Vector<int>& sentence);
  void processReturn(int tokenNr, int startTokenNr, int ruleNr, const Vector<int>& sentence);
  void expandCalls(int tokenNr);
  void scanCalls(int tokenNr, const Vector<int>& sentence);
  void advanceRule(int ruleNr,
		   int pos,
		   int startTokenNr,
		   int tokenNr,
		   const Vector<int>& sentence);
  void makeCall(int tokenNr, int ruleNr, int rhsPosition, int startTokenNr);
  bool makeReturn(int tokenNr, int ruleNr, int startTokenNr);
  IntPair chaseDeterministicReductionPath(int ruleNr, int startTokenNr);
  int makeMemoItem(int nonTerminal, int maxPrec, int ruleNr, int startTokenNr, int nextMemoItem);
  void processBubble(int tokenNr, int bubbleNr, const Vector<int>& sentence);
  void doBubbles(int tokenNr, const Vector<int>& sentence);
  void doEmptyBubbles(int tokenNr);
  void checkForEmptyBubbles(int tokenNr, const Vector<int>& sentence);
  void doEmptyBubbleReturns(int tokenNr, const Vector<int>& sentence);
  //
  //	Pass 2.
  //
  int findRootReturn(int i, int nonTerminal);
  int findReturn(int i, int ruleNr, int rhsPosition, int startTokenNr);
  bool existsCall(int parseListNr, int ruleNr, int rhsPosition, int startTokenNr);
  bool existsMemo(int startTokenNr, int nonTerminal, int prec, int ruleNrToFind, int startTokenNrToFind);
  IntBoolPair findReturnOrDeterministicReductionPath(int i, int ruleNr, int startTokenNr, int endTokenNr);
  void extractFirstSubparse(int nodeNr);
  bool extractNextSubparse(int nodeNr);
  void extractFirstSubparseToTheLeft(int nodeNr, int rightPos, int rightSibling);
  int buildDeterministicReductionPathParseTree(int endTokenNr,
					       int triggeReturnIndex,
					       const Vector<DeferredReturn>& drp,
					       int drpIndex);
  IntPair extractOneStepOfDeterministicReductionPath(int ruleNr, int startTokenNr);
  void extractDeterministicReductionPath(int ruleNr, int startTokenNr, Vector<DeferredReturn>& drp);
  void buildDeterministicReductionPathTree(int endTokenNr, int triggerRet, const Vector<DeferredReturn>& drp, int drpIndex);
  bool returnExists(int i, int ruleNr, int startTokenNr);

  bool haveBubbles;
  bool haveEmptyBubbles;
  Vector<Bubble> bubbles;
  //
  //	Compile time structures.
  //
  Vector<int> firstTerminalRules;	// first terminal rule for each nonterminal
  Vector<int> firstNonTerminalRules;	// first nonterminal rule for each nonterminal
  Vector<int> firstBubbles;		// first bubble for each nonterminal
  Vector<Rule> rules;			// all the grammar rules
  Vector<Vector<Expansion> > expansions;	// table of expansions for each nonterminal
  Vector<int> terminalDecisionTrees;
  Vector<int> nonTerminalDecisionTrees;
  int firstRealCall;  // index of first call that isn't used for expansion tables
  //
  //	Parse time structures.
  //
  int rootNonTerminal;
  bool ambiguous;		// more than one parse seen
  int badTokenIndex;		// one past the last token for which a valid partial parse has been seen
  int lastReturnProcessed;
  Vector<int> firstCalls;
  Vector<int> firstReturns;
  Vector<int> firstMemoItems;
  Vector<Call> calls;
  Vector<Continuation> continuations;
  Vector<Return> returns;
  Vector<MemoItem> memoItems;
  Vector<ParseNode> parseTree;
};

inline int
Parser::flip(int i)
{
  return ~i;  // map nonTerminal number <-> vector index
}

inline bool
Parser::ruleLt(Rule* const& r1, Rule* const& r2)
{
  int r = r1->rhs[0].symbol - r2->rhs[0].symbol;
  if (r != 0)
    return r < 0;
  return r1->prec < r2->prec;
}

inline int
Parser::getNumberOfChildren(int node)
{
  return rules[getProductionNumber(node)].nrNonTerminals;
}

inline int
Parser::getProductionNumber(int node)
{
  return parseTree[node].ruleNr;
}

inline int
Parser::getFirstPosition(int node)
{
  return parseTree[node].startTokenNr;
}

inline int
Parser::getLastPosition(int node)
{
  return parseTree[node].endTokenNr;
}

inline int
Parser::getErrorPosition()
{
  return badTokenIndex;
}

#endif
