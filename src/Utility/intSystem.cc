/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2008 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class IntSystem.
//
#include "macros.hh"
#include "vector.hh"
#include "intSystem.hh"

//	analysis options
#define ANALYZE_GCD 0
//#define TRACE(s) (std::cout << s << endl)
#define TRACE(s)

//	our stuff
#include "intContejeanDevie.cc"


IntSystem::IntSystem(int nrVariables)
  : nrVariables(nrVariables)
{
}

void
IntSystem::insertEqn(const IntVec& eqn)
{
  eqns.push_back(IntVec());
  IntVec& newEqn = eqns.back();
  newEqn.resize(nrVariables);
  int nrEntries = eqn.size();
  //
  //	Copy entries from eqn.
  //
  int i = 0;
  for (; i < nrEntries; ++i)
    newEqn[i] = eqn[i];
  //
  //	Zero pad to full length.
  //
  for (; i < nrVariables; ++i)
    newEqn[i] = 0;
}

void
IntSystem::setUpperBounds(const IntVec& bounds)
{
  upperBounds = bounds;  // deep copy
}

void
IntSystem::initializeUpperBounds()
{
  if (upperBounds.empty())
    {
      upperBounds.resize(nrVariables);
      IntVec::iterator e = upperBounds.end();
      for (IntVec::iterator i = upperBounds.begin(); i != e; ++i)
	*i = UNBOUNDED;
    }
  else
    Assert(static_cast<size_t>(nrVariables) == upperBounds.size(), "row size differs");
}

bool
IntSystem::greaterEqual(const IntVec& arg1, const IntVec& arg2)
{
  IntVec::const_iterator j = arg2.begin();
  FOR_EACH_CONST(i, IntVec, arg1)
    {
      if (*i < *j)
	return false;
      ++j;
    }
  return true;
}

bool
IntSystem::minimal(const IntVec& arg)
{
  //
  //	A vector is minimal if it is not greater or equal to an existing solution.
  //
  FOR_EACH_CONST(i, VecList, solutions)
    {
      if (greaterEqual(arg, *i))
	return false;
    }
  return true;
}

#ifndef NO_ASSERT

void
IntSystem::dumpEqns()
{
  FOR_EACH_CONST(i, VecList, eqns)
    {
      FOR_EACH_CONST(j, IntVec, *i)
	cout << *j << '\t';
      cout << endl;
    }
  cout << endl;
}

#endif
