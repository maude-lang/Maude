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
//	List of ANSI Escape sequences - used to generate branching
//	code in various places. Format is
//	  MACRO(Maude name, class Tty name)
//	We don't include reset because it is special.
//
  MACRO('!', BRIGHT)
  MACRO('?', DIM)
  MACRO('u', UNDERLINE)
  MACRO('f', BLINK)
  MACRO('x', REVERSE)
  MACRO('h', HIDDEN)

  MACRO('p', BLACK)  // pitch black - b is for blue
  MACRO('r', RED)
  MACRO('g', GREEN)
  MACRO('y', YELLOW)
  MACRO('b', BLUE)
  MACRO('m', MAGENTA)
  MACRO('c', CYAN)
  MACRO('w', WHITE)

  MACRO('P', BKGD_BLACK)
  MACRO('R', BKGD_RED)
  MACRO('G', BKGD_GREEN)
  MACRO('Y', BKGD_YELLOW)
  MACRO('B', BKGD_BLUE)
  MACRO('M', BKGD_MAGENTA)
  MACRO('C', BKGD_CYAN)
  MACRO('W', BKGD_WHITE)
