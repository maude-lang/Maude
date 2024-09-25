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
//	Class for discrimination net for the free theory.
//
#ifndef _freeNet_hh_
#define _freeNet_hh_
#include <set>
#include "unionFind.hh"
#include "freeRemainder.hh"

class FreeNet
{
public:
  typedef set<int> PatternSet;

  FreeNet();
  ~FreeNet();

  //
  //	Functions to construct a FreeNet.
  //
  int allocateNode(int nrMatchArcs);
  void fillOutNode(int nodeNr,
		   int position,
		   int argIndex,
		   const Vector<Symbol*>& symbols,
		   const Vector<int>& targets,
		   const Vector<int>& slots,
		   int neqTarget);
  int addRemainderList(const PatternSet& liveSet);
  void translateSlots(int nrRealSlots, const Vector<int>& slotTranslation);
  void buildRemainders(const Vector<Equation*>& equations,
		       const PatternSet& patternsUsed,
		       const Vector<int>& slotTranslation);
  //
  //	Functions to use a FreeNet.
  //
  bool applyReplace(DagNode* subject, RewritingContext& context);
  bool applyReplaceFast(DagNode* subject, RewritingContext& context);
  bool applyReplaceNoOwise(DagNode* subject, RewritingContext& context);
  //
  //	For stack machine execution.
  //
  long findRemainderListIndex(DagNode** argumentList);
  bool fastHandling() const;

#ifdef DUMP
  void dump(ostream& s, int indentLevel = 0);
#endif

private:
  enum SpecialValues
  {
    LESS = 1,
    GREATER = 0
  };

  //
  //	This is the data structure most accessed in the critical loops.
  //
  struct TestNode
  {
    int notEqual[2];  // index of next test node to take for > and < cases (-ve encodes index of applicable list, 0 encodes failure)
    Index position;  // stack slot to get free dagnode argument list from (-1 indicates use old argument)
    int argIndex;   // index of argument to test
    int symbolIndex;  // index within module of symbol we test against
    int slot;  // index of stack slot to store free dagnode argument list in (-1 indicates do not store)
    int equal;  // index of next test node to take for == case (-ve encode index of applicable list)

#if SIZEOF_LONG == 4  // 32-bit machines
    int pad_struct_to_32_bytes_on_32_bit_machines;
#endif
  };

  struct Triple
  {
    Symbol* symbol;
    int slot;
    int subtree;
  };

  static bool tripleLt(const Triple& p1, const Triple& p2);

  void buildTernaryTree(int& nodeIndex,
			Vector<Triple>& triples,
			int first,
			int last,
			int defaultSubtree,
			int position,
			int argIndex);
  bool moreImportant(Symbol* first, Symbol* second);
  bool applyReplace2(DagNode* subject, RewritingContext& context);
  bool applyReplaceFast2(DagNode* subject, RewritingContext& context);
  bool applyReplaceNoOwise2(DagNode* subject, RewritingContext& context);

  Vector<DagNode**> stack;
  Vector<TestNode> net;
  Vector<Vector<FreeRemainder*> > fastApplicable;
  Vector<FreeRemainder*> remainders;
  Vector<PatternSet> applicable;
  bool fast;

  friend class FreeInstruction;
  friend class FreeGeneralExtor;
  friend class FreeGeneralExtorFinal;
};

inline bool
FreeNet::applyReplace(DagNode* subject, RewritingContext& context)
{
  return !applicable.isNull() ? applyReplace2(subject, context) : false;
}

inline bool
FreeNet::applyReplaceFast(DagNode* subject, RewritingContext& context)
{
  return !applicable.isNull() ? applyReplaceFast2(subject, context) : false;
}

inline bool
FreeNet::applyReplaceNoOwise(DagNode* subject, RewritingContext& context)
{
  return !applicable.isNull() ? applyReplaceNoOwise2(subject, context) : false;
}

inline bool
FreeNet::fastHandling() const
{
  return fast;
}

#endif
