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
//	Class for integer Bin->Dec conversion by multiply and add.
//
#ifndef _decBuf_hh_
#define _decBuf_hh_
#include <deque>

class DecBuf
{
public:
  void zero();
  void insertDigit(int d);
  int nrDigits();
  int nrTrailingZeros();
  void shiftAndAdd(int shift, Uint64 c);
  void ascii(char* s, int length);
  void round(int roundTo, bool fracGtZero, int fracCmpHalf);

private:
  void increment(int startDigit);

  deque<char> buf;
};

inline void
DecBuf::zero()
{
  buf.clear();
}

inline void
DecBuf::insertDigit(int d)
{
  Assert(d >= 0 && d <= 9, "bad digit");
  buf.push_front(d);
}

inline int
DecBuf::nrDigits()
{
  return buf.size();
}

#endif
