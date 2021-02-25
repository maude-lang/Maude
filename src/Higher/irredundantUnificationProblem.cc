/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class IrredundantUnificationProblem.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "allSat.hh"
#include "bddUser.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"

//	interface class definitions
#include "dagNode.hh"

//	core class definitions
#include "rewritingContext.hh"

//	higher class definitions
#include "unifierFilter.hh"
#include "irredundantUnificationProblem.hh"

IrredundantUnificationProblem::IrredundantUnificationProblem(Vector<Term*>& lhs,
							     Vector<Term*>& rhs,
							     FreshVariableGenerator* freshVariableGenerator,
							     int incomingVariableFamily)
  : UnificationProblem(lhs, rhs, freshVariableGenerator, incomingVariableFamily)
{
  unifierFilter = 0;
}

IrredundantUnificationProblem::~IrredundantUnificationProblem()
{
  delete unifierFilter;
}
/*
void
IrredundantUnificationProblem::markReachableNodes()
{
  int nrFragile = currentUnifier->nrFragileBindings();
  for (int i = 0; i < nrFragile; i++)
    {
      DagNode* d = currentUnifier->value(i);
      if (d != 0)
	d->mark();
    }
  //
  //	Must mark nodes in base class.
  //
  UnificationProblem::markReachableNodes();
}
*/
bool
IrredundantUnificationProblem::findNextUnifier()
{
  if (unifierFilter == 0)
    {
      if (UnificationProblem::findNextUnifier())
	{
	  int nrOriginalVariables = getVariableInfo().getNrRealVariables();
	  unifierFilter = new UnifierFilter(0, nrOriginalVariables);
	  //
	  //	We need a RewritingContext& to call computeTrueSort() even
	  //	though we're not able to deal with sort constraints.
	  //
	  RewritingContext dummyContext;
	  do
	    {
	      const Substitution& unifier = UnificationProblem::getSolution();
	      int nrFreeVariables = UnificationProblem::getNrFreeVariables();
	      //
	      //	UnifierFilter expects all bindings to have their
	      //	all their sorts computed so we can match them.
	      //
	      for (int i = 0; i < nrOriginalVariables; ++i)
		unifier.value(i)->computeTrueSort(dummyContext);
	      unifierFilter->insertUnifier(unifier, nrFreeVariables);
	    }
	  while (UnificationProblem::findNextUnifier());
	}
      else
	return false;
    }
  //
  //	UnifierFilter stores two ints along with each unifier but we're only using
  //	the first one to hold the number of free variables.
  //
  int dummy;
  return unifierFilter->getNextSurvivingUnifier(currentUnifier, nrFreeVariables, dummy);
}

const Substitution&
IrredundantUnificationProblem::getSolution() const
{
  return *currentUnifier;
}

int
IrredundantUnificationProblem::getNrFreeVariables() const
{
  return nrFreeVariables;
}
