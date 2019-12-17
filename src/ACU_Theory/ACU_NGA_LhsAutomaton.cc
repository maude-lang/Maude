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
//      Implementation for class ACU_NGA_LhsAutomaton.
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
#include "subproblem.hh"
#include "extensionInfo.hh"

//      core class definitions
#include "substitution.hh"

//	ACU Red-Black class definitions
#include "ACU_SlowIter.hh"

//	ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_TreeDagNode.hh"
#include "ACU_NGA_LhsAutomaton.hh"

ACU_NGA_LhsAutomaton::ACU_NGA_LhsAutomaton(ACU_Symbol* symbol,
					   bool matchAtTop,
					   bool collapsePossible,
					   int nrVariables,
					   LhsAutomaton* stripperAutomaton,
					   Term* stripperTerm,
					   VariableTerm* collector)
  : ACU_CollectorLhsAutomaton(symbol,
			      matchAtTop,
			      collapsePossible,
			      nrVariables,
			      collector),
  stripperAutomaton(stripperAutomaton),
  stripperTerm(stripperTerm)
{
  Assert(!(stripperTerm->ground()), "stripper term must be non-ground");
  Assert(stripperTerm->stable(), "stripper term must be stable");
}

ACU_NGA_LhsAutomaton::~ACU_NGA_LhsAutomaton()
{
  delete stripperAutomaton;
}

bool
ACU_NGA_LhsAutomaton::match(DagNode* subject,
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
	      if (s->getTree().findFirstPotentialMatch(stripperTerm,
						       solution,
						       i))
		{
		  DagNode* d = i.getDagNode();
		  Substitution& local = getLocal();
		  for (;;)
		    {
		      local.copy(solution);
		      if (stripperAutomaton->match(d, local, returnedSubproblem))
			{
			  if (returnedSubproblem == 0)
			    {
			      if (!collect(i, s, local))
				goto fullMatch;  // collect() destroys i
			      if (extensionInfo)
				{
				  extensionInfo->setValidAfterMatch(true);
				  extensionInfo->setMatchedWhole(true);
				}
			      solution.copy(local);
			      return true;
			    }
			  else
			    {
			      delete returnedSubproblem;
			      goto fullMatch;
			    }
			}
		      i.next();
		      if (!i.valid())
			break;
		      d = i.getDagNode();
		      if (stripperTerm->partialCompare(solution, d) == Term::LESS)
			{
			  //
			  //	stripperTerm is less than d, so it will be less than
			  //	all next arguments and we can quit.
			  //
			  break;
			}
		    }
		}
	    }
	  else
	    {
	      //
	      //	ArgVec case.
	      //
	      ACU_DagNode* s = safeCast(ACU_DagNode*, subject);
	      int nrArgs = s->argArray.length();
	      int i = s->findFirstPotentialMatch(stripperTerm, solution);
	      if (i < nrArgs)
		{
		  DagNode* d = s->argArray[i].dagNode;
		  Substitution& local = getLocal();
		  for (;;)
		    {
		      local.copy(solution);
		      if (stripperAutomaton->match(d, local, returnedSubproblem))
			{
			  if (returnedSubproblem == 0)
			    {
			      if (!collect(i, s, local))
				goto fullMatch;  // on a hiding to nothing
			      if (extensionInfo)
				{
				  extensionInfo->setValidAfterMatch(true);
				  extensionInfo->setMatchedWhole(true);
				}
			      solution.copy(local);
			      return true;
			    }
			  else
			    {
			      delete returnedSubproblem;
			      goto fullMatch;
			    }
			}
		      ++i;
		      if (i >= nrArgs)
			break;
		      d = s->argArray[i].dagNode;
		      if (stripperTerm->partialCompare(solution, d) == Term::LESS)
			{
			  //
			  //	stripperTerm is less than d, so it will be less than
			  //	all next arguments and we can quit.
			  //
			  break;
			}
		    }
		}
	    }
	  return false;
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
		 "collapse to top not handled by ACU_NGA_LhsAutomaton");
	  if (!(stripperAutomaton->match(subject, solution, returnedSubproblem)))
	    return false;
	  collapse(solution);
	  return true;
	}
    }
 fullMatch:
  return ACU_LhsAutomaton::match(subject, solution, returnedSubproblem, extensionInfo);
}

#ifdef DUMP
void
ACU_NGA_LhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{ACU_NGA_LhsAutomaton}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "\tstripperTerm = \"" << stripperTerm << "\"\n";
  s << Indent(indentLevel) << "stripperAutomaton =\n";
  stripperAutomaton->dump(s, variableInfo, indentLevel + 1);
  ACU_CollectorLhsAutomaton::dump(s, variableInfo, indentLevel);
  s << Indent(indentLevel - 1) << "End{ACU_NGA_LhsAutomaton}\n";
}
#endif
