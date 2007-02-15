/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2006 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class SharedTokens.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//	front end class definitions
#include "token.hh"
#include "sharedTokens.hh"

#define MACRO(Name, String) \
   int SharedTokens::Name;
#include "specialTokens.cc"
#undef MACRO

SharedTokens::SharedTokens()
{
  if (leftParen == 0)
    {
#define MACRO(Name, String) \
      Name = Token::encode(String);
#include "specialTokens.cc"
#undef MACRO
    }
}
