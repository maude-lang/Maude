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
//	Class for slightly enhanced BDDs.
//
//	Most importantly Bdds are totally ordered by operator< and
//	can therefore be used a keys in set<>s and map<>s. 
//
#ifndef _bdd_hh_
#define _bdd_hh_
#include "bdd.h"

class Bdd : public bdd
{
public:
  Bdd();
  Bdd(const bdd& other);

  const Bdd& operator=(const bdd& other);
  bool operator<(const Bdd& other) const;
  bool implies(const Bdd& other) const;
  Bdd extractPrimeImplicant() const;
};

inline
Bdd::Bdd()
{
}

inline
Bdd::Bdd(const bdd& other)
  : bdd(other)
{
} 

inline const Bdd&
Bdd::operator=(const bdd& other)
{
  bdd::operator=(other);
  return *this;
}

inline bool
Bdd::operator<(const Bdd& other) const
{
  return id() < other.id();
}

inline bool
Bdd::implies(const Bdd& other) const
{
  return bdd_imp(*this, other) == bdd_true();
}

#endif
