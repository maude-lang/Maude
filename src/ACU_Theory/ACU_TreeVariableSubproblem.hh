/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for fast subproblems in tree case, where we have stripper and
//	collector variables.
//
//	We try to find each solution by stripping out one argument of the 
//	subject and avoid copying a potentially huge number of subject
//	arguments using non-destructive deletion.
//
#ifndef _ACU_TreeVariableSubproblem_hh_
#define _ACU_TreeVariableSubproblem_hh_
#include "subproblem.hh"
#include "ACU_SlowIter.hh"

class ACU_TreeVariableSubproblem : public Subproblem, private SimpleRootContainer
{
  NO_COPYING(ACU_TreeVariableSubproblem);

public:
  ACU_TreeVariableSubproblem(ACU_BaseDagNode* subject,
			     const ACU_Tree& remaining,  // may have red-black nodes not in the subject that we must protect
			     int stripperVarIndex,
			     Sort* stripperSort,
			     int collectorVarIndex,
			     Sort* collectorSort);
  ~ACU_TreeVariableSubproblem();

  bool solve(bool findFirst, RewritingContext& solution);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  void markReachableNodes();
  DagNode* collect(RewritingContext& solution);

  ACU_BaseDagNode* const subject;
  ACU_Tree remaining;
  const int stripperVarIndex;
  Sort* const stripperSort;
  const int collectorVarIndex;
  Sort* const collectorSort;
  //
  //	Solve time variables.
  //
  ACU_SlowIter currentPath;
  int previousIndex;
  Vector<LocalBinding*> previous;
};

#endif
