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
//	Class template for vectors with optional bounds checking.
//
#ifndef _vector_hh_
#define _vector_hh_
#include <type_traits>
#include <algorithm>
#include "preVector.hh"

template<class T>
class Vector
{
public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

#ifdef NO_ASSERT
  typedef pointer iterator;
  typedef const_pointer const_iterator;
#else
  class iterator;
  friend class iterator;
  class const_iterator;
  friend class const_iterator;
#endif

  Vector();
  Vector(size_type length);
  Vector(const Vector& original);
  Vector(Vector&& original);  // move ctor
  ~Vector();
  Vector& operator=(const Vector& original);
  Vector& operator=(Vector<T>&& original);  // move assignment
  //
  //	We don't support rbegin()/rend() - their semantics are too error prone; or cbegin()/cend()/crbegin()/crend().
  //
  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  //
  //	We don't currently support max_size() or shrink_to_fit().
  //
  size_type size() const;
  void resize(size_type length);
  size_type capacity() const;
  bool empty() const;			// significantly slower than isNull()
  void reserve(size_type n);
  //
  //	We don't support at() since we support bounds checking as a compile time option.
  //
  const T& operator[](size_type i) const;	// access for reading
  T& operator[](size_type i);			// access for writing
  const T& front() const;
  T& front();
  const T& back() const;
  T& back();
  const T* data() const;
  T* data();
  //
  //	We don't support assign(), insert(), erase(), emplace(), emplace_back()
  //
  void push_back(const T& item);
  void push_back(T&& item);	// for item that can be moved
  void pop_back();
  void swap(Vector& other);
  void clear();
  //
  //	Non-standard operators without a standard alternative.
  //
  void expandTo(size_type newLength);
  void expandBy(size_type extra);
  void contractTo(size_type newLength);
  bool isNull() const;			// superfast test for capacity() == 0
  //
  //	Legacy stuff; don't use in new code.
  //
  int length() const;
  void append(const T& item);
  void append(T&& item);	// for item that can be moved

private:
  void reallocate(size_t neededBytes, size_type oldLength);

  PreVector pv;
};

template<class T>
inline
Vector<T>::Vector()
{
  pv.initEmpty();
}

template<class T>
inline
Vector<T>::Vector(size_type length)
{
  if (length == 0)
    pv.initEmpty();
  else
    {
      pv.initAllocate(length * sizeof(T));
      pv.setLength(length);
      T* vec = static_cast<T*>(pv.getBase());
      size_type i = 0;
      do
	(void) new(vec + i) T();  // call default constructor on each element
      while (++i < length);
    }
}

template<class T>
inline
Vector<T>::Vector(const Vector& original)
{
  size_type originalLength = original.length();
  if (originalLength == 0)
    pv.initEmpty();
  else
    {
      size_t neededBytes = originalLength *  sizeof(T);
      pv.initAllocate(neededBytes);
      pv.setLength(originalLength);
      if (std::is_trivially_copyable<T>::value)
	std::memcpy(pv.getBase(), original.pv.getBase(), neededBytes);
      else
	{
	  T* vec = static_cast<T*>(pv.getBase());
	  const T* originalVec = static_cast<const T*>(original.pv.getBase());
	  size_type i = 0;
	  do
	    new(vec + i) T(originalVec[i]);  // call copy constructor on each element
	  while (++i != originalLength);
	}
    }
}

template<class T>
inline
Vector<T>::Vector(Vector&& original)
{
  //
  //	Move ctor; sets original to null Vector.
  //
  pv.initSteal(original.pv);
  original.pv.initEmpty();
}

template<class T>
inline
Vector<T>::~Vector()
{
  T* vec = static_cast<T*>(pv.getBase());
  if (vec != nullptr)
    {
      size_type length = pv.getLength();
      for (size_type i = 0; i != length; i++)
	vec[i].~T();  // call destructor on each object in array
      pv.freeMemory();
    }
}

template<class T>
inline typename Vector<T>::size_type
Vector<T>::size() const
{
  return pv.getLength();
}

template<class T>
inline typename Vector<T>::size_type
Vector<T>::capacity() const
{
  return pv.getAllocatedBytes() / sizeof(T);
}

template<class T>
inline const T*
Vector<T>::data() const
{
  return static_cast<const T*>(pv.getBase());
}

template<class T>
inline T*
Vector<T>::data()
{
  return static_cast<T*>(pv.getBase());
}

template<class T>
inline const T&
Vector<T>::front() const
{
  Assert(!empty(), "empty vector");
  return *(data());
}

template<class T>
inline T&
Vector<T>::front()
{
  Assert(!empty(), "empty vector");
  return *(data());
}

