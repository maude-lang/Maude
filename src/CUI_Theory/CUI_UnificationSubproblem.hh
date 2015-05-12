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
//      Class for unification subproblems in the C theory.
//	It consists of a vector of unification problems of the form f(u, v) =? f(s, t) where f is a C symbol.
//
#ifndef _CUI_UnificationSubproblem_hh_
#define _CUI_UnificationSubproblem_hh_
//#include <list>
#include "unificationSubproblem.hh"
#include "simpleRootContainer.hh"
//#include "natSet.hh"
//#include "dagNode.hh"
//#include "dagNodeSet.hh"
#include "substitution.hh"
#include "pendingUnificationStack.hh"

class CUI_UnificationSubproblem : public UnificationSubproblem, private SimpleRootContainer
{
  NO_COPYING(CUI_UnificationSubproblem);

public:
  CUI_UnificationSubproblem();
  ~CUI_UnificationSubproblem();

  void addUnification(DagNode* lhs, DagNode* rhs, bool marked, UnificationContext& solution);
  bool solve(bool findFirst, UnificationContext& solution, PendingUnificationStack& pending);

#ifdef DUMP
  //void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  struct Problem
  {
    Problem(CUI_DagNode* lhs, CUI_DagNode* rhs);
    Problem();
    Problem(const Problem& original);

    CUI_DagNode* const lhs;
    CUI_DagNode* const rhs;
    Substitution savedSubstitution;
    PendingUnificationStack::Marker savedPendingState;
    bool reverseTried;
  };

  void markReachableNodes();

  Vector<Problem> problems;
};

inline
CUI_UnificationSubproblem::Problem::Problem()  // HACK
  : lhs(0),
    rhs(0),
    savedSubstitution(0)
{
}

inline
CUI_UnificationSubproblem::Problem::Problem(CUI_DagNode* lhs, CUI_DagNode* rhs)
  : lhs(lhs),
    rhs(rhs),
    savedSubstitution(0)
{
}

inline
CUI_UnificationSubproblem::Problem::Problem(const Problem& original)
  : lhs(original.lhs),
    rhs(original.rhs),
    savedSubstitution(0)  // HACK
{
}

#endif
