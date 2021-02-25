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
//	Class to implement a modified PIG-PUG to solve
//	associative unification problems with constraints on variables.
//
//	An equation is strict left linear if each variable in the lhs is
//	either constrained or occurs exactly once in the equation. PIG-PUG
//	can be shown to terminate on such equation with a finite complete
//	set of unifiers.
//
//	For other equations we resort to incomplete methods to ensure termination.
//

#ifndef _pigPug_hh_
#define _pigPug_hh_
#include <list>
#include <map>
#include <vector>
#include "natSet.hh"
#include "variableConstraint.hh"

class PigPug
{
public:
  enum Linearity
    {
     NONLINEAR = 0,
     STRICT_LEFT_LINEAR = 1,
     STRICT_RIGHT_LINEAR = 2,
     LINEAR = STRICT_LEFT_LINEAR | STRICT_RIGHT_LINEAR
    };

  enum OutcomeFlags
    {
      FAILURE = 0,	// no solution found
      SUCCESS = 1,	// solution was found
      INCOMPLETE = 2	// solutions may have been missed
    };

  typedef pair<int, int> ResultPair;  // <outcome flags, next fresh variable>
  typedef Vector<VariableConstraint> ConstraintMap;
  typedef Vector<int> Word;
  typedef Vector<Word> Subst;

  PigPug(const Word& lhs,
	 const Word& rhs,
	 const ConstraintMap& constraintMap,
	 int lastOriginalVariable,
	 int freshVariableStart,
	 int linearity,
	 bool equateOptimization = false);
  //
  //	Returns NONE, or the index for the next unused variable, and fills out unifier.
  //	constraintMap is filled out with entries for all variables, original and fresh.
  //	Variables that appear in binding of some other variable (which necessarily
  //	includes all fresh variables) will get a constraint calculated by PigPug. Other
  //	original variables are given their original constraint.
  //
  ResultPair getNextUnifier(Subst& unifier, ConstraintMap& constraintMap);

private:
  enum Moves
    {
      //
      //	3 basic moves of PigPug algorithm.
      //
      //	RHS_PEEL: (x1 x2 ... =? y1 y2 ...) -> (x1 x2 ... =? y2 ...)
      //	LHS_PEEL: (x1 x2 ... =? y1 y2 ...) -> (x2 ... =? y1 y2 ...)
      //	EQUATE:   (x1 x2 ... =? y1 y2 ...) -> (x2 ... =? y2 ...)
      //
      RHS_PEEL = 1,
      LHS_PEEL = 2,
      EQUATE = RHS_PEEL | LHS_PEEL,
      //
      //	Two final moves. These are forced and never update state.
      //
      FINAL = 4,
      LHS_TAKES_ALL = FINAL | RHS_PEEL,
      RHS_TAKES_ALL = FINAL | LHS_PEEL,
      //
      //	Mask for extracting actual move from other details.
      //
      BASIC_MOVES = RHS_PEEL | LHS_PEEL,
      ALL_MOVES =  BASIC_MOVES | FINAL,
      //
      //	EQUATE was done as rhsVar |-> lhsVar
      //
      //	We OR this flag with EQUATE to indicate that the rhs variable
      //	was instantiated rather than the lhs variable (the usual case).
      //	This happens when the rhs variable is strictly less constrained
      //	than the lhs variable.
      //
      RHS_ASSIGN = 8,
      //
      //	We OR this flag with RHS_PEEL or EQUATE to record the fact
      //	that a modified lhs unificand was pushed on the stack.
      //
      PUSH_LHS = 16,
      //
      //	We OR this flag with LHS_PEEL or EQUATE to record the fact
      //	that a modified rhs unificand was pushed on the stack.
      //
      PUSH_RHS = 32,
      //
      //	We OR this flag with the move anytime we need to push
      //	an updated ConstraintMap on the constraintStack.
      //
      PUSH_CONSTRAINT_MAP = 64,
      //
      //	We OR this flag with EQUATE to indicate a canceling of equal variables.
      //	We need this in the cycle detection case so we don't try to pop a state
      //	that doesn't exist.
      //
      CANCEL = 128
    };

