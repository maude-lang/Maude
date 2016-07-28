/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2012 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for searching for single variant narrowing steps.
//
#ifndef _varientNarrowingSearchState_hh_
#define _varientNarrowingSearchState_hh_
#include "positionState.hh"
#include "narrowingVariableInfo.hh"
#include "unifierFilter.hh"

class VariantNarrowingSearchState : public PositionState
{
  NO_COPYING(VariantNarrowingSearchState);

public:
  //
  //	We assume that we are the only process working on the DagNodes in context and variantSubsitution and
  //	we can therefore write what values we like into the indices of reachable VariableDagNodes.
  //
  VariantNarrowingSearchState(RewritingContext* context,  // contains the variant term
			      const Vector<DagNode*>& variantSubstitution,
			      const Vector<DagNode*>& blockerDags,
			      FreshVariableGenerator* freshVariableGenerator,
			      bool odd,
			      const NarrowingVariableInfo& originalVariables,
			      bool unificationMode = false);
  ~VariantNarrowingSearchState();

  //
  //	Variant passed back is not GC protected by us.
  //
  bool findNextVariant(DagNode*& variantTerm, Vector<DagNode*>& variantSubstitution /*, int& nrFreeVariables */);
  bool isIncomplete() const;
private:
  void collectUnifiers(NarrowingUnificationProblem* unificationProblem, int positionIndex, int equationIndex);

  RewritingContext* const context;  // has own GC protection
  const Vector<DagNode*>& variantSubstitution;  // assumed to be protected from GC by whatever passed it to us
  const Vector<DagNode*>& blockerDags;  // assumed to be protected from GC by whatever passed it to us
  FreshVariableGenerator* const freshVariableGenerator;
  const NarrowingVariableInfo& originalVariables;  // assumed to be protected from GC by whatever passed it to us; only needed for tracing
  Module* const module;

  bool incompleteFlag;
  NarrowingVariableInfo variableInfo;
  UnifierFilter* unifiers;  // has own GC protection
  Substitution blockerSubstitution;  // filled out and done with before GC happens
};

inline bool
VariantNarrowingSearchState::isIncomplete() const
{
  return incompleteFlag;
}

#endif
