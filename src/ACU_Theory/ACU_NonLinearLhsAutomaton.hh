/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for fast left hand side automata in the AC and ACU theories
//	when there is just a variable of multiplicity >= 2 with pure,
//	limit or unit sort which cannot take identity, at the top.
//
#ifndef _ACU_NonLinearLhsAutomaton_hh_
#define _ACU_NonLinearLhsAutomaton_hh_
#include "ACU_LhsAutomaton.hh"

class ACU_NonLinearLhsAutomaton : public ACU_LhsAutomaton
{
  NO_COPYING(ACU_NonLinearLhsAutomaton);

public:
  ACU_NonLinearLhsAutomaton(ACU_Symbol* symbol,
			    int nrVariables,
			    int varIndex,
			    int multiplicity,
			    Sort* varSort);
  //
  //	Standard LhsAutomaton operations.
  //
  bool match(DagNode* subject,
             Substitution& solution,
             Subproblem*& returnedSubproblem,
             ExtensionInfo* extensionInfo);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  void fillOutExtensionInfo(ACU_DagNode* subject,
			    const ArgVec<ACU_DagNode::Pair>::const_iterator chosen,
			    ACU_ExtensionInfo* extensionInfo);

  const int varIndex;
  const int multiplicity;
  Sort* const varSort;
  const bool unitSort;
  const bool pureSort;
};

#endif
