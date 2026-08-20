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
//      Class for set of pointers to terms.
//
#ifndef _termSet_hh_
#define _termSet_hh_
#define USE_POINTER_SET

#ifdef USE_POINTER_SET

#include "pointerSet.hh"

class TermSet : private PointerSet
{
public:
  int insert(Term* t);
  int term2Index(Term* t) const;  // returns NULL if t not in set or index otherwise
  using PointerSet::cardinality;
  using PointerSet::makeEmpty;

private:
  unsigned int hash(void* pointer) const;
  bool isEqual(void* pointer1, void* pointer2) const;
};

#else

#include <unordered_map>

class TermSet
{
public:
  Index insert(Term* t);
  Index term2Index(Term* t) const;  // returns NULL if t not in set or index otherwise
  Index cardinality() const;
  void makeEmpty();

private:
  typedef Term* TermPtr;

  struct TermHash
  {
    std::size_t operator()(const TermPtr& t) const;
  };
  
  struct TermEqual
  {
    bool operator()(const TermPtr& t1, const TermPtr& t2) const;
  };
  
  typedef unordered_map<TermPtr, Index, TermHash, TermEqual> TermMap;

  TermMap termMap;
};

inline Index
TermSet::cardinality() const
{
  return termMap.size();
}
inline void
TermSet::makeEmpty()
{
  termMap.clear();
}

#endif

#endif
