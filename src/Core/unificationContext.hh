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
  UnificationContext(FreshVariableGenerator* freshVariableGenerator, int nrOriginalVariables, int variableFamily);

  VariableDagNode* makeFreshVariable(const ConnectedComponent* component);
  Sort* getFreshVariableSort(int index) const;
  int getNrOriginalVariables() const;

  void unificationBind(VariableDagNode* variable, DagNode* value);
  VariableDagNode* getVariableDagNode(int index);

  void restoreFromClone(const Substitution& substitutionClone);

protected:
  //
  //	We make this protected so that a derived class can override it and then call
  //	our version.
  //
  void markReachableNodes();

private:
  FreshVariableGenerator* const freshVariableGenerator;
  const int nrOriginalVariables;  // actually some the the slots could be unused
  const int variableFamily;  // what family of fresh variable names to use
  //
  //	Fresh variables are always create at the kind level. We keep track of this kind sort
  //	for each fresh variable we create.
  //
  Vector<Sort*> freshVariableSorts;
  //
  //	We track the dagnode of each variable bound using unificationBind(). These dagnodes are used
  //	fo unsolving solved forms (where we need to solve all problems in a given theory simultaneously
  //	for termination reasons) and for resolving compound cycles.
  //
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

inline int
UnificationContext::getNrOriginalVariables() const
{
  return nrOriginalVariables;
}

#endif
