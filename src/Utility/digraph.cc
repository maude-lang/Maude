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
//      Implementation for class Digraph
//
#include "macros.hh"
#include "vector.hh"
#include "digraph.hh"

Digraph::Digraph(int nrNodes)
  : nodeCount(nrNodes), adjMatrix(nrNodes * nrNodes)
{
  int size = adjMatrix.length();
  for (int i = 0; i < size; i++)
    adjMatrix[i] = false;
}

void
Digraph::findLongestDirectedPath(Vector<int>& path)
{
  Vector<int> current;
  for (int i = 0; i < nodeCount; i++)
    extendPath(i, current, path);
}

void
Digraph::extendPath(int i, Vector<int>& current, Vector<int>& longest)
{
  current.append(i);
  int pathLength = current.length();
  bool extended = false;
  int base = i * nodeCount;
  for (int j = 0; j < nodeCount; j++)
    {
      if (adjMatrix[base + j])
	{
	  for (int k = 0; k < pathLength; k++)
	    {
	      if (current[k] == j)
		goto nextEdge;
	    }
	  extendPath(j, current, longest);
	  extended = true;
	}
    nextEdge:
      ;
    }
  if (!extended)
    {
      if (pathLength > longest.length())
	longest = current;  // deep copy
    }
  current.contractTo(pathLength - 1);
}
