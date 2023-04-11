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
//      Class for hash table based sets of ints.
//

#ifndef _intSet_hh_
#define _intSet_hh_
#include "vector.hh"

class IntSet
{
public:
  typedef Vector<int>::const_iterator const_iterator;
  //
  //	Have the compiler make the default constructor, copy constructor and copy asignment operator.
  //
  IntSet() = default;
  IntSet(const IntSet&) = default;
  IntSet& operator=(const IntSet&) = default;

  IntSet(int expectedSize);

  int insert(int k);  // returns index of inserted int
  int find(int k) const;  // returns index or NONE
  bool contains(int k) const;
  int erase(int i);
  int ithElement(int i) const;
  
  const_iterator begin() const;
  const_iterator end() const;

  int size() const;
  bool empty() const;

  void clear();
  void swap(IntSet& other);
  
  void insert(const IntSet& other);
  void subtract(const IntSet& other);
  void intersect(const IntSet& other);
  bool contains(const IntSet& other) const;
  bool disjoint(const IntSet& other) const;
  bool operator==(const IntSet& other) const;
  bool operator!=(const IntSet& other) const;

private:
  enum Parameters
  {
    STARTING_HASH_TABLE_SIZE = 8
  };

  static int hash(int i);
  static int hash2(int i);
  int findEntry(int i) const;
  void resize(int minSize);
  void rehash();

  Vector<int> intTable;
  Vector<int> hashTable;
};

inline
IntSet::IntSet(int expectedSize)
{
  intTable.reserve(expectedSize);
  resize(2 * expectedSize);  // no more the 50% load factor
}

inline int
IntSet::size() const
{
  return intTable.size();
}

inline bool
IntSet::empty() const
{
  return intTable.empty();
}

inline IntSet::const_iterator
IntSet::begin() const
{
  return intTable.begin();
}

inline IntSet::const_iterator
IntSet::end() const
{
  return intTable.end();
}

inline int
IntSet::find(int k) const
{
  return !empty() ? hashTable[findEntry(k)] : NONE;
} 

inline bool
IntSet::contains(int k) const
{
  return !empty() && hashTable[findEntry(k)] != UNUSED;
}

inline int
IntSet::ithElement(int i) const
{
  return intTable[i];
}

inline void
IntSet::clear()
{
  intTable.clear();
  hashTable.clear();
}

inline void
IntSet::swap(IntSet& other)
{
  intTable.swap(other.intTable);
  hashTable.swap(other.hashTable);
}

inline bool
IntSet::operator==(const IntSet& other) const
{
  return size() == other.size() && contains(other);
}

inline bool
IntSet::operator!=(const IntSet& other) const
{
  return !(*this == other);
}

#endif
