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
//      Class for breaking compound cycles arising during unification.
//
//	A cycle is a vector of variables in the solved form with the structure
//	  X1 |-> t1[..., X2,...]
//	  X2 |-> t2[..., X3,...]
//	      :
//	  Xn |-> tn[..., X1,...]
//	where ti is pure in theory Ti.
//
//	Such a situation would normally be considered an occurs-check failure
//	but in the presence of collapse or nonregular theories may have one
//	or more resolutions.
//
#ifndef _compoundCycleSubproblem_hh_
#define _compoundCycleSubproblem_hh_
#include "unificationSubproblem.hh"
#include "simpleRootContainer.hh"
#include "pendingUnificationStack.hh"

class CompoundCycleSubproblem : public UnificationSubproblem, private SimpleRootContainer
{
public:
  CompoundCycleSubproblem();

  void addComponent(int variableIndex);
  bool solve(bool findFirst, UnificationContext& solution, PendingUnificationStack& pending);

#ifdef DUMP
  /*
  virtual void dump(ostream& s,
		    const VariableInfo& variableInfo,
		    int indentLevel = 0);
  */
#endif

private:
  void markReachableNodes();

  Vector<int> cycle;
  Substitution preBreakSubstitution;
  PendingUnificationStack::Marker preBreakPendingState;
  int currentEdgeIndex;
};

inline
CompoundCycleSubproblem::CompoundCycleSubproblem()
  : preBreakSubstitution(0)
{
  DebugAdvisory("Created  CompoundCycleSubproblem");
}

inline void
CompoundCycleSubproblem::addComponent(int variableIndex)
{
  cycle.append(variableIndex);
}

#endif
