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
//	Class for C++ variable names composed of a one character base and a short index.
//
#ifndef _variableName_hh_
#define _variableName_hh_

class VariableName
{
public:
  VariableName();
  VariableName(char base, short index);

private:
  int combined;
  friend ostream& operator<<(ostream& s, VariableName v);
};

inline
VariableName::VariableName()
{
}

inline
VariableName::VariableName(char base, short index)
{
  combined = (base << 16) | index;
}

inline ostream&
operator<<(ostream& s, VariableName v)
{
  s << static_cast<char>(v.combined >> 16) << (v.combined & 0xffff);
  return s;
}

#endif
