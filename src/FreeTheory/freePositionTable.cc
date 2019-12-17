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
//	Implementation for class FreePositionTable.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	free theory class definitions
#include "freePositionTable.hh"

int
FreePositionTable::position2Index(const Vector<int>& position)
{
  int nrPositions = table.length();
  if (nrPositions == 0)
    return build(position, 0);
  int length = position.length();
  int t = 0;
  for (int i = 0; i < length; i++)
    {
      Vector<int>& arcs = table[t].arcs;
      int nrArgs = arcs.length();
      int argNr = position[i];
      if (argNr >= nrArgs)
	{
	  arcs.expandTo(argNr + 1);
	  for (int j = nrArgs; j < argNr; j++)
	    arcs[j] = NONE;
	  arcs[argNr] = nrPositions;  // this is where build will start
	  return build(position, i + 1);
	}
      else
	{
	  t = arcs[argNr];
	  if (t == NONE)
	    {
	      arcs[argNr] = nrPositions;  // this is where build will start
	      return build(position, i + 1);
	    }
	}
    }
  return t;
}

int
FreePositionTable::build(const Vector<int>& position, int start)
{
  int next = table.length();
  int length = position.length();
  table.expandBy(length - start + 1);
  for (int i = start; i < length; i++)
    {
      //
      //	Create a new node by filling out its Vector as a prefix of
      //	position and creating its arcs.
      //
      Pair& p = table[next];
      p.position.expandTo(i);
      for (int j = 0; j < i; j++)
	p.position[j] = position[j];

      int argNr = position[i];
      p.arcs.expandTo(argNr + 1);
      for (int j = 0; j < argNr; j++)
	p.arcs[j] = NONE;
      p.arcs[argNr] = ++next;
    }
  //
  //	Last node contains full position and no arcs.
  //
  table[next].position = position;  // deep copy;
  return next;
}

int
FreePositionTable::parentIndex(int index) const
{
  const Vector<int>& position = table[index].position;
  int pathLength = position.length() - 1;
  if (pathLength < 0)
    return NONE;
  int t = 0;
  for (int i = 0; i < pathLength; i++)
    t = table[t].arcs[position[i]];
  return t;
}

