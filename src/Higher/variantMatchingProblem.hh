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
//	Class for variant matching, given that a complete set of folded
//	variants for the pattern has already been computed.
//
#ifndef _variantMatchingProblem_hh_
#define _variantMatchingProblem_hh_
#include <set>
#include "simpleRootContainer.hh"

class VariantMatchingProblem : private SimpleRootContainer
{
public:
  //
  //	VariantMatchingProblem takes responsibility for deleting subject
  //	on destruction.
  //
  VariantMatchingProblem(VariantFolder* pattern,
			 RewritingContext* subject,
			 FreshVariableGenerator* freshVariableGenerator);
  ~VariantMatchingProblem();
  
  bool findNextMatcher();
  const Vector<DagNode*>& getCurrentMatcher() const;
  bool freshVariablesNeeded() const;

private:
  void markReachableNodes();
  void checkVariablesInSubject();
  
  VariantFolder* const pattern;
  RewritingContext* const subject;
  FreshVariableGenerator* const freshVariableGenerator;
  
  set<int> indicesToAvoid;
  bool checkedVariablesInSubject;
  int indexOfLastUsedVariant;
  RewritingContext* matcher;
  Subproblem* subproblem;
  const Vector<DagNode*>* currentVariant;
  Vector<DagNode*> currentMatcher;
};

inline const Vector<DagNode*>&
VariantMatchingProblem::getCurrentMatcher() const
{
  return currentMatcher;
}

inline bool
VariantMatchingProblem::freshVariablesNeeded() const
{
  return checkedVariablesInSubject;
}

#endif
