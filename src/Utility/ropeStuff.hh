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
//	Put the rope inclusion mess in a single place.
//
#ifndef _ropeStuff_hh_
#define _ropeStuff_hh_
#ifdef __GNUC__
  #if __GNUC__ < 3
    #include <rope.h>
  #else
//
//	In version 3.* of the GNU Standard C++ library, the rope<>
//	class template in considered an extension and lives in a
//	different header file.
//
    #ifdef USE_ROPE_UGLY_HACK
//
//	Mac rope implementation is quite broken and in particular violates C++ scoping rules
//	enforced by newer compilers. Therefore we use a local hacked up version of the Mac
//	rope implementation.
//
      #include "rope_UGLY_HACK"
    #else
      #include <ext/rope>
    #endif
    #if __GNUC_MINOR__ > 0 || __GNUC__ > 3
//
//	In version 3.1 onwards it also lives in a different namespace.
//
      using namespace __gnu_cxx;
    #endif
  #endif
#else
//
//	If we're not using g++, this is the header file mentioned in
//	the SGI STL documentation.
//
#include <rope>
#endif

char* makeZeroTerminatedString(const crope& original);

#endif

