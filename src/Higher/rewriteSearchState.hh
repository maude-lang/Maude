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
//	Class for searching for one step rewrites within a DAG.
//
#ifndef _rewriteSearchState_hh_
#define _rewriteSearchState_hh_
#include "searchState.hh"

class RewriteSearchState : public SearchState
{
  NO_COPYING(RewriteSearchState);

public:
  enum Flags
  {
    ALLOW_NONEXEC = 16
  };

  RewriteSearchState(RewritingContext* context,
		     int label,
		     int flags = 0,
		     int minDepth = 0,
		     int maxDepth = -1);

  Rule* getRule() const;
  DagNode* getReplacement() const;
  bool findNextRewrite();

private:
  const int label;
  const bool withExtension;
  int ruleIndex;
};

#endif
