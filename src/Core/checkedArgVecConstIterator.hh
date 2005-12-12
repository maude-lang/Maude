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
//	Nested class within ArgVec<T> for checked const_iterators.
//
//	Note that in this implementation we assume that swap()ing a pair of
//	ArgVecs invalidates all iterators that point to their elements.
//	This is stricter but safer than other possibilities.
//

template<class T>
class ArgVec<T>::const_iterator
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
  const_iterator();
  const_iterator(const const_iterator& other);
  const_iterator(const iterator& other);  // conversion ctor
  const_iterator& operator=(const const_iterator& other);
  bool operator==(const const_iterator& other) const;
  bool operator!=(const const_iterator& other) const;
  const_reference operator*() const;
  const_pointer operator->() const;
  //
  //	Forward iterator stuff.
  //
  const_iterator& operator++();
  const_iterator operator++(int);
  //
  //	Bidirectional iterator stuff.
  //
  const_iterator& operator--();
  const_iterator operator--(int);
  //
  //	Random access iterator stuff.
  //
  const_iterator& operator+=(difference_type delta);
  const_iterator operator+(difference_type delta) const;
  const_iterator& operator-=(difference_type delta);
  const_iterator operator-(difference_type delta) const;
  difference_type operator-(const const_iterator& other) const;
  const_reference operator[](difference_type i) const;
  bool operator<(const const_iterator& other) const;
  
private:
  const_iterator(const ArgVec* parent, size_type index);
  void checkValid() const;
  void checkDereferenceable() const;
  
  const_pointer ptr;
  const ArgVec* parent;
  size_type index;
  
  friend class ArgVec;
};

template<class T>
inline void
ArgVec<T>::const_iterator::checkValid() const
{
  Assert(parent != 0, "uninitialized iterator");
  Assert(index <= parent->len,
	 "index > len (" << index << " > " << parent->len << ')');
  Assert(ptr == parent->basePtr + index, "bad pointer");
}

template<class T>
inline void
ArgVec<T>::const_iterator::checkDereferenceable() const
{
  Assert(parent != 0, "uninitialized iterator");
  Assert(index < parent->len,
	 "index >= len (" << index << " >= " << parent->len << ')');
  Assert(ptr == parent->basePtr + index, "bad pointer");
}

template<class T>
inline
ArgVec<T>::const_iterator::const_iterator()
{
  parent = 0;
}

template<class T>
inline
ArgVec<T>::const_iterator::const_iterator(const const_iterator& other)
{
  if (other.parent != 0)
    other.checkValid();
  ptr = other.ptr;
  parent = other.parent;
  index = other.index;
}

template<class T>
inline
ArgVec<T>::const_iterator::const_iterator(const iterator& other)
{
  if (other.parent != 0)
    other.checkValid();
  ptr = other.ptr;
  parent = other.parent;
  index = other.index;
}

template<class T>
inline
ArgVec<T>::const_iterator::const_iterator(const ArgVec* parent, size_type index)
  : ptr(parent->basePtr + index),
    parent(parent),
    index(index)
{
}

template<class T>
inline typename ArgVec<T>::const_iterator&
ArgVec<T>::const_iterator::operator=(const const_iterator& other)
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
ArgVec<T>::const_iterator::operator==(const const_iterator& other) const
{
  checkValid();
  other.checkValid();
  return ptr == other.ptr;
}

template<class T>
inline bool
ArgVec<T>::const_iterator::operator!=(const const_iterator& other) const
{
  checkValid();
  other.checkValid();
  return ptr != other.ptr;
}

template<class T>
inline typename ArgVec<T>::const_reference
ArgVec<T>::const_iterator::operator*() const
{
  checkDereferenceable();
  return *ptr;
}

template<class T>
inline typename ArgVec<T>::const_pointer
ArgVec<T>::const_iterator::operator->() const
{
  checkDereferenceable();
  return ptr;
}

template<class T>
inline typename ArgVec<T>::const_iterator&
ArgVec<T>::const_iterator::operator++()
{
  checkDereferenceable();
  ++ptr;
  ++index;
  return *this;
}

template<class T>
inline typename ArgVec<T>::const_iterator
ArgVec<T>::const_iterator::operator++(int)
{
  typename ArgVec::const_iterator tmp(*this);
  operator++();
  return tmp;
}

template<class T>
inline typename ArgVec<T>::const_iterator&
ArgVec<T>::const_iterator::operator--()
{
  checkValid();
  Assert(index > 0, "decrementing past start");
  --ptr;
  --index;
  return *this;
}

template<class T>
inline typename ArgVec<T>::const_iterator
ArgVec<T>::const_iterator::operator--(int)
{
  typename ArgVec::const_iterator tmp(*this);
  operator--();
  return tmp;
}

template<class T>
inline typename ArgVec<T>::const_iterator&
ArgVec<T>::const_iterator::operator+=(difference_type delta)
{
  checkValid();
  Assert(index + delta <= parent->len, "past end");
  ptr += delta;
  index += delta;
  return *this;
}

template<class T>
inline typename ArgVec<T>::const_iterator
ArgVec<T>::const_iterator::operator+(difference_type delta) const
{
  typename ArgVec::const_iterator tmp(*this);
  return tmp += delta;
}

template<class T>
inline typename ArgVec<T>::const_iterator&
ArgVec<T>::const_iterator::operator-=(difference_type delta)
{
  checkValid();
  Assert(index - delta <= parent->len, "past end");
  ptr -= delta;
  index -= delta;
  return *this;
}

template<class T>
inline typename ArgVec<T>::const_iterator
ArgVec<T>::const_iterator::operator-(difference_type delta) const
{
  typename ArgVec::const_iterator tmp(*this);
  return tmp -= delta;
}

template<class T>
inline typename ArgVec<T>::const_reference
ArgVec<T>::const_iterator::operator[](difference_type i) const
{
  checkValid();
  Assert(index + i <= parent->len, "past end");
  return ptr[i];
}

template<class T>
inline bool
ArgVec<T>::const_iterator::operator<(const const_iterator& other) const
{
  checkValid();
  other.checkValid();
  Assert(parent == other.parent, "incomparable iterators");
  return ptr < other.ptr;
}

template<class T>
inline typename ArgVec<T>::difference_type
ArgVec<T>::const_iterator::operator-(const const_iterator& other) const
{
  checkValid();
  other.checkValid();
  Assert(parent == other.parent, "incomparable iterators");
  return ptr - other.ptr;
}
