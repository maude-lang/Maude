/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for initial search phase.
//

int
PigPug::run(int result)
{
  //
  //	Pass result = OK for start, and result = FAIL for next try.
  //
  for (;;)
    {
      if (result == OK)
	result = firstMove();  // make the first available move from this position
      else
	{
	  if (result == FAIL || completed(result) == FAIL)
	    {
	      if (path.empty())
		break;  // no more backtracking possible; we're done
	      result = nextMove();  // backtrack and make the next available move
	    }
	  else
	    {
	      //
	      //	Result was something other than OK or FAIL and check for
	      //	completion didn't fail. Therefore we have a successful
	      //	path.
	      //
	      return result;  // must have completed
	    }
	}
    }
  return FAIL;
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
  if (!feasible())
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
  int result = rhsPeel();
  if (result != FAIL)
    return result;
  result = lhsPeel();
  if (result != FAIL)
    return result;
  return equate();
}

int
PigPug::nextMove()
{
  int previousMove = undoMove() & BASIC_MOVES;  // undoMove() never returns FINAL moves
  if (previousMove == EQUATE)
    return FAIL;  // last move was cancel or equate - no more moves for this state
  //
  //	Try the one or two remaining moves.
  //
  if (previousMove == RHS_PEEL)
    {
      //
      //	Last move was rhs peel so we can try lhs peel.
      //
      int result = lhsPeel();
      if (result != FAIL)
	return result;
    }
  if (equateOptimization && doublePeelPossible())
    {
      //
      //	Currently the equate optimization is only valid for
      //	linear equations where the sort structure cannot interfere
      //	with extra variables taking identity.
      //
      //	A double peel will get us to the same place as an equate 
      //	move, except we introduce a fresh variable which by taking
      //	identity will recover any equate solution. Such a double
      //	peel will already have been initiated by a left peel or
      //	right peel so we're done. We don't test for this in firstMove()
      //	because for a double peel a left peel or right peel have been
      //	made as the first move.
      //
      DebugInfo("skipping equate step");
      return false;
    }
  return equate(); // always final thing to try
}

bool
PigPug::doublePeelPossible()
{
  Unificand& lhs = lhsStack.back();
  int lhsVar = lhs.word[lhs.index];
  int lhsNextVar = lhs.word[lhs.index + 1];
  Unificand& rhs = rhsStack.back();
  int rhsVar = rhs.word[rhs.index];
  int rhsNextVar = rhs.word[rhs.index + 1];
  const ConstraintMap& constraintMap = constraintStack.back();

  return (constraintMap[lhsVar].isUnbounded() && constraintMap[rhsNextVar].isUnbounded()) ||
    (constraintMap[rhsVar].isUnbounded() && constraintMap[lhsNextVar].isUnbounded());
}

PigPug::Result
PigPug::cancel()
{
  //
  //	Check if the current variables on the lhs and rhs are equal.
  //
  Unificand& lhs = lhsStack.back();
  Unificand& rhs = rhsStack.back();
  if (lhs.word[lhs.index] != rhs.word[rhs.index])
    return FAIL;
  //
  //	If so, we cancel them.
  //
  ++(lhs.index);
  ++(rhs.index);
  path.append(EQUATE | CANCEL);
  //
  //	Then we check if one side of the equation has been
  //	reduced to a single variable.
  //
  if (lhs.index + 1 == lhs.word.length())
    return LHS_DONE;
  if (rhs.index + 1 == rhs.word.length())
    return RHS_DONE;
  return OK;
}

