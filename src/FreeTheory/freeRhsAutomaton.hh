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
//	Class for right hand side automata in the free theory.
//
#ifndef _freeRhsAutomaton_hh_
#define _freeRhsAutomaton_hh_
#include "rhsAutomaton.hh"

class FreeRhsAutomaton : public RhsAutomaton
{
public:
  void addFree(Symbol* symbol,
	       int destination,
	       const Vector<int>& sources);
  void remapIndices(VariableInfo& variableInfo);
  DagNode* construct(Substitution& matcher);
  void replace(DagNode* old, Substitution& matcher);

  bool recordInfo(StackMachineRhsCompiler& compiler);

  Index getNrInstructions() const;
  int getArity(Index instructionNr) const;

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

  //#define FREE_DAG_NODE_LOOP(s)	FreeDagNode(s)
#define FREE_DAG_NODE_LOOP(s)	FreeDagNode(s, '\0')
#define FREE_DAG_NODE(s)	FreeDagNode(s, '\0')

protected:
  struct Instruction
    {
      Symbol* symbol;
      int destination;
      Vector<int> sources;
    };

private:
  static void fillOutArgs(const Instruction& instr,
			  Substitution& matcher,
			  FreeDagNode* d);

protected:
  Vector<Instruction> instructions;
};

inline Index
FreeRhsAutomaton::getNrInstructions() const
{
  return instructions.size();
}

inline int
FreeRhsAutomaton::getArity(Index instructionNr) const
{
  return instructions[instructionNr].sources.size();
}

#endif
