/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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
//	Class to record what parts of an ACU DAG node were not matched.
//
#ifndef _ACU_ExtensionInfo_hh_
#define _ACU_ExtensionInfo_hh_
#include "extensionInfo.hh"
#include "dagRoot.hh"

class ACU_ExtensionInfo : public ExtensionInfo
{
public:
  ACU_ExtensionInfo(ACU_BaseDagNode* subject);

  DagNode* buildMatchedPortion() const;
  ExtensionInfo* makeClone() const;
  void copy(ExtensionInfo* extensionInfo);
  //
  //	Stuff specific to ACU_ExtensionInfo.
  //
  DagNode* buildUnmatchedPortion() const;
  //
  //	We (sometimes) keep track of the maximum amount of
  //	stuff we can put in the extension and still match
  //	two arguments.
  //
  void setUpperBound(int multiplicity);
  int getUpperBound() const;
  //
  //	Member functions for tracking the stuff in the extension
  //	as a dag node.
  //
  void setUnmatched(DagNode* unmatched);
  DagNode* getUnmatched() const;
  //
  //	Member functions for tracking the stuff in the
  //	extension as a vector of unmatched multiplicities.
  //
  void clear();  // setup vector and clear it
  void reset();  // setup vector without clearing it
  void setUnmatched(int argIndex, int multiplicity);

private:
  ACU_BaseDagNode* subject;
  DagRoot unmatched;
  Vector<int> unmatchedMultiplicity;
  int upperBound;
};

inline
ACU_ExtensionInfo::ACU_ExtensionInfo(ACU_BaseDagNode* subject)
  : subject(subject)
{
}

inline void
ACU_ExtensionInfo::setUpperBound(int multiplicity)
{
  upperBound = multiplicity;
}

inline int
ACU_ExtensionInfo::getUpperBound() const
{
  return upperBound;
}

inline void
ACU_ExtensionInfo::setUnmatched(DagNode* dagNode)
{
  unmatched.setNode(dagNode);
}

inline DagNode*
ACU_ExtensionInfo::getUnmatched() const
{
  return unmatched.getNode();
}

inline void
ACU_ExtensionInfo::clear()
{
  unmatched.setNode(0);
  int nrArgs = subject->getSize();
  unmatchedMultiplicity.resize(nrArgs);
  for (int i = 0; i < nrArgs; i++)
    unmatchedMultiplicity[i] = 0;
}

inline void
ACU_ExtensionInfo::reset()
{
  unmatched.setNode(0);
  unmatchedMultiplicity.resize(subject->getSize());
}

inline void
ACU_ExtensionInfo::setUnmatched(int argIndex, int multiplicity)
{
  unmatchedMultiplicity[argIndex] = multiplicity;
}

#endif
