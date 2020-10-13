/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for expanding the assignments in a level.
//

bool
WordLevel::handleInitialOccursCheckFailure()
{
  //
  //	We never create assignments that look like
  //	  X |-> ... X ...
  //	other than X |-> X which indicates X is unbound.
  //
  //	However it is possible for a user to pass in such an
  //	assignment so we need to resolve those up front.
  //
  int nrAssignments = partialSolution.size();
  for (int i = 0; i < nrAssignments; ++i)
    {
      Word& word = partialSolution[i];
      if (word.size() > 1)
	{
	  for (int j : word)
	    {
	      if (i == j)
		{
		  if (resolveOccursCheckFailure(i, word))
		    break;
		  else
		    return false;
		}
	    }
	}
    }
  return true;  
}

bool
WordLevel::fullyExpandAssignments()
{
  //
  //	Expand assignments to fixed-point.
  //
  for (;;)
    {
      //
      //	First we handle any null equations.
      //	We only check for null equations if we're not a PIGPUG
      //	level because PigPug does not make null equations. This may make
      //	some assignments that can be propagated into expansion.
      //
      if (levelType != PIGPUG && !handleNullEquations())
	return false;
      //
      //	Now we exand all assignments. This can make some null
      //	equations but only if CHANGED.
      //
      Result result = expandAssignments();
      if (result == FAIL)
	return false;
      if (result == DONE)
	break;
    }
  //
  //	All null equations resolved and all assignments fully inter-reduced.
  //
  return true;
}

WordLevel::Result
WordLevel::expandAssignments()
{
  bool changed = false;
  int nrAssignments = partialSolution.size();
  for (int i = 0; i < nrAssignments; ++i)
    {
      Result result = expandAssignment(i, partialSolution[i]);
      if (result == FAIL)
	return FAIL;
      if (result == CHANGED)
	changed = true;
    }
  return changed ? CHANGED : DONE;
}

WordLevel::Result
WordLevel::expandAssignment(int var, Word& word)
{
  DebugEnter("x" << var << " |-> " << word);
  //
  //	Check if assignment needs expansion, i.e. that a variable in
  //	rhs has an assignment different from itself.
  //
  for (int i : word)
    {
      if (i == var)
	{
	  Assert(word.size() == 1, "already dealt with occurs-check failure");
	  return DONE;
	}
      Word& assigned = partialSolution[i];
      if (!(assigned.size() == 1 && assigned[0] == i))
	return reallyExpandAssignment(var, word) ? CHANGED : FAIL;
    }
  return DONE;
}

bool
WordLevel::reallyExpandAssignment(int var, Word& word)
{
  DebugEnter("x" << var << " |-> " << word);
  //
  //	Do the actual expansion; return false if there was an occur-check failure
  //	or constraint failure.
  //
  Word newWord;
  bool occursCheckFail = false;
  for (int i : word)
    {
      Assert(i != var, "occurs-check issue for assignment variable x"
	     << var << " |-> " << word);
      Word& assigned = partialSolution[i];
      if (assigned.size() == 1 && assigned[0] == i)
	newWord.append(i);
      else
	occursCheckFail |= append(newWord, assigned, var);
    }
  if (occursCheckFail)
    {
      //
      //	We only allow resolution of an occurs-check failure
      //	by making empty assignments in INITIAL and SELECTION layers.
      //
      return levelType != PIGPUG && resolveOccursCheckFailure(var, newWord);
    }
  int length = newWord.size();
  VariableConstraint& lhsConstraint = constraintMap[var];
  if (length == 1)
    {
      //
      //	Variable to variable assignment; compute meet
      //	of constraints.
      //
      VariableConstraint& rhsConstraint = constraintMap[newWord[0]];
      if (!(rhsConstraint.intersect(lhsConstraint)))
	return false;  // incompatible constraints
      //
      //	The lhs variable will disappear from the system so
      //	there's no need to update its constraint.
      //
    }
  else
    {
      if (levelType == PIGPUG)
	{
	  //
	  //	INITIAL and SELECTION layers can make empty assignments.
	  //	But if this is a PIGPUG layer we check for bounds violations.
	  //
	  if (!(lhsConstraint.isUnbounded()) && lhsConstraint.getUpperBound() < length)
	    return false;
	}
    }
  word.swap(newWord);
  DebugExit("x" << var << " |-> " << word);
  return true;
}

bool
WordLevel::append(Word& newWord, const Word& word, int var)
{
  bool occursCheckFail = false;
  for (int i : word)
    {
      newWord.append(i);
      if (i == var)
	occursCheckFail = true;
    }
  return occursCheckFail;
}
