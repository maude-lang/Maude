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
//      Class for right hand side automata for S theory.
//
#ifndef _S_RhsAutomaton_hh_
#define _S_RhsAutomaton_hh_
#include <gmpxx.h>
#include "rhsAutomaton.hh"

class S_RhsAutomaton : public RhsAutomaton
{
  NO_COPYING(S_RhsAutomaton);

public:
  S_RhsAutomaton(S_Symbol* symbol, mpz_class& number, int source, int destination);
  //
  //    Functions required by theory interface.
  //
  void remapIndices(VariableInfo& variableInfo);
  DagNode* construct(Substitution& matcher);
  void replace(DagNode* old, Substitution& matcher);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  S_Symbol* const topSymbol;
  const mpz_class number;
  int source;
  int destination;
};

#endif
