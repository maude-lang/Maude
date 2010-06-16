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
//      Class for hash table based sets of pointers to user data structures.
//
//	By default equality is on pointers (equivalently on unique data structures).
//	hash(void*) and isEqual(void*, void*) may be overloaded for a coarser
//	notion of equality that depends on the contents of the data structures.
//

#ifndef _pointerSet_hh_
#define _pointerSet_hh_

class PointerSet
{
public:
  PointerSet();
  PointerSet(const PointerSet& original);
  virtual ~PointerSet();

  //
  //	If these four members are never called then we guarentee that
  //	hash(void*) will never be called. These may be useful in situations
  //	where the hash function is expensive and caching it would be awkward.
  //
  int insert(void* p);
  void subtract(void* p);
  bool contains(void* p) const;
  int pointer2Index(void *p) const;
  //
  //	Alternative members - hash value must be provided with the pointer.
  //
  int insert(void* p, unsigned int rawHashValue);
  void subtract(void* p, unsigned int rawHashValue);
  bool contains(void* p, unsigned int rawHashValue) const;
  int pointer2Index(void *p, unsigned int rawHashValue) const;

  void makeEmpty();
  void insert(const PointerSet& other);
  void subtract(const PointerSet& other);
  void intersect(const PointerSet& other);
  PointerSet& operator=(const PointerSet& original);
  bool empty() const;
  bool contains(const PointerSet& other) const;
  bool disjoint(const PointerSet& other) const;
  int cardinality() const;
  bool operator==(const PointerSet& other) const;
  bool operator!=(const PointerSet& other) const;
  void* index2Pointer(int i) const;
  void swap(PointerSet& other);

protected:
  virtual unsigned int hash(void* pointer) const;
  virtual bool isEqual(void* pointer1, void* pointer2) const;

#ifndef NO_ASSERT
  //
  //	Allow analysis/debugging of collisions.
  //
  virtual void collision(void* pointer1,
			 unsigned int rawHashValue1, 
			 void* pointer2,
			 unsigned int rawHashValue2,
			 unsigned int tableSize,
			 unsigned int disputedSlot) const {}
#endif

private:
  enum Parameters
  {
    STARTING_HASH_TABLE_SIZE = 8
  };

  struct Pair
  {
    void* pointer;
    unsigned int rawHashValue;
  };

  static int localHash(unsigned int rawHashValue);
  static int localHash2(unsigned int rawHashValue);
  bool isEqual(const Pair& pair, void* p, unsigned int rawHashValue) const;
  int findEntry(void* p, unsigned int rawHashValue) const;
  void resize(int minSize);
  void rehash();

  Vector<Pair> pointerTable;
  Vector<int> hashTable;
};

inline
PointerSet::PointerSet()
{
}

inline
PointerSet::PointerSet(const PointerSet& original)
  : pointerTable(original.pointerTable),
    hashTable(original.hashTable)
{
}

//
//	These are the only four functions that are allowed to call hash(void*).
//

inline int
PointerSet::insert(void* p)
{
  return insert(p, hash(p));
}

inline void
PointerSet::subtract(void* p)
{
  if (pointerTable.length() != 0)
    subtract(p, hash(p));
}

inline bool
PointerSet::contains(void* p) const
{
  return pointerTable.length() != 0 && hashTable[findEntry(p, hash(p))] != UNUSED;
}

inline int
PointerSet::pointer2Index(void *p) const
{
  return (pointerTable.length() != 0) ? hashTable[findEntry(p, hash(p))] : NONE;
}

//
//	Alternative versions.
//
inline bool
PointerSet::contains(void* p, unsigned int rawHashValue) const
{
  return pointerTable.length() != 0 && hashTable[findEntry(p, rawHashValue)] != UNUSED;
}

inline int
PointerSet::pointer2Index(void *p, unsigned int rawHashValue) const
{
  return (pointerTable.length() != 0) ? hashTable[findEntry(p, rawHashValue)] : NONE;
} 

//
//	Remaining member functions
//

inline void
PointerSet::makeEmpty()
{
  pointerTable.contractTo(0);
  hashTable.contractTo(0);
}

inline PointerSet&
PointerSet::operator=(const PointerSet& original)
{
  pointerTable = original.pointerTable;  // deep copy
  hashTable = original.hashTable;  // deep copy
  return *this;
}

inline bool
PointerSet::empty() const
{
  return pointerTable.length() == 0;
}

inline int
PointerSet::cardinality() const
{
  return pointerTable.length();
}

inline bool
PointerSet::operator==(const PointerSet& other) const
{
  return pointerTable.length() == other.pointerTable.length() && contains(other);
}

inline bool
PointerSet::operator!=(const PointerSet& other) const
{
  return !(*this == other);
}

inline void*
PointerSet::index2Pointer(int i) const
{
  return pointerTable[i].pointer;
}

inline void
PointerSet::swap(PointerSet& other)
{
  pointerTable.swap(other.pointerTable);
  hashTable.swap(other.hashTable);
}

#endif