  enum SpecialValues
    {
      DELIMITER = -1  // no variable can have this value
    };

  enum Result
    {
      NOT_ENTERED = -1,	// for cycle detection version only, we didn't enter the state
      FAIL = 0,		// move failed
      LHS_DONE = 1,	// move reduced lhs (and possibly rhs) to a single variable
      RHS_DONE = 2,	// move reduced rhs (but not lhs) to a single variable
      OK = 4		// move succeeded without hitting an end case
    };

  struct Unificand
  {
    int index;		// to record the size of the cancelled prefix
    Word word;		// the word of variables
  };

  typedef list<Unificand> UnificandStack;
  typedef list<ConstraintMap> ConstraintStack;
  typedef Vector<int> Path;
  typedef Vector<int> VariableRenaming;
  //
  //	Cycle detection data structures
  //
  typedef vector<int> CombinedWord;  // use STL vector in order to have comparison
  typedef map<CombinedWord, int> WordMap;
  typedef Vector<int> StateStack;

  struct StateInfo
  {
    bool onStack;
    bool onCycle;
    bool onLivePath;
    bool dummy;  // pad to 4 bytes
  };
  //
  //	PIG-PUG with cycle detection functions.
  //
  void makeStateKey(CombinedWord& stateKey);
  bool variableOccurrencesBoundedBy2(const Word& lhs, const Word& rhs, int maxVarNumber);
  int firstMoveWithCycleDetection();
  int nextMoveWithCycleDetection();
  int runWithCycleDetection(int result);
  bool arrive(const CombinedWord& word);
  void depart();
  void confirmedLive();
  //
  //	PIG-PUG without cycle detection functions.
  //
  int firstMove();
  int nextMove();
  bool doublePeelPossible();
  int run(int result);
  //
  //	Common code.
  //
  Result lhsPeel();
  Result rhsPeel();
  bool feasible();
  Result cancel();
  Result equate();
  bool checkUnificand(UnificandStack& unificandStack, int oldVar, int newVar);
  bool checkUnificand2(UnificandStack& unificandStack, int oldVar, int newVar, int offset);
  bool checkConstraintMap(int knownBigVariable, int otherVariable);
  bool checkConstraintMap(int knownBigVariable, const Unificand& otherSide);
  int undoMove();
  int completed(int status);
  //
  //	Unifier extraction code.
  //
  int extractUnifier(Subst& unifier, ConstraintMap& constraintMap);
  void compose(Subst& subst, int oldVar, int replacement);
  bool compose2(Subst& subst, int oldVar, int replacement);
  bool composeFinal(Subst& subst, int oldVar, const Word& replacement, int index);
  void renameVariables(Subst& subst, const VariableRenaming& variableRenaming);
  void collectRangeVariables(const Subst& subst, NatSet& occursInRange);
  void tightenConstraints(const Subst& subst, ConstraintMap& constraintMap);

  //void checkInvariant(const ConstraintMap& constraintMap, Subst& subst);

  
  const int lastOriginalVariable;
  const int freshVariableStart;
  const int linearity;
  const bool equateOptimization;
  bool cycleDetection;  // true means we will use cycle detection
  int depthBound;  // NONE means we will fully explore the search tree
  int incompletenessFlag;  // 0 or INCOMPLETE

  UnificandStack lhsStack;
  UnificandStack rhsStack;
  ConstraintStack constraintStack;
  Path path;
  //
  //	Cycle detection data.
  //
  WordMap wordMap;
  Vector<StateInfo> stateInfo;
  StateStack traversalStack;
};

ostream& operator<<(ostream& s, const PigPug::Word& word);

#endif
