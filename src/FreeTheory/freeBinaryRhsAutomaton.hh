/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	there is a single binary function symbol.
//
#ifndef _freeBinaryRhsAutomaton_hh_
#define _freeBinaryRhsAutomaton_hh_
#include "freeRhsAutomaton.hh"

class FreeBinaryRhsAutomaton : public FreeRhsAutomaton
{
public:
  //
  //	We hook in code for fast copies here.
  //
  void remapIndices(VariableInfo& variableInfo);
  //
  //	We just replace a couple of virtual functions with ones optimized for this case.
  //
  DagNode* construct(Substitution& matcher);
  void replace(DagNode* old, Substitution& matcher);

private:
  void fillOutArgs(Substitution& matcher, FreeDagNode* d);

  Symbol* symbol;
  int sources[2];
  int destination;
};

#endif
