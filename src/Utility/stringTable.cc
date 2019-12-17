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
//	Implementation for class StringTable
//
//	We use a hash table to index into the string table, with clashes being
//	resolved by open addressing (with double hashing)
//	See Cormen, Lieserson & Rivest p235
//
#include "macros.hh"
#include "vector.hh"
#include "stringTable.hh"

inline int
StringTable::hash(const char* name)
{
  int h = 0;
  for (const char* p = name; *p; p++)
    h = (3 * h) + *p;	// NOTE Undefined behavior (integer overflow)
  return h;
}

inline int
StringTable::hash2(const char* name)  // 2nd hash function must always return an odd value
{
  int h = 0;
  for (const char* p = name; *p; p++)
    h = (5 * h) ^ *p; // NOTE Undefined behavior (integer overflow)
  return h | 1;
}

StringTable::StringTable(int initialSize)
{
  int s = 2;
  while (s < initialSize)
    s *= 2;
  s *= 2;
  hashTable.expandTo(s);
  for (int i = 0; i < s; i++)
    hashTable[i] = UNUSED;
}

StringTable::~StringTable()
{
  int nrStrings = stringTable.length();
  for (int i = 0; i < nrStrings; i++)
    delete [] stringTable[i];
}

int
StringTable::encode(const char* name)
{
  int mask = hashTable.length() - 1;
  int code;
  int step = 0;
  for (int i = hash(name);; i += step)
    {
      i &= mask;
      code = hashTable[i];
      if (code == UNUSED)
	{
	  code = stringTable.length();
	  stringTable.append(strcpy(new char[strlen(name) + 1], name));
	  if (2 * (code + 1) > hashTable.length())
	    resize();
	  else
	    hashTable[i] = code;
	  break;
	}
      if (strcmp(name, stringTable[code]) == 0)
	break;
      if (step == 0)
	step = hash2(name);
    }
  return code;
}

void
StringTable::resize()
{
  int h = 2 * hashTable.length();
  hashTable.expandTo(h);
  for (int i = 0; i < h; i++)
    hashTable[i] = UNUSED;
  int mask = h - 1;
  int nrStrings = stringTable.length();
  for (int i = 0; i < nrStrings; i++)
    {
      char* s = stringTable[i];
      int j = hash(s) & mask;
      if (hashTable[j] != UNUSED)
	{
	  int step = hash2(s);
	  do
	    j = (j + step) & mask;
	  while (hashTable[j] != UNUSED);
	}
      hashTable[j] = i;
    }
}
