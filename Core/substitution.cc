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
//	Implementation for class Substitution
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      core class definitions
#include "substitution.hh"
#include "localBinding.hh"

int Substitution::allocateSize = 1;

LocalBinding*
Substitution::operator-(const Substitution& original) const
{
  int nrDiff = 0;
  Vector<DagNode*>::const_iterator b = values.begin();
  Vector<DagNode*>::const_iterator e = b + copySize;

  Vector<DagNode*>::const_iterator j = original.values.begin();
  for (Vector<DagNode*>::const_iterator i = b; i != e; ++i, ++j)
    {
      Assert(*j == 0 || *i == *j,
	     "substitution inconsistancy at index " << i - b);
      if (*i != *j)
	++nrDiff;
    }

  if (nrDiff == 0)
    return 0;
  LocalBinding* result = new LocalBinding(nrDiff);

  j = original.values.begin();
  for (Vector<DagNode*>::const_iterator i = b; i != e; ++i, ++j)
    {
      DagNode* d = *i;
      if (d != *j)
	result->addBinding(i - b, d);
    }
  return result;
}
