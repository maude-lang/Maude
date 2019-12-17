
/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2016 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for PIG-PUG algorithm with cycle detection.
//

void
PigPug::makeStateKey(CombinedWord& stateKey)
{
  {
    Unificand& lhs = lhsStack.back();
    int len = lhs.word.size();
    for (int i = lhs.index; i < len; ++i)
      stateKey.push_back(lhs.word[i]);
  }
  stateKey.push_back(DELIMITER);
  {
    Unificand& rhs = rhsStack.back();
    int len = rhs.word.size();
    for (int i = rhs.index; i < len; ++i)
      stateKey.push_back(rhs.word[i]);
  }
}

int
PigPug::firstMoveWithCycleDetection()
{
  //
  //	First we cancel any equal variables.
  //
  for (;;)
    {
      int result = cancel();
      if (result == FAIL)
	break;
      if (result != OK)
	return result;  // can only be LHS_DONE or RHS_DONE
    }
  //
  //	Now we check feasibility of remaining equation.
  //
  if (!(feasibleGeneralCase()))
    return NOT_ENTERED;
  //
  //	Check to see if we've reached this state before.
  //
  CombinedWord stateKey;
  makeStateKey(stateKey);
  int safe = arrive(stateKey);
  if (!safe)
    return NOT_ENTERED;  // already been here
  //
  //	Try all three moves until we get success.
  //	It is critcal that equate comes last; i.e. that it has no successor moves.
  //	This is because cancellation generates the equivalent of equate moves and
  //	we do not want these to have successors.
  //
  int result = rhsPeelGeneralCase();
  if (result != FAIL)
    return result;
  result = lhsPeelGeneralCase();
  if (result != FAIL)
    return result;
  return equate();
}

int
PigPug::nextMoveWithCycleDetection()
{
  int previousMove = undoMove();
  if ((previousMove & BOTH) == BOTH)
    {
      //
      //	Last move was cancel or equate - no more moves possible.
      //	However we need to distiguish between them because cancels don't
      //	correspond to fresh states for cycle detection purposes - we only
      //	consider fully canceled states for cycle detection.
      //
      return (previousMove & CANCEL) ? NOT_ENTERED : FAIL;
    }
  //
  //	Try the one or two remaining moves.
  //
  if ((previousMove & BOTH) == INC_RHS)
    {
      //
      //	Last move was rhs peel so we can try lhs peel.
      //
      int result = lhsPeelGeneralCase();
      if (result != FAIL)
	return result;
    }
  return equate(); // always final thing to try
}

int
PigPug::runWithCycleDetection(int result)
{
  //
  //	Pass result = OK for start, and result = FAIL for next try.
  //
  for (;;)
    {
      if (result == OK)
	{
	  //
	  //	Simpify and check a state for feasibility before making first move.
	  //
	  result = firstMoveWithCycleDetection();
	  if (result == FAIL)
	    depart();  // state we just failed to expand is finished
	  continue;
	}
      else if (result == LHS_DONE || result == RHS_DONE)
	{
	  //
	  //	Must have gotten down to lone variable on one side.
	  //
	  if (completed(result) != FAIL)
	    {
	      //
	      //	Success.
	      //
	      confirmedLive();
	      return result;
	    }
	}
      //
      //	Either the last setp was a failure, or we failed to complete.
      //
      if (path.empty())
	break;  // nothing to undo so we're done
      result = nextMoveWithCycleDetection();
      if (result == FAIL)
	depart();  // state we just failed to expand is finished
    }
  return FAIL;
}

bool
PigPug::arrive(const CombinedWord& word)
{
  int stateNr = stateInfo.size();
  pair<WordMap::iterator, bool> p = wordMap.insert(WordMap::value_type(word, stateNr));
  if (p.second)
    {
      //
      //	Saw a new state.
      //
      stateInfo.resize(stateNr + 1);
      StateInfo& s = stateInfo[stateNr];
      s.onStack = true;
      s.onCycle = false;
      s.onLivePath = false;
      traversalStack.append(stateNr);
      return true;
    }
  //
  //	Saw an previous state.
  //
  int index = p.first->second;
  if (stateInfo[index].onStack)
    {
      //
      //	State is already on our traversal stack so we have a cycle.
      //	We mark all states on the cycle as such.
      //
      for (int i = traversalStack.length() - 1;; --i)
	{
	  int stateNr = traversalStack[i];
	  StateInfo& s = stateInfo[stateNr];
	  s.onCycle = true;
	  if (s.onLivePath && incompletenessFlag == 0)
	    {
	      incompletenessFlag = INCOMPLETE;
	      Verbose("Associative unification algorithm detected an infinite family of unifiers.");
	    }
	  if (stateNr == index)
	    break;
	}
      return false;
    }
  //
  //	We reach the previous state via a parallel path, so we push it on the stack.
  //
  stateInfo[index].onStack = true;
  traversalStack.append(index);
  return true;
}

void
PigPug::depart()
{
  Assert(!(traversalStack.empty()), "traversalStack empty");
  //
  //	Pop top state from traversal stack.
  //
  int top = traversalStack.length() - 1;
  int index = traversalStack[top];
  stateInfo[index].onStack = false;
  traversalStack.resize(top);
}

void
PigPug::confirmedLive()
{
  if (traversalStack.empty())
    {
      //
      //	This can only happen if the initial state was trivial and never got pushed.
      //	This shouldn't happen under normal use since the user of PigPug is expected
      //	to cancel trivial equations. But we check it for robustness.
      //
      return;
    }
  //
  //	All those states that are currently on the state have a path to a unifier.
  //
  int stackSize = traversalStack.size();
  for (int i = 0; i < stackSize; ++i)
    {
      int stateNr = traversalStack[i];
      StateInfo& s = stateInfo[stateNr];
      s.onLivePath = true;
      if (s.onCycle && incompletenessFlag == 0)
	{
	  incompletenessFlag = INCOMPLETE;
	  Verbose("Associative unification algorithm detected an infinite family of unifiers.");
	}
    }
}
