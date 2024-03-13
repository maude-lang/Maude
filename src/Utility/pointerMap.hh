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
//      Class for maps from pointers to pointers
//
#ifndef _pointerMap_hh_
#define _pointerMap_hh_

class PointerMap
{
  NO_COPYING(PointerMap);

public:
  PointerMap(int size = 8);

  void* setMap(const void* from, void* to);
  void* getMap(const void* from) const;

private:
  struct Pair
  {
    const void* from;
    void* to;
  };

  static int hash(const void* pointer);
  static int hash2(const void* pointer);
  int findEntry(const void* from) const;
  void resize();

  int used;
  Vector<Pair> hashTable;
};

#endif
