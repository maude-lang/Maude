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
//	Class for occurrence of subterm in/under free skeleton.
//
#ifndef _freeOccurrence_hh_
#define _freeOccurrence_hh_

class FreeOccurrence
{
public:
  FreeOccurrence() {}  // needed for Vector<FreeOccurrence>
  FreeOccurrence(int position, int argIndex, Term* term);

  int position() const;
  int argIndex() const;
  Term* term() const;

private:
  int pos;
  int arg;
  Term* trm;
};

inline
FreeOccurrence::FreeOccurrence(int position, int argIndex, Term* term)
{
  pos = position;
  arg = argIndex;
  trm = term;
}

inline int
FreeOccurrence::position() const
{
  return pos;
}

inline int
FreeOccurrence::argIndex() const
{
  return arg;
}

inline Term*
FreeOccurrence::term() const
{
  return trm;
}

#endif
