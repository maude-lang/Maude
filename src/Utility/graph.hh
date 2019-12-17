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
//	Class for graphs.
//
#ifndef _graph_hh_
#define _graph_hh_
#include <set>

class Graph
{
public:
  Graph(int nrNodes);

  void insertEdge(int n1, int n2);
  int color(Vector<int>& coloring);  // node 0 is guaranteed to get color 0
  void findComponents(Vector<Vector<int> >& components);

private:
  void colorNode(int i, int& maxColor, Vector<int>& coloring);
  void visit(int i, Vector<int>& component, NatSet& visited);

  typedef set<int> AdjSet;

  Vector<AdjSet> adjSets;
};

inline void
Graph::insertEdge(int n1, int n2)
{
  adjSets[n1].insert(n2);
  adjSets[n2].insert(n1);
}

#endif
