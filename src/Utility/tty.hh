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
//	Class to abstract tty attributes.
//
#ifndef _tty_hh_
#define _tty_hh_

class Tty
{
public:
  enum Attribute
  {
    RESET,
    BRIGHT,
    DIM,
    UNDERLINE,
    BLINK,
    REVERSE,
    HIDDEN,
    
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    
    BKGD_BLACK,
    BKGD_RED,
    BKGD_GREEN,
    BKGD_YELLOW,
    BKGD_BLUE,
    BKGD_MAGENTA,
    BKGD_CYAN,
    BKGD_WHITE
  };

  Tty(Attribute attr);

  static void setEscapeSequencesAllowed(bool flag);

  const char* ctrlSequence() const;

private:
  static const char* const ansiCode[];
  static bool allowedFlag;

  const Attribute attr;
};

inline
Tty::Tty(Attribute attr)
  : attr(attr)
{
}

inline void
Tty::setEscapeSequencesAllowed(bool flag)
{
  allowedFlag = flag;
}

inline
ostream& operator<<(ostream& s, const Tty& t)
{
  s << t.ctrlSequence();
  return s;
}

#endif
