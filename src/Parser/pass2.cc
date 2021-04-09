/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017-2021 SRI International, Menlo Park, CA 94025, USA.

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
//	Second pass: extract parses from compact representation as needed.
//

bool
Parser::extractNextParse()
{
  ParserLog("extractNextParse()");
#ifdef PARSER_DEBUG
  dump();
#endif

  int lastIndex = firstReturns.length() - 1;
  if (parseTree.length() == 0)
    {
      ParserLog("  first attempt");
      //
      //	No previous parse. Find a return [S -> w ., 0] in final list.
      //
      int i = findRootReturn(firstReturns[lastIndex], rootNonTerminal);
      if (i == NONE)
	return false;
      //
      //	Check to see if there was an alternative and if so, flag as ambiguous.
      //
      if (findRootReturn(returns[i].nextReturn, rootNonTerminal) != NONE)
	ambiguous = true;
      //
      //	Make root node.
      //
      Return& r = returns[i];
      parseTree.resize(1);
      ParseNode& p = parseTree[0];
      p.ruleNr = r.ruleNr;
      p.startTokenNr = r.startTokenNr;  // must be 0
      p.nextReturnToCheck = r.nextReturn;
      p.endTokenNr = lastIndex;
      p.firstSon = NONE;
      p.nextSibling = NONE;
      extractFirstSubparse(0);
      return true;
    }
  ParserLog("  next attempt");
  //
  //	Previous parse so try to find a new subparse for existing root.
  //
  if (extractNextSubparse(0))
    return true;
  //
  //	Try to find a new root parse.
  //
  ParseNode& p = parseTree[0];
  int i = findRootReturn(p.nextReturnToCheck, rootNonTerminal);
  if (i == NONE)
    {
      parseTree.clear();
      return false;
    }
  //
  //	Remake root node.
  //
  Return& r = returns[i];
  p.ruleNr = r.ruleNr;
  //p.startTokenNr = 0;  // no change
  p.nextReturnToCheck = r.nextReturn;
  //p.endTokenNr = lastIndex;  // no change
  p.firstSon = NONE;
  p.nextSibling = NONE;
  extractFirstSubparse(0);
  return true;
}

void
Parser::extractFirstSubparseToTheLeft(int nodeNr, int rightPos, int rightSibling)
{
  //
  //	We go right to left through the nonterminals in parse node nodeNr
  //	finding a first parse for each.
  //
  ParseNode& p = parseTree[nodeNr];
  int startTokenNr = p.startTokenNr;
  int ruleNr = p.ruleNr;
  Rule* rule = rules[p.ruleNr];

  int lastSibling = rightSibling;
  int tokenNr = parseTree[lastSibling].startTokenNr;
  for (int pos = rightPos; pos >= 0; --pos)
    {
      if (rule->rhs[pos].symbol >= 0)
	{
	  //
	  //	Terminal symbol.
	  //
	  --tokenNr;
	  continue;
	}
      //
      //	Find a subparse for a nonterminal symbol. Note that this cannot
      //	be a rightmost symbol (since we are rebuilding to the left of
      //	an alternative parse) so we don't have to worry about DRPs.
      //
      int returnIndex = findReturn(firstReturns[tokenNr], ruleNr, pos, startTokenNr);
      Assert(returnIndex != NONE, "extractFirstToTheLeft() missing first return for rule " << ruleNr <<
	     "  pos = " << pos << "  startTokenNr = " << startTokenNr);
      if (!ambiguous &&
	  findReturn(returns[returnIndex].nextReturn, ruleNr, pos, startTokenNr) != NONE)
	ambiguous = true;
      //
      //	Make a parse tree node for this return.
      //
      Return& r = returns[returnIndex];
      int nodeNr = parseTree.length();
      parseTree.expandBy(1);
      ParseNode& p2 = parseTree[nodeNr];
      p2.ruleNr = r.ruleNr;
      p2.startTokenNr = r.startTokenNr;
      p2.nextReturnToCheck = r.nextReturn;
      p2.endTokenNr = tokenNr;
      p2.firstSon = NONE;
      ParserLog("set nextSibling of node " << nodeNr << " to " << lastSibling);
      p2.nextSibling = lastSibling;
      lastSibling = nodeNr;
      extractFirstSubparse(nodeNr);
      tokenNr = r.startTokenNr;
    }
  parseTree[nodeNr].firstSon = lastSibling;
}

