//
//	First pass: construct a compact representation of all parses.
//

Parser::IntPair
Parser::chaseDeterministicReductionPath(int ruleNr, int startTokenNr)
{
  //
  //	Find the DRP starting at (ruleNr, startTokenNr) and return the top return
  //	Use existing memo items to short-circuit this search where possible.
  //	Made a place holder memo item for the lhs (nonterminal, prec) of the rule
  //	for any pair encounters that isn't the top return.
  //
  ParserLog("chaseDeterministicReductionPath() called for ruleNr " << ruleNr <<
	    "  startTokenNr = " << startTokenNr);

  for (;;)
    {
      Rule* rule = rules[ruleNr];
      int nonTerminal = rule->nonTerminal;
      int prec = rule->prec;
      //
      //	If we have a memo item that takes us to the top of the DRP,
      //	return the ruleNr and startTokenNr for the top return.
      //
      for (int i = firstMemoItems[startTokenNr]; i != NONE;)
	{
	  MemoItem& m = memoItems[i];
	  i = m.nextMemoItem;
	  //
	  //	We require an exact match for precs. Clearly we couldn't use a
	  //	memo item with a lower prec since our rule simply doesn't satisfy it.
	  //	Less obviously, using a memo item with a higher prec means we
	  //	could lose behavior available with the lower prec that isn't
	  //	captured by the memo item.
	  //
	  if (m.nonTerminal == nonTerminal && m.maxPrec == prec)
	    {
	      ParserLog("vvvvvvvvvvvvvvvvvvvvvvvvvv" <<
			"\n   Using memo item in set " << startTokenNr <<
			" when looking for nonterminal " << nonTerminal << " at prec " << prec <<
			" we saw a DRP that ends at rule " << m.ruleNr << " to set " << m.startTokenNr <<
			"\n^^^^^^^^^^^^^^^^^^^^^");
	      return IntPair(m.ruleNr, m.startTokenNr);
	    }
	  //
	  //	It's possible (with colliding DRPs) that nonterminals are equal
	  //	while precs differ, with both m.maxPrec < prec and m.maxPrec > prec
	  //	possible since we don't know which order returns will be encountered in.
	  //
	}
      //
      //	See if we can extend DRP upwards by one step.
      //
      IntPair p = extractOneStepOfDeterministicReductionPath(ruleNr, startTokenNr);
      if (p.first == NONE)
	{
	  //
	  //	No, so what we have corresponds to the top return.
	  //
	  break;
	}
      //
      //	Yes, so what we have is not the top return. We will require a
      //	memo item that maps (nonTerminal, prec) to the top return. We create
      //	a place holder now in the parse list for startTokenNr and leave
      //	it to the caller to fill in the top return.
      //
      firstMemoItems[startTokenNr] = makeMemoItem(nonTerminal,
						  prec,  // must be the same as we're looking for to be effective
						  NONE,  // dummy
						  NONE,  // dummy
						  firstMemoItems[startTokenNr]);
      ParserLog("################" <<
		"\n   made memo item place holder in set " << startTokenNr <<
		" when looking for nonterminal " << nonTerminal << " at prec " << prec <<
		"\n###################");
      ruleNr = p.first;
      startTokenNr = p.second;
    }
  return IntPair(ruleNr, startTokenNr);
}

