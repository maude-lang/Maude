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
//      Implementation for class ACU_GndLhsAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "indent.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "ACU_Persistent.hh"
#include "ACU_Theory.hh"

//      interface class definitions
#include "associativeSymbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "extensionInfo.hh"

//      core class definitions
#include "substitution.hh"

//	ACU Red-Black class definitions
#include "ACU_SlowIter.hh"

//	ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_GndLhsAutomaton.hh"
#include "ACU_TreeDagNode.hh"

ACU_GndLhsAutomaton::ACU_GndLhsAutomaton(ACU_Symbol* symbol,
					 bool matchAtTop,
					 bool collapsePossible,
					 int nrVariables,
					 Term* stripperTerm,
					 VariableTerm* collector)
  : ACU_CollectorLhsAutomaton(symbol,
			      matchAtTop,
			      collapsePossible,
			      nrVariables,
			      collector),
  stripperTerm(stripperTerm)
{
  Assert(stripperTerm->ground(), "stripper term must be ground");
}

bool
ACU_GndLhsAutomaton::match(DagNode* subject,
			   Substitution& solution,
			   Subproblem*& returnedSubproblem,
			   ExtensionInfo* extensionInfo)
{
  if (collectorFree(solution))
    {
      if (subject->symbol() == getSymbol())
	{
	  //
	  //	Non-collapse case.
	  //
	  if (safeCast(ACU_BaseDagNode*, subject)->isTree())
	    {
	      //
	      //	Red-black case.
	      //
	      ACU_TreeDagNode* s = safeCast(ACU_TreeDagNode*, subject);
	      ACU_SlowIter i;
	      if (!(s->getTree().find(stripperTerm, i)))
		return false;
	      if (collect(i, s, solution))
		{
		  returnedSubproblem = 0;
		  if (extensionInfo)
		    {
		      extensionInfo->setValidAfterMatch(true);
		      extensionInfo->setMatchedWhole(true);
		    }
		  return true;
		}
	    }
	  else
	    {
	      //
	      //	ArgVec case.
	      //
	      ACU_DagNode* s = safeCast(ACU_DagNode*, subject);
	      int pos = s->binarySearch(stripperTerm);
	      if (pos < 0)
		return false;
	      if (collect(pos, s, solution))
		{
		  returnedSubproblem = 0;
		  if (extensionInfo)
		    {
		      extensionInfo->setValidAfterMatch(true);
		      extensionInfo->setMatchedWhole(true);
		    }
		  return true;
		}
	    }
	  if (extensionInfo == 0)
	    return false;  // no extension implies true failure
	}
      else
	{
	  //
	  //	Collapse case.
	  //
	  if (!getCollapsePossible())
	    return false;
	  Assert(extensionInfo == 0 &&
		 subject->getSortIndex() != Sort::SORT_UNKNOWN,
		 "collapse to top not handled by ACU_GndLhsAutomaton");
	  if (!(stripperTerm->equal(subject)))
	    return false;
	  returnedSubproblem = 0;
	  collapse(solution);
	  return true;
	}
    }
  return ACU_LhsAutomaton::match(subject, solution, returnedSubproblem, extensionInfo);
}

#ifdef DUMP
void
ACU_GndLhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{ACU_GndLhsAutomaton}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "stripperTerm = \"" << stripperTerm << '\n';
  ACU_CollectorLhsAutomaton::dump(s, variableInfo, indentLevel);
  s << Indent(indentLevel - 1) << "End{ACU_GndLhsAutomaton}\n";
}
#endif