PigPug::Result
PigPug::rhsPeel()
{
  //
  //	Given
  //	  x xRest =? y yRest
  //	if x has can take at least two variables we make the step x |-> yx and
  //	then cancel the leading y instances to leave us with
  //	  x xRest =? yRest
  //	If we are strict left linear and x is unbounded then this must
  //	be the only occurrence of x; otherwise x may have arbitrary occurences
  //	in both lhs and rhs, so we may need to push modified lhs/rhs.
  //	Either way we consume one rhs variable.
  //
  const Unificand& lhs = lhsStack.back();
  int lhsVar = lhs.word[lhs.index];
  const ConstraintMap& constraintMap = constraintStack.back();
  int lhsUpperBound = constraintMap[lhsVar].getUpperBound();
  if (lhsUpperBound == 1)
    return FAIL;  // x can't take two variables
  //
  //	Make the move.
  //
  int move = RHS_PEEL;
  Unificand& rhs = rhsStack.back();
  int rhsVar = rhs.word[rhs.index];
  ++(rhs.index);  // consume y
  //
  //	Check if we need to make the substitution in yRest or xRest.
  //
  if (lhsUpperBound == 0 && (linearity & STRICT_LEFT_LINEAR))
    {
      //
      //	In a strict left-linear system, a lhs variable that is unbounded
      //	can only appear once, and we just handled the single occurrence.
      //
      Assert(!(checkUnificand2(rhsStack, lhsVar, rhsVar, 0)), "needed to do replacement in rhs");
      Assert(!(checkUnificand2(lhsStack, lhsVar, rhsVar, 1)), "needed to do replacement in lhs");
    }
  else
    {
      //
      //	Look for occurances of x in rhs from new rhs.index;
      //	if so we need to save the rhs and expand them.
      //
      if (checkUnificand2(rhsStack, lhsVar, rhsVar, 0))
	move |= PUSH_RHS;
      //
      //	Look for occurances of x in lhs from lhs.index + 1 (we already
      //	took care of the instance at lhs.index); if so we need to save
      //	the lhs and expand them.
      //
      if (checkUnificand2(lhsStack, lhsVar, rhsVar, 1))
	move |= PUSH_LHS;
    }
  //
  //	Check if x |-> yx changes bounds on x or y.
  //
  if (checkConstraintMap(lhsVar, rhsVar))
    move |= PUSH_CONSTRAINT_MAP;
  //
  //	Move is RHS_PEEL with any of PUSH_LHS, PUSH_RHS, PUSH_CONSTRAINT_MAP
  //
  path.append(move);
  //
  //	Did we get rhs down to one variable?
  //
  Unificand& newRhs = rhsStack.back();
  return (newRhs.index + 1 == newRhs.word.length()) ? RHS_DONE : OK;
}

PigPug::Result
PigPug::lhsPeel()
{
  //
  //	Given
  //	  x xRest =? y yRest
  //	if y is can take at least two variables we make the step y |-> xy
  //	and then cancel leading x instances to leave us with
  //	  xRest =? y yRest
  //	If we are strict left linear and y is unbounded than y cannot have
  //	occurrences in lhs; otherwise y may appear on either side.
  //	We may need to push modified lhs/rhs. Either way we consume one lhs variable.
  //
  Unificand& rhs = rhsStack.back();
  int rhsVar = rhs.word[rhs.index];
  const ConstraintMap& constraintMap = constraintStack.back();
  int rhsUpperBound = constraintMap[rhsVar].getUpperBound();
  if (rhsUpperBound == 1)
    return FAIL;  // y can't take two variables
  //
  //	Make the move.
  //
  int move = LHS_PEEL;
  Unificand& lhs = lhsStack.back();
  int lhsVar = lhs.word[lhs.index];
  ++(lhs.index);  // consume x
  //
  //	Check if we need to make the substitution in xRest.
  //
  if (rhsUpperBound == 0 && (linearity & STRICT_RIGHT_LINEAR))
    {
      //
      //
      //	In a strict right linear system, a rhs variable that is unbounded
      //	can only appear once, and we just handled the single occurance.
      //
      Assert(!(checkUnificand2(lhsStack, rhsVar, lhsVar, 0)), "needed to do replacement in lhs");
      Assert(!(checkUnificand2(rhsStack, rhsVar, lhsVar, 1)), "needed to do replacement in rhs");
    }
  else
    {
      //
      //	Look for occurances of x in rhs from rhs.index + 1
      //
      if (checkUnificand2(rhsStack, rhsVar, lhsVar, 1))
	move |= PUSH_RHS;
      //
      //	We weren't strict right linear but we could still be strict left linear.
      //
      if (rhsUpperBound == 0 && (linearity & STRICT_LEFT_LINEAR))
	{
	  //
	  //	In a strict left linear system, unbounded rhs variable cannot
	  //	appear in the lhs.
	  //
	  Assert(!(checkUnificand2(lhsStack, rhsVar, lhsVar, 0)), "needed to do replacement in lhs");
	}
      else
	{
	  //
	  //	Look for occurances of y in lhs from new lhs.index
	  //
	  if (checkUnificand2(lhsStack, rhsVar, lhsVar, 0))
	    move |= PUSH_LHS;
	}
    }
  //
  //	Check if y |-> xy changes bounds on y or x.
  //
  if (checkConstraintMap(rhsVar, lhsVar))
    move |= PUSH_CONSTRAINT_MAP;
  //
  //	Move is LHS_PEEL with any of PUSH_LHS, PUSH_RHS, PUSH_CONSTRAINT_MAP
  //
  path.append(move);
  //
  //	Did we get lhs down to one variable?
  //
  Unificand& newLhs = lhsStack.back();
  return (newLhs.index + 1 == newLhs.word.length()) ? LHS_DONE : OK;
}

