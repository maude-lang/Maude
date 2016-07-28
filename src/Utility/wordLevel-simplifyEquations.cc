//
//	Code for simplfying equations in a level.
//

WordLevel::Result
WordLevel::simplifyEquations()
{
  //
  //	Try to simplify equations, expanding any changes the partial solution as we go.
  //	Return values:
  //	  DONE : no changes to partial solution
  //	  CHANGED : partial solution changed
  //	  FAIL : variable constraint clash or occur-check failure
  //
  bool changed = false;
  int nrEquations = unsolvedEquations.size();
  for (int i = 0; i < nrEquations; ++i)
    {
      Result result = simplifyEquation(unsolvedEquations[i]);
      if (result == FAIL)
	return FAIL;
      if (result == CHANGED)
	{
	  //
	  //	Partial solution may now be denormalized.
	  //
	  changed = true;
	  if (!fullyExpandAssignments())
	    return FAIL;
	}
    }
  return changed ? CHANGED : DONE;
}

WordLevel::Result
WordLevel::simplifyEquation(Equation& equation)
{
  //
  //	Try to simplify equation by left and right cancellation.
  //	Return values:
  //	  DONE : either no cancellation or cancellation didn't change partial solution.
  //	  CHANGED : cancellation and partial solution changed
  //	  FAIL : variable constraint clash or occur-check failure
  //
  int nrLhsVariables = equation.lhs.size();
  if (nrLhsVariables == 0)
    return DONE;  // equation already simplified away

  bool changed = false;
  //
  //	First we substitute in any assignments to the lhs and rhs.
  //
  Word newLhs;
  expandWord(newLhs, equation.lhs);
  //cout << "lhs old: "; dumpWord(cout, equation.lhs);
  //cout << "  lhs new: ";  dumpWord(cout, newLhs); cout << endl;

  Word newRhs;
  expandWord(newRhs, equation.rhs);
  //cout << "rhs old: "; dumpWord(cout, equation.rhs);
  //cout << "  rhs new: ";  dumpWord(cout, newRhs); cout << endl;

  //
  //	Now we work from the ends, cancelling equal variables and
  //	variables that are both constrained.
  //
  int lhsLeftCursor = 0;
  int lhsRightCursor = newLhs.size() - 1;

  int rhsLeftCursor = 0;
  int rhsRightCursor = newRhs.size() - 1;
  //
  //	Left cancellation
  //
  for (;;)
    {
      Result result = cancel(newLhs[lhsLeftCursor], newRhs[rhsLeftCursor]);
      if (result == FAIL)
	return FAIL;  // variable theory clash
      if (result == DONE)
	break;  // no cancellation
      //
      //	We cancelled.
      //
      ++lhsLeftCursor;
      ++rhsLeftCursor;
      //cout << "cancelled at left end " <<  lhsLeftCursor << " ~ " << rhsLeftCursor << endl;

      if (result == CHANGED)
	{
	  //
	  //	We know that the only possible change is a constrained variable to
	  //	constrained variable assignment, so we can do a cheap update on the remainders.
	  //
	  changed = true;
	  updateRemainder(newLhs, lhsLeftCursor, lhsRightCursor);
	  updateRemainder(newRhs, rhsLeftCursor, rhsRightCursor);
	}

      result = checkForSingleton(newLhs, lhsLeftCursor, lhsRightCursor,
				 newRhs, rhsLeftCursor, rhsRightCursor);
      if (result == FAIL)
	return FAIL;
      if (result != CONTINUE)
	{
	  //
	  //	Singleton case - equation disappears.
	  //
	  equation.lhs.clear();
	  equation.rhs.clear();
	  return changed ? CHANGED : result;
	}
    }
  //
  //	Right cancellation
  //
  for (;;)
    {
      Result result = cancel(newLhs[lhsRightCursor], newRhs[rhsRightCursor]);
      if (result == FAIL)
	return FAIL;  // variable theory clash
      if (result == DONE)
	break;  // no cancellation
      //
      //	We cancelled.
      //
      --lhsRightCursor;
      --rhsRightCursor;
      //cout << "cancelled at right end " <<  lhsRightCursor << " ~ " << rhsRightCursor << endl;

      if (result == CHANGED)
	{
	  //
	  //	We know that the only possible change is a constrained variable to
	  //	constraint variable assignment, so we can do a cheap update on the remainders.
	  //
	  changed = true;
	  updateRemainder(newLhs, lhsLeftCursor, lhsRightCursor);
	  updateRemainder(newRhs, rhsLeftCursor, rhsRightCursor);
	}

      result = checkForSingleton(newLhs, lhsLeftCursor, lhsRightCursor,
				 newRhs, rhsLeftCursor, rhsRightCursor);
      if (result == FAIL)
	return FAIL;
      if (result != CONTINUE)
	{
	  //
	  //	Singleton case - equation disappears.
	  //
	  equation.lhs.clear();
	  equation.rhs.clear();
	  return changed ? CHANGED : result;
	}
    }
  //
  //	If we reach here, we have fully cancelled and both sides are non-singleton.
  //
  copyBack(equation.lhs, newLhs, lhsLeftCursor, lhsRightCursor);
  copyBack(equation.rhs, newRhs, rhsLeftCursor, rhsRightCursor);
  return changed ? CHANGED : DONE;
}

void
WordLevel::expandWord(Word& newWord, const Word& oldWord)
{
  FOR_EACH_CONST(i, Word, oldWord)
    append(newWord, partialSolution[*i]);
}

