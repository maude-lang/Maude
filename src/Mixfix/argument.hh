/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019 SRI International, Menlo Park, CA 94025, USA.

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
//      Abstract base class for arguments to instantiations.
//
//	This class is needed so that views and parameters can have a common base class.
//	We need a virtual destructor so we can dynamic_cast<>() from Argument* to figure
//	out if we have a view or a parameter.
//
#ifndef _argument_hh_
#define _argument_hh_
#include "namedEntity.hh"

class Argument : public NamedEntity
{
  NO_COPYING(Argument);

public:
  Argument(int argumentName);
  virtual ~Argument() {}
};

inline
Argument::Argument(int argumentName)
  : NamedEntity(argumentName)
{
}

#endif
