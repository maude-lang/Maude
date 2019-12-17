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
//	Now a mismomer - contains random stuff that doesn't fit
//	anywhere else.
//
#include <float.h>  // for DBL_MAX
#include "macros.hh"
#include "vector.hh"
#include "mathStuff.hh"
#include "decBuf.hh"
#include "binBuf.hh"
#include "flagSet.hh"

bool globalAdvisoryFlag = true;
bool globalVerboseFlag = false;
//
//	We use this to dump return values that the compiler
//	thinks should not be discarded and would otherwise warn about.
//
int returnValueDump;

const char*
int64ToString(Int64 i, int base)
{
  Assert(base >= 2 && base <= 36, "bad base " << base);
  const int MAX_STRING_SIZE = 64 + 1;  // 64 bits + sign
  const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  static char text[MAX_STRING_SIZE + 1];
  char* p = text + MAX_STRING_SIZE;
  bool negative = false;
  Uint64 u = i;
  if (i < 0)
    {
      u = static_cast<Uint64>(- (i + 1)) + 1;
      negative = true;
    }
  do
    {
      *(--p) = digits[u % base];
      u /= base;
    }
  while (u > 0);
  if (negative)
    *(--p) = '-';
  return p;
}

Int64
stringToInt64(const char* s, bool& error, int base)
{
  Assert(base >= 2 && base <= 36, "bad base " << base);
  error = true;
  bool negative = false;
  switch (*s)
    {
    case '-':
      negative = true;
      // fall thru
    case '+':
      ++s;
    }
  Int64 r = 0;
  for (;;)
    {
      char c = *s++;
      if (c == '\0')
	break;
      int v;
      if (isdigit(c))
	v = c - '0';
      else if (isupper(c))
	v = c - 'A' + 10;
      else if (islower(c))
	v = c - 'a' + 10;
      else
	{
	  error = true;
	  break;
	}
      if (v >= base)
	{
	  error = true;
	  break;
	}
      if (r > (INT64_MAX - v) / base)
	{
	  if (negative && *s == '\0')  // check for INT64_MIN
	    {
	      Int64 t = -(INT64_MIN + base + v);
	      if (t % base == 0 && t / base == r - 1)
		return INT64_MIN;
	    }
	  error = true;
	  break;
	}
      r = r * base + v;
      error = false;
    }
  return negative ? -r : r;
}

bool
looksLikeFloat(const char* s)
{
  bool digits = false;
  bool dot = false;
  char c = *s++;
  if (c == '+' || c == '-')
    c = *s++;
  if (strcmp(s - 1, "Infinity") == 0)
    return true;
  if (isdigit(c))
    {
      digits = true;
      do
	c = *s++;
      while (isdigit(c));
    }
  if (c == '.')
    {
      dot = true;
      c = *s++;
      if (isdigit(c))
	{
	  digits = true;
	  do
	    c = *s++;
	  while (isdigit(c));
	}
    }
  if (!digits)
    return false;
  if (c == 'e' || c == 'E')
    {
      c = *s++;
      if (c == '+' || c == '-')
	c = *s++;
      if (!isdigit(c))
	return false;
      do
	c = *s++;
      while (isdigit(c));
    }
  else
    {
      if (!dot)
	return false;
    }
  return c == '\0';
}

const char*
doubleToString(double d)
{
  if (isnan(d))
    return "NaN";
  if (isinf(d))
    return (d < 0) ? "-Infinity" : "Infinity";
  if (d == 0.0)
    return "0.0";

  static char buffer[DOUBLE_TEXT_SIZE + 1] = "-";
  int decPt;
  int sign;
  //
  //	Get 17 significant digits.
  //
  const int significantDigits = 17;
  correctEcvt(d, significantDigits, buffer + 2, decPt, sign);
  //
  //	Insert decimal point.
  //
  buffer[1] = buffer[2];
  buffer[2] = '.';
  //
  //	Strip trailing zeros.
  //
  int next = 2 + significantDigits;
  while (next > 4 && buffer[next - 1] == '0')
    --next;
  //
  //	 Add exponent if needed.
  //
  int exponent = decPt - 1;
  if (exponent != 0)
    {
      buffer[next++] = 'e';
      if (exponent > 0)
	buffer[next++] = '+';
      strcpy(buffer + next, int64ToString(exponent, 10));
    }
  else
    buffer[next] = '\0';
  return (sign < 0) ? buffer : (buffer + 1);
}

double
stringToDouble(const char* s, bool& error)
{
  if (looksLikeFloat(s))
    {
      error = false;
      //
      //	Some versions of atof() don't handle Infinity.
      //
      char c = s[0];
      if (c == 'I' || ((c == '+' || c == '-') && s[1] == 'I'))
	{
	  double inf = DBL_MAX * DBL_MAX;
	  return (c == '-') ? (- inf) : inf;
	}
      return atof(s);
    }
  else
    {
      error = true;
      return 0;
    }
}

