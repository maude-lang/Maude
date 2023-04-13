/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for left hand side automata in the AC and ACU theories.
//
#ifndef _ACU_LhsAutomaton_hh_
#define _ACU_LhsAutomaton_hh_
#include "lhsAutomaton.hh"
#include "ACU_Tree.hh"

class ACU_LhsAutomaton : public LhsAutomaton
{
  NO_COPYING(ACU_LhsAutomaton);

public:
  enum MatchStrategy
  {
    //
    //	Ground out case: no extension and everthing grounds out;
    //	return fail if any subjects left.
    //
    GROUND_OUT,
    //
    //	Lone variable case: no extension, no aliens, 1 unbound variable which is forced
    //	to match remaining subjects.
    //
    LONE_VARIABLE,
    //
    //	Aliens only case: no extension, all top variables are guaranteed to be
    //	bound on entry to matcher. Alien sequence will be ordered:
    //		(1) to optimize strong constraint propagation (match independent aliens)
    //		(2) to "force" as many aliens as possible (subsumptive aliens)
    //		(3) to optimize weak constraint propagation (aliens left over)
    //	The field nrIndependents holds the total number in parts (1) and (2) of
    //	alien sequence.
    //
    ALIENS_ONLY,
    //
    //	Greedy case: the intersection of each subterm's variables with union of
    //	its context variables and the condition variables contains only variables
    //	guaranteed to be bound on entry to matcher. Alien sequence will be ordered
    //	to maximize the chance that greedy failure is true failure. The field
    //	nrIndependents holds the number of aliens for which greedy failure is true
    //	failure. The top variable sequence will be ordered to maximize the chance of
    //	finding a greedy match, and in the pure greedy case, to maximize the chance
    //	that pure greedy failure is true failure.
    //
    GREEDY,
    //
    //	Full case: if other cases don't apply. Alien sequence will be ordered to
    //	optimize solve-time weak constraint propagation. The field nrIndependents
    //	is unused.
    //
    FULL
  };
  //
  //	Operations to build an ACU_LhsAutomaton
  //
  ACU_LhsAutomaton(ACU_Symbol* symbol,
		   bool matchAtTop,
		   bool collapsePossible,
		   int nrVariables);
  ~ACU_LhsAutomaton();
  
  void addUniqueCollapseAutomaton(LhsAutomaton* ucAutomaton);
  void addTopVariable(const VariableTerm* variable,
		      int multiplicity,
		      bool bound);
  void addAbstractionVariable(int index,
			      Sort* sort,
			      int upperBound,
			      bool takeIdentity,
			      LhsAutomaton* abstracted,
			      int multiplicity);
  void addGroundAlien(Term* alien, int multiplicity);
  void addGroundedOutAlien(Term* alien, LhsAutomaton* automaton, int multiplicity);
  void addNonGroundAlien(Term* alien, LhsAutomaton* automaton, int multiplicity);
  void complete(MatchStrategy strategy, int nrIndependent);
  //
  //	Standard LhsAutomaton operations
  //
  bool match(DagNode* subject,
             Substitution& solution,
             Subproblem*& returnedSubproblem,
             ExtensionInfo* extensionInfo);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

protected:
  //
  //	Allow derived automata to access this stuff and avoid
  //	storing it for themselves.
  //
  ACU_Symbol* getSymbol() const;
  bool getCollapsePossible() const;
  Substitution& getLocal();
  Substitution& getLocal2();
  //
  //	Useful functionality for ACU_NonLinearAutomaton.
  //
  DagNode* makeHighMultiplicityAssignment(int multiplicity,
					  Sort* sort,
					  ACU_Tree& current);

private:
  struct TopVariable
  {
    int index;
    int multiplicity;
    Sort* sort;
    int upperBound;
    AssociativeSymbol::Structure structure;
    bool takeIdentity;
    LhsAutomaton* abstracted;	// automaton for abstracted term
    //
    //	Data storage for match-time use
    //
    int prevUnbound;	// back pointer to last unbound variable
    int firstSubject;	// first subject heuristically assigned to variable
    int nrSubjects;	// number of subjects heuristically assigned to variable
  };

  struct GroundAlien
  {
    Term* term;
    int multiplicity;
  };

  struct NonGroundAlien
  {
    Term* term;		// 0 if not stable
    int multiplicity;
    LhsAutomaton* automaton;
  };

  struct Subject
  {
    int multiplicity;
    int assignee;
  };

  static bool topVariableLt(const TopVariable& t1, const TopVariable& t2);

  void updateTotals(int min, int max);

