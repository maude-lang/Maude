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
//      Class for ACU variable stripper-collector lhs automaton.
//
#ifndef _ACU_VarLhsAutomaton_hh_
#define _ACU_VarLhsAutomaton_hh_
#include "ACU_CollectorLhsAutomaton.hh"

class ACU_VarLhsAutomaton : public ACU_CollectorLhsAutomaton
{
  NO_COPYING(ACU_VarLhsAutomaton);

public:
  ACU_VarLhsAutomaton(ACU_Symbol* symbol,
		      bool matchAtTop,
		      bool collapsePossible,
		      int nrVariables,
		      VariableTerm* stripper,
		      VariableTerm* collector);
  //
  //	Standard ACU_LhsAutomaton operations.
  //
  bool match(DagNode* subject,
             Substitution& solution,
             Subproblem*& returnedSubproblem,
             ExtensionInfo* extensionInfo);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  //
  //	The stripper variable strips off one argument. It must not
  //	be able to take the identity.
  //
  const int stripperVarIndex;
  Sort* const stripperSort;
  //
  //	If stripper variable can take only one thing or it has a pure sort
  //	then if there exists any assignment there will exist a singleton
  //	assignment. Thus if we can't find a singleton assignment we can
  //	return false.
  //
  const bool trueFailure;
};

#endif
