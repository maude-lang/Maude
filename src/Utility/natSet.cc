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
//      Implementation for class NatSet.
//
#include "macros.hh"
#include "vector.hh"
#include "natSet.hh"

int
NatSet::topBit(Word w)
{
  Assert(w != 0, "zero word");
  int i = WORD_SIZE - 1;
  for (Word mask = (1 << i); (mask & w) == 0; mask >>= 1)
    --i;
  return i;
}

int
NatSet::bottomBit(Word w)
{
  Assert(w != 0, "zero word");
  int i = 0;
  for (Word mask = 1; (mask & w) == 0; mask <<= 1)
    ++i;
  return i;
}

int
NatSet::countBits(Word w)
{
  static const char bitCount[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
  int count = 0;
  for (; w != 0; w >>= 4)
    count += bitCount[w & 15];
  return count;
}

local_inline NatSet::Word
NatSet::getMask(value_type i)
{
  Assert(i >= 0, "-ve argument");
  return 1 << (i & (WORD_SIZE - 1));
}

local_inline int
NatSet::getWordNr(value_type i)
{
  Assert(i >= 0, "-ve argument");
  return i >> LOG_WORD_SIZE;
}

NatSet::value_type
NatSet::arrayMin(int i) const
{
  int len = array.length();
  for (; i < len; i++)
    {
      if (array[i] != 0)
	return (i + 1) * WORD_SIZE + bottomBit(array[i]);
    }
  return -1;
}

bool
NatSet::arrayContains(value_type i) const
{
  int w = getWordNr(i);
  Assert(i > 0, "bad value");
  if (w > array.length())
    return false;
  return array[w - 1] & getMask(i);
}

void
NatSet::insert(value_type i)
{
  Assert(i >= 0, "-ve argument");
  if (i < WORD_SIZE)
    firstWord |= (1 << i);
  else
    {
      int w = getWordNr(i);
      Word mask = getMask(i);
      int len = array.length();
      if (w > len)
	{
	  array.expandTo(w);
	  --w;
	  for (int j = len; j < w; j++)
	    array[j] = 0;
	  array[w] = mask;
	}
      else
	array[w - 1] |= mask;
    }
}

void
NatSet::insert(const NatSet& other)
{
  firstWord |= other.firstWord;
  int len = array.length();
  int len2 = other.array.length();
  if (len2 > len)
    {
      array.expandTo(len2);
      for (int i = len; i < len2; i++)
	array[i] = other.array[i];
      len2 = len;
    }
  for (int i = 0; i < len2; i++)
    array[i] |= other.array[i];
}

void
NatSet::subtract(value_type i)
{
  Assert(i >= 0, "-ve argument");
  if (i < WORD_SIZE)
    firstWord &= ~(1 << i);
  else
    {
      int w = getWordNr(i);
      int t = array.length() - w;
      if (t >= 0)
	{
	  array[w - 1] &= ~(getMask(i));
	  if (t == 0)
	    {
	      while (w > 0 && array[w - 1] == 0)
		--w;
	      array.contractTo(w);
	    }
	}
    }
}

void
NatSet::subtract(const NatSet& other)
{
  firstWord &= ~(other.firstWord);
  int i = other.array.length();
  int len = array.length();
  if (len <= i)
    {
      for (i = len - 1; i >= 0; i--)
	{
	  Word t = array[i] & ~(other.array[i]);
	  array[i] = t;
	  if (t != 0)
	    break;
	}
      ++i;
      array.contractTo(i);
    }
  for (i--; i >= 0; i--)
    array[i] &= ~(other.array[i]);
}

void
NatSet::intersect(const NatSet& other)
{
  firstWord &= other.firstWord;
  int i = ::min(array.length(), other.array.length()) - 1;
  for (; i >= 0; i--)
    {
      Word t = array[i] & other.array[i];
      array[i] = t;
      if (t != 0)
	break;
    }
  array.contractTo(i + 1);
  for (; i >= 0; i--)
    array[i] &= other.array[i];
}

bool
NatSet::contains(const NatSet& other) const
{
  if ((firstWord | other.firstWord) != firstWord)
    return false;
  int len2 = other.array.length();
  if (len2 > array.length())
    return false;
  for (int i = 0; i < len2; i++)
    {
      Word t = array[i];
      if ((t | other.array[i]) != t)
	return false;
    }
  return true;
}

bool
NatSet::disjoint(const NatSet& other) const
{
  if ((firstWord & other.firstWord) != 0)
    return false;
  int m = ::min(array.length(), other.array.length());
  for (int i = 0; i < m; i++)
    {
      if ((array[i] & other.array[i]) != 0)
	return false;
    }
  return true;
}

NatSet::size_type
NatSet::size() const
{
  int c = countBits(firstWord);
  int len = array.length();
  for (int i = 0; i < len; i++)
    c += countBits(array[i]);
  return c;
}

bool
NatSet::operator==(const NatSet& other) const
{
  if (firstWord != other.firstWord)
    return false;
  int len = array.length();
  if (len != other.array.length())
    return false;
  for (int i = 0; i < len; i++)
    {
      if (array[i] != other.array[i])
	return false;
    }
  return true;
}

bool
NatSet::operator<(const NatSet& other) const
{
  int len = array.length();
  int d = len - other.array.length();
  if (d != 0)
    return d < 0;
  if (firstWord != other.firstWord)
    return firstWord < other.firstWord;
  for (int i = 0; i < len; i++)
    {
      int a = array[i];
      int b = other.array[i];
      if (a != b)
	return a < b;
    }
  return false;
}

//
//	NatSet::iterator
//
NatSet::iterator&
NatSet::iterator::operator++()
{
  Assert(element != -1, "bad increment");
  int w = getWordNr(element);
  Word word = (w == 0) ? natSet->firstWord : natSet->array[w - 1];
  Word mask = getMask(element);
  for (mask <<= 1, element++; mask != 0; mask <<= 1, element++)
    {
      if (word & mask)
	return *this;
    }
  element = natSet->arrayMin(w);
  return *this;
}

ostream&
operator<<(ostream& s, const NatSet& natSet)
{
  s << '{';
  const NatSet::const_iterator b = natSet.begin();
  const NatSet::const_iterator e = natSet.end();
  for (NatSet::const_iterator i = b; i != e; ++i)
    {
      if (i != b)
	s << ", ";
      s << *i;
    }
  s << '}';
  return s;
}
