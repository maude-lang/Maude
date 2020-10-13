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
//      Implementation for class WordSystem.
//
#include "macros.hh"
#include "vector.hh"
#include "wordLevel.hh"
#include "indent.hh"
#include "natSet.hh"

//	our stuff
#include "wordLevel-null.cc"
#include "wordLevel-simplifyAssignments.cc"
#include "wordLevel-simplifyEquations.cc"
#include "wordLevel-selections.cc"
#include "wordLevel-feasible.cc"

WordLevel::WordLevel(LevelType levelType,
		     int nrVariables,
		     int nrEquations,
		     bool identityOptimizations,
		     WordLevel* parent)
  : levelType(levelType),
    identityOptimizations(identityOptimizations),
    constraintMap(nrVariables),
    partialSolution(nrVariables),
    unsolvedEquations(nrEquations),
    parent(parent)
{
  //
  //	Unbound variables must be bound to themselves;
  //	by default variables are unconstrained.
  //
  for (int i = 0; i < nrVariables; ++i)
    partialSolution[i].append(i);
  chosenEquation = NOT_YET_CHOSEN;
  pigPug = 0;
  incompletenessFlag = 0;
  selection = 0;
}

WordLevel::~WordLevel()
{
  delete pigPug;
}

WordLevel::ResultPair
WordLevel::findNextPartialSolution()
{
  if (selection != 0)
    {
      //
      //	This level is finished with PigPug; but we still need to
      //	explore selections.
      //
      return exploreSelections();
    }
  if (chosenEquation == NOT_YET_CHOSEN)
    {
      //
      //	Must be first call.
      //
      if (!simplify())
	{
	  //
	  // occurs-check failure or constraint clash during simplification.
	  //
	  return ResultPair(FAILURE, 0);
	}
      //
      //	Need to check that all assignments are legal without
      //	collapse and all equations are feasible without collapse.
      //
      //	If we're a PIGPUG level then:
      //	(1) PigPug only made collapse free legal assignments.
      //	(2) Any assignment expansions were checked.
      //	(3) Any assignments produced by equation simplification were checked.
      //	(4) Any equation simplfications were checked.
      //
      //	If we are an INITIAL or SELECTION level:
      //	(1) We can't assume every equation and assignment were checked.
      //	(2) Any checks were done assuming collapse was allowed.
      //	Therefore we need to do a full check.
      //
      if (levelType != PIGPUG && !levelFeasibleWithoutCollapse())
	{
	  //
	  //	If we get a failure there is no point solving with PigPug.
	  //	However if we are in the INITIAL level we may be able to
	  //	make a SELECTION level.
	  //
	  return levelType == INITIAL ? trySelection() : ResultPair(FAILURE, 0);
	}
      if (levelType == SELECTION && !(parent->insertCombination(partialSolution)))
	{
	  //
	  //	We're a SELECTION level and the set of variables set to empty
	  //	that we have after simplification has already been tried so
	  //	don't generate duplicate solutions.
	  //
	  return ResultPair(FAILURE, 0);
	}
      //
      //	Need to chose an equation and create a PigPug.
      //
      int linearity = chooseEquation();
      if (chosenEquation == NONE)
	return ResultPair(SUCCESS, 0);  // no equations left to solve
      //
      //	FIXME: If strictLeftLinear == false and we have more that one
      //	equation, we might want to do a Diophantine check here, because
      //	otherwise we are likely going to hit incompleteness.
      //
      makePigPug(linearity);
    }
  if (pigPug == 0)
    {
      //
      //	We didn't make a PigPug; therefore we can't have any
      //	unsolved equations and we must have return the only solution
      //	already. However if we are the INITIAL level we need to try
      //	selecting varibles to take empty.
      //
      if (levelType == INITIAL)
	return trySelection();
      else
	return ResultPair(FAILURE, 0);
    }
  //
  //	Get next PigPug solution; create a new WordLevel.
  //
  Subst unifier;
  ConstraintMap newConstraintMap;
  PigPug::ResultPair result = pigPug->getNextUnifier(unifier, newConstraintMap);
  int nextFreshVariable = result.second;
  if (nextFreshVariable == NONE)
    {
      //
      //	No more solutions from PigPug. However if we are the INITIAL
      //	level we need to try selecting variables to take empty. PigPug
      //	maybe have returned an incompleteness flag and we have to keep
      //	that to return to the caller.
      //
      if (levelType == INITIAL)
	{
	  ResultPair r = trySelection();
	  r.first |= result.first;  // preserve any incompleteness flag from last PigPug return
	  return r;
	}
      else
	return ResultPair(result.first, 0);  // failure
    }
  WordLevel* next = makeNewLevel(unifier, newConstraintMap, nextFreshVariable);
  return ResultPair(result.first, next);
}

bool
WordLevel::simplify()
{
  //
  //	We start by simplifying the partial solution.
  //
  if (levelType == INITIAL && !handleInitialOccursCheckFailure())
    return false;
  if (!fullyExpandAssignments())
    return false;
  //
  //	Then we simplify the equations until there is no change in the partial solution.
  //
  for (;;)
    {
      Result result = simplifyEquations();
      if (result == FAIL)
	return false;
      if (result == DONE)
	break;
    }
  return true;
}

