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
//	Class for dense sets of natural numbers with constant time membership test.
//
#ifndef _natSet_hh_
#define _natSet_hh_
#include "vector.hh"

class NatSet
{
public:
  typedef int value_type;
  typedef unsigned int size_type;

  class iterator
  {
  public:
    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other) const;
    value_type operator*() const;
    iterator& operator++();
    iterator operator++(int);

  private:
    value_type element;
    const NatSet* natSet;

    friend class NatSet;
  };

  typedef iterator const_iterator;

  NatSet();
  NatSet(const NatSet& original);

  void clear();
  size_type size() const;
  bool empty() const;
  iterator begin() const;
  iterator end() const;

  void insert(value_type i);
  void insert(const NatSet& other);  // i.e. union
  void subtract(value_type i);
  void subtract(const NatSet& other);
  void intersect(const NatSet& other);
  NatSet& operator=(const NatSet& original);
  int_fast8_t contains(value_type i) const;  // constant time
  bool containsSmall(value_type i) const;  // only correct for small arguments
  bool contains(const NatSet& other) const;  // i.e. improper subset test
  bool disjoint(const NatSet& other) const;
  bool operator==(const NatSet& other) const;
  bool operator!=(const NatSet& other) const;
  bool operator<(const NatSet& other) const;  // linear ordering; no other properties guaranteed
  value_type max() const;  // constant time
  value_type min() const;
  void swap(NatSet& other);
  //
  //	Legacy synonyms.
  //
  void makeEmpty();
  int cardinality() const;

private:
  //
  //	long gives that natural word size on most machines.
  //
  typedef unsigned long Word;

  enum Constants
    {
      BITS_PER_WORD = BITS_PER_BYTE * sizeof(Word)
    };

  static int topBit(Word w);
  static int bottomBit(Word w);
  static int countBits(Word w);
  static unsigned int getShift(value_type i);
  static Word getMaskSmall(int i);
  static Word getMask(value_type i);
  static int getWordNr(value_type i);

  value_type arrayMin(int i) const;
  int_fast8_t arrayContains(value_type i) const;
  //
  //	Many sets will fit in a single machine word so we store the first word separately in
  //	the hope of avoiding the allocation of a Vector.
  //
  Word firstWord;
  Vector<Word> array;

  friend class iterator;

public:
  enum Values
    {
      smallIntBound = BITS_PER_WORD - 1
    };
};

inline
NatSet::NatSet()
{
  firstWord = 0;
}

inline
NatSet::NatSet(const NatSet& original)
  : firstWord(original.firstWord),
    array(original.array)
{
}

inline void
NatSet::clear()
{
  firstWord = 0;
  array.clear();
}

inline NatSet&
NatSet::operator=(const NatSet& original)
{
  firstWord = original.firstWord;
  array = original.array;  // deep copy
  return *this;
}

inline bool
NatSet::empty() const
{
  return firstWord == 0 && array.length() == 0;
}

inline NatSet::value_type
NatSet::min() const
{
  return (firstWord == 0) ? arrayMin(0) : bottomBit(firstWord);
}

inline NatSet::iterator
NatSet::begin() const
{
  iterator i;
  i.natSet = this;
  i.element = min();
  return i;
}

inline NatSet::iterator
NatSet::end() const
{
  iterator i;
  i.natSet = this;
  i.element = -1;
  return i;
}

inline int_fast8_t
NatSet::contains(value_type i) const
{
  Assert(i >= 0, "-ve argument");
  return i < BITS_PER_WORD ? ((firstWord >> i) & 1) : arrayContains(i);
}

inline bool
NatSet::containsSmall(value_type i) const
{
  Assert(i >= 0, "-ve argument " << i);
  Assert(i < BITS_PER_WORD, "too big: " << i);
  return ((firstWord >> i) & 1);
}

inline bool
NatSet::operator!=(const NatSet& other) const
{
  return !(*this == other);
}

inline NatSet::value_type
NatSet::max() const
{
  value_type len = array.length();
  if (len == 0)
    return (firstWord == 0) ? -1 : topBit(firstWord);
  return (len * BITS_PER_WORD) + topBit(array[len - 1]);
}

inline void
NatSet::swap(NatSet& other)
{
  Word t = firstWord;
  firstWord = other.firstWord;
  other.firstWord = t;
  array.swap(other.array);
}

//
//	NatSet::iterator
//

inline bool
NatSet::iterator::operator==(const iterator& other) const
{
  return (element == other.element) && (natSet == other.natSet);
}

inline bool
NatSet::iterator::operator!=(const iterator& other) const
{
  return (element != other.element) || (natSet != other.natSet);
}

inline NatSet::value_type
NatSet::iterator::operator*() const
{
  Assert(element != -1, "bad dereference");
  return element;
}

inline NatSet::iterator
NatSet::iterator::operator++(int)
{
  NatSet::iterator tmp(*this);
  operator++();
  return tmp;
}

ostream& operator<<(ostream& s, const NatSet& ns);

inline void 
NatSet::makeEmpty() { clear(); }

inline int
NatSet::cardinality() const { return size(); }

#endif
