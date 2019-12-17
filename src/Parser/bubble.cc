//
//	Code for handling bubble during pass 1.
//

void
Parser::processBubble(int tokenNr, int bubbleNr, const Vector<int>& sentence)
{
  Bubble b = bubbles[bubbleNr];
  int ruleNr = b.ruleNr;
  ParserLog("processBubble tokenNr=" << tokenNr << " ruleNr=" << ruleNr << " bubbleNr=" << bubbleNr);

  int openParens = 0;
  int last = sentence.size();

  for (int i = tokenNr; i < last;)
    {
      int token = sentence[i];
      if (b.upperBound != NONE)
	{
	  //
	  //	Check for syntax errors caused by hitting upper bound.
	  //
	  int tokensLeft = b.upperBound - (i - tokenNr);
	  if (tokensLeft == 0)
	    return;
	  if (token != b.rightParen)
	    {
	      //
	      //	Need to check for syntax errors arising from insufficient upperBounds to
	      //	close parentheses.
	      //
	      int tokensLeft = b.upperBound - (i - tokenNr);
	      if (tokensLeft == openParens)
		return;  // not enough tokens left to close open parens
	      if (tokensLeft == openParens + 1 && token == b.leftParen)
		return;  // no enough tokens left to close parens if we open one more
	    }
	}
      //
      //	Match tokens, keeping in mind that left and right paren could be the same.
      //
      if (token == b.rightParen)
	{
	  if (openParens > 0)
	    --openParens;
	  else if (token == b.leftParen)
	    openParens = 1;
	  else
	    return;  // unmatched closing paren
	}
      else if (token == b.leftParen)
	++openParens;
      else if (openParens == 0)
	{
	  //
	  //	Regular token not inside parens, check if it is allowed.
	  //
	  const Vector<int>& excluded = b.excludedTerminals;
	  FOR_EACH_CONST(i, Vector<int>, excluded)
	    {
	      if (*i == token)
		return;  // exluded token
	    }
	}
      //
      //	Token was part of a bubble with a valid continuation.
      //
      ++i;
      if (i > badTokenIndex)
	badTokenIndex = i;
      if (openParens == 0 &&
	  (b.lowerBound == NONE || b.lowerBound <= i - tokenNr))
	{
	  //
	  //	Bubble is big enough and has no unclosed parens, so it's
	  //	a valid place to end the bubble.
	  //
	  ParserLog("returning bubble at token " << i);
	  makeReturn(i, ruleNr, tokenNr);
	}
    }
}

void
Parser::doBubbles(int tokenNr, const Vector<int>& sentence)
{
  ParserLog(Tty(Tty::RED) << "doBubble() at token " << tokenNr << Tty(Tty::RESET));
  //
  //	Look at each call for a nonterminal.
  //
  for (int i = firstCalls[tokenNr]; i != NONE;)
    {
      Call& call = calls[i];
      i = call.nextCall;
      //
      //	Look at each bubble that can parse to that nonterminal.
      //
      for (int j = firstBubbles[flip(call.nonTerminal)]; j != NONE; j = bubbles[j].nextBubble)
	processBubble(tokenNr, j, sentence);
    }
}

void
Parser::doEmptyBubbles(int tokenNr)
{
  ParserLog(Tty(Tty::RED) << "doEmptyBubble() at token " << tokenNr << Tty(Tty::RESET));
  //
  //	Look at each call for a nonterminal.
  //
  for (int i = firstCalls[tokenNr]; i != NONE;)
    {
      Call& call = calls[i];
      i = call.nextCall;
      //
      //	Look at each bubble that can parse to that nonterminal.
      //
      for (int j = firstBubbles[flip(call.nonTerminal)]; j != NONE;)
	{
	  Bubble& b = bubbles[j];
	  j = b.nextBubble;
	  if (b.lowerBound == 0)
	    {
	      //
	      //	We have the possibility of this nonterminal going to an empty bubble.
	      //	This generates an immediate return in the same parse list.
	      //
	      ParserLog("did empty bubble return for rule " << b.ruleNr);
	      makeReturn(tokenNr, b.ruleNr, tokenNr);
	    }
	}
    }
}

void
Parser::doEmptyBubbleReturns(int tokenNr, const Vector<int>& sentence)
{
  //
  //	We redo old empty bubble returns in case they have more continuations
  //
  int i = firstReturns[tokenNr];
  if (lastReturnProcessed != NONE)
    {
      //
      //	We have already processed some returns. We need to redo any
      //	old empty bubble returns in case they have more continuations.
      //
      for (;; i = returns[i].nextReturn)
	{
	  Return& ret = returns[i];
	  if (tokenNr == ret.startTokenNr)
	    processReturn(tokenNr, ret.startTokenNr, ret.ruleNr, sentence);
	  if (i == lastReturnProcessed)
	    break;  // this was the last of the already processed returns
	}
      i = returns[i].nextReturn;  // first new return
    }
  //
  //	We then do new returns which maybe empty bubble returns or
  //	have arose from processing empty bubble returns.
  //
  for (; i != NONE; i = returns[i].nextReturn)
    {
      lastReturnProcessed = i;
      Return& ret = returns[i];
      processReturn(tokenNr, ret.startTokenNr, ret.ruleNr, sentence);
    }
}
