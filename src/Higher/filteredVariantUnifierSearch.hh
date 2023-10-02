/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020-2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Class to search for a minimal complete set of variant unifiers.
//
//	We derive it from VariantSearch although it only specializes the
//	variant unification functionality of VariantSearch. This is so
//	we can use it as a drop-in replacement for VariantSearch in code
//	that uses VariantSearch to do variant unification.
//
#ifndef _filteredVariantUnifierSearch_hh_
#define _filteredVariantUnifierSearch_hh_
#include "variantSearch.hh"
#include "variantUnifierFilter.hh"

class FilteredVariantUnifierSearch : public VariantSearch
{
  NO_COPYING(FilteredVariantUnifierSearch);

public:
  //
  //	Only flags that can reasonably be passed are DELETE_FRESH_VARIABLE_GENERATOR
  //	and CHECK_VARIABLE_NAMES. UNIFICATION_MODE is always passed to the VariantSearch base object.
  //
  FilteredVariantUnifierSearch(RewritingContext* context,
			       const Vector<DagNode*>& blockerDags,
			       FreshVariableGenerator* freshVariableGenerator,
			       int flags = DELETE_FRESH_VARIABLE_GENERATOR | CHECK_VARIABLE_NAMES,
			       int incomingVariableFamily = NONE);
  bool findNextUnifier() override;
  const Vector<DagNode*>& getCurrentUnifier(int& nrFreeVariables, int& variableFamily) override;
  bool filteringIncomplete() const;

private:
  const int flags;
  VariantUnifierFilter unifiers;
};

inline bool
FilteredVariantUnifierSearch::filteringIncomplete() const
{
  return unifiers.filteringIncomplete();
}

#endif