void
correctEcvt(double d, int nrDigits, char buffer[], int& decPt, int& sign)
{
  union Bits
  {
    Uint64 i;
    double d;
  };
  
  const Uint64 bit52 = (1ULL << 52);
  const Uint64 topBit = (1ULL << 63);
  
  //
  //	Break IEEE-754 double into sign/exponent/mantissa.
  //
  Bits bits;
  bits.d = d;
  sign = (bits.i >> 63) ? -1 : 1;  // top bit = 1 -> -ve
  Uint64 mantissa = bits.i & (bit52 - 1);  // 52 LSBs
  int exponent = (static_cast<int>(bits.i >> 52) & 0x7FF);  // remaining 11 bits
  if (exponent == 2047)
    {
      strcpy(buffer, (mantissa == 0) ? "Infinity" : "NaN");
      decPt = 0;
      return;
    }
  if (exponent == 0)
    {
      if (mantissa == 0)  // +/- zero
	{
	  for (int i = 0; i < nrDigits; i++)
	    buffer[i] = '0';
	  buffer[nrDigits] = '\0';
	  decPt = 0;
	  sign = 0;
	  return;
	}
      exponent = -1022;  // denormalized number
    }
  else
    {
      exponent -= 1023;  // unbiased exponent
      mantissa |= bit52;  // implied leading 1
    }
  DecBuf dec;
  BinBuf bin;
  dec.zero();
  mantissa <<= 11;  // now the 53 MSBs
  ++exponent;  // number of bits to the left of the binary point
  if (exponent > 0)
    {
      //
      //	Case 1: Integer part which is converted by shift and add
      //	on a decimal buffer; plus a possible fraction part.
      //	We convert integer part of exponent bits, 60 bits at a time
      //	because 10 * 2^60 < 2^64.
      //
      {
	int shift = min(exponent, 60);
	dec.shiftAndAdd(shift, mantissa >> (64 - shift));
	mantissa <<= shift;
	exponent -= shift;
      }
      while (exponent > 0)
	{
	  int shift = min(exponent, 60);
	  dec.shiftAndAdd(shift, 0);
	  exponent -= shift;
	}
      if (nrDigits <= 0)  // exact conversion
	{
	  decPt = dec.nrDigits();  // can't increase because we don't round
	  if (mantissa == 0)
	    dec.ascii(buffer, decPt - dec.nrTrailingZeros());
	  else
	    {
	      bin.init(mantissa, 0);
	      do
		dec.insertDigit(bin.times10());
	      while (bin.gtZero());
	      dec.ascii(buffer, dec.nrDigits());
	    }
	}
      else  // round to nrDigits using IEEE-754 "round to nearest" convention
	{
	  int extraDigitsNeeded = nrDigits - dec.nrDigits();
	  if (extraDigitsNeeded <= 0)
	    {
	      //
	      //	We have enough digits so round using fractional part
	      //	of mantissa.
	      //
	      int cmpHalf = (mantissa == topBit) ? 0 : ((mantissa < topBit) ? -1 : 1);
	      dec.round(nrDigits, mantissa > 0, cmpHalf);
	      decPt = dec.nrDigits();
	    }
	  else
	    {
	      //
	      //	Make more digits by converting fraction.
	      //
	      bin.init(mantissa, 0);
	      for (int i = 0; i < extraDigitsNeeded; i++)
		dec.insertDigit(bin.times10());
	      //
	      //	Round using part of fraction left in bin.
	      //
	      dec.round(nrDigits, bin.gtZero(), bin.cmpHalf());
	      decPt = dec.nrDigits() - extraDigitsNeeded;
	    }
	  dec.ascii(buffer, nrDigits);
	}
    }
  else
    {
      //
      //	Case 2: No integer part. Fraction is converted by
      //	repeatedly multiplying it by 10.
      //
      bin.init(mantissa, -exponent);
      decPt = 1;
      int digit;
      do
	{
	  digit = bin.times10();
	  --decPt;
	}
      while (digit == 0);
      dec.insertDigit(digit);
      if (nrDigits <= 0)  // exact conversion
	{
	  while (bin.gtZero())
	    dec.insertDigit(bin.times10());
	  dec.ascii(buffer, dec.nrDigits());
	}
      else  // round to nrDigits using IEEE-754 "round to nearest" convention
	{
	  for (int i = 1; i < nrDigits; i++)
	    dec.insertDigit(bin.times10());
	  //
	  //	Round using part of fraction left in bin.
	  //
	  dec.round(nrDigits, bin.gtZero(), bin.cmpHalf());
	  decPt += (dec.nrDigits() - nrDigits);  // rounding may have given us an extra digit
	  dec.ascii(buffer, nrDigits);
	}
    }
}
