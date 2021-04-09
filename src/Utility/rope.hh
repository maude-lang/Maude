/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for rope (functional string) data structure.
//
//	This is based on:
//	  Hans-J. Boehm, Russ Atkinson, and Michael Plass, 
//	  "Ropes: an Alternative to Strings", 
//	  Software—Practice & Experience. New York, NY, USA: John Wiley & Sons, Inc.
//	  25(12):1315–1330, December 1995.
//
#ifndef _rope_hh_
#define _rope_hh_

#define DEBUG_REF_COUNT 0

class Rope
{
public:
  typedef char value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  //
  //	We only support const iterators; mutable iterators don't have an efficient implementation.
  //
  class const_iterator;
  friend class const_iterator;

  Rope();
  Rope(const Rope& other);

  Rope(char c);
  Rope(const char* cString);
  Rope(const char* cString, size_type n);

  ~Rope();

  bool operator==(const Rope& other) const;
  bool operator!=(const Rope& other) const;
  bool operator<=(const Rope& other) const;
  bool operator>=(const Rope& other) const;
  bool operator<(const Rope& other) const;
  bool operator>(const Rope& other) const;

  int compare(const Rope& other) const;

  size_type length() const;
  void swap(Rope& other);
  void clear();
  bool empty() const;

  Rope& operator=(const Rope& other);
  Rope operator+(const Rope& other) const;
  Rope& operator+=(const Rope& other);
  Rope substr(size_type offset, size_type length) const;
  size_type copy(char* buffer) const;
  char operator[](size_type n) const;

  const_iterator begin() const;
  const_iterator end() const;
  char* makeZeroTerminatedString() const;  // caller is responsible for deallocation

  friend ostream& operator<<(ostream& s, const Rope& r);

private:
  struct Fragment;

  struct BinNode
  {
    Fragment* left;
    Fragment* right;
  };

  enum Magic
    {
      TEXT_SIZE = sizeof(BinNode),
      END_MARKER = TEXT_SIZE + 1,
      RELAXATION = 16,  // we allow this extra height over Fibonacci rebalance criteria - must be at least 2
#if SIZEOF_UNSIGNED_LONG >= 8
      MAX_BALANCED_HEIGHT = 91,  // max height of a balanced rope with 2^64-1 characters
#else
      MAX_BALANCED_HEIGHT = 45,  // max height of a balanced rope with 2^32-1 characters
#endif
      STACK_SIZE = MAX_BALANCED_HEIGHT + 1 + RELAXATION  // number of stack entries to iterate over a rope with relaxed balance
    };

  typedef char TextNode[TEXT_SIZE];

  struct Fragment
  {
    Fragment(size_type len);

    int refCount;
    int height;
    size_type length;

#if DEBUG_REF_COUNT
    bool deleted;
#endif

    union 
    {
      BinNode node;
      TextNode leaf;
    };
  };

  Rope(Fragment* p);
  static Fragment* makeTree(const char* cString, size_type n, size_type nrLeaves);
  static Fragment* rebalance(Fragment* fragment);
  static void addFragment(Fragment* fragment, Fragment* forest[]);
  static void insertFragment(Fragment* fragment, Fragment* forest[]);
  static Fragment* rawConcat(Fragment* left, Fragment* right);
  static void dumpForest(Fragment* forest[]);
  //
  //	Reference count stuff.
  //
  static void incCount(Fragment* ptr);
  static void decCount(Fragment* ptr);
  static void deepDelete(Fragment* ptr);

  Fragment* ptr;

  static const size_type fiboTable[];
};

class Rope::const_iterator
{
public:
  //
  //	Types needed by STL algorithms.
  //
  typedef random_access_iterator_tag iterator_category;
  typedef char value_type;
  typedef const char* pointer;
  typedef const char& reference;
  typedef const char* const_pointer;
  typedef const char& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  //
  //	Trivial iterator stuff.
  //
  const_iterator();
  const_iterator(const const_iterator& other);

