/*

    This file is part of the Maude 3 interpreter.

    Copyright 2012-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for folding variants. We keep a collection of variants that are most general
//	so far and test each inserted variant so that caller knows whether to keep exanding
//	the variant.
//
//	To simpify the code we treat a variant as a vector of dags; we don't distinguish
//	between the term and the substitution since both will be checked for subsumption.
//
//	The caller gives each variant an index, and for each new variant gives the index
//	of the parent variant or NONE if it is the root variant search. A variants index
//	is required to be larger than that of its parent.
//	When an existing variant is ejected by the arrival of a new variant that is more
//	general, all of its descendents are likewise ejected, just as if the search that
//	has already taken place had been pruned.
//
#ifndef _variantFolder_hh_
#define _variantFolder_hh_
#include <map>
#include "simpleRootContainer.hh"
#include "variableInfo.hh"

class VariantFolder : private SimpleRootContainer
{
  NO_COPYING(VariantFolder);

public:
  VariantFolder();
  ~VariantFolder();

  bool insertVariant(const Vector<DagNode*>& variant, int index, int parentIndex, int variableFamily);

  //
  //	Returns the internal index of the next surviving variant or
  //	NONE if there are no more.
  //
  int findNextSurvivingVariant();
  //
  //	Get the details of the variant found by findNextSurvivingVariant()
  //	Can only be safely called after findNextSurvivingVariant() has return
  //	an index other than NONE and before an intervening call to insertVariant()
  //	has taken place (which could purge it). parentNumber and moreInLayer can
  //	be passed null pointers if this information isn't wanted.
  //
  const Vector<DagNode*>& getCurrentVariant(int& nrFreeVariables,
					    int& variableFamily,
					    int* parentNumber = 0,
					    bool* moreInLayer = 0);
  //
  //	Same requirements as above; just returns the variant size (so caller can
  //	determine if it is a variant unifier).
  //
  int getCurrentVariantSize();
  
  //
  //	Returns 0 if variant wasn't retained or was later purged.
  //
  const Vector<DagNode*>* getVariant(int index) const;
  //
  //	Check if variant is subsumed by existing variants without inserting it
  //	or using it to evict older variants.
  //
  bool isSubsumed(const Vector<DagNode*>& variant) const;
  //
  //	For variant matching.
  //
  //	This is needed in order to make the indices in the variant substitution
  //	dags agree with those used by the matching automata.
  //
  void prepareForVariantMatching();
  //
  //	We start looking at variants with index > indexOfLastUsedVariant
  //	(could be -1 to look at the first variant), to find a potential match
  //	against subject. On success, indexOfLastUsedVariant is updated to the
  //	index of the variant we used, and a (partial) match and unresolved
  //	matching subproblem (could be null) is returned, along with a pointer
  //	to the variant and a pointer to the internal VariableInfo object.
  //	Responsibility for deleting matcher and subproblem is passed to the caller.
  //	On failure a null pointer is returned, matcher is set to
  //	0 and subproblem and variableInfo are untouched.
  //
  const Vector<DagNode*>* findNextVariantThatMatches(int& indexOfLastUsedVariant,
						     DagNode* subject,
						     const VariableInfo*& variableInfo,
						     RewritingContext*& matcher,
						     Subproblem*& subproblem);

private:
  struct RetainedVariant
  {
    RetainedVariant(const Vector<DagNode*> original);
    ~RetainedVariant();
    void dump();

    VariableInfo variableInfo;
    int nrVariables;  // number of variables needed for matching; includes any abstraction variables
    int nrFreeVariables;  // number of variables occuring in variant
    int variableFamily;
    int parentIndex;
    Vector<DagNode*> variant;
    Vector<Term*> terms;
    Vector<LhsAutomaton*> matchingAutomata;
    int layerNumber;
  };

  typedef map<int, RetainedVariant*> RetainedVariantMap;

  void markReachableNodes();
  bool subsumes(const RetainedVariant* retainedVariant,
		const Vector<DagNode*>& variant) const;

  RetainedVariantMap mostGeneralSoFar;
  RetainedVariantMap::const_iterator currentVariant;
  int currentVariantIndex;
};

inline const Vector<DagNode*>*
VariantFolder::getVariant(int index) const
{
  RetainedVariantMap::const_iterator i = mostGeneralSoFar.find(index);
  if (i == mostGeneralSoFar.end())
    return 0;  // purged
  return &(i->second->variant);
}

inline int
VariantFolder::getCurrentVariantSize()
{
  Assert(mostGeneralSoFar.find(currentVariantIndex) == currentVariant, "current variant purged");

  return currentVariant->second->variant.size();
}

#endif