PigPug::Result
PigPug::equate()
{
  Unificand& lhs = lhsStack.back();
  Unificand& rhs = rhsStack.back();
  int lhsVar = lhs.word[lhs.index];
  int rhsVar = rhs.word[rhs.index];
  const ConstraintMap& constraintMap = constraintStack.back();
  VariableConstraint lhsConstraint = constraintMap[lhsVar];
  VariableConstraint rhsConstraint = constraintMap[rhsVar];

  VariableConstraint meet(lhsConstraint);
  if (!(meet.intersect(rhsConstraint)))
    return FAIL;
  //
  //	Compatible constraints. Use up one variable from each side.
  //
  ++(lhs.index);
  ++(rhs.index);
  int move = EQUATE;
  //
  //	Need decide which way around to make the assignment.
  //
  if (rhsConstraint == meet)
    {
      //cout << "equate: lhsConstraint = " << lhsConstraint << "   rhsConstraint = " << rhsConstraint <<
      //	"   meet = " << meet << endl;
      //cout << "mapping " << lhsVar << " |-> " << rhsVar << endl;
      //
      //	rhsConstraint is at least as tight as lhsConstraint so we will
      //	make the assignment lhsVar |-> rhsVar
      //
      if ((linearity & STRICT_LEFT_LINEAR) && lhsConstraint.isUnbounded())
	{
	  //
	  //	In a strict left-linear system, a lhs variable that is unbounded
	  //	can only appear once, and we just used up the single occurance.
	  //
	  Assert(!(checkUnificand(lhsStack, lhsVar, rhsVar)), "needed to do replacement in lhs");
	  Assert(!(checkUnificand(rhsStack, lhsVar, rhsVar)), "needed to do replacement in rhs");
	}
      else
	{
	  //
	  //	Need to replace lhsVar with rhsVar in the remaining equation.
	  //
	  if (checkUnificand(lhsStack, lhsVar, rhsVar))
	    move |= PUSH_LHS;
	  if (checkUnificand(rhsStack, lhsVar, rhsVar))
	    move |= PUSH_RHS;
	}
    }
  else if (lhsConstraint == meet)
    {
      //cout << "equate: lhsConstraint = " << lhsConstraint << "   rhsConstraint = " << rhsConstraint <<
      //	"   meet = " << meet << endl;
      //cout << "mapping " << rhsVar << " |-> " << lhsVar << endl;
      //
      //	lhsConstraint is at least as tight as rhsConstraint so we will
      //	make the assignment rhsVar |-> lhsVar
      //
      move |= RHS_ASSIGN;
      if (checkUnificand(rhsStack, rhsVar, lhsVar))
	move |= PUSH_RHS;
      if ((linearity & STRICT_LEFT_LINEAR) && rhsConstraint.isUnbounded())
	{
	  //
	  //	In a strict left-linear system, a rhs varible that is unbounded
	  //	cannot appear in the lhs.
	  //
	  Assert(!(checkUnificand(lhsStack, rhsVar, lhsVar)), "needed to do replacement in lhs");
	}
      else
	{
	  if (checkUnificand(lhsStack, rhsVar, lhsVar))
	    move |= PUSH_LHS;
	}
    }
  else
    {
      //cout << "equate: incompable constraints" << endl;

      DebugAdvisory("*** Incomparable constraints ***");
      //
      //	Compatible but incomparable constraints.
      //
      //	This can only happen when the smaller bounded variable can
      //	take identity and the larger bounded variable cannot, which
      //	is a rather bizarre situation but possible with weird sorting
      //	or a one-sided identity.
      //
      //	We will need to update the constraintMap whichever way we
      //	decide to make the assignment.
      //
      constraintStack.push_back(constraintMap);
      ConstraintMap& newConstraintMap = constraintStack.back();
      move |= PUSH_CONSTRAINT_MAP;
      
      if (rhsConstraint.isUnbounded())
	{
	  //
	  //	Both variables cannot be unbounded. If rhsVar is unbounded then
	  //	lhsVar is bounded and we put rhsVar |-> lhsVar
	  //
	  move |= RHS_ASSIGN;
	  if (checkUnificand(rhsStack, rhsVar, lhsVar))
	    move |= PUSH_RHS;
	  if ((linearity & STRICT_LEFT_LINEAR))
	    {
	      //
	      //	In a strict left-linear system, a rhs variable that is unbounded
	      //	cannot appear in the lhs.
	      //
	      Assert(!(checkUnificand(lhsStack, rhsVar, lhsVar)), "needed to do replacement in lhs");
	    }
	  else
	    {
	      if (checkUnificand(lhsStack, rhsVar, lhsVar))
		move |= PUSH_LHS;
	    }
	  newConstraintMap[lhsVar] = meet;
	}
      else
	{
	  //
	  //	Otherwise the default is to put lhsVar |-> rhsVar
	  //
	  if ((linearity & STRICT_LEFT_LINEAR) && lhsConstraint.isUnbounded())
	    {
	      //
	      //	In a strict left-linear system, a lhs variable that is unbounded
	      //	can only appear once, and we just used up the single occurance.
	      //
	      Assert(!(checkUnificand(lhsStack, lhsVar, rhsVar)), "needed to do replacement in lhs");
	      Assert(!(checkUnificand(rhsStack, lhsVar, rhsVar)), "needed to do replacement in rhs");
	    }
	  else
	    {
	      //
	      //	Need to replace lhsVar with rhsVar in the remaining equation.
	      //
	      if (checkUnificand(lhsStack, lhsVar, rhsVar))
		move |= PUSH_LHS;
	      if (checkUnificand(rhsStack, lhsVar, rhsVar))
		move |= PUSH_RHS;
	    }
	  newConstraintMap[rhsVar] = meet;
	}
    }
  path.append(move);

  Unificand& newLhs = lhsStack.back();
  if (newLhs.index + 1 == newLhs.word.length())
    return LHS_DONE;  // lhs reduced to a single variable

  Unificand& newRhs = rhsStack.back();
  if (newRhs.index + 1 == newRhs.word.length())
    return RHS_DONE;  // rhs reduced to a single variable

  return OK;
}

