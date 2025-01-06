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
//	optimized for the case where there are three low arity function symbols.
//
#ifndef _freeThreeInstructionAutomaton_hh_
#define _freeThreeInstructionAutomaton_hh_
#include "freeRhsAutomaton.hh"
#include "freeDagNode.hh"

template<int n, int m, int p>  // arity n followed by arity m followed by arity p >= 1
class FreeThreeInstructionRhsAutomaton : public FreeRhsAutomaton
{
public:
  FreeThreeInstructionRhsAutomaton(FreeRhsAutomaton& victim) :
    FreeRhsAutomaton(std::move(victim)) {}
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
  Symbol* symbol3;
  int sources[n];
  int sources2[m];
  int sources3[p];
  int destination;
  int destination2;
  int destination3;
};

template<int n, int m, int p>
DagNode*
FreeThreeInstructionRhsAutomaton<n, m, p>::construct(Substitution& matcher)
{
  FreeDagNode* d = new FREE_DAG_NODE(symbol);
  fillArgs<n>(sources, matcher, d);
  matcher.bind(destination, d);

  FreeDagNode* d2 = new FREE_DAG_NODE(symbol2);
  fillArgs<m>(sources2, matcher, d2);
  matcher.bind(destination2, d2);

  FreeDagNode* d3 = new FREE_DAG_NODE(symbol3);
  fillArgs<p>(sources3, matcher, d3);
  matcher.bind(destination3, d3);
  return d3;
}

template<int n, int m, int p>
void
FreeThreeInstructionRhsAutomaton<n, m, p>::replace(DagNode* old, Substitution& matcher)
{
  FreeDagNode* d = new FREE_DAG_NODE(symbol);
  fillArgs<n>(sources, matcher, d);
  matcher.bind(destination, d);
  
  FreeDagNode* d2 = new FREE_DAG_NODE(symbol2);
  fillArgs<m>(sources2, matcher, d2);
  matcher.bind(destination2, d2);
  
  FreeDagNode* d3 = new(old) FREE_DAG_NODE(symbol3);
  fillArgs<p>(sources3, matcher, d3);
}

template<int n, int m, int p>
void
FreeThreeInstructionRhsAutomaton<n, m, p>::remapIndices(VariableInfo& variableInfo)
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
  {
    const Instruction& instr3 = instructions[2];
    symbol3 = instr3.symbol;
    for (Index i = 0; i < p; ++i)
      sources3[i] = instr3.sources[i];
    destination3 = instr3.destination;
  }
}

#endif
