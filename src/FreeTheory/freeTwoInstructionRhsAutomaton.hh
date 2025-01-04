/*

    This file is part of the Maude 3 interpreter.

    Copyright 2025 SRI International, Menlo Park, CA 94025, USA.

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
//	Template class for right hand side automata in the free theory
//	optimized for the case where there are two low arity function symbols.
//
#ifndef _freeTwoInstructionAutomaton_hh_
#define _freeTwoInstructionAutomaton_hh_
#include "freeRhsAutomaton.hh"
#include "freeDagNode.hh"

template<int n, int m>  // arity n followed by arity m >= 1
class FreeTwoInstructionRhsAutomaton : public FreeRhsAutomaton
{
public:
  FreeTwoInstructionRhsAutomaton(FreeRhsAutomaton& victim) : FreeRhsAutomaton(std::move(victim)) {}
  //
  //	We hook in code to make a fast copy of two instructions.
  //
  void remapIndices(VariableInfo& variableInfo);
  //
  //	We just replace a couple of virtual functions with ones optimized for this case.
  //
  DagNode* construct(Substitution& matcher);
  void replace(DagNode* old, Substitution& matcher);

private:
  Symbol* symbol;
  Symbol* symbol2;
  int sources[n];
  int sources2[m];
  int destination;
  int destination2;
};

template<int n, int m>
DagNode*
FreeTwoInstructionRhsAutomaton<n, m>::construct(Substitution& matcher)
{
  FreeDagNode* d = new FREE_DAG_NODE(symbol);
  fillArgs<n>(sources, matcher, d);
  matcher.bind(destination, d);
  FreeDagNode* d2 = new FREE_DAG_NODE(symbol2);
  fillArgs<m>(sources2, matcher, d2);
  matcher.bind(destination2, d2);
  return d2;
}

template<int n, int m>
void
FreeTwoInstructionRhsAutomaton<n, m>::replace(DagNode* old, Substitution& matcher)
{
  FreeDagNode* d = new FREE_DAG_NODE(symbol);
  fillArgs<n>(sources, matcher, d);
  matcher.bind(destination, d);
  FreeDagNode* d2 = new(old) FREE_DAG_NODE(symbol2);
  fillArgs<m>(sources2, matcher, d2);
}

template<int n, int m>
void
FreeTwoInstructionRhsAutomaton<n, m>::remapIndices(VariableInfo& variableInfo)
{
  //
  //	Standard processing.
  //
  FreeRhsAutomaton::remapIndices(variableInfo);
  //
  //	Make fast copy of the two instructions.
  //
  {
    const Instruction& instr = instructions[0];
    symbol = instr.symbol;
    for (Index i = 0; i < n; ++i)
      sources[i] = instr.sources[i];
    destination = instr.destination;
  }
  {
    const Instruction& instr2 = instructions[1];
    symbol2 = instr2.symbol;
    for (Index i = 0; i < m; ++i)
      sources2[i] = instr2.sources[i];
    destination2 = instr2.destination;
  }
}

#endif
