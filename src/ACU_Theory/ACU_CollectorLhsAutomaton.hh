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
//      Base class for ACU stripper-collector left hand side automata.
//
#ifndef _ACU_CollectorLhsAutomaton_hh_
#define _ACU_CollectorLhsAutomaton_hh_
#include "ACU_LhsAutomaton.hh"

class ACU_CollectorLhsAutomaton : public ACU_LhsAutomaton
{
  NO_COPYING(ACU_CollectorLhsAutomaton);

public:
  ACU_CollectorLhsAutomaton(ACU_Symbol* symbol,
			    bool matchAtTop,
			    bool collapsePossible,
			    int nrVariables,
			    VariableTerm* collector);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

protected:
  bool collectorFree(Substitution& solution) const;
  bool collect(int stripped, ACU_DagNode* subject, Substitution& solution) const;
  bool collect(ACU_Stack& stripped,  // destroyed
	       ACU_TreeDagNode* subject,
	       Substitution& solution) const;
  void collapse(Substitution& solution) const;

private:
  //
  //	The stripper variable/automaton/term in the derived class
  //	strips off one argument.
  //	The collector variable collects all the other arguments.
  //
  const int collectorVarIndex;
  //
  //	The collector sort must be unbounded. If it is the error sort
  //	or the unique maximal sort of an error free component we set
  //	it to 0 to switch off sort checks since they should always succeed.
  //
  const Sort* collectorSort;
};

inline bool
ACU_CollectorLhsAutomaton::collectorFree(Substitution& solution) const
{
  return solution.value(collectorVarIndex) == 0;
}

inline void
ACU_CollectorLhsAutomaton::collapse(Substitution& solution) const
{
  solution.bind(collectorVarIndex, getSymbol()->getIdentityDag());
}

#endif
