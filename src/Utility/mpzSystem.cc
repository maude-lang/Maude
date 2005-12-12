/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2005 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class MpzSystem.
//
#include "macros.hh"
#include "vector.hh"
#include "mpzSystem.hh"

//	analysis options
#define ANALYZE_GCD 0
//#define TRACE(s) (std::cout << s << endl)
#define TRACE(s)

//	our stuff
#include "mpzContejeanDevie.cc"
#include "mpzGcdInit.cc"
#include "mpzGcdTrivial.cc"
#include "mpzGcdBasedSolver.cc"

void
MpzSystem::insertEqn(const IntVec& eqn)
{
  Assert(!eqn.empty(), "empty eqn");
  if (eqns.empty())
    nrVariables = eqn.size();
  else
    Assert(nrVariables == eqn.size(), "eqn size differs: " << nrVariables << " vs " << eqn.size());
  eqns.push_back(eqn);  // deep copy
}

void
MpzSystem::setUpperBounds(const IntVec& bounds)
{
  upperBounds = bounds;  // deep copy
}

void
MpzSystem::initializeUpperBounds()
{
  if (upperBounds.empty())
    {
      upperBounds.resize(nrVariables);
      IntVec::iterator e = upperBounds.end();
      for (IntVec::iterator i = upperBounds.begin(); i != e; ++i)
	*i = UNBOUNDED;
    }
  else
    Assert(nrVariables == upperBounds.size(), "row size differs");
}

bool
MpzSystem::greaterEqual(const IntVec& arg1, const IntVec& arg2)
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
MpzSystem::minimal(const IntVec& arg)
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
MpzSystem::dumpEqns()
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
