/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for handling null equations and occurs-check failure.
//

bool
WordLevel::handleNullEquations()
{
  Assert(levelType != PIGPUG, "should not be called in PIGPUG level");
  //
  //	Resolve existing null equations, which may create new null equations
  //	until we encounter failure or there are no null equations left.
  //	There can only be finitely many new null equations created since
  //	each one results from a variable being assigned empty.
  //
  while (!(nullEquations.empty()))
    {
      for (int i : nullEquations.front())
	{
	  Word& currentAssignment = partialSolution[i];
	  if (!(currentAssignment.empty()))
	    {
	      if (constraintMap[i].canTakeEmpty())
		{
		  if (currentAssignment.size() == 1 && currentAssignment[0] == i)
		    ;  // maps to itself so can update without issue
		  else
		    {
		      //
		      //	Maps to something else so we need to create a new null equation.
		      //
		      nullEquations.push_back(currentAssignment);
		    }
		  //
		  //	Variable x_i is assigned the empty word.
		  //
		  DebugInfo("binding x" << i << " to empty to solve null equation");
		  currentAssignment.clear();
		}
	      else
		{
		  //
		  //	We needed x_i to take the empty word but this is
		  //	forbidden by its constraint so the whole WordLevel
		  //	will fail so we don't care about leaving it in an
		  //	inconsistent state.
		  //
		  return false;
		}
	    }
	}
      //
      //	Null equation at the front of the queue has been resolved.
      //
      nullEquations.pop_front();
    }
  //
  //	All null equations sucessfully resolved.
  //
  return true;
}

bool
WordLevel::resolveOccursCheckFailure(int index, const Word& newValue)
{
  Assert(levelType != PIGPUG, "should not be called in PIGPUG level");
  //
  //	We want to assign newValue to variable index, but newValue
  //	contains index. We see if this can be resolved via collapse.
  //	Maybe update assignments and create new null equations.
  //
  int nrOccurrences = 0;
  for (int i : newValue)
    {
      if (i == index)
	++nrOccurrences;
      else
	{
	  Word& currentAssignment = partialSolution[i];
	  //
	  //	Could appear multiple times in newValue so maybe we already
	  //	assigned it the empty word.
	  //
	  if (!(currentAssignment.empty()))
	    {
	      if (constraintMap[i].canTakeEmpty())
		{
		  if (currentAssignment.size() == 1 && currentAssignment[0] == i)
		    ;  // maps to itself so can update without issue
		  else
		    {
		      //
		      //	Maps to something else so we need to create a new null equation.
		      //
		      nullEquations.push_back(currentAssignment);
		    }
		  //
		  //	Variable x_i is assigned the empty word.
		  //
		  DebugInfo("binding x" << i << " to empty to solve occurs-check failure");
		  currentAssignment.clear();
		}
	      else
		{
		  //
		  //	We needed x_i to take the empty word but this is
		  //	forbidden by its constraint so the whole WordLevel
		  //	will fail so we don't care about leaving it in an
		  //	inconsistent state.
		  //
		  return false;
		}
	    }
	}
    }
  Assert(nrOccurrences >= 1, "must have occurence for an occur-check failure");
  if (nrOccurrences > 1)
    {
      //
      //	Variable index occurs multiple time so it must take empty itself.
      //
      if (constraintMap[index].canTakeEmpty())
	{
	  //
	  //	We assume we can freely replace the value of variable index
	  //	because that's what were called to do.
	  //
	  DebugInfo("binding x" << index << " to empty to solve multiple occurs-check failure");
	  partialSolution[index].clear();
	}
      else
	{
	  //
	  //	As above, we can bail and leave things in an inconsistent state
	  return false;
	}
    }
  else
    {
      //
      //	Variable index becomes free because everything else has collapsed.
      //
      Word& currentAssignment = partialSolution[index];
      currentAssignment.resize(1);
      currentAssignment[0] = index;
    }
  return true;
}
