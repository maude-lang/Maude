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
//	Implementation for class DisjunctiveSubproblemAccumulator.
//
//
//      Implementation for class SubproblemDisjunction
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "extensionInfo.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "localBinding.hh"
#include "subproblemDisjunction.hh"
#include "disjunctiveSubproblemAccumulator.hh"

DisjunctiveSubproblemAccumulator::~DisjunctiveSubproblemAccumulator()
{
  //
  //	It's legal to quit at any time; maybe because we found a greedy alternative
  //	and so were not interested in the options we have accumulated to date.
  //
  if (!first)
    {
      delete firstDifference;
      delete firstSubproblem;
      delete firstExtensionInfo;
      delete disjunction;
    }
}

void
DisjunctiveSubproblemAccumulator::addOption(Substitution& local,
					    Subproblem* subproblem,
					    ExtensionInfo* extensionInfo)
{
  if (first)
    {
      firstDifference = local - global;
      firstSubproblem = subproblem;
      if (extensionInfo != 0 && extensionInfo->validAfterMatch())
	firstExtensionInfo = extensionInfo->makeClone();
      else
	firstExtensionInfo = 0;
      first = false;
      disjunction = 0;
    }
  else
    {
      if (disjunction == 0)
	{
	  disjunction = new SubproblemDisjunction;
	  disjunction->addOption(firstDifference, firstSubproblem, firstExtensionInfo);
	  firstDifference = 0;
	  firstSubproblem = 0;
	  firstExtensionInfo = 0;
	}
      disjunction->addOption(local - global,
			     subproblem,
			     (extensionInfo != 0 && extensionInfo->validAfterMatch()) ?
			     extensionInfo->makeClone() : 0);
    }
}

bool
DisjunctiveSubproblemAccumulator::extract(Substitution& solution,
					  Subproblem*&returnedSubproblem,
					  ExtensionInfo* extensionInfo)
{
  //
  //	If no options accumulated we fail.
  //
  if (first)
    return false;
  //
  //	If just one option accumulated, force it.
  //
  if (disjunction == 0)
    {
      if (firstDifference != 0)
	{
#ifdef NO_ASSERT
	  (void) firstDifference->assert(solution);
#else
	  bool ok = firstDifference->assert(solution);
	  Assert(ok, "binding clash");
#endif
	}
      returnedSubproblem = firstSubproblem;
      firstSubproblem = 0;  // so dtor won't delete it
      if (firstExtensionInfo != 0)
	{
	  Assert(extensionInfo != 0, "extensionInfo null while firstExtensionInfo not null");
	  extensionInfo->copy(firstExtensionInfo);
	}
      else
	{
	  if (extensionInfo != 0)
	    extensionInfo->setValidAfterMatch(false);
	}
      return true;
    }
  //
  //	If multiple options accumulated return a true disjunction.
  //
  returnedSubproblem = disjunction;
  disjunction = 0;  // so dtor won't delete it
  if (extensionInfo != 0)
    extensionInfo->setValidAfterMatch(false);
  return true;
}