int
PigPug::undoMove()
{
  //
  //	Undo last move.
  //
  int pos = path.size() - 1;
  int move = path[pos];
  if (move & FINAL)
    {
      //
      //	Final moves are forced, and they are notional -
      //	they don't update anything except possibly the
      //	constraint stack.
      //
      if (move & PUSH_CONSTRAINT_MAP)
	constraintStack.pop_back();
      //
      //	Get next move to undo.
      //
      --pos;
      move = path[pos];
    }
  //
  //	Dispose of the move we're undoing
  //
  path.resize(pos);
  //
  //	Possible moves in left linear case:
  //	  RHS_PEEL
  //	  LHS_PEEL
  //	  LHS_PEEL | PUSH_RHS
  //	  LHS_PEEL | RHS_PEEL with any combination of RHS_ASSIGN, PUSH_LHS, PUSH_RHS
  //
  //	In the general case the following is also possible:
  //	  RHS_PEEL with any combination of PUSH_LHS, PUSH_RHS, PUSH_CONSTRAINT_MAP
  //	  LHS_PEEL with any combination of PUSH_LHS, PUSH_RHS, PUSH_CONSTRAINT_MAP
  //
  //	In the case that LHS (RHS) is both incremented and pushed, the incrementing
  //	happens before the push, and thus for undo the decrementing must happen after
  //	the pop.
  //
  if (move & PUSH_LHS)
    lhsStack.pop_back();
  if (move & LHS_PEEL)
    --(lhsStack.back().index);

  if (move & PUSH_RHS)
    rhsStack.pop_back();
  if (move & RHS_PEEL)
    --(rhsStack.back().index);

  if (move & PUSH_CONSTRAINT_MAP)
    constraintStack.pop_back();

  return move;
}
