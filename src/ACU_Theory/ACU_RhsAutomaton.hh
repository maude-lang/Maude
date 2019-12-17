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
//      Class for right hand side automata in the AC and ACU theories.
//
#ifndef _ACU_RhsAutomaton_hh_
#define _ACU_RhsAutomaton_hh_
#include "rhsAutomaton.hh"

class ACU_RhsAutomaton : public RhsAutomaton
{
  NO_COPYING(ACU_RhsAutomaton);

public:
  ACU_RhsAutomaton(ACU_Symbol* symbol, int nrArgs);
  void addArgument(int index, int multiplicity);
  void close(int destinationIndex);
  //
  //	Standard RhsAutomaton operations.
  //
  void remapIndices(VariableInfo& variableInfo);
  DagNode* construct(Substitution& matcher);
  void replace(DagNode* old, Substitution& matcher);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  struct Argument
    {
      int index;
      int multiplicity;
    };

  void buildArguments(ArgVec<ACU_DagNode::Pair>& argArray, Substitution& matcher) const;
  
  ACU_Symbol* const topSymbol;
  Vector<Argument> arguments;
  int nrArguments;
  int destination;
};

#endif
