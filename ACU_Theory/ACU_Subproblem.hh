/*

    This file is part of the Maude 2 interpreter.

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
//      Class for subproblems in the AC and ACU theories;
//	they consist of a bipartite graph matching 
//	problem and (possibly) a diophantine system
//
#ifndef _ACU_Subproblem_hh_
#define _ACU_Subproblem_hh_
#include "subproblem.hh"

class ACU_Subproblem : public Subproblem
{
  NO_COPYING(ACU_Subproblem);

public:
  ACU_Subproblem(ACU_DagNode* subjectDagNode,
		 ACU_ExtensionInfo* extension);
  ~ACU_Subproblem();

  int addPatternNode(int multiplicity);
  void removePatternNode(int& uniqueSubject,
			 LocalBinding*& difference,
			 Subproblem*& subproblem);
  bool noPatterns() const;
  void addEdge(int pattern,
	       int target,
	       LocalBinding* difference,
	       Subproblem* subproblem);
  void addTopVariable(int index,
		      int multiplicity,
		      int lowerBound,
		      int upperBound,
		      const Sort* sort);
  void addSubjects(Vector<int>& multiplicity);

  bool solve(bool findFirst, RewritingContext& solution);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  struct TopVariable
    {
      int index;
      int multiplicity;
      int lowerBound;
      int upperBound;
      const Sort* sort;
    };

  struct Edge
    {
      int target;
      LocalBinding* difference;
      Subproblem* subproblem;
    };

  struct PatternNode
    {
      bool solve(bool findFirst,
		 RewritingContext& solution,
		 Vector<int>& currentMultiplicity);

      int multiplicity;
      Vector<Edge> edges;
      int selectedEdge;
    };

  bool solvePatterns(bool findFirst, RewritingContext& solution);
  bool solveVariables(bool findFirst, RewritingContext& solution);
  bool noVariableCase(const Vector<int>& multVec);
  bool oneVariableCase(const Vector<int>& multVec, RewritingContext& solution);
  bool extractDiophantineSystem(RewritingContext& solution);
  DagNode* computeAssignment(int row);
  void fillOutExtensionInfo();

  ACU_DagNode* const subject;
  ACU_ExtensionInfo* const extensionInfo;
  Vector<int> currentMultiplicity;
  Vector<TopVariable> topVariables;
  Vector<PatternNode> patternNodes;
  //
  //	Structures needed for building and solving diophantine problem
  //
  DiophantineSystem* system;
  Vector<int> variableMap;
  Vector<int> subjectMap;
  Vector<int> afterMultiplicity;
};

inline bool
ACU_Subproblem::noPatterns() const
{
  return patternNodes.empty();
}

#endif
