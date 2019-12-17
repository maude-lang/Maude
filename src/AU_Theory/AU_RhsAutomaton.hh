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
//      Class for right hand side automata in the the A, AUl, AUr and AU theories.
//
#ifndef _AU_RhsAutomaton_hh_
#define _AU_RhsAutomaton_hh_
#include "rhsAutomaton.hh"

class AU_RhsAutomaton : public RhsAutomaton
{
  NO_COPYING(AU_RhsAutomaton);

public:
  AU_RhsAutomaton(AU_Symbol* symbol, int nrArgs);

  void remapIndices(VariableInfo& variableInfo);
  DagNode* construct(Substitution& matcher);
  void replace(DagNode* old, Substitution& matcher);
  void addArgument(int index);
  void close(int destinationIndex);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  void buildArguments(ArgVec<DagNode*>& argArray, Substitution& matcher) const;

  AU_Symbol* const topSymbol;
  Vector<int> arguments;
  int nrArguments;
  int destination;
};

inline void
AU_RhsAutomaton::addArgument(int index)
{
  arguments.append(index);
}

inline void
AU_RhsAutomaton::close(int destinationIndex)
{
  destination = destinationIndex;
  nrArguments = arguments.size();
}

#endif
