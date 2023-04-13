/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class IntSet.
//
#include <functional>
#include "macros.hh"
#include "vector.hh"
#include "intSet.hh"

inline int
IntSet::hash(int i)
{
  return std::hash<int>{}(i);
}
 
inline int
IntSet::hash2(int i)  // 2nd hash function must always return an odd value
{
  std::size_t h = std::hash<int>{}(i);
  return (h ^ (h >> 8)) | 1;
}

int
IntSet::insert(int k)
{
  if (intTable.empty())
    resize(STARTING_HASH_TABLE_SIZE);
  int i = findEntry(k);
  int j = hashTable[i];
  if (j == UNUSED)
    {
      j = intTable.length();
      intTable.append(k);
      int size = 2 * (j + 1);
      if (size > hashTable.length())
	resize(size);
      else
	hashTable[i] = j;
    }
  return j;
}

void
IntSet::insert(const IntSet& other)
{
  int size = 2 * (intTable.length() + other.intTable.length());
  if (size > hashTable.length())
    resize(size);  // pre-emptively resize hash table based on estimate
  for (int i = other.intTable.length() - 1; i >= 0; i--)
    {
      int k = other.intTable[i];
      int j = findEntry(k);
      if (hashTable[j] == UNUSED)
	{
	  hashTable[j] = intTable.length();
	  intTable.append(k);
	}
    }
}

int
IntSet::erase(int k)
{
  int n = intTable.length();
  if (n == 0)
    return 0;
  int i = findEntry(k);
  int j = hashTable[i];
  if (j == UNUSED)
    return 0;
  --n;
  intTable[j] = intTable[n];
  intTable.contractTo(n);
  rehash();  // removing an element is expensive with closed hashing
  return 1;
}

void
IntSet::subtract(const IntSet& other)
{
  int n = intTable.length();
  if (n == 0 || other.intTable.empty())
    return;
  int j = 0;
  for (int i = 0; i < n; i++)
    {
      int k = intTable[i];
      if (other.hashTable[other.findEntry(k)] == UNUSED)
	intTable[j++] = k;
    }
  if (j != n)
    {
      intTable.contractTo(j);
      rehash();
    }
}

void
IntSet::intersect(const IntSet& other)
{
  int n = intTable.length();
  if (n == 0)
    return;
  if (other.intTable.empty())
    {
      intTable.clear();
      hashTable.clear();
      return;
    }
  int j = 0;
  for (int i = 0; i < n; i++)
    {
      int k = intTable[i];
      if (other.hashTable[other.findEntry(k)] != UNUSED)
	intTable[j++] = k;
    }
  if (j != n)
    {
      intTable.contractTo(j);
      rehash();
    }
}

bool
IntSet::contains(const IntSet& other) const
{
  int i = other.intTable.length();
  if (i == 0)
    return true;
  if (intTable.length() < i)
    return false;
  for (i--; i >= 0; i--)
    {
      if (hashTable[findEntry(other.intTable[i])] == UNUSED)
	return false;
    }
  return true;
}

bool
IntSet::disjoint(const IntSet& other) const
{
  int i = other.intTable.length();
  if (i == 0 || intTable.empty())
    return true;
  for (i--; i >= 0; i--)
    {
      if (hashTable[findEntry(other.intTable[i])] != UNUSED)
	return false;
    }
  return true;
}

int
IntSet::findEntry(int k) const
{
  Assert(!hashTable.empty(), "empty length hash table");
  int mask = hashTable.length() - 1;
  int i = hash(k) & mask;
  int j = hashTable[i];
  if (j != UNUSED && intTable[j] != k)
    {
      int step = hash2(k);
      do
	{
	  i = (i + step) & mask;
	  j = hashTable[i];
	}
      while (j != UNUSED && intTable[j] != k);
    }
  return i;
}

void
IntSet::resize(int minSize)
{
  int n = hashTable.length();
  n = (n == 0) ? STARTING_HASH_TABLE_SIZE : 2 * n;
  while (n < minSize)
    n *= 2;
  hashTable.expandTo(n);
  rehash();
}

void
IntSet::rehash()
{
  int n = hashTable.length();
  for (int i = 0; i < n; i++)
    hashTable[i] = UNUSED;
  int mask = n - 1;
  for (int i = intTable.length() - 1; i >= 0; i--)
    {
      int k = intTable[i];
      int j = hash(k) & mask;
      if (hashTable[j] != UNUSED)
	{
	  int step = hash2(k);
	  do
	    j = (j + step) & mask;
	  while (hashTable[j] != UNUSED);
	}
      hashTable[j] = i;
    }
}
