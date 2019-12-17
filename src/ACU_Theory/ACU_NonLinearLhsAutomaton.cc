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
//      Implementation for class ACU_NonLinearLhsAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "indent.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "ACU_Persistent.hh"
#include "ACU_Theory.hh"

//      interface class definitions
#include "associativeSymbol.hh"
#include "dagNode.hh"
#include "term.hh"

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
#include "ACU_TreeDagNode.hh"
#include "ACU_NonLinearLhsAutomaton.hh"
#include "ACU_ExtensionInfo.hh"

ACU_NonLinearLhsAutomaton::ACU_NonLinearLhsAutomaton(ACU_Symbol* symbol,
						     int nrVariables,
						     int varIndex,
						     int multiplicity,
						     Sort* varSort)
  : ACU_LhsAutomaton(symbol, true, false, nrVariables),
    varIndex(varIndex),
    multiplicity(multiplicity),
    varSort(varSort),
    unitSort(symbol->sortBound(varSort) == 1),
    pureSort(symbol->sortStructure(varSort) == AssociativeSymbol::PURE_SORT)
{
  Assert(unitSort || pureSort ||
	 symbol->sortStructure(varSort) == AssociativeSymbol::LIMIT_SORT,
	 "bad sort");
  Assert(multiplicity >= 2, "bad multiplicity");
}

void
ACU_NonLinearLhsAutomaton::
fillOutExtensionInfo(ACU_DagNode* subject,
		     const ArgVec<ACU_DagNode::Pair>::const_iterator chosen,
		     ACU_ExtensionInfo* extensionInfo)
{
  int nrArgs = subject->argArray.length();
  ArgVec<ACU_DagNode::Pair>::const_iterator args = subject->argArray.begin();

  if (nrArgs == 1 && args->multiplicity == multiplicity)
    extensionInfo->setMatchedWhole(true);
  else
    {
      extensionInfo->setMatchedWhole(false);
      extensionInfo->reset();
      for (int i = 0; i < nrArgs; i++, ++args)
	{
	  int m = args->multiplicity;
	  extensionInfo->setUnmatched(i, args == chosen ? (m - multiplicity) : m);
	}
      Assert(args == subject->argArray.end(), "iterator inconsistent");
    }
  extensionInfo->setValidAfterMatch(true);
}

