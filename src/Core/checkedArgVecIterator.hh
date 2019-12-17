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
//	Nested class within ArgVec<T> for checked iterators.
//
//	Note that in this implementation we assume that swap()ing a pair of
//	ArgVecs invalidates all iterators that point to their elements.
//	This is stricter but safer than other possibilities.
//

template<class T>
class ArgVec<T>::iterator
// Can't do this because of g++ bug.
//  : public random_access_iterator<T, ArgVec<T>::difference_type>
{
public:
  //
  //	Types needed by STL algorithms.
  //
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  //
  //	Trivial iterator stuff.
  //
  iterator();
  iterator(const iterator& other);
  iterator& operator=(const iterator& other);
  bool operator==(const iterator& other) const;
  bool operator!=(const iterator& other) const;
  reference operator*() const;
  pointer operator->() const;
  //
  //	Forward iterator stuff.
  //
  iterator& operator++();
  iterator operator++(int);
  //
  //	Bidirectional iterator stuff.
  //
  iterator& operator--();
  iterator operator--(int);
  //
  //	Random access iterator stuff.
  //
  iterator& operator+=(difference_type delta);
  iterator operator+(difference_type delta) const;
  iterator& operator-=(difference_type delta);
  iterator operator-(difference_type delta) const;
  difference_type operator-(const iterator& other) const;
  reference operator[](difference_type i) const;
  bool operator<(const iterator& other) const;
  
private:
  iterator(ArgVec* parent, size_type index);
  void checkValid() const;
  void checkDereferenceable() const;
  
  pointer ptr;
  ArgVec* parent;
  size_type index;
  
  friend class ArgVec;
  friend class const_iterator;
};

template<class T>
inline void
ArgVec<T>::iterator::checkValid() const
{
  Assert(parent != 0, "uninitialized iterator");
  Assert(index <= parent->len,
	 "index > len (" << index << " > " << parent->len << ')');
  Assert(ptr == parent->basePtr + index, "bad pointer");
}

template<class T>
inline void
ArgVec<T>::iterator::checkDereferenceable() const
{
  Assert(parent != 0, "uninitialized iterator");
  Assert(index < parent->len,
	 "index >= len (" << index << " >= " << parent->len << ')');
  Assert(ptr == parent->basePtr + index, "bad pointer");
}

template<class T>
inline
ArgVec<T>::iterator::iterator()
{
  parent = 0;
}

template<class T>
inline
ArgVec<T>::iterator::iterator(const iterator& other)
{
  if (other.parent != 0)
    other.checkValid();
  ptr = other.ptr;
  parent = other.parent;
  index = other.index;
}

template<class T>
inline
ArgVec<T>::iterator::iterator(ArgVec* parent, size_type index)
  : ptr(parent->basePtr + index),
    parent(parent),
    index(index)
{
}

template<class T>
inline typename ArgVec<T>::iterator&
ArgVec<T>::iterator::operator=(const iterator& other)
{
  if (other.parent != 0)
    other.checkValid();
  ptr = other.ptr;
  parent = other.parent;
  index = other.index;
  return *this;
}

template<class T>
inline bool
ArgVec<T>::iterator::operator==(const iterator& other) const
{
  checkValid();
  other.checkValid();
  return ptr == other.ptr;
}

template<class T>
inline bool
ArgVec<T>::iterator::operator!=(const iterator& other) const
{
  checkValid();
  other.checkValid();
  return ptr != other.ptr;
}

template<class T>
inline typename ArgVec<T>::reference
ArgVec<T>::iterator::operator*() const
{
  checkDereferenceable();
  return *ptr;
}

template<class T>
inline typename ArgVec<T>::pointer
ArgVec<T>::iterator::operator->() const
{
  checkDereferenceable();
  return ptr;
}

template<class T>
inline typename ArgVec<T>::iterator&
ArgVec<T>::iterator::operator++()
{
  checkDereferenceable();
  ++ptr;
  ++index;
  return *this;
}

template<class T>
inline typename ArgVec<T>::iterator
ArgVec<T>::iterator::operator++(int)
{
  typename ArgVec::iterator tmp(*this);
  operator++();
  return tmp;
}

template<class T>
inline typename ArgVec<T>::iterator&
ArgVec<T>::iterator::operator--()
{
  checkValid();
  Assert(index > 0, "decrementing past start");
  --ptr;
  --index;
  return *this;
}

template<class T>
inline typename ArgVec<T>::iterator
ArgVec<T>::iterator::operator--(int)
{
  typename ArgVec::iterator tmp(*this);
  operator--();
  return tmp;
}

template<class T>
inline typename ArgVec<T>::iterator&
ArgVec<T>::iterator::operator+=(difference_type delta)
{
  checkValid();
  Assert(index + delta <= parent->len, "past end");
  ptr += delta;
  index += delta;
  return *this;
}

template<class T>
inline typename ArgVec<T>::iterator
ArgVec<T>::iterator::operator+(difference_type delta) const
{
  typename ArgVec::iterator tmp(*this);
  return tmp += delta;
}

template<class T>
inline typename ArgVec<T>::iterator&
ArgVec<T>::iterator::operator-=(difference_type delta)
{
  checkValid();
  Assert(index - delta <= parent->len, "past end");
  ptr -= delta;
  index -= delta;
  return *this;
}

template<class T>
inline typename ArgVec<T>::iterator
ArgVec<T>::iterator::operator-(difference_type delta) const
{
  typename ArgVec::iterator tmp(*this);
  return tmp -= delta;
}

template<class T>
inline typename ArgVec<T>::reference
ArgVec<T>::iterator::operator[](difference_type i) const
{
  checkValid();
  Assert(index + i <= parent->len, "past end");
  return ptr[i];
}

template<class T>
inline bool
ArgVec<T>::iterator::operator<(const iterator& other) const
{
  checkValid();
  other.checkValid();
  Assert(parent == other.parent, "incomparable iterators");
  return ptr < other.ptr;
}

template<class T>
inline typename ArgVec<T>::difference_type
ArgVec<T>::iterator::operator-(const iterator& other) const
{
  checkValid();
  other.checkValid();
  Assert(parent == other.parent, "incomparable iterators");
  return ptr - other.ptr;
}
