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
//      Implementation for class ACU_VarLhsAutomaton.
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
//#include "term.hh"
#include "extensionInfo.hh"

//      core class definitions
#include "variableInfo.hh"
#include "substitution.hh"

//      variable class definitions
#include "variableSymbol.hh"
#include "variableTerm.hh"

//	ACU Red-Black class definitions
#include "ACU_SlowIter.hh"

//	ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_VarLhsAutomaton.hh"
#include "ACU_TreeDagNode.hh"

ACU_VarLhsAutomaton::ACU_VarLhsAutomaton(ACU_Symbol* symbol,
					 bool matchAtTop,
					 bool collapsePossible,
					 int nrVariables,
					 VariableTerm* stripper,
					 VariableTerm* collector)
  : ACU_CollectorLhsAutomaton(symbol,
			      matchAtTop,
			      collapsePossible,
			      nrVariables,
			      collector),
  stripperVarIndex(stripper->getIndex()),
  stripperSort(stripper->getSort()),
  trueFailure(symbol->sortBound(stripperSort) == 1 ||
	      symbol->sortStructure(stripperSort) == AssociativeSymbol::PURE_SORT)
{
  Assert(!(symbol->takeIdentity(stripperSort)),
	 "stripper variable shouldn't be able take identity");
}

bool
ACU_VarLhsAutomaton::match(DagNode* subject,
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
	  if (solution.value(stripperVarIndex) == 0)
	    {
	      if (safeCast(ACU_BaseDagNode*, subject)->isTree())
		{
		  //
		  //	Red-black case.
		  //
		  ACU_TreeDagNode* s = safeCast(ACU_TreeDagNode*, subject);
		  ACU_SlowIter i(s->getTree());
		  do
		    {
		      DagNode* d = i.getDagNode();
		      if (d->leq(stripperSort))
			{
			  if (!collect(i, s, solution))
			    goto fullMatch;  // collect() destroys i
			  solution.bind(stripperVarIndex, d);
			  returnedSubproblem = 0;
			  if (extensionInfo)
			    {
			      extensionInfo->setValidAfterMatch(true);
			      extensionInfo->setMatchedWhole(true);
			    }
			  return true;
			}
		      i.next();
		    }
		  while (i.valid());
		}
	      else
		{
		  //
		  //	ArgVec case.
		  //
		  ACU_DagNode* s = safeCast(ACU_DagNode*, subject);
		  int nrArgs = s->argArray.length();
		  int i = 0;
		  do
		    {
		      DagNode* d = s->argArray[i].dagNode;
		      if (d->leq(stripperSort))
			{
			  if (!collect(i, s, solution))
			    goto fullMatch;  // on a hiding to nothing
			  solution.bind(stripperVarIndex, d);
			  returnedSubproblem = 0;
			  if (extensionInfo)
			    {
			      extensionInfo->setValidAfterMatch(true);
			      extensionInfo->setMatchedWhole(true);
			    }
			  return true;
			}
		      ++i;
		    }
		  while (i != nrArgs);
		}
	      if (trueFailure)
		return false;
	    }
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
		 "collapse to top not handled by ACU_VarLhsAutomaton");
	  DagNode* d = solution.value(stripperVarIndex);
	  if (d == 0)
	    {
	      if (!(subject->leq(stripperSort)))
		return false;
	      solution.bind(stripperVarIndex, subject);
	    }
	  else
	    {
	      if (!(d->equal(subject)))
		return false;
	    }
	  returnedSubproblem = 0;
	  collapse(solution);
	  return true;
	}
    }
 fullMatch:
  return ACU_LhsAutomaton::match(subject, solution, returnedSubproblem, extensionInfo);
}

#ifdef DUMP
void
ACU_VarLhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{ACU_VarLhsAutomaton}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "stripperVarIndex = " << stripperVarIndex <<
    " \"" << variableInfo.index2Variable(stripperVarIndex) << '"' <<
    "\tstripperSort = \"" << stripperSort << "\"\t" <<
    "trueFailure = " << trueFailure << '\n';
  ACU_CollectorLhsAutomaton::dump(s, variableInfo, indentLevel);
  s << Indent(indentLevel - 1) << "End{ACU_VarLhsAutomaton}\n";
}
#endif
