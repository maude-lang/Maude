/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for DAG nodes that are variables.
//
#ifndef _variableDagNode_hh_
#define _variableDagNode_hh_
#include "dagNode.hh"
#include "namedEntity.hh"
#include "variableSymbol.hh"

class VariableDagNode : public DagNode, public NamedEntity
{
  NO_COPYING(VariableDagNode);

public:
  VariableDagNode(Symbol* symbol, int name, int index);
  
  RawDagArgumentIterator* arguments();
  size_t getHashValue();
  int compareArguments(const DagNode* other) const;
  void overwriteWithClone(DagNode* old);
  DagNode* makeClone();
  DagNode* copyWithReplacement(int argIndex, DagNode* replacement);
  DagNode* copyWithReplacement(Vector<RedexPosition>& redexStack, int first, int last);
  //
  //	Unification member functions.
  //
  ReturnResult computeBaseSortForGroundSubterms(bool warnAboutUnimplemented);
  bool computeSolvedForm2(DagNode* rhs, UnificationContext& solution, PendingUnificationStack& pending);
  void insertVariables2(NatSet& occurs);
  DagNode* instantiate2(const Substitution& substitution, bool maintainInvariants);
  //
  //	Narrowing member functions.
  //
  bool indexVariables2(NarrowingVariableInfo& indicies, int baseIndex);
  DagNode* instantiateWithCopies2(const Substitution& substitution, const Vector<DagNode*>& eagerCopies);
  //
  //	Function needed for variant matching.
  //
  void indexVariables(VariableInfo& indicies);
  //
  //	Functions specific to VariableDagNode.
  //  
  int getIndex() const;
  VariableSymbol* symbol() const;
  VariableDagNode* lastVariableInChain(Substitution& solution);

private:
  DagNode* markArguments();
  DagNode* copyEagerUptoReduced2();
  DagNode* copyAll2();
  void clearCopyPointers2();

  bool safeVirtualReplacement(VariableDagNode* oldVar,
			      VariableDagNode* newVar,
			      UnificationContext& solution,
			      PendingUnificationStack& pending);

  int index;
};

inline
VariableDagNode::VariableDagNode(Symbol* symbol, int name, int index)
  : DagNode(symbol),
    NamedEntity(name),
    index(index)
{
}

inline VariableSymbol*
VariableDagNode::symbol() const
{
  return safeCastNonNull<VariableSymbol*>(DagNode::symbol());
}

inline int
VariableDagNode::getIndex() const
{
  return index;
}

#endif