void
Parser::processReturn(int tokenNr, int startTokenNr, int ruleNr, const Vector<int>& sentence)
{
  //
  //	In list tokenNr we have a return for ruleNr starting at startTokenNr.
  //
  ParserLog("processReturn() tokenNr=" << tokenNr << " startTokenNr=" << startTokenNr <<
	    " ruleNr=" << ruleNr);
  Rule* rule = rules[ruleNr];
  //
  //	Get the (nonterminal, prec) we recognized.
  //
  int nonTerminal = rule->nonTerminal;
  int prec = rule->prec;
  ParserLog("nonterminal=" << nonTerminal << " prec=" << prec);
  //
  //	See if we handled this (nonterminal, prec) at startToken before and
  //	memoized a DRP.
  //
  for (int j = firstMemoItems[startTokenNr]; j != NONE;)
    {
      MemoItem& m = memoItems[j];
      j = m.nextMemoItem;
      if (m.nonTerminal == nonTerminal && m.maxPrec == prec)
	{
	  ParserLog("Found memoized DRP that ends at rule="<< m.ruleNr << " startTokenNr=" << m.startTokenNr);
	  makeReturn(tokenNr, m.ruleNr, m.startTokenNr);
	  return;
	}
    }
  //
  //	No memo applied so we now look for calls satisfied by (nonterminal, prec).
  //
  int drpPossible = true;
  int deferredReturnRuleNr = NONE;
  int deferredReturnStartTokenNr = NONE;
  //
  //	We look at all the calls at startTokenNr.
  //
  for (int j = firstCalls[startTokenNr]; j != NONE;)  // look for what called our nonterminal
    {
      Call& call = calls[j];
      int nonTerminal2 = call.nonTerminal;
      int maxPrec = call.maxPrec;
      j = call.nextCall;
      if (nonTerminal2 == nonTerminal)
	{
	  //
	  //	This is the call for our nonterminal - handle explicit continuations.
	  //
	  for (int k = call.firstContinuation; k != NONE;)
	    {
	      Continuation& cont = continuations[k];
	      int ruleNr2 = cont.ruleNr;
	      int startTokenNr2 = cont.startTokenNr;
	      int pos = cont.rhsPosition;
	      k = cont.nextContinuation;
	      Rule* rule2 = rules[ruleNr2];
	      if (rule2->rhs[pos].prec >= prec)
		{
		  int rhsRemaining = rule2->rhs.length() - (pos + 1);
		  if (rhsRemaining == 0)
		    {
		      ParserLog("right recursive return for rule=" << ruleNr2);
		      //
		      //	This continuation immediately generates a return.
		      //
		      if (drpPossible && deferredReturnRuleNr == NONE)
			{
			  //
			  //	We haven't seem a mid-production user nor have we
			  //	put another right recursive user aside. Thus we may
			  //	have the start of a DRP and we put this right recursive
			  //	user aside.
			  //
			  deferredReturnRuleNr = ruleNr2;
			  deferredReturnStartTokenNr = startTokenNr2;
			}
		      else
			{
			  //
			  //	DRP not possible - do standard processing.
			  //
			  drpPossible = false;
			  makeReturn(tokenNr, ruleNr2, startTokenNr2);
			}
		    }
		  else
		    {
		      ParserLog("continuation for rule=" << ruleNr2);
		      drpPossible = false;
		      //
		      //	This continuation has more to match.
		      //
		      advanceRule(ruleNr2, pos + 1, startTokenNr2, tokenNr, sentence);
		    }
		}
	    }
	}
      //
      //	We see a call for nonTerminal2. Get the tree of implied start-ups for nonTerminal2,
      //	and see if we have nonTerminal2 ::= nonTerminal ...
      //	where the prec for nonTerminal is >= the prec of the rule we just recognized.
      //
      int r = nonTerminalDecisionTrees[flip(nonTerminal2)];
      while (r != NONE)
	{
	  Rule* rule2 = rules[r];
	  int t = nonTerminal - rule2->rhs[0].symbol;
	  if (t == 0)
	    break;
	  r = (t > 0) ? rule2->bigger : rule2->smaller;
	}
      while (r != NONE)
	{
	  Rule* rule2 = rules[r];
	  if (rule2->prec > maxPrec)
	    {
	      //
	      //	rule2 didn't have small enough prec to satisfy call. Rules in the equals path
	      //	are ordered by increasing prec so we are done.
	      //
	      break;
	    }
	  if (rule2->rhs[0].prec < prec)
	    {
	      //
	      //	rule2 required a smaller prec than our return provided.
	      //
	      r = rule2->equal;
	      continue;
	    }
	  ParserLog("saw implied call by rule=" << r);
	  //
	  //	Normal processesing.
	  //
	  int rhsRemaining = rule2->rhs.length() - 1;
	  if (rhsRemaining == 0)
	    {
	      ParserLog("implied right recursive return for rule=" << r);
	      if (drpPossible && deferredReturnRuleNr == NONE)
		{
		  //
		  //	We haven't seem a mid-production user nor have we
		  //	put another right recursive user aside. Thus we may
		  //	have the start of a DRP and we put this right recursive
		  //	user aside.
		  //
		  deferredReturnRuleNr = r;
		  deferredReturnStartTokenNr = startTokenNr;
		}
	      else
		{
		  //
		  //	DRP not possible - do standard processing.
		  //
		  drpPossible = false;
		  makeReturn(tokenNr, r, startTokenNr);
		}
	    }
	  else
	    {
	      ParserLog("implied continuation for rule=" << r);
	      drpPossible = false;
	      advanceRule(r, 1, startTokenNr, tokenNr, sentence);
	    }
	  r = rule2->equal;
	}
    }
  //
  //	We've now processed all the explicit and implicit callers for (nonterminal, prec)
  //	pair we recognized. Now see if we have a DRP.
  //
  ParserLog("After processing callers seenContinuationBranch=" << seenContinuationBranch <<
	    " deferredReturns.size()=" << deferredReturns.size());
  if (drpPossible && deferredReturnRuleNr != NONE)
    {
      //
      //	When looking for continuations of (nonTerminal, prec) in calls from
      //	token startTokenNr we found only a single continuation that immediately
      //	generates a return (that we placed in deferredReturns).
      //
      //	We treat this as the bottom of a deterministic reduction path (DRP) and
      //	follow it to the top of the path. Rather than generate a linear number
      //	of returns, we only generate the final return, and memoize this
      //	return at startTokenNr for continuations of (nonTerminal, prec)
      //	so we don't chase this DRP a second time.
      //
      ParserLog("deterministic reduction seen in set " << tokenNr <<
		" from rule " << ruleNr << " starting  at " << startTokenNr <<
		" to rule " << deferredReturnRuleNr <<
		" starting at " << deferredReturnStartTokenNr);

      int firstNewMemoNr = memoItems.size();
      firstMemoItems[startTokenNr] = makeMemoItem(nonTerminal,
						  prec,
						  NONE,
						  NONE,
						  firstMemoItems[startTokenNr]);
      IntPair topMost = chaseDeterministicReductionPath(deferredReturnRuleNr, deferredReturnStartTokenNr);
      //
      //	Fix up all the dummy memo items we created.
      //
      int nrMemoItems = memoItems.size();
      for (int i = firstNewMemoNr; i < nrMemoItems; ++i)
	{
	  MemoItem& m = memoItems[i];
	  m.ruleNr = topMost.first;
	  m.startTokenNr = topMost.second;
	}
      ParserLog("made memo item in set " << startTokenNr <<
		" when looking for nonterminal " << nonTerminal << " at prec " << prec <<
		" we saw a DRP that ends at rule " << topMost.first << " to set " << topMost.second);
      makeReturn(tokenNr, topMost.first, topMost.second);
    }
  else
    {
      ParserLog("No DRP seen tokeNr = " << tokenNr << "  ruleNr = " << ruleNr <<
		"  startTokenNr = " << startTokenNr <<
		"  seenContinuationBranch = " << seenContinuationBranch <<
		"  deferredReturns.size() = " << deferredReturns.size());
      //
      //	If we deferred a return, process it normally.
      //
      if (deferredReturnRuleNr != NONE)
	makeReturn(tokenNr, deferredReturnRuleNr, deferredReturnStartTokenNr);
    }
}

