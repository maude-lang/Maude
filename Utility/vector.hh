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
//	Class template for vectors with optional bounds checking.
//
#ifndef _vector_hh_
#define _vector_hh_
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

  Vector(size_type length = 0);
  Vector(size_type length, size_type preallocate);
  Vector(const Vector& original);
  ~Vector();

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  const T& operator[](size_type i) const;	// access for reading
  T& operator[](size_type i);			// access for writing
  void expandTo(size_type newLength);
  void expandBy(size_type extra);
  void contractTo(size_type newLength);
  void resize(size_type length);
  void append(const T& item);
  size_type size() const;
  size_type capacity() const;
  Vector& operator=(const Vector& original);
  void swap(Vector& other);
  size_t bytesAllocated() const;
  bool isNull() const;			// superfast test for capacity() == 0
  bool empty() const;			// significantly slower than isNull()
  void clear();
  //
  //	Legacy stuff.
  //
  int length() const;

private:
  PreVector pv;
};

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
//
//	Slow, with extensive runtime checking.
//
#include <checkedIterator.hh>

template<class T>
inline typename Vector<T>::iterator
Vector<T>::begin()
{
  return iterator(this, 0);
}

template<class T>
inline typename Vector<T>::iterator
Vector<T>::end()
{
  return iterator(this, pv.getLength());
}

#include <checkedConstIterator.hh>

template<class T>
inline typename Vector<T>::const_iterator
Vector<T>::begin() const
{
  return const_iterator(this, 0);
}

template<class T>
inline typename Vector<T>::const_iterator
Vector<T>::end() const
{
  return const_iterator(this, pv.getLength());
}

#endif

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
inline size_t
Vector<T>::bytesAllocated() const
{
  return pv.getAllocatedBytes();
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
      T* vector = static_cast<T*>(pv.getBase());
      size_type i = 0;
      do
	(void) new(vector + i) T();  // call default constructor on each element
      while (++i < length);
    }
}

template<class T>
inline
Vector<T>::Vector(size_type length, size_type preallocate)
{
  Assert(preallocate >= length, "preallocate < length: "
	 << preallocate << " < " << length);
  if (preallocate == 0)
    pv.initEmpty();
  else
    {
      pv.initAllocate(preallocate * sizeof(T));
      pv.setLength(length);
      T* vector = static_cast<T*>(pv.getBase());
      for (size_type i = 0; i < length; i++)
	(void) new(vector + i) T();  // call default constructor on each element
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
      pv.initAllocate(originalLength *  sizeof(T));
      pv.setLength(originalLength);
      T* vector = static_cast<T*>(pv.getBase());
      const T* originalVector = static_cast<const T*>(original.pv.getBase());
      size_type i = 0;
      do
	new(vector + i) T(originalVector[i]);  // call copy constructor on each element
      while (++i < originalLength);
    }
}

template<class T>
inline
Vector<T>::~Vector()
{
  T* vector = static_cast<T*>(pv.getBase());
  if (vector != 0)
    {
      size_type length = pv.getLength();
      for (size_type i = 0; i < length; i++)
	vector[i].~T();  // call destructor on each object in array
      pv.freeMemory();
    }
}

template<class T>
inline const T& 
Vector<T>::operator[](size_type i) const
{
  Assert(i < size(), "index (" << i << ") too big, length: " << length());
  return static_cast<const T*>(pv.getBase())[i];
}

template<class T>
inline T& 
Vector<T>::operator[](size_type i)
{
  Assert(i < size(), "index (" << i << ") too big, length: " << length());
  return static_cast<T*>(pv.getBase())[i];
}

template<class T>
inline void
Vector<T>::expandTo(size_type newLength)
{
  size_type oldLength = pv.getLength();
  Assert(newLength >= oldLength, "new length < old length: " <<
	 newLength << " < " << oldLength);
  size_t neededBytes = newLength * sizeof(T);
  if (pv.getAllocatedBytes() < neededBytes)
    {
      PreVector tmp;
      tmp.initAllocate(neededBytes);
      T* originalVector = static_cast<T*>(pv.getBase());
      if (originalVector != 0)
	{
	  T* vector = static_cast<T*>(tmp.getBase());
	  for (size_type i = 0; i < oldLength; i++)
	    {
	      new(vector + i) T(originalVector[i]);  // copy each orginal element
	      originalVector[i].~T();  // then destroy original.
	    }
	  pv.freeMemory();
	}
      pv.initSteal(tmp);
    }
  T* vector = static_cast<T*>(pv.getBase());
  if (vector != 0)
    {
      pv.setLength(newLength);
      for (size_type i = oldLength; i < newLength; i++)
	new(vector + i) T();  // call default constructor on each new element
    }
}

template<class T>
inline void
Vector<T>::expandBy(size_type extra)
{
  if (extra > 0)
    expandTo(length() + extra);
}

template<class T>
inline void
Vector<T>::contractTo(size_type newLength)
{
  size_type oldLength = pv.getLength();
  Assert(newLength <= oldLength, "new length > old length: " <<
	 newLength << " > " << oldLength);
  T* vector = static_cast<T*>(pv.getBase());
  if (vector != 0)
    {
      pv.setLength(newLength);
      for (size_type i = newLength; i < oldLength; i++)
	vector[i].~T();  // call destructor on each "lost" element
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
Vector<T>::append(const T& item)
{
  T* vector = static_cast<T*>(pv.getBase());
  if (vector == 0)
    {
      pv.initAllocate(sizeof(T));
      pv.setLength(1);
      new(static_cast<T*>(pv.getBase())) T(item);
    }
  else
    {
      size_type oldLength = pv.getLength();
      size_type newLength = oldLength + 1;
      size_t neededBytes = newLength * sizeof(T);
      if (pv.getAllocatedBytes() < neededBytes)
	{
	  PreVector tmp;
	  tmp.initAllocate(neededBytes);
	  T* newVector = static_cast<T*>(tmp.getBase());
	  for (size_type i = 0; i < oldLength; i++)
	    {
	      new(newVector + i) T(vector[i]);  // copy each orginal element
	      vector[i].~T();  // then destroy original.
	    }
	  pv.freeMemory();
	  pv.initSteal(tmp);
	  vector = newVector;
	}
      pv.setLength(newLength);
      new(vector + oldLength) T(item);  // use copy constructor to copy item into vector
    }
}

template<class T>
inline Vector<T>&
Vector<T>::operator=(const Vector<T>& original)
{
  //
  //	destroy destination vector
  //
  size_type length = pv.getLength();
  T* vector = static_cast<T*>(pv.getBase());
  for (size_type i = 0; i < length; i++)
    vector[i].~T();  // call destructor on each object in destination array
  //
  //	reallocate memory if necessary
  //
  size_type originalLength = original.pv.getLength();
  size_t neededBytes = originalLength * sizeof(T);
  if (pv.getAllocatedBytes() < neededBytes)
    {
      if (vector != 0)
	pv.freeMemory();
      pv.initAllocate(neededBytes);
      vector = static_cast<T*>(pv.getBase());
    }
  //
  //	make copy of source vector
  //
  const T* originalVector = static_cast<const T*>(original.pv.getBase());
  if (vector != 0)
    {
      pv.setLength(originalLength);
      for (size_type i = 0; i < originalLength; i++) 
	new(vector + i) T(originalVector[i]);  // call copy constructor on used elements only
    }
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
  return pv.getBase() == 0;
}

template<class T>
inline bool
Vector<T>::empty() const
{
  return pv.empty();
}

//
//	Legacy stuff.
//
template<class T>
inline int
Vector<T>::length() const
{
  return pv.getLength();
}

#endif