template<class T>
inline const T&
Vector<T>::back() const
{
  Assert(!empty(), "empty vector");
  return data()[size() - 1];
}

template<class T>
inline T&
Vector<T>::back()
{
  Assert(!empty(), "empty vector");
  return data()[size() - 1];
}

template<class T>
inline const T&
Vector<T>::operator[](size_type i) const
{
  Assert(i < size(), "index (" << i << ") too big, size: " << size());
  return data()[i];
}

template<class T>
inline T&
Vector<T>::operator[](size_type i)
{
  Assert(i < size(), "index (" << i << ") too big, size: " << size());
  return data()[i];
}

template<class T>
inline void
Vector<T>::reallocate(size_t neededBytes, size_type oldLength)
{
  //
  //	We don't copy the length after reallocation in the expectation that the
  //	caller will set a new length.
  //
  PreVector tmp;
  tmp.initAllocate(neededBytes);
  void* base = pv.getBase();
  if (base != nullptr)
    {
      if (std::is_trivially_copyable<T>::value)
	{
	  //
	  //	The destructor is required to be trivial (performs no action) so
	  //	we just copy the original as bytes without destructing it.
	  //
	  std::memcpy(tmp.getBase(), base, oldLength * sizeof(T));
	}
      else
	{
	  T* originalVec = static_cast<T*>(base);
	  T* vec = static_cast<T*>(tmp.getBase());
	  for (size_type i = 0; i != oldLength; ++i)
	    {
	      T& objectToMove = originalVec[i];
	      new(vec + i) T(std::move(objectToMove));  // move or copy each orginal element
	      objectToMove.~T();  // then destroy original.
	    }
	}
      pv.freeMemory();
    }
  pv.initSteal(tmp);
}

template<class T>
inline void
Vector<T>::reserve(size_type n)
{
  size_t neededBytes = n * sizeof(T);
  if (pv.getAllocatedBytes() < neededBytes)
    {
      size_type oldLength = pv.getLength();
      reallocate(neededBytes, oldLength);
      pv.setLength(oldLength);
    }
}

template<class T>
inline void
Vector<T>::expandTo(size_type newLength)
{
  size_type oldLength = pv.getLength();
  Assert(newLength >= oldLength, "new length < old length: " << newLength << " < " << oldLength);
  size_t neededBytes = newLength * sizeof(T);
  if (pv.getAllocatedBytes() < neededBytes)
    reallocate(neededBytes, oldLength);

  T* vec = static_cast<T*>(pv.getBase());
  if (vec != nullptr)
    {
      pv.setLength(newLength);
      for (size_type i = oldLength; i != newLength; ++i)
	new(vec + i) T();  // call default constructor on each new element
    }
}

template<class T>
inline void
Vector<T>::expandBy(size_type extra)
{
  if (extra > 0)
    expandTo(size() + extra);
}

template<class T>
inline void
Vector<T>::contractTo(size_type newLength)
{
  size_type oldLength = pv.getLength();
  Assert(newLength <= oldLength, "new length > old length: " <<
	 newLength << " > " << oldLength);
  T* vec = static_cast<T*>(pv.getBase());
  if (vec != nullptr)
    {
      pv.setLength(newLength);
      for (size_type i = newLength; i != oldLength; i++)
	vec[i].~T();  // call destructor on each "lost" element
    }
}

template<class T>
inline void
Vector<T>::clear()
{
  contractTo(0);
}

template<class T>
inline void
Vector<T>::resize(size_type newLength)
{
  size_type t =  pv.getLength();
  if (newLength > t)
    expandTo(newLength);
  else if (newLength < t)
    contractTo(newLength);
}

template<class T>
inline void
Vector<T>::push_back(const T& item)
{
  size_type oldLength = pv.getLength();
  size_type newLength = oldLength + 1;
  size_t neededBytes = newLength * sizeof(T);
  if (pv.getAllocatedBytes() < neededBytes)
    reallocate(neededBytes, oldLength);
  pv.setLength(newLength);

  T* vec = static_cast<T*>(pv.getBase());
  new(vec + oldLength) T(item);  // use copy constructor to copy item into vector
}

template<class T>
inline void
Vector<T>::push_back(T&& item)
{
  size_type oldLength = pv.getLength();
  size_type newLength = oldLength + 1;
  size_t neededBytes = newLength * sizeof(T);
  if (pv.getAllocatedBytes() < neededBytes)
    reallocate(neededBytes, oldLength);
  pv.setLength(newLength);

  T* vec = static_cast<T*>(pv.getBase());
  new(vec + oldLength) T(std::move(item));  // use move constructor if possible
}