void
Parser::extractFirstSubparse(int nodeNr)
{
  ParserLog("extractFirstSubparse() for nodeNr=" << nodeNr);
  //
  //	We are looking for a first way to parse the tokens below this node.
  //	We should always succeed.
  //
  ParseNode& p = parseTree[nodeNr];
  int ruleNr = p.ruleNr;
  int startTokenNr = p.startTokenNr;
  Rule* rule = rules[ruleNr];
  if (rule->rhs[0].symbol == BUBBLE_TERMINAL)
    return;  // we don't expand the single son of a bubble rule
  int nrSons = rule->nrNonTerminals;
  if (nrSons == 0)
    return;  // empty parse tree

  int tokenNr = p.endTokenNr;
  int endPos = rule->rhs.length() - 1;
  //
  //	Go right->left through the rhs of the rule.
  //
  int lastSibling = NONE;
  for (int pos = endPos; pos >= 0; --pos)
    {
      if (rule->rhs[pos].symbol >= 0)
	{
	  //
	  //	Terminal symbol.
	  //
	  --tokenNr;
	  continue;
	}
      //
      //	Find a subparse for a nonterminal symbol.
      //
      int returnIndex;
      if (pos == endPos)
	{
	  //
	  //	Final symbol of rule is a nonterminal - right-recursive case.
	  //	Need to consider the possibility of rule being at the top of a DRP.
	  //
	  IntBoolPair retPair = findReturnOrDeterministicReductionPath(firstReturns[tokenNr], ruleNr, startTokenNr, tokenNr);
	  returnIndex = retPair.first;
	  Assert(returnIndex != NONE, "missing first return for rule " << ruleNr <<
		 "  pos = " << pos << "  startTokenNr = " << startTokenNr);
	  //
	  //	Check for ambiguity if we don't know we're ambiguous
	  //
	  if (!ambiguous &&
	      findReturnOrDeterministicReductionPath(returns[returnIndex].nextReturn, ruleNr, startTokenNr, tokenNr).first != NONE)
	    ambiguous = true;
	  //
	  //	Check if we found the bottom of a DRP rather than a direct return.
	  //
	  if (retPair.second)
	    {
	      ParserLog("------ Saw DRP -------");
	      //
	      //	First we recover a sequence of (mostly) virtual returns for the DRP.
	      //
	      Return& r = returns[returnIndex];
	      Vector<DeferredReturn> drp;
	      extractDeterministicReductionPath(r.ruleNr, r.startTokenNr, drp);
	      //
	      //	The bottom of the DRP (i.e. the return that triggered the DRP) is r.
	      //	The next (necessarily right-recursive) return up from r is  drp[0].
	      //	The top of the DRP is drp[drp.size() - 1] and is the (necessarily
	      //	right-recursive) return for the parse node we are currently expanding.
	      //
#ifdef PARSER_DEBUG
	      for (auto& k : DeferredReturn)
		    cout << "DRP link (" << k.ruleNr << ", " << k.startTokenNr << ")" << endl;
#endif
	      int drpSize = drp.size();
	      int oneFromTop = drpSize - 2;  // -1 means we have a trivial DRP with just a top and a bottom
	      //
	      //	We want to process the returns in the order
	      //	  drp[oneFromTop], ... , drp[0], r
	      //
	      lastSibling = parseTree.length();  // where the first node of the DRP subparse will be built
	      buildDeterministicReductionPathParseTree(tokenNr, returnIndex, drp, oneFromTop);
	      //
	      //	We need to search for alternatives to DRP from the one-below-top node,
	      //	which could be the trigger return in the trivial case.
	      //
	      parseTree[lastSibling].nextReturnToCheck = r.nextReturn;
	      //
	      //	Need to handle tokens to the left of the DRP nonterminal in our current node.
	      //
	      tokenNr = (drpSize == 1) ? r.startTokenNr : drp[oneFromTop].startTokenNr;
	      continue;
	    }
	  //
	  //	Fall into the direct return code.
	  //
	}
      else
	{
	  //
	  //	Non-right-recursive case.
	  //
	  returnIndex = findReturn(firstReturns[tokenNr], ruleNr, pos, startTokenNr);
	  Assert(returnIndex != NONE, "missing first return for rule " << ruleNr <<
		 "  pos = " << pos << "  startTokenNr = " << startTokenNr);
	  //
	  //	Check for ambiguity if we don't know we're ambiguous
	  //
	  if (!ambiguous &&
	      findReturn(returns[returnIndex].nextReturn, ruleNr, pos, startTokenNr) != NONE)
	    ambiguous = true;
	}
      //
      //	Make a parse tree node for this return.
      //
      Return& r = returns[returnIndex];
      int nodeNr = parseTree.length();
      parseTree.expandBy(1);
      ParseNode& p2 = parseTree[nodeNr];
      p2.ruleNr = r.ruleNr;
      p2.startTokenNr = r.startTokenNr;
      p2.nextReturnToCheck = r.nextReturn;
      p2.endTokenNr = tokenNr;
      p2.firstSon = NONE;
      ParserLog("set nextSibling of node " << nodeNr << " to " << lastSibling);
      p2.nextSibling = lastSibling;
      lastSibling = nodeNr;
      extractFirstSubparse(nodeNr);
      tokenNr = r.startTokenNr;
    }
  Assert(lastSibling != NONE, "lastSibling == NONE");
  ParserLog("set firstSon of node " << nodeNr << " to " << lastSibling);
  parseTree[nodeNr].firstSon = lastSibling;
}

