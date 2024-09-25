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
//	Class for terms in the free theory.
//

#ifndef _freeTerm_hh_
#define _freeTerm_hh_
#include "term.hh"
#include "freeSubterm.hh"

class FreeTerm final : public Term
{
public:
  FreeTerm(FreeSymbol* symbol, const Vector<Term*>& arguments);
  ~FreeTerm();
  //
  //	functions required by theory interface
  //
  RawArgumentIterator* arguments() const;
  void deepSelfDestruct();
  Term* deepCopy2(SymbolMap* translator) const;
  Term* normalize(bool full, bool& changed);
  int compareArguments(const Term* other) const;
  int compareArguments(const DagNode* other) const;
  void findEagerVariables(bool atTop, NatSet& eagerVariables) const;
  void analyseConstraintPropagation(NatSet& BoundUniquely) const;
  LhsAutomaton* compileLhs2(bool matchAtTop,
			    const VariableInfo& variableInfo,
			    NatSet& boundUniquely,
			    bool& subproblemLikely);
  void markEagerArguments(int nrVariables,
			  const NatSet& eagerVariables,
			  Vector<int>& problemVariables);
  DagNode* dagify2();

  void findAvailableTerms(TermBag& availableTerms, bool eagerContext, bool atTop);
  int compileRhs2(RhsBuilder& rhsBuilder,
		  VariableInfo& variableInfo,
		  TermBag& availableTerms,
		  bool eagerContext);
  //
  //	Optional stuff that is easy to define for free theory.
  //
  bool earlyMatchFailOnInstanceOf(const Term* other) const;
  bool subsumes(const Term* other, bool sameVariableSet) const;
  int partialCompareArguments(const Substitution& partialSubstitution,
			      DagNode* other) const;

  void computeMatchIndices() const;
  //
  //	Functions particular to free terms.
  //
  FreeSymbol* symbol() const;
  Term* locateSubterm(const Vector<int>& position, int backup = 0);
  Term* locateSubterm2(Vector<int>& position);
  int getSlotIndex() const;
  void setSlotIndex(int index);
  void setVisitedFlag(bool state);
  void findActiveSlots(NatSet& slots);
  FreeRemainder* compileRemainder(Equation* equation, const Vector<int>& slotTranslation);
  bool  scanFreeSkeleton(const NatSet& usedVariables,
			 Vector<int>& path,
			 FreePositionTable& positions,
			 NatSet& boundVariables,
			 Vector<FreeSubterm>& subterms);
  void resetSlotIndices();
  //
  //	Gross hack to allow surgery on free theory terms from ouside of the theory.
  //	Only safe if there has been no processing on the term.
  //	Garbage collection of the old argument (parts of which the caller
  //	may have reused in newArgument) is the caller's responsibility.
  //
  Term* replaceArgument(Index index, Term* newArgument);

private:
  FreeTerm(const FreeTerm& original, FreeSymbol* symbol, SymbolMap* translator);

  struct CP_Sequence;

  static void findConstraintPropagationSequence(const Vector<FreeOccurrence>& aliens,
						const NatSet& boundUniquely,
						CP_Sequence& bestSequence);
  static void findConstraintPropagationSequence(const Vector<FreeOccurrence>& aliens,
						Vector<int>& currentSequence,
						const NatSet& boundUniquely,
						int step,
						CP_Sequence& bestSequence);
  static bool remainingAliensContain(const Vector<FreeOccurrence>& aliens,
				     Vector<int>& currentSequence,
				     int step,
				     int us,
				     const NatSet& interestingVariables);
  void scanFreeSkeleton(Vector<FreeOccurrence>& freeSymbols,
			Vector<FreeOccurrence>& otherSymbols,
			int parent = -1,
			int argIndex = -1);
  void compileRhsAliens(RhsBuilder& rhsBuilder,
			VariableInfo& variableInfo,
			TermBag& availableTerms,
			bool eagerContext,
			int& maxArity,
			int& nrFree);
  int compileRhs3(FreeRhsAutomaton* automaton,
		  RhsBuilder& rhsBuilder,
		  VariableInfo& variableInfo,
		  TermBag& availableTerms,
		  bool eagerContext);

  Vector<Term*> argArray;
  int slotIndex;
  bool visitedFlag;
};

inline FreeSymbol*
FreeTerm::symbol() const
{
  return safeCast(FreeSymbol*, Term::symbol());
}

inline int
FreeTerm::getSlotIndex() const
{
  return slotIndex;
}

inline void
FreeTerm::setSlotIndex(int index)
{
  slotIndex = index;
}

inline void
FreeTerm::setVisitedFlag(bool state)
{
  visitedFlag = state;
}

inline Term*
FreeTerm::replaceArgument(Index index, Term* newArgument)
{
  Assert(occursBelow().empty(), "can't do surgery on term after analysis");
  Assert(occursInContext().empty(), "can't do surgery on term after analysis");
  Assert(collapseSymbols().empty(), "can't do surgery on term after analysis");
  Term* oldArgument = argArray[index];
  argArray[index] = newArgument;
  return oldArgument;
}
  
#endif
