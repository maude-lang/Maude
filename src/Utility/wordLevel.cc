/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2015 SRI International, Menlo Park, CA 94025, USA.

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
#include "wordLevel-simplifyAssignments.cc"
#include "wordLevel-simplifyEquations.cc"

WordLevel::WordLevel(int nrVariables, int nrEquations)
  : constraintMap(nrVariables),
    partialSolution(nrVariables),
    unsolvedEquations(nrEquations)
{
  //
  //	Unbound variables must be bound to themselves; by default variables are unconstrained.
  //
  for (int i = 0; i < nrVariables; ++i)
    {
      partialSolution[i].append(i);
      constraintMap[i] = NONE;
    }
  chosenEquation = NOT_YET_CHOSEN;
  pigPug = 0;
  incompletenessFlag = 0;
}

WordLevel::~WordLevel()
{
  delete pigPug;
}

WordLevel::ResultPair
WordLevel::findNextPartialSolution()
{
  if (chosenEquation == NOT_YET_CHOSEN)
    {
      //
      //	Must be first call.
      //
      if (!simplify())
	return ResultPair(FAILURE, 0);  // occurs check failure or constraint clash during simplification
      //
      //	Need to chose an equation and create a PigPug.
      //
      bool strictLeftLinear = chooseEquation();
      if (chosenEquation == NONE)
	return ResultPair(SUCCESS, 0);  // no equations left to solve
      makePigPug(strictLeftLinear);
    }
  if (pigPug == 0)
    return ResultPair(FAILURE, 0);  // must have already returned the unique solution
  //
  //	Get next PigPug solution; create a new WordLevel.
  //
  Subst unifier;
  PigPug::ResultPair result = pigPug->getNextUnifier(unifier);
  int nextFreshVariable = result.second;
  if (nextFreshVariable == NONE)
    return ResultPair(result.first, 0);  // failure

  WordLevel* next = makeNewLevel(unifier, nextFreshVariable);
  return ResultPair(result.first, next);
}

bool
WordLevel::simplify()
{
  //
  //	We start by simplifying the partial solution.
  //
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
WordLevel::makeNewLevel(const Subst& unifier, int nextFreshVariable)
{
  //
  //	We have a PigPug solution - need to copy old stuff and new solution into a new WordLevel object.
  //
  int equationCount = 0;
  FOR_EACH_CONST(i, EquationVec, unsolvedEquations)
    {
      if (!(i->lhs.empty()))
	++equationCount;
    }
  WordLevel* newLevel = new WordLevel(nextFreshVariable, equationCount - 1);
  //
  //	Copy in old information and unifier.
  //
  int nrConstraints = constraintMap.size();
  for (int i = 0; i < nrConstraints; ++i)
    newLevel->addConstraint(i, constraintMap[i]);
  int nrVariables = partialSolution.size();
  for (int i = 0; i < nrVariables; ++i)
    {
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

bool
WordLevel::chooseEquation()
{
  //
  //	Chosen an unsolved equation and set chosenEquation to it true.
  //	If there are no unsolved equations set chosenEquation to NONE.
  //	Returns true if chosen equation is strict left-linear and false otherwise.
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
	      if (lhsNonlinear.empty())
		{
		  //
		  //	We found a strict left-linear equation so choose it.
		  //
		  chosenEquation = i;
		  return true;
		}
	      if (rhsNonlinear.empty())
		{
		  //
		  //	We found a strict right-linear equation so flip it into a
		  //	strict left-linear equation and choose it.
		  //
		  e.lhs.swap(e.rhs);
		  chosenEquation = i;
		  return true;
		}
	      chosenEquation = i;  // prefer an equation with disjoint lhs/rhs variable sets
	    }
	  if (chosenEquation == NONE)
	    chosenEquation = i;  // we'll take this one if we don't see anything better
	}
    }
  return false;  // we didn't find a strict left-linear equation
}

void
WordLevel::checkUnconstrainedVariables(const Word& word, NatSet& occurs, NatSet& nonlinear)
{
  FOR_EACH_CONST(i, Word, word)
    {
      int index = *i;
      if (constraintMap[index] == NONE)
	{
	  if (occurs.contains(index))
	    nonlinear.insert(index);
	  else
	    occurs.insert(index);
	}
    }
}

void
WordLevel::makePigPug(bool strictLeftLinear)
{
  Equation& e = unsolvedEquations[chosenEquation];
  int nrVariables = partialSolution.size();
  pigPug = new PigPug(e.lhs, e.rhs, constraintMap, nrVariables - 1, nrVariables, strictLeftLinear);
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
	s << Indent(indentLevel + 2) << "X" << i << " : ";
	int c = constraintMap[i];
	if (c == PigPug::ELEMENT)
	  s << "ELEMENT";
	else if (c == NONE)
	  s << "NONE";
	else
	  s << "theory" << c;
	s << endl;
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
}

void
WordLevel::dumpWord(ostream& s, const Word& word)
{
  FOR_EACH_CONST(i, Word, word)
    s << "x" << *i << " ";
}
