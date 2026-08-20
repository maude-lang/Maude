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
//      Implementation for class TermSet
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "term.hh"

//	core class definitions
#include "termSet.hh"


#ifdef USE_POINTER_SET

int
TermSet::insert(Term* t)
{
  return PointerSet::insert(t, t->getHashValue());
}

int
TermSet::term2Index(Term* t) const
{
  return pointer2Index(t, t->getHashValue());
}

unsigned int
TermSet::hash(void* pointer ) const
{
  CantHappen("should never be called");
  return 0;
}

bool
TermSet::isEqual(void* pointer1, void* pointer2) const
{
  Term* t1 = static_cast<Term*>(pointer1);
  Term* t2 = static_cast<Term*>(pointer2);
  return t1->hasEagerContext() == t2->hasEagerContext() && t1->equal(t2);
}

#else

std::size_t
TermSet::TermHash::operator()(const TermPtr& t) const
{
  return t->getHashValue();
}

bool
TermSet::TermEqual::operator()(const TermPtr& t1, const TermPtr& t2) const
{
  return t1->hasEagerContext() == t2->hasEagerContext() && t1->equal(t2);
}

Index
TermSet::insert(Term* t)
{
  Index index = termMap.size();
  auto p = termMap.insert(TermMap::value_type(t, index));
  return p.first->second;
}

Index
TermSet::term2Index(Term* t) const
{
  auto i = termMap.find(t);
  return i == termMap.end() ? NONE : i->second;
}

#endif
