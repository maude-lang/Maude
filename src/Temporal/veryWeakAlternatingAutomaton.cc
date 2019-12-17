/*

    This file is part of the Maude 3 interpreter.

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
//	Implementation for class VeryWeakAlternatingAutomaton.
//
#include <set>

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"

//	temporal class definitions
#include "logicFormula.hh"
#include "veryWeakAlternatingAutomaton.hh"

VeryWeakAlternatingAutomaton::VeryWeakAlternatingAutomaton(LogicFormula* formula, int top)
  : states(top + 1),
    formula(formula),
    renaming(top + 1)
{
  dnf(top, initialStates);
#ifdef TDEBUG
  dump(cout);
#endif
  reachabilityOpt();
#ifdef TDEBUG
  dump(cout);
#endif
}

void
VeryWeakAlternatingAutomaton::dnf(int subformulaIndex, TransitionSet& result)
{
  //
  //	Compute a disjunction of conjunctions of states. For convenience
  //	we store this set of conjunctions of states as a set of transitions
  //	where each conjunction is a transition (hyperarc) with the label true.
  //	We also call computeTransitionSet() on all reachable states.
  //
   switch (formula->getOp(subformulaIndex))
    {
    case LogicFormula::AND:
      {
	TransitionSet left;
	dnf(formula->getArg(subformulaIndex, 0), left);
	TransitionSet right;
	dnf(formula->getArg(subformulaIndex, 1), right);
	result.product(left, right);
	break;
      }
    case LogicFormula::OR:
      {
	dnf(formula->getArg(subformulaIndex, 0), result);
	TransitionSet right;
	dnf(formula->getArg(subformulaIndex, 1), right);
	result.insert(right);
	break;
      }
    default:
      {
	Transition t;
	t.first.insert(subformulaIndex);
	t.second = bdd_true();
	result.insert(t);
	computeTransitionSet(subformulaIndex);  // reachable
      }
    }
}

void
VeryWeakAlternatingAutomaton::computeTransitionSet(int subformulaIndex)
{
  TransitionSet& result = states[subformulaIndex];
  if (!(result.getMap().empty()))
    return;  // already computed (actually we miss false case - but false case does nothing)
  LogicFormula::Op op = formula->getOp(subformulaIndex);
  switch (op)
    {
    case LogicFormula::PROPOSITION:
      {
	Transition t;
	t.second = ithvar(formula->getProp(subformulaIndex));
	result.insert(t);
	break;
      }
    case LogicFormula::LTL_TRUE:
      {
	Transition t;
	t.second = bdd_true();
	result.insert(t);
	break;
      }
    case LogicFormula::LTL_FALSE:
      break;  // empty transition set
    case LogicFormula::NOT:
      {
	Transition t;
	t.second = nithvar(formula->getProp(formula->getArg(subformulaIndex, 0)));
	result.insert(t);
	break;
      }
    case LogicFormula::NEXT:
      {
	dnf(formula->getArg(subformulaIndex, 0), result);
	break;
      }
    default:
      {
	int leftIndex = formula->getArg(subformulaIndex, 0);
	int rightIndex = formula->getArg(subformulaIndex, 1);
	computeTransitionSet(leftIndex);
	computeTransitionSet(rightIndex);
	const TransitionSet& left = states[leftIndex];
	const TransitionSet& right = states[rightIndex];
	switch (op)
	  {
	  case LogicFormula::AND:
	    {
	      result.product(left, right);
	      break;
	    }
	  case LogicFormula::OR:
	    {
	      result = left;  // deep copy
	      result.insert(right);
	      break;
	    }
	  default:
	    {
	      Transition t;
	      t.first.insert(subformulaIndex);
	      t.second = bdd_true();
	      if (op == LogicFormula::UNTIL)
		{
		  TransitionSet self;
		  self.insert(t);
		  result.product(left, self);
		  result.insert(right);
		  finalStates.append(subformulaIndex);
		}
	      else
		{
		  Assert(op == LogicFormula::RELEASE, "bad op");
		  TransitionSet sum(left);
		  sum.insert(t);
		  result.product(sum, right);
		}
	    }
	  }
      }
    }
}

void
VeryWeakAlternatingAutomaton::reachabilityOpt()
{
  int nrStates = states.length();
  nrNewStates = 0;
  for (int i = 0; i < nrStates; i++)
    renaming[i] = NONE;
  findReachable(initialStates);

  TransitionSet newInitialStates;
  newInitialStates.rename(initialStates, renaming);
  initialStates.swap(newInitialStates);

  Vector<TransitionSet> newStates(nrNewStates);
  for (int i = 0; i < nrStates; i++)
    {
      int newName = renaming[i];
      if (newName != NONE)
	newStates[newName].rename(states[i], renaming);
    }
  states.swap(newStates);

  Vector<int> newFinalStates;
  int nrFinalStates = finalStates.length();
  for (int i = 0; i < nrFinalStates; i++)
    {
      int newName = renaming[finalStates[i]];
      if (newName != NONE)
	newFinalStates.append(newName);
    }
  finalStates.swap(newFinalStates);
}

void
VeryWeakAlternatingAutomaton::findReachable(const TransitionSet& ts)
{
  const TransitionSet::TransitionMap& tm = ts.getMap();
  TransitionSet::TransitionMap::const_iterator ei = tm.end();
  for (TransitionSet::TransitionMap::const_iterator i = tm.begin(); i != ei; ++i)
    {
      NatSet::const_iterator ej = i->first.end();
      for (NatSet::const_iterator j = i->first.begin(); j != ej; ++j)
	{
	  int k = *j;
	  if (renaming[k] == NONE)
	    {
	      renaming[k] = nrNewStates++;
	      findReachable(states[k]);
	    }
	}
    }
}

bool
VeryWeakAlternatingAutomaton::checkFairness(const Transition& transition,
					    int finalStateIndex) const
{
  if (!(transition.first.contains(finalStateIndex)))
    return true;
  const TransitionSet::TransitionMap& tm = states[finalStateIndex].getMap();
  TransitionSet::TransitionMap::const_iterator e = tm.end();
  for (TransitionSet::TransitionMap::const_iterator i = tm.begin(); i != e; ++i)
    {
      if (!(i->first.contains(finalStateIndex)) &&  // no self loop
	  transition.first.contains(i->first) &&  // contained in test transition
	  transition.second.implies(i->second))  // implied by test transition
	return true;
    }
  return false;
}

void
VeryWeakAlternatingAutomaton::computeFairnessSet(const Transition& transition,
						 NatSet& fairnessSet)
{
  int nrFinalsStates = finalStates.length();
  for (int i = 0; i < nrFinalsStates; i++)
    {
      if (checkFairness(transition, finalStates[i]))
	fairnessSet.insert(i);
    }
}

void
VeryWeakAlternatingAutomaton::dump(ostream& s) const
{
  s << "begin{VeryWeakAlternatingAutomaton}\n";
  int nrFinalStates = finalStates.length();
  int nrStates = states.length();
  for (int i = 0; i < nrStates; i++)
    {
      s << "state " << i;
      for (int j = 0 ; j < nrFinalStates; j++)
	{
	  if (finalStates[j] == i)
	    {
	      s << "\tfinal";
	      break;
	    }
	}
      s << '\n';
      states[i].dump(s);
      s << '\n';
    }
  s << "initial state conjunctions\n";
  initialStates.dump(s);
  s << "end{VeryWeakAlternatingAutomaton}\n";
}
