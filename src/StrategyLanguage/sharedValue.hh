/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2006 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for a simplified RewriteSearchState, together with reference counting smart pointers to
//	determine when it is deleted, when it gets shared between multiple users.
//
#ifndef _sharedValue_hh_
#define _sharedValue_hh_

template <typename T>
class SharedValue
{
public:
  SharedValue(T* value);
  SharedValue(const SharedValue& other);
  ~SharedValue();
  T* operator->();

private:
  T* value;
  int* refCount;
};

template <typename T>
inline T*
SharedValue<T>::operator->()
{
  return value;
}

template <typename T>
SharedValue<T>::SharedValue(T* value)
  : value(value),
    refCount(new int(1))
{
}

template <typename T>
SharedValue<T>::SharedValue(const SharedValue& other)
{
  value = other.value;
  refCount = other.refCount;
  ++(*refCount);
}

template <typename T>
SharedValue<T>::~SharedValue()
{
  if (--(*refCount) == 0)
    {
      delete value;
      delete refCount;
    }
}

#endif
