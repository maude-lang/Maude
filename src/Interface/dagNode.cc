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
//      Implementation for base class DagNode
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "symbol.hh"
#include "binarySymbol.hh"
#include "dagNode.hh"
#include "subproblem.hh"

//	core class definitions
#include "substitution.hh"
#include "rewritingContext.hh"
#include "sortCheckSubproblem.hh"
#include "dagArgumentIterator.hh"
#include "module.hh"
#include "rootContainer.hh"
#include "sortBdds.hh"

bool
DagNode::checkSort(const Sort* boundSort, Subproblem*& returnedSubproblem)
{
  returnedSubproblem = 0;
  if (getSortIndex() != Sort::SORT_UNKNOWN)
    return leq(boundSort);
  topSymbol->computeBaseSort(this);
  if (leq(boundSort))
    {
      if (!(topSymbol->sortConstraintFree()))
	setSortIndex(Sort::SORT_UNKNOWN);
    }
  else
    { 
      if (topSymbol->sortConstraintFree())
	return false;
      setSortIndex(Sort::SORT_UNKNOWN);
      returnedSubproblem = new SortCheckSubproblem(this, boundSort);
    }
  return true;
}

bool
DagNode::checkSort(const Sort* boundSort, RewritingContext& context)
{
  if (getSortIndex() == Sort::SORT_UNKNOWN)
    {
      topSymbol->computeBaseSort(this);
      if (leq(boundSort))
	{
	  if (!(topSymbol->sortConstraintFree()))
	    setSortIndex(Sort::SORT_UNKNOWN);
	  return true;
	}
      else
	{
	  if (topSymbol->sortConstraintFree())
	    return false;
	  RewritingContext* local = context.makeSubcontext(this, RewritingContext::SORT_EVAL);
	  topSymbol->constrainToSmallerSort(this, *local);
	  context.addInCount(*local);
	  delete local;
	}
    }
  return leq(boundSort);
}

void
DagNode::computeGeneralizedSort(const SortBdds& sortBdds,
				const Vector<int> realToBdd,
				Vector<Bdd>& generalizedSort)
{
  int sortIndex = getSortIndex();
  if (sortIndex == Sort::SORT_UNKNOWN)
    symbol()->computeGeneralizedSort(sortBdds, realToBdd, this, generalizedSort);
  else
    {
      int nrBdds = sortBdds.getNrVariables(symbol()->rangeComponent()->getIndexWithinModule());
      sortBdds.makeIndexVector(nrBdds, sortIndex, generalizedSort);
    }
}

bool
DagNode::matchVariable(int index,
		       const Sort* sort,
		       bool copyToAvoidOverwriting,
		       Substitution& solution,
		       Subproblem*& returnedSubproblem,
		       ExtensionInfo* extensionInfo)
{
  if (extensionInfo != 0)
    return matchVariableWithExtension(index, sort, solution, returnedSubproblem, extensionInfo);
  DagNode* d = solution.value(index);
  if (d == 0)
    {
      if (checkSort(sort, returnedSubproblem))
        {
          solution.bind(index, copyToAvoidOverwriting ? makeClone() : this);
          return true;
        }
    }
  else
    {
      if (compare(d) == 0)
        {
          returnedSubproblem = 0;
          return true;
        }
    }
  return false;
}

ExtensionInfo*
DagNode::makeExtensionInfo()
{
  return 0;
}

//
//	Dummy functions to allow linking of theories which don't use extension.
//

bool
DagNode::matchVariableWithExtension(int /* index */,
				    const Sort* /* sort */,
				    Substitution& /* solution */,
				    Subproblem*& /* returnedSubproblem */,
				    ExtensionInfo* /* extensionInfo */)
{
  CantHappen("Called on subject " << this);
  return false;
}

void
DagNode::partialReplace(DagNode* /* replacement */, ExtensionInfo* /* extensionInfo */)
{
  CantHappen("Called on subject " << this);
}

DagNode*
DagNode::partialConstruct(DagNode* /* replacement */, ExtensionInfo* /* extensionInfo */)
{
  CantHappen("Called on subject " << this);
  return 0;
}

#ifdef DUMP
void
DagNode::dump(ostream& s, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{DagNode}\n";
  ++indentLevel;
  dumpCommon(s, indentLevel);
  s << Indent(indentLevel) << "arguments:\n";
  ++indentLevel;
  for (DagArgumentIterator a(*this); a.valid(); a.next())
    a.argument()->dump(s, indentLevel);
  s << Indent(indentLevel - 2) << "End{DagNode}\n";
}

void
DagNode::dumpCommon(ostream& s, int indentLevel)
{
  s << Indent(indentLevel) << "topSymbol = " << topSymbol << '\n';
}
#endif
