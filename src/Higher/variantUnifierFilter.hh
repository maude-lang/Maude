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
//	Class for filtering variant unifiers, keeping a minimal set that cannot
//	be shown to be subsumed (variant subsumption could potentially be
//	incomplete because of incomplete unifications algorithms).
//
#ifndef _variantUnifierFilter_hh_
#define _variantUnifierFilter_hh_
#include <list>
#include "dagRoot.hh"
#include "substitution.hh"

class VariantUnifierFilter
{
  NO_COPYING(VariantUnifierFilter);

public:
  //
  //	We just use the context to make new contexts to call VariantSearch on.
  //
  VariantUnifierFilter(RewritingContext* context,
		       FreshVariableGenerator* freshVariableGenerator);
  ~VariantUnifierFilter();
  //
  //	Unifiers must have the same number of assignments, and kinds must match for
  //	the ith family. Variables in the assignments of a unifier  must be drawn from
  //	a single variable family but variable families can differ between unifers.
  //
  void insertUnifier(const Vector<DagNode*>& unifier,int nrFreeVariables, int variableFamily);
  bool findNextSurvivingUnifier();
  const Vector<DagNode*>& getCurrentSurvivingUnifier(int& nrFreeVariables, int& variableFamily);
  bool filteringIncomplete() const;

private:
  struct RetainedUnifier
  {
    RetainedUnifier(const Vector<DagNode*>& unifier,
		    int nrFreeVariables,
		    int variableFamily,
		    DagNode* encodedUnifier,
		    VariantSearch* variants);
    ~RetainedUnifier();
    bool subsumes(DagNode* otherUnifier);
    
    Vector<DagNode*> unifier;
    int nrFreeVariables;
    int variableFamily;
    DagNode* const encodedUnifier;
    VariantSearch* variants;
  };

  typedef list<RetainedUnifier*> RetainedUnifierList;

  bool subsumes(const RetainedUnifier* retainedUnifier, DagNode* otherEncodedUnifier);

  RewritingContext* const context;
  FreshVariableGenerator* const freshVariableGenerator;

  Symbol* tupleSymbol;
  DagRoot encodedUnifier;
  RetainedUnifierList mostGeneralSoFar;
  
  bool startedExtractingUnifiers;
  bool currentUnifierSubsumed;
  bool filteringIncompleteFlag;
  RetainedUnifierList::const_iterator currentUnifier;
};

inline
VariantUnifierFilter::RetainedUnifier::RetainedUnifier(const Vector<DagNode*>& unifier,
						       int nrFreeVariables,
						       int variableFamily,
						       DagNode* encodedUnifier,
						       VariantSearch* variants)
  : unifier(unifier),  // shallow copy
    nrFreeVariables(nrFreeVariables),
    variableFamily(variableFamily),
    encodedUnifier(encodedUnifier),
    variants(variants)
{
}

inline
VariantUnifierFilter::RetainedUnifier::~RetainedUnifier()
{
  delete variants;
}

inline bool
VariantUnifierFilter::RetainedUnifier::subsumes(DagNode* otherEncodedUnifier)
{
  return variants->isSubsumed(otherEncodedUnifier);
}

inline const Vector<DagNode*>&
VariantUnifierFilter::getCurrentSurvivingUnifier(int& nrFreeVariables, int& variableFamily)
{
  Assert(startedExtractingUnifiers == true, "didn't call findNextSurvivingUnifier()");
  nrFreeVariables = (*currentUnifier)->nrFreeVariables;
  variableFamily = (*currentUnifier)->variableFamily;
  return (*currentUnifier)->unifier;
}

inline bool
VariantUnifierFilter::filteringIncomplete() const
{
  return filteringIncompleteFlag;
}

#endif
