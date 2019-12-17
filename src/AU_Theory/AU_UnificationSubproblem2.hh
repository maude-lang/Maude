/*

    This file is part of the Maude 3 interpreter.

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
//      Class for unification subproblems in the A theory.
//
#ifndef _AU_UnificationSubproblem2_hh_
#define _AU_UnificationSubproblem2_hh_
#include <list>
#include "unificationSubproblem.hh"
#include "simpleRootContainer.hh"
#include "natSet.hh"
#include "dagNode.hh"
#include "substitution.hh"
#include "pendingUnificationStack.hh"
#include "wordSystem.hh"

class AU_UnificationSubproblem2 : public UnificationSubproblem, private SimpleRootContainer
{
  NO_COPYING(AU_UnificationSubproblem2);

public:
  AU_UnificationSubproblem2(AU_Symbol* topSymbol);
  ~AU_UnificationSubproblem2();

  void addUnification(DagNode* lhs, DagNode* rhs, bool marked, UnificationContext& solution);
  bool solve(bool findFirst, UnificationContext& solution, PendingUnificationStack& pending);



private:
  struct Assignment
  {
    int variable;
    WordSystem::Word value;
  };

  struct Unification
  {
    WordSystem::Word lhs;
    WordSystem::Word rhs;
  };

  void markReachableNodes();

  int dagToAbstract(DagNode* dagNode, UnificationContext& solution);
  void assocToAbstract(DagNode* dagNode, WordSystem::Word& word, UnificationContext& solution);
  void makeWordSystem(UnificationContext& solution);
  void unsolve(int index, UnificationContext& solution);
  DagNode* abstractToFreshVariable(int variableIndex, UnificationContext& solution);
  bool buildSolution(UnificationContext& solution, PendingUnificationStack& pending);
 
  AU_Symbol* const topSymbol;
  Vector<DagNode*> subterms;
  list<Assignment> assignments;
  list<Unification> unifications;
  WordSystem* wordSystem;
  Vector<DagNode*> freshVariables;
  //
  //	For backtracking.
  //
  Substitution preSolveSubstitution;
  Substitution savedSubstitution;
  PendingUnificationStack::Marker preSolveState;
  PendingUnificationStack::Marker savedPendingState;
};

#endif
