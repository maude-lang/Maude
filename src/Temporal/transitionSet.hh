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
//	Intuitively a transition set maps each valuation to a disjunction
//	of conjunctions of next states. In practice is it maintained as a set of
//		<set of states, boolean formula>
//	pairs where the truth of the boolean formula on a valuation indicates that
//	the set of states in the pair is in the disjunction for that valuation.
//
//	We have the simplification rule that for any valuation, a conjunction
//	of states in its disjunction subsumes any larger conjunction of states
//	in its disjunction.
//
//	We maintain a canonical form by storing the set of pairs as a map
//	from sets of states to formulae such that for any set Q of states, the
//	formulae is true on exactly those valuations in which Q appears as part
//	of the simplified disjunction.
//
#ifndef _transitionSet_hh_
#define _transitionSet_hh_
#include <map>
#include "natSet.hh"
#include "bdd.hh"

typedef pair<NatSet, Bdd> Transition;  // HACK

class TransitionSet
{
public:
  typedef map<NatSet, Bdd> TransitionMap;

  void insert(const Transition& transition);
  void insert(const TransitionSet& transitionSet);
  void product(const TransitionSet& ts1, const TransitionSet& ts2);
  void swap(TransitionSet& other);
  void rename(const TransitionSet& original, const Vector<int>& renaming);

  const TransitionMap& getMap() const;
  void dump(ostream& s, int indentLevel = 0) const;

private:
  TransitionMap transitionMap;
};

inline void
TransitionSet::swap(TransitionSet& other)
{
  transitionMap.swap(other.transitionMap);
}

inline const TransitionSet::TransitionMap&
TransitionSet::getMap() const
{
  return transitionMap;
}

#endif
