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
//	Class for symbols belonging to the free theory.
//
#ifndef _freeSymbol_hh_
#define _freeSymbol_hh_
#include "symbol.hh"
#include "freeNet.hh"

#define GET_NET getNet
#define DISC_NET discriminationNet
#define FREE_NET FreeNet

class FreeSymbol : public Symbol
{
  NO_COPYING(FreeSymbol);

public:
  FreeSymbol(int id, int arity, const Vector<int>& strategy = standard, bool memoFlag = false);
  static FreeSymbol* newFreeSymbol(int id,
				   int arity,
				   const Vector<int>& strategy = standard,
				   bool memoFlag = false);

  Term* makeTerm(const Vector<Term*>& args);
  DagNode* makeDagNode(const Vector<DagNode*>& args);
  void compileEquations();
  bool eqRewrite(DagNode* subject, RewritingContext& context);
  void computeBaseSort(DagNode* subject);
  void normalizeAndComputeTrueSort(DagNode* subject, RewritingContext& context);
  void stackArguments(DagNode* subject,
		      Vector<RedexPosition>& stack,
		      int parentIndex,
		      bool respectFrozen,
		      bool respectUnstackable,
		      bool eagerContext);
  Term* termify(DagNode* dagNode);
  //
  //	Unification stuff.
  //
  int unificationPriority() const;
  void computeGeneralizedSort(const SortBdds& sortBdds,
			      const Vector<int>& realToBdd,
			      DagNode* subject,
			      Vector<Bdd>& generalizedSort);
  void computeGeneralizedSort2(const SortBdds& sortBdds,
			       const Vector<int>& realToBdd,
			       DagNode* subject,
			       Vector<Bdd>& outputBdds);
  bool isStable() const;
  bool determineGround(DagNode* dagNode);
  //
  //	Hash cons stuff.
  //
  DagNode* makeCanonical(DagNode* original, HashConsSet* hcs);
  DagNode* makeCanonicalCopy(DagNode* original, HashConsSet* hcs);
  //
  //	Stack machine stuff.
  //
  Instruction* generateFinalInstruction(const Vector<int>& argumentSlots);
  Instruction* generateInstruction(int destination, const Vector<int>& argumentSlots, Instruction* nextInstruction);
  FreeNet& getNet();

#ifdef COMPILER
  void generateCode(CompilationContext& context) const;
  void generateCons(CompilationContext& context, int indentLevel) const;
#endif

#ifdef DUMP
  void dump(ostream& s, int indentLevel = 0);
#endif

private:
  bool complexStrategy(DagNode* subject, RewritingContext& context);
  void memoStrategy(MemoTable::SourceSet& from, DagNode* subject, RewritingContext& context);

  template<int n>
  static bool eqRewriteFast(Symbol* symbol, DagNode* subject, RewritingContext& context);

  static bool eqRewriteSlow(Symbol* symbol, DagNode* subject, RewritingContext& context);

protected:
  FreeNet discriminationNet;
};

inline FreeNet&
FreeSymbol::getNet()
{
  return discriminationNet;
}

#endif
