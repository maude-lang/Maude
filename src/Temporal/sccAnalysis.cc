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
//	Code for strongly connected component analysis.
//

void
GenBuchiAutomaton::sccAnalysis()
{
  //
  //	First we partition the graph of states into strongly connected
  //	components, assigning a component number to each state in such
  //	a way that components can only reach lower numbered components.
  //
  int nrStates = states.length();
  stateInfo.expandTo(nrStates);
  for (int i = 0; i < nrStates; i++)
    stateInfo[i].traversalNumber = 0;
  traversalCount = 0;
  componentCount = 0;
  const NatSet::const_iterator e = initialStates.end();
  for (NatSet::const_iterator i = initialStates.begin(); i != e; ++i)
    strongConnected(*i);
  //
  //	Then we analyse each component to see if it is fair, unfair or dead.
  //	For fair components, we also compute a set of redundant fairness sets;
  //	non-redundant fairness sets in fair components have their indices
  //	inserted into essential.
  //
  componentInfo.expandTo(componentCount);
  for (int i = 0; i < componentCount; i++)
    {
      handleComponent(i);
#ifdef TDEBUG
      static const char* const text[] = {"dead", "unfair", "fair"};
      cout << "Component " << i << "\tstatus: " <<
	text[componentInfo[i].status] <<
	"\tredundant fair sets" << componentInfo[i].redundant << endl;
#endif
    }
#ifdef TDEBUG
  cout << "essential fairness " << essential << endl;
#endif
}

int
GenBuchiAutomaton::strongConnected(int v)
{
  //
  //	Push state v on the stack and give it a traversal order number.
  //
  stateStack.push(v);
  int vLowLink = ++traversalCount;
  stateInfo[v].traversalNumber = vLowLink;
  //
  //	Examine each outgoing arc from state v.
  //
  const FairTransitionSet& fts = fairTransitionSets.ithElement(states[v]);
  FairTransitionSet::const_iterator e = fts.end();
  for(FairTransitionSet::const_iterator i = fts.begin(); i != e; ++i)
    {
      int w = i->first.first;
      int wNumber = stateInfo[w].traversalNumber;
      if (wNumber == 0)
	{
	  //
	  //	Tree arc.
	  //
	  int wLowLink = strongConnected(w);
	  if (wLowLink < vLowLink)
	    vLowLink = wLowLink;
	}
      else
	{
	  if (wNumber < vLowLink)  // implies wNumber < stateInfo[v].traversalNumber;
	    vLowLink = wNumber;    // i.e. cross-link or frond
	}
    }

  if (stateInfo[v].traversalNumber == vLowLink)
    {
      //
      //	Root of a new SCC; remove SCC from stack.
      //
#ifdef TDEBUG
      cout << "Component " << componentCount << endl;
#endif
      int i;
      do
        {
          i = stateStack.top();
	  stateStack.pop();
#ifdef TDEBUG
          cout << i << ' ';
#endif
	  stateInfo[i].traversalNumber = INT_MAX;  // mark as no longer in stack
	  stateInfo[i].component = componentCount;
        }
      while (i != v);
#ifdef TDEBUG
      cout << endl;
#endif
      ++componentCount;
    }
  return vLowLink;
}

void
GenBuchiAutomaton::handleComponent(int number)
{
  //
  //	implies[i] is the set of indices of fairness sets implied by
  //	the ith fairness set. We start with everything and remove
  //	contradictions for each internal transition.
  //
  Vector<NatSet> implies(nrFairnessSets);
  for (int i = 0; i < nrFairnessSets; i++)
    implies[i] = allFair;
  NatSet sum;
  bool reachLiveScc = false;
  bool hasInternalTransitions = false;  // to detect degenerate SCCs
  int nrStates = states.length();
  for (int i = 0; i < nrStates; i++)
    {
      if (stateInfo[i].component == number)
	{
	  const FairTransitionSet& fts = fairTransitionSets.ithElement(states[i]);
	  FairTransitionSet::const_iterator e = fts.end();
	  for(FairTransitionSet::const_iterator j = fts.begin(); j != e; ++j)
	    {
	      int target = j->first.first;
	      int targetComponentNr = stateInfo[target].component;
	      if (targetComponentNr == number)
		{
		  //
		  //	Internal transition.
		  //
		  hasInternalTransitions = true;
		  const NatSet& fairness = fairnessConditions.ithElement(j->first.second);
		  sum.insert(fairness);
		  for (int k = 0; k < nrFairnessSets; k++)
		    {
		      if (fairness.contains(k))
			implies[k].intersect(fairness);
		    }
		}
	      else
		{
		  //
		  //	External transition.
		  //
		  Assert(targetComponentNr < number, "SCC partial order problem");
		  if (componentInfo[targetComponentNr].status != DEAD)
		    reachLiveScc = true;
		}
	    }
	}
    }
  if (!hasInternalTransitions || sum != allFair)
    {
      componentInfo[number].status = reachLiveScc ? UNFAIR : DEAD;
      return;
    }
  componentInfo[number].status = FAIR;
  NatSet& redundant = componentInfo[number].redundant;
  for (int i = 0; i < nrFairnessSets; i++)
    {
      if (!(redundant.contains(i)))
	{
	  implies[i].subtract(i);
	  redundant.insert(implies[i]);
	};
    }
  NatSet used(allFair);
  used.subtract(redundant);
  essential.insert(used);
}
