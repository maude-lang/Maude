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
//      Implementation for class Graph
//
#include "macros.hh"
#include "vector.hh"
#include "natSet.hh"
#include "graph.hh"

Graph::Graph(int nrNodes)
  : nodeCount(nrNodes), adjMatrix(nrNodes * nrNodes)
{
  int size = adjMatrix.length();
  for (int i = 0; i < size; i++)
    adjMatrix[i] = false;
}

int
Graph::color(Vector<int>& coloring)
{
  coloring.resize(nodeCount);
  for (int i = 0; i < nodeCount; i++)
    coloring[i] = UNDEFINED;
  int maxColor = UNDEFINED;
  for (int i = 0; i < nodeCount; i++)
    colorNode(i, maxColor, coloring);
  return maxColor + 1;
}

void
Graph::colorNode(int i, int& maxColor, Vector<int>& coloring)
{
  if (coloring[i] != UNDEFINED)
    return;
  NatSet used;
  int offset = i * nodeCount;
  for (int j = 0; j < nodeCount; j++)
    {
      if (adjMatrix[offset + j])
	{
	  int c = coloring[j];
	  if (c != UNDEFINED)
	    used.insert(c);
	}
    }
  for (int c = 0;; c++)
    {
      if (!(used.contains(c)))
	{
	  coloring[i] = c;
	  if (c > maxColor)
	    maxColor = c;
	  break;
	}
    }  
  for (int j = 0; j < nodeCount; j++)
    {
      if (adjMatrix[offset + j])
	colorNode(j, maxColor, coloring);
    }
}

void
Graph::findComponents(Vector<Vector<int> >& components)
{
  NatSet visited;
  for (int i = 0; i < nodeCount; i++)
    {
      if (!visited.contains(i))
	{
	  int nrComponents = components.length();
	  components.expandBy(1);
	  visit(i, components[nrComponents], visited);
	}
    }
}

void
Graph::visit(int i, Vector<int>& component, NatSet& visited)
{
  visited.insert(i);
  component.append(i);
  int base = i * nodeCount;
  for (int j = 0; j < nodeCount; j++)
    {
      if (adjMatrix[base + j] && !(visited.contains(j)))
	visit(j, component, visited);
    }
}

 


