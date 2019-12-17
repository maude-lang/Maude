/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2017 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for right hand side automata in the free theory optimized for the case where
//	all function symbols have <= 3 arguments
//
#ifndef _freeFast3RhsAutomaton_hh_
#define _freeFast3RhsAutomaton_hh_
#include "freeRhsAutomaton.hh"

class FreeFast3RhsAutomaton : public FreeRhsAutomaton
{
public:
  //
  //	We hook in code for dummy arguments here.
  //
  void remapIndices(VariableInfo& variableInfo);
  //
  //	We just replace a couple of virtual functions with ones optimized for this case.
  //
  DagNode* construct(Substitution& matcher);
  void replace(DagNode* old, Substitution& matcher);

private:
  struct FastInstruction
    {
      Symbol* symbol;
      int sources[3];
      int destination;
    };

  static void fillOutArgs(const FastInstruction& instr,
			  Substitution& matcher,
			  FreeDagNode* d);

  Vector<FastInstruction>::size_type nrInstructions;  // so we can get this without accounting for null vector possibility
  Vector<FastInstruction> fastInstructions;
};

#endif