WordLevel*
WordLevel::makeNewLevel(const Subst& unifier,
			const ConstraintMap& newConstraintMap,
			int nextFreshVariable)
{
  //
  //	We have a PigPug solution - need to copy old stuff and new solution
  //	into a new WordLevel object.
  //
  int equationCount = 0;
  for (const Equation& i : unsolvedEquations)
    {
      if (!(i.lhs.empty()))
	++equationCount;
    }
  WordLevel* newLevel = new WordLevel(PIGPUG,
				      nextFreshVariable,
				      equationCount - 1,
				      identityOptimizations);
  //
  //	Copy in old information and unifier.
  //
  newLevel->constraintMap = newConstraintMap;  // deep copy
  int nrVariables = partialSolution.size();
  for (int i = 0; i < nrVariables; ++i)
    {
      DebugInfo("x" << i << " = " << unifier[i] << " newConstraint = " << newConstraintMap[i]);
      const Word& u = unifier[i];
      if (u.size() != 1 || u[0] != i)
	newLevel->addAssignment(i, u);
      else
	newLevel->addAssignment(i, partialSolution[i]);
    }
  int equationIndex = 0;
  int nrEquations = unsolvedEquations.size();
  for (int i = 0; i < nrEquations; ++i)
    {
      if (i != chosenEquation)
	{
	  Equation& e = unsolvedEquations[i];
	  if (!(e.lhs.empty()))
	    {
	      newLevel->addEquation(equationIndex, e.lhs, e.rhs);
	      ++equationIndex;
	    }
	}
    }
  return newLevel;
}

int
WordLevel::chooseEquation()
{
  //
  //	Chosen an unsolved equation and set chosenEquation to it true.
  //	If there are no unsolved equations set chosenEquation to NONE.
  //	Returns NONLINEAR, STRICT_LEFT_LINEAR, or LINEAR depending on the
  //	chosen equation.
  //
  chosenEquation = NONE;  // if we don't find one
  int nrEquations = unsolvedEquations.size();
  for (int i = 0; i < nrEquations; ++i)
    {
      Equation& e = unsolvedEquations[i];
      if (!(e.lhs.empty()))
	{
	  //
	  //	Examine unconstrained variables.
	  //
	  NatSet lhsOccurs;
	  NatSet lhsNonlinear;
	  checkUnconstrainedVariables(e.lhs, lhsOccurs, lhsNonlinear);
	  NatSet rhsOccurs;
	  NatSet rhsNonlinear;
	  checkUnconstrainedVariables(e.rhs, rhsOccurs, rhsNonlinear);
	  if (lhsOccurs.disjoint(rhsOccurs))
	    {
	      //
	      //	No unbounded variable occurs in both the lhs and rhs.
	      //
	      if (lhsNonlinear.empty())
		{
		  //
		  //	We found a strict left-linear equation so choose it.
		  //
		  chosenEquation = i;
		  return rhsNonlinear.empty() ? LINEAR : STRICT_LEFT_LINEAR;
		}
	      if (rhsNonlinear.empty())
		{
		  //
		  //	We found a strict right-linear equation so flip it into a
		  //	strict left-linear equation and choose it.
		  //
		  e.lhs.swap(e.rhs);
		  chosenEquation = i;
		  return STRICT_LEFT_LINEAR;
		}
	      chosenEquation = i;  // prefer an equation with disjoint lhs/rhs variable sets
	    }
	  if (chosenEquation == NONE)
	    chosenEquation = i;  // we'll take this one if we don't see anything better
	}
    }
  return NONLINEAR;  // we didn't find a strict left-linear equation
}

void
WordLevel::checkUnconstrainedVariables(const Word& word, NatSet& occurs, NatSet& nonlinear)
{
  //
  //	We look at the contents of a word and compile the set of unbounded
  //	variables that occur and the subset of unbounded variables that occur
  //	more than once.
  //
  for (int index : word)
    {
      if (constraintMap[index].isUnbounded())
	{
	  if (occurs.contains(index))
	    nonlinear.insert(index);
	  else
	    occurs.insert(index);
	}
    }
}

void
WordLevel::makePigPug(int linearity)
{
  Equation& e = unsolvedEquations[chosenEquation];
  int nrVariables = partialSolution.size();
  pigPug = new PigPug(e.lhs,
		      e.rhs,
		      constraintMap,
		      nrVariables - 1,
		      nrVariables,
		      linearity,
		      false /* need to think about this identityOptimizations && (linearity == LINEAR)*/);
  //dump(cerr, 0);
}

void
WordLevel::dump(ostream& s, int indentLevel)
{
  s << Indent(indentLevel) << "begin{WordLevel}" << endl;
  {
    s << Indent(indentLevel + 1) << "constraintMap = " << endl;
    int nrVariables = constraintMap.size();
    for (int i = 0; i < nrVariables; ++i)
      {
	s << Indent(indentLevel + 2) << "X" << i << " : " <<
	  constraintMap[i] << endl;
      }
  }
  {
    s << Indent(indentLevel + 1) << "partialSolution = " << endl;
    int nrVariables = partialSolution.size();
    for (int i = 0; i < nrVariables; ++i)
      {
	s << Indent(indentLevel + 2) << "x" << i << " |-> ";
	dumpWord(s, partialSolution[i]);
	s << endl;
      }
  }
  {
    s << Indent(indentLevel + 1) << "unsolvedEquations = " << endl;
    int nrEquations = unsolvedEquations.size();
    for (int i = 0; i < nrEquations; ++i)
      {
	s << Indent(indentLevel + 2) << "e" << i << " : ";
	if (unsolvedEquations[i].lhs.empty())
	  s << "(cancelled)" << endl;
	else
	  {
	    dumpWord(s, unsolvedEquations[i].lhs);
	    s << " =?  ";
	    dumpWord(s, unsolvedEquations[i].rhs);
	    s << endl;
	  }
      }
  }
  s << Indent(indentLevel) << "end{WordLevel}" << endl;
}

void
WordLevel::dumpWord(ostream& s, const Word& word)
{
  for (int index : word)
    s << "x" << index << " ";
}