template<class T>
inline void
Vector<T>::pop_back()
{
  Assert(!empty(), "empty vector");
  size_type newLength = pv.getLength() - 1;
  data()[newLength].~T();
  pv.setLength(newLength);
}

template<class T>
inline Vector<T>&
Vector<T>::operator=(const Vector<T>& original)
{
  //
  //	destroy destination vector
  //
  void* base = pv.getBase();
  {
    size_type length = pv.getLength();
    T* vec = static_cast<T*>(base);
    for (size_type i = 0; i != length; ++i)
      vec[i].~T();  // call destructor on each object in destination array
  }
  //
  //	reallocate memory if necessary
  //
  size_type originalLength = original.pv.getLength();
  size_t neededBytes = originalLength * sizeof(T);
  if (pv.getAllocatedBytes() < neededBytes)
    {
      if (base != nullptr)
	pv.freeMemory();
      pv.initAllocate(neededBytes);
      base = pv.getBase();
    }
  //
  //	make copy of source vector
  //
  if (base != nullptr)
    {
      pv.setLength(originalLength);
      if (originalLength != 0)
	{
	  //
	  //	Need to copy objects from original.
	  //
	  const void* originalBase = original.pv.getBase();
	  if (std::is_trivially_copyable<T>::value)
	    std::memcpy(base, originalBase, neededBytes);
	  else
	    {
	      T* vec = static_cast<T*>(base);
	      const T* originalVec = static_cast<const T*>(originalBase);
	      for (size_type i = 0; i != originalLength; ++i)
		new(vec + i) T(originalVec[i]);   // call copy constructor on original objects
	    }
	}
    }
  return *this;
}

template<class T>
inline Vector<T>&
Vector<T>::operator=(Vector<T>&& original)
{
  //
  //	Move assignment.
  //
  //	Destroy destination vector.
  //
  T* vec = static_cast<T*>(pv.getBase());
  if (vec != nullptr)
    {
      size_type length = pv.getLength();
      for (size_type i = 0; i != length; ++i)
	vec[i].~T();  // call destructor on each object in array
      pv.freeMemory();
    }
  //
  //	Move original and set original to null vector.
  //
  pv.initSteal(original.pv);
  original.pv.initEmpty();
  return *this;
}

template<class T>
inline void
Vector<T>::swap(Vector& other)
{
  PreVector t(pv);  // default copy ctor
  pv = other.pv;
  other.pv = t;
}

template<class T>
inline bool
Vector<T>::isNull() const
{
  return pv.getBase() == nullptr;
}

template<class T>
inline bool
Vector<T>::empty() const
{
  return pv.empty();
}

#ifdef NO_ASSERT
//
//	Fast, with no runtime checking.
//
template<class T>
inline typename Vector<T>::iterator
Vector<T>::begin()
{
  return static_cast<pointer>(pv.getBase());
}

template<class T>
inline typename Vector<T>::iterator
Vector<T>::end()
{
  return static_cast<pointer>(pv.getBase()) + pv.getLength();
}

template<class T>
inline typename Vector<T>::const_iterator
Vector<T>::begin() const
{
  return static_cast<const_pointer>(pv.getBase());
}

template<class T>
inline typename Vector<T>::const_iterator
Vector<T>::end() const
{
  return static_cast<const_pointer>(pv.getBase()) + pv.getLength();
}

#else
#include "slowVector.hh"
#endif

//
//	Legacy stuff.
//
template<class T>
inline int
Vector<T>::length() const
{
  return pv.getLength();
}

template<class T>
inline void
Vector<T>::append(const T& item)
{
  size_type oldLength = pv.getLength();
  size_type newLength = oldLength + 1;
  size_t neededBytes = newLength * sizeof(T);
  if (pv.getAllocatedBytes() < neededBytes)
    reallocate(neededBytes, oldLength);
  pv.setLength(newLength);

  T* vec = static_cast<T*>(pv.getBase());
  new(vec + oldLength) T(item);  // use copy constructor to copy item into vector
}

template<class T>
inline void
Vector<T>::append(T&& item)
{
  size_type oldLength = pv.getLength();
  size_type newLength = oldLength + 1;
  size_t neededBytes = newLength * sizeof(T);
  if (pv.getAllocatedBytes() < neededBytes)
    reallocate(neededBytes, oldLength);
  pv.setLength(newLength);

  T* vec = static_cast<T*>(pv.getBase());
  new(vec + oldLength) T(std::move(item));  // use move constructor if possible
}

template<class T>
inline bool
operator<(const Vector<T>& lhs,const Vector<T>& rhs)
{
  return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

#endif
