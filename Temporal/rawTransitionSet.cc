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
//	Implementation for class RawTransitionSet.
//
#include <algorithm>

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	temporal class definitions
#include "rawTransitionSet.hh"

RawTransitionSet::RawTransitionSet(const TransitionSet& transitionSet)
{
  const TransitionSet::TransitionMap& tm = transitionSet.getMap();
  copy(tm.begin(), tm.end(), inserter(*this, begin()));
}

RawTransitionSet::RawTransitionSet(const RawTransitionSet& ts1, const RawTransitionSet& ts2)
{
  //
  //	Construct the raw product of the two argument transition sets.
  //
  const const_iterator ie = ts1.end();
  const const_iterator je = ts2.end();
  Transition newTransition;
  for (const_iterator i = ts1.begin(); i != ie; ++i)
    {
      for (const_iterator j = ts2.begin(); j != je; ++j)
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
