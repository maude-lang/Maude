/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	Wrapper around raw argument iterators to provide cleaner interface
//
#ifndef _argumentIterator_hh_
#define _argumentIterator_hh_
#include "rawArgumentIterator.hh"

class ArgumentIterator
{
public:
  ArgumentIterator(const Term& t);
  ~ArgumentIterator();
  bool valid() const;
  Term* argument() const;
  void next();

private:
  RawArgumentIterator* argumentIterator;
};

inline
ArgumentIterator::ArgumentIterator(const Term& t)
{
  argumentIterator = t.arguments();
}

inline
ArgumentIterator::~ArgumentIterator()
{
  delete argumentIterator;
}

inline bool
ArgumentIterator::valid() const
{
  return (argumentIterator != 0) && argumentIterator->valid();
}

inline Term*
ArgumentIterator::argument() const
{
  return argumentIterator->argument();
}

inline void
ArgumentIterator::next()
{
  argumentIterator->next();
}

#endif
