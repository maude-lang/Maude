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
//	Class for DAG nodes in the free theory.
//
#ifndef _freeDagNode_hh_
#define _freeDagNode_hh_
#include "dagNode.hh"

class FreeDagNode : public DagNode
{
public:
  FreeDagNode(Symbol* symbol);
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
  void stackArguments(Vector<RedexPosition>& stack,
		      int parentIndex,
		      bool respectFrozen);
  //
  //	Fast theory specific access to argument list
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
  void clearCopyPointers2();
    
  friend class FreeSymbol;		// to reduce subterms prior to rewrite
  friend class FreeUnarySymbol;		// to reduce subterms prior to rewrite
  friend class FreeBinarySymbol;	// to reduce subterms prior to rewrite
  friend class FreeTernarySymbol;	// to reduce subterms prior to rewrite
  friend class FreeTerm;		// for term->DAG conversion & comparison
  friend class FreeLhsAutomaton;	// for matching DAG subject
  friend class FreeNet;			// for matching DAG subject
  friend class FreeRhsAutomaton;	// for constructing replacement DAG
};

inline
FreeDagNode::FreeDagNode(Symbol* symbol) : DagNode(symbol)
{
  int nrArgs = symbol->arity();
  if (nrArgs > nrWords)
    {
      setCallDtor();  // need our dtor called when garbage collected so we can free arg array
      external = new (DagNode*)[nrArgs];
    }
}

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

