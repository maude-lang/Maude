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
//	Code for LTL satisfiability solving.
//
#include <queue>

struct GenBuchiAutomaton::Step
{
  Step();

  int parent;
  Bdd formula;
};

local_inline
GenBuchiAutomaton::Step::Step()
{
  parent = NONE;
}

bool
GenBuchiAutomaton::satSolve(list<Bdd>& leadIn, list<Bdd>& cycle)
{
#ifdef TDEBUG
  dump(cout);
#endif
  maximallyCollapseStates();
#ifdef TDEBUG
  dump(cout);
#endif

  sccAnalysis();
  int nrComponents = componentInfo.length();
  for (int i = 0; i < nrComponents; i++)
    {
      if (componentInfo[i].status == FAIR)
	{
	  //
	  //	First we find a shortest path to a state in a fair component.
	  //
	  leadIn.clear();
	  cycle.clear();
	  int fairState;
	  bfsToFairComponent(fairState, leadIn);
	  //
	  //	Now we need to find a path within this component with
	  //	transitions satisfying each of the fairness conditions.
	  //	We try to minimize the length of this path by only looking
	  //	for subpaths that satisfy at least one additional non-redundant
	  //	fairness condition.
	  //
	  int currentState = fairState;
	  NatSet fairness(componentInfo[stateInfo[fairState].component].redundant);
	  while (fairness != allFair)
	    {
	      list<Bdd> path;
	      bfsToMoreFairness(fairness, currentState, path);
	      cycle.splice(cycle.end(), path);
	    }
	  //
	  //	Finally we need to complete the cycle.
	  //
	  if (currentState != fairState || cycle.empty())
	    {
	      list<Bdd> path;
	      bfsToTarget(currentState, fairState, path);
	      cycle.splice(cycle.end(), path);
	    }
	  //
	  //	We now see if the lead-in can be "rolled" into the cycle.
	  //
	  while (!(leadIn.empty()) && cycle.back().implies(leadIn.back()))
	    {
	      //
	      //	Last formula of cycle implies last formula of leadIn
	      //	so right-rotate cycle and delete last formula of leadIn.
	      //
	      cycle.splice(cycle.begin(), cycle, --(cycle.end()));
	      leadIn.pop_back();
	    }
	  return true;
	}
    }
  return false;
}

void
GenBuchiAutomaton::bfsToFairComponent(int& fairState, list<Bdd>& path) const
{
  //
  //	Find a (possibly empty) shortest path to a state in a fair component.
  //
  Vector<Step> steps(states.length());
  queue<int> toVisit;
  const NatSet::const_iterator e = initialStates.end();
  for (NatSet::const_iterator i = initialStates.begin(); i != e; ++i)
    {
      int s = *i;
      if (componentInfo[stateInfo[s].component].status == FAIR)
	{
	  fairState = s;
	  return;
	}
      toVisit.push(s);
      steps[s].parent = UNBOUNDED;
    }

  for (;;)
    {
      Assert(!(toVisit.empty()), "toVisit empty");
      int stateNr = toVisit.front();
      toVisit.pop();
      
      const FairTransitionSet& fts = fairTransitionSets.ithElement(states[stateNr]);
      FairTransitionSet::const_iterator e = fts.end();
      for(FairTransitionSet::const_iterator i = fts.begin(); i != e; ++i)
	{
	  int s = i->first.first;
	  if (componentInfo[stateInfo[s].component].status == FAIR)
	    {
	      fairState = s;
	      path.push_front(i->second);
	      for(;;)
		{
		  const Step& step = steps[stateNr];
		  stateNr = step.parent;
		  if (stateNr == UNBOUNDED)
		    break;
		  path.push_front(step.formula);
		}
	      return;
	    }

	  Step& step = steps[s];
	  if (step.parent == NONE)
	    {
	      step.parent = stateNr;
	      step.formula = i->second;
	      toVisit.push(s);
	    }
	}
    }
}

void
GenBuchiAutomaton::bfsToMoreFairness(NatSet& fairness, int& start, list<Bdd>& path) const
{
  //
  //	Staying within the connected component containing start, find a
  //	shortest sequence of transitions that give us additional fairness.
  //
  int ourComponent = stateInfo[start].component;
  Vector<Step> steps(states.length());
  queue<int> toVisit;
  toVisit.push(start);
  steps[start].parent = UNBOUNDED;

  for (;;)
    {
      Assert(!(toVisit.empty()), "toVisit empty");
      int stateNr = toVisit.front();
      toVisit.pop();
     
      const FairTransitionSet& fts = fairTransitionSets.ithElement(states[stateNr]);
      FairTransitionSet::const_iterator e = fts.end();
      for(FairTransitionSet::const_iterator i = fts.begin(); i != e; ++i)
	{
	  int s = i->first.first;
	  if (stateInfo[s].component == ourComponent)
	    {
	      const NatSet& arcFairness = fairnessConditions.ithElement(i->first.second);
	      if (!(fairness.contains(arcFairness)))
		{
		  fairness.insert(arcFairness);
		  start = s;
		  path.push_front(i->second);
		  for(;;)
		    {
		      const Step& step = steps[stateNr];
		      stateNr = step.parent;
		      if (stateNr == UNBOUNDED)
			break;
		      path.push_front(step.formula);
		    }
		  return;
		}

	      Step& step = steps[s];
	      if (step.parent == NONE)
		{
		  step.parent = stateNr;
		  step.formula = i->second;
		  toVisit.push(s);
		}
	    }
	}
    }
}

void
GenBuchiAutomaton::bfsToTarget(int start, int target, list<Bdd>& path) const
{
  //
  //	Staying within the connected component containing start, find a
  //	shortest non-empty sequence of transitions that arrives at target.
  //
  int ourComponent = stateInfo[start].component;
  Vector<Step> steps(states.length());
  queue<int> toVisit;
  toVisit.push(start);
  steps[start].parent = UNBOUNDED;

  for (;;)
    {
      Assert(!(toVisit.empty()), "toVisit empty");
      int stateNr = toVisit.front();
      toVisit.pop();

      const FairTransitionSet& fts = fairTransitionSets.ithElement(states[stateNr]);
      FairTransitionSet::const_iterator e = fts.end();
      for(FairTransitionSet::const_iterator i = fts.begin(); i != e; ++i)
	{
	  int s = i->first.first;
	  if (stateInfo[s].component == ourComponent)
	    {
	      if (s == target)
		{
		  path.push_front(i->second);
		  for(;;)
		    {
		      const Step& step = steps[stateNr];
		      stateNr = step.parent;
		      if (stateNr == UNBOUNDED)
			break;
		      path.push_front(step.formula);
		    }
		  return;
		}

	      Step& step = steps[s];
	      if (step.parent == NONE)
		{
		  step.parent = stateNr;
		  step.formula = i->second;
		  toVisit.push(s);
		}
	    }
	}
    }
}
