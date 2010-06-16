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
//	Class for unification contexts.
//
#ifndef _unificationContext_hh_
#define _unificationContext_hh_
#include "substitution.hh"
#include "simpleRootContainer.hh"

//
//	In addition to a substitution, we keep a track of stuff needed for unification:
//	(1) a fresh variable generator
//	(2) the sort (always a kind actually) of fresh variables
//	(3) A dagnode representation of each variable that gets bound
//
class UnificationContext : public Substitution, private SimpleRootContainer
{
public:
  UnificationContext(FreshVariableGenerator* freshVariableGenerator, int nrOriginalVariables);

  DagNode* makeFreshVariable(ConnectedComponent* component);
  Sort* getFreshVariableSort(int index) const;

  void unificationBind(VariableDagNode* variable, DagNode* value);
  VariableDagNode* getVariableDagNode(int index);

protected:
  //
  //	We make this protected so that a derived class can override it and then call
  //	our version.
  //
  void markReachableNodes();

private:
  FreshVariableGenerator* const freshVariableGenerator;
  const int nrOriginalVariables;
  Vector<Sort*> freshVariableSorts;
  Vector<VariableDagNode*> variableDagNodes;
};

inline Sort*
UnificationContext::getFreshVariableSort(int index) const
{
  return freshVariableSorts[index - nrOriginalVariables];
}

inline VariableDagNode*
UnificationContext::getVariableDagNode(int index)
{
  int nrVariableDagNodes = variableDagNodes.size();
  return (index < nrVariableDagNodes) ? variableDagNodes[index] : 0;
}

#endif
