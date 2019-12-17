//
//      Implementation for class Parser
//
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"
#include "parser.hh"

//#define PARSER_DEBUG

#ifdef PARSER_DEBUG
#define ParserLog(message) (cout << "ParserLog: " << message << endl)
#else
#define ParserLog(message)
#endif

//	our stuff
#include "compile.cc"
#include "compile2.cc"
#include "pass1.cc"
#include "bubble.cc"
#include "pass2.cc"
#include "drp.cc"

Parser::Parser()
{
  haveBubbles = false;
  haveEmptyBubbles = false;
}

Parser::~Parser()
{
  FOR_EACH_CONST(i, Vector<Rule*>, rules)
    delete *i;
}

void
Parser::insertProd(int nonTerminal,
		   const Vector<int>& rhs,
		   const int prec,
		   const Vector<int>& gather)
{
  int nrNonTerminals = firstTerminalRules.length();
  int index = flip(nonTerminal);
  Assert(index >= 0, "Bad non terminal");
  //
  //	Expand tables to encompass new nonterminal if necessary.
  //
  if (index >= nrNonTerminals)
    {
      firstTerminalRules.expandTo(index + 1);
      firstNonTerminalRules.expandTo(index + 1);
      firstBubbles.expandTo(index + 1);
      for (; nrNonTerminals <= index; nrNonTerminals++)
	{
	  firstTerminalRules[nrNonTerminals] = NONE;
	  firstNonTerminalRules[nrNonTerminals] = NONE;
	  firstBubbles[nrNonTerminals] = NONE;
	}
    }
  //
  //	Make new rule.
  //
  int nrRules = rules.length();
  rules.expandBy(1);
  Rule* r = new Rule;
  rules[nrRules] = r;
  r->index = nrRules;
  r->prec = prec;
  r->nonTerminal = nonTerminal;

  Assert(!(rhs.empty()), "epsilon rules not supported");
  if (rhs[0] < 0)
    {
      //
      //	Nonterminal case.
      //
      r->nextRule = firstNonTerminalRules[index];
      firstNonTerminalRules[index] = nrRules;
    }
  else
    {
      //
      //	Terminal case.
      //
      r->nextRule = firstTerminalRules[index];
      firstTerminalRules[index] = nrRules;
    }
  //
  //	Copy in rhs with prec derived from gather values.
  //
  int nrSymbols = rhs.length();
  r->rhs.expandTo(nrSymbols);
  int gatherIndex = 0;
  for (int i = 0; i < nrSymbols; i++)
    {
      Pair& p = r->rhs[i];
      p.symbol = rhs[i];
      p.prec = p.symbol < 0 ? gather[gatherIndex++] : NONE;
    }
  r->nrNonTerminals = gatherIndex;
}

void
Parser::insertProd(int nonTerminal,
		   int lowerBound,
		   int upperBound,
		   int leftParen,
		   int rightParen,
		   const Vector<int>& excludedTerminals)
{
  ParserLog("saw bubble added for " << nonTerminal);
  int nrBubbles = bubbles.size();
  bubbles.expandBy(1);
  Bubble& b = bubbles[nrBubbles];
  b.nonTerminal = nonTerminal;
  b.lowerBound = lowerBound;
  b.upperBound = upperBound;
  b.leftParen = leftParen;
  b.rightParen = rightParen;
  b.excludedTerminals = excludedTerminals;  // deep copy
  b.ruleNr = rules.size();
  //
  //	<nonTerminal> ::= built-in-bubble  [prec 0 gather (1)]
  //
  //	This is the rule we will report as a bubble in the parse tree.
  //
  Vector<int> gather(1);
  Vector<int> rhs(1);
  rhs[0] = BUBBLE_TERMINAL;  // dummy terminal
  gather[0] = 1;
  insertProd(b.nonTerminal, rhs, 0, gather);
  //
  //	Add it to the bubbles for this nonterminal.
  //
  b.nextBubble = firstBubbles[flip(nonTerminal)];
  firstBubbles[flip(nonTerminal)] = nrBubbles;
  //
  //	Flag the need to take slow paths through code.
  //
  haveBubbles = true;
  if (lowerBound == 0)
    haveEmptyBubbles = true;
}

