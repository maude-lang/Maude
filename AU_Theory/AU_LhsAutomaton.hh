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
//      Class for left hand side automata in the A, AUl, AUr and AU theories.
//
#ifndef _AU_LhsAutomaton_hh_
#define _AU_LhsAutomaton_hh_
#include "lhsAutomaton.hh"

class AU_LhsAutomaton : public LhsAutomaton
{
  NO_COPYING(AU_LhsAutomaton);

public:
  enum MatchStrategy
  {
    //
    //	Ground out case: no extension and no flex part; everything is in
    //	the rigid part.
    //
    GROUND_OUT,
    //
    //	Lone variable case: no extension and flex part consists of a single
    //	variable that is forced to match whatever is left after the rigid part
    //	has been matched.
    //
    LONE_VARIABLE,
    //
    //	Fast lone variable case: as above but variable has a PURE_SORT
    //	and is not an abstraction variable.
    //
    FAST_LONE_VARIABLE,
    //
    //	Greedy case: In the flex part, the intersection of each subterm's variables
    //	with union of its context variables and the condition variables contains
    //	only variables guaranteed to be bound once the rigid part has been matched.
    //	None of the subautomata for the flex part are likely to return subproblems and
    //	each variable in the subterm that is not guaranteed to be bound has either a
    //	LIMIT_SORT or a PURE_SORT or has upperBound = 1.
    //
    GREEDY,
    //
    //	Full case: if other cases don't apply; build a DAG which provides a
    //	compact representation of the possibly exponential number of matching
    //	possibilities and pass it back as a subproblem.
    //
    FULL
  };
  //
  //	Operations to build an AU_LhsAutomaton
  //
  AU_LhsAutomaton(AU_Symbol* symbol,
		  bool matchAtTop,
		  bool collapsePossible,
		  LhsAutomaton* uniqueCollapseAutomaton,
		  int nrVariables);
  ~AU_LhsAutomaton();

  void addRigidVariable(const VariableTerm* variable, bool leftEnd, bool idPossible);
  void addRigidGroundAlien(Term* alien, bool leftEnd);
  void addRigidNonGroundAlien(LhsAutomaton* automaton, bool leftEnd);
  void addFlexVariable(const VariableTerm* variable, int shiftFactor, bool idPossible);
  void addFlexAbstractionVariable(int index,
				  Sort* sort,
				  int upperBound,
				  bool takeIdentity,
				  bool awkward,
				  LhsAutomaton* abstracted);
  void addFlexGroundAlien(Term* alien, int shiftFactor);
  void addFlexNonGroundAlien(LhsAutomaton* automaton, int shiftFactor);
  void complete(MatchStrategy strategy);
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

private:
  enum SubtermType
  {
    VARIABLE,
    GROUND_ALIEN,
    NON_GROUND_ALIEN
  };

  enum Special
  {
    STALLED = -1,
    NOT_FIXED = -1
  };

  struct TopVariable
  {
    short index;
    Bool takeIdentity;		// take identity instead of piece of subject (always)
    Bool awkward;		// take identity instead of piece of subject (special)
    Sort* sort;
    int upperBound;
    LhsAutomaton* abstracted;	// automaton for abstracted term
    //
    //	Data storage for full match time use, flex part only
    //
    int nastyBinding;		// if >= 0, minimum number of subterms
    				// that binding might match
    //
    //	Data storage for greedy match time use, flex part only
    //
    int firstSubject;
    int lastSubject;
  };

  struct Subterm
  {
    SubtermType type;
    int shiftFactor;	// for flex part only
    union
    {
      bool leftEnd;	// for rigid part only
      int blockLength;	// for flex part only; +ve for fixed start, -1 for variable
    };
    union
    {
      TopVariable variable;
      Term* groundAlien;
      LhsAutomaton* alienAutomaton;
    };
  };

  //
  //	Rigid blocks within the flex part are only determined at match time
  //	since they depend on what variables are bound.
  //
  struct RigidBlock
  {
    int start;		// index into flexPart
    int end;		// index into flexPart
    int nrSubjectsForUs;	// needed because value of bound variable may be in our theory
    int nrSubjectsToLeave;	// for preceeding variables
    int firstMatch;	// first matching shift for this rigid block
  };