void
WordLevel::append(Word& newWord, const Word& word)
{
  FOR_EACH_CONST(i, Word, word)
    newWord.append(*i);
}

void
WordLevel::updateRemainder(Word& word, int leftCursor, int rightCursor)
{
  for (; leftCursor <= rightCursor; ++leftCursor)
    word[leftCursor] = partialSolution[word[leftCursor]][0];  // if variable got a binding, it will be another variable
}

WordLevel::Result
WordLevel::cancel(int lhsVar, int rhsVar)
{
  //cout << "cancel(x" << lhsVar << ", x" << rhsVar << ")" << endl;

  //
  //	Try to cancel lhsVar and rhsVar by unifying them.
  //	Return values:
  //	  DONE : no cancellation possible
  //	  CHANGED : cancellation caused change to partial solution
  //	  FAIL : incompatible constrained variables
  //	  CONTINUE : cancellation, no change to partial solution
  //	
  //
  if (lhsVar == rhsVar)
    return CONTINUE;  // cancel variable
  else
    {
      //
      //	If both variables are constrained we cancel via an assignment or we have a clash.
      //
      if (constraintMap[lhsVar] != NONE && constraintMap[rhsVar] != NONE)
	return unifyVariables(lhsVar, rhsVar);  // we never return DONE since lhsVar != rhsVar in this branch
    }
  return DONE;
}


WordLevel::Result
WordLevel::checkForSingleton(const Word& newLhs, int lhsLeftCursor, int lhsRightCursor,
			     const Word& newRhs, int rhsLeftCursor, int rhsRightCursor)
{
  //
  //	Handle cases when one or both unificands has been cancelled down to a single variable.
  //	Return values:
  //	  DONE : handled singleton case, but partial solution didn't change (equal variables)
  //	  CHANGED : handled singleton, partial solution changed
  //	  FAIL : handled singleton case, variable constraint clash or occur-check failure
  //	  CONTINUE : non-singleton equation
  //
  if (lhsLeftCursor == lhsRightCursor)
    {
      if (rhsLeftCursor == rhsRightCursor)
	return unifyVariables(newLhs[lhsLeftCursor], newRhs[rhsLeftCursor]);
      else
	return makeAssignment(newLhs[lhsLeftCursor], newRhs, rhsLeftCursor, rhsRightCursor);
    }
  else if (rhsLeftCursor == rhsRightCursor)
    return makeAssignment(newRhs[rhsLeftCursor], newLhs, lhsLeftCursor, lhsRightCursor);

  return CONTINUE;
}

WordLevel::Result
WordLevel::unifyVariables(int lhsVar, int rhsVar)
{
  //cout << "unifyVariables(x" << lhsVar << ", x" << rhsVar << ")" << endl;
  //
  //	We try to unify two variables by assigning one to the other.
  //	We always want to assign from the least constrained variable to the most constrained variable.
  //	Return values:
  //	  DONE : equal variables
  //	  CHANGED : assignment made
  //	  FAIL : incompatible constrained variables
  //
  if (lhsVar == rhsVar)
    return DONE;

  int lhsConstraint = constraintMap[lhsVar];
  int rhsConstraint = constraintMap[rhsVar];
  if (lhsConstraint == NONE ||
      (lhsConstraint == PigPug::ELEMENT && rhsConstraint != NONE) ||
      lhsConstraint == rhsConstraint)
    {
      //
      //	lhsVar |-> rhsVar
      //
      Word& assignment = partialSolution[lhsVar];
      assignment.resize(1);
      assignment[0] = rhsVar;
      return CHANGED;
    }
  if (rhsConstraint == NONE || rhsConstraint == PigPug::ELEMENT)
    {
      //
      //	rhsVar |-> lhsVar
      //
      Word& assignment = partialSolution[rhsVar];
      assignment.resize(1);
      assignment[0] = lhsVar;
      return CHANGED;
    }
  //
  //	Incompatible constraints.
  //
  return FAIL;
}

WordLevel::Result
WordLevel::makeAssignment(int variable, const Word& source, int leftCursor, int rightCursor)
{
  //
  //	Assign
  //	  variable |-> source[leftCursor]...source[rightCursor]
  //	We require
  //	  leftCursor < rightCursor
  //	This only succeeds if variable is unconstrained.
  //	Since exactly one unconstrained variable can be assigned to during an equation
  //	simplification (this is it), and all bound variables were substituted out
  //	at the start of simplification, we assumed that variable unbound (actually
  //	bound to itself using curren Subst representation.
  //	Return values:
  //	  CHANGED : assignment made
  //	  FAIL : variable is constrained, or occur-check failure
  //
  int variableConstraint = constraintMap[variable];
  if (variableConstraint != NONE)
    return FAIL;

  int nrVariables = rightCursor - leftCursor + 1;
  Assert(nrVariables >= 2, "nrVariables = " << nrVariables);

  Word& destination = partialSolution[variable];
  Assert(destination.size() == 1 && destination[0] == variable, "variable " << variable << " not bound to itself");

  destination.resize(nrVariables);
  for (int i = 0; i < nrVariables; ++i, ++leftCursor)
    {
      int var = source[leftCursor];
      if (var == variable)
	return FAIL;   // occur-check failure
      destination[i] = var;
    }
  return CHANGED;
}

void
WordLevel::copyBack(Word& destination, const Word& source, int leftCursor, int rightCursor)
{
  int nrVariables = rightCursor - leftCursor + 1;
  destination.resize(nrVariables);
  for (int i = 0; i < nrVariables; ++i, ++leftCursor)
    destination[i] = source[leftCursor];
}
