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
//	Code for expanding selections of variables to set to empty.
//

WordLevel::ResultPair
WordLevel::trySelection()
{
  int nrOriginalVariables = partialSolution.size();
  for (int i = 0; i < nrOriginalVariables; ++i)
    {
      Word& binding = partialSolution[i];
      if (constraintMap[i].canTakeEmpty() && binding.size() == 1 && binding[0] == i)
	{
	  DebugInfo("x" << i << " can take empty and is free");
	  idVariables.append(i);
	}
    }
  int nrIdVariables = idVariables.size();
  nrSelections = (1 << nrIdVariables) - 1;
  DebugInfo("idVariables.size() = " << nrIdVariables <<
	    "  nrSelections = " << nrSelections);
  return (nrIdVariables == 0) ? ResultPair(FAILURE, 0) : exploreSelections();
}

WordLevel::ResultPair
WordLevel::exploreSelections()
{
  ++selection;
  if (selection > nrSelections)
    return ResultPair(FAILURE, 0);
  DebugInfo("-------- Trying selection " << selection << " --------");
  //
  //	Count the number of equations that survived simplification.
  //
  int equationCount = 0;
  for (const Equation& i : unsolvedEquations)
    {
      if (!(i.lhs.empty()))
	++equationCount;
    }
  //
  //	Make a selection WordLevel.
  //
  WordLevel* newLevel = new WordLevel(SELECTION,
				      partialSolution.size(),
				      equationCount,
				      identityOptimizations,
				      this);
  newLevel->constraintMap = constraintMap;  // deep copy
  {
    int index = 0;
    int bitMask = 1;
    Word emptyWord;
    int nrIdVariables = idVariables.size();
    int nrVariables = partialSolution.size();
    for (int i = 0; i < nrVariables; ++i)
      {
	if (index < nrIdVariables && i == idVariables[index])
	  {
	    int oldBitMask = bitMask;
	    ++index;
	    bitMask <<= 1;
	    if (selection & oldBitMask)
	      {
		//
		//	We're going to null variable i because it is part of
		//	our selection.
		//
		DebugInfo("setting x" << i << " to null");
		newLevel->addAssignment(i,emptyWord);
		continue;
	      }
	  }
	newLevel->addAssignment(i, partialSolution[i]);
      }
  }
  {
    int equationIndex = 0;
    int nrEquations = unsolvedEquations.size();
    for (int i = 0; i < nrEquations; ++i)
      {
	Equation& e = unsolvedEquations[i];
	if (!(e.lhs.empty()))
	  {
	    newLevel->addEquation(equationIndex, e.lhs, e.rhs);
	    ++equationIndex;
	  }
      }
  }
  //
  //	Creating a selection level cannot introduce incompleteness itself.
  //
  return ResultPair(SUCCESS, newLevel);
}

bool
WordLevel::insertCombination(const Subst& substitution)
{
  Assert(levelType == INITIAL, "levelType = " << levelType);
  int code = 0;
  int bitMask = 1;
  int index = 0;
  int nrIdVariables = idVariables.size();
  int nrOriginalVariables = partialSolution.size();
  for (int i = 0; i < nrOriginalVariables; ++i)
    {
      if (index < nrIdVariables && i == idVariables[index])
	{
	  //
	  //	One of the variables we are selecting over.
	  //
	  if (substitution[i].empty())
	    code |= bitMask;
	  ++index;
	  bitMask <<= 1;
	}
    }
  return finalCombinations.insert(code).second;
}

