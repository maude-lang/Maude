/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Class to search for a complete set of variants from a given dag.
//
//	It can also do variant unification, with UNIFICATION_MODE flag.
//	It can also do variant subsumption, with SUBSUMPTION_MODE flag.
//	It can also do variant MATCHING, with MATCH_MODE flag.

//
#ifndef _variantSearch_hh_
#define _variantSearch_hh_
#include <map>
#include "cacheableState.hh"
#include "simpleRootContainer.hh"
#include "narrowingVariableInfo.hh"
#include "variantFolder.hh"
#include "variantMatchingProblem.hh"

class VariantSearch : public CacheableState, private SimpleRootContainer
{
  NO_COPYING(VariantSearch);

public:
  enum Flags
    {
     UNIFICATION_MODE = 0x0001,
     IRREDUNDANT_MODE = 0x0002,
     DELETE_FRESH_VARIABLE_GENERATOR = 0x0004,
     CHECK_VARIABLE_NAMES = 0x0008,
     SUBSUMPTION_MODE = 0x0010,
     MATCH_MODE = 0x0020,
     DELETE_LAST_VARIANT_MATCHING_PROBLEM = 0x0040
    };
  //
  //	Initial dag is the root of context. This context should not go away nor should the
  //	initial dag be rewritten in place while the VariantSearch object is in existence
  //	because we rely in the variable dag nodes in the dag being protected from garbage
  //	collection. This context is deleted on destruction.
  //
  //	Blocker dags are protected from garbage collection.
  //
  //	Fresh variable generator can optionally be deleted on destruction (for use in a
  //	cached metalevel object).
  //
  VariantSearch(RewritingContext* context,
		const Vector<DagNode*>& blockerDags,
		FreshVariableGenerator* freshVariableGenerator,
		int flags = DELETE_FRESH_VARIABLE_GENERATOR | CHECK_VARIABLE_NAMES,
		int incomingVariableFamily = NONE);
  ~VariantSearch();

  bool problemOK() const;
  bool getIrredundantMode() const;
  const NarrowingVariableInfo& getVariableInfo() const;
  RewritingContext* getContext() const;
  bool isIncomplete() const;
  //
  //	Only works in SUBSUMPTION_MODE
  //
  bool isSubsumed(DagNode* target) const;
  //
  //	Only works in UNIFICATION_MODE
  //
  virtual bool findNextUnifier();
  virtual const Vector<DagNode*>& getCurrentUnifier(int& nrFreeVariables,
						    int& variableFamily);
  //
  //	Only works if not in SUBSUMPTION_MODE or UNIFICATION_MODE
  //
  const Vector<DagNode*>* getNextVariant(int& nrFreeVariables,
					 int& variableFamily,
					 int& parentIndex,
					 bool& moreInLayer);
  const Vector<DagNode*>* getLastReturnedVariant(int& nrFreeVariables,
						 int& variableFamily,
						 int& parentIndex,
						 bool& moreInLayer);

  bool findNextVariant();
  const Vector<DagNode*>& getCurrentVariant(int& nrFreeVariables,
					    int& variableFamily,
					    int* parentNumber = 0,
					    bool* moreInLayer = 0);
  //
  //	Only works in MATCH_MODE
  //
  VariantMatchingProblem* makeVariantMatchingProblem(RewritingContext* subject);
  VariantMatchingProblem* getLastVariantMatchingProblem() const;

private:
  typedef map<int, int> IntMap;
  typedef Vector<int> VariantIndexVec;

  void markReachableNodes();
  void expandLayer();
  void expandVariant(const Vector<DagNode*>& variant, int index);

#ifdef DUMP
  void dumpVariant(const Vector<DagNode*>& variant, int index,  int parentIndex);
#endif

  RewritingContext* const context;
  const Vector<DagNode*> blockerDags;
  FreshVariableGenerator* const freshVariableGenerator;
  const int flags;
  const int firstVariableFamily;
  const int secondVariableFamily;

  DagNode* targetCopy;
  bool problemOkay;
  bool incompleteFlag;
  NarrowingVariableInfo variableInfo;
  int nrVariantVariables;
  VariantFolder variantCollection;

  VariantIndexVec frontier;
  VariantIndexVec newFrontier;
  int currentIndex;
  bool useFirstVariableFamily;

  int nrVariantsFound;
  IntMap internalIndexToExternalIndex;

  Vector<DagNode*> protectedVariant;
  VariantMatchingProblem* lastVariantMatchingProblem;
};

inline bool
VariantSearch::problemOK() const
{
  return problemOkay;
}

inline bool
VariantSearch::getIrredundantMode() const
{
  return flags & IRREDUNDANT_MODE;
}

inline const NarrowingVariableInfo&
VariantSearch::getVariableInfo() const
{
  return variableInfo;
}

inline RewritingContext*
VariantSearch::getContext() const
{
  return context;
}

inline bool
VariantSearch::isIncomplete() const
{
  return incompleteFlag;
}

inline const Vector<DagNode*>&
VariantSearch::getCurrentVariant(int& nrFreeVariables,
				 int& variableFamily,
				 int* parentNumber,
				 bool* moreInLayer)
{
  const Vector<DagNode*>& variant = variantCollection.getCurrentVariant(nrFreeVariables,
									variableFamily,
									parentNumber,
									moreInLayer);
  //
  //	Need to translate parentNumber to the external numbering scheme.
  //
  if (parentNumber)
    *parentNumber = (*parentNumber == NONE) ? NONE : internalIndexToExternalIndex[*parentNumber];
  return variant;
}

inline VariantMatchingProblem*
VariantSearch::makeVariantMatchingProblem(RewritingContext* subject)
{
  lastVariantMatchingProblem = new VariantMatchingProblem(&variantCollection,
							  subject,
							  freshVariableGenerator);
  return lastVariantMatchingProblem;
}

inline VariantMatchingProblem*
VariantSearch::getLastVariantMatchingProblem() const
{
  return lastVariantMatchingProblem;
}

#endif
