/*

    This file is part of the Maude 3 interpreter.

    Copyright 2021 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for expanding selections of variables to set to empty.
//

bool
WordLevel::systemLinear()
{
  //
  //	Variables in an unsafe binding are considered pseudo-constrained.
  //	They will either be eliminated by an empty assignment or constrained
  //	by the pass that removed the unsafe designation before PigPug runs.
  //
  NatSet pseudoConstrained;
  for (int i : unsafeAssignments)
    {
      for (int index : partialSolution[i])
	pseudoConstrained.insert(index);
    }
  //
  //	Check if no unconstrained variable appears more than once in the
  //	system of equations. Pseudo-constrained variables are treated as
  //	being constrained for this computation.
  //
  NatSet seenVariables;
  for (const Equation& e : unsolvedEquations)
    {
      if (!(e.lhs.empty()))  // simplification may have disposed of some equations
	{
	  for (int index : e.lhs)
	    {
	      if (constraintMap[index].isUnbounded() && !(pseudoConstrained.contains(index)))
		{
		  if (seenVariables.contains(index))
		    return false;
		  seenVariables.insert(index);
		}
	    }
	  for (int index : e.rhs)
	    {
	      if (constraintMap[index].isUnbounded() && !(pseudoConstrained.contains(index)))
		{
		  if (seenVariables.contains(index))
		    return false;
		  seenVariables.insert(index);
		}
	    }
	}
    }
  return true;
}

void
WordLevel::computePinches(const Word& pincher, const Word& pinched, NatSet& pinchedVariables)
{
  int pincherSize = pincher.size();
  int pinchedSize = pinched.size();
  
  if (!(constraintMap[pincher[0]].isUnbounded()))
    {
      //
      //	Left end of pincher is a constrained variable. This can pinch
      //	from the left end until it hits a variable that can't take empty
      //	or the right end variable.
      //
       for (int i = 0; i < pinchedSize - 1; ++i)
	{
	  int index = pinched[i];
	  if (!(constraintMap[index].canTakeEmpty()))
	    break;  // can't pinch past this variable
	  pinchedVariables.insert(index);
	}
    }

  if (!(constraintMap[pincher[pincherSize - 1]].isUnbounded()))
    {
      //
      //	Right end of pincher is a constrained variable. This can pinch
      //	from the right end until it hits a variable that can't take empty
      //	or the left end variable.
      //
      int pinchedSize = pinched.size();
      for (int i = pinchedSize - 1; i > 0; --i)
	{
	  int index = pinched[i];
	  if (!(constraintMap[index].canTakeEmpty()))
	    break;  // can't pinch past this variable
	  pinchedVariables.insert(index);
	}
    }

  for (int i = pincherSize - 1; i > 0; --i)
    {
      if (!(constraintMap[pincher[i]].isUnbounded()) &&
	  !(constraintMap[pincher[i - 1]].isUnbounded()))
	{
	  //
	  //	Adjacent pair of constrained variables is assumed to
	  //	pinch everything other than the ends (which they may
	  //	have already pinched above.
	  //
	  for (int j = pinchedSize - 2; j > 0; --j)
	    pinchedVariables.insert(pinched[j]);
	  break;
	}
    }
}

void
WordLevel::determinePinchedVariables(NatSet& pinchedVariables)
{
  //
  //	A variable is considered to be "pinched" if
  //	it can be hemmed in by constrained variables in
  //	the other unificand and forced to identity.
  //
  //	We don't have a precise operational way
  //	to determine pinched variables so we compute
  //	an over-approximation.
  //
  for (const Equation& e : unsolvedEquations)
    {
      if (!(e.lhs.empty()))  // simplification may be disposed of some equations
	{
	  computePinches(e.lhs, e.rhs, pinchedVariables);
	  computePinches(e.rhs, e.lhs, pinchedVariables);
	}
    }
  //
  //	Unsafe assignments can pinch variables since they
  //	don't respect constraints on the assumption they will
  //	be resolved by collapse.
  //
  for (int i : unsafeAssignments)
    {
      for (int index : partialSolution[i])
	pinchedVariables.insert(index);
    }
}

void
WordLevel::chooseVariablesToSelectFrom()
{
  int nrOriginalVariables = partialSolution.size();

  if (identityOptimizations && systemLinear())
    {
      //
      //	Actually an over-approximation.
      //
      NatSet pinchedVariables;
      determinePinchedVariables(pinchedVariables);
      //
      //	Only variables that are potentially pinched will
      //	be considered for identity assignment. Identity
      //	assignment to a variable X that can't be pinched
      //	must produce a unifier that is less general than
      //	one where the X takes a fresh variable #1 and
      //	the #1 is also added to the assignment for some
      //	Y variable in the other unificand without disturbing
      //	the other assignments.
      //
      //	This can happen because Y doesn't appear anywhere
      //	else and Y is guaranteed to exist because X can't
      //	be pinched between two constrained variables or
      //	a constrained variable and a word end in the other
      //	unificand.
      //
      //	But there is also the possibility of an assignment,
      //	say A |-> X Y Z, pinching X Y Z if A is constrained.
      //	The assignment is made because we are assuming potential
      //	collapses which must be explored.
      //
      for (int i = 0; i < nrOriginalVariables; ++i)
	{
	  Word& binding = partialSolution[i];
	  if (constraintMap[i].canTakeEmpty() &&
	      pinchedVariables.contains(i) &&
	      binding.size() == 1 && binding[0] == i)
	    {
	      DebugInfo("x" << i << " can take empty, is potentially pinched and is free");
	      idVariables.append(i);
	    }
	}
    }
  else
    {
      //
      //	All variables that can take the identity and are free
      //	must be considered. This will usually lead to an
      //	explosion of redundant unifiers.
      //
      for (int i = 0; i < nrOriginalVariables; ++i)
	{
	  Word& binding = partialSolution[i];
	  if (constraintMap[i].canTakeEmpty() && binding.size() == 1 && binding[0] == i)
	    {
	      DebugInfo("x" << i << " can take empty and is free");
	      idVariables.append(i);
	    }
	}
    }
}

WordLevel::ResultPair
WordLevel::trySelection()
{
  chooseVariablesToSelectFrom();
  int nrIdVariables = idVariables.size();
  nrSelections = (1 << nrIdVariables) - 1;
  DebugInfo("idVariables.size() = " << nrIdVariables <<
	    "  nrSelections = " << nrSelections);
  return (nrIdVariables == 0) ? ResultPair(FAILURE, nullptr) : exploreSelections();
}

WordLevel::ResultPair
WordLevel::exploreSelections()
{
  ++selection;
  if (selection > nrSelections)
    return ResultPair(FAILURE, nullptr);
  DebugInfo("-------- Trying selection " << selection << " --------");
  //
  //	Count the number of equations that survived simplification.
  //
  int equationCount = 0;
  for (const Equation& i : unsolvedEquations)
    {
      if (!(i.lhs.empty()))
	++equationCount;
    }
  //
  //	Make a selection WordLevel.
  //
  std::unique_ptr<WordLevel> newLevel(new WordLevel(SELECTION,
						    partialSolution.size(),
						    equationCount,
						    identityOptimizations,
						    // passing this pointer is ugly since
						    // we're owned by a unique_ptr<>
						    this));
  newLevel->constraintMap = constraintMap;  // deep copy
  {
    int index = 0;
    int bitMask = 1;
    Word emptyWord;
    int nrIdVariables = idVariables.size();
    int nrVariables = partialSolution.size();
    for (int i = 0; i < nrVariables; ++i)
      {
	if (index < nrIdVariables && i == idVariables[index])
	  {
	    int oldBitMask = bitMask;
	    ++index;
	    bitMask <<= 1;
	    if (selection & oldBitMask)
	      {
		//
		//	We're going to null variable i because it is part of
		//	our selection.
		//
		DebugInfo("setting x" << i << " to null");
		newLevel->addAssignment(i, emptyWord);
		continue;
	      }
	  }
	newLevel->addAssignment(i, partialSolution[i]);
      }
  }
  {
    int equationIndex = 0;
    int nrEquations = unsolvedEquations.size();
    for (int i = 0; i < nrEquations; ++i)
      {
	Equation& e = unsolvedEquations[i];
	if (!(e.lhs.empty()))
	  {
	    newLevel->addEquation(equationIndex, e.lhs, e.rhs);
	    ++equationIndex;
	  }
      }
  }
  //
  //	Creating a selection level cannot introduce incompleteness itself.
  //
  return ResultPair(SUCCESS, std::move(newLevel));
}

bool
WordLevel::insertCombination(const Subst& substitution)
{
  Assert(levelType == INITIAL, "levelType = " << levelType);
  int code = 0;
  int bitMask = 1;
  int index = 0;
  int nrIdVariables = idVariables.size();
  int nrOriginalVariables = partialSolution.size();
  for (int i = 0; i < nrOriginalVariables; ++i)
    {
      if (index < nrIdVariables && i == idVariables[index])
	{
	  //
	  //	One of the variables we are selecting over.
	  //
	  if (substitution[i].empty())
	    code |= bitMask;
	  ++index;
	  bitMask <<= 1;
	}
    }
  return finalCombinations.insert(code).second;
}
