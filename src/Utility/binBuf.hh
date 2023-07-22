/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for fractional binary to decimal conversion.
//
#ifndef _binBuf_hh_
#define _binBuf_hh_

class BinBuf
{
public:
  void init(uint_fast64_t bits, int leadingZeros);
  int times10();
  int cmpHalf();
  bool gtZero();

private:
  enum Sizes
  {
    //
    //	A double could have 52 + 1022 = 1074 bits following the binary point so
    //	we need at most ceiling(1074 / 32) = 34 32-bit words to hold the fixed point
    //	binary fraction.
    //
    MAX_WORDS = 34
  };

  uint_fast32_t buf[MAX_WORDS];
  int begin;
  int end;
};

#endif