  const_iterator& operator=(const const_iterator& other);
  bool operator==(const const_iterator& other) const;
  bool operator!=(const const_iterator& other) const;
  const_reference operator*() const;
  //const_pointer operator->() const;
  //
  //	Forward iterator stuff.
  //
  const_iterator& operator++();
  //const_iterator operator++(int);
  //
  //	Bidirectional iterator stuff.
  //
  const_iterator& operator--();
  //const_iterator operator--(int);
  //
  //	Random access iterator stuff.
  //
  const_iterator& operator+=(difference_type delta);
  const_iterator operator+(difference_type delta) const;
  const_iterator& operator-=(difference_type delta);
  const_iterator operator-(difference_type delta) const;
  difference_type operator-(const const_iterator& other) const;
  //const_reference operator[](difference_type i) const;
  bool operator<(const const_iterator& other) const;

private:
  struct Entry
  {
    bool right;
    Fragment* ptr;
  };

  const_iterator(const Rope& ptr);  // begin iterator
  const_iterator(const Rope* ptr);  // end iterator

  void rebuildStack();

  size_type absolutePosition;
  size_type index;
  Entry* stackPtr;
  Entry ptrStack[STACK_SIZE];  // an extra slot for leaf node

  friend class Rope;
};

#if DEBUG_REF_COUNT
#define CHECK_COUNT(p)				\
  Assert(p == 0 || (p->refCount > 0 || !(p->deleted)), "p->refCount = " << p->refCount << "   p->deleted = " << p->deleted);
#else
#define CHECK_COUNT(p)
#endif

inline void
Rope::incCount(Fragment* ptr)
{
  if (ptr != 0)
    ++(ptr->refCount);
}

inline void
Rope::decCount(Fragment* ptr)
{
  if (ptr != 0 && --(ptr->refCount) == 0)
    deepDelete(ptr);
}

inline
Rope::Fragment::Fragment(size_type len)
{
  refCount = 1;
  length = len;
#if DEBUG_REF_COUNT
  deleted = false;
#endif
}
  
inline
Rope::Rope()
{
  ptr = 0;
}

inline
Rope::Rope(const Rope& other)
{
  incCount(other.ptr);
  ptr = other.ptr;
}

inline
Rope::Rope(char c)
{
  ptr = new Fragment(1);
  ptr->height = 0;
  ptr->leaf[0] = c;
}

inline
Rope::Rope(Fragment* p)
{
  //
  //	This member function is internal and assumes that the caller has already incremented or set the
  //	reference cound for p appropriately.
  //
  ptr = p;
}

inline Rope::size_type
Rope::length() const
{
  return ptr == 0 ? 0 : ptr->length;
}

inline bool
Rope::empty() const
{
  return ptr == 0;
}

inline void
Rope::swap(Rope& other)
{
  Fragment* t = ptr;
  ptr = other.ptr;
  other.ptr = t;
}

inline void
Rope::clear()
{
  decCount(ptr);
  ptr = 0;
}

inline bool
Rope::operator!=(const Rope& other) const
{
  return !(*this == other);
}

inline bool
Rope::operator>=(const Rope& other) const
{
  return other <= *this;
}

inline bool
Rope::operator<(const Rope& other) const
{
  return !(*this >= other);
}

inline bool
Rope::operator>(const Rope& other) const
{
  return !(*this <= other);
}

inline Rope&
Rope::operator=(const Rope& other)
{
  incCount(other.ptr);
  decCount(ptr);
  ptr = other.ptr;
  return *this;
}

inline Rope&
Rope::operator+=(const Rope& other)
{
  *this = *this + other;
  return *this;
}

inline Rope::const_iterator
Rope::begin() const
{
  return const_iterator(*this);
}

inline Rope::const_iterator
Rope::end() const
{
  return const_iterator(this);
}

inline Rope::const_iterator::difference_type
Rope::const_iterator::operator-(const const_iterator& other) const
{
  return absolutePosition - other.absolutePosition;
}

inline Rope::const_iterator&
Rope::const_iterator::operator+=(difference_type delta)
{
  absolutePosition += delta;
  rebuildStack();
  return *this;
}

inline Rope::const_iterator&
Rope::const_iterator::operator-=(difference_type delta)
{
  absolutePosition -= delta;
  rebuildStack();
  return *this;
}

inline bool
Rope::const_iterator::operator<(const const_iterator& other) const
{
  return absolutePosition < other.absolutePosition;
}

inline Rope::const_iterator
Rope::const_iterator::operator+(difference_type delta) const
{
  return const_iterator(*this) += delta;
}

inline Rope::const_iterator
Rope::const_iterator::operator-(difference_type delta) const
{
  return const_iterator(*this) -= delta;
}

#endif
