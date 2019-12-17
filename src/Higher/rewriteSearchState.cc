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
//	Implementation for class RewriteSearchState.
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
#include "rhsAutomaton.hh"
#include "rule.hh"
#include "rewriteSearchState.hh"

RewriteSearchState::RewriteSearchState(RewritingContext* context,
				       int label,
				       int flags,
				       int minDepth,
				       int maxDepth)
  : SearchState(context, flags | RESPECT_FROZEN, minDepth, maxDepth),
    label(label),
    withExtension(maxDepth >= 0)
{
  Assert(label == NONE || !(getFlags() & SET_UNREWRITABLE),
	  "shouldn't set unrewritable flag if only looking at rules with a given label");
  ruleIndex = -1;
}

bool
RewriteSearchState::findNextRewrite()
{
  bool rewriteSeenAtCurrentPosition;
  if (ruleIndex > -1)
    {
      if (findNextSolution())
	return true;
      rewriteSeenAtCurrentPosition = true;
    }
  else
    {
      if (!findNextPosition())
	return false;
      rewriteSeenAtCurrentPosition = false;
    }
  ++ruleIndex;
  bool allowNonexec = getFlags() & ALLOW_NONEXEC;
  do
    {
      DagNode* d = getDagNode();
      if (!(d->isUnrewritable()))
	{
	  const Vector<Rule*>& rules = d->symbol()->getRules();
	  for (int nrRules = rules.length(); ruleIndex < nrRules; ruleIndex++)
	    {
	      Rule* rl = rules[ruleIndex];
	      if ((allowNonexec || !(rl->isNonexec())) &&
		  (label == UNDEFINED || rl->getLabel().id() == label))
		{
		  LhsAutomaton* a = withExtension ? rl->getExtLhsAutomaton() :
		    rl->getNonExtLhsAutomaton();
		  //cerr << "trying " << rl << " at " << " positionIndex " <<  getPositionIndex() << " dagNode " << getDagNode() << endl;
		  if (findFirstSolution(rl, a))
		    return true;
		}
	    }
	  if (!rewriteSeenAtCurrentPosition && (getFlags() & SET_UNREWRITABLE))
	    d->setUnrewritable();
	}
      rewriteSeenAtCurrentPosition = false;
      ruleIndex = 0;
    }
  while (findNextPosition());
  return false;
}

Rule*
RewriteSearchState::getRule() const
{
  return (getDagNode()->symbol()->getRules())[ruleIndex];
}

DagNode*
RewriteSearchState::getReplacement() const
{
  return getRule()->getRhsBuilder().construct(*(getContext()));
}
