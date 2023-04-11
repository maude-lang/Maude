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
//	Compile productions rules into ternary trees.
//

void
Parser::buildTerminalDecisionTrees()
{
  Vector<Rule*> ruleTable;
  Vector<int> starts;
  int nrNonTerminals = firstTerminalRules.length();
  terminalDecisionTrees.resize(nrNonTerminals);
  for (int i = 0; i < nrNonTerminals; i++)
    {
      //
      //	Collect all the terminal rules that have i as the nonterminal.
      //	We use pointers rather than indices for ruleLt.
      //
      ruleTable.clear();
      for (int r = firstTerminalRules[i]; r != NONE; r = rules[r].nextRule)
	ruleTable.append(&(rules[r]));
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
      starts.clear();
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
  nonTerminalDecisionTrees.resize(nrNonTerminals);
  for (int i = 0; i < nrNonTerminals; i++)
    {
      //
      //	Collect all the terminal rules that have i as the nonterminal.
      //	We use pointers rather than indices for ruleLt.
      //
      ruleTable.clear();
      for (int r = firstNonTerminalRules[i]; r != NONE; r = rules[r].nextRule)
	ruleTable.append(&(rules[r]));
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
      starts.clear();
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
      ruleTable[k]->equal = ruleTable[k + 1] - rules.data();  // compute index in rules[] by pointer subtraction
    }
  ruleTable[k]->equal = NONE;
  ruleTable[j]->bigger = buildDecisionTree(ruleTable, starts, i + 1, last);
  return ruleTable[j] - rules.data();  // compute index in rules[] by pointer subtraction
}
