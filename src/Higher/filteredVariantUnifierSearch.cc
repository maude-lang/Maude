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
//	Implementation for class FilteredVariantUnifierSearch.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "higher.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//	core class definitions
#include "rewritingContext.hh"

//	higher class definitions
#include "filteredVariantUnifierSearch.hh"

FilteredVariantUnifierSearch::FilteredVariantUnifierSearch(RewritingContext* context,
							   const Vector<DagNode*>& blockerDags,
							   FreshVariableGenerator* freshVariableGenerator,
							   int flags,
							   int incomingVariableFamily)
  : VariantSearch(context,
		  blockerDags,
		  freshVariableGenerator,
		  flags | UNIFICATION_MODE,
		  incomingVariableFamily),
    flags(flags),
    unifiers(context, freshVariableGenerator)
{
  if (flags & IRREDUNDANT_MODE)
    {
      //
      //	Extract and filter all unifiers before returning one.
      //
      //	This means that later unifiers can subsume earlier unifiers
      //	and remove them from the returned set of unifiers.
      //
      //	In practice, because variants are expanded breathfirst, it
      //	seems rare that a later (and hence more narrowed) unifier
      //	can subsume an earlier one, but sometimes one just wants
      //	to be sure that no subsumed unifiers are returned.
      //
      while (VariantSearch::findNextUnifier())
	{
	  int nrFreeVariables;
	  int variableFamily;
	  const Vector<DagNode*>& unifier = VariantSearch::getCurrentUnifier(nrFreeVariables, variableFamily);
	  unifiers.insertUnifier(unifier, nrFreeVariables, variableFamily);
	}
    }
}

bool
FilteredVariantUnifierSearch::findNextUnifier()
{
  if (flags & IRREDUNDANT_MODE)
    return unifiers.findNextSurvivingUnifier();
  //
  //	Incremental extraction.
  //	Each time we insert a unifier, we check to see if we have a survivor.
  //
  do
    {
      if (VariantSearch::findNextUnifier())
	{
	  int nrFreeVariables;
	  int variableFamily;
	  const Vector<DagNode*>& unifier = VariantSearch::getCurrentUnifier(nrFreeVariables, variableFamily);
	  unifiers.insertUnifier(unifier, nrFreeVariables, variableFamily);
	}
      else
	return false;
    }
  while (!(unifiers.findNextSurvivingUnifier()));
  return true;
}

const Vector<DagNode*>&
FilteredVariantUnifierSearch::getCurrentUnifier(int& nrFreeVariables, int& variableFamily)
{
  return unifiers.getCurrentSurvivingUnifier(nrFreeVariables, variableFamily);
}
