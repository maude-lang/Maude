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
//      Implementation for class UnionFind.
//
#include "macros.hh"
#include "vector.hh"
#include "unionFind.hh"

int
UnionFind::makeElement()
{
  int e = vector.length();
  vector.expandBy(1);
  vector[e].next = e;
  vector[e].rank = 0;
  return e;
}

int
UnionFind::findRep(int element) const
{
  //
  //	Find representative
  //
  int r = element;
  while(vector[r].next != r)
    r = vector[r].next;
  //
  //	Compress path
  //
  int j;
  for (int i = element; i != r; i = j)
    {
      j = vector[i].next;
      vector[i].next = r;
    }
  return r;
}

void
UnionFind::formUnion(int e1, int e2)
{
  int r1 = findRep(e1);
  int r2 = findRep(e2);
  if (vector[r1].rank > vector[r2].rank)
    vector[r2].next = r1;
  else
    {
      if (vector[r1].rank == vector[r2].rank)
	++(vector[r2].rank);
      vector[r1].next = r2;
    } 
}
