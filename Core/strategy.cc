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
//      Implementation for abstract class Strategy.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
//#include "indent.hh"
 
//	core class definitions
#include "strategy.hh"

const Vector<int> Strategy::standard;  // constant empty vector

Strategy::Strategy()
{
}

void
Strategy::setStrategy(const Vector<int>& userStrategy, int nrArgs, bool memoFlag)
{
  int stratLen = userStrategy.length();
  stdStrategy = true;
  unevalArgs = false;
  if (stratLen == 0)
    {
      if (memoFlag)
	{
	  for (int i = 0; i < nrArgs; i++)
	    {
	      strategy.append(i + 1);
	      eager.insert(i);
	      evaluated.insert(i);
	    }
	  strategy.append(0);
	  stdStrategy = false;
	}
      return;
    }
  //
  //	Compute a normalized strategy.
  //
  NatSet eagerSet;
  NatSet evaluatedSet;
  bool lastWasZero = false;
  bool semiEager = false;
  for (int i = 0; i < stratLen; i++)
    {
      int a = userStrategy[i];
      if (a < 0)
	a = -a;
      if (a > nrArgs)
	continue;
      if (a == 0)
	{
	  if (!lastWasZero)
	    {
	      strategy.append(0);
	      lastWasZero = true;
	      semiEager = true;
	    }
	}
      else
	{
	  if (!(evaluatedSet.contains(a - 1)))
	    {
	      strategy.append(a);
	      lastWasZero = false;
	      evaluatedSet.insert(a - 1);
	      if (!semiEager)
		eagerSet.insert(a - 1);
	    }
	}
    }
  if (!lastWasZero)
    strategy.append(0);
  //
  //	Check to see if normalized strategy is in fact standard strategy.
  //
  if (memoFlag || strategy.length() != nrArgs + 1)
    stdStrategy = false;
  else
    {
      for (int i = 0; i < nrArgs; i++)
	{
	  if (strategy[i] != i + 1)
	    {
	      stdStrategy = false;
	      break;
	    }
	}
    }
  //
  //	If we don't have the standard strategy we have to keep track of which
  //	arguments are evaulated and which are eager.
  //
  if (!stdStrategy)
    {
      for (int i = 0; i < nrArgs; i++)
	{
	  if (!(evaluatedSet.contains(i)))
	    {
	      unevalArgs = true;
	      break;
	    }
	}
      eager = eagerSet;  // deep copy
      evaluated = evaluatedSet;  // deep copy
    }
}

void
Strategy::setFrozen(const NatSet& frozen)
{
  Strategy::frozen = frozen;
}
