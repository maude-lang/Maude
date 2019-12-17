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
//      Class for line numbers.
//
#ifndef _lineNumber_hh_
#define _lineNumber_hh_

class LineNumber
{
public:
  LineNumber(int lineNumber = UNDEFINED);
  int getLineNumber() const;
  void setLineNumber(int lineNr);

private:
  int lineNumber;
};

inline
LineNumber::LineNumber(int lineNumber)
  : lineNumber(lineNumber)
{
}

inline int
LineNumber::getLineNumber() const
{
  return lineNumber;
}

inline void
LineNumber::setLineNumber(int lineNr)
{
  lineNumber = lineNr;
}

//
//	Output function for LineNumber must be defined by library user.
//
ostream& operator<<(ostream& s, const LineNumber& lineNumber);

#endif