  void updateWholeBounds(int min, int max);
  void updateFlexBounds(int min, int max);
  //
  //	First rigid part matcher and preprocessing code.
  //
  bool matchRigidPart(AU_DagNode* subject,
		      Substitution& solution,
		      SubproblemAccumulator& subproblems);
  bool checkForRigidEnds(AU_DagNode* subject,
			 Substitution& solution,
			 SubproblemAccumulator& subproblems);
  int checkLeftEnd(AU_DagNode* subject,
		   Substitution& solution,
		   SubproblemAccumulator& subproblems,
		   int& mustLeave,
		   bool& rightStalled);
  int checkRightEnd(AU_DagNode* subject,
		    Substitution& solution,
		    SubproblemAccumulator& subproblems,
		    int& mustLeave,
		    bool& leftStalled);
  bool forcedLoneVariableCase(AU_DagNode* subject,
			      Substitution& solution,
			      Subproblem*& returnedSubproblem);
  void determineRigidBlocks(Substitution& solution);
  //
  //	Greedy Matcher.
  //
  int greedyMatch(AU_DagNode* subject,
		  Substitution& solution,
		  AU_ExtensionInfo* extensionInfo);
  void greedyBindVariables(AU_DagNode* subject,
			   Substitution& solution);
  int greedyMatchRigidBlock(AU_DagNode* subject,
			    RigidBlock& block,
			    int minShift,
			    int maxShift,
			    int& matchShift);
  int greedyMatchRigidBlock2(AU_DagNode* subject,
			     RigidBlock& block,
			     int nextSubject,
			     int& shiftFactor);
  int greedyMatchFixedLengthBlock(ArgVec<DagNode*>& args,
				  int blockStart,
				  int attemptedShift,
				  int& shiftFactor);
  int greedyMatchBlocks(AU_DagNode* subject, AU_ExtensionInfo* extensionInfo);
  bool greedyMatchVariableBlock(ArgVec<DagNode*>& args,
				int blockStart,
				int blockLength,
				int firstSubject,
				int nrSubjects,
				int spare,
				bool leftExtend,
				bool rightExtend,
				AU_ExtensionInfo* extensionInfo);
  //
  //	Full Matcher.
  //
  void addVariableBlocks(Substitution& solution,
			 AU_Subproblem* subproblem,
			 SubproblemAccumulator& subproblems);
  bool fullMatchRigidBlock(AU_DagNode* subject,
			   Substitution& solution,
			   RigidBlock& block,
			   int minShift,
			   int maxShift,
			   int& matchShift,
			   Subproblem*& subproblem);
  bool fullMatchRigidBlock2(AU_DagNode* subject,
			    RigidBlock& block,
			    int nextSubject,
			    Subproblem*& subproblem,
			    int& shiftFactor);
  bool fullMatchFixedLengthBlock(ArgVec<DagNode*>& args,
				 int blockStart,
				 int attemptedShift,
				 SubproblemAccumulator& subproblems,
				 int& shiftFactor);
  AU_Subproblem* buildLeftmostPath(AU_DagNode* subject,
				   Substitution& solution,
				   AU_ExtensionInfo* extensionInfo);
  void addRemainingPaths(AU_DagNode* subject,
			 Substitution& solution,
			 AU_Subproblem* subproblem);
  bool fullMatch(AU_DagNode* subject,
		 Substitution& solution,
		 SubproblemAccumulator& subproblems,
		 AU_ExtensionInfo* extensionInfo);
  //
  //	Collapse matcher.
  //
  bool collapseMatch(DagNode* subject,
		     Substitution& solution,
		     Subproblem*& returnedSubproblem,
		     ExtensionInfo* extensionInfo);
  bool uniqueCollapseMatch(DagNode* subject,
			   Substitution& solution,
			   Subproblem*& returnedSubproblem,
			   ExtensionInfo* extensionInfo);
  bool multiwayCollapseMatch(DagNode* subject,
			     Substitution& solution,
			     Subproblem*& returnedSubproblem,
			     ExtensionInfo* extensionInfo);
  void bindUnboundVariablesToIdentity(Substitution& solution, int exception);

  int dequeMatch(AU_DequeDagNode* subject,
		 Substitution& solution,
		 Subproblem*& returnedSubproblem);

  AU_Symbol* const topSymbol;
  const bool matchAtTop;
  const bool collapsePossible;
  LhsAutomaton* const uniqueCollapseAutomaton;
  MatchStrategy matchStrategy;
  int wholeLowerBound;
  int wholeUpperBound;
  int flexLowerBound;
  int flexUpperBound;
  Vector<Subterm> rigidPart;
  Vector<Subterm> flexPart;
  //
  //	Data storage for match-time use.
  //
  //	Left and right limits of subject not matched by rigid part.
  //
  int leftPos;
  int rightPos;
  //
  //	Left and right limits of flex part not used up.
  //
  int flexLeftPos;
  int flexRightPos;
  //
  //	Rigid blocks within flex part determined by examining variable
  //	bindings at match time.
  //
  Vector<RigidBlock> rigidBlocks;
  //
  //	Minimum number of subjects needed for flex part.
  //
  int nrSubjectsUsed;
  //
  //	Minimum number of subjects needed for variables to the right of
  //	last rigid block.
  //
  int nrSubjectsForRightVars;
  //
  //	Throw away substitutions for exploring match space.
  //
  Substitution local;
  Substitution scratch;

  friend ostream& operator<<(ostream& s, SubtermType type);
};

#ifdef DUMP
ostream& operator<<(ostream& s, AU_LhsAutomaton::MatchStrategy strategy);
ostream& operator<<(ostream& s, AU_LhsAutomaton::SubtermType type);
#endif

#endif
