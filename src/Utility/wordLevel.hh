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
//	Class to store and simplify a level in the solution of a word system.
//
//	A level constains a map from variable to constraints, partial solution and a set
//	of unsolved equations.
//
//	As we solve equations, we may introduce new variables, or in the worst case
//	constrain existing variables to ensure termination. We will generate bindings
//	for unbound variables, and substitute new bindings into existing bindings
//	and unsolved equations.
//
//	Whenever we make a nondeterministic choice we need a new level so we can backtrack
//	to the old level to consider the other choices.
//
#include <list>
#include <map>
#include "pigPug.hh"

#ifndef _wordLevel_hh_
#define _wordLevel_hh_

class WordLevel
{
public:
  enum OutcomeFlags
    {
      FAILURE = 0,	// no solution found
      SUCCESS = 1,	// solution was found
      INCOMPLETE = 2	// solutions may have been missed
    };

  typedef pair<int, WordLevel*> ResultPair;
  //
  //	Local names for PigPug data types.
  //
  typedef PigPug::Word Word;
  typedef PigPug::Subst Subst;
  typedef PigPug::ConstraintMap ConstraintMap;

  WordLevel(int nrVariables, int nrEquations);
  ~WordLevel();

  void addConstraint(int variable, int constraint);
  void addAssignment(int variable, const Word& value);
  void addEquation(int index, const Word& lhs, const Word& rhs);

  ResultPair findNextPartialSolution();
  const Word& getAssignment(int variable) const;
  int getNrVariables() const;

  bool simplify();

  void dump(ostream& s, int indentLevel = 0);
  void dumpWord(ostream& s, const Word& word);

private:
  enum Result
    {
      FAIL,		// occur-check fail or constraint clash - need to fall back to last decision point
      DONE,		// successful completion, no change to partial solution
      CHANGED,		// successful completion, partial solution changed
      CONTINUE		// successful step, maybe more work to do
    };

  enum EquationIndex
    {
      NOT_YET_CHOSEN = -2  // initial value; we use NONE (= -1) if there is no equation to chose
    };

  struct Equation
  {
    Word lhs;
    Word rhs;
  };

  typedef Vector<Equation> EquationVec;

  WordLevel* makeNewLevel(const Subst& unifier, int nextFreshVariable);
  bool chooseEquation();
  void checkUnconstrainedVariables(const Word& word, NatSet& occurs, NatSet& nonlinear);
  void makePigPug(bool strictLeftLinear);
  //
  //	Functions for simplifying partial solution.
  //
  bool fullyExpandAssignments();
  Result expandAssignments();
  Result expandAssignment(int var, Word& word);
  bool reallyExpandAssignment(int var,
			      Word& word,
			      Word::const_iterator firstToExpand,
			      const Word& expansion);
  bool append(Word& newWord, const Word& word, int var);
  //
  //	Functions for simplifying equations.	
  //
  Result simplifyEquations();
  Result simplifyEquation(Equation& equation);
  void expandWord(Word& newWord, const Word& oldWord);
  void append(Word& newWord, const Word& word);
  void updateRemainder(Word& word, int leftCursor, int rightCursor);
  Result cancel(int lhsVar, int rhsVar);
  Result checkForSingleton(const Word& newLhs, int lhsLeftCursor, int lhsRightCursor,
			   const Word& newRhs, int rhsLeftCursor, int rhsRightCursor);
  Result unifyVariables(int lhsVar, int rhsVar);
  Result makeAssignment(int variable, const Word& source, int leftCursor, int rightCursor);
  void copyBack(Word& destination, const Word& source, int leftCursor, int rightCursor);


  ConstraintMap constraintMap;
  Subst partialSolution;
  EquationVec unsolvedEquations;
  //
  //	If there are unsolved equations we need to pick one and solve it with PigPug.
  //
  int chosenEquation;
  PigPug* pigPug;
  int incompletenessFlag;  // to record the use of a transformation that does not preserve completeness
};

inline void
WordLevel::addConstraint(int variable, int constraint)
{
  constraintMap[variable] = constraint;
}

inline void
WordLevel::addAssignment(int variable, const Word& value)
{
  Word& rhs = partialSolution[variable];
  if (rhs.size() == 1 && rhs[0] == variable)
    partialSolution[variable] = value;  // deep copy
  else
    {
      //
      //	Already have an assignement for variable so turn extra assignment into an equation.
      //
      int nrEquations = unsolvedEquations.size();
      unsolvedEquations.expandBy(1);
      addEquation(nrEquations, value, rhs);
    }
}

inline void
WordLevel::addEquation(int index, const Word& lhs, const Word& rhs)
{
  Equation& e = unsolvedEquations[index];
  e.lhs = lhs;  // deep copy
  e.rhs = rhs;  // deep copy
}

inline const WordLevel::Word&
WordLevel::getAssignment(int variable) const
{
  return partialSolution[variable];
}

inline int
WordLevel::getNrVariables() const
{
  return partialSolution.size();
}

#endif
