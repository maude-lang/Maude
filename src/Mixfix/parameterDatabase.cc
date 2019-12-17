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
//      Implementation for class ParameterDatabase.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "mixfix.hh"

//	front end class definitions
#include "parameterDatabase.hh"

ParameterDatabase::~ParameterDatabase()
{
  //
  //	Cleaning up makes for a slow exit, but it helps catch
  //	dangling pointers and leaked memory when debugging.
  //	It's also important now that we have meta-interpreters.
  //
  const ParameterMap::const_iterator e = parameterMap.end();
  for (ParameterMap::const_iterator i = parameterMap.begin(); i != e; ++i)
    delete i->second;
}

Parameter* 
ParameterDatabase::getParameter(int parameterName)
{
  ParameterMap::const_iterator i = parameterMap.find(parameterName);
  if (i == parameterMap.end())
    {
      Parameter* p = new Parameter(parameterName);
      parameterMap.insert(ParameterMap::value_type(parameterName, p));
      return p;
    }
  return i->second;
}