bool
Parser::extractNextSubparse(int nodeNr)
{
  //
  //	We have a parse tree for a first way to parse tokens below this node.
  //	We need to find the next subparse using current rule, if such a
  //	subparse exists.
  //
  ParseNode& p = parseTree[nodeNr];
  Rule* rule = rules[p.ruleNr];
  int nrSons = rule->nrNonTerminals;
  if (rule->rhs[0].symbol == BUBBLE_TERMINAL)
    return false;  // we don't expand the single son of a bubble rule
  if (nrSons == 0)
    return false;  // current rule only has terminals
  //
  //	We look at each son node, from left to right, to see if it has an alternative parse.
  //
  int pos = 0;
  int son = p.firstSon;
  int endPos = rule->rhs.length() - 1;
  for (; son != NONE; ++pos)
    {
      if (rule->rhs[pos].symbol >= 0)
	continue;  // terminal symbol
      //
      //	Symbol is a nonterminal so might have an alternative parse.
      //
      //	First we see if there is an alternative parse for the existing return.
      //
      if (extractNextSubparse(son))
	{
	  //
	  //	Alternative parse below parseTree[son] found; rebuild the parse tree
	  //	to the left.
	  //
	  extractFirstSubparseToTheLeft(nodeNr, pos - 1, son);
	  return true;
	}
      //
      //	Save 3 values that will be lost when we delete parse node.
      //
      int nextSibling = parseTree[son].nextSibling;
      int nextReturnToCheck = parseTree[son].nextReturnToCheck;
      int endTokenNr = parseTree[son].endTokenNr;
      //
      //	Failure to extract a new subparse for this son should have deleted
      //	the parse tree below it. We now delete the son itself and look
      //	for an alternative return for this nonterminal.
      //
      parseTree.contractTo(son);
      son = nextSibling;
      if (nextReturnToCheck == NONE)
	{
	  //	
	  //	No more returns to look at. So we move to the next nonterminal node.
	  //
	  continue; // FIXME: what happens if this was made by a DRP?
	}

      int returnIndex;
      if (pos == endPos)
	{
	  //
	  //	Son was an expansion of the rightmost symbol in the rule's rhs so
	  //	look for a return or DRP.
	  //
	  IntBoolPair retPair = findReturnOrDeterministicReductionPath(nextReturnToCheck, p.ruleNr, p.startTokenNr, endTokenNr);  // pos is implicit
	  returnIndex = retPair.first;
	  if (returnIndex == NONE)
	    {
	      //
	      //	Couldn't find an alternative return so move to the next nonterminal node.
	      //
	      continue;
	    }
	  if (retPair.second)
	    {
	      ParserLog("extractNextSubparse() : Saw DRP");
	      //
	      //	First we recover a sequence of (mostly) virtual returns for the DRP.
	      //
	      Return& r = returns[returnIndex];
	      Vector<DeferredReturn> drp;
	      extractDeterministicReductionPath(r.ruleNr, r.startTokenNr, drp);
	      //
	      //	The bottom of the DRP (i.e. the return that triggered the DRP) is r.
	      //	The next (necessarily right-recursive) return up from r is  drp[0].
	      //	The top of the DRP is drp[drp.size() - 1] and is the (necessarily
	      //	right-recursive) return for the parse node we are currently expanding.
	      //
#ifdef PARSER_DEBUG
	      for (auto& k : drp)
		cout << "DRP link (" << k.ruleNr << ", " << k.startTokenNr << ")" << endl;
#endif
	      int drpSize = drp.size();
	      int oneFromTop = drpSize - 2;  // -1 means we have a trivial DRP with just a top and a bottom
	      //
	      //	We want to process the returns in the order
	      //	  drp[oneFromTop], ... , drp[0], r
	      //
	      int topNode = parseTree.length();  // where the first node of the DRP subparse will be built
	      buildDeterministicReductionPathParseTree(endTokenNr, returnIndex, drp, oneFromTop);
	      parseTree[topNode].nextReturnToCheck = r.nextReturn;
	      //
	      //	Need to handle tokens to the left of the DRP nonterminal in our current node.
	      //
	      extractFirstSubparseToTheLeft(nodeNr, pos - 1, topNode);
	      return true;
	    } 
	  //
	  //	Fall into the direct return code.
	  //
	}
      else
	{
	  //
	  //	Son was an expansion of a symbol other than the rightmost so
	  //	look for a regular return.
	  //
	  returnIndex = findReturn(nextReturnToCheck, p.ruleNr, pos, p.startTokenNr);
	  if (returnIndex == NONE)
	    {
	      //
	      //	Couldn't find an alternative return so move to the next nonterminal node.
	      //
	      continue;
	    }
	}
      //
      //	Make a parse tree node for this return.
      //
      Return& r = returns[returnIndex];
      int newNodeNr = parseTree.length();
      parseTree.expandBy(1);
      ParseNode& p2 = parseTree[newNodeNr];
      p2.ruleNr = r.ruleNr;
      p2.startTokenNr = r.startTokenNr;
      p2.nextReturnToCheck = r.nextReturn;
      p2.endTokenNr = endTokenNr;
      p2.firstSon = NONE;
      ParserLog("set nextSibling of node " << newNodeNr << " to " << son);
      p2.nextSibling = son;
      extractFirstSubparse(newNodeNr);
      extractFirstSubparseToTheLeft(nodeNr, pos - 1, newNodeNr);
      return true;
    }
  //
  //	None of the nonterminals of our rule had an alternative subparse.
  //
  return false;
}

