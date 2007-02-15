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
#include "variable.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      core class definitions
#include "substitution.hh"
#include "localBinding.hh"

//	variable class definitions
#include "variableDagNode.hh"

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
	     "substitution inconsistency at index " << i - b);
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

bool
Substitution::unificationBind(int index, Sort* varSort, DagNode* value)
{
  Assert(values[index] == 0, "trying to bind bound variable " << index << " to " << value);
  //
  //	First we instantiate the value with the current substitution.
  //
  DagNode* n = value->instantiate(*this);
  if (n == 0)
    n = value;
  //
  //	Check to see if we are binding to a ground or non-ground term.
  //
  int sortIndex = n->getSortIndex();
  if (sortIndex == Sort::SORT_UNKNOWN)
    {
      //
      //	Check to see if we are binding a variable to an expression
      //	containing the variable.
      //
      if (n->occurs(index))
	{
	  //
	  //	Check to see if we are binding a variable to itself.
	  //
	  if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(n))
	    {
	      if (v->getIndex() == index)
		return true;
	    }
	  return false;
	}
    }
  else
    {
      //
      //	Check the sort of the ground term we are binding to.
      //
      if (varSort != 0 && !leq(sortIndex, varSort))
	return false;
    }
  //
  //	Finally we can bind the variable.
  //
  bind(index, n);
  //
  //	Now we eliminate the variable from existing bindings.
  //
  for (int i = 0; i < copySize; ++i)
    {
      if (i != index)
	{
	  DagNode* v = values[i];
	  if (v != 0)
	    {
	      v = v->instantiate(*this);
	      if (v != 0)
		values[i] = v;
	    }
	}
    }
  return true;
}

LocalBinding*
Substitution::unificationDifference(const Substitution& original) const
{
  //
  //	We allow the original and ourselves to have differing bindings for the
  //	same variable as bindings can change under instantiation.
  //	We are only interested in the variables for which we have a binding and the
  //	original substitution does not. The converse case represents an inconsistancy.
  //
  int nrDiff = 0;
  Vector<DagNode*>::const_iterator b = values.begin();
  Vector<DagNode*>::const_iterator e = b + copySize;

  Vector<DagNode*>::const_iterator j = original.values.begin();
  for (Vector<DagNode*>::const_iterator i = b; i != e; ++i, ++j)
    {
      Assert(*j == 0 || *i != 0,
	     "substitution inconsistency at index " << i - b);
      if (*i != 0 && *j == 0)
	++nrDiff;
    }

  if (nrDiff == 0)
    return 0;
  LocalBinding* result = new LocalBinding(nrDiff);

  j = original.values.begin();
  for (Vector<DagNode*>::const_iterator i = b; i != e; ++i, ++j)
    {
      DagNode* d = *i;
      if (d != 0 && *j == 0)
	result->addBinding(i - b, d);
    }
  return result;
}
