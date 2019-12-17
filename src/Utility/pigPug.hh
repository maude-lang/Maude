/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2014 SRI International, Menlo Park, CA 94025, USA.

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

class PigPug
{
public:
  enum OutcomeFlags
    {
      FAILURE = 0,	// no solution found
      SUCCESS = 1,	// solution was found
      INCOMPLETE = 2	// solutions may have been missed
    };

  typedef pair<int, int> ResultPair;  // <outcome flags, next fresh variable>

  enum Constraints
    {
      ELEMENT = -2,
    };
  //
  //	Constraint for each variable is ELEMENT, NONE or non-negative integer indexing a (collapse-free) theory.
  //
  typedef Vector<int> ConstraintMap;

  typedef Vector<int> Word;
  typedef Vector<Word> Subst;

  PigPug(const Word& lhs,
	 const Word& rhs,
	 const ConstraintMap& constraintMap,
	 int lastOriginalVariable,
	 int freshVariableStart,
	 bool strictLeftLinear);
  //
  //	Returns NONE, or the index for the next unused variable, and fills out unifier.
  //
  ResultPair getNextUnifier(Subst& unifier);

#ifdef DUMP
  static void dumpWord(ostream& s, const Word& word);
#endif

private:
  enum Moves
    {
      INC_RHS = 1,
      INC_LHS = 2,
      MOVES = INC_RHS | INC_LHS,
      BOTH = INC_RHS | INC_LHS,
      //
      //	3 basic moves.
      //
      //	RHS_PEEL = 1,
      //	LHS_PEEL = 2,
      //	EQUATE = 3,
      //
      //	We OR this flag with EQUATE to indicate that the rhs variable
      //	was instantiated rather than the lhs variable (the usual case).
      //	This happens when the rhs variable is strictly less constrained
      //	than the lhs variable.
      //
      RHS_ASSIGN = 4,
      //
      //	We OR this flag with RHS_PEEL or EQUATE to record the fact
      //	that a modified lhs unificand was pushed on the stack.
      //
      PUSH_LHS = 8,
      //
      //	We OR this flag with LHS_PEEL or EQUATE to record the fact
      //	that a modified rhs unificand was pushed on the stack.
      //
      PUSH_RHS = 16,
      //
      //	We OR this flag with EQUATE to indicate a canceling of equal variables.
      //	We need in the cycle detection case so we don't try to pop a state
      //	that doesn't exist.
      //
      CANCEL = 32
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
  int run(int result);
  //
  //	Stuff for general case.
  //
  Result lhsPeelGeneralCase();
  Result rhsPeelGeneralCase();
  bool feasibleGeneralCase();
  bool canCancelUnconstrained(const Unificand& big, const Unificand& small);
  //
  //	Stuff for strict left-linear case.
  //
  Result lhsPeel();
  Result rhsPeel();
  bool feasible();
  bool fullyConstrained(const Unificand& unificand);
  //
  //	Common code.
  //
  Result cancel();
  Result equate();
  bool checkUnificand(UnificandStack& unificandStack, int oldVar, int newVar);
  bool checkUnificand2(UnificandStack& unificandStack, int oldVar, int newVar, int offset);
  int undoMove();
  bool occurs(int variable, const Unificand& unificand);
  int completed(int status);
  //
  //	Unifier extraction code.
  //
  int extractUnifier(Subst& unifier);
  void compose(Subst& subst, int oldVar, int replacement);
  void compose2(Subst& subst, int oldVar, int replacement);
  void compose(Subst& subst, int oldVar, const Word& replacement, int index);
  void renameVariables(Subst& subst, const VariableRenaming& variableRenaming);
  void collectRangeVariables(const Subst& subst, NatSet& occursInRange);

#ifdef DUMP
  void dump(ostream& s, Subst& subst);
#endif

  const ConstraintMap& constraintMap;
  const int lastOriginalVariable;
  const int freshVariableStart;
  const bool strictLeftLinear;
  bool cycleDetection;  // true means we will use cycle detection
  int depthBound;  // NONE means we will fully explore the search tree
  int incompletenessFlag;  // 0 or INCOMPLETE

  UnificandStack lhsStack;
  UnificandStack rhsStack;
  Path path;
  //
  //	Cycle detection data.
  //
  WordMap wordMap;
  Vector<StateInfo> stateInfo;
  StateStack traversalStack;
};

#endif