  bool multiplicityChecks(ACU_DagNode* subject);
  bool eliminateGroundAliens(ACU_DagNode* subject);
  bool eliminateBoundVariables(ACU_DagNode* subject, Substitution& solution);
  bool eliminateGroundedOutAliens(ACU_DagNode* subject, Substitution& solution);
  bool forcedLoneVariableCase(ACU_DagNode* subject,
			      Substitution& solution,
			      Subproblem*& returnedSubproblem);
  bool aliensOnlyMatch(ACU_DagNode* subject,
		       Substitution& solution,
		       Subproblem*& returnedSubproblem);
  ACU_Subproblem* buildBipartiteGraph(ACU_DagNode* subject,
				      Substitution& solution,
				      ACU_ExtensionInfo* extensionInfo,
				      int firstAlien,
				      SubproblemAccumulator& subproblems);
  bool handleElementVariables(ACU_DagNode* subject,
			      Substitution& solution,
			      ACU_Subproblem* subproblem);
  int computeTotalMultiplicity();
  //
  //	Tree matcher.
  //
  int treeMatch(ACU_TreeDagNode* subject,
		Substitution& solution,
		Subproblem*& returnedSubproblem,
		ACU_ExtensionInfo* extensionInfo);
  int eliminateBoundVariables(Substitution& solution);
  bool eliminateGroundAliens();
  bool eliminateGroundedOutAliens(Substitution& solution);
  int greedyMatch(ACU_TreeDagNode* subject,
		  Substitution& solution,
		  ACU_ExtensionInfo* extensionInfo);
  bool tryToBindVariable(const TopVariable& tv, Substitution& solution);
  bool tryToBindLastVariable(ACU_TreeDagNode* subject,
			     const TopVariable& tv,
			     Substitution& solution);
  bool greedyPureMatch(ACU_TreeDagNode* subject,
		       Substitution& solution,
		       ACU_ExtensionInfo* extensionInfo);
  bool forcedLoneVariableCase(ACU_TreeDagNode* subject,
			      const TopVariable& tv,
			      Substitution& solution,
			      Subproblem*& returnedSubproblem);
  //
  //	ArgVec greedy matcher.
  //
  int greedyMatch(ACU_DagNode* subject,
		  Substitution& solution,
		  ACU_ExtensionInfo* extensionInfo);
  int decidePhase1FailureMode(TopVariable& fv);
  int decidePhase2FailureMode();
  int greedyPureMatch(ACU_DagNode* subject,
		      Substitution& solution,
		      ACU_ExtensionInfo* extensionInfo);
  //
  //	Collapse matcher.
  //
  void bindUnboundVariablesToIdentity(Substitution& solution, int exception);
  bool uniqueCollapseMatch(DagNode* subject,
			   Substitution& solution,
			   Subproblem*& returnedSubproblem,
			   ExtensionInfo* extensionInfo);
  bool multiwayCollapseMatch(DagNode* subject,
			     Substitution& solution,
			     Subproblem*& returnedSubproblem,
			     ExtensionInfo* extensionInfo);
  bool collapseMatch(DagNode* subject,
		     Substitution& solution,
		     Subproblem*& returnedSubproblem,
		     ExtensionInfo* extensionInfo);
  //
  //	Full matcher.
  //
  bool fullMatch(ACU_DagNode* subject,
		 Substitution& solution,
		 Subproblem*& returnedSubproblem,
		 ACU_ExtensionInfo* extensionInfo);

  ACU_Symbol* const topSymbol;
  const bool matchAtTop;
  const bool collapsePossible;
  bool treeMatchOK;
  bool collectorSeen;
  MatchStrategy matchStrategy;
  int totalLowerBound;  // must have at least this total mutiplicity of subjects
  int totalUpperBound;	// can't have more than this total mutiplicity of subjects
  int maxPatternMultiplicity;  // must have at least on subject with >= this multiplicity
  int totalNonGroundAliensMultiplicity;
  int nrIndependentAliens;
  int nrExpectedUnboundVariables;  // used at semicompile time only
  LhsAutomaton* uniqueCollapseAutomaton;
  Vector<TopVariable> topVariables;
  Vector<GroundAlien> groundAliens;
  Vector<NonGroundAlien> groundedOutAliens;
  Vector<NonGroundAlien> nonGroundAliens;
  //
  //	Data storage for match-time use
  //
  Substitution local;
  Substitution scratch;
  //
  //	Use by ArgVec matcher only.
  //
  int lastUnboundVariable;
  int totalMultiplicity;
  Vector<int> currentMultiplicity;
  Vector<Subject> subjects;
  //
  //	Used by red-black matcher only.
  //
  int nrUnboundVariables;  // so we know when we are at the last unbound variable
  int matchedMultiplicity;  // so we know when we've matched at least 2 subjects
  ACU_Tree current;
  Vector<ACU_DagNode::Pair> matched;
};

inline ACU_Symbol* 
ACU_LhsAutomaton::getSymbol() const
{
  return topSymbol;
}

inline bool
ACU_LhsAutomaton::getCollapsePossible() const
{
  return collapsePossible;
}

inline Substitution&
ACU_LhsAutomaton::getLocal()
{
  return local;
}

inline Substitution&
ACU_LhsAutomaton::getLocal2()
{
  return scratch;
}

inline void
ACU_LhsAutomaton::addUniqueCollapseAutomaton(LhsAutomaton* ucAutomaton)
{
  uniqueCollapseAutomaton = ucAutomaton;
}

#ifdef DUMP
ostream& operator<<(ostream& s, ACU_LhsAutomaton::MatchStrategy strategy);
#endif

#endif