int
Parser::buildDeterministicReductionPathParseTree(int endTokenNr,
						 int triggerReturnIndex,
						 const Vector<DeferredReturn>& drp,
						 int drpIndex)
{
  ParserLog("buildDeterministicReductionPathParseTree() for endTokenNr =" << endTokenNr <<
	    " triggerReturnIndex=" << triggerReturnIndex <<
	    " drpIndex=" << drpIndex);
  //
  //	We want to build the parse tree for the DRP drp[drpIndex], ..., drp[0] including
  //	parses for any nonterminals other than the right-recursive nonterminals in the DRP.
  //
  if (drpIndex == -1)
    {
      //
      //	End of DRP reached - build parse tree node for trigger return.
      //
      Return& r = returns[triggerReturnIndex];
      int nodeNr = parseTree.length();
      parseTree.expandBy(1);
      ParseNode& p = parseTree[nodeNr];
      p.ruleNr = r.ruleNr;
      p.startTokenNr = r.startTokenNr;
      p.nextReturnToCheck = NONE;
      p.endTokenNr = endTokenNr;
      p.firstSon = NONE;
      p.nextSibling = NONE;  // nothing to the right of us in parent
      extractFirstSubparse(nodeNr);
      return nodeNr;
    }
  //
  //	Deal with right-recursive rule somewhere above trigger return but below top return.
  //
  const DeferredReturn& d = drp[drpIndex];
  const Rule* rule = rules[d.ruleNr];
  int nrSons = rule->nrNonTerminals;
  //
  //	We assume we are not dealing with the trigger return, so there must at least be a rightmost nonterminal
  //	in the rule.
  //
  Assert(nrSons > 0, "didn't see a son in a mid-DRP rule");
  //
  //	Make new node.
  //
  int nodeNr = parseTree.length();
  parseTree.expandBy(1);
  ParseNode& p = parseTree[nodeNr];
  p.ruleNr = d.ruleNr;
  p.startTokenNr = d.startTokenNr;
  p.nextReturnToCheck = NONE;
  p.endTokenNr = endTokenNr;
  //p.firstSon = NONE;
  p.nextSibling = NONE;  // nothing to the right of use in parent
  //
  //	Deal with right recursion.
  //
  int lastSibling = buildDeterministicReductionPathParseTree(endTokenNr, triggerReturnIndex, drp, drpIndex - 1);
  //
  //	Deal with any nonterminals to the left of right recursion.
  //
  if (nrSons > 1)
    extractFirstSubparseToTheLeft(nodeNr, rule->rhs.size() - 2, lastSibling);
  else
    parseTree[nodeNr].firstSon = lastSibling;
  return nodeNr;
}

