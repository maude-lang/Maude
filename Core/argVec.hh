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
//	Class for garbage collected argument vectors.
//
//	For efficiency we assume that the parameter class T is very simple:
//	(1) no constructor;
//	(2) no destructor;
//	(3) can be copied efficiently and correctly by assignment operator.
//
//	Resizing ArgVecs is expensive so correct length should be supplied
//	to constructor (there is no default constructor).
//
//	Typically T will be a DagNode* or a small struct containing a DagNode*
//	and other simple data types (ints, ptrs).
//
//	Garbage collection sweep phase invalidates all ArgVec objects that
//	have not had their evacuate() member function called during the mark phase.
//	Calling evacuate() invalidates all pointers/references/interators
//	into the ArgVec.
//
#ifndef _argVec_hh_
#define _argVec_hh_

template<class T>
class ArgVec
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

  ArgVec(size_type length);

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  const_reference operator[](size_type i) const;	// access for reading
  reference operator[](size_type i);			// access for writing
  void expandBy(size_type extra);
  void resizeWithoutPreservation(size_type newSize);
  void contractTo(size_type length);
  size_type size() const;
  int length() const;
  void swap(ArgVec& other);
  //
  //	Move contents of ArgVec to a safe place during mark phase.
  //	Invalidates all pointers/references/interators into ArgVec.
  //
  void evacuate();

private:
  size_type len;		// number of objects in ArgVec
  size_t allocatedBytes;	// space allocated in bytes
  pointer basePtr;
};

template<class T>
inline
ArgVec<T>::ArgVec(size_type length)
{
  Assert(length >= 0, cerr << "-ve length");
  len = length;
  allocatedBytes = length * sizeof(T);
  basePtr = static_cast<pointer>(MemoryCell::allocateStorage(allocatedBytes));
}

#ifdef NO_ASSERT
//
//	Fast, with no runtime checking.
//
template<class T>
inline typename ArgVec<T>::iterator
ArgVec<T>::begin()
{
  return basePtr;
}

template<class T>
inline typename ArgVec<T>::iterator
ArgVec<T>::end()
{
  return basePtr + len;
}

template<class T>
inline typename ArgVec<T>::const_iterator
ArgVec<T>::begin() const
{
  return basePtr;
}

template<class T>
inline typename ArgVec<T>::const_iterator
ArgVec<T>::end() const
{
  return basePtr + len;
}

#else
//
//	Slow, with extensive runtime checking.
//
#include <checkedArgVecIterator.hh>

template<class T>
inline typename ArgVec<T>::iterator
ArgVec<T>::begin()
{
  return iterator(this, 0);
}

template<class T>
inline typename ArgVec<T>::iterator
ArgVec<T>::end()
{
  return iterator(this, len);
}

#include <checkedArgVecConstIterator.hh>

template<class T>
inline typename ArgVec<T>::const_iterator
ArgVec<T>::begin() const
{
  return const_iterator(this, 0);
}

template<class T>
inline typename ArgVec<T>::const_iterator
ArgVec<T>::end() const
{
  return const_iterator(this, len);
}

#endif

template<class T>
inline typename ArgVec<T>::const_reference
ArgVec<T>::operator[](size_type i) const
{
  Assert(i < len, "index too big");
  return basePtr[i];
}

template<class T>
inline typename ArgVec<T>::reference
ArgVec<T>::operator[](size_type i)
{
  Assert(i < len, "index too big");
  return basePtr[i];
}

template<class T>
inline void
ArgVec<T>::expandBy(size_type extra)
{
  size_type oldLen = len;
  len += extra;
  size_t neededBytes = len * sizeof(T);
  if (neededBytes > allocatedBytes)
    {
      pointer oldBasePtr = basePtr;
      basePtr = static_cast<pointer>(MemoryCell::allocateStorage(neededBytes));
      for (pointer n = basePtr; oldLen > 0; oldLen--)
	*n++ = *oldBasePtr++;
      allocatedBytes = neededBytes;
    }
}

template<class T>
inline void
ArgVec<T>::resizeWithoutPreservation(size_type newSize)
{
  //
  //	Fast but original contents of ArgVec are lost.
  //
  len = newSize;
  size_t neededBytes = newSize * sizeof(T);
  if (neededBytes > allocatedBytes)
    {
      basePtr = static_cast<pointer>(MemoryCell::allocateStorage(neededBytes));
      allocatedBytes = neededBytes;
    }
}

template<class T>
inline void
ArgVec<T>::evacuate()
{
  size_type l = len;
  allocatedBytes = l * sizeof(T);
  pointer v = basePtr;
  basePtr = static_cast<pointer>(MemoryCell::allocateStorage(allocatedBytes));
  for (pointer n = basePtr; l > 0; l--)
    *n++ = *v++;
}

template<class T>
inline void
ArgVec<T>::contractTo(size_type length)
{
  Assert(length <= len, "new length > old length");
  len = length;
}

template<class T>
inline typename ArgVec<T>::size_type
ArgVec<T>::size() const
{
  return len;
}

template<class T>
inline int
ArgVec<T>::length() const
{
  return len;
}

template<class T>
inline void
ArgVec<T>::swap(ArgVec& other)
{
  int t = len;
  len = other.len;
  other.len = t;
  t = allocatedBytes;
  allocatedBytes = other.allocatedBytes;
  other.allocatedBytes = t;
  pointer p = basePtr;
  basePtr = other.basePtr;
  other.basePtr = p;
}

#endif
