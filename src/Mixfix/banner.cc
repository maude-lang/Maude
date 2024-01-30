/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Banner gets recompiled every build to update time; so keep it small.
//
#if HAVE_CONFIG_H
#include <config.h>
#endif
#if HAVE_OSTREAM
#include <ostream>
#elif HAVE_OSTREAM_H
#include <ostream.h>
#endif

using namespace std;

#include "timeStuff.hh"
#include "tty.hh"

void
printBanner(std::ostream& s, const char* date, const char* time, time_t seconds)
{
  s << "\t\t     \\||||||||||||||||||/\n";
  s << "\t\t   --- Welcome to " <<
    Tty(Tty::RED) << 'M' <<
    Tty(Tty::CYAN) << 'a' <<
    Tty(Tty::BLUE) << 'u' <<
    Tty(Tty::MAGENTA) << 'd' <<
    Tty(Tty::GREEN) << 'e' <<
    Tty(Tty::RESET) << " ---\n";
  s << "\t\t     /||||||||||||||||||\\\n";
  s << "\t    " << PACKAGE_STRING << " built: " << date << ' ' << time << '\n';
  s << "\t     Copyright 1997-2024 SRI International\n";
  s << "\t\t   " << ctime(&seconds);
}
