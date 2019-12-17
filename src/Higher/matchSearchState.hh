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
//	Class for searching for matches within a DAG.
//
#ifndef _matchSearchState_hh_
#define _matchSearchState_hh_
#include "searchState.hh"

class MatchSearchState : public SearchState
{
  NO_COPYING(MatchSearchState);

public:
  enum Flags
  {
    GC_PATTERN = 16
  };

  MatchSearchState(RewritingContext* context,
		   Pattern* pattern,
		   int flags = 0,
		   int minDepth = 0,
		   int maxDepth = -1);
  ~MatchSearchState();

  Pattern* getPattern() const;
  bool findNextMatch();

private:
  Pattern* const pattern;
  bool firstMatch;
};

inline Pattern*
MatchSearchState::getPattern() const
{
  return pattern;
}

#endif
