/*

    This file is part of the Maude 3 interpreter.

    Copyright 2026 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for ground right hand side automata which just does a construct
//	or replace using a ground Term*. This is slower than using an automaton
//	where all the sharing and slots for intermediate results have already been
//	worked out. It may be preferable if the ground Term* is so large it causes
//	the rhs compiler to be unreasonably slow or use vast amounts of memory; or
//	produces a requirement for a huge substitution.
//
#ifndef _groundRhsAutomaton_hh_
#define _groundRhsAutomaton_hh_
#include "rhsAutomaton.hh"

class GroundRhsAutomaton : public RhsAutomaton
{
  NO_COPYING(GroundRhsAutomaton);

public:
  GroundRhsAutomaton(Term* groundTerm);

  void remapIndices(VariableInfo& variableInfo) {}
  DagNode* construct(Substitution& matcher);
  void replace(DagNode* old, Substitution& matcher);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  //
  //	We assume markEager has already been called during the preprocessing
  //	when it was identified as being huge and ground.
  //	
  Term* groundTerm;
};

#endif
