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
//      Class for left hand side automata for S theory.
//
#ifndef _S_LhsAutomaton_hh_
#define _S_LhsAutomaton_hh_
#include <gmpxx.h>
#include "lhsAutomaton.hh"

class S_LhsAutomaton : public LhsAutomaton
{
  NO_COPYING(S_LhsAutomaton);

public:
  S_LhsAutomaton(S_Symbol* symbol,
		 const mpz_class& number,
		 bool matchAtTop);
  ~S_LhsAutomaton();
  bool addSubpattern(Term* term,
		     int abstractionVariableIndex,
		     const VariableInfo& variableInfo,
		     NatSet& boundUniquely);
  //
  //    Functions required by theory interface.
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
    GROUND_ALIEN,
    VARIABLE,
    NON_GROUND_ALIEN,
    ABSTRACTED_NON_GROUND_ALIEN,
  };

  bool matchEq(DagNode* arg,
	       Substitution& solution,
	       Subproblem*& returnedSubproblem);
  bool nonExtMatchGt(DagNode* subject,
		     DagNode* arg,
		     const mpz_class& subjectNumber,
		     Substitution& solution,
		     Subproblem*& returnedSubproblem);
  bool extMatchGt(S_DagNode* subject,
		  DagNode* arg,
		  const mpz_class& subjectNumber,
		  Substitution& solution,
		  Subproblem*& returnedSubproblem,
		  S_ExtensionInfo* extensionInfo);

  S_Symbol* const topSymbol;
  const mpz_class number;
  const bool matchAtTop;
  //
  //	Stuff for argument.
  //
  PatternType type;
  union
  {
    Term* groundTerm;		// GROUND_ALIEN cases
    LhsAutomaton* automaton;	// NON_GROUND_ALIEN & ABSTRACTED_NON_GROUND_ALIEN cases
  };
  int varIndex;			// VARIABLE & ABSTRACTED_NON_GROUND_ALIEN cases
  Sort* sort;			// VARIABLE & ABSTRACTED_NON_GROUND_ALIEN cases
};

#endif
