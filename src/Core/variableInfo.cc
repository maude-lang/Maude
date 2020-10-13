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
//      Implementation for class VariableInfo.
//

#include "macros.hh"
#include "vector.hh"
#include "graph.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//      core class definitions
#include "substitution.hh"
#include "variableInfo.hh"

//      variable class definitions
#include "variableDagNode.hh"
#include "freshVariableGenerator.hh"

VariableInfo::VariableInfo()
{
  nrProtectedVariables = 0;
  fragmentNumber = 0;
}

int
VariableInfo::variable2Index(VariableTerm* variable)
{
  Assert(variable != 0, "null term");
  int nrRealVariables = variables.length();
  Assert(nrRealVariables == nrProtectedVariables,
	 "can't add new real variables at this stage");
  for (int i = 0; i < nrRealVariables; ++i)
    {
      if (variable->equal(variables[i]))
	return i;
    }
  variables.append(variable);
  ++nrProtectedVariables;
  return nrRealVariables;
}

int
VariableInfo::variable2Index(VariableDagNode* variable) const
{
  Assert(variable != 0, "null dag");
  int nrRealVariables = variables.length();
  for (int i = 0; i < nrRealVariables; ++i)
    {
      if (variables[i]->equal(variable))
	return i;
    }
  return NONE;
}

int
VariableInfo::makeConstructionIndex()
{
  int nrConstructionIndices = constructionIndices.length();
  constructionIndices.expandBy(1);
  constructionIndices[nrConstructionIndices].assignedFragment = fragmentNumber;
  constructionIndices[nrConstructionIndices].lastUseFragment = fragmentNumber;
  return MAX_NR_PROTECTED_VARIABLES + nrConstructionIndices;
}

int
VariableInfo::computeIndexRemapping()
{
  int nrConstructionIndices = constructionIndices.length();
  /*
  cerr << "dumping constructionIndices\n";
  for (int i = 0; i < nrConstructionIndices; ++i)
    {
      cerr << i << '\t' <<
	constructionIndices[i].assignedFragment << '\t' <<
	constructionIndices[i].lastUseFragment << '\t' <<
	constructionIndices[i].lastUseTime << endl;
    }
  */

  //
  //	All construction indices that need to be protected between different fragments
  //	get remapped to a new protected variable.
  //
  for (int i = 0; i < nrConstructionIndices; ++i)
    {
      if (constructionIndices[i].assignedFragment != constructionIndices[i].lastUseFragment)
	constructionIndices[i].newIndex = makeProtectedVariable();
    }
  //
  //	We now build a graph of conflicts between remaining construction indices.
  //
  DebugAdvisoryCheck(nrConstructionIndices < 100, "nrConstructionIndices = " << nrConstructionIndices);
  Graph conflicts(nrConstructionIndices);
  Vector<int> conflictCandidates;
  Vector<int> nextConflictCandidates;
  for (int i = 0; i < nrConstructionIndices; ++i)
    {
      if (constructionIndices[i].assignedFragment ==
	  constructionIndices[i].lastUseFragment)
	{
	  //
	  //	A remaining construction index i conflicts with any earlier
	  //	remaining construction index j whose last use is after the
	  //	allocation of construction index i. To speed things up
	  //	when the number of construction indices is huge we keep track
	  //	a smaller pool of candidates.
	  //
	  nextConflictCandidates.clear();
	  for (int c : conflictCandidates)
	    {
	      if (constructionIndices[c].lastUseTime > i)
		{
		  conflicts.insertEdge(i, c);
		  nextConflictCandidates.append(c);
		}
	    }
	  nextConflictCandidates.append(i);
	  conflictCandidates.swap(nextConflictCandidates);
	}
    }
  //
  //	We now use graph coloring to remap the remaining construction indices.
  //
  Vector<int> coloring;
  int nrColors = conflicts.color(coloring);
  for (int i = 0; i < nrConstructionIndices; ++i)
    {
      if (constructionIndices[i].assignedFragment ==
	  constructionIndices[i].lastUseFragment)
	constructionIndices[i].newIndex = nrProtectedVariables + coloring[i];
    }
  //
  //	Finally, we need return the minimum size of substitution needed.
  //
  return nrProtectedVariables + nrColors;
  /*
  DebugAdvisory("nrProtectedVariables = " << nrProtectedVariables <<
		"\tnrColors = " << nrColors);
  */
}

Term*
VariableInfo::variableNameConflict(FreshVariableGenerator& variableSource)
{
  for (Term* t : variables)
    {
      VariableTerm* v = safeCastNonNull<VariableTerm*>(t);
      if (variableSource.variableNameConflict(v->id()))
	return v;
    }
  return 0;
}
