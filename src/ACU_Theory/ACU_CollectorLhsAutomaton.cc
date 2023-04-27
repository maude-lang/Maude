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
//      Implementation for class ACU_CollectorLhsAutomaton.
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

//	ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_TreeDagNode.hh"
#include "ACU_CollectorLhsAutomaton.hh"

ACU_CollectorLhsAutomaton::ACU_CollectorLhsAutomaton(ACU_Symbol* symbol,
						     bool matchAtTop,
						     bool collapsePossible,
						     int nrVariables,
						     VariableTerm* collector)
  : ACU_LhsAutomaton(symbol, matchAtTop, collapsePossible, nrVariables),
    collectorVarIndex(collector->getIndex())
{
  Assert(symbol->sortConstraintFree(), "not sort constraint free");

  collectorSort = collector->getSort();
  Assert(symbol->sortBound(collectorSort) == UNBOUNDED,
	 "collector must have unbounded sort");

  int index = collectorSort->index();
  if (index == 0)
    collectorSort = 0;  // kind can take anything
  else if (index == 1)
    {
      ConnectedComponent* c = collectorSort->component();
      if (c->nrMaximalSorts() == 1 && c->errorFree())
	collectorSort = 0;  // top sort in an error-free component can take anything
    }
}


bool
ACU_CollectorLhsAutomaton::finishTreeCollect(bool reduced, ACU_TreeDagNode* result, Substitution& solution) const
{
  ACU_Symbol* topSymbol = result->symbol();
  const Sort* cs = collectorSort;
  if (cs == 0)
    {
      //
      //	The collector sort is maximal (error-free kind) or
      //	the error sort. Either way sort checks are not needed.
      //
      if (reduced)
	{
	  int index = topSymbol->getUniqueSortIndex();
	  Assert(index != 0, "bad uniqueSortIndex");
	  if (index < 0)
	    index = result->getTree().computeBaseSort(topSymbol);
	  result->setSortIndex(index);
	  result->setReduced();
	}
    }
  else
    {
      //
      //	Need to check sort of the dag we are going to
      //	bind to the collector variable.
      //
      int index = result->getTree().computeBaseSort(topSymbol);
      if (!leq(index, cs))
	return false;
      if (reduced)
	{
	  result->setSortIndex(index);
	  result->setReduced();
	}
    }
  solution.bind(collectorVarIndex, result);
  return true;
}

