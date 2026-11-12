/*

    This file is part of the Maude 3 strategy-aware model checker.

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
//      Class for symbols for built in model checking with strategies.
//
#ifndef _strategyModelCheckerSymbol_hh_
#define _strategyModelCheckerSymbol_hh_
#include "temporalSymbol.hh"
#include "cachedDag.hh"
#include "modelChecker2.hh"
#include "strategyTransitionGraph.hh"

class StrategyModelCheckerSymbol : public TemporalSymbol
{
  NO_COPYING(StrategyModelCheckerSymbol);

public:
  StrategyModelCheckerSymbol(int id);

protected:
  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);
  bool attachTerm(const char* purpose, Term* term);
  void copyAttachments(Symbol* original, SymbolMap* map);
  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getSymbolAttachments(Vector<const char*>& purposes,
			    Vector<Symbol*>& symbols);
  void getTermAttachments(Vector<const char*>& purposes,
			  Vector<Term*>& terms);

  bool eqRewrite(DagNode* subject, RewritingContext& context);
  void postInterSymbolPass();
  void compileEquations();
  void reset();

private:
  struct SystemAutomaton : public ModelChecker2::System
  {
    int getNextState(int stateNr, int transitionNr);
    bool checkProposition(int stateNr, int propositionIndex) const;

    DagNodeSet propositions;
    Symbol* satisfiesSymbol;
    RewritingContext* parentContext;
    DagNode* trueTerm;
    StrategyTransitionGraph* systemStates;

    #ifndef NO_SMC_2NDCACHE
    typedef std::map<std::pair<DagNode*,int>, bool> PropositionCache;
    mutable PropositionCache propositionCache;
    #endif
  };

  static bool eqRewrite(Symbol* symbol, DagNode* subject, RewritingContext& context);

  DagNode* makeTransition(const StrategyTransitionGraph& states, int stateNr, int target);
  DagNode* makeTransitionList(const StrategyTransitionGraph& states,
			      const list<int>& path,
			      int lastTarget,
			      bool firstPath);
  DagNode* makeCounterexample(const StrategyTransitionGraph& states, const ModelChecker2& mc);

  Symbol* satisfiesSymbol;
  //
  //	Symbols needed for returning counterexamples.
  //
  QuotedIdentifierSymbol* qidSymbol;
  Symbol* unlabeledSymbol;
  Symbol* solutionSymbol;
  Symbol* opaqueSymbol;
  Symbol* transitionSymbol;
  Symbol* transitionListSymbol;
  Symbol* nilTransitionListSymbol;
  Symbol* counterexampleSymbol;

  CachedDag trueTerm;
};

#endif
