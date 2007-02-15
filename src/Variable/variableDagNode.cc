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
//      Implementation for class VariableDagNode
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      core class definitions
#include "substitution.hh"

//	variable class definitions
#include "variableSymbol.hh"
#include "variableDagNode.hh"

RawDagArgumentIterator*
VariableDagNode::arguments()
{
  return 0;
}

size_t
VariableDagNode::getHashValue()
{
  return hash(symbol()->getHashValue(), id());
}

int
VariableDagNode::compareArguments(const DagNode* other) const
{
  return id() - safeCast(const VariableDagNode*, other)->id();
}

DagNode*
VariableDagNode::markArguments()
{
  return 0;
}

DagNode*
VariableDagNode::copyEagerUptoReduced2()
{
  return new VariableDagNode(symbol(), id(), index);
}

void
VariableDagNode::clearCopyPointers2()
{
}

void
VariableDagNode::overwriteWithClone(DagNode* old)
{
  
  VariableDagNode* d = new(old) VariableDagNode(symbol(), id(), index);
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
}

DagNode*
VariableDagNode::makeClone()
{
  VariableDagNode* d = new VariableDagNode(symbol(), id(), index);
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
  return d;
}

DagNode*
VariableDagNode::copyWithReplacement(int /* argIndex */, DagNode* /* replacement */)
{
  CantHappen("should never be called");
  return 0;
}

DagNode*
VariableDagNode::copyWithReplacement(Vector<RedexPosition>& /* redexStack  */,
				     int /* first */,
				     int /* last */)
{
  CantHappen("should never be called");
  return 0;
}

void
VariableDagNode::stackArguments(Vector<RedexPosition>& /* stack */,
				int /* parentIndex */,
				bool /* respectFrozen */)
{
}

bool
VariableDagNode::unify(DagNode* rhs,
		       Substitution& solution,
		       Subproblem*& returnedSubproblem,
		       ExtensionInfo* extensionInfo)
{
  if (DagNode* b = solution.value(index))
    return b->unify(rhs, solution, returnedSubproblem, 0);
  returnedSubproblem = 0;
  return solution.unificationBind(index, safeCast(VariableSymbol*, symbol())->getSort(), rhs);
}

bool
VariableDagNode::computeBaseSortForGroundSubterms()
{
  return false;
}

DagNode*
VariableDagNode::instantiate2(Substitution& substitution)
{
  return substitution.value(index);
}

bool
VariableDagNode::occurs2(int index)
{
  return index == this->index;
}
