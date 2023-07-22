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
//      Implementation for class Indent
//
#include "macros.hh"
#include "tty.hh"

const char* const Tty::ansiCode[] =
{
  "\033[0m",
  "\033[1m",
  "\033[2m",
  "\033[4m",
  "\033[5m",
  "\033[7m",
  "\033[8m",

  "\033[30m",
  "\033[31m",
  "\033[32m",
  "\033[33m",
  "\033[34m",
  "\033[35m",
  "\033[36m",
  "\033[37m",

  "\033[40m",
  "\033[41m",
  "\033[42m",
  "\033[43m",
  "\033[44m",
  "\033[45m",
  "\033[46m",
  "\033[47m"
};

bool Tty::allowedFlag = true;
bool Tty::savedFlag = false;

const char*
Tty::ctrlSequence() const
{
  if (allowedFlag)
    return ansiCode[attr];
  return "";
}
