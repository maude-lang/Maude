/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for disjunctions of subproblems.
//
//	Disjunctions arise from theory clashes where both theories want to
//	resolve the clash. We solve a sequence of such clashes by trying all
//	combinations of controlling symbols.
//
#ifndef _unificationSubproblemDisjunction_hh_
#define _unificationSubproblemDisjunction_hh_
#include "unificationSubproblem.hh"
//#include "simpleRootContainer.hh"
//#include "substitution.hh"
#include "pendingUnificationStack.hh"

class UnificationSubproblemDisjunction : public UnificationSubproblem
{
public:
  UnificationSubproblemDisjunction();

  void addUnification(DagNode* lhs, DagNode* rhs, bool marked, UnificationContext& solution);
  bool solve(bool findFirst, UnificationContext& solution, PendingUnificationStack& pending);

#ifdef DUMP
  /*
  virtual void dump(ostream& s,
		    const VariableInfo& variableInfo,
		    int indentLevel = 0);
  */
#endif

private:
  struct TheoryClash
  {
    DagNode* lhs;
    DagNode* rhs;
    PendingUnificationStack::Marker savedPendingState;
    bool lhsControlling;
  };

  Vector<TheoryClash> problems;
};

inline
UnificationSubproblemDisjunction::UnificationSubproblemDisjunction()
{
  DebugAdvisory("Created  UnificationSubproblemDisjunction");
}

#endif
