/*

    This file is part of the Maude 3 interpreter.

    Copyright 2018-2021 SRI International, Menlo Park, CA 94025, USA.

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
//	Compile productions rules into expansion lists for left recursion.
//

bool
Parser::mergeInMap(SymbolToPrecMap& target,
		   const SymbolToPrecMap& donor,
		   int targetNonterminal,
		   int targetPrec)
{
  //
  //	We merge the expansions from donor into target, skipped any
  //	expansions that are worse equal to (targetNonterminal, targetPrec).
  //
  bool changed = false;
  for (auto& i : donor)
    {
      int nonterminal = i.first;
      int prec = i.second;
      if (nonterminal == targetNonterminal && prec <= targetPrec)
	continue;  // donor mapping provides no broadening
      //
      //	See if we already map the nonterminal.
      //
      SymbolToPrecMap::iterator j = target.find(nonterminal);
      if (j == target.end())
	{
	  target.insert(i);
	  changed = true;
	}
      else
	{
	  if (j->second < prec)
	    {
	      j->second = prec;
	      changed = true;
	    }
	}
    }
  return changed;
}

int
Parser::makeExpansionList(const SymbolToPrecMap mapForm, int nonterminal)
{
  //
  //	Convert the expansions in mapForm to a list of calls.
  //
  int nrExpansions = mapForm.size();
  int startCall = calls.size();
  calls.expandBy(nrExpansions);
  int nextCall = startCall;
  //
  //	Want to put nonterminal expansion first, if it exists.
  //
  const SymbolToPrecMap::const_iterator nt = mapForm.find(nonterminal);
  if (nt != mapForm.end())
    {
      Call& call = calls[nextCall];
      ++nextCall;
      call.nonTerminal = nonterminal;
      call.maxPrec = nt->second;
      call.firstContinuation = NONE;
      call.nextCall = nextCall;
    }

  for (auto& i : mapForm)
    {
      if (i.first != nonterminal)
	{
	  Call& call = calls[nextCall];
	  ++nextCall;
	  call.nonTerminal = i.first;
	  call.maxPrec = i.second;
	  call.firstContinuation = NONE;
	  call.nextCall = nextCall;
	}
    }
  calls[nextCall - 1].nextCall = NONE;  // patch tail of list
  return startCall;
}

void
Parser::buildExpansionTables()
{
  int nrNonTerminals = firstTerminalRules.length();
  //
  //	First we make an initial collection of expansion tables for each
  //	nonterminal by considering each rule having its rhs headed by a nonterminal.
  //
  Expansions expansionMaps(nrNonTerminals);
  //
  //	For each nonterminal.
  //
  for (int i = 0; i < nrNonTerminals; ++i)
    {
      //
      //	Get the empty map from precs to expansions maps for this nonterminal.
      //
      PrecToExpansionMap& m = expansionMaps[i];
      int nonterminal = flip(i);
      //
      //	Look for each nonterminal rule returning the nonterminal
      //
      for (int r = firstNonTerminalRules[i]; r != NONE; r = rules[r].nextRule)
	{
	  Rule& rule = rules[r];
	  int prec = rule.prec;
	  int nonterminal2 = rule.rhs[0].symbol;
	  int prec2 = rule.rhs[0].prec;
	  if (nonterminal2 == nonterminal && prec2 <= prec)
	    continue;  // rule provides no broadening of nonterminal
	  //
	  //	See if the expansion map for (nonterminal, prec)
	  //	needs updating.
	  //
	  SymbolToPrecMap& m2 = m[prec];
	  SymbolToPrecMap::iterator j = m2.find(nonterminal2);
	  if (j == m2.end())
	    m2.insert(SymbolToPrecMap::value_type(nonterminal2, prec2));  // new entry
	  else
	    {
	      //
	      //	Maybe update existing entry.
	      //
	      if (j->second < prec2)
		j->second = prec2;
	    }
	}
      //
      //	Now we close upwards by merging the map for
      //	(N, p) into all maps for (N, p') with p' > p.
      //
      if (!(m.empty()))
	{
	  const PrecToExpansionMap::iterator e = m.end();
	  PrecToExpansionMap::iterator prev = m.begin();
	  PrecToExpansionMap::iterator next = prev;
	  for (++next; next != e;)
	    {

	      PrecToExpansionMap::iterator current = next;
	      ++next;
	      mergeInMap(current->second, prev->second, nonterminal, current->first);

		prev = current;
	    }
	}
    }
  //
  //	Next we propagate one step expansions up to fixed point.
  //
  bool changed;
  do
    {
      changed = false;
      for (int i = 0; i < nrNonTerminals; ++i)
	{
	  //
	  //	Get the empty map from precs to expansions maps for this nonterminal.
	  //
	  PrecToExpansionMap& m = expansionMaps[i];
	  int nonterminal = flip(i);
	  //
	  //	Go through each of the maps in ascending prec order.
	  //
	  const PrecToExpansionMap::iterator endj = m.end();
	  for (PrecToExpansionMap::iterator j = m.begin(); j != endj; ++j)
	    {
	      int prec = j->first;
	      SymbolToPrecMap& m2 = j->second;
	      //
	      //	We now have m2 which is the current expansion for 
	      //	(nonterminal, prec) pair (i, prec). We want to update
	      //	it by merging in expansions for (nonterminal, prec)
	      //	pairs reachable from it.
	      //
	      //	This merging will not delete entries so we don't
	      //	worry that we are merging entries into the map we
	      //	are iterating over.
	      //
	      for (auto& k : m2)
		{
		  int nonterminal2 = k.first;
		  int prec2 = k.second;
		  PrecToExpansionMap& m3 = expansionMaps[flip(nonterminal2)];
		  if (!(m3.empty()))
		    {
		      //
		      //	We want the largest prec that is <= prec2 but there
		      //	is no function for this.
		      //
		      //	We find the smallest prec that > prec2
		      //
		      PrecToExpansionMap::const_iterator l = m3.upper_bound(prec2);
		      if (l == m3.begin())
			{
			  //
			  //	All precs in m3 are > prec2; there are none <= prec2
			  //
			}
		      else
			{
			  --l; // must point to a prec that is <= prec2; need to check this for m3.end()
			  changed |= mergeInMap(m2, l->second, nonterminal, prec);
			}
		    }
		}
	    }
	}
    }
  while (changed);
  //
  //	Finally we convert the expansionMaps into lists of calls.
  //
  expansions.expandTo(nrNonTerminals);
  for (int i = 0; i < nrNonTerminals; ++i)
    {
      Vector<Expansion>& expansionVector = expansions[i];
      //
      //	Get the map from precs to expansions maps for this nonterminal.
      //
      PrecToExpansionMap& m = expansionMaps[i];
      int nonterminal = flip(i);

      const SymbolToPrecMap* last = 0;
      for (auto& j : m)
	{
	  if (last != 0 && j.second == *last)
	    continue;
	  last = &(j.second);
	  //
	  //	Add expansion list.
	  //
	  int nrExpansions = expansionVector.size();
	  expansionVector.resize(nrExpansions + 1);
	  Expansion& expansion = expansionVector[nrExpansions];
	  expansion.prec = j.first;
	  expansion.firstExpansionCall = makeExpansionList(j.second, nonterminal);
	}
    }
}

void
Parser::dumpMap(PrecToExpansionMap& m, int nonterminal)
{
  for (auto& i : m)
    {
      cout << nonterminal << "/" << i.first << " -> ";
      for (auto& j : i.second)
	{
	  cout << "  " << j.first << "/" << j.second;
	}
      cout << endl;
    }
}
