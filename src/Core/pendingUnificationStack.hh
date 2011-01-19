/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2008 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for a stack of pending unification problems.
//
//	Problems get pushed on the stack because they potentially have multiple solutions.
//	Great pains are taken to ensure that when we solve such a problem we solve all existing
//	problems that are in the same theory simultaneously since this is crucial for termination
//	in some theories.
//
#ifndef _pendingUnificationStack_hh_
#define _pendingUnificationStack_hh_
#include "simpleRootContainer.hh"
#include "substitution.hh"

class PendingUnificationStack : private SimpleRootContainer
{
  NO_COPYING(PendingUnificationStack);

public:
  typedef int Marker;

  PendingUnificationStack();
  ~PendingUnificationStack();

  void push(Symbol* controllingSymbol, DagNode* lhs, DagNode* rhs, bool marked = false);
  bool resolveTheoryClash(DagNode* lhs, DagNode* rhs);

  Marker checkPoint() const;
  void restore(Marker mark);

  bool solve(bool findFirst, UnificationContext& solution);
  bool solve2(bool findFirst, UnificationContext& solution);

  void dump(ostream& s);

private:
  struct Theory
  {
    Symbol* controllingSymbol;	// controlling symbol for this theory
    int firstProblemInTheory;	// index into stack of first problem in this theory isn't active (or NONE)
  };

  struct PendingUnification
  {
    int theoryIndex;		// index into the theory table
    int nextProblemInTheory;	// index into the stack for the next problem in our theory that isn't active (or NONE)
    DagNode* lhs;
    DagNode* rhs;
    bool marked;		// set this to force the lhs to collapse;
  };

  struct ActiveSubproblem
  {
    int theoryIndex;		// index into the theory table
    int savedFirstProblem;	// saved value of firstProblemInTheory
    UnificationSubproblem* subproblem;
  };

  void markReachableNodes();
  bool makeNewSubproblem(UnificationContext& solution);
  void killTopSubproblem();

  Vector<Theory> theoryTable;
  Vector<PendingUnification> unificationStack;
  Vector<ActiveSubproblem> subproblemStack;
  //
  //	Cycle handling.
  //
  enum Status
    {
      UNEXPLORED = -1,
      EXPLORED = -2
    };

  enum SpecialTheory
    {
      COMPOUND_CYCLE = -2
    };

  int findCycle(UnificationContext& solution);
  int findCycleFrom(int index, UnificationContext& solution);
  Vector<int> variableStatus;
  Vector<int> variableOrder;
};

inline PendingUnificationStack::Marker
PendingUnificationStack::checkPoint() const
{
  return unificationStack.size();
}

#endif
