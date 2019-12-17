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
//	Implementation for class TransitionSet.
//
#include <set>

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"
#include "bddUser.hh"  // for dump()

//	temporal class definitions
#include "transitionSet.hh"

void 
TransitionSet::insert(const Transition& transition)
{
  Bdd formula = transition.second;
  if (formula == bdd_false())
    return;  // ignore empty set of valuations for robustness
  const TransitionMap::iterator e = transitionMap.end();
  TransitionMap::iterator equal = e;
  for (TransitionMap::iterator i = transitionMap.begin(); i != e;)
    {
      TransitionMap::iterator t = i++;  // because we might want to erase(t)
      if (t->first == transition.first)
	{
	  //
	  //	We've found a pair with the same conjunction of next states.
	  //	There can only be one of these, but we keep going because
	  //	we may need to trim the valuations of some other pair with
	  //	a strictly larger conjunction of states, or some other
	  //	pair with a strictly smaller conjunction of states may
	  //	need to trim our valuation set.
	  //
	  equal = t;
	}
      else if (t->first.contains(transition.first))
	{
	  //
	  //	Existing  pair has a strictly larger conjunction of next states.
	  //	We subtract out our valuations from its valuations and
	  //	erase it if this becomes empty.
	  //
	  t->second = bdd_and(t->second, bdd_not(formula));
	  if (t->second == bdd_false())
	    transitionMap.erase(t);  // existing pair completely subsumed
	}
      else if (transition.first.contains(t->first))
	{
	  //
	  //	Existing pair has a strictly smaller conjunction of next states.
	  //	We subtract out its valuations from ours and quit
	  //	if this becomes empty.
	  //
	  formula = bdd_and(formula, bdd_not(t->second));
	  if (formula == bdd_false())
	    return;  // new transition completely subsumed
	}
    }
  if (equal == e)
    {
      //
      //	No existing pair with the same conjunction of next states
      //	so we juct do a simple insert.
      //
      pair<TransitionMap::iterator, bool> p = transitionMap.insert(transition);
      Assert(p.second, "failed to insert transition but no pair with the same conjunction of states currently exists");
      p.first->second = formula;  // may have been updated in loop
    }
  else
    {
      //
      //	OR our valuations into the valuations of a pair with the same
      //	conjunction of next states.
      //
      equal->second = bdd_or(equal->second, formula);
    }
}

void
TransitionSet::insert(const TransitionSet& transitionSet)
{
  const TransitionMap::const_iterator e = transitionSet.transitionMap.end();
  for (TransitionMap::const_iterator i = transitionSet.transitionMap.begin(); i != e; ++i)
    insert(*i);
}

void
TransitionSet::product(const TransitionSet& ts1, const TransitionSet& ts2)
{
  transitionMap.clear();
  const TransitionMap::const_iterator ie = ts1.transitionMap.end();
  const TransitionMap::const_iterator je = ts2.transitionMap.end();
  Transition newTransition;
  for (TransitionMap::const_iterator i = ts1.transitionMap.begin(); i != ie; ++i)
    {
      for (TransitionMap::const_iterator j = ts2.transitionMap.begin(); j != je; ++j)
	{
	  newTransition.second = bdd_and(i->second, j->second);
	  if (newTransition.second != bdd_false())
	    {
	      newTransition.first = i->first;
	      newTransition.first.insert(j->first);
	      insert(newTransition);
	    }
	}
    }
}

void
TransitionSet::rename(const TransitionSet& original, const Vector<int>& renaming)
{
  //
  //	Renaming must be injective: no two old states must map to
  //	the same new state. We make use of that to do a fast insertion
  //	of the renamed state sets.
  //
  transitionMap.clear();
  const TransitionMap::const_iterator ei = original.transitionMap.end();
  for (TransitionMap::const_iterator i = original.transitionMap.begin(); i != ei; ++i)
    {
      Transition t;
      const NatSet::const_iterator ej = i->first.end();
      for (NatSet::const_iterator j  = i->first.begin(); j != ej; ++j)
	{
	  Assert(renaming[*j] != NONE, "no renaming for " << *j);
	  t.first.insert(renaming[*j]);
	}
      t.second = i->second;
      transitionMap.insert(t);
    }
}

void
TransitionSet::dump(ostream& s, int indentLevel) const
{
  const TransitionMap::const_iterator e = transitionMap.end();
  for (TransitionMap::const_iterator i = transitionMap.begin(); i != e; ++i)
    {
      s << Indent(indentLevel) << i->first << '\t';
      BddUser::dump(s, i->second);
      s << '\n';
    }
}
