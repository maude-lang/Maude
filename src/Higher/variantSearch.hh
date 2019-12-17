/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2016 SRI International, Menlo Park, CA 94025, USA.

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
//	It can also do variant unification, if unificationMode == true.
//
#ifndef _variantSearch_hh_
#define _variantSearch_hh_
#include <map>
#include "cacheableState.hh"
#include "simpleRootContainer.hh"
#include "narrowingVariableInfo.hh"
#include "variantFolder.hh"

class VariantSearch : public CacheableState, private SimpleRootContainer
{
  NO_COPYING(VariantSearch);

public:
  //
  //	Initial dag is the root of context. This context should not go away nor should the initial dag be
  //	rewritten in place while the VariantSearch object is in existence because we rely in the variable dag nodes
  //	in the dag being protected from garbage collection. This context is deleted on destruction.
  //
  //	Blocker dags are protected from garbage collection.
  //
  //	Fresh variable generator can optionally be deleted on destruction (for use in a cached metalevel object).
  //
  VariantSearch(RewritingContext* context,
		const Vector<DagNode*>& blockerDags,
		FreshVariableGenerator* freshVariableGenerator,
		bool unificationMode,
		bool irredundantMode,
		bool deleteFreshVariableGenerator = true,
		int incomingVariableFamily = NONE,
		bool checkVariableNames = true);
  ~VariantSearch();

  bool getIrredundantMode() const;
  const NarrowingVariableInfo& getVariableInfo() const;
  const Vector<DagNode*>* getNextVariant(int& nrFreeVariables, int& variableFamily, int& parentIndex, bool& moreInLayer);
  const Vector<DagNode*>* getNextUnifier(int& nrFreeVariables, int& variableFamily);
  RewritingContext* getContext() const;
  bool isIncomplete() const;
  //
  //	Returns the last variant returned by getNextVariant().
  //
  const Vector<DagNode*>* getLastReturnedVariant(int& nrFreeVariables, int& variableFamily, int& parentIndex, bool& moreInLayer);
  //
  //	Returns the last unifier returned by getNextUnifier().
  //
  const Vector<DagNode*>* getLastReturnedUnifier(int& nrFreeVariables, int& variableFamily);

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
  const bool unificationMode;
  const bool irredundantMode;
  const bool deleteFreshVariableGenerator;
  const int firstVariableFamily;
  const int secondVariableFamily;

  DagNode* targetCopy;
  bool incompleteFlag;
  NarrowingVariableInfo variableInfo;
  int nrVariantVariables;
  VariantFolder variantCollection;

  VariantIndexVec frontier;
  VariantIndexVec newFrontier;
  int currentIndex;
  bool useFirstVariableFamily;

  int nrVariantsReturned;
  IntMap internalIndexToExternalIndex;

  Vector<DagNode*> protectedVariant;
};

inline bool
VariantSearch::getIrredundantMode() const
{
  return irredundantMode;
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

inline const Vector<DagNode*>*
VariantSearch::getLastReturnedUnifier(int& nrFreeVariables, int& variableFamily)
{
  return variantCollection.getLastReturnedVariant(nrFreeVariables, variableFamily);
}

inline bool
VariantSearch::isIncomplete() const
{
  return incompleteFlag;
}

#endif
