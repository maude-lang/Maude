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
//	Class for DAG nodes in the successor theory.
//
#ifndef _S_DagNode_hh_
#define _S_DagNode_hh_
#include "gmpxx.h"
#include "dagNode.hh"

class S_DagNode : public DagNode
{
  NO_COPYING(S_DagNode);

public:
  S_DagNode(S_Symbol* symbol, const mpz_class& number, DagNode* arg);
  ~S_DagNode();
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
  void stackArguments(Vector<RedexPosition>& stack,
		      int parentIndex,
		      bool respectFrozen);
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
  //	Unification member functions.
  //
  ReturnResult computeBaseSortForGroundSubterms();
  bool computeSolvedForm2(DagNode* rhs, UnificationContext& solution, PendingUnificationStack& pending);

  mpz_class nonVariableSize();
  void insertVariables2(NatSet& occurs);
  DagNode* instantiate2(const Substitution& substitution);
  //
  //	Narrowing member functions.
  //
  DagNode* instantiateWithReplacement(const Substitution& substitution, int argIndex, DagNode* newDag);
  //
  //	Functions specific to S_DagNode.
  //
  S_Symbol* symbol() const;
  const mpz_class& getNumber() const;
  DagNode* getArgument() const;

private:
  //
  //	Functions required by theory interface.
  //
  DagNode* markArguments();
  DagNode* copyEagerUptoReduced2();
  void clearCopyPointers2();
  //
  //	Normalization functions.
  //
  void normalizeAtTop();

  mpz_class* number;
  DagNode* arg;
  
  friend class S_Symbol;           	// to reduce subterms prior to rewrite, normalization
};

inline S_Symbol*
S_DagNode::symbol() const
{
  return safeCast(S_Symbol*, DagNode::symbol());
}

inline const mpz_class&
S_DagNode::getNumber() const
{
  return *number;
}

inline DagNode*
S_DagNode::getArgument() const
{
  return arg;
}

#endif
