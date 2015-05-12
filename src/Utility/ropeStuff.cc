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
//	Rope utilities.
//
#include "macros.hh"
#include "ropeStuff.hh"

char* makeZeroTerminatedString(const crope& original)
{
  //
  //	We don't trust rope::c_str() from the rope library. We create
  //	zero terminated array that is alive after the rope goes away. It
  //	is the callers responsibility to delete it.
  //
  int length = original.size();
  char* zeroTerminatedString = new char[length + 1];
  original.copy(zeroTerminatedString);
  zeroTerminatedString[length] = 0;
  //
  //	Check for the problem we are trying to code around.
  //
  DebugAdvisoryCheck(strcmp(zeroTerminatedString, original.c_str()) == 0,
		     "makeZeroTerminatedString() : c_str() disagreement " <<
		     zeroTerminatedString << " of length " << strlen(zeroTerminatedString) <<
		     " vs " << original.c_str() << " of length " << strlen(original.c_str()));
  return zeroTerminatedString;
}
