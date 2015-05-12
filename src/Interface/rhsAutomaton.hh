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
//      Abstract base class for right hand side automata.
//
#ifndef _rhsAutomaton_hh_
#define _rhsAutomaton_hh_

class RhsAutomaton
{
public:
  virtual ~RhsAutomaton() {}

  virtual void remapIndices(VariableInfo& variableInfo) = 0;
  virtual DagNode* construct(Substitution& matcher) = 0;
  virtual void replace(DagNode* old, Substitution& matcher) = 0;

  virtual bool recordInfo(StackMachineRhsCompiler& compiler) { return false;  /* HACK until new engine is supported by all theories */ }

#ifdef DUMP
  virtual void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel = 0) = 0;
#endif

};

#endif
