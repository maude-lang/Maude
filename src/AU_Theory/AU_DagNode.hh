/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for DAG nodes in the A, AUl, AUr and AU theories.
//
#ifndef _AU_DagNode_hh_
#define _AU_DagNode_hh_
#include "AU_BaseDagNode.hh"
#include "argVec.hh"

class AU_DagNode : public AU_BaseDagNode
{
  NO_COPYING(AU_DagNode);

public:
  AU_DagNode(AU_Symbol* symbol, int size);
  //
  //	Functions required by theory interface.
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
  //	Functions required to handle extension information.
  //
  bool matchVariableWithExtension(int index,
				  const Sort* sort,
				  Substitution& solution,
				  Subproblem*& returnedSubproblem,
				  ExtensionInfo* extensionInfo);
  void partialReplace(DagNode* replacement, ExtensionInfo* extensionInfo);
  DagNode* partialConstruct(DagNode* replacement, ExtensionInfo* extensionInfo);
  ExtensionInfo* makeExtensionInfo();
  //
  //	Although we currently don't support unification or narrowing in AU nodes
  //	we still need some functionality from the unification and narrowing interfaces
  //	to allow narrowing to happen below us or in a sibling branch.
  //
  //	Unification member functions.
  //
  DagNode* instantiate2(const Substitution& substitution, bool maintainInvariants);
  //
  //	Supported for A only.
  //
  ReturnResult computeBaseSortForGroundSubterms(bool warnAboutUnimplemented);
  bool computeSolvedForm2(DagNode* rhs, UnificationContext& solution, PendingUnificationStack& pending);
  void insertVariables2(NatSet& occurs);

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
  //	Functions particular to AU_DagNode.
  //
  void setProducedByAssignment();

private:
  enum NormalizationResult
  {
    COLLAPSED,
    DEQUED,
    NORMAL,
    FLATTENED
  };
  //
  //	Functions required by theory interface.
  //
  DagNode* markArguments();
  DagNode* copyEagerUptoReduced2();
  DagNode* copyAll2();
  void clearCopyPointers2();
  //
  //	Functions particular to AU_DagNode.
  //
  bool disappear(AU_Symbol* s, ArgVec<DagNode*>::const_iterator i);
  NormalizationResult normalizeAtTop(bool dumb = false);
  bool eliminateForward(DagNode* target, int& pos, int limit) const;
  bool eliminateBackward(DagNode* target, int& pos, int limit) const;
  DagNode* makeFragment(int start, int nrSubterms, bool extraId) const;

  ArgVec<DagNode*> argArray;

  friend class AU_Symbol;           	// to reduce subterms prior to rewriting
  friend class AU_Term;          	// for term->DAG conversion & comparison
  friend class AU_LhsAutomaton;	      	// for matching DAG subject
  friend class AU_RhsAutomaton;		// for constructing replacement DAG
  friend class AU_Layer;		// for constructing substitution
  friend class AU_Subproblem;		// for constructing substitution
  friend class AU_ExtensionInfo;	// for constructing matched portion
  friend class AU_DequeDagNode;		// for conversion & comparison
  friend class AU_UnificationSubproblem;
  friend class AU_UnificationSubproblem2;
};

AU_DagNode* getAU_DagNode(DagNode* dagNode);

inline
AU_DagNode::AU_DagNode(AU_Symbol* symbol, int size)
  : AU_BaseDagNode(symbol),
    argArray(size)
{
  setNormalizationStatus(FRESH);
}

inline void
AU_DagNode::setProducedByAssignment()
{
  setNormalizationStatus(ASSIGNMENT);

#ifndef NO_ASSERT
  //
  //	Look for Riesco 1/18/10 bug.
  //
  for (int i = 0; i < argArray.length(); ++i)
    {
      DagNode* d = argArray[i];
      Assert(d->getSortIndex() != Sort::SORT_UNKNOWN,
	     "AU_DagNode::setProducedByAssignment(): unknown sort for AU argument " << d <<
	     " at index " << i << " in subject " << this);
    }
#endif

}

#endif
