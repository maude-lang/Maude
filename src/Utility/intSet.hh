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
//      Class for hash table based sets of ints.
//

#ifndef _intSet_hh_
#define _intSet_hh_

class IntSet
{
public:
  IntSet();
  IntSet(const IntSet& original);

  int insert(int k);  // return index of inserted int
  void subtract(int i);
  bool contains(int i) const;
  int int2Index(int i) const;
  void makeEmpty();
  void insert(const IntSet& other);
  void subtract(const IntSet& other);
  void intersect(const IntSet& other);
  IntSet& operator=(const IntSet& original);
  bool empty() const;
  bool contains(const IntSet& other) const;
  bool disjoint(const IntSet& other) const;
  int cardinality() const;
  bool operator==(const IntSet& other) const;
  bool operator!=(const IntSet& other) const;
  int index2Int(int i) const;

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
IntSet::IntSet()
{
}

inline
IntSet::IntSet(const IntSet& original)
  : intTable(original.intTable),
    hashTable(original.hashTable)
{
}

inline bool
IntSet::contains(int i) const
{
  return intTable.length() != 0 && hashTable[findEntry(i)] != UNUSED;
}

inline int
IntSet::int2Index(int i) const
{
  return (intTable.length() != 0) ? hashTable[findEntry(i)] : NONE;
} 

inline void
IntSet::makeEmpty()
{
  intTable.contractTo(0);
  hashTable.contractTo(0);
}

inline IntSet&
IntSet::operator=(const IntSet& original)
{
  intTable = original.intTable;  // deep copy
  hashTable = original.hashTable;  // deep copy
  return *this;
}

inline bool
IntSet::empty() const
{
  return intTable.length() == 0;
}

inline int
IntSet::cardinality() const
{
  return intTable.length();
}

inline bool
IntSet::operator==(const IntSet& other) const
{
  return intTable.length() == other.intTable.length() && contains(other);
}

inline bool
IntSet::operator!=(const IntSet& other) const
{
  return !(*this == other);
}

inline int
IntSet::index2Int(int i) const
{
  return intTable[i];
}

#endif
