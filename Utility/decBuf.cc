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
//	Implementation for class DecBuf.
//
#include "macros.hh"
#include "decBuf.hh"

void
DecBuf::shiftAndAdd(int shift, Uint64 c)
{
  for (deque<char>::iterator i = buf.begin(); i != buf.end(); ++i)
    {
      Uint64 r = c + (static_cast<Uint64>(*i) << shift);
      c = r / 10;
      *i = r - 10 * c;
    }
  while (c > 0)
    {
      Uint64 r = c;
      c = r / 10;
      buf.push_back(r - 10 * c);
    }
}

void
DecBuf::ascii(char* s, int length)
{
  Assert(length <= nrDigits(), "bad length" << length);
  for (deque<char>::reverse_iterator i = buf.rbegin(); length > 0; ++i, --length)
    *s++ = *i + '0';
  *s = '\0';
}

int
DecBuf::nrTrailingZeros()
{
  int nrZeros = 0;
  for (deque<char>::iterator i = buf.begin(); i != buf.end(); ++i)
    {
      if (*i != 0)
	break;
      ++nrZeros;
    }
  return nrZeros;
}

void
DecBuf::increment(int startDigit)
{
  for (deque<char>::iterator i = buf.begin() + startDigit; i != buf.end(); ++i)
    {
      int r = *i + 1;
      if (r < 10)
	{
	  *i = r;
	  return;
	}
      *i = 0;
    }
  buf.push_back(1);
}

void
DecBuf::round(int roundTo, bool fracGtZero, int fracCmpHalf)
{
  int roundDigit = nrDigits() - roundTo;
  //
  //	We need to round up if remaining part  of the number (including the fraction)
  //	is either > 0.5; or = 0.5 and the last digit (roundDigit) of the piece we
  //	want is odd.
  //
  bool roundUp;
  if (roundDigit == 0)
    roundUp = (fracCmpHalf > 0 || (fracCmpHalf == 0 && (buf[0] & 1)));
  else
    {
      int critical = buf[roundDigit - 1];
      if (critical < 5)
	roundUp = false;
      else if (critical > 5 || fracGtZero || (buf[roundDigit] & 1))
	roundUp = true;
      else
	{
	  roundUp = false;
	  for (int i = 0; i < roundDigit - 1; i++)
	    {
	      if (buf[i] != 0)
		{
		  roundUp = true;
		  break;
		}
	    }
	}
    }
  if (roundUp)
    increment(roundDigit);
}
