//
//	Code for handling deterministic reduction paths during parse tree construction.
//

bool
Parser::existsMemo(int startTokenNr, int nonTerminal, int prec, int ruleNrToFind, int startTokenNrToFind)
{
  //
  //	Look at the memo items in list startTokenNr and see if there is one which maps a return
  //    for (nonTerminal, prec) to a top-of-DRP return, (ruleNrToFind, startTokenNrToFind).
  //
  for (int i = firstMemoItems[startTokenNr]; i != NONE;)
    {
      MemoItem& m = memoItems[i];
      if (m.nonTerminal == nonTerminal && m.maxPrec == prec)
	{
	  ParserLog("in list " << startTokenNr << " a return for nonterminal " << m.nonTerminal <<
		    " with prec " << m.maxPrec << " starts a DRP ending in a return (ruleNr = " << m.ruleNr <<
		    ", " << m.startTokenNr << ")");
	  if (m.ruleNr == ruleNrToFind && m.startTokenNr == startTokenNrToFind)
	    {
	      ParserLog("*** This was the memo we were looking for ***");
	      return true;
	    }
	  else
	    {
	      ParserLog("but we are looking for (ruleNr = " << ruleNrToFind <<
			", " << startTokenNrToFind << ")");
	    }
	}
      i = m.nextMemoItem;
    }
  return false;
}

Parser::IntBoolPair
Parser::findReturnOrDeterministicReductionPath(int i, int ruleNr, int startTokenNr, int endTokenNr)
{
  ParserLog("findReturnOrDeterministicReductionPath(" << i << ", " << ruleNr << ", " << startTokenNr << ")");
  //
  //	rhsPosition is the last position in the rule rhs and must hold a nonterminal.
  //	Starting with return[i], find a return that either:
  //	(1) deals with the (maybe implied) call (ruleNr, rhsPosition, startTokenNr); OR
  //	(2) initiates a deterministic reduction path that ends in a return (ruleNr, startTokenNr).
  //
  const Rule* rule = rules[ruleNr];  // the rule
  int rhsPosition = rule->rhs.size() - 1;  // last position in the rule's rhs
  int nonTerminal = rule->rhs[rhsPosition].symbol;  // the nonterminal in that position
  int prec = rule->rhs[rhsPosition].prec;  // and its precedence

  ParserLog("  looking for a return for nonTerminal=" << nonTerminal << "  prec=" << prec);
  while (i != NONE)
    {
      Return& ret = returns[i];
      int returnStartTokenNr = ret.startTokenNr;
      int returnRuleNr = ret.ruleNr;
      Rule* returnRule = rules[returnRuleNr];
      int returnNonTerminal = returnRule->nonTerminal;
      int returnPrec = returnRule->prec;
      //
      //	Does the return deal with a call (ruleNr, rhsPosition, startTokenNr) ?
      //
      if (returnNonTerminal == nonTerminal &&  // the return is for the nonterminal we are trying to parse
	  returnPrec <= prec &&  // and the rule has low enough precedence
	  existsCall(returnStartTokenNr, ruleNr, rhsPosition, startTokenNr))  // and there was a call for the last symbol in the rules rhs
	{
	  ParserLog("  saw return of rule " << ret.ruleNr << " that is a direct return");
	  return IntBoolPair(i, false);  // direct return
	}
      //
      //	Does the return initiate a deterministic reduction path that ends in a return (ruleNr, startTokenNr) ?
      //
      //	We look for a memo item created when the DRP is recognized.
      //
      if (existsMemo(returnStartTokenNr, returnNonTerminal, returnPrec, ruleNr, startTokenNr))
	{
	  ParserLog("  saw return of rule " << ret.ruleNr << " that is the bottom of a DRP for rule " << ruleNr);
	  //
	  //	Need to check that that the return R directly above it in the DRP is not concrete.
	  //	If this is the case, the return will be seen as a direct return, when R is expanded
	  //	and returning a DRP here will generate the same parse twice.
	  //
	  //	The situation in which R is concrete is highly contrived:
	  //	We essentially need to make R the trigger return of a sub-DRP, by having a return
	  //	P below it that can't be a trigger return because it has a second user U. But then to
	  //	have DRP we just found be deterministic we need to make sure U cannot use R
	  //	by using the prec/gather mechanism.
	  //
	  ParserLog("---> trigger of DRP is (" << returnRuleNr << ", " << returnStartTokenNr << ") <---");

	  Parser::IntPair p = extractOneStepOfDeterministicReductionPath(returnRuleNr, returnStartTokenNr);
	  ParserLog("---> next step of DRP is (" << p.first << ", " << p.second << ") <---");

	  Assert(p.first != NONE && p.second != NONE, "couldn't find DRP abover trigger");
	  //
	  //	Does parse list startTokerNr contain the return for rule number p.first starting at
	  //	token number p.second?
	  //
	  if (!returnExists(firstReturns[endTokenNr], p.first, p.second))
	    return IntBoolPair(i, true);  // initiating return for DRP ending with our return (ruleNr, startTokenNr).
	  ParserLog("---> Abandoning DRP because next step of DRP, ruleNr=" << p.first <<
		    " startTokenNr=" << startTokenNr << " is CONCRETE <---");
	}
      //
      //	Look at next return in list.
      //
      i = ret.nextReturn;
    }
  ParserLog("  no return found for rule " << ruleNr);
  return IntBoolPair(NONE, false);  // didn't find a return
}

