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
//	Class for ArgVec based DAG nodes in the AC and ACU theories.
//
#ifndef _ACU_DagNode_hh_
#define _ACU_DagNode_hh_
#include "ACU_BaseDagNode.hh"
#include "ACU_Pair.hh"
#include "argVec.hh"
#include "ACU_FastIter.hh"

#if 1
extern int CONVERT_THRESHOLD;
extern int MERGE_THRESHOLD;
#endif

class ACU_DagNode : public ACU_BaseDagNode
{
public:
  ACU_DagNode(ACU_Symbol* symbol, int size, NormalizationStatus status = FRESH);
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
  void stackArguments(Vector<RedexPosition>& stack,
		      int parentIndex,
		      bool respectFrozen);
  //
  //	Member functions required to handle extension information.
  //
  bool matchVariableWithExtension(int index,
				  const Sort* sort,
				  Substitution& solution,
				  Subproblem*& returnedSubproblem,
				  ExtensionInfo* extensionInfo);
  void partialReplace(DagNode* replacement, ExtensionInfo* extensionInfo);
  DagNode* partialConstruct(DagNode* replacement, ExtensionInfo* extensionInfo);
  //
  //	Unification member functions.
  //
  ReturnResult computeBaseSortForGroundSubterms();
  bool computeSolvedForm2(DagNode* rhs, UnificationContext& solution, PendingUnificationStack& pending);
  void insertVariables2(NatSet& occurs);
  DagNode* instantiate2(const Substitution& substitution);
  //
  //	Interface for narrowing.
  //
  bool indexVariables2(NarrowingVariableInfo& indices, int baseIndex);
  DagNode* instantiateWithReplacement(const Substitution& substitution, int argIndex, DagNode* newDag);
  //
  //    Functions particular to ACU_DagNode.
  //
  int nrArgs() const;
  DagNode* getArgument(int i) const;
  int getMultiplicity(int i) const;
  int argVecComputeBaseSort() const;

private:
  enum Sizes
  {
#if 1
    CONVERT_THRESHOLD = 8,
    MERGE_THRESHOLD = 16,
#endif

    INITIAL_RUNS_BUFFER_SIZE = 4	// must be > 0
  };
  
  typedef ACU_Pair Pair;

  //
  //	Theory interface functions.
  //
  DagNode* markArguments();
  DagNode* copyEagerUptoReduced2();
  void clearCopyPointers2();
  //
  //	Normalization functions.
  //
  static ArgVec<Pair>::iterator fastCopy(ArgVec<Pair>::const_iterator i,
					 ArgVec<Pair>::const_iterator e,
					 ArgVec<Pair>::iterator d);
  static ArgVec<Pair>::iterator fastCopy(ACU_FastIter& i,
					 ArgVec<Pair>::iterator d);

  bool normalizeAtTop();
  bool dumbNormalizeAtTop();
  void copyAndBinaryInsert(const ACU_DagNode* source, DagNode* dagNode, int multiplicity);
  void fastMerge(const ACU_DagNode* source0, const ACU_DagNode* source1);
  void fastMerge(const ACU_DagNode* source0, const ACU_TreeDagNode* source1);
  void fastMerge(const ACU_TreeDagNode* source0, const ACU_TreeDagNode* source1);

  void sortAndUniquize();
  void flattenSortAndUniquize(int expansion);
  void mergeSortAndUniquize();

  void collapse(DagNode* arg);
  void insertAlien(ACU_BaseDagNode* normalForm,
		   int nMult,
		   DagNode* alien,
		   int aMult);
  //
  //	Functions for ACU specific operations.
  //
  int findFirstPotentialMatch(Term* key, const Substitution& partial) const;
  int binarySearch(DagNode* key) const;
  int binarySearch(Term* key) const;
  bool eliminateSubject(DagNode* target,
			int multiplicity,
			Vector<int>& subjectMultiplicity);
  bool eliminateArgument(Term* target);
  //
  //	Arguments under ACU symbol.
  //
  ArgVec<Pair> argArray;
  //
  //	Pre-allocated buffer for run-time use.
  //
  static Vector<int> runsBuffer;
  //
  //	Most related classes need direct access to our argument list for efficiency.
  //
  friend class ACU_Symbol;		// to reduce subterms prior to rewrite, normalization
  friend class ACU_Term;		// for term->DAG conversion & comparison
  friend class ACU_LhsAutomaton;	// for matching DAG subject
  friend class ACU_AlienAlienLhsAutomaton;	// for matching DAG subject
  friend class ACU_CollectorLhsAutomaton;	// for matching DAG subject
  friend class ACU_NGA_LhsAutomaton;	// for matching DAG subject
  friend class ACU_VarLhsAutomaton;	// for matching DAG subject
  friend class ACU_GndLhsAutomaton;	// for matching DAG subject
  friend class ACU_NonLinearLhsAutomaton;	// for matching DAG subject
  friend class ACU_RhsAutomaton;	// for constructing replacement DAG
  friend class ACU_Subproblem;		// for constructing substitution
  friend class ACU_UnificationSubproblem2;	// for constructing substitution
  friend class ACU_ExtensionInfo;	// for constructing matched portion
  friend class ACU_DagArgumentIterator;	// to accesss Pair

  friend class ACU_TreeDagNode;		// for conversion
  friend class ACU_BaseDagNode;		// HACK for getSize

  friend ACU_DagNode* getACU_DagNode(DagNode* dagNode);
};

ACU_DagNode* getACU_DagNode(DagNode* dagNode);

inline
ACU_DagNode::ACU_DagNode(ACU_Symbol* symbol, int size, NormalizationStatus status)
  : ACU_BaseDagNode(symbol), argArray(size)
{
  setNormalizationStatus(status);
}

inline int
ACU_DagNode::nrArgs() const
{
  return argArray.length();
}

inline DagNode*
ACU_DagNode::getArgument(int i) const
{
  return argArray[i].dagNode;
}

inline int
ACU_DagNode::getMultiplicity(int i) const
{
  return argArray[i].multiplicity;
}

inline ArgVec<ACU_DagNode::Pair>::iterator
ACU_DagNode::fastCopy(ArgVec<Pair>::const_iterator i,
		      ArgVec<Pair>::const_iterator e,
		      ArgVec<Pair>::iterator d)
{
  //
  //	This beats the STL copy() algorithm because the latter maintains
  //	a counter.
  //
  while (i != e)
    {
      *d = *i;
      ++d;
      ++i;
    }
  return d;
}

inline ArgVec<ACU_DagNode::Pair>::iterator
ACU_DagNode::fastCopy(ACU_FastIter& i,
		      ArgVec<Pair>::iterator d)
{
  while (i.valid())
    {
      d->dagNode = i.getDagNode();
      d->multiplicity = i.getMultiplicity();
      ++d;
      i.next();
    }
  return d;
}

#endif
