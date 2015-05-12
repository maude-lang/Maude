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
//	Class to hold variable information passed down through
//	compilation functions.
//
#ifndef _variableInfo_hh_
#define _variableInfo_hh_
#include "natSet.hh"
#include "variableTerm.hh"

class VariableInfo
{
public: 
  VariableInfo();

  int getNrRealVariables() const;
  int getNrProtectedVariables() const;
  //
  //	These two functions map between indexes and real variables.
  //
  int variable2Index(VariableTerm* variableTerm);
  Term* index2Variable(int index) const;
  //
  //	The following function makes a new variable which will be
  //	(1) Cleared before matching;
  //	(2) Copied when a copy of substitution is made;
  //	(3) Considered for local binding in a given branch of matching; and
  //	(4) Protected from garbage collection.
  //
  int makeProtectedVariable();
  //
  //	Construction indexes are initially >=  MAX_NR_PROTECTED_VARIABLES.
  //	A construction index that is reused across two or more different
  //	fragments must be remapped to a protected variable.
  //	Others construction indices will be remapped to the indices at the
  //	end of the substitution and these indices can be reused for different
  //	things in different fragments since they are not protected in any way.
  //
  int makeConstructionIndex();
  void endOfFragment();
  void useIndex(int index);
  int computeIndexRemapping();
  int remapIndex(int original);
  //
  //	Separate from the above we keep sets (of indices) of real variables
  //	at occur in the condition or are used before being bound.
  //
  void addConditionVariables(const NatSet& vars);
  void addUnboundVariables(const NatSet& vars);
  const NatSet& getConditionVariables() const;
  const NatSet& getUnboundVariables() const;

  //
  //	Decide if an index is read or virtual.
  //
  static bool isReal(int index);

private:
  enum Values
  {
    MAX_NR_PROTECTED_VARIABLES = 10000000
  };

  struct ConstructionIndex
  {
    int lastUseTime;  // number of construction indices allocated at last use
    short assignedFragment;
    short lastUseFragment;
    int newIndex;
  };

  Vector<Term*> variables;
  int nrProtectedVariables;
  int fragmentNumber;
  Vector<ConstructionIndex> constructionIndices;
  NatSet conditionVariables;
  NatSet unboundVariables;
};

inline int
VariableInfo::getNrRealVariables() const
{
  return variables.length();
}

inline int
VariableInfo::getNrProtectedVariables() const
{
  return nrProtectedVariables;
}

inline Term*
VariableInfo::index2Variable(int index) const
{
  return variables[index];
}

inline int
VariableInfo::makeProtectedVariable()
{
  return nrProtectedVariables++;
}

inline void
VariableInfo::endOfFragment()
{
  ++fragmentNumber;
}

inline int
VariableInfo::remapIndex(int original)
{
  if (original >= MAX_NR_PROTECTED_VARIABLES)
    return constructionIndices[original - MAX_NR_PROTECTED_VARIABLES].newIndex;
  return original;
}

inline void
VariableInfo::useIndex(int index)
{
  if (index >= MAX_NR_PROTECTED_VARIABLES)
    {
      index -= MAX_NR_PROTECTED_VARIABLES;
      constructionIndices[index].lastUseTime = constructionIndices.length();
      constructionIndices[index].lastUseFragment = fragmentNumber;
    }
}

inline const NatSet&
VariableInfo::getConditionVariables() const
{
  return conditionVariables;
}

inline const NatSet&
VariableInfo::getUnboundVariables() const
{
  return unboundVariables;
}

inline void
VariableInfo::addConditionVariables(const NatSet& vars)
{
  conditionVariables.insert(vars);
}

inline void
VariableInfo::addUnboundVariables(const NatSet& vars)
{
  unboundVariables.insert(vars);
}

inline bool
VariableInfo::isReal(int index)
{
  return index < MAX_NR_PROTECTED_VARIABLES;
}

#endif
