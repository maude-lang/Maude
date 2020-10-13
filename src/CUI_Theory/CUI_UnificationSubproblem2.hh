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
//      Class for unification subproblems in the CU, U, Ul and Ur theories.
//	It consists of a vector of subproblems of the forms f(u, v) =? f(s, t) or
//	f(u, v) =? X or f(u, v) = g(...) where f is a CUI symbol and X is a variable and
//	g is an alien symbol.
//
#ifndef _CUI_UnificationSubproblem2_hh_
#define _CUI_UnificationSubproblem2_hh_
#include "unificationSubproblem.hh"
#include "simpleRootContainer.hh"
#include "substitution.hh"
#include "pendingUnificationStack.hh"
#include "natSet.hh"
#include "variableDagNode.hh"
#include "unificationContext.hh"

class CUI_UnificationSubproblem2 : public UnificationSubproblem, private SimpleRootContainer
{
  NO_COPYING(CUI_UnificationSubproblem2);

public:
  CUI_UnificationSubproblem2();
  ~CUI_UnificationSubproblem2();

  void addUnification(DagNode* lhs, DagNode* rhs, bool marked, UnificationContext& solution);
  bool solve(bool findFirst, UnificationContext& solution, PendingUnificationStack& pending);

#ifdef DUMP
  //void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  enum Alternatives
    {
      FORWARDS,
      REVERSE,
      LHS_ARG0_TAKES_ID,
      LHS_ARG1_TAKES_ID,
      RHS_ARG0_TAKES_ID,
      RHS_ARG1_TAKES_ID,
      RHS_VARIABLE_TAKES_ALL,
      NO_MORE_ALTERNATIVES
    };

  struct Problem
  {
    Problem(CUI_DagNode* lhs, DagNode* rhs, const NatSet& legalAlternatives);
    Problem(const Problem& original);

    bool findAlternative(bool first, UnificationContext& solution, PendingUnificationStack& pending);
    bool tryAlternative(UnificationContext& solution, PendingUnificationStack& pending);

    CUI_DagNode* const lhs;
    DagNode* const rhs;
    NatSet legalAlternatives;

    Substitution savedSubstitution;
    PendingUnificationStack::Marker savedPendingState;
    int alternative;
  };

  void markReachableNodes();
  static bool leftCollapse(DagNode* leftArg,
			   CUI_Symbol* topSymbol,
			   UnificationContext& solution);
  static bool rightCollapse(DagNode* rightArg,
			    CUI_Symbol* topSymbol,
			    UnificationContext& solution);
  static bool equivalent(DagNode* first,
			 DagNode* second,
			 UnificationContext& solution);
  static bool equivalentToGroundDag(DagNode* dag,
				    DagNode* groundDag,
				    UnificationContext& solution);
  Vector<Problem> problems;
};

inline bool
CUI_UnificationSubproblem2::equivalentToGroundDag(DagNode* dag,
						  DagNode* groundDag,
						  UnificationContext& solution)
{
  if (dag->equal(groundDag))
    return true;
  if (VariableDagNode* var = dynamic_cast<VariableDagNode*>(dag))
    {
      VariableDagNode* rep = var->lastVariableInChain(solution);
      DagNode* binding = solution.value(rep->getIndex());
      if (binding != 0 && binding->equal(groundDag))
	return true;
    }
  return false;
}

inline
CUI_UnificationSubproblem2::Problem::Problem(CUI_DagNode* lhs,
					     DagNode* rhs,
					     const NatSet& legalAlternatives)
  : lhs(lhs),
    rhs(rhs),
    legalAlternatives(legalAlternatives),
    savedSubstitution(0)
{
}

inline
CUI_UnificationSubproblem2::Problem::Problem(const Problem& original)
  : lhs(original.lhs),
    rhs(original.rhs),
    legalAlternatives(original.legalAlternatives),
    savedSubstitution(0)
{
  //
  //	This is only a partial copy constructor - it copies the first three
  //	data members, which is what is needed for addUnification(). We also
  //	need to initialize saveSubstitution because Substitution has no
  //	default constructor.
  //
}

#endif