int
Parser::findRootReturn(int i, int nonTerminal)
{
  //
  //	Find return for root nonterminal.
  //
  while (i != NONE)
    {
      Return& ret = returns[i];
      if (ret.startTokenNr == 0 && rules[ret.ruleNr]->nonTerminal == nonTerminal)
	break;
      i = ret.nextReturn;
    }
  return i;
}

inline bool
Parser::existsCall(int parseListNr, int ruleNr, int rhsPosition, int startTokenNr)
{
  //
  //	Check that a call (ruleNr, rhsPosition, startTokenNr) actually
  //	exists in call list "parseListNr".
  //
  if (rhsPosition == 0)
    {
      //
      //	Call will be implied rather than real.
      //
      if (startTokenNr != parseListNr)
	return false;
      int lhs = rules[ruleNr]->nonTerminal;
      for (int i = firstCalls[parseListNr]; i != NONE; i = calls[i].nextCall)
	{
	  if (calls[i].nonTerminal == lhs)
	    return rules[ruleNr]->prec <= calls[i].maxPrec;
	}
      return false;
    }
  int nonTerminal = rules[ruleNr]->rhs[rhsPosition].symbol;
  for (int i = firstCalls[parseListNr]; i != NONE; i = calls[i].nextCall)
    {
      Call& call = calls[i];
      if (call.nonTerminal == nonTerminal)
	{
	  for (int j = call.firstContinuation; j != NONE;)
	    {
	      Continuation& cont = continuations[j];
	      if (cont.ruleNr == ruleNr && cont.rhsPosition == rhsPosition &&
		  cont.startTokenNr == startTokenNr)
		return true;
	      j = cont.nextContinuation;
	    }
	  break;
	}
    }
  return false;
}

int
Parser::findReturn(int i, int ruleNr, int rhsPosition, int startTokenNr)
{
  //
  //	Starting with returns[i], find a return that deals with the
  //	(maybe implied) call (ruleNr, rhsPosition, startTokenNr).
  //
  int nonTerminal = rules[ruleNr]->rhs[rhsPosition].symbol;
  int prec = rules[ruleNr]->rhs[rhsPosition].prec;
  while (i != NONE)
    {
      Return& ret = returns[i];
      if (rules[ret.ruleNr]->nonTerminal == nonTerminal &&
	  rules[ret.ruleNr]->prec <= prec &&
	  existsCall(ret.startTokenNr, ruleNr, rhsPosition, startTokenNr))
	break;
      i = ret.nextReturn;
    }
  return i;
}
