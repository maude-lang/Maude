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
//      Class for left hand side automata in the free theory.
//
#ifndef _freeLhsAutomaton_hh_
#define _freeLhsAutomaton_hh_
#include "lhsAutomaton.hh"
#include "freeLhsStructs.hh"

class FreeLhsAutomaton : public LhsAutomaton, private FreeLhsStructs
{
  NO_COPYING(FreeLhsAutomaton);

public:
  FreeLhsAutomaton(const Vector<FreeOccurrence>& freeSymbols,
		   const Vector<FreeOccurrence>& uncertainVars,
		   const Vector<FreeOccurrence>& boundVars,
		   const Vector<FreeOccurrence>& gndAliens,
		   const Vector<FreeOccurrence>& nonGndAliens,
		   const Vector<int>& bestSequence,
		   const Vector<LhsAutomaton*>& subAutomata);
  ~FreeLhsAutomaton();

  bool match(DagNode* subject,
	     Substitution& solution,
	     Subproblem*& returnedSubproblem,
	     ExtensionInfo* extensionInfo);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  struct FreeSubterm
    {
      int position;
      int argIndex;
      Symbol* symbol;
      int saveIndex;  // for left->right sharing
    };

  Symbol* topSymbol;
  Vector<DagNode**> stack;
  Vector<FreeSubterm> freeSubterms;
  Vector<FreeVariable> uncertainVariables;
  Vector<BoundVariable> boundVariables;
  Vector<GroundAlien> groundAliens;
  Vector<NonGroundAlien> nonGroundAliens;
};

#endif
