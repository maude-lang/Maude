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
//      Class for left hand side automata for theories made from C, Ul, Ur and I axioms.
//
#ifndef _CUI_LhsAutomaton_hh_
#define _CUI_LhsAutomaton_hh_
#include "lhsAutomaton.hh"

class CUI_LhsAutomaton : public LhsAutomaton
{
  NO_COPYING(CUI_LhsAutomaton);

public:
  enum Flags
  {
    GREEDY_MATCH_OK = 1,
    UNIQUE_BRANCH = 2,
    FORWARD = 4,
    REVERSE = 8,
    CONDITIONAL_REVERSE = 16,
    ID0_COLLAPSE = 32,			// Collapse to subpattern1 possible
    ID1_COLLAPSE = 64,			// Collapse to subpattern0 possible
    IDEM_COLLAPSE = 128,
    COLLAPSE = ID0_COLLAPSE | ID1_COLLAPSE | IDEM_COLLAPSE
  };

  CUI_LhsAutomaton(CUI_Symbol* topSymbol,
		   bool matchAtTop,
		   Flags flags,
		   int nrVariables);
  ~CUI_LhsAutomaton();

  bool addSubpattern(Term* term,
		     const VariableInfo& variableInfo,
		     NatSet& boundUniquely);
  //
  //	Standard LhsAutomaton operations.
  //
  bool match(DagNode* subject,
             Substitution& solution,
             Subproblem*& returnedSubproblem,
             ExtensionInfo* extensionInfo);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  enum PatternType
  {
    UNDEFINED,
    GROUND_ALIEN,
    VARIABLE,
    NON_GROUND_ALIEN
  };

  struct Subpattern
  {
    bool match(DagNode* subject,
	       Substitution& solution,
	       Subproblem*& returnedSubproblem,
	       bool matchAtTop = false,
	       ExtensionInfo* extensionInfo = 0);

#ifdef DUMP
    void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

    PatternType type;
    union
    {
      Term* term;		// for ground aliens
      int varIndex;		// for variables
    };
    LhsAutomaton* automaton;	// for non ground aliens (and sometimes variables)
    LhsAutomaton* topAutomaton;	// for when we match whole subject at top
    Sort* sort;			// for variables only
  };

  bool freeMatch(DagNode* subject0,
		 DagNode* subject1,
		 Substitution& solution,
		 Subproblem*& returnedSubproblem);
  bool id0CollapseMatch(DagNode* subject,
			Substitution& solution,
			Subproblem*& returnedSubproblem,
			ExtensionInfo* extensionInfo);
  bool id1CollapseMatch(DagNode* subject,
			Substitution& solution,
			Subproblem*& returnedSubproblem,
			ExtensionInfo* extensionInfo);
  bool idemCollapseMatch(DagNode* subject,
			 Substitution& solution,
			 Subproblem*& returnedSubproblem);
  bool idemCollapseMatch(DagNode* subject,
			 Substitution& solution,
			 Subproblem*& returnedSubproblem,
			 ExtensionInfo* extensionInfo);

  CUI_Symbol* const topSymbol;
  const bool matchAtTop;
  const Flags flags;
  Subpattern subpattern0;
  Subpattern subpattern1;
  //
  //	Data storage for match-time use
  //
  Substitution local;

  friend ostream& operator<<(ostream& s, PatternType type);
};

inline bool
CUI_LhsAutomaton::Subpattern::match(DagNode* subject,
				    Substitution& solution,
				    Subproblem*& returnedSubproblem,
				    bool matchAtTop,
				    ExtensionInfo* extensionInfo)
{
  if (matchAtTop)
    {
      //
      //	Even if the subject has no extension, the top automaton may
      //	need to do something special; like clone the subject's top
      //	node if it gets assigned to a variable that occurs in the
      //	rhs of an equation to avoid intoducing loop into dag.
      //
      return topAutomaton->match(subject, solution, returnedSubproblem, extensionInfo);
    }
  else if (type == VARIABLE)
    {
      DagNode* d = solution.value(varIndex);
      if (d == 0)
	{
	  if (subject->checkSort(sort, returnedSubproblem))
	    {
	      solution.bind(varIndex, subject);
	      return true;
	    }
	  return false;
	}
      else
	{
	  returnedSubproblem = 0;
	  return subject->equal(d);
	}
    }
  else if (type == GROUND_ALIEN)
    {
      returnedSubproblem = 0;
      return term->equal(subject);
    }
  return automaton->match(subject, solution, returnedSubproblem);
}

#ifdef DUMP
ostream& operator<<(ostream& s, CUI_LhsAutomaton::Flags flags);
ostream& operator<<(ostream& s, CUI_LhsAutomaton::PatternType type);
#endif

#endif
