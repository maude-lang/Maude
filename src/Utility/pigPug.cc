/*

    This file is part of the Maude 2 interpreter.

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
//      Implementation for class PigPug.
//
#include "macros.hh"
#include "vector.hh"
#include "pigPug.hh"

#include "pigPug-cycleDetection.cc"

PigPug::PigPug(const Word& lhs,
	       const Word& rhs,
	       const ConstraintMap& constraintMap,
	       int lastOriginalVariable,
	       int freshVariableStart,
	       bool strictLeftLinear)
  : constraintMap(constraintMap),
    lastOriginalVariable(lastOriginalVariable),
    freshVariableStart(freshVariableStart),
    strictLeftLinear(strictLeftLinear)
{
  lhsStack.push_back(Unificand());
  lhsStack.back().index = 0;
  lhsStack.back().word = lhs;  // deep copy
  rhsStack.push_back(Unificand());
  rhsStack.back().index = 0;
  rhsStack.back().word = rhs;  // deep copy

  incompletenessFlag = 0;
  depthBound = NONE;
  cycleDetection = false;
  if (!strictLeftLinear)
    {
      //
      //	If we are not strictly left linear then we need either cycle detection
      //	or depth bounded to ensure termination.
      //	Both of these measures introduce the possibility of incompleteness.
      //
      if (variableOccurrencesBoundedBy2(lhs, rhs, lastOriginalVariable))
	{
	  cycleDetection = true;
	  Verbose("Associative unification using cycle detection.");
	}
      else
	{
	  depthBound =  lhs.size() + rhs.size();
	  Verbose("Associative unification using depth bound of " << depthBound << ".");
	}
    }
}

bool
PigPug::variableOccurrencesBoundedBy2(const Word& lhs, const Word& rhs, int maxVarNumber)
{
  //
  //	We are only concerned with unconstrained variables.
  //
  Vector<int> varCount(maxVarNumber + 1);
  for (int i = 0; i <= maxVarNumber; ++i)
    varCount[i] = 0;
  {
    FOR_EACH_CONST(i, Word, lhs)
      {
	int var = *i;
	if (++(varCount[var]) > 2 && constraintMap[var] == NONE)
	  return false;
      }
  }
  {
    FOR_EACH_CONST(i, Word, rhs)
      {
	int var = *i;
	if (++(varCount[var]) > 2 && constraintMap[var] == NONE)
	  return false;
      }
  }
  return true;
}

PigPug::ResultPair
PigPug::getNextUnifier(Subst& unifier)
{
  //
  //	Returns NONE or index of next unused variable.
  //
  int result = cycleDetection ?
    runWithCycleDetection(path.empty() ? OK : FAIL) :
    run(path.empty() ? OK : FAIL);
  if (result == FAIL)
    return ResultPair(FAILURE | incompletenessFlag, NONE);

  int nextFreshVariable = extractUnifier(unifier);
  return ResultPair(SUCCESS | incompletenessFlag, nextFreshVariable);
}

PigPug::Result
PigPug::rhsPeel()
{
  //
  //	Given
  //	  x... =? y...
  //	if x is unconstrained we make the step x |-> yx and then cancel leading y instances.
  //	Since x is linear there is no need to modify either unificand - we just increment the rhs index.
  //
  Unificand& lhs = lhsStack.back();
  int lhsVar = lhs.word[lhs.index];
  if (constraintMap[lhsVar] != NONE)
    return FAIL;  // lhs variable is constrained

  path.append(INC_RHS);
  Unificand& rhs = rhsStack.back();
  int newRhsIndex = ++(rhs.index);
  return (newRhsIndex + 1 == rhs.word.length()) ? RHS_DONE : OK;
}

bool
PigPug::checkUnificand(UnificandStack& unificandStack, int oldVar, int newVar)
{
  //
  //	Check unificand for occurrences of oldVar _from_ current position, and
  //	if necessary push a modified unificand (oldVar |-> newVar) on the stack.
  //	Returns true if a modified unificand is pushed, false if the unificand is unmodified.
  //
  Unificand& unificand = unificandStack.back();
  int index = unificand.index;
  int unificandLength = unificand.word.size();
  for (int i = index; i < unificandLength; ++i)
    {
      if (unificand.word[i] == oldVar)
	{
	  //
	  //	Another instance of oldVar found - make a new unificand
	  //
	  int remainingUnificand = unificandLength - index;
	  unificandStack.push_back(Unificand());
	  Unificand& newUnificand = unificandStack.back();
	  newUnificand.index = 0;
	  newUnificand.word.resize(remainingUnificand);
	  for (int j = 0; j < remainingUnificand; ++j)
	    {
	      int var = unificand.word[j + index];
	      newUnificand.word[j] = (var == oldVar) ? newVar : var;
	    }
	  return true;
	}
    }
  return false;
}

PigPug::Result
PigPug::equate()
{
  //
  //	Given
  //	  x... =? y...
  //	we try to make x and y equal and cancel them. Whether we put x |-> y or y |-> x
  //	depends on which variable is least constrained - we want to replace
  //	the least constrained variable with more constrained variable
  //	breaking ties with x |-> y because if x in unconstrained it must be linear.
  //
  //	In all other cases, the variable being replaced may be nonlinear so we
  //	need to check both unificands and push modified versions on the stacks
  //	if necessary.
  //
  Unificand& lhs = lhsStack.back();
  Unificand& rhs = rhsStack.back();
  int lhsVar = lhs.word[lhs.index];
  int rhsVar = rhs.word[rhs.index];
  int lhsConstraint = constraintMap[lhsVar];
  int rhsConstraint = constraintMap[rhsVar];

  int move = INC_RHS | INC_LHS;
  if (lhsConstraint == NONE)
    {
      //
      //	lhsVar |-> rhsVar
      //
      //	lhsVar is unconstrained so in the linear case it disappears,
      //	taking rhsVar with it.
      //
      ++(lhs.index);
      ++(rhs.index);
      if (!strictLeftLinear)
	{
	  //
	  //	General case: lhsVar might appear anywhere in remaining equation.
	  //
	  if (checkUnificand(lhsStack, lhsVar, rhsVar))
	    move |= PUSH_LHS;
	  if (checkUnificand(rhsStack, lhsVar, rhsVar))
	    move |= PUSH_RHS;
	}
    }
  else if ((lhsConstraint == ELEMENT && rhsConstraint != NONE) ||
	   lhsConstraint == rhsConstraint)
    {
      //
      //	lhsVar |-> rhsVar
      //
      //	lhsVar is constrained and might be nonlinear.
      //
      ++(lhs.index);
      ++(rhs.index);
      //
      //	Check if lhsVar has further appearences in lhs.
      //
      if (checkUnificand(lhsStack, lhsVar, rhsVar))
	move |= PUSH_LHS;
      //
      //	Check if lhsVar appears in the rhs.
      //
      if (checkUnificand(rhsStack, lhsVar, rhsVar))
	move |= PUSH_RHS;
    }
  else if (rhsConstraint == NONE || rhsConstraint == ELEMENT)
    {
      //
      //	rhsVar |-> lhsVar
      //
      ++(lhs.index);
      ++(rhs.index);
      move |= RHS_ASSIGN;
      //
      //	Check if rhsVar has further appearences in rhs.
      //
      if (checkUnificand(rhsStack, rhsVar, lhsVar))
	move |= PUSH_RHS;
      //
      //	If rhsVar is constrained or we are in general case, it might appear in lhs.
      //
      if (rhsConstraint != NONE || !strictLeftLinear)
	{
	  if (checkUnificand(lhsStack, rhsVar, lhsVar))
	    move |= PUSH_LHS;
	}
    }
  else
    return FAIL;  // incompatible constraints

  path.append(move);

  Unificand& newLhs = lhsStack.back();
  if (newLhs.index + 1 == newLhs.word.length())
    return LHS_DONE;

  Unificand& newRhs = rhsStack.back();
  if (newRhs.index + 1 == newRhs.word.length())
    return RHS_DONE;

  return OK;
}

bool
PigPug::checkUnificand2(UnificandStack& unificandStack, int oldVar, int newVar, int offset)
{
  //
  //	Check unificand for occurrences of oldVar from current position + offset, and
  //	if necessary push a modified unificand (oldVar |-> newVar oldVar) on the stack.
  //	Returns true if a modified unificand is pushed, false if the unificand is unmodified.
  //
  Unificand& unificand = unificandStack.back();
  int index = unificand.index;
  int unificandLength = unificand.word.size();
  for (int i = index + offset; i < unificandLength; ++i)
    {
      if (unificand.word[i] == oldVar)
	{
	  //
	  //	Another instance of oldVar found - make a new unificand
	  //
	  unificandStack.push_back(Unificand());
	  Unificand& newUnificand = unificandStack.back();
	  newUnificand.index = 0;
	  for (int j = index; j < i; ++j)
	    newUnificand.word.append(unificand.word[j]);
	  newUnificand.word.append(newVar);
	  newUnificand.word.append(oldVar);
	  for (int j = i + 1; j < unificandLength; ++j)
	    {
	      int var = unificand.word[j];
	      if (var == oldVar)
		newUnificand.word.append(newVar);
	      newUnificand.word.append(var);
	    }
	  return true;
	}
    }
  return false;
}

PigPug::Result
PigPug::lhsPeel()
{
  //
  //	Given
  //	  x... =? y...
  //	if y is unconstrained we make the step y |-> xy and then cancel leading x instances.
  //	Since y may be nonlinear we need to check rhs for more occurences of y and push
  //	a modified rhs if so. Either way we consume one lhs variable.
  //
  Unificand& rhs = rhsStack.back();
  int rhsVar = rhs.word[rhs.index];
  if (constraintMap[rhsVar] != NONE)
    return FAIL;  // rhs variable is constrained

  int move = INC_LHS;
  Unificand& lhs = lhsStack.back();
  int lhsVar = lhs.word[lhs.index];
  if (checkUnificand2(rhsStack, rhsVar, lhsVar, 1))
    move |= PUSH_RHS;

  path.append(move);  // move will be either INC_LHS or INC_LHS | STACK_RHS
  int newLhsIndex = ++(lhs.index);
  return (newLhsIndex + 1 == lhs.word.length()) ? LHS_DONE : OK;
}

PigPug::Result
PigPug::lhsPeelGeneralCase()
{
  //
  //	Given
  //	  x... =? y...
  //	if y is unconstrained we make the step y |-> xy and then cancel leading x instances.
  //	This is the general case where y may have arbitrary occurences in both lhs and rhs,
  //	so we may need to push modified lhs/rhs. Either way we consume one lhs variable.
  //
  Unificand& rhs = rhsStack.back();
  int rhsVar = rhs.word[rhs.index];
  if (constraintMap[rhsVar] != NONE)
    return FAIL;  // rhs variable is constrained

  int move = INC_LHS;
  Unificand& lhs = lhsStack.back();
  int lhsVar = lhs.word[lhs.index];
  ++(lhs.index);  // consume lhs variable
  if (checkUnificand2(lhsStack, rhsVar, lhsVar, 0))  // look for occurances of rhs variable from new index
    move |= PUSH_LHS;
  if (checkUnificand2(rhsStack, rhsVar, lhsVar, 1))  // look for occurances of rhs variable from index + 1
    move |= PUSH_RHS;

  path.append(move);  // move INC_LHS with any of PUSH_LHS, PUSH_RHS
  //
  //	Did we get lhs down to one variable?
  //
  Unificand& newLhs = lhsStack.back();
  return (newLhs.index + 1 == newLhs.word.length()) ? LHS_DONE : OK;
}

PigPug::Result
PigPug::rhsPeelGeneralCase()
{
  //
  //	Given
  //	  x... =? y...
  //	if x is unconstrained we make the step x |-> yx and then cancel leading y instances.
  //	This is the general case where x may have arbitrary occurences in both lhs and rhs,
  //	so we may need to push modified lhs/rhs. Either way we consume one rhs variable.
  //
  Unificand& lhs = lhsStack.back();
  int lhsVar = lhs.word[lhs.index];
  if (constraintMap[lhsVar] != NONE)
    return FAIL;  // lhs variable is constrained

  int move = INC_RHS;
  Unificand& rhs = rhsStack.back();
  int rhsVar = rhs.word[rhs.index];
  ++(rhs.index);  // consume rhs variable
  if (checkUnificand2(rhsStack, lhsVar, rhsVar, 0))  // look for occurances of lhs variable from new index; BAD HERE
    move |= PUSH_RHS;
  if (checkUnificand2(lhsStack, lhsVar, rhsVar, 1))  // look for occurances of lhs variable from index + 1
    move |= PUSH_LHS;

  path.append(move);  // move INC_RHS with any of PUSH_LHS, PUSH_RHS
  //
  //	Did we get rhs down to one variable?
  //
  Unificand& newRhs = rhsStack.back();
  return (newRhs.index + 1 == newRhs.word.length()) ? RHS_DONE : OK;
}

int
PigPug::undoMove()
{
  //
  //	Undo last move.
  //
  int pos = path.size() - 1;
  int move = path[pos];
  path.resize(pos);
  //
  //	Possible moves in left linear case:
  //	  INC_RHS
  //	  INC_LHS
  //	  INC_LHS | PUSH_RHS
  //	  INC_LHS | INC_RHS with any combination of RHS_ASSIGN, PUSH_LHS, PUSH_RHS
  //
  //	In the general case the following is also possible:
  //	  INC_RHS with any combination of PUSH_LHS, PUSH_RHS
  //	  INC_LHS with any combination of PUSH_LHS, PUSH_RHS
  //
  //	In the case that LHS (RHS) is both incremented and pushed, the incrementing
  //	happens before the push, and thus for undo the decrementing must happen after
  //	the pop.
  //
  if (move & PUSH_LHS)
    lhsStack.pop_back();
  if (move & INC_LHS)
    --(lhsStack.back().index);

  if (move & PUSH_RHS)
    rhsStack.pop_back();
  if (move & INC_RHS)
    --(rhsStack.back().index);

  return move;
}

PigPug::Result
PigPug::cancel()
{
  Unificand& lhs = lhsStack.back();
  int lhsVar = lhs.word[lhs.index];
  Unificand& rhs = rhsStack.back();
  int rhsVar = rhs.word[rhs.index];

  if (lhsVar != rhsVar)
    return FAIL;

  ++(lhs.index);
  ++(rhs.index);

  path.append(INC_RHS | INC_LHS | CANCEL);

  if (lhs.index + 1 == lhs.word.length())
    return LHS_DONE;

  if (rhs.index + 1 == rhs.word.length())
    return RHS_DONE;

  return OK;
}

bool
PigPug::fullyConstrained(const Unificand& unificand)
{
  int length = unificand.word.size();
  for (int i = unificand.index; i < length; ++i)
    {
      if (constraintMap[unificand.word[i]] == NONE)
	return false;
    }
  return true;
}

bool
PigPug::feasible()
{
  //
  //	Check to see if the smaller unificand has become fully constrained.
  //
  Unificand& lhs = lhsStack.back();
  Unificand& rhs = rhsStack.back();
  int lhsSize = lhs.word.size() - lhs.index;
  int rhsSize = rhs.word.size() - rhs.index;
  if (lhsSize > rhsSize)
    return !fullyConstrained(rhs);
  if (lhsSize < rhsSize)
    return !fullyConstrained(lhs);
  return true;
}

bool
PigPug::canCancelUnconstrained(const Unificand& big, const Unificand& small)
{
  //
  //	Check if big can cancel all unconstrained variables in small, under
  //	commutativity. This guarantees unification failure.
  //	Degenerate case is if small has no unconstrained variables.
  //
  Vector<int> counts(lastOriginalVariable + 1);
  for (int i = 0; i <= lastOriginalVariable; ++i)
    counts[i] = 0;
  {
    //
    //	First we make a negative count of the unconstrained variables in big.
    //
    int length = big.word.size();
    for (int i = big.index; i < length; ++i)
      {
	int var = big.word[i];
	if (constraintMap[var] == NONE)
	  --(counts[var]);
      }
  }
  {
    //
    //	Now we check if there is any unconstrained variable in small that is not
    //	cancelled by these negative counts.
    //
    int length = small.word.size();
    for (int i = small.index; i < length; ++i)
      {
	int var = small.word[i];
	if (constraintMap[var] == NONE)
	  {
	    ++(counts[var]);
	    if (counts[var] > 0)
	      return false;  // var is not cancelled
	  }     
      }
  }
  return true;
}

bool
PigPug::feasibleGeneralCase()
{
  Unificand& lhs = lhsStack.back();
  Unificand& rhs = rhsStack.back();
  int lhsSize = lhs.word.size() - lhs.index;
  int rhsSize = rhs.word.size() - rhs.index;
  if (lhsSize > rhsSize)
    return !canCancelUnconstrained(lhs, rhs);
  if (lhsSize < rhsSize)
    return !canCancelUnconstrained(rhs, lhs);
  return true;
}

int
PigPug::firstMove()
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
	return result;
    }
  //
  //	Now we check feasibility of remaining equation.
  //
  bool ok = strictLeftLinear ? feasible() : feasibleGeneralCase();
  if (!ok)
    return FAIL;
  if (depthBound != NONE && path.length() >= depthBound)
    {
      //
      //	We cut off the seach to ensure termination but
      //	this means that the set of solutions we return
      //	may be incomplete.
      //
      if (incompletenessFlag != INCOMPLETE)
	{
	  incompletenessFlag = INCOMPLETE;
	  Verbose("Associative unification algorithm hit depth bound.");
	}
      return FAIL;
    }
  //
  //	Try all three moves until we get success.
  //	It is critcal that equate comes last; i.e. that it has no successor moves.
  //	This is because cancellation generates the equivalent of equate moves and
  //	we do not want these to have successors.
  //
  int result = strictLeftLinear ? rhsPeel() : rhsPeelGeneralCase();
  if (result != FAIL)
    return result;
  result = strictLeftLinear ? lhsPeel() : lhsPeelGeneralCase();
  if (result != FAIL)
    return result;
  return equate();
}

int
PigPug::nextMove()
{
  int previousMove = undoMove() & MOVES;
  if (previousMove == BOTH)
    return FAIL;  // last move was cancel or equate - no more moves for this state
  //
  //	Try the one or two remaining moves.
  //
  if (previousMove == INC_RHS)
    {
      //
      //	Last move was rhs peel so we can try lhs peel.
      //
      int result = strictLeftLinear ? lhsPeel() : lhsPeelGeneralCase();
      if (result != FAIL)
	return result;
    }
  return equate(); // always final thing to try
}

bool
PigPug::occurs(int variable, const Unificand& unificand)
{
  int length = unificand.word.size();
  for (int i = unificand.index; i < length; ++i)
    {
      if (unificand.word[i] == variable)
	return true;
    }
  return false;
}

int
PigPug::completed(int status)
{
  //
  //	At least side of current equation is down to a single variable; see
  //	if we can make a solution.
  //
  if (status == LHS_DONE)
    {
      Unificand& lhs = lhsStack.back();
      int lhsVar = lhs.word[lhs.index];
      int lhsConstraint = constraintMap[lhsVar];
      if (lhsConstraint == NONE)
	{
	  //
	  //	Last lhs variable can take anything. If we are in general case,
	  //	do an occurs check.
	  //
	  if (!strictLeftLinear)
	    {
	      Unificand& rhs = rhsStack.back();
	      int remaining = rhs.word.size() - rhs.index;
	      if (remaining > 1 && occurs(lhsVar, rhs))
		return FAIL;
	    }
	  return status;
	}
      //
      //	Constrained variable can only unify with a single rhs variable.
      //
      Unificand& rhs = rhsStack.back();
      if (rhs.index + 1 == rhs.word.length())
	{
	  //
	  //	Either the rhs variable must be unconstrained or
	  //	its constraint must be compatible with that of the lhs variable.
	  //	We can also end up in this case if the variables are equal.
	  //
	  int rhsVar = rhs.word[rhs.index];
	  int rhsConstraint = constraintMap[rhsVar];
	  if (rhsConstraint == NONE ||
	      rhsConstraint == ELEMENT ||
	      lhsConstraint == ELEMENT ||
	      lhsConstraint == rhsConstraint)
	    return status;
	}
      return FAIL;
    }

  Assert(status == RHS_DONE, "bad status for completed()");
  Unificand& rhs = rhsStack.back();
  int rhsVar = rhs.word[rhs.index];
  //
  //	We fail unless the rhs variable is unconstrained since the lhs
  //	has more that one variable left. If we are in general case we
  //	need to do an occurs check.
  //
  if (constraintMap[rhsVar] != NONE)
    return FAIL;
  if (!strictLeftLinear && occurs(rhsVar, lhsStack.back()))
    return FAIL;
  return status;
}

int
PigPug::run(int result)
{
  //
  //	Pass result = OK for start, and result = FAIL for next try.
  //
  for (;;)
    {
      if (result == OK)
	result = firstMove();
      else
	{
	  if (result == FAIL || completed(result) == FAIL)
	    {
	      if (path.empty())
		break;
	      result = nextMove();
	    }
	  else
	    return result;  // must have completed
	}
    }
  return FAIL;
}

void
PigPug::compose(Subst& subst, int oldVar, int replacement)
{
  //
  //	Compose subst with { oldVar |-> replacement }.
  //
  for (int i = 0; i <= lastOriginalVariable; ++i)
    {
      Word& word = subst[i];
      int wordLength = word.size();
      for (int j = 0; j < wordLength; ++j)
	{
	  if (word[j] == oldVar)
	    word[j] = replacement;
	}
    }
}

void
PigPug::compose2(Subst& subst, int oldVar, int replacement)
{
  //
  //	Compose subst with { oldVar |-> replacement oldVar }.
  //
  for (int i = 0; i <= lastOriginalVariable; ++i)
    {
      Word& word = subst[i];
      int wordLength = word.size();
      for (int j = 0; j < wordLength; ++j)
	{
	  if (word[j] == oldVar)
	    {
	      Word newWord(j + 2);
	      for (int k = 0; k < j; ++k)
		newWord[k] = word[k];
	      newWord[j] = replacement;
	      newWord[j + 1] = oldVar;
	      for (int k = j + 1; k < wordLength; ++k)
		{
		  int var = word[k];
		  if (var == oldVar)
		    newWord.append(replacement);
		  newWord.append(var);
		}
	      word.swap(newWord);
	      break;
	    }
	}
    }
}

void
PigPug::compose(Subst& subst, int oldVar, const Word& replacement, int index)
{
  //
  //	Compose subst with { var |-> suffix } where suffix
  //	is the suffix of replacement starting at index.
  //
  int replacementLength = replacement.size();
  for (int i = 0; i <= lastOriginalVariable; ++i)
    {
      Word& word = subst[i];
      int wordLength = word.size();
      for (int j = 0; j < wordLength; ++j)
	{
	  if (word[j] == oldVar)
	    {
	      Word newWord(j);
	      for (int k = 0; k < j; ++k)
		newWord[k] = word[k];
	      for (int k = j ; k < wordLength; ++k)
		{
		  int var = word[k];
		  if (var == oldVar)
		    {
		      for (int l = index; l < replacementLength; ++l)
			newWord.append(replacement[l]);
		    }
		  else
		    newWord.append(var);
		}
	      word.swap(newWord);
	      break;
	    }
	}
    }
}

void
PigPug::renameVariables(Subst& subst, const VariableRenaming& variableRenaming)
{
  for (int i = 0; i <= lastOriginalVariable; ++i)
    {
      Word& word = subst[i];
      int wordLength = word.size();
      for (int j = 0; j < wordLength; ++j)
	word[j] = variableRenaming[word[j]];
    }
}

void
PigPug::collectRangeVariables(const Subst& subst, NatSet& occursInRange)
{
  FOR_EACH_CONST(i, Subst, subst)
    {
      const Word& word = *i;
      FOR_EACH_CONST(j, Word, word)
	occursInRange.insert(*j);
    }
}

int
PigPug::extractUnifier(Subst& unifier)
{
  //
  //	We found a successful sequence of moves. Now we need to convert it into a unifier.
  //
  UnificandStack::const_iterator lhs = lhsStack.begin();
  UnificandStack::const_iterator rhs = rhsStack.begin();
  int lhsIndex = 0;
  int rhsIndex = 0;

  //
  //	Start with the identity substitution.
  //
  unifier.resize(lastOriginalVariable + 1);
  for (int i = 0; i <= lastOriginalVariable; ++i)
    {
      Word& word = unifier[i];
      word.resize(1);
      word[0] = i;
    }
  //
  //	Go through the moves recorded in path, generating and composing
  //	the corresponding substitutions.
  //
  FOR_EACH_CONST(i, Path, path)
    {
      int lhsVar = lhs->word[lhsIndex];
      int rhsVar = rhs->word[rhsIndex];
      int move = *i;

      switch (move & MOVES)
	{
	case INC_RHS:
	  {
	    //
	    //	lhsVar |-> rhsVar lhsVar
	    //
	    compose2(unifier, lhsVar, rhsVar);
	    ++rhsIndex;
	    break;
	  }
	case INC_LHS:
	  {
	    //
	    //	rhsVar |-> lhsVar rhsVar
	    //
	    compose2(unifier, rhsVar, lhsVar);
	    ++lhsIndex;
	    break;
	  }
	case INC_RHS | INC_LHS:
	  {
	    if (move & RHS_ASSIGN)
	      {
		//
		//	rhsVar |-> lhsVar
		//
		compose(unifier, rhsVar, lhsVar);
	      }
	    else
	      {
		//
		//	lhsVar |-> rhsVar (assuming they are distinct)
		//
		if (lhsVar != rhsVar)
		  compose(unifier, lhsVar, rhsVar);
	      }
	    ++lhsIndex;
	    ++rhsIndex;
	    break;
	  }
	default:
	  CantHappen("unknown move " << move);
	}
      if (move & PUSH_LHS)
	{
	  ++lhs;
	  lhsIndex = 0;
	}
      if (move & PUSH_RHS)
	{
	  ++rhs;
	  rhsIndex = 0;
	}
    }

  //
  //	Now we have to solve the final case of a bare variable against a unificand.
  //
  int lhsVar = lhs->word[lhsIndex];
  int rhsVar = rhs->word[rhsIndex];
  if (lhsIndex + 1 == lhs->word.length())
    {
      if (rhsIndex + 1 == rhs->word.length())
	{
	  //
	  //	Both sides reduced to a single variable.
	  //
	  if (lhsVar != rhsVar)
	    {
	      int lhsConstraint = constraintMap[lhsVar];
	      int rhsConstraint = constraintMap[rhsVar];
	      if (lhsConstraint == NONE)
		compose(unifier, lhsVar, rhsVar);
	      else if (rhsConstraint == NONE)
		compose(unifier, rhsVar, lhsVar);
	      else if (lhsConstraint == ELEMENT)
		compose(unifier, lhsVar, rhsVar);
	      else if (rhsConstraint == ELEMENT)
		compose(unifier, rhsVar, lhsVar);
	      else
		{
		  Assert(lhsConstraint == rhsConstraint, "constraint mismatch");
		  compose(unifier, lhsVar, rhsVar);
		}
	    }
	}
      else
	{
	  Assert(constraintMap[lhsVar] == NONE, "unexpected lhs constrained variable");
	  compose(unifier, lhsVar, rhs->word, rhsIndex);
	}
    }
  else
    {
      Assert(rhsIndex + 1 == rhs->word.length(), "path ended with neither side reduced to a single variable");
      Assert(constraintMap[rhsVar] == NONE, "unexpected rhs constrained variable");
      compose(unifier, rhsVar, lhs->word, lhsIndex);
    }
  //
  //	Now find out what variables show up in range of substitution.
  //
  NatSet occursInRange;
  collectRangeVariables(unifier, occursInRange);
  //
  //	Then look for original variables that map to something other than themselves, and
  //	if they occur in the range, rename them.
  //
  int nextVariableName = freshVariableStart;
  VariableRenaming variableRenaming(lastOriginalVariable + 1);
  for (int i = 0; i <= lastOriginalVariable; ++i)
    {
      const Word& word = unifier[i];
      if ((word.size() == 1 && word[0] == i) || !(occursInRange.contains(i)))
	variableRenaming[i] = i;
      else
	{
	  variableRenaming[i] = nextVariableName;
	  ++nextVariableName;
	}
    }
  renameVariables(unifier, variableRenaming);
  return nextVariableName;
}

#ifdef DUMP
void
PigPug::dumpWord(ostream& s, const Word& word)
{
  FOR_EACH_CONST(i, Word, word)
    s << "x" << *i << " ";
}

void
PigPug::dump(ostream& s, Subst& subst)
{
  int nrVariables = subst.size();
  for (int i = 0; i < nrVariables; ++i)
    {
      s << "  #" << i << " |->";
      FOR_EACH_CONST(j, PigPug::Word, subst[i])
	cout << " #" << *j;
      s << endl;
    }
}
#endif
