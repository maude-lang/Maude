/*

    This file is part of the Maude 3 interpreter.

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
//	Implementation for class MatchSearchState.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "pattern.hh"
#include "matchSearchState.hh"

MatchSearchState::MatchSearchState(RewritingContext* context,
				   Pattern* pattern,
				   int flags,
				   int minDepth,
				   int maxDepth)
  : SearchState(context, flags, minDepth, maxDepth),
    pattern(pattern)
{
  firstMatch = true;
}

MatchSearchState::~MatchSearchState()
{
  //
  //	Delete stuff entrusted to us by our creator.
  //
  if (getFlags() & GC_PATTERN)
    delete pattern;
}

bool
MatchSearchState::findNextMatch()
{
  if (firstMatch)
    firstMatch = false;
  else
    {
      if (findNextSolution())
	return true;
    }

  while (findNextPosition())
    {
      if (findFirstSolution(pattern, pattern->getLhsAutomaton()))
	return true;
    }
  return false;
}