void
Parser::doReturns(int tokenNr, const Vector<int>& sentence)
{
  lastReturnProcessed = NONE;  // in case we don't process any returns for this list
  //
  //	Do all returns.
  //
  for (int i = firstReturns[tokenNr]; i != NONE; i = returns[i].nextReturn)
    {
      lastReturnProcessed = i;
      Return& ret = returns[i];
      processReturn(tokenNr, ret.startTokenNr, ret.ruleNr, sentence);
    }
}

void
Parser::expandCalls(int tokenNr)
{
  int firstCall = firstCalls[tokenNr];
  if (firstCall == NONE)
    return;
  if (calls[firstCall].nextCall == NONE && !haveEmptyBubbles)
    {
      //
      //	Single call case, with no empty bubbles so we can
      //	safely use the expansion list itself.
      //
      Call& call = calls[firstCall];
      int nonTerminal = call.nonTerminal;
      int maxPrec = call.maxPrec;

      Vector<Expansion>& exs = expansions[flip(nonTerminal)];
      for (int j = exs.length() - 1; j >= 0; j--)
	{
	  Expansion& ex = exs[j];
	  if (ex.prec <= maxPrec)
	    {
	      //
	      //	We've found the largest prec expansion that we can use.
	      //
	      int firstExpansionCall = ex.firstExpansionCall;
	      Call& call2 = calls[firstExpansionCall];
	      if (call2.nonTerminal == nonTerminal)
		{
		  if (call2.maxPrec > call.maxPrec)
		    call.maxPrec = call2.maxPrec;  //  increase prec of single call
		  call.nextCall = call2.nextCall;  // pick up any calls to other nonterminals
		}
	      else
		call.nextCall = firstExpansionCall;  // pick up all the expansion calls
	      return;
	    }
	}
      return;  // no expansion for this call
    }
  //
  //	General case with multiple calls in list.
  //
  for (int i = firstCall; i != NONE;)
    {
      Call& call = calls[i];
      i = call.nextCall;
      int nonTerminal = call.nonTerminal;
      int maxPrec = call.maxPrec;
      Vector<Expansion>& exs = expansions[flip(nonTerminal)];
      for (int j = exs.length() - 1; j >= 0; j--)
	{
	  Expansion& ex = exs[j];
	  if (ex.prec <= maxPrec)
	    {
	      //
	      //	We've found the largest prec expansion that we can use.
	      //
	      for (int k = ex.firstExpansionCall; k != NONE;)
		{
		  Call& call2 = calls[k];
		  k = call2.nextCall;
		  int nt2 = call2.nonTerminal;
		  int prec2 = call2.maxPrec;
		  //
		  //	See if we should increase the prec on an existing call.
		  //
		  for (int l = firstCall; l != NONE;)
		    {
		      Call& call3 = calls[l];
		      l = call3.nextCall;
		      if (call3.nonTerminal == nt2)
			{
			  if (call3.maxPrec < prec2)
			    call3.maxPrec = prec2;
			  goto nextPair;
			}
		    }
		  {
		    //
		    //	Add a new call, and push it on the front so we don't visit it
		    //	in the outer loop (since it's fully expanded).
		    //
		    int nrCalls = calls.length();
		    calls.expandBy(1);
		    Call& call4 = calls[nrCalls];
		    call4.nonTerminal = nt2;
		    call4.maxPrec = prec2;
		    call4.firstContinuation = NONE;
		    call4.nextCall = firstCall;
		    firstCall = nrCalls;
		    ParserLog("created call for " << nt2 << " from " << nonTerminal);
		  }
		nextPair:
		  ;
		}
	      break;
	    }
	}
    }
  firstCalls[tokenNr] = firstCall;
}

