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
//      Implementation for class PointerMap
//
//      We use a hash table to map void* to void* with
//      clashes being resolved by open addressing (with double hashing)
//      See Cormen, Lieserson & Rivest p235
//
#include "macros.hh"
#include "vector.hh"
#include "pointerMap.hh"

inline int
PointerMap::hash(void* pointer)
{
  int i = reinterpret_cast<long>(pointer);
  return (i >> NR_PTR_LOSE_BITS) ^ (i >> (2 * NR_PTR_LOSE_BITS));
}

inline int
PointerMap::hash2(void* pointer)  // 2nd hash function must always return an odd value
{
  int i = reinterpret_cast<long>(pointer);
  return ((i >> NR_PTR_LOSE_BITS) ^ (i >> (WORD_SIZE / 2))) | 1;
}

PointerMap::PointerMap(int size)
{
  int s = 2;
  while (s < size)
    s *= 2;
  s *= 2;
  hashTable.expandTo(s);
  for (int i = 0; i < s; i++)
    hashTable[i].from = 0;
  used = 0;
}

void*
PointerMap::setMap(void* from, void* to)
{
  Assert(from != 0, "can't map null pointer");
  if (hashTable.length() <= 2 * used)
    resize();
  Pair& p = hashTable[findEntry(from)];
  void* r;
  if (p.from == 0)
    {
      r = 0;
      p.from = from;
      ++used;
    }
  else
    r = p.to;
  p.to = to;
  return r;
}

void*
PointerMap::getMap(void* from) const
{
  Assert(from != 0, "can't map null pointer");
  const Pair& p = hashTable[findEntry(from)];
  return (p.from == 0) ? 0 : p.to;
}

int
PointerMap::findEntry(void* from) const
{
  int mask = hashTable.length() - 1;
  int i = hash(from) & mask;
  if (hashTable[i].from == 0 || hashTable[i].from == from)
    return i;
  int step = hash2(from);
  do
    i = (i + step) & mask;
  while (hashTable[i].from != 0 && hashTable[i].from != from);
  return i;
}

void
PointerMap::resize()
{
  int s = hashTable.length();
  int n = 2 * s;
  Vector<Pair> newHashTable(n);
  for (int i = 0; i < n; i++)
    newHashTable[i].from = 0;
  int mask = n - 1;
  for (int i = 0; i < s; i++)
    {
      Pair& p = hashTable[i];
      if (p.from != 0 && p.to != 0)
        {
	  int j = hash(p.from) & mask;
	  if (newHashTable[j].from != 0)
	    {
	      int step = hash2(p.from);
	      do
		j = (j + step) & mask;
	      while (newHashTable[j].from != 0);
	    }
	  newHashTable[j] = p;
	}
    }
  hashTable.swap(newHashTable);
}
