/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for searching for single narrowing step using variant unification.
//	This version is used for sequence search.
//
#ifndef _narrowingSearchState3_hh_
#define _narrowingSearchState3_hh_
#include "positionState.hh"
#include "simpleRootContainer.hh"
#include "narrowingVariableInfo.hh"
#include "variantUnificationProblem.hh"

class NarrowingSearchState3 : public PositionState
{
  NO_COPYING(NarrowingSearchState3);

public:
  enum Flags
  {
    ALLOW_NONEXEC = 32,		// allow narrowing with nonexecutable rules, unbound
    				// variables being treated as fresh
    GC_VAR_GEN = 64,		// delete freshVariableGenerator in dtor
  };
  //
  //	Narrowing is done without extension and maxDepth may be
  //	UNBOUNDED to indicate no bound.
  //
  //	Takes over responsibility for deleting context and on destruction.
  //	Can also take responsibility for deleting variable generator if flag given.
  //
  NarrowingSearchState3(RewritingContext* context,
			Substitution* accumulatedSubstitution,
			FreshVariableGenerator* freshVariableGenerator,
			int incomingVariableFamily,
			int flags,
			int minDepth = 0,
			int maxDepth = UNBOUNDED,
			int variantFlags = 0);
  ~NarrowingSearchState3();

  bool findNextNarrowing();
  RewritingContext* getContext() const;
  //
  //	Get information about last narrowing step.
  //
  Rule* getRule() const;
  DagNode* getNarrowedDag(DagNode*& replacement, DagNode*& replacementContext) const;
  const Substitution& getUnifier() const;
  const NarrowingVariableInfo& getVariableInfo() const;
  int getVariableFamily() const;
  DagNode* getReplacedDag() const;
  Substitution* makeAccumulatedSubstitution();
  bool isIncomplete() const;

private:
  RewritingContext* const context;
  Substitution* const accumulatedSubstitution;
  const Vector<DagNode*> blockerDags;  // always an empty vector
  FreshVariableGenerator* const freshVariableGenerator;
  const int incomingVariableFamily;
  const int variantFlags;
  Module* const module;

  NarrowingVariableInfo variableInfo;
  int ruleIndex;  // index of current rule being tried
  VariantUnificationProblem* unificationProblem;
  //int variableFamily;
  bool incompleteFlag;
};

inline RewritingContext*
NarrowingSearchState3::getContext() const
{
  return context;
}

inline const NarrowingVariableInfo&
NarrowingSearchState3::getVariableInfo() const
{
  return variableInfo;
}

inline bool
NarrowingSearchState3::isIncomplete() const
{
  //
  //	Returns true if any incompleteness has been encountered so far.
  //
  return incompleteFlag;
}

inline Rule*
NarrowingSearchState3::getRule() const
{
  return module->getRules()[ruleIndex];
}

inline int
NarrowingSearchState3::getVariableFamily() const
{
  return unificationProblem->getVariableFamily();
}

inline const Substitution&
NarrowingSearchState3::getUnifier() const
{
  return unificationProblem->getSolution();
}

inline DagNode*
NarrowingSearchState3::getReplacedDag() const
{
  return getDagNode();
}

#endif
