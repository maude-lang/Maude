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
//	Implementation for class StateTransitionGraph.
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
#include "stateTransitionGraph.hh"

StateTransitionGraph::StateTransitionGraph(RewritingContext* initial)
  : initial(initial)
{
  initial->reduce();
  int hashConsIndex = hashConsSet.insert(initial->root());
  hashCons2seen.resize(hashConsIndex + 1);
  for (int i = 0; i < hashConsIndex; ++i)
    hashCons2seen[i] = NONE;
  hashCons2seen[hashConsIndex] = seen.size();
  seen.append(new State(hashConsIndex, NONE));
}

StateTransitionGraph::~StateTransitionGraph()
{
  int nrStates = seen.length();
  for (int i = 0; i < nrStates; i++)
    {
      delete seen[i]->rewriteState;
      delete seen[i];
    }
  delete initial;
}

int
StateTransitionGraph::getNextState(int stateNr, int index)
{
  State* n = seen[stateNr];
  int nrNextStates = n->nextStates.length();
  if (index < nrNextStates)
    return n->nextStates[index];
  if (n->fullyExplored)
    return NONE;
  if (n->rewriteState == 0)
    {
      DagNode* canonicalStateDag = hashConsSet.getCanonical(seen[stateNr]->hashConsIndex);
      RewritingContext* newContext = initial->makeSubcontext(canonicalStateDag);
      n->rewriteState = new RewriteSearchState(newContext,
					       NONE,
					       RewriteSearchState::GC_CONTEXT |
					       RewriteSearchState::SET_UNREWRITABLE |
					       PositionState::SET_UNSTACKABLE,
					       0,
					       UNBOUNDED);
    }

  RewriteSearchState* rewriteState = n->rewriteState;
  RewritingContext *context = rewriteState->getContext();
  while (nrNextStates <= index)
    {
      bool success = rewriteState->findNextRewrite();
      rewriteState->transferCountTo(*initial);
      
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

	  int nextState;
	  int hashConsIndex = hashConsSet.insert(r.first);
	  int mapSize = hashCons2seen.size();
	  //DebugAdvisory("replacement dag = " << r.first << "hashConsIndex = " << hashConsIndex);
	  if (hashConsIndex >= mapSize)
	    {
	      //
	      //	Definitely a new state.
	      //
	      hashCons2seen.resize(hashConsIndex + 1);
	      for (int i = mapSize; i < hashConsIndex; ++i)
		hashCons2seen[i] = NONE;
	      nextState = seen.size();
	      hashCons2seen[hashConsIndex] = nextState;
	      seen.append(new State(hashConsIndex, stateNr));
	      DebugAdvisory("new state dag = " << r.first <<
			    " hashConsIndex = " << hashConsIndex <<
			    " collisionCounter = " << hashConsSet.collisionCounter);
	    }
	  else
	    {
	      //
	      //	Seen before.
	      //
	      nextState = hashCons2seen[hashConsIndex];
	      if (nextState == NONE)
		{
		  //
		  //	 But only as a subdag, not as a state dag, so it counts as a new state.
		  //
		  nextState = seen.size();
		  hashCons2seen[hashConsIndex] = nextState;
		  seen.append(new State(hashConsIndex, stateNr));
		}
	    }
	  n->nextStates.append(nextState);
	  n->fwdArcs[nextState].insert(rule);
	  ++nrNextStates;
	  //
	  //	If we didn't do any equational rewriting we will not have had a chance to
	  //	collect garbage.
	  //
	  MemoryCell::okToCollectGarbage();
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
