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
//	Class for folding variants. We keep a collection of variants that are most general so far and test
//	each inserted variant so that caller knows whether to keep exanding the variant.
//
//	To simpify the code we treat a variant as a vector of dags; we don't distinguish between the term and
//	the substitution since both will be checked for subsumption.
//
//	The caller gives each variant an index, and for each new variant gives the index of the parent variant or NONE if
//	it is the root variant search. A variants index is required to be larger than that of its parent.
//	When an existing variant is ejected by the arrival of a new variant that is more general, all of its descendents are
//	likewise ejected, just as if the search that has already taken place had been pruned.
//
#ifndef _variantFolder_hh_
#define _variantFolder_hh_
#include <map>
#include "simpleRootContainer.hh"

class VariantFolder : private SimpleRootContainer
{
  NO_COPYING(VariantFolder);

public:
  VariantFolder();
  ~VariantFolder();

  bool insertVariant(const Vector<DagNode*>& variant, int index, int parentIndex);
  //
  //	Returns 0 if variant wasn't retained or was later purged.
  //
  const Vector<DagNode*>* getVariant(int index) const;

  const Vector<DagNode*>* getNextSurvivingVariant(int& nrFreeVariables);

private:
  struct RetainedVariant
  {
    RetainedVariant(const Vector<DagNode*> original);
    ~RetainedVariant();
    void dump();
  
    int nrVariables;  // number of variables needed for matching; includes any abstraction variables
    int nrFreeVariables;  // number of variables occuring in variant
    int parentIndex;
    Vector<DagNode*> variant;
    Vector<Term*> terms;
    Vector<LhsAutomaton*> matchingAutomata;
  };

  typedef map<int, RetainedVariant*> RetainedVariantMap;

  void markReachableNodes();
  bool subsumes(const RetainedVariant* retainedVariant, const Vector<DagNode*>& variant);

  RetainedVariantMap mostGeneralSoFar;
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

#endif
