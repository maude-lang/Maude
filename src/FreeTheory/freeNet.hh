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
//	Class for discrimination net for the free theory.
//
#ifndef _freeNet_hh_
#define _freeNet_hh_
#include <set>
#include "unionFind.hh"

class FreeNet
{
public:
  typedef set<int> PatternSet;

  ~FreeNet();

  //
  //	Functions to construct a FreeNet.
  //
  int allocateNode(int nrMatchArcs);
  void fillOutNode(int nodeNr,
		   int position,
		   int argIndex,
		   const Vector<Symbol*>&  symbols,
		   const Vector<int>& targets,
		   const Vector<int>& slots,
		   int neqTarget);
  int addRemainderList(const PatternSet& liveSet);
  void translateSlots(int nrRealSlots, const Vector<int>& slotTranslation);
  void buildRemainders(const Vector<Equation*>& equations,
		       const PatternSet& patternsUsed,
		       const Vector<int>& slotTranslation);
  //
  //	Function to use a FreeNet.
  //
  bool applyReplace(DagNode* subject, RewritingContext& context);
  bool applyReplaceNoOwise(DagNode* subject, RewritingContext& context);

#ifdef DUMP
  void dump(ostream& s, int indentLevel = 0);
#endif

private:
  enum SpecialValues
  {
    LESS = 1,
    GREATER = 0
  };

  struct TestNode
  {
    Symbol* symbol;
    union
    {
      int position;
      DagNode*** positionPtr;
    };
    union
    {
      int slot;
      DagNode*** slotPtr;
    };
#if (SIZEOF_VOID_P == 4) && (SIZEOF_SHORT == 2)
    //
    //	Optimize layout for 64bit ptrs, 16bit shorts.
    //
    short argIndex;
    short equal;
    short notEqual[2];
#else
    //
    //	Optimize layout for 32bit ptrs and ints.
    //
    int argIndex;
    int equal;
    int notEqual[2];
    int dummy;  // to round size up to 8 machine words
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
  bool applyReplaceNoOwise2(DagNode* subject, RewritingContext& context);

  Vector<DagNode**> stack;
  Vector<TestNode> net;
  Vector<Vector<FreeRemainder*> > fastApplicable;
  Vector<FreeRemainder*> remainders;
  Vector<PatternSet> applicable;
};

inline bool
FreeNet::applyReplace(DagNode* subject, RewritingContext& context)
{
  return !applicable.isNull() ? applyReplace2(subject, context) : false;
}

inline bool
FreeNet::applyReplaceNoOwise(DagNode* subject, RewritingContext& context)
{
  return !applicable.isNull() ? applyReplaceNoOwise2(subject, context) : false;
}

#endif
