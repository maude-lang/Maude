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
//	Class for DAG nodes belonging to theories made from C, Ul, Ur and I axioms.
//
#ifndef _CUI_DagNode_hh_
#define _CUI_DagNode_hh_
#include "dagNode.hh"
#include "variable.hh"

class CUI_DagNode : public DagNode
{
public:
  CUI_DagNode(CUI_Symbol* symbol);
  //
  //	Member functions required by theory interface.
  //
  RawDagArgumentIterator* arguments();
  size_t getHashValue();
  int compareArguments(const DagNode* other) const;
  void overwriteWithClone(DagNode* old);
  DagNode* makeClone();
  DagNode* copyWithReplacement(int argIndex, DagNode* replacement);
  DagNode* copyWithReplacement(Vector<RedexPosition>& redexStack,
			       int first,
			       int last);
  //
  //	Unification member functions.
  //
  ReturnResult computeBaseSortForGroundSubterms(bool warnAboutUnimplemented);
  bool computeSolvedForm2(DagNode* rhs,  UnificationContext& solution, PendingUnificationStack& pending);
  void insertVariables2(NatSet& occurs);
  DagNode* instantiate2(const Substitution& substitution);
  //
  //	Interface for narrowing.
  //
  bool indexVariables2(NarrowingVariableInfo& indices, int baseIndex);
  DagNode* instantiateWithReplacement(const Substitution& substitution,
				      const Vector<DagNode*>* eagerCopies,
				      int argIndex,
				      DagNode* newDag);
  DagNode* instantiateWithCopies2(const Substitution& substitution, const Vector<DagNode*>& eagerCopies);
  //
  //    Functions particular to CUI_DagNode.
  //
  CUI_Symbol* symbol() const;
  DagNode* getArgument(int i) const;

  bool indirectOccursCheck(VariableDagNode* repVar, UnificationContext& solution);
  CUI_DagNode* makePurifiedVersion(UnificationContext& solution, PendingUnificationStack& pending);

private:
  //
  //	Theory interface functions.
  //
  DagNode* markArguments();
  DagNode* copyEagerUptoReduced2();
  DagNode* copyAll2();
  void clearCopyPointers2();
  //
  //	Normalization functions.
  //
  void collapseTo(int argNr);
  bool normalizeAtTop();
  //
  //	Private unification stuff.
  //
  bool computeSolvedFormCommutativeCase(CUI_DagNode* rhs,
					UnificationContext& solution,
					PendingUnificationStack& pending);
  //
  //	Arguments under CUI symbol.
  //
  DagNode* argArray[2];
  //
  //	Slot for caching hash value.
  //
  size_t hashCache;
  //
  //	Most related classes need direct access to our argument list for efficiency.
  //
  friend class CUI_Symbol;		// to reduce subterms prior to rewrite, normalization
  friend class CUI_Term;		// for term->DAG conversion & comparison
  friend class CUI_LhsAutomaton;	// for matching DAG subject
  friend class CUI_RhsAutomaton;	// for constructing replacement DAG
};

inline
CUI_DagNode::CUI_DagNode(CUI_Symbol* symbol)
  : DagNode(symbol)
{ 
}

inline CUI_Symbol*
CUI_DagNode::symbol() const
{
  return static_cast<CUI_Symbol*>(DagNode::symbol());
}

inline DagNode*
CUI_DagNode::getArgument(int i) const
{
  Assert(i == 0 || i == 1, "bad argument index");
  return argArray[i];
}

#endif