bool
Parser::returnExists(int i, int ruleNr, int startTokenNr)
{
  while (i != NONE)
    {
      Return& ret = returns[i];
      if (ret.ruleNr == ruleNr && ret.startTokenNr == startTokenNr)
	return true;
      i = ret.nextReturn;
    }
  return false;
}

Parser::IntPair
Parser::extractOneStepOfDeterministicReductionPath(int ruleNr, int startTokenNr)
{
  //
  //	Returns (nextRuleNr, nextStartTokenNr) or (NONE, NONE) if we
  //	are already at the top of the DRP.
  //
  Rule* rule = rules[ruleNr];
  int nonTerminal = rule->nonTerminal;
  int prec = rule->prec;
  //
  //	We go through the parse list startTokenNr looking for callers of the
  //	nonterminal recognized by rule ruleNr.
  //
  //	If we find exactly one and it is for a right recursive rule that
  //	generates a return based on its final nonterminal being recognized
  //	then we have the next step of a DRP.
  //
  int nextRuleNr = NONE;
  int nextStartTokenNr = NONE;

  for (int j = firstCalls[startTokenNr]; j != NONE;)
    {
      Call& call = calls[j];
      int nonTerminal2 = call.nonTerminal;
      int maxPrec = call.maxPrec;
      j = call.nextCall;
      if (nonTerminal2 == nonTerminal)
	{
	  if (maxPrec == UNBOUNDED)
	    {
	      ParserLog("*****> end of DRP seen because of root object is a caller");
	      return IntPair(NONE, NONE);
	    }
	  //
	  //	Handle explicit continuations.
	  //
	  for (int k = call.firstContinuation; k != NONE;)
	    {
	      Continuation& cont = continuations[k];
	      int ruleNr2 = cont.ruleNr;
	      int pos = cont.rhsPosition;
	      k = cont.nextContinuation;
	      Rule* rule2 = rules[ruleNr2];
	      if (rule2->rhs[pos].prec >= prec)
		{
		  //
		  //	Found a continutation.
		  //
		  if (nextRuleNr != NONE)
		    {
		      ParserLog("*****> end of DRP seen because of multiple continuations");
		      return IntPair(NONE, NONE);
		    }
		  int rhsRemaining = rule2->rhs.length() - (pos + 1);
		  if (rhsRemaining != 0)
		    {
		      ParserLog("******> end of DRP seen because of non return event");
		      return IntPair(NONE, NONE);
		    }
		  //
		  //	It generates a return that potentially continues the DRP.
		  //
		  nextRuleNr = ruleNr2;
		  nextStartTokenNr = cont.startTokenNr;
		}
	    }
	}

      //
      //	Check for implied start ups.
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
	    break;
	  if (rule2->rhs[0].prec >= prec)
	    {
	      //
	      //	Found a continutation.
	      //
	      if (nextRuleNr != NONE)
		{
		  ParserLog("*******> end of DRP seen because of multiple continuations - implied start up");
		  return IntPair(NONE, NONE);
		}
	      int rhsRemaining = rule2->rhs.length() - 1;
	      if (rhsRemaining != 0)
		{
		  ParserLog("********> end of DRP seen because of non return event - implied start up");
		  return IntPair(NONE, NONE);
		}
	      //
	      //	It generates a return that potentially continues the DRP.
	      //	Can only happen for a <nonterminal> ::= <nonterminal> production.
	      //
	      nextRuleNr = r;
	      nextStartTokenNr = startTokenNr;
	    }
	  r = rule2->equal;
	}
    }
  Assert(nextRuleNr != NONE, "didn't find caller for rule " << ruleNr << " at position " << startTokenNr);
  ParserLog("***********2 continuing DRP chase with rule " << nextRuleNr << " at position " << nextStartTokenNr);
  return IntPair(nextRuleNr, nextStartTokenNr);
}

void
Parser::extractDeterministicReductionPath(int ruleNr, int startTokenNr, Vector<DeferredReturn>& drp)
{
  for(;;)
    {
      IntPair p = extractOneStepOfDeterministicReductionPath(ruleNr, startTokenNr);
      ruleNr = p.first;
      if (ruleNr == NONE)
	break;
      startTokenNr = p.second;

      int nrReturns = drp.size();
      drp.expandBy(1);
      drp[nrReturns].ruleNr = ruleNr;
      drp[nrReturns].startTokenNr = startTokenNr;
    }
}
