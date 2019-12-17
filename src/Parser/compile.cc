//
//	Compile productions rules into ternary trees.
//

void
Parser::buildTerminalDecisionTrees()
{
  Vector<Rule*> ruleTable;
  Vector<int> starts;
  int nrNonTerminals = firstTerminalRules.length();
  terminalDecisionTrees.expandTo(nrNonTerminals);
  for (int i = 0; i < nrNonTerminals; i++)
    {
      //
      //	Collect all the terminal rules that have i as the nonterminal.
      //
      ruleTable.contractTo(0);
      for (int r = firstTerminalRules[i]; r != NONE; r = rules[r]->nextRule)
	ruleTable.append(rules[r]);
      //
      //	Sort them by rhs start symbol, then prec, smallest first.
      //
      sort(ruleTable.begin(), ruleTable.end(), ruleLt);
      //
      //	Make a list of the index in ruleTable of the first production
      //	for each rhs start symbol.
      //
      int nrRules = ruleTable.length();
      int lastSymbol = UNBOUNDED;  // bigger than any terminal symbol
      starts.contractTo(0);
      for (int j = 0; j < nrRules; j++)
	{
	  int startSymbol = ruleTable[j]->rhs[0].symbol;
	  if (startSymbol == BUBBLE_TERMINAL)
	    {
	      //
	      //	<nonTerminal> ::= BUBBLE_TERMINAL is a special rule
	      //	that will be the last one, since BUBBLE_TERMINAL is bigger
	      //	than any real terminal, and which shouldn't go in the tree.
	      //
	      ruleTable.contractTo(j);
	      break;
	    }
	  if (startSymbol != lastSymbol)
	    {
	      lastSymbol = ruleTable[j]->rhs[0].symbol;
	      starts.append(j);
	    }
	}
      //
      //	Make a decision tree by divid-and-conqueor.
      //
      terminalDecisionTrees[i] = buildDecisionTree(ruleTable, starts, 0, starts.length() - 1);
    }
}

void
Parser::buildNonTerminalDecisionTrees()
{
  Vector<Rule*> ruleTable;
  Vector<int> starts;
  int nrNonTerminals = firstTerminalRules.length();
  nonTerminalDecisionTrees.expandTo(nrNonTerminals);
  for (int i = 0; i < nrNonTerminals; i++)
    {
      //
      //	Collect all the terminal rules that have i as the nonterminal.
      //
      ruleTable.contractTo(0);
      for (int r = firstNonTerminalRules[i]; r != NONE; r = rules[r]->nextRule)
	ruleTable.append(rules[r]);
      //
      //	Sort them by rhs start symbol, then prec, smallest first.
      //
      sort(ruleTable.begin(), ruleTable.end(), ruleLt);
      //
      //	Make a list of the index in ruleTable of the first production
      //	for each rhs start symbol.
      //
      int nrRules = ruleTable.length();
      int lastSymbol = UNBOUNDED;  // bigger than any terminal symbol
      starts.contractTo(0);
      for (int j = 0; j < nrRules; j++)
	{
	  if (ruleTable[j]->rhs[0].symbol != lastSymbol)
	    {
	      lastSymbol = ruleTable[j]->rhs[0].symbol;
	      starts.append(j);
	    }
	}
      nonTerminalDecisionTrees[i] = buildDecisionTree(ruleTable, starts, 0, starts.length() - 1);
    }
}

int
Parser::buildDecisionTree(Vector<Rule*>& ruleTable, Vector<int>& starts, int first, int last)
{
  if (first > last)
    return NONE;
  int i = (first + last) / 2;
  int j = starts[i]; 
  ruleTable[j]->smaller = buildDecisionTree(ruleTable, starts, first, i - 1);
  int k = j;
  int end = ruleTable.length() - 1;
  for (; k < end; k++)
    {
      if (ruleTable[k]->rhs[0].symbol != ruleTable[k + 1]->rhs[0].symbol)
	break;
      ruleTable[k]->equal = ruleTable[k + 1]->index;
    }
  ruleTable[k]->equal = NONE;
  ruleTable[j]->bigger = buildDecisionTree(ruleTable, starts, i + 1, last);
  return ruleTable[j]->index;
}