bool
ACU_CollectorLhsAutomaton::collect(int stripped,
				   ACU_DagNode* subject,
				   Substitution& solution) const
{
  int nrArgs = subject->argArray.length();
  if (nrArgs <= 2)
    {
      //
      //	Check for case where there's only one arg left after stripping.
      //
      int m = subject->argArray[0].multiplicity;
      int t = 0;
      if (nrArgs == 2)
	{
	  m += subject->argArray[1].multiplicity;
	  t = 1 - stripped;
	}
      if (m == 2)
	{
	  DagNode* sd = subject->argArray[t].dagNode;
	  if (collectorSort == 0 || sd->leq(collectorSort))
	    {
	      solution.bind(collectorVarIndex, sd);
	      return true;
	    }
	  else
	    return false;
	}
    }
  //
  //	Make binding for collector variable.
  //
  ArgVec<ACU_DagNode::Pair>::const_iterator source = subject->argArray.begin();
  const ArgVec<ACU_DagNode::Pair>::const_iterator e = subject->argArray.end();
  const ArgVec<ACU_DagNode::Pair>::const_iterator victim = source + stripped;
  int strippedMultiplicity = victim->multiplicity - 1;
  if (strippedMultiplicity == 0)
    --nrArgs;
  ACU_Symbol* topSymbol = subject->symbol();
  ACU_DagNode* d = new ACU_DagNode(topSymbol, nrArgs, ACU_DagNode::ASSIGNMENT);
  ArgVec<ACU_DagNode::Pair>::iterator dest = d->argArray.begin();
  for (; source != victim; ++dest, ++source)
    *dest = *source;
  if (strippedMultiplicity > 0)
    {
      dest->dagNode = source->dagNode;
      dest->multiplicity = strippedMultiplicity;
      ++dest;
    }
  for (++source; source != e; ++dest, ++source)
    *dest = *source;
  Assert(dest == d->argArray.end(), "iterators inconsistant");
  //
  //	Check if we want to make a tree.
  //
  bool reduced = subject->isReduced();
  if (topSymbol->useTree() && nrArgs >= COLLECT_THRESHOLD)
    {
      //
      //	Transform our new ACU_DagNode into an ACU_TreeDagNode.
      //
      ACU_Tree t(d->argArray);
      return finishTreeCollect(reduced, new (d) ACU_TreeDagNode(topSymbol, t), solution);
    }
  //
  //	Do any sort calculations needed.
  //
  const Sort* cs = collectorSort;
  if (cs == 0)
    {
      //
      //	The collector sort is maximal (error-free kind) or the error sort.
      //	Either way sort checks are not needed, but if the subject was reduced
      //	its ACU subterm can also be marked as reduced after we calculate the sort.
      //
      if (reduced)
	{
	  int index = topSymbol->getUniqueSortIndex();
	  Assert(index != 0, "bad uniqueSortIndex");
	  if (index < 0)
	    index = d->argVecComputeBaseSort();
	  d->setSortIndex(index);
	  d->setReduced();
	}
    }
  else
    {
      int index;
      const Sort* u = topSymbol->uniformSort();
      if (u != nullptr && reduced && subject->getSortIndex() != Sort::ERROR_SORT)
	{
	  //
	  //	The symbol topSymbol has a uniform sort u, so barring sort constraints, any term which
	  //	it heads must have sort u if all the subterms have sort <= u and the error sort otherwise.
	  //
	  //	Because the subject was reduced, it must have had its sort computed, and in the
	  //	absence of sort constraints, since it doesn't have the error sort, it must have sort u.
	  //	Thus all of its subterms have sorts <= u and thus our proposed binding, d, must have sort u.
	  //		  
	  index = u->index();
	  Assert(subject->getSortIndex() == index, "bad sort index " << subject->getSortIndex() <<
		     " was expecting " << index << " for " << u);
	}
      else
	index = d->argVecComputeBaseSort();
      if (!leq(index, cs))
	return false;
      if (reduced)
	{
	  d->setSortIndex(index);
	  d->setReduced();
	}
    }
  solution.bind(collectorVarIndex, d);
  return true;
}

bool
ACU_CollectorLhsAutomaton::collect(ACU_Stack& stripped,  // destroyed
				   ACU_TreeDagNode* subject,
				   Substitution& solution) const
{
  ACU_Tree t(subject->getTree());
  t.deleteMult(stripped, 1);
  DagNode* d;
  const Sort* cs = collectorSort;
  if (t.getSize() == 1 && t.getMaxMult() == 1)
    {
      //
      //	Only one argument left for collector.
      //
      d = t.getSoleDagNode();
      if (cs != 0 && !leq(d->getSortIndex(), cs))
	return false;
    }
  else
    {
      ACU_Symbol* topSymbol = subject->symbol();
      if (cs == 0)
	{
	  //
	  //	The collector sort is maximal (error-free kind) or
	  //	the error sort. Either way sort checks are disabled.
	  //
	  d = new ACU_TreeDagNode(topSymbol, t);
	  if (subject->isReduced())
	    {
	      int index = topSymbol->getUniqueSortIndex();
	      Assert(index != 0, "bad uniqueSortIndex");
	      if (index < 0)
		index = t.computeBaseSort(topSymbol);
	      d->setSortIndex(index);
	      d->setReduced();
	    }
	}
      else
	{
	  //
	  //	Need to check sort of the dag we are going to
	  //	bind to the collector variable.
	  //
	  int index = t.computeBaseSort(topSymbol);
	  if (!leq(index, cs))
	    return false;
	  d = new ACU_TreeDagNode(topSymbol, t);
	  if (subject->isReduced())
	    {
	      d->setSortIndex(index);
	      d->setReduced();
	    }
	}
    }
  solution.bind(collectorVarIndex, d);
  return true;
}

#ifdef DUMP
void
ACU_CollectorLhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "collectorVarIndex = " << collectorVarIndex <<
    " \"" << variableInfo.index2Variable(collectorVarIndex) <<
    "\"\tcollectorSort = ";
  if (collectorSort == 0)
    s << "(sort checks off)\n";
  else
    s << collectorSort << '\n';
  ACU_LhsAutomaton::dump(s, variableInfo, indentLevel);
}
#endif
