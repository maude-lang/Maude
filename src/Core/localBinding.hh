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
//      Class for local bindings within matching subproblems.
//
#ifndef _localBinding_hh_
#define _localBinding_hh_
#include "simpleRootContainer.hh"
#undef assert  //HACK

class LocalBinding : private SimpleRootContainer
{
  NO_COPYING(LocalBinding);

public:
  LocalBinding(int maxSize);

  void addBinding(int index, DagNode* value);
  bool assert(Substitution& substitution);
  void retract(Substitution& substitution);

  bool unificationAssert(Substitution& substitution, Subproblem*& returnedSubproblem);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel) const;
  void dump(ostream& s, const VariableInfo* variableInfo = 0) const;
#endif

private:
  struct Binding
  {
    bool active;
    int variableIndex;
    DagNode* value;
  };

  void markReachableNodes();

  Vector<Binding> bindings;
};

inline void
LocalBinding::addBinding(int index, DagNode* value)
{
  int t = bindings.length();
  bindings.expandBy(1);
  Binding& b = bindings[t];
  b.variableIndex = index;
  b.value = value;
  b.active = false;
}

#endif
