/*

    This file is part of the Maude 3 interpreter.

    Copyright 2007 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class AllSat.
//
#include "macros.hh"
#include "vector.hh"
#include "allSat.hh"

AllSat::AllSat(Bdd formula, int firstVariable, int lastVariable)
  : formula(formula),
    firstVariable(firstVariable),
    lastVariable(lastVariable),
    nodeStack(0, lastVariable - firstVariable + 1),
    dontCareSet(0, lastVariable - firstVariable + 1)
{
  firstAssignment = true;
}

bool
AllSat::nextAssignment()
{
  if (firstAssignment)
    {
      //
      //	First solution.
      //
      if (formula == bddfalse)
	return false;
      assignment.resize(lastVariable + 1);
      for (int i = firstVariable; i <= lastVariable; ++i)
	assignment[i] = UNDEFINED;
      forward(formula);
      firstAssignment = false;
      return true;
    }
  //
  //	Try to find another way of assigning to don't care variables.
  //
  int nrDontCares = dontCareSet.size();
  for (int i = nrDontCares - 1; i >= 0; --i)
    {
      int var = dontCareSet[i];
      if (assignment[var] == 0)
	{
	  assignment[var] = 1;
	  for (++i; i < nrDontCares; ++i)
	    assignment[dontCareSet[i]] = 0;
	  return true;
	}
    }
  for (int i = 0; i < nrDontCares; ++i)
    assignment[dontCareSet[i]] = UNDEFINED;
  dontCareSet.clear();
  //
  //	Try to find another route to true through BDD.
  //
  int nodeDepth = nodeStack.size();
  for (int i = nodeDepth - 1; i >= 0; --i)
    {
      Bdd b = nodeStack[i];
      int var = bdd_var(b);
      if (assignment[var] == 0)
	{
	  Bdd n = bdd_high(b);
	  if (n != bddfalse)
	    {
	      assignment[var] = 1;
	      nodeStack.resize(i + 1);
	      forward(n);
	      return true;
	    }
	}
      assignment[var] = UNDEFINED;
    }
  return false;
}

void
AllSat::forward(Bdd b)
{
  Assert(b != bddfalse, "false BDD");
  //
  //	There must be at least one path to true from b; find the least one.
  //
  while (b != bddtrue)
    {
      nodeStack.append(b);
      int var = bdd_var(b);
      Bdd n = bdd_low(b);
      if (n == bddfalse)
	{
	  n = bdd_high(b);
	  assignment[var] = 1;
	}
      else
	assignment[var] = 0;
      b = n;
    }
  //
  //	Any variables of interest not assigned to on this path are don't cares.
  //
  for (int i = firstVariable; i <= lastVariable; ++i)
    {
      if (assignment[i] == UNDEFINED)
	{
	  assignment[i] = 0;
	  dontCareSet.append(i);
	}
    }
}
