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
//	Class for argument iterator for AU deque dag nodes.
//
#ifndef _AU_DequeDagArgumentIterator_hh_
#define _AU_DequeDagArgumentIterator_hh_
#include "rawDagArgumentIterator.hh"
#include "AU_DequeIter.hh"

class AU_DequeDagArgumentIterator : public RawDagArgumentIterator
{
  NO_COPYING(AU_DequeDagArgumentIterator);

public:
  AU_DequeDagArgumentIterator(const AU_Deque& deque);
 
  bool valid() const;
  DagNode* argument() const;
  void next();
 
private:
  AU_DequeIter iter;
};

inline
AU_DequeDagArgumentIterator::AU_DequeDagArgumentIterator(const AU_Deque& deque)
  : iter(deque)
{
}
 
#endif