bool
ACU_NonLinearLhsAutomaton::match(DagNode* subject,
				 Substitution& solution,
				 Subproblem*& returnedSubproblem,
				 ExtensionInfo* extensionInfo)
{
  Assert(extensionInfo != 0, "should only be called in extension case");

  if (subject->symbol() != getSymbol())
    return false;
  if (solution.value(varIndex) != 0)
    goto undecided;
  returnedSubproblem = 0;

  if (safeCast(ACU_BaseDagNode*, subject)->isTree())
    {
      //
      //	Red-black case.
      //
      ACU_Tree t(safeCast(ACU_TreeDagNode*, subject)->getTree());
      if (t.getMaxMult() < multiplicity)
	return false;
      DagNode* d = makeHighMultiplicityAssignment(multiplicity, varSort, t);
      if (d == 0)
	{
	  //
	  //	Because makeHighMultiplicityAssignment() just looks at
	  //	first argument with enough multiplicity we must assume we
	  //	may have missed a match.
	  //
	  goto undecided;
	}
      solution.bind(varIndex, d);
      ACU_ExtensionInfo* e = safeCast(ACU_ExtensionInfo*, extensionInfo);
      e->setValidAfterMatch(true);
      int size = t.getSize();
      if (size == 0)
	e->setMatchedWhole(true);
      else
	{
	  e->setMatchedWhole(false);
	  if (size == 1 && t.getMaxMult() == 1)
	    e->setUnmatched(t.getSoleDagNode());
	  else
	    e->setUnmatched(new ACU_TreeDagNode(getSymbol(), t));
	}
      return true;
    }
  else
    {
      //
      //	ArgVec case.
      //
      ACU_DagNode* s = safeCast(ACU_DagNode*, subject);
      int fastMult = multiplicity;  // register copy to avoid reloading after writes
      ACU_ExtensionInfo* e = safeCast(ACU_ExtensionInfo*, extensionInfo);

      if (unitSort)
	{
	  //
	  //	Can only assign one subject.
	  //
	  FOR_EACH_CONST(i, ArgVec<ACU_DagNode::Pair>, s->argArray)
	    {
	      if (i->multiplicity >= fastMult && i->dagNode->leq(varSort))
		{
		  solution.bind(varIndex, i->dagNode);
		  fillOutExtensionInfo(s, i, e);
		  return true;
		}
	    }
	  return false;
	}
      else
	{
	  //
	  //	First find out how many subjects are assignable.
	  //
	  int size = 0;
	  ArgVec<ACU_DagNode::Pair>::const_iterator last;  // gcc gives uninitialized warning
	  FOR_EACH_CONST(i, ArgVec<ACU_DagNode::Pair>, s->argArray)
	    {
	      if (i->multiplicity >= fastMult && i->dagNode->leq(varSort))
		{
		  ++size;
		  last = i;
		}
	    }
	  if (size == 0)
	    {
	      if (pureSort)
		return false;
	      goto undecided;
	    }
	  //
	  //	Now make binding.
	  //
	  if (size == 1 && last->multiplicity < 2 * fastMult)
	    {
	      solution.bind(varIndex, last->dagNode);
	      fillOutExtensionInfo(s, last, e);
	    }
	  else
	    {
	      ACU_DagNode* d = new ACU_DagNode(getSymbol(), size, ACU_DagNode::ASSIGNMENT);
	      ArgVec<ACU_DagNode::Pair>::iterator j = d->argArray.begin();
	      e->reset();
	      bool whole = true;
	      FOR_EACH_CONST(i, ArgVec<ACU_DagNode::Pair>, s->argArray)
		{
		  int m = i->multiplicity;
		  if (m >= fastMult && i->dagNode->leq(varSort))
		    {
		      int rem = m % fastMult;
		      int result = m / fastMult;
		      j->dagNode = i->dagNode;;
		      j->multiplicity = result;
		      ++j;
		      e->setUnmatched(i - s->argArray.begin(), rem);
		      if (rem != 0)
			whole = false;
		    }
		  else
		    {
		      e->setUnmatched(i - s->argArray.begin(), m);
		      whole = false;
		    }
		}
	      Assert(j == d->argArray.end(), "arg list mismatch");
	      e->setMatchedWhole(whole);
	      e->setValidAfterMatch(true);
	      if (s->isReduced() && getSymbol()->sortConstraintFree())
		{
		  getSymbol()->computeBaseSort(d);
		  d->setReduced();
		}
	      solution.bind(varIndex, d);
	    }
	  return true;
	}
    }
 undecided:
  return ACU_LhsAutomaton::match(subject,
				 solution,
				 returnedSubproblem,
				 extensionInfo);
}

#ifdef DUMP
void
ACU_NonLinearLhsAutomaton::dump(ostream& s,
				const VariableInfo& variableInfo,
				int indentLevel)
{
  s << Indent(indentLevel) << "Begin{ACU_NonLinearLhsAutomaton}\n";
  ++indentLevel;
  s << Indent(indentLevel) <<
    "varIndex = " << varIndex << " \"" << variableInfo.index2Variable(varIndex) <<
    "\tmultiplicity = " << multiplicity << '\n';
  s << Indent(indentLevel) << "varSort = " << varSort <<
    "\tunitSort = " << unitSort <<
    "\tpureSort = " << pureSort << '\n';
  ACU_LhsAutomaton::dump(s, variableInfo, indentLevel);
  s << Indent(indentLevel - 1) << "End{ACU_NonLinearLhsAutomaton}\n";
}
#endif
