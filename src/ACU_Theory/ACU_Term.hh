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
//	Class for terms in the AC and ACU theories.
//
#ifndef _ACU_Term_hh_
#define _ACU_Term_hh_
#include "ACU_Symbol.hh"
#include "term.hh"

class ACU_Term : public Term
{
  NO_COPYING(ACU_Term);

public:
  ACU_Term(ACU_Symbol* symbol, const Vector<Term*>& arguments);
  //
  //    Member functions required by theory interface.
  //
  RawArgumentIterator* arguments();
  void deepSelfDestruct();
  Term* deepCopy2(SymbolMap* map) const;
  Term* normalize(bool full, bool& changed);
  int compareArguments(const Term* other) const;
  int compareArguments(const DagNode* other) const;
  void findEagerVariables(bool atTop, NatSet& eagerVariables) const;
  void analyseConstraintPropagation(NatSet& boundUniquely) const;
  void analyseCollapses2();
  void insertAbstractionVariables(VariableInfo& variableInfo);
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
  //    Functions particular to ACU_Term.
  //
  ACU_Symbol* symbol() const;

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  struct Pair  // misnomer!
    {
      Term* term;
      int multiplicity;
      short abstractionVariableIndex;	// if subterm could enter our theory we abstract it
      Bool collapseToOurSymbol;		// first possible reason for variable abstraction
      Bool matchOurIdentity;		// second possible reason for variable abstraction
    };

  struct CP_Sequence;

  ACU_Term(const ACU_Term& original, SymbolMap* map);
  bool normalizeAliensAndFlatten();
  static bool pairLt(const Pair& p1, const Pair& p2);
  static void weakConstraintPropagation(const Vector<Pair>& aliens,
					const NatSet& boundUniquely,
					int step,
					Vector<int>& sequence);
  void compileLhs3(bool matchAtTop,
		   const VariableInfo& variableInfo,
		   NatSet& boundUniquely,
		   bool& subproblemLikely,
		   ACU_LhsAutomaton* automaton);
  //
  //	Fuctions for compiling special case automata.
  //
  ACU_LhsAutomaton* tryToMakeSpecialCaseAutomaton(bool matchAtTop,
						  const VariableInfo& variableInfo,
						  NatSet& boundUniquely);
  ACU_LhsAutomaton* tryToMakeNonLinearLhsAutomaton(const VariableInfo& variableInfo,
						   NatSet& boundUniquely);
  ACU_LhsAutomaton* tryToMakeCollectorLhsAutomaton(bool matchAtTop,
						   const VariableInfo& variableInfo,
						   NatSet& boundUniquely,
						   int collectorCandidate);
  //
  //	Functions needed to compile aliens only case matching.
  //
  static void compileAliensOnlyCase(ACU_LhsAutomaton* automaton,
				    const Vector<Pair>& nonGroundAliens,
				    const VariableInfo& variableInfo,
				    NatSet& boundUniquely,
				    bool& subproblemLikely);
  static void findConstraintPropagationSequence(const Vector<Pair>& aliens,
						const NatSet& boundUniquely,
						CP_Sequence& bestSequence);
  static void findConstraintPropagationSequence(const Vector<Pair>& aliens,
						const Vector<int>& currentSequence,
						const NatSet& boundUniquely,
						int step,
						CP_Sequence& bestSequence);
  static void addIndependentAliens(const Vector<Pair>& aliens,
				   CP_Sequence& bestSequence);
  //
  //	Functions needed to compile full and greedy case matching.
  //
  static void compileGreedyAndFullCases(ACU_LhsAutomaton* automaton,
					const Vector<Pair>& nonGroundAliens,
					const VariableInfo& variableInfo,
					NatSet& boundUniquely,
					bool& subproblemLikely);
  static int findGreedySequence(const Vector<Pair>& aliens, Vector<int>& sequence);
  static void findIndependentSets(const Vector<Pair>& aliens,
				  Vector<Vector<int> >& independents);
  static void findLongestIncreasingSequence(const Vector<Pair>& aliens,
					    Vector<int>& dependents,
					    Vector<int>& sequence);
  static void findFullSequence(const Vector<Pair>& aliens,
			       const NatSet& boundUniquely,
			       Vector<int>& sequence);

  Vector<Pair> argArray;
  int uniqueCollapseSubtermIndex;

  friend class ACU_ArgumentIterator;
};

inline ACU_Symbol*
ACU_Term::symbol() const
{
  return safeCast(ACU_Symbol*, Term::symbol());
}

#endif
