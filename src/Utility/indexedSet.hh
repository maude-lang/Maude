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
//	Class template for (constructive) indexed sets.
//
#ifndef _indexedSet_hh_
#define _indexedSet_hh_
#include <map>
#include <vector>

template<class T>
class IndexedSet
{
public:
  //
  //	The pupose of FastPair is to allow an insert() which avoids copying
  //	the inserted element if it is already in the indexed set.
  //	We assume T is a complex data type for which copies, compares
  //	and destructs are relatively expensive. A less ugly method would
  //	be to do a find() before inserting but this would cost a second
  //	round of compares if we actually had to do the insert().
  //
  typedef pair<T, int> FastPair;

  void clear();
  int insert(const T& element);
  int insert(const FastPair& fastPair);
  int find(const T& element) const;
  const T& ithElement(int i) const;
  int size() const;
  void swap(IndexedSet& other);

private:
  typedef map<T, int> EltMap;
  typedef vector<typename EltMap::const_iterator> IndexMap;

  EltMap eltMap;
  IndexMap indexMap;
};

template<class T>
inline void
IndexedSet<T>::clear()
{
  indexMap.clear();
  eltMap.clear();
}

template<class T>
inline void
IndexedSet<T>::swap(IndexedSet& other)
{
  indexMap.swap(other.indexMap);
  eltMap.swap(other.eltMap);
}

template<class T>
inline int
IndexedSet<T>::insert(const T& element)
{
  pair<typename EltMap::iterator, bool> p =
    eltMap.insert(typename EltMap::value_type(element, indexMap.size()));
  if (p.second)
    indexMap.push_back(p.first);
  return p.first->second;
}

template<class T>
inline int
IndexedSet<T>::insert(const FastPair& fastPair)
{
  pair<typename EltMap::iterator, bool> p = eltMap.insert(fastPair);
  if (p.second)
    {
      p.first->second = indexMap.size();
      indexMap.push_back(p.first);
    }
  return p.first->second;
}

template<class T>
inline int
IndexedSet<T>::find(const T& element) const
{
  typename EltMap::const_iterator i = eltMap.find(element);
  return (i == eltMap.end()) ? -1 : i->second;
}

template<class T>
inline const T&
IndexedSet<T>::ithElement(int i) const
{
  return indexMap[i]->first;
}

template<class T>
inline int
IndexedSet<T>::size() const
{
  return indexMap.size();
}

#endif
