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
//	Class for digraphs.
//
#ifndef _digraph_hh_
#define _digraph_hh_

class Digraph
{
public:
  Digraph(int nrNodes);

  void insertEdge(int from, int to);
  void findLongestDirectedPath(Vector<int>& path);

private:
  void extendPath(int i, Vector<int>& current, Vector<int>& longest);

  const int nodeCount;
  Vector<Bool> adjMatrix;
};

inline void
Digraph::insertEdge(int from, int to)
{
  adjMatrix[from * nodeCount + to] = true;
}

#endif