void
Parser::scanCalls(int tokenNr, const Vector<int>& sentence)
{
  int currentToken = sentence[tokenNr];
  int nextTokenNr = tokenNr + 1;
  for (int i = firstCalls[tokenNr]; i != NONE;)
    {
      Call& call = calls[i];
      int maxPrec = call.maxPrec;
      i = call.nextCall;
      int r = terminalDecisionTrees[flip(call.nonTerminal)];
      while (r != NONE)
	{
	  Rule* rule = rules[r];
	  int t = currentToken - rule->rhs[0].symbol;
	  if (t == 0)
	    {
	      if (rule->prec <= maxPrec)
		{
		  if (nextTokenNr > badTokenIndex)
		    badTokenIndex = nextTokenNr;
		  do
		    {
		      advanceRule(r, 1, tokenNr, nextTokenNr, sentence);
		      r = rule->equal;
		      if (r == NONE)
			break;
		      rule = rules[r];
		    }
		  while (rule->prec <= maxPrec);
		}
	      break;
	    }
	  r = (t > 0) ? rule->bigger : rule->smaller;
	}
    }
}

void
Parser::advanceRule(int ruleNr,
		    int pos,
		    int startTokenNr,
		    int tokenNr,
		    const Vector<int>& sentence)
{
  Rule* rule = rules[ruleNr];
  int end = rule->rhs.size();
  int nrTokens = sentence.size();
  Assert(tokenNr <= nrTokens, "tokenNr past last parse list");
  for (; pos < end; pos++)
    {
      int symbol = rule->rhs[pos].symbol;
      if (symbol < 0)
	{
	  makeCall(tokenNr, ruleNr, pos, startTokenNr);
	  return;
	}
      if (tokenNr == nrTokens)
	{
	  //
	  //	Ran out of tokens.
	  //
	  return;
	}
      int token = sentence[tokenNr];
      if (symbol != token)
	return; // die
      ++tokenNr;
      //
      //	Successful match of token. All previous tokens must be part of at least
      //	one legal partial parse so check if we need to update badTokenIndex.
      //
      if (tokenNr > badTokenIndex)
	badTokenIndex = tokenNr;
    }
  makeReturn(tokenNr, ruleNr, startTokenNr);
}

