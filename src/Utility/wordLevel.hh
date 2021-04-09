/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2021 SRI International, Menlo Park, CA 94025, USA.

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
#include <set>
#include "natSet.hh"
#include "pigPug.hh"

#ifndef _wordLevel_hh_
#define _wordLevel_hh_

class WordLevel
{
public:
  enum LevelType
    {
     INITIAL,		// first level in search, can use selection or PigPug to branch
     SELECTION,		// second level in search if it appears, produced by selection
     PIGPUG		// second or higher level, produced by PigPug
    };

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

  WordLevel(LevelType levelType,
	    int nrVariables,
	    int nrEquations,
	    bool identityOptimizations,
	    WordLevel* parent = 0);
  ~WordLevel();

  void setTheoryConstraint(int variable, int theoryIndex);
  void setUpperBound(int variable, int upperBound);
  void setTakeEmpty(int variable);
  void addAssignment(int variable, const Word& value);
  void addEquation(int index, const Word& lhs, const Word& rhs);
  void addNullEquation(const Word& word);

  ResultPair findNextPartialSolution();
  const Word& getAssignment(int variable) const;
  int getNrVariables() const;

  void dump(ostream& s, int indentLevel = 0);
  void dumpWord(ostream& s, const Word& word);

private:
  enum Result
    {
     FAIL,	// occur-check fail or constraint clash - need to fall back to last decision point
     DONE,	// successful completion, no change to partial solution
     CHANGED,	// successful completion, partial solution changed
     CONTINUE,	// successful step, maybe more work to do
     UNSAFE     // no work done because on unsafeAssignment
    };

  enum EquationIndex
    {
      NOT_YET_CHOSEN = -2  // initial value; we use NONE (= -1) if there is no equation to chose
    };

  enum Linearity
    {
     NONLINEAR = 0,
     STRICT_LEFT_LINEAR = 1,
     STRICT_RIGHT_LINEAR = 2,
     LINEAR = STRICT_LEFT_LINEAR | STRICT_RIGHT_LINEAR
    };

  struct Equation
  {
    Word lhs;
    Word rhs;
  };

  typedef Vector<Equation> EquationVec;
  typedef list<Word> NullEquationQueue;

  //
  //	Code for handling assignments in the normal case.
  //
  WordLevel::Result checkAssignmentNormalCase(int i);
  WordLevel::Result checkAssignmentsNormalCase();
  bool checkAssignmentsToFixedPointNormalCase();
  
  bool reallyExpandAssignmentNormalCase(int i);
  WordLevel::Result expandAssignmentNormalCase(int i);
  WordLevel::Result expandAssignmentsNormalCase();
  bool expandAssignmentsToFixedPointNormalCase();
  //
  //	Code for handling assignments in the collapse case.
  //
  WordLevel::Result checkAssignmentCollapseCase(int i);
  WordLevel::Result checkAssignmentsCollapseCase();
  bool checkAssignmentsToFixedPointCollapseCase();
  
  bool reallyExpandAssignmentCollapseCase(int i);
  WordLevel::Result expandAssignmentCollapseCase(int i);
  WordLevel::Result expandAssignmentsCollapseCase();
  bool expandAssignmentsToFixedPointCollapseCase();
  
  bool simplify();
  //
  //	Selection (for identity assignment).
  //
  bool systemLinear();
  void computePinches(const Word& pincher, const Word& pinched, NatSet& pinchedVariables);
  void determinePinchedVariables(NatSet& pinchedVariables);
  void chooseVariablesToSelectFrom();
  WordLevel::ResultPair trySelection();
  WordLevel::ResultPair exploreSelections();
  
  WordLevel* makeNewLevel(const Subst& unifier,
			  const ConstraintMap& newConstraintMap,
			  int nextFreshVariable);
  int chooseEquation();
  void checkUnconstrainedVariables(const Word& word, NatSet& occurs, NatSet& nonlinear);
  void makePigPug(int linearity);
  //
  //	Functions for dealing with the empty word.
  //
  bool handleNullEquations();
  bool resolveOccursCheckFailure(int index, const Word& newValue);
  bool makeEmptyAssignment(int i);
  //
  //	Functions for simplifying partial solution.
  //
  bool handleInitialOccursCheckFailure();
  bool fullyExpandAssignments();
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
  Result checkForNull(const Word& newLhs, const Word& newRhs);
  Result checkForSingleton(const Word& newLhs, int lhsLeftCursor, int lhsRightCursor,
			   const Word& newRhs, int rhsLeftCursor, int rhsRightCursor);
  Result unifyVariables(int lhsVar, int rhsVar);
  Result makeAssignment(int variable, const Word& source, int leftCursor, int rightCursor);
  void copyBack(Word& destination, const Word& source, int leftCursor, int rightCursor);
  //
  //	Checks.
  //
  bool feasibleWithoutCollapse(const Word& lhs, const Word& rhs) const;
  bool levelFeasibleWithoutCollapse() const;
  bool insertCombination(const Subst& substitution);

  const LevelType levelType;
  const bool identityOptimizations;
  ConstraintMap constraintMap;
  NullEquationQueue nullEquations;
  Subst partialSolution;
  NatSet unsafeAssignments;
  EquationVec unsolvedEquations;
  //
  //	If there are unsolved equations we need to pick one and solve it with PigPug.
  //
  int chosenEquation;
  PigPug* pigPug;
  int incompletenessFlag;  // to record the use of a transformation that does not preserve completeness
  //
  //	If we are the INITIAL level and there are variables that can take empty
  //	which were not set to empty during simplification, after PigPug fails (or
  //	if there are no equations left to be solved with PigPug) we look at
  //	selections of these variables to set to identity.
  //
  Vector<int> idVariables;
  int selection;
  int nrSelections;
  set<int> finalCombinations;
  //
  //	If we are a SELECTION level we keep a pointer the INTIAL level that
  //	created us.
  //
  WordLevel* parent;
};

inline void
WordLevel::setTheoryConstraint(int variable, int theoryIndex)
{
  constraintMap[variable].setTheoryConstraint(theoryIndex);
}

inline void
WordLevel::setUpperBound(int variable, int upperBound)
{
  constraintMap[variable].setUpperBound(upperBound);
}

inline void
WordLevel::setTakeEmpty(int variable)
{
  constraintMap[variable].setTakeEmpty();
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

inline void
WordLevel::addNullEquation(const Word& word)
{
  nullEquations.push_back(word);
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
