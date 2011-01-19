/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2007 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class UnificationContext.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	core class definitions
#include "freshVariableGenerator.hh"
#include "connectedComponent.hh"
#include "unificationContext.hh"

//	variable class definitions
#include "variableDagNode.hh"

UnificationContext::UnificationContext(FreshVariableGenerator* freshVariableGenerator, int nrOriginalVariables)
  : Substitution(nrOriginalVariables),
    freshVariableGenerator(freshVariableGenerator),
    nrOriginalVariables(nrOriginalVariables)
{
}

void
UnificationContext::markReachableNodes()
{
  //
  //	Protect our bindings.
  //
  int nrFragile = nrFragileBindings();
  for (int i = 0; i < nrFragile; ++i)
    {
      if (DagNode* d = value(i))
	d->mark();
    }
  //
  //	Protect the variables that we've tracked for each slot.
  //
  int nrVariableDagNodes = variableDagNodes.size();
  for (int i = 0; i < nrVariableDagNodes; ++i)
    {
      if (DagNode* v = variableDagNodes[i])
	v->mark();
    }
}

VariableDagNode*
UnificationContext::makeFreshVariable(const ConnectedComponent* component)
{
  Sort* s = component->sort(Sort::ERROR_SORT);
  Symbol* vs = freshVariableGenerator->getBaseVariableSymbol(s);
  int index = addNewVariable();
  int freshVariableNr = index - nrOriginalVariables;
  freshVariableSorts.resize(freshVariableNr + 1);
  freshVariableSorts[freshVariableNr] = s;
  int name = freshVariableGenerator->getFreshVariableName(freshVariableNr);
  VariableDagNode* v = new VariableDagNode(vs, name, index);
  //cout << "created " << v << endl;
  return v;
}

void
UnificationContext::unificationBind(VariableDagNode* variable, DagNode* value)
{
  int index = variable->getIndex();
  DebugAdvisory("unificationBind() index = " << index << '\t' << safeCast(DagNode*, variable) << " <- " << value);
  bind(index, value);

  int nrVariableDagNodes = variableDagNodes.size();
  if (index >= nrVariableDagNodes)
    {
      variableDagNodes.resize(index + 1);
      for (int i = nrVariableDagNodes; i < index; ++i)
	variableDagNodes[i] = 0;  // for GC safety
    }
  else
    {
      Assert(variableDagNodes[index] == 0 || variableDagNodes[index]->equal(variable),
	     "inconsistancy for index " << index << ' ' <<
	     safeCast(DagNode*, variableDagNodes[index]) << " vs " << safeCast(DagNode*, variable));
    }
  variableDagNodes[index] = variable;
}