int
Parser::parseSentence(const Vector<int>& sentence, int root)
{
  if (expansions.length() == 0)
    {
      buildExpansionTables();
      firstRealCall = calls.size();
      ParserLog("expansion lists use " << firstRealCall << " calls");
      buildTerminalDecisionTrees();
      buildNonTerminalDecisionTrees();
    }
  rootNonTerminal = root;
  int nrTokens = sentence.length();
  continuations.contractTo(0);
  calls.contractTo(firstRealCall);
  returns.contractTo(0);
  memoItems.clear();
  firstCalls.resize(nrTokens + 1);
  firstReturns.resize(nrTokens + 1);
  firstMemoItems.resize(nrTokens + 1);
  for (int i = 0; i <= nrTokens; i++)
    {
      firstCalls[i] = NONE;
      firstReturns[i] = NONE;
      firstMemoItems[i] = NONE;
    }
  //
  //	Start by creating call to root with unbounded precedence.
  //
  calls.expandBy(1);
  Call& call = calls[firstRealCall];
  call.nonTerminal = root;
  call.maxPrec = UNBOUNDED;
  call.firstContinuation = NONE;
  call.nextCall = NONE;
  firstCalls[0] = firstRealCall;
  //
  //	Do passes.
  //
  badTokenIndex = 0;
  lastReturnProcessed = NONE;
  for (int i = 0; i < nrTokens;)
    {
      if (haveEmptyBubbles)
	checkForEmptyBubbles(i, sentence);
      else
	expandCalls(i);

      if (haveBubbles)
	doBubbles(i, sentence);

      scanCalls(i, sentence);
      ++i;
      lastReturnProcessed = NONE;
      doReturns(i, sentence);
    }
  //
  //	Deal with empty bubbles happening after the last token.
  //
  if (haveEmptyBubbles)
    checkForEmptyBubbles(nrTokens, sentence);
 
  ambiguous = false;
  parseTree.contractTo(0);
  int t = extractNextParse() ? (ambiguous ? 2 : 1) : 0;
  ParserLog("\n parseSentence() returns " << t << "\n\n");
  return t;
}

void
Parser::checkForEmptyBubbles(int tokenNr, const Vector<int>& sentence)
{
  for (;;)
    {
      expandCalls(tokenNr);
      Vector<Continuation>::size_type nrContinuations = continuations.size();
      doEmptyBubbles(tokenNr);
      doEmptyBubbleReturns(tokenNr, sentence);
      if (nrContinuations == continuations.size())
	break;  // no new continuations added by returns for empty bubbles
    }
}

int
Parser::getChild(int node, int sonNr)
{
  int son = parseTree[node].firstSon;
  for (int i = sonNr; i > 0; --i)
    {
      son = parseTree[son].nextSibling;
    }

  ParserLog("node " << node << " has " << sonNr << " son " << son);
  Assert(son != 0, "0 child seen for node=" << node << " sonNr=" << sonNr);
  return son;
}

void
Parser::dump()
{
  for (int i = 0; i < firstCalls.length(); i++)
    {
      cout << "Parse List #" << i << '\n';
      for (int j = firstCalls[i]; j != NONE; j = calls[j].nextCall)
	{
	  Call& call = calls[j];
	  cout << "Call " << call.nonTerminal << "\tmaxPrec = " << call.maxPrec << '\n';
	  for (int k = call.firstContinuation; k != NONE; k = continuations[k].nextContinuation)
	    {
	      Continuation& cont = continuations[k];
	      Rule* rule = rules[cont.ruleNr];
	      cout << "\tContinutation for rule #" << cont.ruleNr << " [" << rule->nonTerminal << " -> ";
	      for (int l = 0;; l++)
		{
		  if (l == cont.rhsPosition)
		    cout << " .";
		  if (l == rule->rhs.length())
		    break;
		  cout << ' ' << rule->rhs[l].symbol;
		}
	      cout << " , " << cont.startTokenNr << "]\n";
	    }
	}
      for (int j = firstReturns[i]; j != NONE; j = returns[j].nextReturn)
	{
	  Return& r = returns[j];
	  Rule* rule = rules[r.ruleNr];
	  cout << "Return #" << j << " for rule #" << r.ruleNr << " [" << rule->nonTerminal << " -> ";
	  for (int k = 0; k < rule->rhs.length(); k++)
	    cout << ' ' << rule->rhs[k].symbol;
	  cout << " , " << r.startTokenNr << "]\n";
	}
      cout << '\n';
      for (int j = firstMemoItems[i]; j != NONE; j = memoItems[j].nextMemoItem)
	{
	  MemoItem& m = memoItems[j];
	  Rule* rule = rules[m.ruleNr];
	  cout << "Memo  (" << m.nonTerminal << ", " << m.maxPrec << ") -> rule #" << m.ruleNr << " [" << rule->nonTerminal << " -> ";
	  for (int k = 0; k < rule->rhs.length(); k++)
	    cout << ' ' << rule->rhs[k].symbol;
	   cout << " , " << m.startTokenNr << "]\n";
	}
      cout << '\n';
    }
  cout << '\n';
}
