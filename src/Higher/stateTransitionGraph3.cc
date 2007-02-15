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
//	Implementation for class StateTransitionGraph3.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	core class definitions
#include "rewriteSearchState.hh"
#include "stateTransitionGraph3.hh"

local_inline void
StateTransitionGraph3::insertNewState(int parent)
{
  State* s = new State;
  s->rewriteState = 0;
  s->fullyExplored = false;
  s->parent = parent;
  seen.append(s);
}

StateTransitionGraph3::StateTransitionGraph3(RewritingContext* initial)
  : initial(initial)
{
  initial->reduce();
  insertNewState(NONE);
  seenSet.insert(initial->root());
}

StateTransitionGraph3::~StateTransitionGraph3()
{
  int nrStates = seen.length();
  for (int i = 0; i < nrStates; i++)
    {
      delete seen[i]->rewriteState;
      delete seen[i];
    }
  delete initial;
}

void
StateTransitionGraph3::markReachableNodes()
{
  int nrStates = seen.length();
  for (int i = 0; i < nrStates; i++)
    seenSet.index2DagNode(i)->mark();
}

int
StateTransitionGraph3::getNextState(int stateNr, int index)
{
  State* n = seen[stateNr];
  int nrNextStates = n->nextStates.length();
  if (index < nrNextStates)
    return n->nextStates[index];
  if (n->fullyExplored)
    return NONE;
  if (n->rewriteState == 0)
    {
      RewritingContext* newContext = initial->makeSubcontext(seenSet.index2DagNode(stateNr));
      n->rewriteState = new RewriteSearchState(newContext,
					       NONE,
					       RewriteSearchState::GC_CONTEXT,
					       0,
					       UNBOUNDED);
    }

  RewriteSearchState* rewriteState = n->rewriteState;
  RewritingContext *context = rewriteState->getContext();
  while (nrNextStates <= index)
    {
      bool success = rewriteState->findNextRewrite();
      rewriteState->transferCount(*initial);
      
      if (success)
	{
	  Rule* rule = rewriteState->getRule();
	  bool trace = RewritingContext::getTraceStatus();
	  if (trace)
	    {
	      context->tracePreRuleRewrite(rewriteState->getDagNode(), rule);
	      if (context->traceAbort())
		return NONE;
	    }
	  DagNode* replacement = rewriteState->getReplacement();
	  RewriteSearchState::DagPair r = rewriteState->rebuildDag(replacement);
          RewritingContext* c = context->makeSubcontext(r.first);
	  initial->incrementRlCount();
	  if (trace)
	    {
	      c->tracePostRuleRewrite(r.second);
	      if (c->traceAbort())
		{
		  delete c;
		  return NONE;
		}
	    }
	  c->reduce();
	  if (c->traceAbort())
            {
              delete c;
              return NONE;
            }
	  initial->addInCount(*c);
	  delete c;
	  int nextState = seenSet.dagNode2Index(r.first);
	  if (nextState == NONE)
	    {
	      nextState = seen.length();
	      insertNewState(stateNr);
	      seenSet.insert(r.first);
	    }
	  n->nextStates.append(nextState);
	  n->fwdArcs[nextState].insert(rule);
	  ++nrNextStates;
	}
      else
	{
	  delete rewriteState;
	  n->fullyExplored = true;
	  n->rewriteState = 0;
	  return NONE;
	}
    }
  return n->nextStates[index];
}
