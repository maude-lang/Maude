/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2017 SRI International, Menlo Park, CA 94025, USA.

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
//	Template for stacks.
//
#ifndef _stack_hh_
#define _stack_hh_
#include "vector.hh"

template<class T>
class Stack
{
public:
  bool empty() const;
  const T& top() const;
  void pop();
  void push(const T& item);

private:
  Vector<T> items;
};

template<class T>
inline bool
Stack<T>::empty() const
{
  return items.empty();
}

template<class T>
inline const T&
Stack<T>::top() const
{
  return items.back();
}

template<class T>
inline void
Stack<T>::pop()
{
  items.pop_back();
}

template<class T>
inline void
Stack<T>::push(const T& item)
{
  items.push_back(item);
}

#endif
