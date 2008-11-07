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
//      Class for unification subproblems in the AC theory.
//	It consists of the AC symbol, a vector of abstracted subterms and a basis for the
//	Diophantine system corresponding the purified AC unification problem.
//
#ifndef _ACU_UnificationSubproblem2_hh_
#define _ACU_UnificationSubproblem2_hh_
#include <list>
#include "unificationSubproblem.hh"
#include "simpleRootContainer.hh"
#include "natSet.hh"
#include "dagNode.hh"
//#include "dagNodeSet.hh"
#include "substitution.hh"
#include "pendingUnificationStack.hh"

class ACU_UnificationSubproblem2 : public UnificationSubproblem, private SimpleRootContainer
{
  NO_COPYING(ACU_UnificationSubproblem2);

public:
  ACU_UnificationSubproblem2(ACU_Symbol* topSymbol);
  ~ACU_UnificationSubproblem2();

  void addUnification(DagNode* lhs, DagNode* rhs);
  bool solve(bool findFirst, UnificationContext& solution, PendingUnificationStack& pending);

#ifdef DUMP
  //void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  //  DagNodeSet subterms;

  struct Entry
  {
    Vector<int> element;	// practical basis elements will fit in 32-bit machine integers
    NatSet remainder;		// which subterms will be assigned by remaining elements
  };

  typedef list<Entry> Basis;

  void markReachableNodes();

  void setMultiplicity(DagNode* dagNode, int multiplicity);
  bool unsolve(int index, UnificationContext& solution);
  bool buildAndSolveDiophantineSystem(UnificationContext& solution);
  bool nextSelection(bool findFirst);
  bool includable(Basis::const_iterator potential);
  bool buildSolution(UnificationContext& solution, PendingUnificationStack& pending);

  ACU_Symbol* topSymbol;
  Vector<DagNode*> subterms;
  list<Vector<int> > unifications;

  Basis basis;
  Vector<int> multiplicities;
  NatSet accumulator;
  Vector<int> totals;
  Vector<int> upperBounds;
  NatSet uncovered;
  Vector<Basis::const_iterator> selection;
  Basis::const_iterator current;
  Substitution savedSubstitution;
  Substitution preSolveSubstitution;
  PendingUnificationStack::Marker savedPendingState;
};

#endif
