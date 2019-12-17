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
//	Class for DAG nodes in the free theory.
//
#ifndef _freeDagNode_hh_
#define _freeDagNode_hh_
#include "dagNode.hh"
#include "freeSymbol.hh"

class FreeDagNode : public DagNode
{
public:
  FreeDagNode(Symbol* symbol);
  //
  //	For # args <= 3 case.
  //
  FreeDagNode(Symbol* symbol, char /* dummy */);
  //
  //	For stack based execution.
  //
  FreeDagNode(Symbol* symbol, int dummy);
  FreeDagNode(Symbol* symbol, DagNode* a0);
  FreeDagNode(Symbol* symbol, DagNode* a0, DagNode* a1);
  FreeDagNode(Symbol* symbol, DagNode* a0, DagNode* a1, DagNode* a2);
  /*
  FreeDagNode(Symbol* symbol, int dummy, DagNode* a0);
  FreeDagNode(Symbol* symbol, int dummy, DagNode* a1, DagNode* a2);
  FreeDagNode(Symbol* symbol, int dummy, DagNode* a0, DagNode* a1, DagNode* a2);
  */
  ~FreeDagNode();

  RawDagArgumentIterator* arguments();
  size_t getHashValue();
  int compareArguments(const DagNode* other) const;
  void overwriteWithClone(DagNode* old);
  DagNode* makeClone();
  DagNode* copyWithReplacement(Vector<RedexPosition>& redexStack,
			       int first,
			       int last);
  DagNode* copyWithReplacement(int argIndex, DagNode* replacement);
  //
  //	Unification member functions.
  //
  ReturnResult computeBaseSortForGroundSubterms(bool warnAboutUnimplemented);
  bool computeSolvedForm2(DagNode* rhs, UnificationContext& solution, PendingUnificationStack& pending);
  void insertVariables2(NatSet& occurs);
  DagNode* instantiate2(const Substitution& substitution);
  //
  //	Narrowing member functions.
  //
  bool indexVariables2(NarrowingVariableInfo& indices, int baseIndex);
  DagNode* instantiateWithReplacement(const Substitution& substitution,
				      const Vector<DagNode*>* eagerCopies,
				      int argIndex,
				      DagNode* newDag);
  DagNode* instantiateWithCopies2(const Substitution& substitution, const Vector<DagNode*>& eagerCopies);
  //
  //	Functions particular to free dag nodes.
  //
  FreeSymbol* symbol() const;
  //
  //
  //	Fast theory specific access to argument list.
  //
  DagNode* getArgument(int i) const;

private:
  union Word  // HACK - should make MemoryCell::Word visible
  {
    void* pointer;
    int integer;
    size_t size;
  };

  union
    {
      DagNode* internal[nrWords];
      DagNode** external;
      Word hashCache[nrWords];
    };

  DagNode** argArray() const;
  DagNode* markArguments();
  DagNode* copyEagerUptoReduced2();  
  DagNode* copyAll2();  
  void clearCopyPointers2();
  //
  //	Unification stuff
  //
  enum PurificationStatus
    {
      OCCURS_CHECK_FAIL,
      PURE_AS_IS,
      PURIFIED
    };

  PurificationStatus purifyAndOccurCheck(DagNode* repVar,
					 UnificationContext& solution,
					 PendingUnificationStack& pending,
					 FreeDagNode*& purified);

  friend class FreeSymbol;		// to reduce subterms prior to rewrite
  friend class FreeUnarySymbol;		// to reduce subterms prior to rewrite
  friend class FreeBinarySymbol;	// to reduce subterms prior to rewrite
  friend class FreeTernarySymbol;	// to reduce subterms prior to rewrite
  friend class FreeTerm;		// for term->DAG conversion & comparison
  friend class FreeLhsAutomaton;	// for matching DAG subject
  friend class FreeNet;			// for matching DAG subject
  friend class FreeRhsAutomaton;	// for constructing replacement DAG
  friend class FreeFast3RhsAutomaton;	// for constructing replacement DAG
  friend class FreeFast2RhsAutomaton;	// for constructing replacement DAG
  friend class FreeUnaryRhsAutomaton;	// for constructing replacement DAG
  friend class FreeBinaryRhsAutomaton;	// for constructing replacement DAG
  friend class FreeTernaryRhsAutomaton;	// for constructing replacement DAG

  friend class FreeGeneralCtor;
  friend class FreeGeneralCtorFinal;
  friend class FreeGeneralExtor;
  friend class FreeGeneralExtorFinal;

};

inline FreeSymbol*
FreeDagNode::symbol() const
{
  return safeCast(FreeSymbol*, DagNode::symbol());
}

inline
FreeDagNode::FreeDagNode(Symbol* symbol) : DagNode(symbol)
{
  int nrArgs = symbol->arity();
  if (nrArgs > nrWords)
    {
      setCallDtor();  // need our dtor called when garbage collected so we can free arg array
      external = new DagNode*[nrArgs];
    }
}

inline
FreeDagNode::FreeDagNode(Symbol* symbol, char /* dummy */)
  : DagNode(symbol)
{
}

inline
FreeDagNode::FreeDagNode(Symbol* symbol, int /* dummy */)
  : DagNode(symbol, symbol->traverse(0, 0))
{
}

inline
FreeDagNode::FreeDagNode(Symbol* symbol, DagNode* a0)
  : DagNode(symbol, symbol->traverse(0, a0->getSortIndex()))
{
  internal[0] = a0;
}

inline
FreeDagNode::FreeDagNode(Symbol* symbol, DagNode* a0, DagNode* a1)
  : DagNode(symbol, symbol->traverse(symbol->traverse(0, a0->getSortIndex()), a1->getSortIndex()))
{
  internal[0] = a0;
  internal[1] = a1;
}

inline
FreeDagNode::FreeDagNode(Symbol* symbol, DagNode* a0, DagNode* a1, DagNode* a2)
  : DagNode(symbol, symbol->traverse(symbol->traverse(symbol->traverse(0, a0->getSortIndex()), a1->getSortIndex()), a2->getSortIndex()))
{
  internal[0] = a0;
  internal[1] = a1;
  internal[2] = a2;
}

#if 0
inline
FreeDagNode::FreeDagNode(Symbol* symbol, int /* dummy */, DagNode* a0)
  : DagNode(symbol, symbol->getUniqueSortIndex())
{
  internal[0] = a0;
}

inline
FreeDagNode::FreeDagNode(Symbol* symbol, int /* dummy */, DagNode* a0, DagNode* a1)
  : DagNode(symbol, symbol->getUniqueSortIndex())
{
  internal[0] = a0;
  internal[1] = a1;
}

inline
FreeDagNode::FreeDagNode(Symbol* symbol, int /* dummy */, DagNode* a0, DagNode* a1, DagNode* a2)
  : DagNode(symbol, symbol->getUniqueSortIndex())
{
  internal[0] = a0;
  internal[1] = a1;
  internal[2] = a2;
}
#endif

inline DagNode*
FreeDagNode::getArgument(int i) const
{
  Assert(i >= 0, "-ve arg number");
  int nrArgs = symbol()->arity();
  Assert(i < nrArgs, "arg number too big");
  return (nrArgs > nrWords) ? external[i] : internal[i];
}

inline DagNode**
FreeDagNode::argArray() const
{
  return (symbol()->arity() > nrWords) ? external : const_cast<DagNode**>(&(internal[0]));
}

#endif