void
Parser::makeCall(int tokenNr, int ruleNr, int rhsPosition, int startTokenNr)
{
  int nonTerminal = rules[ruleNr]->rhs[rhsPosition].symbol;
  int prec = rules[ruleNr]->rhs[rhsPosition].prec;
  for (int i = firstCalls[tokenNr]; i != NONE;)
    {
      Call& call = calls[i];
      i = call.nextCall;
      if (call.nonTerminal == nonTerminal)
	{
	  for (int j = call.firstContinuation; j != NONE;)
	    {
	      Continuation& cont = continuations[j];
	      if (cont.ruleNr == ruleNr && cont.rhsPosition == rhsPosition &&
		  cont.startTokenNr == startTokenNr)
		return;
	      j = cont.nextContinuation;
	    }
	  if (prec > call.maxPrec)
	    call.maxPrec = prec;
	  int nrContinuations = continuations.length();
	  continuations.expandBy(1);
	  Continuation& cont = continuations[nrContinuations];
	  cont.ruleNr = ruleNr;
	  cont.rhsPosition = rhsPosition;
	  cont.startTokenNr = startTokenNr;
	  cont.nextContinuation = call.firstContinuation;
	  call.firstContinuation = nrContinuations;
	  return;
	}
    }
  int nrContinuations = continuations.length();
  continuations.expandBy(1);
  Continuation& cont = continuations[nrContinuations];
  cont.ruleNr = ruleNr;
  cont.rhsPosition = rhsPosition;
  cont.startTokenNr = startTokenNr;
  cont.nextContinuation = NONE;

  int nrCalls = calls.length();
  calls.expandBy(1);
  Call& call = calls[nrCalls];
  call.nonTerminal = nonTerminal;
  call.maxPrec = prec;
  call.firstContinuation = nrContinuations;
  call.nextCall = firstCalls[tokenNr];
  firstCalls[tokenNr] = nrCalls;
}

bool
Parser::makeReturn(int tokenNr, int ruleNr, int startTokenNr)
{
  int prev = NONE;
  for (int i = firstReturns[tokenNr]; i != NONE;)
    {
      Return& ret = returns[i];
      if (ret.ruleNr == ruleNr && ret.startTokenNr == startTokenNr)
	return false;
      prev = i;
      i = ret.nextReturn;
    }
  ParserLog("makeReturn() << " << tokenNr << "  " << ruleNr << "  " << startTokenNr);
  int nrReturns = returns.length();
  returns.expandBy(1);
  Return& ret = returns[nrReturns];
  ret.ruleNr = ruleNr;
  ret.startTokenNr = startTokenNr;
  ret.nextReturn = NONE;
  if (prev == NONE)
    firstReturns[tokenNr] = nrReturns;
  else
    returns[prev].nextReturn = nrReturns;
  return true;
}

int
Parser::makeMemoItem(int nonTerminal, int maxPrec, int ruleNr, int startTokenNr, int nextMemoItem)
{
  int nrMemoItems = memoItems.size();
  memoItems.expandBy(1);
  MemoItem& m = memoItems[nrMemoItems];
  m.nonTerminal = nonTerminal;
  m.maxPrec = maxPrec;
  m.ruleNr = ruleNr;
  m.startTokenNr = startTokenNr;
  m.nextMemoItem = nextMemoItem;
  return nrMemoItems;
}
